/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
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

class UpdateExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;
    std::vector<Condition> conds_;
    RmFileHandle* fh_;
    std::vector<Rid> rids_;
    std::string tab_name_;
    std::vector<SetClause> set_clauses_;
    SmManager* sm_manager_;

   public:
    UpdateExecutor(SmManager* sm_manager, const std::string& tab_name, std::vector<SetClause> set_clauses, std::vector<Condition> conds, std::vector<Rid> rids, Context* context) {
        sm_manager_ = sm_manager;
        tab_name_ = tab_name;
        set_clauses_ = set_clauses;
        tab_ = sm_manager_->db_.get_table(tab_name);
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        conds_ = conds;
        rids_ = rids;
        context_ = context;
    }

    std::unique_ptr<RmRecord> Next() override {
        // 检查类型
        for (auto& set_clause : set_clauses_) {
            auto lhs_col = tab_.get_col(set_clause.lhs.col_name);
            if (lhs_col->type == TYPE_BIG_INT && set_clause.rhs.type == TYPE_INT) {
                set_clause.rhs.type = TYPE_BIG_INT;
                set_clause.rhs.big_int_val = set_clause.rhs.int_val;
            } else if (lhs_col->type == TYPE_FLOAT && set_clause.rhs.type == TYPE_INT) {
                set_clause.rhs.type = TYPE_FLOAT;
                set_clause.rhs.float_val = set_clause.rhs.int_val;
            } else if (lhs_col->type == TYPE_STRING && set_clause.rhs.type == TYPE_DATETIME) {
                set_clause.rhs.type = TYPE_STRING;
                set_clause.rhs.str_val = set_clause.rhs.datetime_val;
            } else if (lhs_col->type != set_clause.rhs.type) {
                throw IncompatibleTypeError(coltype2str(lhs_col->type), coltype2str(set_clause.rhs.type));
            }

            if (!set_clause.flag) {
                set_clause.rhs.init_raw(lhs_col->len);
            }
        }

        for (auto& rid : rids_) {
            // 获取源数据
            std::unique_ptr<RmRecord> rec = fh_->get_record(rid, context_);
            RmRecord old_value = *rec;

            // 构造新数据
            for (size_t i = 0; i < set_clauses_.size(); i++) {
                auto col = tab_.get_col(set_clauses_[i].lhs.col_name);
                if (set_clauses_[i].flag) {
                    std::shared_ptr<RmRecord> raw = std::make_shared<RmRecord>(col->len);
                    if (set_clauses_[i].rhs.type == TYPE_INT) {
                        *(int*)(raw->data) = *reinterpret_cast<int*>(rec->data + col->offset) + set_clauses_[i].rhs.int_val;
                    } else if (set_clauses_[i].rhs.type == TYPE_FLOAT) {
                        *(float*)(raw->data) = *reinterpret_cast<float*>(rec->data + col->offset) + set_clauses_[i].rhs.float_val;
                    }
                    memcpy(rec->data + col->offset, raw->data, col->len);
                } else {
                    memcpy(rec->data + col->offset, set_clauses_[i].rhs.raw->data, col->len);
                }
            }
            RmRecord new_value = *rec;

            // 唯一性检查
            for (auto& index : tab_.indexes) {
                char old_key[index.col_tot_len], new_key[index.col_tot_len];
                index.get_key(&old_value, old_key);
                index.get_key(&new_value, new_key);
                if (memcmp(old_key, new_key, index.col_tot_len) != 0) {  // 只有当索引值发生变化时才进行检查
                    auto& ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols));
                    if (ih->contains(new_key, context_->txn_))
                        throw UniqueConstraintError();
                }
            }

            // 更新索引
            for (auto& index : tab_.indexes) {
                char old_key[index.col_tot_len], new_key[index.col_tot_len];
                index.get_key(&old_value, old_key);
                index.get_key(&new_value, new_key);
                if (memcmp(old_key, new_key, index.col_tot_len) != 0) {
                    auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
                    ih->delete_entry(old_key, context_->txn_);
                    ih->insert_entry(new_key, rid, context_->txn_);
                }
            }

            // 更新数据
            fh_->update_record(rid, rec->data, context_);

            // 记录事务
            // if(!trick)
            auto write_record = WriteRecord(WType::UPDATE_TUPLE, tab_name_, rid, old_value);
            context_->txn_->append_write_record(write_record);

            // 记录日志
            UpdateLogRecord log_record(context_->txn_->get_transaction_id(), rid, old_value, new_value, tab_name_);
            context_->log_mgr_->add_log_to_buffer(&log_record);
        }
        return nullptr;
    }

    Rid& rid() override { return _abstract_rid; }
};