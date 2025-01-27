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
#line 1 "yacc.y"

#include "ast.h"
#include "yacc.tab.h"
#include <iostream>
#include <memory>

int yylex(YYSTYPE *yylval, YYLTYPE *yylloc);

void yyerror(YYLTYPE *locp, const char* s) {
    std::cerr << "Parser Error at line " << locp->first_line << " column " << locp->first_column << ": " << s << std::endl;
}

using namespace ast;

#line 86 "yacc.tab.cpp"

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

#include "yacc.tab.hpp"
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
  YYSYMBOL_FLOAT = 24,                     /* FLOAT  */
  YYSYMBOL_DATETIME = 25,                  /* DATETIME  */
  YYSYMBOL_INDEX = 26,                     /* INDEX  */
  YYSYMBOL_AND = 27,                       /* AND  */
  YYSYMBOL_JOIN = 28,                      /* JOIN  */
  YYSYMBOL_EXIT = 29,                      /* EXIT  */
  YYSYMBOL_HELP = 30,                      /* HELP  */
  YYSYMBOL_TXN_BEGIN = 31,                 /* TXN_BEGIN  */
  YYSYMBOL_TXN_COMMIT = 32,                /* TXN_COMMIT  */
  YYSYMBOL_TXN_ABORT = 33,                 /* TXN_ABORT  */
  YYSYMBOL_TXN_ROLLBACK = 34,              /* TXN_ROLLBACK  */
  YYSYMBOL_ORDER_BY = 35,                  /* ORDER_BY  */
  YYSYMBOL_AS = 36,                        /* AS  */
  YYSYMBOL_SUM = 37,                       /* SUM  */
  YYSYMBOL_MAX = 38,                       /* MAX  */
  YYSYMBOL_MIN = 39,                       /* MIN  */
  YYSYMBOL_COUNT = 40,                     /* COUNT  */
  YYSYMBOL_LIMIT = 41,                     /* LIMIT  */
  YYSYMBOL_LOAD = 42,                      /* LOAD  */
  YYSYMBOL_SET_OFF = 43,                   /* SET_OFF  */
  YYSYMBOL_LEQ = 44,                       /* LEQ  */
  YYSYMBOL_NEQ = 45,                       /* NEQ  */
  YYSYMBOL_GEQ = 46,                       /* GEQ  */
  YYSYMBOL_T_EOF = 47,                     /* T_EOF  */
  YYSYMBOL_IDENTIFIER = 48,                /* IDENTIFIER  */
  YYSYMBOL_VALUE_STRING = 49,              /* VALUE_STRING  */
  YYSYMBOL_FILE_PATH = 50,                 /* FILE_PATH  */
  YYSYMBOL_VALUE_INT = 51,                 /* VALUE_INT  */
  YYSYMBOL_VALUE_BIG_INT = 52,             /* VALUE_BIG_INT  */
  YYSYMBOL_VALUE_FLOAT = 53,               /* VALUE_FLOAT  */
  YYSYMBOL_VALUE_DATETIME = 54,            /* VALUE_DATETIME  */
  YYSYMBOL_55_ = 55,                       /* ';'  */
  YYSYMBOL_56_ = 56,                       /* '('  */
  YYSYMBOL_57_ = 57,                       /* ')'  */
  YYSYMBOL_58_ = 58,                       /* ','  */
  YYSYMBOL_59_ = 59,                       /* '.'  */
  YYSYMBOL_60_ = 60,                       /* '='  */
  YYSYMBOL_61_ = 61,                       /* '<'  */
  YYSYMBOL_62_ = 62,                       /* '>'  */
  YYSYMBOL_63_ = 63,                       /* '*'  */
  YYSYMBOL_YYACCEPT = 64,                  /* $accept  */
  YYSYMBOL_start = 65,                     /* start  */
  YYSYMBOL_stmt = 66,                      /* stmt  */
  YYSYMBOL_txnStmt = 67,                   /* txnStmt  */
  YYSYMBOL_dbStmt = 68,                    /* dbStmt  */
  YYSYMBOL_ddl = 69,                       /* ddl  */
  YYSYMBOL_dml = 70,                       /* dml  */
  YYSYMBOL_fieldList = 71,                 /* fieldList  */
  YYSYMBOL_colNameList = 72,               /* colNameList  */
  YYSYMBOL_field = 73,                     /* field  */
  YYSYMBOL_type = 74,                      /* type  */
  YYSYMBOL_valueList = 75,                 /* valueList  */
  YYSYMBOL_value = 76,                     /* value  */
  YYSYMBOL_condition = 77,                 /* condition  */
  YYSYMBOL_optWhereClause = 78,            /* optWhereClause  */
  YYSYMBOL_whereClause = 79,               /* whereClause  */
  YYSYMBOL_col = 80,                       /* col  */
  YYSYMBOL_colList = 81,                   /* colList  */
  YYSYMBOL_op = 82,                        /* op  */
  YYSYMBOL_expr = 83,                      /* expr  */
  YYSYMBOL_setClauses = 84,                /* setClauses  */
  YYSYMBOL_setClause = 85,                 /* setClause  */
  YYSYMBOL_optAggFunc = 86,                /* optAggFunc  */
  YYSYMBOL_optAggFuncList = 87,            /* optAggFuncList  */
  YYSYMBOL_selector = 88,                  /* selector  */
  YYSYMBOL_tableList = 89,                 /* tableList  */
  YYSYMBOL_opt_asc_desc = 90,              /* opt_asc_desc  */
  YYSYMBOL_order_clause = 91,              /* order_clause  */
  YYSYMBOL_order_clause_list = 92,         /* order_clause_list  */
  YYSYMBOL_opt_order_clause = 93,          /* opt_order_clause  */
  YYSYMBOL_limitClause = 94,               /* limitClause  */
  YYSYMBOL_tbName = 95,                    /* tbName  */
  YYSYMBOL_colName = 96,                   /* colName  */
  YYSYMBOL_filePath = 97                   /* filePath  */
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
#define YYLAST   170

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  64
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  90
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  179

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


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
      56,    57,    63,     2,    58,     2,    59,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    55,
      61,    60,    62,     2,     2,     2,     2,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    62,    62,    67,    72,    77,    85,    86,    87,    88,
      89,    96,   100,   104,   108,   115,   122,   126,   130,   134,
     138,   142,   149,   153,   157,   161,   168,   172,   179,   183,
     190,   197,   201,   205,   209,   213,   220,   224,   231,   235,
     239,   243,   247,   254,   261,   262,   269,   273,   280,   284,
     291,   295,   302,   306,   310,   314,   318,   322,   329,   333,
     340,   344,   351,   355,   362,   366,   370,   374,   378,   382,
     386,   390,   397,   401,   405,   409,   413,   417,   424,   428,
     433,   439,   446,   450,   457,   461,   465,   469,   472,   474,
     476
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
  "BIG_INT", "CHAR", "FLOAT", "DATETIME", "INDEX", "AND", "JOIN", "EXIT",
  "HELP", "TXN_BEGIN", "TXN_COMMIT", "TXN_ABORT", "TXN_ROLLBACK",
  "ORDER_BY", "AS", "SUM", "MAX", "MIN", "COUNT", "LIMIT", "LOAD",
  "SET_OFF", "LEQ", "NEQ", "GEQ", "T_EOF", "IDENTIFIER", "VALUE_STRING",
  "FILE_PATH", "VALUE_INT", "VALUE_BIG_INT", "VALUE_FLOAT",
  "VALUE_DATETIME", "';'", "'('", "')'", "','", "'.'", "'='", "'<'", "'>'",
  "'*'", "$accept", "start", "stmt", "txnStmt", "dbStmt", "ddl", "dml",
  "fieldList", "colNameList", "field", "type", "valueList", "value",
  "condition", "optWhereClause", "whereClause", "col", "colList", "op",
  "expr", "setClauses", "setClause", "optAggFunc", "optAggFuncList",
  "selector", "tableList", "opt_asc_desc", "order_clause",
  "order_clause_list", "opt_order_clause", "limitClause", "tbName",
  "colName", "filePath", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-92)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-89)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      52,     4,     0,     6,   -38,     7,    14,   -38,   -32,   -92,
     -92,   -92,   -92,   -92,   -92,   -17,   -92,    36,   -16,   -92,
     -92,   -92,   -92,   -92,    28,   -38,   -38,   -38,   -38,   -92,
     -92,   -38,   -38,    24,   -15,   -92,   -92,    -4,   104,    12,
     -92,   -92,    46,   -92,   -92,   -38,     9,    21,   -92,    23,
      80,    75,    45,    47,    40,    41,    44,    59,   -92,   -11,
      45,   -38,   -92,    45,    45,    45,    66,    47,   -92,   -92,
      -6,   -92,    42,   -92,    47,    47,    47,   -29,   -38,   104,
     -92,   -92,   -44,   -92,   106,   -37,   -92,     5,    72,   -92,
      74,    43,    45,   -92,    58,    51,    76,    88,    89,    90,
     -10,   -92,   -92,   -92,    45,   -92,   -92,    92,   -92,   -92,
     -92,   -92,    45,   -92,   -92,   -92,   -92,   -92,   -92,    16,
     -92,    47,   -92,   -92,   -92,   -92,   -92,   -92,    65,   -92,
     -92,    72,    96,   113,   114,   115,   116,   -38,   -38,   138,
     -92,   103,   -92,   -92,    72,   -92,   -92,   -92,   -92,   -92,
      45,    45,    45,    45,    45,   -92,   -92,   139,   118,    99,
     -92,   -92,   -92,   -92,   -92,   -92,    47,    72,   -92,   -92,
       1,   -92,   102,   -92,   -92,   -92,   -92,    47,   -92
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,    74,     4,
       3,    11,    12,    13,    14,     0,     5,     0,     0,     9,
       6,     7,     8,    15,     0,     0,     0,     0,     0,    88,
      18,     0,     0,     0,    89,    72,    50,    73,    69,     0,
      49,    90,     0,     1,     2,     0,     0,     0,    17,     0,
       0,    44,     0,     0,     0,     0,     0,     0,    70,     0,
       0,     0,    21,     0,     0,     0,     0,     0,    23,    89,
      44,    60,     0,    51,     0,     0,     0,     0,     0,    69,
      48,    10,     0,    26,     0,     0,    28,     0,     0,    46,
      45,     0,     0,    24,     0,     0,     0,     0,     0,     0,
      44,    75,    71,    16,     0,    31,    32,     0,    34,    35,
      30,    19,     0,    20,    41,    38,    39,    40,    42,     0,
      36,     0,    56,    55,    57,    52,    53,    54,     0,    61,
      62,     0,     0,     0,     0,     0,     0,     0,     0,    85,
      27,     0,    29,    22,     0,    47,    58,    59,    43,    63,
       0,     0,     0,     0,     0,    77,    76,     0,    87,     0,
      37,    64,    65,    66,    68,    67,     0,     0,    25,    33,
      80,    82,    84,    86,    79,    78,    81,     0,    83
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -92,   -92,   -92,   -92,   -92,   -92,   -92,   -92,    97,    53,
     -92,   -92,   -91,    49,   -65,   -92,    -8,   -92,   -92,   -92,
     -92,    69,    84,   -92,   -92,   -92,   -92,   -13,   -92,   -92,
     -92,    -3,   -14,   -92
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    17,    18,    19,    20,    21,    22,    82,    85,    83,
     110,   119,   120,    89,    68,    90,    91,    37,   128,   148,
      70,    71,    58,    59,    38,   100,   176,   171,   172,   158,
     168,    39,    40,    42
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      36,    30,    78,   130,    33,    93,    25,    67,    23,   174,
      29,    67,    27,   103,   104,   175,    34,    31,   137,    34,
     111,   112,    46,    47,    48,    49,    26,    32,    50,    51,
      24,    35,    28,    41,    98,   139,    43,   146,    72,    44,
     149,    45,    62,    52,   -88,    73,    80,    79,   138,    84,
      86,    86,    92,   160,    53,     1,    61,     2,    81,     3,
       4,     5,   113,   112,     6,    63,    95,    96,    97,    99,
       7,    60,     8,   143,   144,   101,   173,    64,    72,    65,
     131,     9,    10,    11,    12,    13,    14,   122,   123,   124,
      84,    66,    67,    69,    15,    34,    74,    75,   142,    16,
      76,   121,    94,   125,   126,   127,    69,   114,   132,   115,
     116,   117,   118,    34,   114,    77,   115,   116,   117,   118,
     147,   114,    88,   115,   116,   117,   118,   105,   106,   107,
     108,   109,   150,   133,   155,   156,   161,   162,   163,   164,
     165,    54,    55,    56,    57,   134,   135,   136,   141,   151,
     152,   153,   154,   157,   159,   166,   169,   140,   170,   167,
     177,   129,    87,   102,   178,     0,     0,     0,     0,   170,
     145
};

