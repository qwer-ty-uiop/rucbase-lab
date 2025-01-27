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

class InsertExecutor : public AbstractExecutor {
   private:
    TabMeta tab_;                // 表的元数据
    std::vector<Value> values_;  // 需要插入的数据
    RmFileHandle* fh_;           // 表的数据文件句柄
    std::string tab_name_;       // 表名称
    Rid rid_;                    // 插入的位置，由于系统默认插入时不指定位置，因此当前rid_在插入后才赋值
    SmManager* sm_manager_;

   public:
    InsertExecutor(SmManager* sm_manager,
                   const std::string& tab_name,
                   std::vector<Value> values,
                   Context* context) {
        sm_manager_ = sm_manager;
        tab_ = sm_manager_->db_.get_table(tab_name);
        values_ = values;
        tab_name_ = tab_name;
        if (values.size() != tab_.cols.size()) {
            throw InvalidValueCountError();
        }
        fh_ = sm_manager_->fhs_.at(tab_name).get();
        context_ = context;
    };

    std::unique_ptr<RmRecord> Next() override {
        RmRecord rec(fh_->get_file_hdr().record_size);
        // 检查类型
        for (size_t i = 0; i < values_.size(); i++) {
            auto& col = tab_.cols[i];
            auto& val = values_[i];
            if (col.type == TYPE_BIG_INT && val.type == TYPE_INT) {
                val.type = TYPE_BIG_INT;
                val.raw = std::make_shared<RmRecord>(8);
                *(long long int*)(val.raw->data) = (long long int)val.int_val;
            } else if (col.type == TYPE_FLOAT && val.type == TYPE_INT) {
                val.type = TYPE_FLOAT;
                val.raw = std::make_shared<RmRecord>(4);
                *(float*)(val.raw->data) = (float)val.int_val;
            } else if (col.type == TYPE_STRING && val.type == TYPE_DATETIME) {
                val.type = TYPE_DATETIME;
                val.init_raw(col.len);
            } else if (col.type != val.type) {
                throw IncompatibleTypeError(coltype2str(col.type), coltype2str(val.type));
            } else {
                val.init_raw(col.len);
            }
            memcpy(rec.data + col.offset, val.raw->data, col.len);
        }

        /**
         *  唯一性检查
         * @note： 唯一性检查应该在数据和索引的插入之前完成，不能边插入边检查，
         *         否则当有多个索引时，可能会出现
         * 前面的索引插入成功，后面的索引插入失败的情况。导致数据不一致
         */
        for (auto& index : tab_.indexes) {
            auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
            char key[index.col_tot_len];
            index.get_key(&rec, key);
            if (ih->contains(key, context_->txn_)) {
                throw UniqueConstraintError();
            }
        }

        context_->lock_mgr_->lock_exclusive_on_record(context_->txn_, rid_, fh_->GetFd());

        // 插入数据
        rid_ = fh_->insert_record(rec.data, context_);

        // 插入索引
        for (auto& index : tab_.indexes) {
            auto ih = sm_manager_->ihs_.at(sm_manager_->get_ix_manager()->get_index_name(tab_name_, index.cols)).get();
            char key[index.col_tot_len];
            index.get_key(&rec, key);
            ih->insert_entry(key, rid_, context_->txn_);
        }

        // 记录事务
        auto write_record = WriteRecord(WType::INSERT_TUPLE, tab_name_, rid_);
        context_->txn_->append_write_record(write_record);

        // 记录日志
        InsertLogRecord log_record(context_->txn_->get_transaction_id(), rec, rid_, tab_name_);
        context_->log_mgr_->add_log_to_buffer(&log_record);
        return nullptr;
    }

    Rid& rid() override { return rid_; }
};