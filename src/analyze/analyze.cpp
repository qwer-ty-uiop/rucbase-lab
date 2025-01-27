/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "analyze.h"

/**
 * @description: 分析器，进行语义分析和查询重写，需要检查不符合语义规定的部分
 * @param {shared_ptr<ast::TreeNode>} parse parser生成的结果集
 * @return {shared_ptr<Query>} Query 
 */
std::shared_ptr<Query> Analyze::do_analyze(std::shared_ptr<ast::TreeNode> parse)
{
    std::shared_ptr<Query> querytable = std::make_shared<Query>();
    if (auto x = std::dynamic_pointer_cast<ast::SelectStmt>(parse))
    {
        // 处理表名
        querytable->tables = std::move(x->tabs);
        /** TODO: 检查表是否存在 */
        for (const auto tabName : x->tabs) {
            if (!(sm_manager_->db_.is_table(tabName))) {
                throw TableNotFoundError(tabName);
            }
        }
        
        // 处理target list，再target list中添加上表名，例如 a.id
        for (const auto &handle_sel_col : x->cols) {
            TabCol Selcol = {.tab_name = handle_sel_col->tab_name, .col_name = handle_sel_col->col_name};
            querytable->cols.push_back(Selcol);
        }

        std::vector<ColMeta> all_cols;
        get_all_cols(querytable->tables, all_cols);     
        if (!x->agg_funcs.empty() && x->agg_funcs[0]) {
            // handle agg_funcs
            for (const auto &handle_agg_func : x->agg_funcs) {
             AggFunc agg_func;
                agg_func.func_name = handle_agg_func->func_name;
                agg_func.new_col_name = handle_agg_func->new_col_name;
                if (handle_agg_func->col) {
                    agg_func.col = {.tab_name = handle_agg_func->col->tab_name,
                               .col_name = handle_agg_func->col->col_name};
                    agg_func.col = check_column(all_cols, agg_func.col);      
                } else {
                    // COUNT *
                    agg_func.func_name = "COUNT*";
                    TabCol Selcol = {.tab_name = all_cols[0].tab_name, .col_name = all_cols[0].name};
                    querytable->cols.push_back(Selcol);    
                }

                querytable->agg_funcs.push_back(agg_func);
            }
        } else {
            if (querytable->cols.empty()) {
                // select all columns
                for (const auto &col : all_cols) {
                    TabCol sel_col = {.tab_name = col.tab_name, .col_name = col.name};
                    querytable->cols.push_back(sel_col);
                }
            } else {
                // infer table name from column name
                for (auto &sel_col : querytable->cols) {
                    sel_col = check_column(all_cols, sel_col);  // 列元数据校验
                }
            }
        }
          
        //处理where条件
        get_clause(x->conds, querytable->conds);
        check_clause(querytable->tables, querytable->conds);

        // handle LIMIT
        querytable->limit_num = x->limit_num;
    } else if (auto x = std::dynamic_pointer_cast<ast::UpdateStmt>(parse)) {
        /** TODO: */
        // 处理where条件
        get_clause(x->conds, querytable->conds);
        check_clause({x->tab_name}, querytable->conds);
        // 处理 set
        for (const auto &handle_set_clause : x->set_clauses) {
            SetClause Setclause = {.lhs = {.tab_name = "", .col_name = handle_set_clause->col_name},
                                    .rhs = convert_sv_value(handle_set_clause->val), .flag = handle_set_clause->flag };
            querytable->set_clauses.push_back(Setclause);                       
        }
    } else if (auto x = std::dynamic_pointer_cast<ast::DeleteStmt>(parse)) {
        //处理where条件
        get_clause(x->conds, querytable->conds);
        check_clause({x->tab_name}, querytable->conds);        
    } else if (auto x = std::dynamic_pointer_cast<ast::InsertStmt>(parse)) {
        // 处理insert 的values值
        for (const auto &handle_val : x->vals) {
            querytable->values.push_back(convert_sv_value(handle_val));
        }
    } else { 
        // do nothing
    }
    querytable->parse = std::move(parse);
    return querytable;
}



TabCol Analyze::check_column(const std::vector<ColMeta> &all_cols, TabCol target) {
// Check if the target table name is specified
if (target.tab_name.empty()) {
    // Infer the table name from the column name
    std::string inferredTabName;
    for (const auto& col : all_cols) {
        if (col.name == target.col_name) {
            if (!inferredTabName.empty()) {
                throw AmbiguousColumnError(target.col_name); // Multiple tables with the same column name
            }
            inferredTabName = col.tab_name;
        }
    }
    if (inferredTabName.empty()) {
        throw ColumnNotFoundError(target.col_name); // No column found
    }
    target.tab_name = inferredTabName; // Assign the inferred table name
} else {
    // Verify that the specified table and column exist
    if (!sm_manager_->db_.is_table(target.tab_name) ||
        !sm_manager_->db_.get_table(target.tab_name).is_col(target.col_name)) {
        throw ColumnNotFoundError(target.tab_name + '.' + target.col_name); // Specified column not found
    }
}

    return target;
}

void Analyze::get_all_cols(const std::vector<std::string> &tab_names, std::vector<ColMeta> &all_cols) {
    for (auto &sel_tab_name : tab_names) {
        // 这里db_不能写成get_db(), 注意要传指针
        const auto &sel_tab_cols = sm_manager_->db_.get_table(sel_tab_name).cols;
        all_cols.insert(all_cols.end(), sel_tab_cols.begin(), sel_tab_cols.end());
    }
}