static const yytype_int16 yycheck[] =
{
       8,     4,    13,    94,     7,    70,     6,    17,     4,     8,
      48,    17,     6,    57,    58,    14,    48,    10,    28,    48,
      57,    58,    25,    26,    27,    28,    26,    13,    31,    32,
      26,    63,    26,    50,    63,   100,     0,   128,    52,    55,
     131,    13,    45,    19,    59,    53,    60,    58,    58,    63,
      64,    65,    58,   144,    58,     3,    10,     5,    61,     7,
       8,     9,    57,    58,    12,    56,    74,    75,    76,    77,
      18,    59,    20,    57,    58,    78,   167,    56,    92,    56,
      94,    29,    30,    31,    32,    33,    34,    44,    45,    46,
     104,    11,    17,    48,    42,    48,    56,    56,   112,    47,
      56,    27,    60,    60,    61,    62,    48,    49,    57,    51,
      52,    53,    54,    48,    49,    56,    51,    52,    53,    54,
     128,    49,    56,    51,    52,    53,    54,    21,    22,    23,
      24,    25,    36,    57,   137,   138,   150,   151,   152,   153,
     154,    37,    38,    39,    40,    57,    57,    57,    56,    36,
      36,    36,    36,    15,    51,    16,    57,   104,   166,    41,
      58,    92,    65,    79,   177,    -1,    -1,    -1,    -1,   177,
     121
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     5,     7,     8,     9,    12,    18,    20,    29,
      30,    31,    32,    33,    34,    42,    47,    65,    66,    67,
      68,    69,    70,     4,    26,     6,    26,     6,    26,    48,
      95,    10,    13,    95,    48,    63,    80,    81,    88,    95,
      96,    50,    97,     0,    55,    13,    95,    95,    95,    95,
      95,    95,    19,    58,    37,    38,    39,    40,    86,    87,
      59,    10,    95,    56,    56,    56,    11,    17,    78,    48,
      84,    85,    96,    80,    56,    56,    56,    56,    13,    58,
      96,    95,    71,    73,    96,    72,    96,    72,    56,    77,
      79,    80,    58,    78,    60,    80,    80,    80,    63,    80,
      89,    95,    86,    57,    58,    21,    22,    23,    24,    25,
      74,    57,    58,    57,    49,    51,    52,    53,    54,    75,
      76,    27,    44,    45,    46,    60,    61,    62,    82,    85,
      76,    96,    57,    57,    57,    57,    57,    28,    58,    78,
      73,    56,    96,    57,    58,    77,    76,    80,    83,    76,
      36,    36,    36,    36,    36,    95,    95,    15,    93,    51,
      76,    96,    96,    96,    96,    96,    16,    41,    94,    57,
      80,    91,    92,    76,     8,    14,    90,    58,    91
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    64,    65,    65,    65,    65,    66,    66,    66,    66,
      66,    67,    67,    67,    67,    68,    69,    69,    69,    69,
      69,    69,    70,    70,    70,    70,    71,    71,    72,    72,
      73,    74,    74,    74,    74,    74,    75,    75,    76,    76,
      76,    76,    76,    77,    78,    78,    79,    79,    80,    80,
      81,    81,    82,    82,    82,    82,    82,    82,    83,    83,
      84,    84,    85,    85,    86,    86,    86,    86,    86,    86,
      87,    87,    88,    88,    88,    89,    89,    89,    90,    90,
      90,    91,    92,    92,    93,    93,    94,    94,    95,    96,
      97
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     1,     1,     1,     2,     6,     3,     2,     6,
       6,     4,     7,     4,     5,     8,     1,     3,     1,     3,
       2,     1,     1,     4,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     3,     0,     2,     1,     3,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     3,     4,     6,     6,     6,     6,     6,     0,
       1,     3,     1,     1,     0,     1,     3,     3,     1,     1,
       0,     2,     1,     3,     3,     0,     2,     0,     1,     1,
       1
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
#line 63 "yacc.y"
    {
        parse_tree = (yyvsp[-1].sv_node);
        YYACCEPT;
    }
#line 1691 "yacc.tab.cpp"
    break;

  case 3: /* start: HELP  */
#line 68 "yacc.y"
    {
        parse_tree = std::make_shared<Help>();
        YYACCEPT;
    }
#line 1700 "yacc.tab.cpp"
    break;

  case 4: /* start: EXIT  */
#line 73 "yacc.y"
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
#line 1709 "yacc.tab.cpp"
    break;

  case 5: /* start: T_EOF  */
#line 78 "yacc.y"
    {
        parse_tree = nullptr;
        YYACCEPT;
    }
#line 1718 "yacc.tab.cpp"
    break;

  case 10: /* stmt: LOAD filePath INTO tbName  */
#line 90 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<LoadTable>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 1726 "yacc.tab.cpp"
    break;

  case 11: /* txnStmt: TXN_BEGIN  */
#line 97 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnBegin>();
    }
