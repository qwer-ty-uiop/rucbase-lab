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

class SortExecutor : public AbstractExecutor {
   private:
    std::unique_ptr<AbstractExecutor> prev_;       // 上一个执行器
    std::vector<ColMeta> cols_;                    // 列元数据
    std::vector<ColMeta> sort_cols_;               // 排序列元数据
    std::deque<std::unique_ptr<RmRecord>> tuples;  // 元组队列
    std::vector<bool> is_descs_;                   // 是否使用降序排序
    std::unique_ptr<RmRecord> current_tuple;       // 当前元组

   public:
    // 初始化排序执行器
    SortExecutor(std::unique_ptr<AbstractExecutor> prev,
                 std::vector<TabCol> sel_sort_cols,
                 std::vector<bool> is_descs) {
        prev_ = std::move(prev);
        cols_ = prev_->cols();
        for (auto sel_sort_col : sel_sort_cols) {
            sort_cols_.push_back(prev_->get_col_offset(sel_sort_col));
        }
        is_descs_ = std::move(is_descs);
        tuples.clear();
    }

    // 从 prev_ 获取所有记录元组，并排序
    void beginTuple() override {
        prev_->beginTuple();
        while (true) {
            std::unique_ptr<RmRecord> tuple = prev_->Next();
            if (!tuple) {
                break;
            }
            tuples.push_back(std::move(tuple));
            prev_->nextTuple();
        }

        std::sort(tuples.begin(), tuples.end(), [this](const std::unique_ptr<RmRecord>& lhs, const std::unique_ptr<RmRecord>& rhs) {
            for (size_t index = 0; index < sort_cols_.size(); ++index) {
                const ColMeta& columnMetadata = sort_cols_[index];
                const bool sortByDescending = is_descs_[index];
                int comparisonResult = compareColumns(lhs.get(), rhs.get(), columnMetadata);

                if (comparisonResult != 0) {
                    return sortByDescending ? comparisonResult > 0 : comparisonResult < 0;
                }
            }
            return false;
        });

        nextTuple();
    }
    // 获取下一个元组
    void nextTuple() override {
        if (!tuples.empty()) {
            current_tuple = std::move(tuples.front());
            tuples.pop_front();
        } else {
            current_tuple = nullptr;
        }
    }
    // 获取下一个元组
    std::unique_ptr<RmRecord> Next() override {
        return std::move(current_tuple);
    }
    // 获取列元数据
    const std::vector<ColMeta>& cols() const override { return cols_; }
    // 是否结束
    bool is_end() const override {
        if (!current_tuple)
            return true;
        else
            return false;
    }
    // 返回当前rid
    Rid& rid() override { return _abstract_rid; }

   private:
    // 比较器
    int compareColumns(const RmRecord* a,
                       const RmRecord* b,
                       const ColMeta& col) const {
        if (col.type == TYPE_INT) {
            int val_a = *(int*)(a->data + col.offset);
            int val_b = *(int*)(b->data + col.offset);
            return val_a - val_b;
        } else if (col.type == TYPE_FLOAT) {
            float val_a = *(float*)(a->data + col.offset);
            float val_b = *(float*)(b->data + col.offset);
            return (val_a < val_b) ? -1 : (val_a > val_b) ? 1
                                                          : 0;
        } else if (col.type == TYPE_STRING) {
            const std::string str_a(a->data + col.offset);
            const std::string str_b(b->data + col.offset);
            return str_a.compare(str_b);
        } else
            throw InternalError("Unexpected Type");
    }
};