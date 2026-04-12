%{
#include "ast.h"
#include "yacc.tab.h"
#include <iostream>
#include <memory>

int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

void yyerror(YYLTYPE *locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column " << locp->first_column << ": " << s << std::endl;
}

using namespace ast;
%}

// request a pure (reentrant) parser
%define api.pure full
// enable location in error handler
%locations
// enable verbose syntax error message
%define parse.error verbose

// keywords
%token SHOW TABLES CREATE TABLE DROP DESC INSERT INTO VALUES DELETE FROM ASC ORDER BY
WHERE UPDATE SET SELECT INT BIG_INT CHAR VARCHAR FLOAT DATETIME INDEX AND JOIN EXIT HELP TXN_BEGIN TXN_COMMIT TXN_ABORT TXN_ROLLBACK ORDER_BY
AS SUM MAX MIN COUNT LIMIT LOAD SET_OFF PRIMARY KEY LEFT RIGHT INNER OUTER CROSS ON
// non-keywords
%token LEQ NEQ GEQ T_EOF

// type-specific tokens
%token <sv_str> IDENTIFIER VALUE_STRING FILE_PATH
%token <sv_int> VALUE_INT
%token <sv_big_int> VALUE_BIG_INT
%token <sv_float> VALUE_FLOAT
%token <sv_datetime> VALUE_DATETIME

// specify types for non-terminal symbol
%type <sv_node> stmt dbStmt ddl dml txnStmt
%type <sv_field> field
%type <sv_fields> fieldList
%type <sv_type_len> type
%type <sv_comp_op> op
%type <sv_expr> expr
%type <sv_val> value limitClause
%type <sv_vals> valueList
%type <sv_str> tbName colName filePath
%type <sv_strs> colNameList
%type <sv_col> col
%type <sv_cols> colList selector
%type <sv_set_clause> setClause
%type <sv_set_clauses> setClauses
%type <sv_cond> condition
%type <sv_conds> whereClause optWhereClause
%type <sv_orderby>  order_clause 
%type <sv_orderby_dir> opt_asc_desc
%type <sv_orderbys> order_clause_list opt_order_clause
%type <sv_agg_func> optAggFunc
%type <sv_agg_funcs> optAggFuncList
%type <sv_join_expr> joinClause
%type <sv_from_clause> fromClause tableList

%%
start:
        stmt ';'
    {
        parse_tree = $1;
        YYACCEPT;
    }
    |   HELP
    {
        parse_tree = std::make_shared<Help>();
        YYACCEPT;
    }
    |   EXIT
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
    |   T_EOF
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
    ;

stmt:
        dbStmt
    |   ddl
    |   dml
    |   txnStmt
    |   LOAD filePath INTO tbName
    {
        $$ = std::make_shared<LoadTable>($2, $4);
    } 
    ;

txnStmt:
        TXN_BEGIN
    {
        $$ = std::make_shared<TxnBegin>();
    }
    |   TXN_COMMIT
    {
        $$ = std::make_shared<TxnCommit>();
    }
    |   TXN_ABORT
    {
        $$ = std::make_shared<TxnAbort>();
    }
    |   TXN_ROLLBACK
    {
        $$ = std::make_shared<TxnRollback>();
    }
    ;

dbStmt:
        SHOW TABLES
    {
        $$ = std::make_shared<ShowTables>();
    }
    ;

ddl:
        CREATE TABLE tbName '(' fieldList ')'
    {
        $$ = std::make_shared<CreateTable>($3, $5);
    }
    |   DROP TABLE tbName
    {
        $$ = std::make_shared<DropTable>($3);
    }
    |   DESC tbName
    {
        $$ = std::make_shared<DescTable>($2);
    }
    |   CREATE INDEX tbName '(' colNameList ')'
    {
        $$ = std::make_shared<CreateIndex>($3, $5);
    }
    |   DROP INDEX tbName '(' colNameList ')'
    {
        $$ = std::make_shared<DropIndex>($3, $5);
    }
    |   SHOW INDEX FROM tbName
    {
        $$ = std::make_shared<ShowIndex>($4);
    }
    ;

dml:
        INSERT INTO tbName VALUES '(' valueList ')'
    {
        $$ = std::make_shared<InsertStmt>($3, $6);
    }
    |   DELETE FROM tbName optWhereClause
    {
        $$ = std::make_shared<DeleteStmt>($3, $4);
    }
    |   UPDATE tbName SET setClauses optWhereClause
    {
        $$ = std::make_shared<UpdateStmt>($2, $4, $5);
    }
    |   SELECT selector optAggFuncList FROM fromClause optWhereClause opt_order_clause limitClause
    {
        $$ = std::make_shared<SelectStmt>($2, $3, $5->tabs, $6, $5->jointree, $7, $8);
    }
    ;

