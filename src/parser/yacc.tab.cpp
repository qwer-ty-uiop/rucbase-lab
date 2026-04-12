/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"

#include "ast.h"
#include "yacc.tab.h"
#include <iostream>
#include <memory>

int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

void yyerror(YYLTYPE *locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column " << locp->first_column << ": " << s << std::endl;
}

using namespace ast;

#line 86 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SHOW = 3,                       /* SHOW  */
  YYSYMBOL_TABLES = 4,                     /* TABLES  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_DROP = 7,                       /* DROP  */
  YYSYMBOL_DESC = 8,                       /* DESC  */
  YYSYMBOL_INSERT = 9,                     /* INSERT  */
  YYSYMBOL_INTO = 10,                      /* INTO  */
  YYSYMBOL_VALUES = 11,                    /* VALUES  */
  YYSYMBOL_DELETE = 12,                    /* DELETE  */
  YYSYMBOL_FROM = 13,                      /* FROM  */
  YYSYMBOL_ASC = 14,                       /* ASC  */
  YYSYMBOL_ORDER = 15,                     /* ORDER  */
  YYSYMBOL_BY = 16,                        /* BY  */
  YYSYMBOL_WHERE = 17,                     /* WHERE  */
  YYSYMBOL_UPDATE = 18,                    /* UPDATE  */
  YYSYMBOL_SET = 19,                       /* SET  */
  YYSYMBOL_SELECT = 20,                    /* SELECT  */
  YYSYMBOL_INT = 21,                       /* INT  */
  YYSYMBOL_BIG_INT = 22,                   /* BIG_INT  */
  YYSYMBOL_CHAR = 23,                      /* CHAR  */
  YYSYMBOL_VARCHAR = 24,                   /* VARCHAR  */
  YYSYMBOL_FLOAT = 25,                     /* FLOAT  */
  YYSYMBOL_DATETIME = 26,                  /* DATETIME  */
  YYSYMBOL_INDEX = 27,                     /* INDEX  */
  YYSYMBOL_AND = 28,                       /* AND  */
  YYSYMBOL_JOIN = 29,                      /* JOIN  */
  YYSYMBOL_EXIT = 30,                      /* EXIT  */
  YYSYMBOL_HELP = 31,                      /* HELP  */
  YYSYMBOL_TXN_BEGIN = 32,                 /* TXN_BEGIN  */
  YYSYMBOL_TXN_COMMIT = 33,                /* TXN_COMMIT  */
  YYSYMBOL_TXN_ABORT = 34,                 /* TXN_ABORT  */
  YYSYMBOL_TXN_ROLLBACK = 35,              /* TXN_ROLLBACK  */
  YYSYMBOL_ORDER_BY = 36,                  /* ORDER_BY  */
  YYSYMBOL_AS = 37,                        /* AS  */
  YYSYMBOL_SUM = 38,                       /* SUM  */
  YYSYMBOL_MAX = 39,                       /* MAX  */
  YYSYMBOL_MIN = 40,                       /* MIN  */
  YYSYMBOL_COUNT = 41,                     /* COUNT  */
  YYSYMBOL_LIMIT = 42,                     /* LIMIT  */
  YYSYMBOL_LOAD = 43,                      /* LOAD  */
  YYSYMBOL_SET_OFF = 44,                   /* SET_OFF  */
  YYSYMBOL_PRIMARY = 45,                   /* PRIMARY  */
  YYSYMBOL_KEY = 46,                       /* KEY  */
  YYSYMBOL_LEFT = 47,                      /* LEFT  */
  YYSYMBOL_RIGHT = 48,                     /* RIGHT  */
  YYSYMBOL_INNER = 49,                     /* INNER  */
  YYSYMBOL_OUTER = 50,                     /* OUTER  */
  YYSYMBOL_CROSS = 51,                     /* CROSS  */
  YYSYMBOL_ON = 52,                        /* ON  */
  YYSYMBOL_LEQ = 53,                       /* LEQ  */
  YYSYMBOL_NEQ = 54,                       /* NEQ  */
  YYSYMBOL_GEQ = 55,                       /* GEQ  */
  YYSYMBOL_T_EOF = 56,                     /* T_EOF  */
  YYSYMBOL_IDENTIFIER = 57,                /* IDENTIFIER  */
  YYSYMBOL_VALUE_STRING = 58,              /* VALUE_STRING  */
  YYSYMBOL_FILE_PATH = 59,                 /* FILE_PATH  */
  YYSYMBOL_VALUE_INT = 60,                 /* VALUE_INT  */
  YYSYMBOL_VALUE_BIG_INT = 61,             /* VALUE_BIG_INT  */
  YYSYMBOL_VALUE_FLOAT = 62,               /* VALUE_FLOAT  */
  YYSYMBOL_VALUE_DATETIME = 63,            /* VALUE_DATETIME  */
  YYSYMBOL_64_ = 64,                       /* ';'  */
  YYSYMBOL_65_ = 65,                       /* '('  */
  YYSYMBOL_66_ = 66,                       /* ')'  */
  YYSYMBOL_67_ = 67,                       /* ','  */
  YYSYMBOL_68_ = 68,                       /* '.'  */
  YYSYMBOL_69_ = 69,                       /* '='  */
  YYSYMBOL_70_ = 70,                       /* '<'  */
  YYSYMBOL_71_ = 71,                       /* '>'  */
  YYSYMBOL_72_ = 72,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 73,                  /* $accept  */
  YYSYMBOL_start = 74,                     /* start  */
  YYSYMBOL_stmt = 75,                      /* stmt  */
  YYSYMBOL_txnStmt = 76,                   /* txnStmt  */
  YYSYMBOL_dbStmt = 77,                    /* dbStmt  */
  YYSYMBOL_ddl = 78,                       /* ddl  */
  YYSYMBOL_dml = 79,                       /* dml  */
  YYSYMBOL_fieldList = 80,                 /* fieldList  */
  YYSYMBOL_colNameList = 81,               /* colNameList  */
  YYSYMBOL_field = 82,                     /* field  */
  YYSYMBOL_type = 83,                      /* type  */
  YYSYMBOL_valueList = 84,                 /* valueList  */
  YYSYMBOL_value = 85,                     /* value  */
  YYSYMBOL_condition = 86,                 /* condition  */
  YYSYMBOL_optWhereClause = 87,            /* optWhereClause  */
  YYSYMBOL_whereClause = 88,               /* whereClause  */
  YYSYMBOL_col = 89,                       /* col  */
  YYSYMBOL_colList = 90,                   /* colList  */
  YYSYMBOL_op = 91,                        /* op  */
  YYSYMBOL_expr = 92,                      /* expr  */
  YYSYMBOL_setClauses = 93,                /* setClauses  */
  YYSYMBOL_setClause = 94,                 /* setClause  */
  YYSYMBOL_optAggFunc = 95,                /* optAggFunc  */
  YYSYMBOL_optAggFuncList = 96,            /* optAggFuncList  */
  YYSYMBOL_selector = 97,                  /* selector  */
  YYSYMBOL_tableList = 98,                 /* tableList  */
  YYSYMBOL_joinClause = 99,                /* joinClause  */
  YYSYMBOL_fromClause = 100,               /* fromClause  */
  YYSYMBOL_opt_asc_desc = 101,             /* opt_asc_desc  */
  YYSYMBOL_order_clause = 102,             /* order_clause  */
  YYSYMBOL_order_clause_list = 103,        /* order_clause_list  */
  YYSYMBOL_opt_order_clause = 104,         /* opt_order_clause  */
  YYSYMBOL_limitClause = 105,              /* limitClause  */
  YYSYMBOL_tbName = 106,                   /* tbName  */
  YYSYMBOL_colName = 107,                  /* colName  */
  YYSYMBOL_filePath = 108                  /* filePath  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  43
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   205

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  106
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  217

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   318


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      65,    66,    72,     2,    67,     2,    68,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    64,
      70,    69,    71,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    64,    64,    69,    74,    79,    87,    88,    89,    90,
      91,    98,   102,   106,   110,   117,   124,   128,   132,   136,
     140,   144,   151,   155,   159,   163,   170,   174,   181,   185,
     192,   196,   203,   207,   211,   215,   219,   223,   230,   234,
     241,   245,   249,   253,   257,   264,   271,   272,   279,   283,
     290,   294,   301,   305,   312,   316,   320,   324,   328,   332,
     339,   343,   350,   354,   361,   365,   372,   376,   380,   384,
     388,   392,   396,   400,   407,   411,   415,   419,   424,   429,
     438,   442,   446,   450,   454,   458,   462,   466,   470,   474,
     478,   482,   486,   493,   500,   504,   509,   515,   522,   526,
     533,   537,   541,   545,   548,   550,   552
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SHOW", "TABLES",
  "CREATE", "TABLE", "DROP", "DESC", "INSERT", "INTO", "VALUES", "DELETE",
  "FROM", "ASC", "ORDER", "BY", "WHERE", "UPDATE", "SET", "SELECT", "INT",
  "BIG_INT", "CHAR", "VARCHAR", "FLOAT", "DATETIME", "INDEX", "AND",
  "JOIN", "EXIT", "HELP", "TXN_BEGIN", "TXN_COMMIT", "TXN_ABORT",
  "TXN_ROLLBACK", "ORDER_BY", "AS", "SUM", "MAX", "MIN", "COUNT", "LIMIT",
  "LOAD", "SET_OFF", "PRIMARY", "KEY", "LEFT", "RIGHT", "INNER", "OUTER",
  "CROSS", "ON", "LEQ", "NEQ", "GEQ", "T_EOF", "IDENTIFIER",
  "VALUE_STRING", "FILE_PATH", "VALUE_INT", "VALUE_BIG_INT", "VALUE_FLOAT",
  "VALUE_DATETIME", "';'", "'('", "')'", "','", "'.'", "'='", "'<'", "'>'",
  "'*'", "$accept", "start", "stmt", "txnStmt", "dbStmt", "ddl", "dml",
  "fieldList", "colNameList", "field", "type", "valueList", "value",
  "condition", "optWhereClause", "whereClause", "col", "colList", "op",
  "expr", "setClauses", "setClause", "optAggFunc", "optAggFuncList",
  "selector", "tableList", "joinClause", "fromClause", "opt_asc_desc",
  "order_clause", "order_clause_list", "opt_order_clause", "limitClause",
  "tbName", "colName", "filePath", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-120)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-105)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     118,     7,     9,    12,   -43,    31,     4,   -43,   -45,  -120,
    -120,  -120,  -120,  -120,  -120,   -37,  -120,    44,   -17,  -120,
    -120,  -120,  -120,  -120,    42,   -43,   -43,   -43,   -43,  -120,
    -120,   -43,   -43,    43,    -3,  -120,  -120,    11,   105,    14,
    -120,  -120,    70,  -120,  -120,   -43,    21,    23,  -120,    27,
      86,    87,    63,    67,    74,    75,    76,    82,  -120,   -10,
      63,   -43,  -120,    63,    63,    63,    89,    67,  -120,  -120,
     -11,  -120,    59,  -120,    67,    67,    67,     1,   -43,   105,
    -120,  -120,   -57,  -120,    88,   -13,  -120,     5,    71,  -120,
     107,    30,    63,  -120,    33,    90,    91,    92,    93,    94,
     -16,    87,  -120,  -120,  -120,    63,  -120,  -120,    97,    99,
    -120,  -120,   110,  -120,    63,  -120,  -120,  -120,  -120,  -120,
    -120,     8,  -120,    67,  -120,  -120,  -120,  -120,  -120,  -120,
      45,  -120,  -120,    71,   129,   132,   133,   134,   135,   -43,
      -1,    13,   144,   146,   -43,  -120,   161,  -120,   117,   119,
     138,  -120,  -120,    71,  -120,  -120,  -120,  -120,  -120,    63,
      63,    63,    63,    63,   126,   -43,   151,   -43,   152,   -43,
     -43,  -120,   170,   145,   122,   123,  -120,  -120,  -120,  -120,
    -120,  -120,  -120,    67,   139,   -43,   140,   -43,   141,  -120,
      67,    71,  -120,  -120,  -120,  -120,    67,   142,    67,   143,
      67,    38,  -120,   130,  -120,  -120,    67,  -120,    67,  -120,
    -120,  -120,  -120,    67,  -120,  -120,  -120
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,    76,     4,
       3,    11,    12,    13,    14,     0,     5,     0,     0,     9,
       6,     7,     8,    15,     0,     0,     0,     0,     0,   104,
      18,     0,     0,     0,   105,    74,    52,    75,    71,     0,
      51,   106,     0,     1,     2,     0,     0,     0,    17,     0,
       0,    46,     0,     0,     0,     0,     0,     0,    72,     0,
       0,     0,    21,     0,     0,     0,     0,     0,    23,   105,
      46,    62,     0,    53,     0,     0,     0,     0,     0,    71,
      50,    10,     0,    26,     0,     0,    28,     0,     0,    48,
      47,     0,     0,    24,     0,     0,     0,     0,     0,     0,
      93,    46,    77,    73,    16,     0,    32,    33,     0,     0,
      36,    37,    30,    19,     0,    20,    43,    40,    41,    42,
      44,     0,    38,     0,    58,    57,    59,    54,    55,    56,
       0,    63,    64,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    79,   101,    27,     0,     0,
       0,    29,    22,     0,    49,    60,    61,    45,    65,     0,
       0,     0,     0,     0,    81,     0,     0,     0,     0,     0,
       0,    78,     0,   103,     0,     0,    31,    39,    66,    67,
      68,    70,    69,     0,    86,     0,    90,     0,    83,    84,
       0,     0,    25,    34,    35,    80,     0,    88,     0,    92,
       0,    96,    98,   100,   102,    85,     0,    89,     0,    82,
      95,    94,    97,     0,    87,    91,    99
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -120,  -120,  -120,  -120,  -120,  -120,  -120,  -120,   125,    95,
    -120,  -120,   -93,  -119,   -63,  -120,    -8,  -120,  -120,  -120,
    -120,   104,   120,  -120,  -120,  -120,  -120,  -120,  -120,   -15,
    -120,  -120,  -120,    -2,   -44,  -120
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    17,    18,    19,    20,    21,    22,    82,    85,    83,
     112,   121,   122,    89,    68,    90,    91,    37,   130,   157,
      70,    71,    58,    59,    38,   100,   145,   101,   212,   202,
     203,   173,   192,    39,    40,    42
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,   132,    30,    78,   154,    33,    67,    93,    72,   104,
     105,    23,    34,   139,    29,    25,    80,    32,    27,    84,
      86,    86,    41,    46,    47,    48,    49,    35,   165,    50,
      51,   140,   141,   142,    24,   143,    26,   155,   146,    28,
     158,    31,   167,    62,    43,    73,   210,    44,    72,   166,
     133,   144,   211,   113,   114,    45,    92,    79,    34,    81,
     177,    84,    52,   168,   195,  -104,    95,    96,    97,    99,
     151,   115,   114,    98,   152,   153,   102,   205,    53,   207,
      61,   209,    60,   124,   125,   126,    63,   214,    64,   215,
      69,   116,    65,   117,   118,   119,   120,    66,   204,   127,
     128,   129,    34,   116,    67,   117,   118,   119,   120,   106,
     107,   108,   109,   110,   111,   178,   179,   180,   181,   182,
      69,     1,   156,     2,    34,     3,     4,     5,    94,   116,
       6,   117,   118,   119,   120,   123,     7,   164,     8,    74,
      75,    76,   171,    54,    55,    56,    57,    77,     9,    10,
      11,    12,    13,    14,    88,   150,   134,   135,   136,   137,
     138,    15,   148,   184,   149,   186,   159,   188,   189,   160,
     161,   162,   163,   169,    16,   170,   172,   174,   183,   175,
     185,   187,   201,   197,   176,   199,   190,   191,   193,   194,
      87,   196,   198,   200,   206,   208,   131,   213,   216,   103,
     147,     0,     0,     0,     0,   201
};

static const yytype_int16 yycheck[] =
{
       8,    94,     4,    13,   123,     7,    17,    70,    52,    66,
      67,     4,    57,    29,    57,     6,    60,    13,     6,    63,
      64,    65,    59,    25,    26,    27,    28,    72,    29,    31,
      32,    47,    48,    49,    27,    51,    27,   130,   101,    27,
     133,    10,    29,    45,     0,    53,     8,    64,    92,    50,
      94,    67,    14,    66,    67,    13,    67,    67,    57,    61,
     153,   105,    19,    50,   183,    68,    74,    75,    76,    77,
     114,    66,    67,    72,    66,    67,    78,   196,    67,   198,
      10,   200,    68,    53,    54,    55,    65,   206,    65,   208,
      57,    58,    65,    60,    61,    62,    63,    11,   191,    69,
      70,    71,    57,    58,    17,    60,    61,    62,    63,    21,
      22,    23,    24,    25,    26,   159,   160,   161,   162,   163,
      57,     3,   130,     5,    57,     7,     8,     9,    69,    58,
      12,    60,    61,    62,    63,    28,    18,   139,    20,    65,
      65,    65,   144,    38,    39,    40,    41,    65,    30,    31,
      32,    33,    34,    35,    65,    45,    66,    66,    66,    66,
      66,    43,    65,   165,    65,   167,    37,   169,   170,    37,
      37,    37,    37,    29,    56,    29,    15,    60,    52,    60,
      29,    29,   190,   185,    46,   187,    16,    42,    66,    66,
      65,    52,    52,    52,    52,    52,    92,    67,   213,    79,
     105,    -1,    -1,    -1,    -1,   213
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,     7,     8,     9,    12,    18,    20,    30,
      31,    32,    33,    34,    35,    43,    56,    74,    75,    76,
      77,    78,    79,     4,    27,     6,    27,     6,    27,    57,
     106,    10,    13,   106,    57,    72,    89,    90,    97,   106,
     107,    59,   108,     0,    64,    13,   106,   106,   106,   106,
     106,   106,    19,    67,    38,    39,    40,    41,    95,    96,
      68,    10,   106,    65,    65,    65,    11,    17,    87,    57,
      93,    94,   107,    89,    65,    65,    65,    65,    13,    67,
     107,   106,    80,    82,   107,    81,   107,    81,    65,    86,
      88,    89,    67,    87,    69,    89,    89,    89,    72,    89,
      98,   100,   106,    95,    66,    67,    21,    22,    23,    24,
      25,    26,    83,    66,    67,    66,    58,    60,    61,    62,
      63,    84,    85,    28,    53,    54,    55,    69,    70,    71,
      91,    94,    85,   107,    66,    66,    66,    66,    66,    29,
      47,    48,    49,    51,    67,    99,    87,    82,    65,    65,
      45,   107,    66,    67,    86,    85,    89,    92,    85,    37,
      37,    37,    37,    37,   106,    29,    50,    29,    50,    29,
      29,   106,    15,   104,    60,    60,    46,    85,   107,   107,
     107,   107,   107,    52,   106,    29,   106,    29,   106,   106,
      16,    42,   105,    66,    66,    86,    52,   106,    52,   106,
      52,    89,   102,   103,    85,    86,    52,    86,    52,    86,
       8,    14,   101,    67,    86,    86,   102
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    73,    74,    74,    74,    74,    75,    75,    75,    75,
      75,    76,    76,    76,    76,    77,    78,    78,    78,    78,
      78,    78,    79,    79,    79,    79,    80,    80,    81,    81,
      82,    82,    83,    83,    83,    83,    83,    83,    84,    84,
      85,    85,    85,    85,    85,    86,    87,    87,    88,    88,
      89,    89,    90,    90,    91,    91,    91,    91,    91,    91,
      92,    92,    93,    93,    94,    94,    95,    95,    95,    95,
      95,    95,    96,    96,    97,    97,    97,    98,    98,    98,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,   100,   101,   101,   101,   102,   103,   103,
     104,   104,   105,   105,   106,   107,   108
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     1,     1,     1,     2,     6,     3,     2,     6,
       6,     4,     7,     4,     5,     8,     1,     3,     1,     3,
       2,     4,     1,     1,     4,     4,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     3,     0,     2,     1,     3,
       3,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     4,     6,     6,     6,     6,
       6,     0,     1,     3,     1,     1,     0,     1,     3,     2,
       4,     2,     5,     3,     3,     5,     3,     6,     4,     5,
       3,     6,     4,     1,     1,     1,     0,     2,     1,     3,
       3,     0,     2,     0,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: stmt ';'  */
