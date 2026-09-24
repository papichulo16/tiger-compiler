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
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 4 "tiger.grm"

#include <stdio.h>
#include "intf/util.h"
#include "intf/symbol.h" 
#include "intf/errormsg.h"
#include "intf/absyn.h"

extern int yylineno;
extern char* yytext;
extern char curr_line[];

int EM_err_count = 0;
A_exp absyn_root = NULL;

int yylex(void); /* function prototype */

void yyerror(char *s)
{

 
 EM_error(EM_tokPos, "\"%s\"\n\t\t%s in line %d, near '%s'\n", curr_line, s, yylineno, yytext);
 EM_err_count += 1;

}

#line 97 "y.tab.c"

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

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
    ID = 258,                      /* ID  */
    STRING = 259,                  /* STRING  */
    INT = 260,                     /* INT  */
    COMMA = 261,                   /* COMMA  */
    COLON = 262,                   /* COLON  */
    SEMICOLON = 263,               /* SEMICOLON  */
    LPAREN = 264,                  /* LPAREN  */
    RPAREN = 265,                  /* RPAREN  */
    LBRACK = 266,                  /* LBRACK  */
    RBRACK = 267,                  /* RBRACK  */
    LBRACE = 268,                  /* LBRACE  */
    RBRACE = 269,                  /* RBRACE  */
    DOT = 270,                     /* DOT  */
    PLUS = 271,                    /* PLUS  */
    MINUS = 272,                   /* MINUS  */
    TIMES = 273,                   /* TIMES  */
    DIVIDE = 274,                  /* DIVIDE  */
    EQ = 275,                      /* EQ  */
    NEQ = 276,                     /* NEQ  */
    LT = 277,                      /* LT  */
    LE = 278,                      /* LE  */
    GT = 279,                      /* GT  */
    GE = 280,                      /* GE  */
    AND = 281,                     /* AND  */
    OR = 282,                      /* OR  */
    ASSIGN = 283,                  /* ASSIGN  */
    ARRAY = 284,                   /* ARRAY  */
    IF = 285,                      /* IF  */
    THEN = 286,                    /* THEN  */
    ELSE = 287,                    /* ELSE  */
    WHILE = 288,                   /* WHILE  */
    FOR = 289,                     /* FOR  */
    TO = 290,                      /* TO  */
    DO = 291,                      /* DO  */
    LET = 292,                     /* LET  */
    IN = 293,                      /* IN  */
    END = 294,                     /* END  */
    OF = 295,                      /* OF  */
    BREAK = 296,                   /* BREAK  */
    NIL = 297,                     /* NIL  */
    FUNCTION = 298,                /* FUNCTION  */
    VAR = 299,                     /* VAR  */
    TYPE = 300                     /* TYPE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define ID 258
#define STRING 259
#define INT 260
#define COMMA 261
#define COLON 262
#define SEMICOLON 263
#define LPAREN 264
#define RPAREN 265
#define LBRACK 266
#define RBRACK 267
#define LBRACE 268
#define RBRACE 269
#define DOT 270
#define PLUS 271
#define MINUS 272
#define TIMES 273
#define DIVIDE 274
#define EQ 275
#define NEQ 276
#define LT 277
#define LE 278
#define GT 279
#define GE 280
#define AND 281
#define OR 282
#define ASSIGN 283
#define ARRAY 284
#define IF 285
#define THEN 286
#define ELSE 287
#define WHILE 288
#define FOR 289
#define TO 290
#define DO 291
#define LET 292
#define IN 293
#define END 294
#define OF 295
#define BREAK 296
#define NIL 297
#define FUNCTION 298
#define VAR 299
#define TYPE 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 31 "tiger.grm"


	int pos;
	int ival;
	string sval;

	A_var var;
	A_exp exp;
  A_dec dec;
  A_ty ty;
  A_field field;
  A_fundec fundec;
  A_namety namety;
  A_efield efield;

  A_decList decl;
  A_expList expl;
  A_fieldList fieldl;
  A_fundecList fundecl;
  A_nametyList nametyl;
  A_efieldList efieldl;

	

#line 265 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_COMMA = 6,                      /* COMMA  */
  YYSYMBOL_COLON = 7,                      /* COLON  */
  YYSYMBOL_SEMICOLON = 8,                  /* SEMICOLON  */
  YYSYMBOL_LPAREN = 9,                     /* LPAREN  */
  YYSYMBOL_RPAREN = 10,                    /* RPAREN  */
  YYSYMBOL_LBRACK = 11,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 12,                    /* RBRACK  */
  YYSYMBOL_LBRACE = 13,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 14,                    /* RBRACE  */
  YYSYMBOL_DOT = 15,                       /* DOT  */
  YYSYMBOL_PLUS = 16,                      /* PLUS  */
  YYSYMBOL_MINUS = 17,                     /* MINUS  */
  YYSYMBOL_TIMES = 18,                     /* TIMES  */
  YYSYMBOL_DIVIDE = 19,                    /* DIVIDE  */
  YYSYMBOL_EQ = 20,                        /* EQ  */
  YYSYMBOL_NEQ = 21,                       /* NEQ  */
  YYSYMBOL_LT = 22,                        /* LT  */
  YYSYMBOL_LE = 23,                        /* LE  */
  YYSYMBOL_GT = 24,                        /* GT  */
  YYSYMBOL_GE = 25,                        /* GE  */
  YYSYMBOL_AND = 26,                       /* AND  */
  YYSYMBOL_OR = 27,                        /* OR  */
  YYSYMBOL_ASSIGN = 28,                    /* ASSIGN  */
  YYSYMBOL_ARRAY = 29,                     /* ARRAY  */
  YYSYMBOL_IF = 30,                        /* IF  */
  YYSYMBOL_THEN = 31,                      /* THEN  */
  YYSYMBOL_ELSE = 32,                      /* ELSE  */
  YYSYMBOL_WHILE = 33,                     /* WHILE  */
  YYSYMBOL_FOR = 34,                       /* FOR  */
  YYSYMBOL_TO = 35,                        /* TO  */
  YYSYMBOL_DO = 36,                        /* DO  */
  YYSYMBOL_LET = 37,                       /* LET  */
  YYSYMBOL_IN = 38,                        /* IN  */
  YYSYMBOL_END = 39,                       /* END  */
  YYSYMBOL_OF = 40,                        /* OF  */
  YYSYMBOL_BREAK = 41,                     /* BREAK  */
  YYSYMBOL_NIL = 42,                       /* NIL  */
  YYSYMBOL_FUNCTION = 43,                  /* FUNCTION  */
  YYSYMBOL_VAR = 44,                       /* VAR  */
  YYSYMBOL_TYPE = 45,                      /* TYPE  */
  YYSYMBOL_YYACCEPT = 46,                  /* $accept  */
  YYSYMBOL_prog = 47,                      /* prog  */
  YYSYMBOL_let = 48,                       /* let  */
  YYSYMBOL_decs = 49,                      /* decs  */
  YYSYMBOL_dec = 50,                       /* dec  */
  YYSYMBOL_tydec = 51,                     /* tydec  */
  YYSYMBOL_ty = 52,                        /* ty  */
  YYSYMBOL_tyfields = 53,                  /* tyfields  */
  YYSYMBOL_vardec = 54,                    /* vardec  */
  YYSYMBOL_arrdec = 55,                    /* arrdec  */
  YYSYMBOL_fundec = 56,                    /* fundec  */
  YYSYMBOL_lvalue = 57,                    /* lvalue  */
  YYSYMBOL_fparams = 58,                   /* fparams  */
  YYSYMBOL_valess = 59,                    /* valess  */
  YYSYMBOL_recpara = 60,                   /* recpara  */
  YYSYMBOL_rec = 61,                       /* rec  */
  YYSYMBOL_expseq = 62,                    /* expseq  */
  YYSYMBOL_exp = 63                        /* exp  */
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  39
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   368

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  143

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   300


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    90,    90,    93,    97,    98,    99,   103,   104,   105,
     109,   111,   112,   113,   116,   117,   118,   122,   123,   127,
     128,   132,   133,   137,   138,   139,   140,   144,   145,   146,
     150,   151,   152,   153,   154,   155,   156,   160,   161,   162,
     165,   168,   169,   170,   171,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "STRING", "INT",
  "COMMA", "COLON", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK",
  "LBRACE", "RBRACE", "DOT", "PLUS", "MINUS", "TIMES", "DIVIDE", "EQ",
  "NEQ", "LT", "LE", "GT", "GE", "AND", "OR", "ASSIGN", "ARRAY", "IF",
  "THEN", "ELSE", "WHILE", "FOR", "TO", "DO", "LET", "IN", "END", "OF",
  "BREAK", "NIL", "FUNCTION", "VAR", "TYPE", "$accept", "prog", "let",
  "decs", "dec", "tydec", "ty", "tyfields", "vardec", "arrdec", "fundec",
  "lvalue", "fparams", "valess", "recpara", "rec", "expseq", "exp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-114)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-45)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     134,    -1,  -114,  -114,    77,   134,   134,   134,     8,    25,
    -114,  -114,     9,  -114,  -114,     7,  -114,  -114,   320,   134,
     134,    12,  -114,    13,    17,   320,    37,   276,   197,   -11,
      25,    26,    28,    54,    21,    25,  -114,  -114,  -114,  -114,
     134,    55,   134,   134,   134,   134,   134,   134,   134,   134,
     134,   134,   134,   134,   134,    51,   163,   292,    47,    57,
     134,  -114,   134,  -114,   134,   134,   134,  -114,    63,     6,
      53,   119,  -114,   308,  -114,   320,    37,    37,  -114,  -114,
      20,    20,    20,    20,    20,    20,   343,   332,  -114,   134,
      34,   134,  -114,   320,   320,   259,   320,   239,    72,    73,
     134,     1,    69,    -7,  -114,  -114,   134,   185,   134,   134,
      76,    74,    60,   320,  -114,    72,    39,  -114,  -114,   320,
      12,   320,   218,    87,    -4,   134,    78,    90,  -114,   134,
      89,    93,   134,   320,  -114,  -114,   320,    72,    79,   320,
    -114,   134,   320
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    23,    65,    64,     0,     0,     0,     0,     0,     0,
      36,    63,     0,    62,    60,    59,    58,    61,     2,    29,
       0,    39,    20,     0,     0,    41,    54,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     8,     9,     1,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    28,     0,     0,     0,
       0,    67,     0,    66,     0,     0,     0,     5,     0,     0,
       0,     0,     4,     0,    24,    31,    53,    55,    56,    57,
      47,    48,    49,    50,    51,    52,    46,    45,    30,    29,
      26,     0,    40,    43,    42,    33,    34,     0,    16,     0,
       0,     0,     0,     0,    25,    27,     0,    37,     0,     0,
       0,     0,     0,    17,    11,    16,     0,    10,     3,    19,
      39,    32,     0,     0,     0,     0,     0,     0,    38,     0,
      14,     0,     0,    18,    12,    13,    35,    16,     0,    21,
      15,     0,    22
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -114,  -114,  -114,    -2,  -114,  -114,  -114,  -113,  -114,  -114,
    -114,  -114,    14,  -114,   -23,  -114,    27,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    12,    13,    34,    35,    36,   117,   111,    37,    14,
      38,    15,    55,    16,    59,    17,    24,    25
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      18,    62,   126,   131,   114,    26,    27,    28,    19,    39,
      20,    29,    21,    99,   115,    58,   132,    66,    40,    56,
      57,    60,    41,    61,   140,    62,    30,    63,    67,    68,
     116,    69,   118,    72,   100,    42,    43,    44,    45,    46,
      73,    22,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    45,    46,    70,    74,    71,
      93,    88,    94,    -6,    95,    96,    97,    91,    31,    32,
      33,    92,    98,   101,   106,   110,   112,    60,    23,   127,
       1,     2,     3,   123,   124,   -44,     4,   -44,   125,    56,
     130,   107,   134,   135,     5,   137,   138,   128,   103,   141,
     113,     0,     0,   105,     0,     0,   119,     6,   121,   122,
       7,     8,     0,     0,     9,     0,     0,     0,    10,    11,
     102,     0,     1,     2,     3,   133,     0,   -44,     4,   136,
       0,     0,   139,     0,     0,     0,     5,     1,     2,     3,
       0,   142,     0,     4,     0,     0,     0,     0,     0,     6,
       0,     5,     7,     8,     0,     0,     9,     0,   -44,     0,
      10,    11,     0,     0,     6,     0,     0,     7,     8,    89,
       0,     9,     0,     0,     0,    10,    11,     0,     0,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,   120,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,     0,     0,     0,     0,     0,
       0,     0,     0,    65,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,     0,     0,     0,     0,
       0,     0,     0,     0,   129,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,     0,     0,     0,
       0,     0,     0,     0,   109,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,     0,     0,     0,
       0,   108,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    90,     0,     0,    64,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
     104,     0,     0,     0,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52
};