fieldList:
        field
    {
        $$ = std::vector<std::shared_ptr<Field>>{$1};
    }
    |   fieldList ',' field
    {
        $$.push_back($3);
    }
    ;

colNameList:
        colName
    {
        $$ = std::vector<std::string>{$1};
    }
    | colNameList ',' colName
    {
        $$.push_back($3);
    }
    ;

field:
        colName type
    {
        $$ = std::make_shared<ColDef>($1, $2);
    }
    |   colName type PRIMARY KEY
    {
        $$ = std::make_shared<ColDef>($1, $2, true);
    }
    ;

type:
        INT
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
    }
    |   BIG_INT
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_BIG_INT, sizeof(long long int));
    }
    |   CHAR '(' VALUE_INT ')'
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_STRING, $3);
    }
    |   VARCHAR '(' VALUE_INT ')'
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_STRING, $3);
    }
    |   FLOAT
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(float));
    }
    |   DATETIME
    {
        $$ = std::make_shared<TypeLen>(SV_TYPE_DATETIME, 19);
    }
    ;

valueList:
        value
    {
        $$ = std::vector<std::shared_ptr<ast::Value>>{$1};
    }
    |   valueList ',' value
    {
        $$.push_back($3);
    }
    ;

value:
        VALUE_INT
    {
        $$ = std::make_shared<IntLit>($1);
    }
    |   VALUE_BIG_INT
    {
        $$ = std::make_shared<BigIntLit>($1);
    }
    |   VALUE_FLOAT
    {
        $$ = std::make_shared<FloatLit>($1);
    }
    |   VALUE_STRING
    {
        $$ = std::make_shared<StringLit>($1);
    }
    |   VALUE_DATETIME
    {
        $$ = std::make_shared<DatetimeLit>($1);
    }
    ;

condition:
        col op expr
    {
        $$ = std::make_shared<BinaryExpr>($1, $2, $3);
    }
    ;

optWhereClause:
        /* epsilon */ { /* ignore*/ }
    |   WHERE whereClause
    {
        $$ = $2;
    }
    ;

whereClause:
        condition 
    {
        $$ = std::vector<std::shared_ptr<BinaryExpr>>{$1};
    }
    |   whereClause AND condition
    {
        $$.push_back($3);
    }
    ;

col:
        tbName '.' colName
    {
        $$ = std::make_shared<Col>($1, $3);
    }
    |   colName
    {
        $$ = std::make_shared<Col>("", $1);
    }
    ;

colList:
        col
    {
        $$ = std::vector<std::shared_ptr<Col>>{$1};
    }
    |   colList ',' col
    {
        $$.push_back($3);
    }
    ;

op:
        '='
    {
        $$ = SV_OP_EQ;
    }
    |   '<'
    {
        $$ = SV_OP_LT;
    }
    |   '>'
    {
        $$ = SV_OP_GT;
    }
    |   NEQ
    {
        $$ = SV_OP_NE;
    }
    |   LEQ
    {
        $$ = SV_OP_LE;
    }
    |   GEQ
    {
        $$ = SV_OP_GE;
    }
    ;

expr:
        value
    {
        $$ = std::static_pointer_cast<Expr>($1);
    }
    |   col
    {
        $$ = std::static_pointer_cast<Expr>($1);
    }
    ;

setClauses:
        setClause
    {
        $$ = std::vector<std::shared_ptr<ast::SetClause>>{$1};
    }
    |   setClauses ',' setClause
    {
        $$.push_back($3);
    }
    ;

setClause:
        colName '=' value
    {
        $$ = std::make_shared<ast::SetClause>($1, $3);
    }
    |   colName '=' colName value
    {
        $$ = std::make_shared<ast::SetClause>($1, $4, true);
    }
    ;

optAggFunc:
        SUM '(' col ')' AS colName
    {
        $$ = std::make_shared<ast::AggFunc>("SUM", $3, $6);
    } 
    |   MAX '(' col ')' AS colName
    {
        $$ = std::make_shared<ast::AggFunc>("MAX", $3, $6);
    }   
    |   MIN '(' col ')' AS colName
    {
        $$ = std::make_shared<ast::AggFunc>("MIN", $3, $6);
    } 
    |   COUNT '(' col ')' AS colName
    {
        $$ = std::make_shared<ast::AggFunc>("COUNT", $3, $6);
    }
    |   COUNT '(' '*' ')' AS colName
    {
        $$ = std::make_shared<ast::AggFunc>("COUNT", nullptr, $6);
    }
    |   /* epsilon */ { /* ignore*/ }
    ;