#line 1734 "yacc.tab.cpp"
    break;

  case 12: /* txnStmt: TXN_COMMIT  */
#line 101 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnCommit>();
    }
#line 1742 "yacc.tab.cpp"
    break;

  case 13: /* txnStmt: TXN_ABORT  */
#line 105 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnAbort>();
    }
#line 1750 "yacc.tab.cpp"
    break;

  case 14: /* txnStmt: TXN_ROLLBACK  */
#line 109 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<TxnRollback>();
    }
#line 1758 "yacc.tab.cpp"
    break;

  case 15: /* dbStmt: SHOW TABLES  */
#line 116 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<ShowTables>();
    }
#line 1766 "yacc.tab.cpp"
    break;

  case 16: /* ddl: CREATE TABLE tbName '(' fieldList ')'  */
#line 123 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateTable>((yyvsp[-3].sv_str), (yyvsp[-1].sv_fields));
    }
#line 1774 "yacc.tab.cpp"
    break;

  case 17: /* ddl: DROP TABLE tbName  */
#line 127 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropTable>((yyvsp[0].sv_str));
    }
#line 1782 "yacc.tab.cpp"
    break;

  case 18: /* ddl: DESC tbName  */
#line 131 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DescTable>((yyvsp[0].sv_str));
    }