static const yytype_int16 yycheck[] =
{
       0,     8,   115,     7,     3,     5,     6,     7,     9,     0,
      11,     3,    13,     7,    13,     3,    20,    28,    11,    19,
      20,     8,    15,    10,   137,     8,     1,    10,    30,     3,
      29,     3,    39,    35,    28,    28,    16,    17,    18,    19,
      40,    42,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    18,    19,     3,     3,    38,
      60,    10,    62,    38,    64,    65,    66,    20,    43,    44,
      45,    14,     9,    20,    40,     3,     3,     8,     1,    40,
       3,     4,     5,     7,    10,     8,     9,    10,    28,    89,
       3,    91,    14,     3,    17,     6,     3,   120,    71,    20,
     100,    -1,    -1,    89,    -1,    -1,   106,    30,   108,   109,
      33,    34,    -1,    -1,    37,    -1,    -1,    -1,    41,    42,
       1,    -1,     3,     4,     5,   125,    -1,     8,     9,   129,
      -1,    -1,   132,    -1,    -1,    -1,    17,     3,     4,     5,
      -1,   141,    -1,     9,    -1,    -1,    -1,    -1,    -1,    30,
      -1,    17,    33,    34,    -1,    -1,    37,    -1,    39,    -1,
      41,    42,    -1,    -1,    30,    -1,    -1,    33,    34,     6,
      -1,    37,    -1,    -1,    -1,    41,    42,    -1,    -1,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,     6,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    36,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    35,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    -1,    -1,    -1,
      -1,    32,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    12,    -1,    -1,    31,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      12,    -1,    -1,    -1,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     9,    17,    30,    33,    34,    37,
      41,    42,    47,    48,    55,    57,    59,    61,    63,     9,
      11,    13,    42,     1,    62,    63,    63,    63,    63,     3,
       1,    43,    44,    45,    49,    50,    51,    54,    56,     0,
      11,    15,    28,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    58,    63,    63,     3,    60,
       8,    10,     8,    10,    31,    36,    28,    49,     3,     3,
       3,    38,    49,    63,     3,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    10,     6,
      12,    20,    14,    63,    63,    63,    63,    63,     9,     7,
      28,    20,     1,    62,    12,    58,    40,    63,    32,    35,
       3,    53,     3,    63,     3,    13,    29,    52,    39,    63,
       6,    63,    63,     7,    10,    28,    53,    40,    60,    36,
       3,     7,    20,    63,    14,     3,    63,     6,     3,    63,
      53,    20,    63
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    46,    47,    48,    49,    49,    49,    50,    50,    50,
      51,    52,    52,    52,    53,    53,    53,    54,    54,    55,
      55,    56,    56,    57,    57,    57,    57,    58,    58,    58,
      59,    59,    59,    59,    59,    59,    59,    60,    60,    60,
      61,    62,    62,    62,    62,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     5,     2,     2,     0,     1,     1,     1,
       4,     1,     3,     3,     3,     5,     0,     4,     6,     6,
       2,     7,     9,     1,     3,     4,     4,     3,     1,     0,
       4,     3,     6,     4,     4,     8,     1,     3,     5,     0,
       4,     1,     3,     3,     0,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3
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
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
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

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

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

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
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
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

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
      yychar = yylex ();
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* prog: exp  */
#line 90 "tiger.grm"
            { absyn_root=(yyvsp[0].exp); }
#line 1438 "y.tab.c"
    break;

  case 3: /* let: LET decs IN expseq END  */
#line 93 "tiger.grm"
                             { (yyval.exp) = A_LetExp(EM_tokPos, (yyvsp[-3].decl), (yyvsp[-1].exp)); }
#line 1444 "y.tab.c"
    break;

  case 4: /* decs: dec decs  */
#line 97 "tiger.grm"
               { (yyval.decl) = A_DecList((yyvsp[-1].dec), (yyvsp[0].decl)); }
#line 1450 "y.tab.c"
    break;

  case 5: /* decs: error decs  */
#line 98 "tiger.grm"
                 { (yyval.decl) = A_DecList(NULL, (yyvsp[0].decl)); }
#line 1456 "y.tab.c"
    break;

  case 6: /* decs: %empty  */
#line 99 "tiger.grm"
      { (yyval.decl) = NULL; }
#line 1462 "y.tab.c"
    break;

  case 7: /* dec: tydec  */
#line 103 "tiger.grm"
            { (yyval.dec) = A_TypeDec(EM_tokPos, (yyvsp[0].nametyl)); }
#line 1468 "y.tab.c"
    break;

  case 8: /* dec: vardec  */
#line 104 "tiger.grm"
             { (yyval.dec) = (yyvsp[0].dec); }
#line 1474 "y.tab.c"
    break;

  case 9: /* dec: fundec  */
#line 105 "tiger.grm"
             { (yyval.dec) = A_FunctionDec(EM_tokPos, (yyvsp[0].fundecl)); }
#line 1480 "y.tab.c"
    break;

  case 10: /* tydec: TYPE ID EQ ty  */
#line 109 "tiger.grm"
                    { (yyval.nametyl) = A_NametyList(A_Namety(S_Symbol((yyvsp[-2].sval)), (yyvsp[0].ty)), NULL); }
#line 1486 "y.tab.c"
    break;

  case 11: /* ty: ID  */
#line 111 "tiger.grm"
         { (yyval.ty) = A_NameTy(EM_tokPos, S_Symbol((yyvsp[0].sval))); }
#line 1492 "y.tab.c"
    break;

  case 12: /* ty: LBRACE tyfields RBRACE  */
#line 112 "tiger.grm"
                             { (yyval.ty) = A_RecordTy(EM_tokPos, (yyvsp[-1].fieldl)); }
#line 1498 "y.tab.c"
    break;

  case 13: /* ty: ARRAY OF ID  */
#line 113 "tiger.grm"
                  { (yyval.ty) = A_ArrayTy(EM_tokPos, S_Symbol((yyvsp[0].sval))); }
#line 1504 "y.tab.c"
    break;

  case 14: /* tyfields: ID COLON ID  */
#line 116 "tiger.grm"
                  { (yyval.fieldl) = A_FieldList(A_Field(EM_tokPos, S_Symbol((yyvsp[-2].sval)), S_Symbol((yyvsp[0].sval))), NULL); }
#line 1510 "y.tab.c"
    break;

  case 15: /* tyfields: ID COLON ID COMMA tyfields  */
#line 117 "tiger.grm"
                                 { (yyval.fieldl) = A_FieldList(A_Field(EM_tokPos, S_Symbol((yyvsp[-4].sval)), S_Symbol((yyvsp[-2].sval))), (yyvsp[0].fieldl)); }
#line 1516 "y.tab.c"
    break;

  case 16: /* tyfields: %empty  */
#line 118 "tiger.grm"
      { (yyval.fieldl) = NULL; }
#line 1522 "y.tab.c"
    break;

  case 17: /* vardec: VAR ID ASSIGN exp  */
#line 122 "tiger.grm"
                        { (yyval.dec) = A_VarDec(EM_tokPos, S_Symbol((yyvsp[-2].sval)), NULL, (yyvsp[0].exp)); }
#line 1528 "y.tab.c"
    break;

  case 18: /* vardec: VAR ID COLON ID ASSIGN exp  */
#line 123 "tiger.grm"
                                 { (yyval.dec) = A_VarDec(EM_tokPos, S_Symbol((yyvsp[-4].sval)), S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp)); }
