/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once

#include "execution_defs.h"
#include "execution_manager.h"
#include "executor_abstract.h"
#include "index/ix.h"
#include "system/sm.h"

class IndexScanExecutor : public AbstractExecutor {
   private:
    std::string tab_name_;              // 表名称
    TabMeta tab_;                       // 表的元数据
    std::vector<Condition> conds_;      // 扫描条件
    RmFileHandle* fh_;                  // 表的数据文件句柄
    std::vector<ColMeta> cols_;         // 需要读取的字段
    size_t len_;                        // 选取出来的一条记录的长度
    std::vector<Condition> fed_conds_;  // 扫描条件，和conds_字段相同
    std::vector<std::string>
        index_col_names_;   // index scan涉及到的索引包含的字段
    IndexMeta index_meta_;  // index scan涉及到的索引元数据
    Rid rid_;
    std::unique_ptr<IxScan> scan_;
    SmManager* sm_manager_;

   public:
    IndexScanExecutor(SmManager* sm_manager,
                      std::string tab_name,
                      std::vector<Condition> conds,
                      std::vector<std::string> index_col_names,
                      Context* context) {
        sm_manager_ = sm_manager;
        context_ = context;
        tab_name_ = std::move(tab_name);
        tab_ = sm_manager_->db_.get_table(tab_name_);
        conds_ = std::move(conds);
        index_col_names_ = index_col_names;
        index_meta_ = *(tab_.get_index_meta(index_col_names_));
        fh_ = sm_manager_->fhs_.at(tab_name_).get();
        cols_ = tab_.cols;
        len_ = cols_.back().offset + cols_.back().len;
        std::map<CompOp, CompOp> swap_op = {
            {OP_EQ, OP_EQ},
            {OP_NE, OP_NE},
            {OP_LT, OP_GT},
            {OP_GT, OP_LT},
            {OP_LE, OP_GE},
            {OP_GE, OP_LE},
        };

        for (auto& cond : conds_) {
            if (cond.lhs_col.tab_name != tab_name_) {
                // lhs is on other table, now rhs must be on this table
                assert(!cond.is_rhs_val && cond.rhs_col.tab_name == tab_name_);
                // swap lhs and rhs
                std::swap(cond.lhs_col, cond.rhs_col);
                cond.op = swap_op.at(cond.op);
            }
        }
        fed_conds_ = conds_;
    }

    void fill_up_info(std::string col_name, char* up_key, int offset, int len) {
        switch (tab_.get_col(col_name)->type) {
            case TYPE_INT:
                memcpy(up_key + offset, &INT_MAX, len);
                break;
            case TYPE_BIG_INT:
                memcpy(up_key + offset, &BIG_INT_MAX, len);
                break;
            case TYPE_FLOAT:
                memcpy(up_key + offset, &FLOAT_MAX, len);
                break;
            case TYPE_STRING:
                memset(up_key + offset, 255, len);
                break;
            case TYPE_DATETIME:
                memcpy(up_key + offset, &DATETIME_MAX, len);
                break;
        }
    }

    void fill_low_info(std::string col_name,
                       char* low_key,
                       int offset,
                       int len) {
        switch (tab_.get_col(col_name)->type) {
            case TYPE_INT:
                memcpy(low_key + offset, &INT_MIN, len);
                break;
            case TYPE_BIG_INT:
                memcpy(low_key + offset, &BIG_INT_MIN, len);
                break;
            case TYPE_FLOAT:
                memcpy(low_key + offset, &FLOAT_MIN, len);
                break;
            case TYPE_STRING:
                memset(low_key + offset, 0, len);
                break;
            case TYPE_DATETIME:
                memcpy(low_key + offset, &DATETIME_MIN, len);
                break;
        }
    }