#line 65 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        parse_tree = (yyvsp[-1].sv_node);
        YYACCEPT;
    }
#line 1725 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 3: /* start: HELP  */
#line 70 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        parse_tree = std::make_shared<Help>();
        YYACCEPT;
    }
#line 1734 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 4: /* start: EXIT  */
#line 75 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
#line 1743 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 5: /* start: T_EOF  */
#line 80 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
#line 1752 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 10: /* stmt: LOAD filePath INTO tbName  */
#line 92 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<LoadTable>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 1760 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 11: /* txnStmt: TXN_BEGIN  */
#line 99 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnBegin>();
    }
#line 1768 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 12: /* txnStmt: TXN_COMMIT  */
#line 103 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnCommit>();
    }
#line 1776 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 13: /* txnStmt: TXN_ABORT  */
#line 107 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnAbort>();
    }
#line 1784 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 14: /* txnStmt: TXN_ROLLBACK  */
#line 111 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnRollback>();
    }
#line 1792 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 15: /* dbStmt: SHOW TABLES  */
#line 118 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<ShowTables>();
    }
#line 1800 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 16: /* ddl: CREATE TABLE tbName '(' fieldList ')'  */
#line 125 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateTable>((yyvsp[-3].sv_str), (yyvsp[-1].sv_fields));
    }