optAggFuncList:
        optAggFunc
    {
        $$ = std::vector<std::shared_ptr<ast::AggFunc>>{$1};
    }
    |   optAggFuncList ',' optAggFunc
    {
        $$.push_back($3);
    }
    ;

selector:
        '*'
    {
        $$ = {};
    }
    |   colList
    {

    }
    |   /* epsilon */ { /* ignore*/ }
    ;

tableList:
        tbName
    {
        $$ = std::make_shared<FromClause>();
        $$->tabs.push_back($1);
    }
    |   tableList ',' tbName
    {
        $$ = $1;
        $$->tabs.push_back($3);
    }
    |   tableList joinClause
    {
        $$ = $1;
        $$->tabs.push_back($2->right);
        $$->jointree.push_back($2);
    }
    ;

joinClause:
        JOIN tbName ON condition
    {
        $$ = std::make_shared<JoinExpr>("", $2, std::vector<std::shared_ptr<BinaryExpr>>{$4}, JoinType::INNER_JOIN);
    }
    |   JOIN tbName
    {
        $$ = std::make_shared<JoinExpr>("", $2, std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::INNER_JOIN);
    }
    |   INNER JOIN tbName ON condition
    {
        $$ = std::make_shared<JoinExpr>("", $3, std::vector<std::shared_ptr<BinaryExpr>>{$5}, JoinType::INNER_JOIN);
    }
    |   INNER JOIN tbName
    {
        $$ = std::make_shared<JoinExpr>("", $3, std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::INNER_JOIN);
    }
    |   CROSS JOIN tbName
    {
        $$ = std::make_shared<JoinExpr>("", $3, std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::INNER_JOIN);
    }
    |   LEFT JOIN tbName ON condition
    {
        $$ = std::make_shared<JoinExpr>("", $3, std::vector<std::shared_ptr<BinaryExpr>>{$5}, JoinType::LEFT_JOIN);
    }
    |   LEFT JOIN tbName
    {
        $$ = std::make_shared<JoinExpr>("", $3, std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::LEFT_JOIN);
    }
    |   LEFT OUTER JOIN tbName ON condition
    {
        $$ = std::make_shared<JoinExpr>("", $4, std::vector<std::shared_ptr<BinaryExpr>>{$6}, JoinType::LEFT_JOIN);
    }
    |   LEFT OUTER JOIN tbName
    {
        $$ = std::make_shared<JoinExpr>("", $4, std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::LEFT_JOIN);
    }
    |   RIGHT JOIN tbName ON condition
    {
        $$ = std::make_shared<JoinExpr>("", $3, std::vector<std::shared_ptr<BinaryExpr>>{$5}, JoinType::RIGHT_JOIN);
    }
    |   RIGHT JOIN tbName
    {
        $$ = std::make_shared<JoinExpr>("", $3, std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::RIGHT_JOIN);
    }
    |   RIGHT OUTER JOIN tbName ON condition
    {
        $$ = std::make_shared<JoinExpr>("", $4, std::vector<std::shared_ptr<BinaryExpr>>{$6}, JoinType::RIGHT_JOIN);
    }
    |   RIGHT OUTER JOIN tbName
    {
        $$ = std::make_shared<JoinExpr>("", $4, std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::RIGHT_JOIN);
    }
    ;

fromClause:
        tableList
    {
        $$ = $1;
    }
    ;

opt_asc_desc:
        ASC          
    { 
        $$ = OrderBy_ASC;     
    }
    |   DESC      
    { 
        $$ = OrderBy_DESC;    
    }
    |       
    { 
        $$ = OrderBy_DEFAULT; 
    }
    ;    

order_clause:
        col  opt_asc_desc 
    { 
        $$ = std::make_shared<OrderBy>($1, $2);
    }
    ;   

order_clause_list:
        order_clause
    {
        $$ = std::vector<std::shared_ptr<OrderBy>>{$1};
    }
    |   order_clause_list ',' order_clause
    {
        $$.push_back($3);
    }
    ;

opt_order_clause:
        ORDER BY order_clause_list
    { 
        $$ = $3;
    }
    |   /* epsilon */ { /* ignore*/ }
    ;

limitClause:
        LIMIT value
    {
        $$ = $2;
    }
    |   /* epsilon */ { /* ignore*/ }
    ;

tbName: IDENTIFIER;

colName: IDENTIFIER;

filePath: FILE_PATH;
%%