#line 1534 "y.tab.c"
    break;

  case 19: /* arrdec: ID LBRACK exp RBRACK OF exp  */
#line 127 "tiger.grm"
                                  { (yyval.exp) = A_ArrayExp(EM_tokPos, S_Symbol((yyvsp[-5].sval)), (yyvsp[-3].exp), (yyvsp[0].exp)); }
#line 1540 "y.tab.c"
    break;

  case 20: /* arrdec: ID NIL  */
#line 128 "tiger.grm"
             { (yyval.exp) = (yyval.exp) = A_ArrayExp(EM_tokPos, S_Symbol((yyvsp[-1].sval)), NULL, NULL); }
#line 1546 "y.tab.c"
    break;

  case 21: /* fundec: FUNCTION ID LPAREN tyfields RPAREN EQ exp  */
#line 132 "tiger.grm"
                                                { (yyval.fundecl) = A_FundecList(A_Fundec(EM_tokPos, S_Symbol((yyvsp[-5].sval)), (yyvsp[-3].fieldl), NULL, (yyvsp[0].exp)), NULL); }
#line 1552 "y.tab.c"
    break;

  case 22: /* fundec: FUNCTION ID LPAREN tyfields RPAREN COLON ID EQ exp  */
#line 133 "tiger.grm"
                                                         { (yyval.fundecl) = A_FundecList(A_Fundec(EM_tokPos, S_Symbol((yyvsp[-7].sval)), (yyvsp[-5].fieldl), S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp)), NULL); }