#line 1790 "yacc.tab.cpp"
    break;

  case 19: /* ddl: CREATE INDEX tbName '(' colNameList ')'  */
#line 135 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<CreateIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1798 "yacc.tab.cpp"
    break;

  case 20: /* ddl: DROP INDEX tbName '(' colNameList ')'  */
#line 139 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DropIndex>((yyvsp[-3].sv_str), (yyvsp[-1].sv_strs));
    }
#line 1806 "yacc.tab.cpp"
    break;

  case 21: /* ddl: SHOW INDEX FROM tbName  */
#line 143 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<ShowIndex>((yyvsp[0].sv_str));
    }
#line 1814 "yacc.tab.cpp"
    break;

  case 22: /* dml: INSERT INTO tbName VALUES '(' valueList ')'  */
#line 150 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<InsertStmt>((yyvsp[-4].sv_str), (yyvsp[-1].sv_vals));
    }
#line 1822 "yacc.tab.cpp"
    break;

  case 23: /* dml: DELETE FROM tbName optWhereClause  */
#line 154 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<DeleteStmt>((yyvsp[-1].sv_str), (yyvsp[0].sv_conds));
    }
#line 1830 "yacc.tab.cpp"
    break;

  case 24: /* dml: UPDATE tbName SET setClauses optWhereClause  */
