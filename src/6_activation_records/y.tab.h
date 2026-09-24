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

	

#line 182 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