void Analyze::get_clause(const std::vector<std::shared_ptr<ast::BinaryExpr>>& binaryExpressions, std::vector<Condition>& conditions) {
    conditions.clear(); // Clear any existing conditions before adding new ones

    // Process each binary expression to extract conditions
    for (const auto& expression : binaryExpressions) {
        Condition condition;

        // Set left-hand side column info from the expression
        condition.lhs_col = {expression->lhs->tab_name, expression->lhs->col_name};

        // Convert the operation type from expression format to condition format
        condition.op = convert_sv_comp_op(expression->op);

        // Attempt to cast right-hand side of the expression to a value type
        if (auto rhsValue = std::dynamic_pointer_cast<ast::Value>(expression->rhs)) {
            condition.is_rhs_val = true;
            condition.rhs_val = convert_sv_value(rhsValue);
        }
        // If RHS is not a value, attempt to cast it as a column
        else if (auto rhsCol = std::dynamic_pointer_cast<ast::Col>(expression->rhs)) {      
            condition.is_rhs_val = false;
            condition.rhs_col = {rhsCol->tab_name, rhsCol->col_name};
        }

        // Add the fully formed condition to the list
        conditions.push_back(condition);
    }
}

void Analyze::performTypeConversion(Condition& condition, ColType lhsType, ColType rhsType) {
    switch (lhsType) {
        case TYPE_FLOAT:
            if (rhsType == TYPE_INT) {
                condition.rhs_val.type = TYPE_FLOAT;
                *(float*)(condition.rhs_val.raw->data) = static_cast<float>(condition.rhs_val.int_val);
            }
            break;
        case TYPE_BIG_INT:
            if (rhsType == TYPE_INT) {
                condition.rhs_val.type = TYPE_BIG_INT;
                *(long long*)(condition.rhs_val.raw->data) = static_cast<long long>(condition.rhs_val.int_val);
            }
            break;
        case TYPE_STRING:
            if (rhsType == TYPE_DATETIME) {
                condition.rhs_val.type = TYPE_STRING; // Assuming conversion is trivial and handled elsewhere
            }
            break;
        default:
            throw IncompatibleTypeError(coltype2str(lhsType), coltype2str(rhsType));
    }
}
void Analyze::check_clause(const std::vector<std::string>& tableNames, std::vector<Condition>& conditions) {
    std::vector<ColMeta> allColumns;
    get_all_cols(tableNames, allColumns); // Populate allColumns with metadata

    // Process each condition to ensure correct types and resolve table and column metadata
    for (auto& condition : conditions) {
        // Resolve column metadata for left-hand side
        condition.lhs_col = check_column(allColumns, condition.lhs_col);
        TabMeta& lhsTable = sm_manager_->db_.get_table(condition.lhs_col.tab_name);
        auto lhsColumn = lhsTable.get_col(condition.lhs_col.col_name);
        ColType lhsType = lhsColumn->type;

        ColType rhsType;

        if (condition.is_rhs_val) {
            // Initialize and set type for right-hand side value
            condition.rhs_val.init_raw(lhsColumn->len);
            rhsType = condition.rhs_val.type;
        } else {
            // Resolve column metadata for right-hand side
            condition.rhs_col = check_column(allColumns, condition.rhs_col);
            TabMeta& rhsTable = sm_manager_->db_.get_table(condition.rhs_col.tab_name);
            auto rhsColumn = rhsTable.get_col(condition.rhs_col.col_name);
            rhsType = rhsColumn->type;
        }

        // Perform type compatibility check and conversions
        if (lhsType != rhsType) {
            performTypeConversion(condition, lhsType, rhsType);
        }
    }
}



Value Analyze::convert_sv_value(const std::shared_ptr<ast::Value> &sv_val) {
    Value val;
    if (auto int_lit = std::dynamic_pointer_cast<ast::IntLit>(sv_val)) {
        val.set_int(int_lit->val);
    } else if (auto big_int_lit = std::dynamic_pointer_cast<ast::BigIntLit>(sv_val)) {
        val.set_big_int(big_int_lit->val);
    } else if (auto float_lit = std::dynamic_pointer_cast<ast::FloatLit>(sv_val)) {
        val.set_float(float_lit->val);
    } else if (auto str_lit = std::dynamic_pointer_cast<ast::StringLit>(sv_val)) {
        val.set_str(str_lit->val);
    } else if (auto datetime_lit = std::dynamic_pointer_cast<ast::DatetimeLit>(sv_val)) {
        val.set_datetime(datetime_lit->val);
    } else {
        throw InternalError("Unexpected sv value type");
    }
    return val;
}

CompOp Analyze::convert_sv_comp_op(ast::SvCompOp op) {
    std::map<ast::SvCompOp, CompOp> m = {
        {ast::SV_OP_EQ, OP_EQ}, {ast::SV_OP_NE, OP_NE}, {ast::SV_OP_LT, OP_LT},
        {ast::SV_OP_GT, OP_GT}, {ast::SV_OP_LE, OP_LE}, {ast::SV_OP_GE, OP_GE},
    };
    return m.at(op);
}