#line 158 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<UpdateStmt>((yyvsp[-3].sv_str), (yyvsp[-1].sv_set_clauses), (yyvsp[0].sv_conds));
    }
#line 1838 "yacc.tab.cpp"
    break;

  case 25: /* dml: SELECT selector optAggFuncList FROM tableList optWhereClause opt_order_clause limitClause  */
#line 162 "yacc.y"
    {
        (yyval.sv_node) = std::make_shared<SelectStmt>((yyvsp[-6].sv_cols), (yyvsp[-5].sv_agg_funcs), (yyvsp[-3].sv_strs), (yyvsp[-2].sv_conds), (yyvsp[-1].sv_orderbys), (yyvsp[0].sv_val));
    }
#line 1846 "yacc.tab.cpp"
    break;

  case 26: /* fieldList: field  */
#line 169 "yacc.y"
    {
        (yyval.sv_fields) = std::vector<std::shared_ptr<Field>>{(yyvsp[0].sv_field)};
    }
#line 1854 "yacc.tab.cpp"
    break;

  case 27: /* fieldList: fieldList ',' field  */
#line 173 "yacc.y"
    {
        (yyval.sv_fields).push_back((yyvsp[0].sv_field));
    }
#line 1862 "yacc.tab.cpp"
    break;

  case 28: /* colNameList: colName  */