#line 1558 "y.tab.c"
    break;

  case 23: /* lvalue: ID  */
#line 137 "tiger.grm"
         { (yyval.var) = A_SimpleVar(EM_tokPos, S_Symbol((yyvsp[0].sval))); }
#line 1564 "y.tab.c"
    break;

  case 24: /* lvalue: lvalue DOT ID  */
#line 138 "tiger.grm"
                    { (yyval.var) = A_FieldVar(EM_tokPos, (yyvsp[-2].var), S_Symbol((yyvsp[0].sval))); }
#line 1570 "y.tab.c"
    break;

  case 25: /* lvalue: lvalue LBRACK exp RBRACK  */
#line 139 "tiger.grm"
                               { (yyval.var) = A_SubscriptVar(EM_tokPos, (yyvsp[-3].var), (yyvsp[-1].exp)); }
#line 1576 "y.tab.c"
    break;

  case 26: /* lvalue: ID LBRACK exp RBRACK  */
#line 140 "tiger.grm"
                           { (yyval.var) = A_SubscriptVar(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol((yyvsp[-3].sval))), (yyvsp[-1].exp)); }
#line 1582 "y.tab.c"
    break;

  case 27: /* fparams: exp COMMA fparams  */
#line 144 "tiger.grm"
                        { (yyval.expl) = A_ExpList((yyvsp[-2].exp), (yyvsp[0].expl)); }
