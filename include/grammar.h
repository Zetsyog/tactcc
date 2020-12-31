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
    REF = 260,
    BEGIN_TOK = 261,
    END_TOK = 262,
    ASSIGN = 263,
    ARRAY = 264,
    OF = 265,
    FUNC = 266,
    RETURN = 267,
    WHILE = 268,
    IF = 269,
    THEN = 270,
    ELSE = 271,
    DO = 272,
    READ = 273,
    WRITE = 274,
    AND = 275,
    OR = 276,
    XOR = 277,
    NOT = 278,
    TRUE = 279,
    FALSE = 280,
    EQUAL = 281,
    NOT_EQUAL = 282,
    LOWER_EQUAL = 283,
    UPPER_EQUAL = 284,
    UNIT = 285,
    BOOL = 286,
    INT = 287,
    IDENT = 288,
    STR_CST = 289,
    OPU = 290,
    NO_ELSE = 291
  };
#endif
/* Tokens.  */
#define PROG 258
#define VAR 259
#define REF 260
#define BEGIN_TOK 261
#define END_TOK 262
#define ASSIGN 263
#define ARRAY 264
#define OF 265
#define FUNC 266
#define RETURN 267
#define WHILE 268
#define IF 269
#define THEN 270
#define ELSE 271
#define DO 272
#define READ 273
#define WRITE 274
#define AND 275
#define OR 276
#define XOR 277
#define NOT 278
#define TRUE 279
#define FALSE 280
#define EQUAL 281
#define NOT_EQUAL 282
#define LOWER_EQUAL 283
#define UPPER_EQUAL 284
#define UNIT 285
#define BOOL 286
#define INT 287
#define IDENT 288
#define STR_CST 289
#define OPU 290
#define NO_ELSE 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 20 "src/grammar/grammar.y"

    struct list_t *pos;
    struct expr_val_t expr_val;
    struct {
        struct list_t *next;
    } instr_val;
    struct node_t *list;
    unsigned int quad;
    int intVal;
    char strVal[SYM_NAME_MAX_LEN];
    struct symbol_t *sym;
    unsigned int a_type;
    enum operation_t operation;

#line 144 "./include/grammar.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_GRAMMAR_H_INCLUDED  */