    bool get_key_info(std::string col_name,
                      char* low_key,
                      char* up_key,
                      int offset,
                      int len) {
        int re = 0;
        for (auto& cond : fed_conds_) {
            if (cond.lhs_col.col_name == col_name && cond.op == OP_EQ &&
                cond.is_rhs_val) {
                memcpy(up_key + offset, cond.rhs_val.raw->data, len);
                memcpy(low_key + offset, cond.rhs_val.raw->data, len);
                return true;
            } else if (cond.lhs_col.col_name == col_name && cond.op == OP_LT &&
                       cond.is_rhs_val) {
                memcpy(up_key + offset, cond.rhs_val.raw->data, len);
                re += 2;
            } else if (cond.lhs_col.col_name == col_name && cond.op == OP_GT &&
                       cond.is_rhs_val) {
                memcpy(low_key + offset, cond.rhs_val.raw->data, len);
                re += 1;
            } else if (cond.lhs_col.col_name == col_name && cond.op == OP_LE &&
                       cond.is_rhs_val) {
                memcpy(up_key + offset, cond.rhs_val.raw->data, len);
                re += 2;
            } else if (cond.lhs_col.col_name == col_name && cond.op == OP_GE &&
                       cond.is_rhs_val) {
                memcpy(low_key + offset, cond.rhs_val.raw->data, len);
                re += 1;
            }
        }

        if (re == 0) {
            fill_low_info(col_name, low_key, offset, len);
            fill_up_info(col_name, up_key, offset, len);
        } else if (re == 1) {
            fill_up_info(col_name, up_key, offset, len);
        } else if (re == 2) {
            fill_low_info(col_name, low_key, offset, len);
        }
        return false;
    }
    // 开始
    void beginTuple() override {
        check_runtime_conds();

        auto ih = sm_manager_->ihs_
                      .at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index_col_names_))
                      .get();
        char low_key[index_meta_.col_tot_len];
        char up_key[index_meta_.col_tot_len];
        int offset = 0;

        // Fill low_key and up_key with initial values
        for (int j = 0; j < index_meta_.col_num; ++j) {
            if (!get_key_info(index_meta_.cols[j].name, low_key, up_key, offset, index_meta_.cols[j].len)) {
                for (; j < index_meta_.col_num; ++j) {
                    fill_up_info(index_meta_.cols[j].name, up_key, offset, index_meta_.cols[j].len);
                    fill_low_info(index_meta_.cols[j].name, low_key, offset, index_meta_.cols[j].len);
                    offset += index_meta_.cols[j].len;
                }
                break;
            }
            offset += index_meta_.cols[j].len;
        }

        // Ensure up_key > low_key
        offset = 0;
        int res = 0;
        for (const auto& col : index_meta_.cols) {
            res = ix_compare(low_key + offset, up_key + offset, col.type, col.len);
            if (res != 0) {
                break;
            }
            offset += col.len;
        }

        if (res > 0) {
            scan_ = std::make_unique<IxScan>(ih, ih->leaf_end(), ih->leaf_end(), sm_manager_->get_bpm());
        } else {
            scan_ = std::make_unique<IxScan>(ih, ih->lower_bound(low_key), ih->upper_bound(up_key), sm_manager_->get_bpm());
        }

        // Get the first matching record
        while (!scan_->is_end()) {
            rid_ = scan_->rid();
            auto rec = fh_->get_record(rid_, context_);
            if (eval_conds(cols_, fed_conds_, rec.get())) {
                context_->lock_mgr_->lock_shared_on_record(context_->txn_, rid_, fh_->GetFd());
                break;
            }
            scan_->next();
        }
    }

    // 下一个元组
    void nextTuple() override {
        check_runtime_conds();
        scan_->next();
        while (!scan_->is_end()) {
            rid_ = scan_->rid();
            auto rec = fh_->get_record(rid_, context_);
            // 利用eval_conds判断是否当前记录(rec.get())满足谓词条件
            if (eval_conds(cols_, fed_conds_, rec.get())) {
                context_->lock_mgr_->lock_shared_on_record(context_->txn_, rid_,
                                                           fh_->GetFd());
                break;
            }
            scan_->next();  // 找下一个有record的位置
        }
    }
    // 是否结束
    bool is_end() const override { return scan_->is_end(); }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta>& cols() const override { return cols_; }
    // 返回下一个元组
    std::unique_ptr<RmRecord> Next() override {
        if (scan_->is_end())
            return nullptr;
        else
            return fh_->get_record(rid_, context_);
    }

    void feed(const std::map<TabCol, Value>& feed_dict) override {
        fed_conds_ = conds_;
        for (auto& cond : fed_conds_) {
            if (!cond.is_rhs_val && cond.rhs_col.tab_name != tab_name_) {
                cond.is_rhs_val = true;
                cond.rhs_val = feed_dict.at(cond.rhs_col);
            }
        }
        check_runtime_conds();
    }

    Rid& rid() override { return rid_; }

    ColMeta get_col_offset(const TabCol& target) {
        for (auto col : cols_) {
            if (target.col_name == col.name) {
                return col;
            }
        }
        return ColMeta();
    };

    void check_runtime_conds() {
        for (auto& cond : fed_conds_) {
            assert(cond.lhs_col.tab_name == tab_name_);
            if (!cond.is_rhs_val) {
                assert(cond.rhs_col.tab_name == tab_name_);
            }
        }
    }

    bool eval_cond(const std::vector<ColMeta>& rec_cols,
                   const Condition& cond,
                   const RmRecord* rec) {
        auto lhs_col = get_col(rec_cols, cond.lhs_col);
        char* lhs = rec->data + lhs_col->offset;
        char* rhs;
        ColType rhs_type;
        if (cond.is_rhs_val) {
            // value
            rhs_type = cond.rhs_val.type;
            rhs = cond.rhs_val.raw->data;
        } else {
            // column
            auto rhs_col = get_col(rec_cols, cond.rhs_col);
            rhs_type = rhs_col->type;
            rhs = rec->data + rhs_col->offset;
        }
        assert(rhs_type == lhs_col->type);
        int cmp = ix_compare(lhs, rhs, rhs_type, lhs_col->len);
        if (cond.op == OP_EQ) {
            return cmp == 0;
        } else if (cond.op == OP_NE) {
            return cmp != 0;
        } else if (cond.op == OP_LT) {
            return cmp < 0;
        } else if (cond.op == OP_GT) {
            return cmp > 0;
        } else if (cond.op == OP_LE) {
            return cmp <= 0;
        } else if (cond.op == OP_GE) {
            return cmp >= 0;
        } else {
            throw InternalError("Unexpected op type");
        }
    }

    bool eval_conds(const std::vector<ColMeta>& rec_cols,
                    const std::vector<Condition>& conds,
                    const RmRecord* rec) {
        return std::all_of(conds.begin(), conds.end(),
                           [&](const Condition& cond) {
                               return eval_cond(rec_cols, cond, rec);
                           });
    }
};