#line 1808 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 17: /* ddl: DROP TABLE tbName  */
#line 129 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropTable>((yyvsp[0].sv_str));
    }
#line 1816 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 18: /* ddl: DESC tbName  */
#line 133 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DescTable>((yyvsp[0].sv_str));
    }
#line 1824 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 19: /* ddl: CREATE INDEX tbName '(' colNameList ')'  */
#line 137 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1832 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 20: /* ddl: DROP INDEX tbName '(' colNameList ')'  */
#line 141 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1840 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 21: /* ddl: SHOW INDEX FROM tbName  */
#line 145 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<ShowIndex>((yyvsp[0].sv_str));
    }
#line 1848 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 22: /* dml: INSERT INTO tbName VALUES '(' valueList ')'  */
#line 152 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<InsertStmt>((yyvsp[-4].sv_str), (yyvsp[-1].sv_vals));
    }
#line 1856 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 23: /* dml: DELETE FROM tbName optWhereClause  */
#line 156 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DeleteStmt>((yyvsp[-1].sv_str), (yyvsp[0].sv_conds));
    }
#line 1864 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 24: /* dml: UPDATE tbName SET setClauses optWhereClause  */
#line 160 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<UpdateStmt>((yyvsp[-3].sv_str), (yyvsp[-1].sv_set_clauses), (yyvsp[0].sv_conds));
    }