#line 180 "yacc.y"
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 1870 "yacc.tab.cpp"
    break;

  case 29: /* colNameList: colNameList ',' colName  */
#line 184 "yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 1878 "yacc.tab.cpp"
    break;

  case 30: /* field: colName type  */
#line 191 "yacc.y"
    {
        (yyval.sv_field) = std::make_shared<ColDef>((yyvsp[-1].sv_str), (yyvsp[0].sv_type_len));
    }
#line 1886 "yacc.tab.cpp"
    break;

  case 31: /* type: INT  */
#line 198 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_INT, sizeof(int));
    }
#line 1894 "yacc.tab.cpp"
    break;

  case 32: /* type: BIG_INT  */
#line 202 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_BIG_INT, sizeof(long long int));
    }
#line 1902 "yacc.tab.cpp"
    break;

  case 33: /* type: CHAR '(' VALUE_INT ')'  */
#line 206 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_STRING, (yyvsp[-1].sv_int));
    }
#line 1910 "yacc.tab.cpp"
    break;

  case 34: /* type: FLOAT  */
#line 210 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_FLOAT, sizeof(float));
    }
#line 1918 "yacc.tab.cpp"
    break;

  case 35: /* type: DATETIME  */
#line 214 "yacc.y"
    {
        (yyval.sv_type_len) = std::make_shared<TypeLen>(SV_TYPE_DATETIME, 19);
    }
#line 1926 "yacc.tab.cpp"
    break;

  case 36: /* valueList: value  */
#line 221 "yacc.y"
    {
        (yyval.sv_vals) = std::vector<std::shared_ptr<Value>>{(yyvsp[0].sv_val)};
    }
#line 1934 "yacc.tab.cpp"
    break;

  case 37: /* valueList: valueList ',' value  */
#line 225 "yacc.y"
    {
        (yyval.sv_vals).push_back((yyvsp[0].sv_val));
    }
#line 1942 "yacc.tab.cpp"
    break;

  case 38: /* value: VALUE_INT  */
#line 232 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<IntLit>((yyvsp[0].sv_int));
    }
#line 1950 "yacc.tab.cpp"
    break;

  case 39: /* value: VALUE_BIG_INT  */
#line 236 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<BigIntLit>((yyvsp[0].sv_big_int));
    }
#line 1958 "yacc.tab.cpp"
    break;

  case 40: /* value: VALUE_FLOAT  */
#line 240 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<FloatLit>((yyvsp[0].sv_float));
    }
#line 1966 "yacc.tab.cpp"
    break;

  case 41: /* value: VALUE_STRING  */
#line 244 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<StringLit>((yyvsp[0].sv_str));
    }
#line 1974 "yacc.tab.cpp"
    break;

  case 42: /* value: VALUE_DATETIME  */
#line 248 "yacc.y"
    {
        (yyval.sv_val) = std::make_shared<DatetimeLit>((yyvsp[0].sv_datetime));
    }
#line 1982 "yacc.tab.cpp"
    break;

  case 43: /* condition: col op expr  */
#line 255 "yacc.y"
    {
        (yyval.sv_cond) = std::make_shared<BinaryExpr>((yyvsp[-2].sv_col), (yyvsp[-1].sv_comp_op), (yyvsp[0].sv_expr));
    }
#line 1990 "yacc.tab.cpp"
    break;

  case 44: /* optWhereClause: %empty  */
#line 261 "yacc.y"
                      { /* ignore*/ }
#line 1996 "yacc.tab.cpp"
    break;

  case 45: /* optWhereClause: WHERE whereClause  */
#line 263 "yacc.y"
    {
        (yyval.sv_conds) = (yyvsp[0].sv_conds);
    }
#line 2004 "yacc.tab.cpp"
    break;

  case 46: /* whereClause: condition  */
#line 270 "yacc.y"
    {
        (yyval.sv_conds) = std::vector<std::shared_ptr<BinaryExpr>>{(yyvsp[0].sv_cond)};
    }
#line 2012 "yacc.tab.cpp"
    break;

  case 47: /* whereClause: whereClause AND condition  */
#line 274 "yacc.y"
    {
        (yyval.sv_conds).push_back((yyvsp[0].sv_cond));
    }
#line 2020 "yacc.tab.cpp"
    break;

  case 48: /* col: tbName '.' colName  */
#line 281 "yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>((yyvsp[-2].sv_str), (yyvsp[0].sv_str));
    }