#line 1588 "y.tab.c"
    break;

  case 28: /* fparams: exp  */
#line 145 "tiger.grm"
          { (yyval.expl) = A_ExpList((yyvsp[0].exp), NULL); }
#line 1594 "y.tab.c"
    break;

  case 29: /* fparams: %empty  */
#line 146 "tiger.grm"
      { (yyval.expl) = NULL; }
#line 1600 "y.tab.c"
    break;

  case 30: /* valess: ID LPAREN fparams RPAREN  */
#line 150 "tiger.grm"
                               { (yyval.exp) = A_CallExp(EM_tokPos, S_Symbol((yyvsp[-3].sval)), (yyvsp[-1].expl)); }
#line 1606 "y.tab.c"
    break;

  case 31: /* valess: lvalue ASSIGN exp  */
#line 151 "tiger.grm"
                        { (yyval.exp) = A_AssignExp(EM_tokPos, (yyvsp[-2].var), (yyvsp[0].exp)); }
#line 1612 "y.tab.c"
    break;

  case 32: /* valess: IF exp THEN exp ELSE exp  */
#line 152 "tiger.grm"
                               { (yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1618 "y.tab.c"
    break;

  case 33: /* valess: IF exp THEN exp  */
#line 153 "tiger.grm"
                      { (yyval.exp) = A_IfExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].exp), NULL); }