#line 1872 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 25: /* dml: SELECT selector optAggFuncList FROM fromClause optWhereClause opt_order_clause limitClause  */
#line 164 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_node) = std::make_shared<SelectStmt>((yyvsp[-6].sv_cols), (yyvsp[-5].sv_agg_funcs), (yyvsp[-3].sv_from_clause)->tabs, (yyvsp[-2].sv_conds), (yyvsp[-3].sv_from_clause)->jointree, (yyvsp[-1].sv_orderbys), (yyvsp[0].sv_val));
    }
#line 1880 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 26: /* fieldList: field  */
#line 171 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_fields) = std::vector<std::shared_ptr<Field>>{(yyvsp[0].sv_field)};
    }
#line 1888 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 27: /* fieldList: fieldList ',' field  */
#line 175 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_fields).push_back((yyvsp[0].sv_field));
    }
#line 1896 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 28: /* colNameList: colName  */
#line 182 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 1904 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 29: /* colNameList: colNameList ',' colName  */
#line 186 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 1912 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 30: /* field: colName type  */
#line 193 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-1].sv_str), (yyvsp[0].sv_type_len));
    }
#line 1920 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 31: /* field: colName type PRIMARY KEY  */
#line 197 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-3].sv_str), (yyvsp[-2].sv_type_len), true);
    }
