/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_MNT_E_CODE_PROJECT_RUCBASE_LAB_SRC_PARSER_YACC_TAB_H_INCLUDED
# define YY_YY_MNT_E_CODE_PROJECT_RUCBASE_LAB_SRC_PARSER_YACC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SHOW = 258,                    /* SHOW  */
    TABLES = 259,                  /* TABLES  */
    CREATE = 260,                  /* CREATE  */
    TABLE = 261,                   /* TABLE  */
    DROP = 262,                    /* DROP  */
    DESC = 263,                    /* DESC  */
    INSERT = 264,                  /* INSERT  */
    INTO = 265,                    /* INTO  */
    VALUES = 266,                  /* VALUES  */
    DELETE = 267,                  /* DELETE  */
    FROM = 268,                    /* FROM  */
    ASC = 269,                     /* ASC  */
    ORDER = 270,                   /* ORDER  */
    BY = 271,                      /* BY  */
    WHERE = 272,                   /* WHERE  */
    UPDATE = 273,                  /* UPDATE  */
    SET = 274,                     /* SET  */
    SELECT = 275,                  /* SELECT  */
    INT = 276,                     /* INT  */
    BIG_INT = 277,                 /* BIG_INT  */
    CHAR = 278,                    /* CHAR  */
    VARCHAR = 279,                 /* VARCHAR  */
    FLOAT = 280,                   /* FLOAT  */
    DATETIME = 281,                /* DATETIME  */
    INDEX = 282,                   /* INDEX  */
    AND = 283,                     /* AND  */
    JOIN = 284,                    /* JOIN  */
    EXIT = 285,                    /* EXIT  */
    HELP = 286,                    /* HELP  */
    TXN_BEGIN = 287,               /* TXN_BEGIN  */
    TXN_COMMIT = 288,              /* TXN_COMMIT  */
    TXN_ABORT = 289,               /* TXN_ABORT  */
    TXN_ROLLBACK = 290,            /* TXN_ROLLBACK  */
    ORDER_BY = 291,                /* ORDER_BY  */
    AS = 292,                      /* AS  */
    SUM = 293,                     /* SUM  */
    MAX = 294,                     /* MAX  */
    MIN = 295,                     /* MIN  */
    COUNT = 296,                   /* COUNT  */
    LIMIT = 297,                   /* LIMIT  */
    LOAD = 298,                    /* LOAD  */
    SET_OFF = 299,                 /* SET_OFF  */
    PRIMARY = 300,                 /* PRIMARY  */
    KEY = 301,                     /* KEY  */
    LEFT = 302,                    /* LEFT  */
    RIGHT = 303,                   /* RIGHT  */
    INNER = 304,                   /* INNER  */
    OUTER = 305,                   /* OUTER  */
    CROSS = 306,                   /* CROSS  */
    ON = 307,                      /* ON  */
    LEQ = 308,                     /* LEQ  */
    NEQ = 309,                     /* NEQ  */
    GEQ = 310,                     /* GEQ  */
    T_EOF = 311,                   /* T_EOF  */
    IDENTIFIER = 312,              /* IDENTIFIER  */
    VALUE_STRING = 313,            /* VALUE_STRING  */
    FILE_PATH = 314,               /* FILE_PATH  */
    VALUE_INT = 315,               /* VALUE_INT  */
    VALUE_BIG_INT = 316,           /* VALUE_BIG_INT  */
    VALUE_FLOAT = 317,             /* VALUE_FLOAT  */
    VALUE_DATETIME = 318           /* VALUE_DATETIME  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (void);


#endif /* !YY_YY_MNT_E_CODE_PROJECT_RUCBASE_LAB_SRC_PARSER_YACC_TAB_H_INCLUDED  */