#line 1624 "y.tab.c"
    break;

  case 34: /* valess: WHILE exp DO exp  */
#line 154 "tiger.grm"
                       { (yyval.exp) = A_WhileExp(EM_tokPos, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1630 "y.tab.c"
    break;

  case 35: /* valess: FOR ID ASSIGN exp TO exp DO exp  */
#line 155 "tiger.grm"
                                      { (yyval.exp) = A_ForExp(EM_tokPos, S_Symbol((yyvsp[-6].sval)), (yyvsp[-4].exp), (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1636 "y.tab.c"
    break;

  case 36: /* valess: BREAK  */
#line 156 "tiger.grm"
            { (yyval.exp) = A_BreakExp(EM_tokPos); }
#line 1642 "y.tab.c"
    break;

  case 37: /* recpara: ID EQ exp  */
#line 160 "tiger.grm"
                { (yyval.efieldl) = A_EfieldList(A_Efield(S_Symbol((yyvsp[-2].sval)), (yyvsp[0].exp)), NULL); }
#line 1648 "y.tab.c"
    break;

  case 38: /* recpara: ID EQ exp COMMA recpara  */
#line 161 "tiger.grm"
                              { (yyval.efieldl) = A_EfieldList(A_Efield(S_Symbol((yyvsp[-4].sval)), (yyvsp[-2].exp)), (yyvsp[0].efieldl)); }
#line 1654 "y.tab.c"
    break;

  case 39: /* recpara: %empty  */
#line 162 "tiger.grm"
      { (yyval.efieldl) = NULL; }
#line 1660 "y.tab.c"
    break;

  case 40: /* rec: ID LBRACE recpara RBRACE  */
#line 165 "tiger.grm"
                               { (yyval.exp) = A_RecordExp(EM_tokPos, S_Symbol((yyvsp[-3].sval)), (yyvsp[-1].efieldl)); }
#line 1666 "y.tab.c"
    break;

  case 41: /* expseq: exp  */
#line 168 "tiger.grm"
          { (yyval.exp) =  (yyvsp[0].exp); }
#line 1672 "y.tab.c"
    break;

  case 42: /* expseq: expseq SEMICOLON exp  */
#line 169 "tiger.grm"
                           { (yyval.exp) = A_SeqExp(EM_tokPos, A_ExpList((yyvsp[-2].exp), A_ExpList((yyvsp[0].exp), NULL))); }
#line 1678 "y.tab.c"
    break;

  case 43: /* expseq: error SEMICOLON exp  */
#line 170 "tiger.grm"
                          { (yyval.exp) = A_SeqExp(EM_tokPos, A_ExpList(NULL, A_ExpList((yyvsp[0].exp), NULL))); }
#line 1684 "y.tab.c"
    break;

  case 44: /* expseq: %empty  */
#line 171 "tiger.grm"
      { (yyval.exp) = NULL; }
#line 1690 "y.tab.c"
    break;

  case 45: /* exp: exp OR exp  */
#line 174 "tiger.grm"
                 { (yyval.exp) = A_OpExp(EM_tokPos, A_orOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1696 "y.tab.c"
    break;

  case 46: /* exp: exp AND exp  */
#line 175 "tiger.grm"
                  { (yyval.exp) = A_OpExp(EM_tokPos, A_andOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1702 "y.tab.c"
    break;

  case 47: /* exp: exp EQ exp  */
#line 176 "tiger.grm"
                 { (yyval.exp) = A_OpExp(EM_tokPos, A_eqOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1708 "y.tab.c"
    break;

  case 48: /* exp: exp NEQ exp  */
#line 177 "tiger.grm"
                  { (yyval.exp) = A_OpExp(EM_tokPos, A_neqOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1714 "y.tab.c"
    break;

  case 49: /* exp: exp LT exp  */
#line 178 "tiger.grm"
                 { (yyval.exp) = A_OpExp(EM_tokPos, A_ltOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1720 "y.tab.c"
    break;

  case 50: /* exp: exp LE exp  */
#line 179 "tiger.grm"
                 { (yyval.exp) = A_OpExp(EM_tokPos, A_leOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1726 "y.tab.c"
    break;

  case 51: /* exp: exp GT exp  */
#line 180 "tiger.grm"
                 { (yyval.exp) = A_OpExp(EM_tokPos, A_gtOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1732 "y.tab.c"
    break;

  case 52: /* exp: exp GE exp  */
#line 181 "tiger.grm"
                 { (yyval.exp) = A_OpExp(EM_tokPos, A_geOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1738 "y.tab.c"
    break;

  case 53: /* exp: exp PLUS exp  */
#line 182 "tiger.grm"
                   { (yyval.exp) = A_OpExp(EM_tokPos, A_plusOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1744 "y.tab.c"
    break;

  case 54: /* exp: MINUS exp  */
#line 183 "tiger.grm"
                { (yyval.exp) = A_OpExp(EM_tokPos, A_timesOp, (yyvsp[0].exp), A_IntExp(EM_tokPos, -1)); }
#line 1750 "y.tab.c"
    break;

  case 55: /* exp: exp MINUS exp  */
#line 184 "tiger.grm"
                    { (yyval.exp) = A_OpExp(EM_tokPos, A_minusOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1756 "y.tab.c"
    break;

  case 56: /* exp: exp TIMES exp  */
#line 185 "tiger.grm"
                    { (yyval.exp) = A_OpExp(EM_tokPos, A_timesOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1762 "y.tab.c"
    break;

  case 57: /* exp: exp DIVIDE exp  */
#line 186 "tiger.grm"
                     { (yyval.exp) = A_OpExp(EM_tokPos, A_divideOp, (yyvsp[-2].exp), (yyvsp[0].exp)); }
#line 1768 "y.tab.c"
    break;

  case 58: /* exp: valess  */
#line 187 "tiger.grm"
             { (yyval.exp) = (yyvsp[0].exp); }
#line 1774 "y.tab.c"
    break;

  case 59: /* exp: lvalue  */
#line 188 "tiger.grm"
             { (yyval.exp) = A_VarExp(EM_tokPos, (yyvsp[0].var)); }
#line 1780 "y.tab.c"
    break;

  case 60: /* exp: arrdec  */
#line 189 "tiger.grm"
             { (yyval.exp) = (yyvsp[0].exp); }
#line 1786 "y.tab.c"
    break;

  case 61: /* exp: rec  */
#line 190 "tiger.grm"
          { (yyval.exp) = (yyvsp[0].exp); }
#line 1792 "y.tab.c"
    break;

  case 62: /* exp: let  */
#line 191 "tiger.grm"
          { (yyval.exp) = (yyvsp[0].exp); }
#line 1798 "y.tab.c"
    break;

  case 63: /* exp: NIL  */
#line 192 "tiger.grm"
          { (yyval.exp) = A_NilExp(EM_tokPos); }
#line 1804 "y.tab.c"
    break;

  case 64: /* exp: INT  */
#line 193 "tiger.grm"
          { (yyval.exp) = A_IntExp(EM_tokPos, (yyvsp[0].ival)); }
#line 1810 "y.tab.c"
    break;

  case 65: /* exp: STRING  */
#line 194 "tiger.grm"
             { (yyval.exp) = A_StringExp(EM_tokPos, (yyvsp[0].sval)); }
#line 1816 "y.tab.c"
    break;

  case 66: /* exp: LPAREN expseq RPAREN  */
#line 195 "tiger.grm"
                           { (yyval.exp) = (yyvsp[-1].exp); }
#line 1822 "y.tab.c"
    break;

  case 67: /* exp: LPAREN error RPAREN  */
#line 196 "tiger.grm"
                          { (yyval.exp) = NULL; }
#line 1828 "y.tab.c"
    break;


#line 1832 "y.tab.c"

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
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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
  yyerror (YY_("memory exhausted"));
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
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