#line 1928 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 32: /* type: INT  */
#line 204 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
    }
#line 1936 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 33: /* type: BIG_INT  */
#line 208 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_BIG_INT, sizeof(long long int));
    }
#line 1944 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 34: /* type: CHAR '(' VALUE_INT ')'  */
#line 212 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
    }
#line 1952 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 35: /* type: VARCHAR '(' VALUE_INT ')'  */
#line 216 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
    }
#line 1960 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 36: /* type: FLOAT  */
#line 220 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(float));
    }
#line 1968 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 37: /* type: DATETIME  */
#line 224 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_DATETIME, 19);
    }
#line 1976 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 38: /* valueList: value  */
#line 231 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_vals) = std::vector<std::shared_ptr<ast::Value>>{(yyvsp[0].sv_val)};
    }
#line 1984 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 39: /* valueList: valueList ',' value  */
#line 235 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_vals).push_back((yyvsp[0].sv_val));
    }
#line 1992 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 40: /* value: VALUE_INT  */
#line 242 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<IntLit>((yyvsp[0].sv_int));
    }
#line 2000 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 41: /* value: VALUE_BIG_INT  */
#line 246 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<BigIntLit>((yyvsp[0].sv_big_int));
    }
#line 2008 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 42: /* value: VALUE_FLOAT  */
#line 250 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<FloatLit>((yyvsp[0].sv_float));
    }
