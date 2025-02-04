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
#include "common/common.h"
#include "index/ix.h"
#include "system/sm.h"

class AbstractExecutor {
   public:
    Rid _abstract_rid;

    Context *context_;

    virtual ~AbstractExecutor() = default;

    virtual size_t tupleLen() const { return 0; };

    virtual const std::vector<ColMeta> &cols() const {
        std::vector<ColMeta> *_cols = nullptr;
        return *_cols;
    };

    virtual std::string getType() { return "AbstractExecutor"; };

    virtual void beginTuple(){};

    virtual void nextTuple(){};

    virtual bool is_end() const { return true; };

    virtual Rid &rid() = 0;

    virtual std::unique_ptr<RmRecord> Next() = 0;

    virtual void feed(const std::map<TabCol, Value> &feed_dict){};

    virtual ColMeta get_col_offset(const TabCol &target) { return ColMeta();};

    virtual std::string get_tab_name() { 
        std::string tab_name;
        return tab_name;
    }

    virtual std::vector<std::string> get_func_names() { 
        std::vector<std::string> func_names;
        return func_names;
    }

    virtual size_t get_len() {
        return -1;
    }
    
    std::vector<ColMeta>::const_iterator get_col(const std::vector<ColMeta> &rec_cols, const TabCol &target) {
        auto pos = std::find_if(rec_cols.begin(), rec_cols.end(), [&](const ColMeta &col) {
            return col.tab_name == target.tab_name && col.name == target.col_name;
        });
        if (pos == rec_cols.end()) {
            throw ColumnNotFoundError(target.tab_name + '.' + target.col_name);
        }
        return pos;
    }

    std::map<TabCol, Value> rec2dict(const std::vector<ColMeta> &cols, const RmRecord *rec) {
        std::map<TabCol, Value> rec_dict;
        for (auto &col : cols) {
            TabCol key = {.tab_name = col.tab_name, .col_name = col.name};
            Value val;
            char *val_buf = rec->data + col.offset;
            if (col.type == TYPE_INT) {
                val.set_int(*(int *)val_buf);
            } else if (col.type == TYPE_FLOAT) {
                val.set_float(*(float *)val_buf);
            } else if (col.type == TYPE_STRING) {
                std::string str_val((char *)val_buf, col.len);
                str_val.resize(strlen(str_val.c_str()));
                val.set_str(str_val);
            }
            assert(rec_dict.count(key) == 0);
            val.init_raw(col.len);
            rec_dict[key] = val;
        }
        return rec_dict;
    }
};