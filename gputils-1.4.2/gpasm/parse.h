/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_PARSE_H_INCLUDED
# define YY_YY_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LABEL = 258,
    IDENTIFIER = 259,
    IDENT_BRACKET = 260,
    CBLOCK = 261,
    DEBUG_LINE = 262,
    ENDC = 263,
    ERRORLEVEL = 264,
    FILL = 265,
    LIST = 266,
    NUMBER = 267,
    PROCESSOR = 268,
    STRING = 269,
    DEFINE = 270,
    UPPER = 271,
    HIGH = 272,
    LOW = 273,
    LSH = 274,
    RSH = 275,
    GREATER_EQUAL = 276,
    LESS_EQUAL = 277,
    EQUAL = 278,
    NOT_EQUAL = 279,
    LOGICAL_AND = 280,
    LOGICAL_OR = 281,
    ASSIGN_PLUS = 282,
    ASSIGN_MINUS = 283,
    ASSIGN_MULTIPLY = 284,
    ASSIGN_DIVIDE = 285,
    ASSIGN_MODULUS = 286,
    ASSIGN_LSH = 287,
    ASSIGN_RSH = 288,
    ASSIGN_AND = 289,
    ASSIGN_OR = 290,
    ASSIGN_XOR = 291,
    INCREMENT = 292,
    DECREMENT = 293,
    POSTINCREMENT = 294,
    POSTDECREMENT = 295,
    INDFOFFSET = 296,
    TBL_NO_CHANGE = 297,
    TBL_POST_INC = 298,
    TBL_POST_DEC = 299,
    TBL_PRE_INC = 300
  };
#endif
/* Tokens.  */
#define LABEL 258
#define IDENTIFIER 259
#define IDENT_BRACKET 260
#define CBLOCK 261
#define DEBUG_LINE 262
#define ENDC 263
#define ERRORLEVEL 264
#define FILL 265
#define LIST 266
#define NUMBER 267
#define PROCESSOR 268
#define STRING 269
#define DEFINE 270
#define UPPER 271
#define HIGH 272
#define LOW 273
#define LSH 274
#define RSH 275
#define GREATER_EQUAL 276
#define LESS_EQUAL 277
#define EQUAL 278
#define NOT_EQUAL 279
#define LOGICAL_AND 280
#define LOGICAL_OR 281
#define ASSIGN_PLUS 282
#define ASSIGN_MINUS 283
#define ASSIGN_MULTIPLY 284
#define ASSIGN_DIVIDE 285
#define ASSIGN_MODULUS 286
#define ASSIGN_LSH 287
#define ASSIGN_RSH 288
#define ASSIGN_AND 289
#define ASSIGN_OR 290
#define ASSIGN_XOR 291
#define INCREMENT 292
#define DECREMENT 293
#define POSTINCREMENT 294
#define POSTDECREMENT 295
#define INDFOFFSET 296
#define TBL_NO_CHANGE 297
#define TBL_POST_INC 298
#define TBL_POST_DEC 299
#define TBL_PRE_INC 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 323 "parse.y" /* yacc.c:1909  */

  gpasmVal i;
  char *s;
  struct pnode *p;

#line 150 "parse.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