#line 2016 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 43: /* value: VALUE_STRING  */
#line 254 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<StringLit>((yyvsp[0].sv_str));
    }
#line 2024 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 44: /* value: VALUE_DATETIME  */
#line 258 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_val) = std::make_shared<DatetimeLit>((yyvsp[0].sv_datetime));
    }
#line 2032 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 45: /* condition: col op expr  */
#line 265 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_cond) = std::make_shared<BinaryExpr>((yyvsp[-2].sv_col), (yyvsp[-1].sv_comp_op), (yyvsp[0].sv_expr));
    }
#line 2040 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 46: /* optWhereClause: %empty  */
#line 271 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2046 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 47: /* optWhereClause: WHERE whereClause  */
#line 273 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_conds) = (yyvsp[0].sv_conds);
    }
#line 2054 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 48: /* whereClause: condition  */
#line 280 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_conds) = std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
    }
#line 2062 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 49: /* whereClause: whereClause AND condition  */
#line 284 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
    }
#line 2070 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 50: /* col: tbName '.' colName  */
#line 291 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 2078 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 51: /* col: colName  */
#line 295 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[0].sv_str));
    }
#line 2086 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 52: /* colList: col  */
#line 302 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_cols) = std::vector<std::shared_ptr<Col>>{(yyvsp[0].sv_col)};
    }
