/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     GMN = 258,
     GROUPSTART = 259,
     GROUPEND = 260,
     ASEP = 261,
     EQ = 262,
     IDENT = 263,
     BOTTOM = 264,
     TOP = 265,
     TAIL = 266,
     HEAD = 267,
     PAR = 268,
     SEQ = 269,
     APPLY = 270,
     ABSTRACT = 271
   };
#endif
/* Tokens.  */
#define GMN 258
#define GROUPSTART 259
#define GROUPEND 260
#define ASEP 261
#define EQ 262
#define IDENT 263
#define BOTTOM 264
#define TOP 265
#define TAIL 266
#define HEAD 267
#define PAR 268
#define SEQ 269
#define APPLY 270
#define ABSTRACT 271




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 30 "glang.y"
{         
	SGLExpr*  exprPtr;
	std::string * strPtr;
}
/* Line 1529 of yacc.c.  */
#line 86 "glangparse.hpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE glanglval;

