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

#include <cassert>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "parser/parser.h"
#include "system/sm.h"
#include "common/common.h"

class Query{
    public:
    std::shared_ptr<ast::TreeNode> parse;
    std::vector<JoinCondition> jointree;
    std::vector<Condition> conds;
    std::vector<TabCol> cols;
    std::vector<std::string> tables;
    std::vector<SetClause> set_clauses;
    std::vector<Value> values;
    std::vector<AggFunc> agg_funcs;

    int limit_num;

    Query(){}
};

class Analyze
{
private:
    SmManager *sm_manager_;
public:
    Analyze(SmManager *sm_manager) : sm_manager_(sm_manager){}
    ~Analyze(){}

    std::shared_ptr<Query> do_analyze(std::shared_ptr<ast::TreeNode> root);

private:
    TabCol check_column(const std::vector<ColMeta> &all_cols, TabCol target);
    void get_all_cols(const std::vector<std::string> &tab_names, std::vector<ColMeta> &all_cols);
    void get_clause(const std::vector<std::shared_ptr<ast::BinaryExpr>> &sv_conds, std::vector<Condition> &conds);
    void check_clause(const std::vector<std::string> &tab_names, std::vector<Condition> &conds);
    void check_join_clause(const std::vector<std::string> &tab_names, std::vector<JoinCondition> &join_conds);
    Value convert_sv_value(const std::shared_ptr<ast::Value> &sv_val);
    CompOp convert_sv_comp_op(ast::SvCompOp op);
    void performTypeConversion(Condition &condition, ColType lhsType, ColType rhsType);
    void processSelectStatement(const std::shared_ptr<ast::SelectStmt> &selectStmt, std::shared_ptr<Query> &query);
    void handleAggregateFunctions(const std::shared_ptr<ast::SelectStmt> &selectStmt, std::shared_ptr<Query> &query);
    void handleNormalColumns(std::shared_ptr<Query> &query);
    void handleSelectAllColumns(std::shared_ptr<Query> &query);
    void processUpdateStatement(const std::shared_ptr<ast::UpdateStmt> &updateStmt, std::shared_ptr<Query> &query);
};