#line 2094 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 53: /* colList: colList ',' col  */
#line 306 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_cols).push_back((yyvsp[0].sv_col));
    }
#line 2102 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 54: /* op: '='  */
#line 313 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_EQ;
    }
#line 2110 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 55: /* op: '<'  */
#line 317 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LT;
    }
#line 2118 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 56: /* op: '>'  */
#line 321 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GT;
    }
#line 2126 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 57: /* op: NEQ  */
#line 325 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_NE;
    }
#line 2134 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 58: /* op: LEQ  */
#line 329 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LE;
    }
#line 2142 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 59: /* op: GEQ  */
#line 333 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GE;
    }
#line 2150 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 60: /* expr: value  */
#line 340 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_val));
    }
#line 2158 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 61: /* expr: col  */
#line 344 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_col));
    }
#line 2166 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 62: /* setClauses: setClause  */
#line 351 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_set_clauses) = std::vector<std::shared_ptr<ast::SetClause>>{(yyvsp[0].sv_set_clause)};
    }
#line 2174 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 63: /* setClauses: setClauses ',' setClause  */
#line 355 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_set_clauses).push_back((yyvsp[0].sv_set_clause));
    }
#line 2182 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 64: /* setClause: colName '=' value  */
#line 362 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_set_clause) = std::make_shared<ast::SetClause>((yyvsp[-2].sv_str), (yyvsp[0].sv_val));
    }
#line 2190 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 65: /* setClause: colName '=' colName value  */
#line 366 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_set_clause) = std::make_shared<ast::SetClause>((yyvsp[-3].sv_str), (yyvsp[0].sv_val), true);
    }
#line 2198 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 66: /* optAggFunc: SUM '(' col ')' AS colName  */
#line 373 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<ast::AggFunc>("SUM", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2206 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 67: /* optAggFunc: MAX '(' col ')' AS colName  */
#line 377 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<ast::AggFunc>("MAX", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2214 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 68: /* optAggFunc: MIN '(' col ')' AS colName  */
#line 381 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<ast::AggFunc>("MIN", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2222 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 69: /* optAggFunc: COUNT '(' col ')' AS colName  */
#line 385 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<ast::AggFunc>("COUNT", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2230 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 70: /* optAggFunc: COUNT '(' '*' ')' AS colName  */
#line 389 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<ast::AggFunc>("COUNT", nullptr, (yyvsp[0].sv_str));
    }
#line 2238 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 71: /* optAggFunc: %empty  */
#line 392 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2244 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 72: /* optAggFuncList: optAggFunc  */
#line 397 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_agg_funcs) = std::vector<std::shared_ptr<ast::AggFunc>>{(yyvsp[0].sv_agg_func)};
    }
#line 2252 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 73: /* optAggFuncList: optAggFuncList ',' optAggFunc  */
#line 401 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_agg_funcs).push_back((yyvsp[0].sv_agg_func));
    }
#line 2260 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 74: /* selector: '*'  */
#line 408 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_cols) = {};
    }
#line 2268 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 75: /* selector: colList  */
#line 412 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {

    }
#line 2276 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 76: /* selector: %empty  */
#line 415 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2282 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 77: /* tableList: tbName  */
#line 420 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_from_clause) = std::make_shared<FromClause>();
        (yyval.sv_from_clause)->tabs.push_back((yyvsp[0].sv_str));
    }
#line 2291 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 78: /* tableList: tableList ',' tbName  */
#line 425 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_from_clause) = (yyvsp[-2].sv_from_clause);
        (yyval.sv_from_clause)->tabs.push_back((yyvsp[0].sv_str));
    }