#line 2028 "yacc.tab.cpp"
    break;

  case 49: /* col: colName  */
#line 285 "yacc.y"
    {
        (yyval.sv_col) = std::make_shared<Col>("", (yyvsp[0].sv_str));
    }
#line 2036 "yacc.tab.cpp"
    break;

  case 50: /* colList: col  */
#line 292 "yacc.y"
    {
        (yyval.sv_cols) = std::vector<std::shared_ptr<Col>>{(yyvsp[0].sv_col)};
    }
#line 2044 "yacc.tab.cpp"
    break;

  case 51: /* colList: colList ',' col  */
#line 296 "yacc.y"
    {
        (yyval.sv_cols).push_back((yyvsp[0].sv_col));
    }
#line 2052 "yacc.tab.cpp"
    break;

  case 52: /* op: '='  */
#line 303 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_EQ;
    }
#line 2060 "yacc.tab.cpp"
    break;

  case 53: /* op: '<'  */
#line 307 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LT;
    }
#line 2068 "yacc.tab.cpp"
    break;

  case 54: /* op: '>'  */
#line 311 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GT;
    }
#line 2076 "yacc.tab.cpp"
    break;

  case 55: /* op: NEQ  */
#line 315 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_NE;
    }
#line 2084 "yacc.tab.cpp"
    break;

  case 56: /* op: LEQ  */
#line 319 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_LE;
    }
#line 2092 "yacc.tab.cpp"
    break;

  case 57: /* op: GEQ  */
#line 323 "yacc.y"
    {
        (yyval.sv_comp_op) = SV_OP_GE;
    }
#line 2100 "yacc.tab.cpp"
    break;

  case 58: /* expr: value  */
#line 330 "yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_val));
    }
#line 2108 "yacc.tab.cpp"
    break;

  case 59: /* expr: col  */
#line 334 "yacc.y"
    {
        (yyval.sv_expr) = std::static_pointer_cast<Expr>((yyvsp[0].sv_col));
    }
#line 2116 "yacc.tab.cpp"
    break;

  case 60: /* setClauses: setClause  */
#line 341 "yacc.y"
    {
        (yyval.sv_set_clauses) = std::vector<std::shared_ptr<SetClause>>{(yyvsp[0].sv_set_clause)};
    }
#line 2124 "yacc.tab.cpp"
    break;

  case 61: /* setClauses: setClauses ',' setClause  */
#line 345 "yacc.y"
    {
        (yyval.sv_set_clauses).push_back((yyvsp[0].sv_set_clause));
    }
#line 2132 "yacc.tab.cpp"
    break;

  case 62: /* setClause: colName '=' value  */
#line 352 "yacc.y"
    {
        (yyval.sv_set_clause) = std::make_shared<SetClause>((yyvsp[-2].sv_str), (yyvsp[0].sv_val));
    }
#line 2140 "yacc.tab.cpp"
    break;

  case 63: /* setClause: colName '=' colName value  */
#line 356 "yacc.y"
    {
        (yyval.sv_set_clause) = std::make_shared<SetClause>((yyvsp[-3].sv_str), (yyvsp[0].sv_val), true);
    }
#line 2148 "yacc.tab.cpp"
    break;

  case 64: /* optAggFunc: SUM '(' col ')' AS colName  */
#line 363 "yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<AggFunc>("SUM", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2156 "yacc.tab.cpp"
    break;

  case 65: /* optAggFunc: MAX '(' col ')' AS colName  */
#line 367 "yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<AggFunc>("MAX", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2164 "yacc.tab.cpp"
    break;

  case 66: /* optAggFunc: MIN '(' col ')' AS colName  */
#line 371 "yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<AggFunc>("MIN", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2172 "yacc.tab.cpp"
    break;

  case 67: /* optAggFunc: COUNT '(' col ')' AS colName  */
#line 375 "yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<AggFunc>("COUNT", (yyvsp[-3].sv_col), (yyvsp[0].sv_str));
    }
#line 2180 "yacc.tab.cpp"
    break;

  case 68: /* optAggFunc: COUNT '(' '*' ')' AS colName  */
#line 379 "yacc.y"
    {
        (yyval.sv_agg_func) = std::make_shared<AggFunc>("COUNT", nullptr, (yyvsp[0].sv_str));
    }
#line 2188 "yacc.tab.cpp"
    break;

  case 69: /* optAggFunc: %empty  */
