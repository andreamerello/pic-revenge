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

#ifndef YY_PP_PPPARSE_H_INCLUDED
# define YY_PP_PPPARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int ppdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    HV = 258,
    NUMBER = 259,
    IDENTIFIER = 260,
    LOGICAL_OR = 261,
    LOGICAL_AND = 262,
    EQUAL = 263,
    NOT_EQUAL = 264,
    GREATER_EQUAL = 265,
    LESS_EQUAL = 266,
    LSH = 267,
    RSH = 268,
    UPPER = 269,
    HIGH = 270,
    LOW = 271,
    NEG = 272,
    POS = 273
  };
#endif
/* Tokens.  */
#define HV 258
#define NUMBER 259
#define IDENTIFIER 260
#define LOGICAL_OR 261
#define LOGICAL_AND 262
#define EQUAL 263
#define NOT_EQUAL 264
#define GREATER_EQUAL 265
#define LESS_EQUAL 266
#define LSH 267
#define RSH 268
#define UPPER 269
#define HIGH 270
#define LOW 271
#define NEG 272
#define POS 273

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 37 "ppparse.y" /* yacc.c:1909  */

  gpasmVal i;
  char *s;

#line 95 "ppparse.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE pplval;

int ppparse (void);

#endif /* !YY_PP_PPPARSE_H_INCLUDED  */
