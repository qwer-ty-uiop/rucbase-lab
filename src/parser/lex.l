    /* keywords are case insensitive */
%option caseless
    /* we don't need yywrap() function */
%option noyywrap
    /* we don't need yyunput() function */
%option nounput
    /* we don't need input() function */
%option noinput
    /* enable location */
%option bison-bridge
%option bison-locations

%{
#include "ast.h"
#include "yacc.tab.h"
#include "errors.h"
#include <iostream>
#include <limits>

// automatically update location
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line; \
    yylloc->first_column = yylloc->last_column; \
    for (int i = 0; yytext[i] != '\0'; i++) { \
        if(yytext[i] == '\n') { \
            yylloc->last_line++; \
            yylloc->last_column = 1; \
        } else { \
            yylloc->last_column++; \
        } \
    }

bool isValidDatetime(std::string datetime_str);
%}

alpha [a-zA-Z]
digit [0-9]
white_space [ \t]+
new_line "\r"|"\n"|"\r\n"
sign "+"|"-"
identifier {alpha}(_|{alpha}|{digit})*
file_path "../../src/test/performance_test/"[^\n]+\.csv
value_int {sign}?{digit}+
value_float {sign}?{digit}+\.({digit}+)?
value_datetime '{digit}{1,4}-{digit}{1,2}-{digit}{1,2}\ {digit}{1,2}:{digit}{1,2}:{digit}{1,2}'
value_string '[^']*'
single_op ";"|"("|")"|","|"*"|"="|">"|"<"|"."

%x STATE_COMMENT

%%
    /* block comment */
"/*" { BEGIN(STATE_COMMENT); }
<STATE_COMMENT>"*/" { BEGIN(INITIAL); }
<STATE_COMMENT>[^*] { /* ignore the text of the comment */ }
<STATE_COMMENT>\* { /* ignore *'s that aren't part of */ }
    /* single line comment */
"--".* { /* ignore single line comment */ }
    /* white space and new line */
{white_space} { /* ignore white space */ }
{new_line} { /* ignore new line */ }
    /* keywords */
"SHOW" { return SHOW; }
"BEGIN" { return TXN_BEGIN; }
"COMMIT" { return TXN_COMMIT; }
"ABORT" { return TXN_ABORT; }
"ROLLBACK" { return TXN_ROLLBACK; }
"TABLES" { return TABLES; }
"CREATE" { return CREATE; }
"TABLE" { return TABLE; }
"DROP" { return DROP; }
"DESC" { return DESC; }
"INSERT" { return INSERT; }
"INTO" { return INTO; }
"VALUES" { return VALUES; }
"DELETE" { return DELETE; }
"FROM" { return FROM; }
"WHERE" { return WHERE; }
"UPDATE" { return UPDATE; }
"SET" { return SET; }
"SELECT" { return SELECT; }
"INT" { return INT; }
"BIGINT" { return BIG_INT; }
"CHAR" { return CHAR; }
"FLOAT" { return FLOAT; }
"DATETIME" { return DATETIME; }
"INDEX" { return INDEX; }
"AND" { return AND; }
"JOIN" {return JOIN;}
"EXIT" { return EXIT; }
"HELP" { return HELP; }
"ORDER" { return ORDER; }
"BY" { return BY;  }
"LIMIT" { return LIMIT; }
"ASC" { return ASC; }
"AS" { return AS; }
"SUM" { return SUM; }
"MAX" { return MAX; }
"MIN" { return MIN; }
"COUNT" { return COUNT; }
"LOAD" { return LOAD; }
    /* operators */
">=" { return GEQ; }
"<=" { return LEQ; }
"<>" { return NEQ; }
{single_op} { return yytext[0]; }
    /* id */
{identifier} {
    yylval->sv_str = yytext;
    return IDENTIFIER;
}
    /* literals */
{value_int} {
    try {
        long long int value = std::stoll(yytext);
        if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
            // 超出 4 字节有符号数的范围内
            yylval->sv_big_int = value;
            return VALUE_BIG_INT;
        } 
        yylval->sv_int = atoi(yytext);
        return VALUE_INT;
    } catch (const std::out_of_range& e) {
        throw InternalError("failure");
    } 
}
{value_float} {
    yylval->sv_float = atof(yytext);
    return VALUE_FLOAT;
}
{value_datetime} { 
    std::string datetime_str = std::string(yytext + 1, strlen(yytext) - 2);
    if (isValidDatetime(datetime_str)) {
        yylval->sv_datetime = datetime_str;
        return VALUE_DATETIME;
    } else {
        throw InternalError("failure");
    }    
}
{value_string} {
    yylval->sv_str = std::string(yytext + 1, strlen(yytext) - 2);
    return VALUE_STRING;
}
{file_path} { 
    yylval->sv_str = yytext;
    return FILE_PATH; 
}
    /* EOF */
<<EOF>> { return T_EOF; }
    /* unexpected char */
. { std::cerr << "Lexer Error: unexpected character " << yytext[0] << std::endl; }
%%

bool isValidDatetime(std::string datetime_str) {
    if (datetime_str.size() < 19) return false;
    if (datetime_str < "1000-01-01 00:00:00" || datetime_str > "9999-12-31 23:59:59") {
        return false;
    }
    int month = std::stoi(datetime_str.substr(5, 6));
    if (month < 1 || month > 12) return false;
    int day = std::stoi(datetime_str.substr(8, 9));
    if (month == 2 && day == 30) return false;
    if (day < 1 || day > 31) return false;
    int hour = std::stoi(datetime_str.substr(11, 12));
    int minute = std::stoi(datetime_str.substr(14, 15));
    int second = std::stoi(datetime_str.substr(17, 18));
    if (hour > 23 || minute > 59 || second > 59) return false;

    return true;
} 