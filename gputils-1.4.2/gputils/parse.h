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
    LEXEOF = 0,
    SECTION = 258,
    TYPE = 259,
    SYMBOL = 260,
    NUMBER = 261,
    COMMENT = 262,
    EOL = 263,
    MUL = 264,
    DIV = 265,
    MOD = 266,
    PLUS = 267,
    MINUS = 268,
    LSH = 269,
    RSH = 270,
    AND = 271,
    XOR = 272,
    OR = 273,
    ASSIGN = 274,
    UMINUS = 275
  };
#endif
/* Tokens.  */
#define LEXEOF 0
#define SECTION 258
#define TYPE 259
#define SYMBOL 260
#define NUMBER 261
#define COMMENT 262
#define EOL 263
#define MUL 264
#define DIV 265
#define MOD 266
#define PLUS 267
#define MINUS 268
#define LSH 269
#define RSH 270
#define AND 271
#define XOR 272
#define OR 273
#define ASSIGN 274
#define UMINUS 275

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 51 "parse.y" /* yacc.c:1909  */

  unsigned int code;
  long value;
  char *string;

#line 102 "parse.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSE_H_INCLUDED  */
