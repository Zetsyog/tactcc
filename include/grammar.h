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
    ARRAY = 262,
    FUNC = 263,
    REF = 264,
    IF = 265,
    THEN = 266,
    ELSE = 267,
    WHILE = 268,
    RETURN = 269,
    BEGIN_TOK = 270,
    READ = 271,
    WRITE = 272,
    END_TOK = 273,
    AND = 274,
    OR = 275,
    XOR = 276,
    NOT = 277,
    DO = 278,
    OF = 279,
    TRUE = 280,
    FALSE = 281,
    INT = 282,
    IDENT = 283,
    STR_CST = 284,
    OPU = 285
  };
#endif
/* Tokens.  */
#define PROG 258
#define VAR 259
#define UNIT 260
#define BOOL 261
#define ARRAY 262
#define FUNC 263
#define REF 264
#define IF 265
#define THEN 266
#define ELSE 267
#define WHILE 268
#define RETURN 269
#define BEGIN_TOK 270
#define READ 271
#define WRITE 272
#define END_TOK 273
#define AND 274
#define OR 275
#define XOR 276
#define NOT 277
#define DO 278
#define OF 279
#define TRUE 280
#define FALSE 281
#define INT 282
#define IDENT 283
#define STR_CST 284
#define OPU 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 19 "src/grammar/grammar.y"

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

#line 132 "./include/grammar.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_GRAMMAR_H_INCLUDED  */
