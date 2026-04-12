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

class NestedLoopJoinExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> left_;
    std::unique_ptr<AbstractExecutor> right_;
    size_t len_;
    std::vector<ColMeta> cols_;

    std::vector<Condition> fed_conds_;
    bool isend;

    std::map<TabCol, Value> prev_feed_dict_;

    std::vector<std::unique_ptr<RmRecord>> left_buffer_;
    size_t left_idx_;
    std::unique_ptr<RmRecord> cur_right_record;

    std::vector<ColMeta> conds_col_type;
    
    JoinType join_type_;
    std::vector<bool> left_matched_;
    std::unique_ptr<RmRecord> null_right_record_;
    bool emitting_null_records_;
    size_t null_emit_idx_;

   public:
    NestedLoopJoinExecutor(std::unique_ptr<AbstractExecutor> left, std::unique_ptr<AbstractExecutor> right, 
                           std::vector<Condition> conds, JoinType join_type = INNER_JOIN) {
        left_ = std::move(left);
        right_ = std::move(right);
        len_ = left_->tupleLen() + right_->tupleLen();
        cols_ = left_->cols();
        auto right_cols = right_->cols();
        for (auto& col : right_cols) {
            col.offset += left_->tupleLen();
        }
        cols_.insert(cols_.end(), right_cols.begin(), right_cols.end());
        isend = false;
        fed_conds_ = std::move(conds);
        join_type_ = join_type;
        emitting_null_records_ = false;
        null_emit_idx_ = 0;

        for (auto cond : fed_conds_) {
            if (!cond.is_rhs_val && cond.rhs_col.tab_name != left_->get_tab_name()) {
                for (auto col : right_->cols()) {
                    if (cond.rhs_col.col_name == col.name) {
                        conds_col_type.push_back(col);
                        break;
                    }
                }
            }
        }
        
        null_right_record_ = std::make_unique<RmRecord>(right_->tupleLen());
        memset(null_right_record_->data, 0, right_->tupleLen());
    }

    void beginTuple() override {
        left_->beginTuple();
        if (left_->is_end()) {
            return;
        }
        
        left_buffer_.clear();
        left_matched_.clear();
        while (!left_->is_end()) {
            left_buffer_.push_back(std::move(left_->Next()));
            left_matched_.push_back(false);
            left_->nextTuple();
        }
        left_idx_ = 0;
        
        right_->beginTuple();
        if (right_->is_end()) {
            if (join_type_ == LEFT_JOIN && !left_buffer_.empty()) {
                emitting_null_records_ = true;
                null_emit_idx_ = 0;
            }
            return;
        }
        cur_right_record = right_->Next();
    }

    void nextTuple() override {
        if (emitting_null_records_) {
            null_emit_idx_++;
            return;
        }
        
        while (true) {
            left_idx_++;
            
            if (left_idx_ >= left_buffer_.size()) {
                right_->nextTuple();
                
                if (right_->is_end()) {
                    if (join_type_ == LEFT_JOIN) {
                        emitting_null_records_ = true;
                        null_emit_idx_ = 0;
                    }
                    return;
                }
                
                cur_right_record = right_->Next();
                left_idx_ = 0;
            }
            
            auto left_record = left_buffer_[left_idx_].get();
            auto right_record = cur_right_record.get();
            
            if (fed_conds_.empty() || check_join_conds(left_record, right_record)) {
                left_matched_[left_idx_] = true;
                return;
            }
        }
    }

    std::unique_ptr<RmRecord> Next() override {
        if (emitting_null_records_) {
            while (null_emit_idx_ < left_buffer_.size()) {
                if (!left_matched_[null_emit_idx_]) {
                    auto left_record = left_buffer_[null_emit_idx_].get();
                    auto rec = std::make_unique<RmRecord>(len_);
                    memcpy(rec->data, left_record->data, left_record->size);
                    memcpy(rec->data + left_record->size, null_right_record_->data, null_right_record_->size);
                    return rec;
                }
                null_emit_idx_++;
            }
            return nullptr;
        }
        
        if (left_idx_ < left_buffer_.size()) {
            auto left_record = left_buffer_[left_idx_].get();
            auto right_record = cur_right_record.get();
            auto rec = std::make_unique<RmRecord>(len_);
            memcpy(rec->data, left_record->data, left_record->size);
            memcpy(rec->data + left_record->size, right_record->data, right_record->size);
            return rec;
        }
        return nullptr;
    }

    void feed(const std::map<TabCol, Value>& feed_dict) override {
        prev_feed_dict_ = feed_dict;
        right_->feed(feed_dict);
    }

    void feed_left() {
        auto right_dict = rec2dict(right_->cols(), right_->Next().get());
        auto feed_dict = prev_feed_dict_;

        feed_dict.insert(right_dict.begin(), right_dict.end());
        left_->feed(feed_dict);
    }

    size_t tupleLen() const override { return len_; }

    const std::vector<ColMeta>& cols() const override { return cols_; }

    bool is_end() const override {
        if (emitting_null_records_) {
            return null_emit_idx_ >= left_buffer_.size();
        }
        return right_->is_end() && left_idx_ >= left_buffer_.size();
    }

    Rid& rid() override { return _abstract_rid; }

    ColMeta get_col_offset(const TabCol& target) {
        ColMeta col_meta = left_->get_col_offset(target);
        if (col_meta.tab_name.empty()) {
            col_meta = right_->get_col_offset(target);
            col_meta.offset += left_->get_len();
        }
        return col_meta;
    };

   private:
    bool check_join_cond(const std::vector<ColMeta>& rec_cols, const Condition& cond, const RmRecord* rec) {
        auto lhs_col = get_col(rec_cols, cond.lhs_col);
        char* lhs = rec->data + lhs_col->offset;
        char* rhs;
        ColType rhs_type;
        if (cond.is_rhs_val) {
            rhs_type = cond.rhs_val.type;
            rhs = cond.rhs_val.raw->data;
        } else {
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

    bool check_join_conds(const RmRecord* left_record, const RmRecord* right_record) {
        auto fed_conds = fed_conds_;
        int i = 0;

        for (auto& cond : fed_conds) {
            if (!cond.is_rhs_val && cond.rhs_col.tab_name != left_->get_tab_name()) {
                cond.is_rhs_val = true;

                Value val;
                ColMeta col = conds_col_type[i++];
                char* val_buf = right_record->data + col.offset;

                switch (col.type) {
                    case TYPE_INT:
                        val.set_int(*(int*)val_buf);
                        break;
                    case TYPE_FLOAT:
                        val.set_float(*(float*)val_buf);
                        break;
                    case TYPE_STRING: {
                        std::string str_val(val_buf, col.len);
                        str_val.resize(strlen(str_val.c_str()));
                        val.set_str(str_val);
                        break;
                    }
                    default:
                        throw IncompatibleTypeError(coltype2str(col.type), "Unsupported Type");
                }

                val.init_raw(col.len);
                cond.rhs_val = val;
            }
        }

        return std::all_of(fed_conds.begin(), fed_conds.end(),
                           [&](const Condition& fed_cond) {
                               return check_join_cond(left_->cols(), fed_cond, left_record);
                           });
    }
};
