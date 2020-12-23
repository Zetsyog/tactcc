/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_INCLUDE_GRAMMAR_H_INCLUDED
# define YY_YY_INCLUDE_GRAMMAR_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PROG = 258,
    VAR = 259,
    UNIT = 260,
    BOOL = 261,
    INT = 262,
    ARRAY = 263,
    FUNC = 264,
    REF = 265,
    IF = 266,
    THEN = 267,
    ELSE = 268,
    WHILE = 269,
    RETURN = 270,
    BEGIN_TOK = 271,
    READ = 272,
    WRITE = 273,
    IDENT = 274,
    COM = 275,
    END = 276,
    AND = 277,
    OR = 278,
    XOR = 279,
    NOT = 280,
    DO = 281,
    OF = 282
  };
#endif
/* Tokens.  */
#define PROG 258
#define VAR 259
#define UNIT 260
#define BOOL 261
#define INT 262
#define ARRAY 263
#define FUNC 264
#define REF 265
#define IF 266
#define THEN 267
#define ELSE 268
#define WHILE 269
#define RETURN 270
#define BEGIN_TOK 271
#define READ 272
#define WRITE 273
#define IDENT 274
#define COM 275
#define END 276
#define AND 277
#define OR 278
#define XOR 279
#define NOT 280
#define DO 281
#define OF 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_GRAMMAR_H_INCLUDED  */