#line 2300 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 79: /* tableList: tableList joinClause  */
#line 430 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_from_clause) = (yyvsp[-1].sv_from_clause);
        (yyval.sv_from_clause)->tabs.push_back((yyvsp[0].sv_join_expr)->right);
        (yyval.sv_from_clause)->jointree.push_back((yyvsp[0].sv_join_expr));
    }
#line 2310 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 80: /* joinClause: JOIN tbName ON condition  */
#line 439 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[-2].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)}, JoinType::INNER_JOIN);
    }
#line 2318 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 81: /* joinClause: JOIN tbName  */
#line 443 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[0].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::INNER_JOIN);
    }
#line 2326 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 82: /* joinClause: INNER JOIN tbName ON condition  */
#line 447 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[-2].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)}, JoinType::INNER_JOIN);
    }
#line 2334 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 83: /* joinClause: INNER JOIN tbName  */
#line 451 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[0].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::INNER_JOIN);
    }
#line 2342 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 84: /* joinClause: CROSS JOIN tbName  */
#line 455 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[0].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::INNER_JOIN);
    }
#line 2350 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 85: /* joinClause: LEFT JOIN tbName ON condition  */
#line 459 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[-2].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)}, JoinType::LEFT_JOIN);
    }
#line 2358 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 86: /* joinClause: LEFT JOIN tbName  */
#line 463 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[0].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::LEFT_JOIN);
    }
#line 2366 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 87: /* joinClause: LEFT OUTER JOIN tbName ON condition  */
#line 467 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[-2].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)}, JoinType::LEFT_JOIN);
    }
#line 2374 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 88: /* joinClause: LEFT OUTER JOIN tbName  */
#line 471 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[0].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::LEFT_JOIN);
    }
#line 2382 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 89: /* joinClause: RIGHT JOIN tbName ON condition  */
#line 475 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[-2].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)}, JoinType::RIGHT_JOIN);
    }
#line 2390 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 90: /* joinClause: RIGHT JOIN tbName  */
#line 479 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[0].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::RIGHT_JOIN);
    }
#line 2398 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 91: /* joinClause: RIGHT OUTER JOIN tbName ON condition  */
#line 483 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[-2].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)}, JoinType::RIGHT_JOIN);
    }
#line 2406 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 92: /* joinClause: RIGHT OUTER JOIN tbName  */
#line 487 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_join_expr) = std::make_shared<JoinExpr>("", (yyvsp[0].sv_str), std::vector<std::shared_ptr<BinaryExpr>>{}, JoinType::RIGHT_JOIN);
    }
#line 2414 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 93: /* fromClause: tableList  */
#line 494 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_from_clause) = (yyvsp[0].sv_from_clause);
    }
#line 2422 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 94: /* opt_asc_desc: ASC  */
#line 501 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    { 
        (yyval.sv_orderby_dir) = OrderBy_ASC;     
    }
#line 2430 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 95: /* opt_asc_desc: DESC  */
#line 505 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    { 
        (yyval.sv_orderby_dir) = OrderBy_DESC;    
    }
#line 2438 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 96: /* opt_asc_desc: %empty  */
#line 509 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    { 
        (yyval.sv_orderby_dir) = OrderBy_DEFAULT; 
    }
#line 2446 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 97: /* order_clause: col opt_asc_desc  */
#line 516 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    { 
        (yyval.sv_orderby) = std::make_shared<OrderBy>((yyvsp[-1].sv_col), (yyvsp[0].sv_orderby_dir));
    }
#line 2454 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 98: /* order_clause_list: order_clause  */
#line 523 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_orderbys) = std::vector<std::shared_ptr<OrderBy>>{(yyvsp[0].sv_orderby)};
    }
#line 2462 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 99: /* order_clause_list: order_clause_list ',' order_clause  */
#line 527 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_orderbys).push_back((yyvsp[0].sv_orderby));
    }
#line 2470 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 100: /* opt_order_clause: ORDER BY order_clause_list  */
#line 534 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    { 
        (yyval.sv_orderbys) = (yyvsp[0].sv_orderbys);
    }
#line 2478 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 101: /* opt_order_clause: %empty  */
#line 537 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2484 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 102: /* limitClause: LIMIT value  */
#line 542 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
    {
        (yyval.sv_val) = (yyvsp[0].sv_val);
    }
#line 2492 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;

  case 103: /* limitClause: %empty  */
#line 545 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"
                      { /* ignore*/ }
#line 2498 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"
    break;


#line 2502 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 553 "/mnt/e/Code/Project/rucbase-lab/src/parser/yacc.y"