#line 382 "yacc.y"
                      { /* ignore*/ }
#line 2194 "yacc.tab.cpp"
    break;

  case 70: /* optAggFuncList: optAggFunc  */
#line 387 "yacc.y"
    {
        (yyval.sv_agg_funcs) = std::vector<std::shared_ptr<AggFunc>>{(yyvsp[0].sv_agg_func)};
    }
#line 2202 "yacc.tab.cpp"
    break;

  case 71: /* optAggFuncList: optAggFuncList ',' optAggFunc  */
#line 391 "yacc.y"
    {
        (yyval.sv_agg_funcs).push_back((yyvsp[0].sv_agg_func));
    }
#line 2210 "yacc.tab.cpp"
    break;

  case 72: /* selector: '*'  */
#line 398 "yacc.y"
    {
        (yyval.sv_cols) = {};
    }
#line 2218 "yacc.tab.cpp"
    break;

  case 73: /* selector: colList  */
#line 402 "yacc.y"
    {

    }
#line 2226 "yacc.tab.cpp"
    break;

  case 74: /* selector: %empty  */
#line 405 "yacc.y"
                      { /* ignore*/ }
#line 2232 "yacc.tab.cpp"
    break;

  case 75: /* tableList: tbName  */
#line 410 "yacc.y"
    {
        (yyval.sv_strs) = std::vector<std::string>{(yyvsp[0].sv_str)};
    }
#line 2240 "yacc.tab.cpp"
    break;

  case 76: /* tableList: tableList ',' tbName  */
#line 414 "yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 2248 "yacc.tab.cpp"
    break;

  case 77: /* tableList: tableList JOIN tbName  */
#line 418 "yacc.y"
    {
        (yyval.sv_strs).push_back((yyvsp[0].sv_str));
    }
#line 2256 "yacc.tab.cpp"
    break;

  case 78: /* opt_asc_desc: ASC  */
#line 425 "yacc.y"
    { 
        (yyval.sv_orderby_dir) = OrderBy_ASC;     
    }
#line 2264 "yacc.tab.cpp"
    break;

  case 79: /* opt_asc_desc: DESC  */
#line 429 "yacc.y"
    { 
        (yyval.sv_orderby_dir) = OrderBy_DESC;    
    }
#line 2272 "yacc.tab.cpp"
    break;

  case 80: /* opt_asc_desc: %empty  */
#line 433 "yacc.y"
    { 
        (yyval.sv_orderby_dir) = OrderBy_DEFAULT; 
    }
#line 2280 "yacc.tab.cpp"
    break;

  case 81: /* order_clause: col opt_asc_desc  */
#line 440 "yacc.y"
    { 
        (yyval.sv_orderby) = std::make_shared<OrderBy>((yyvsp[-1].sv_col), (yyvsp[0].sv_orderby_dir));
    }
#line 2288 "yacc.tab.cpp"
    break;

  case 82: /* order_clause_list: order_clause  */
#line 447 "yacc.y"
    {
        (yyval.sv_orderbys) = std::vector<std::shared_ptr<OrderBy>>{(yyvsp[0].sv_orderby)};
    }
#line 2296 "yacc.tab.cpp"
    break;

  case 83: /* order_clause_list: order_clause_list ',' order_clause  */
#line 451 "yacc.y"
    {
        (yyval.sv_orderbys).push_back((yyvsp[0].sv_orderby));
    }
#line 2304 "yacc.tab.cpp"
    break;

  case 84: /* opt_order_clause: ORDER BY order_clause_list  */
#line 458 "yacc.y"
    { 
        (yyval.sv_orderbys) = (yyvsp[0].sv_orderbys);
    }
#line 2312 "yacc.tab.cpp"
    break;

  case 85: /* opt_order_clause: %empty  */
#line 461 "yacc.y"
                      { /* ignore*/ }
#line 2318 "yacc.tab.cpp"
    break;

  case 86: /* limitClause: LIMIT value  */
#line 466 "yacc.y"
    {
        (yyval.sv_val) = (yyvsp[0].sv_val);
    }
#line 2326 "yacc.tab.cpp"
    break;

  case 87: /* limitClause: %empty  */
#line 469 "yacc.y"
                      { /* ignore*/ }
#line 2332 "yacc.tab.cpp"
    break;


#line 2336 "yacc.tab.cpp"

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

#line 477 "yacc.y"


