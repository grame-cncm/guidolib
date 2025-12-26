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
     NUMBER = 258,
     PNUMBER = 259,
     NNUMBER = 260,
     FLOAT = 261,
     STARTCHORD = 262,
     ENDCHORD = 263,
     STARTSEQ = 264,
     ENDSEQ = 265,
     STARTPARAM = 266,
     ENDPARAM = 267,
     STARTRANGE = 268,
     ENDRANGE = 269,
     SEP = 270,
     IDSEP = 271,
     BAR = 272,
     TAGNAME = 273,
     ID = 274,
     DIATONIC = 275,
     CHROMATIC = 276,
     SOLFEGE = 277,
     EMPTY = 278,
     REST = 279,
     DOT = 280,
     DDOT = 281,
     SHARP = 282,
     FLAT = 283,
     MLS = 284,
     SEC = 285,
     UNIT = 286,
     MULT = 287,
     DIV = 288,
     EQUAL = 289,
     STRING = 290,
     EXTRA = 291
   };
#endif
/* Tokens.  */
#define NUMBER 258
#define PNUMBER 259
#define NNUMBER 260
#define FLOAT 261
#define STARTCHORD 262
#define ENDCHORD 263
#define STARTSEQ 264
#define ENDSEQ 265
#define STARTPARAM 266
#define ENDPARAM 267
#define STARTRANGE 268
#define ENDRANGE 269
#define SEP 270
#define IDSEP 271
#define BAR 272
#define TAGNAME 273
#define ID 274
#define DIATONIC 275
#define CHROMATIC 276
#define SOLFEGE 277
#define EMPTY 278
#define REST 279
#define DOT 280
#define DDOT 281
#define SHARP 282
#define FLAT 283
#define MLS 284
#define SEC 285
#define UNIT 286
#define MULT 287
#define DIV 288
#define EQUAL 289
#define STRING 290
#define EXTRA 291




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 48 "guido.y"
{         
	long int		num;
	float			real;
	const char*		token;
	std::string*	str;
	char			c;
	guido::Sguidoelement *		elt;
	guido::Sguidoattribute*		attr;
	std::vector<guido::Sguidoelement>*	 velt;
	std::vector<guido::Sguidoattribute>* vattr;
	guido::rational *		r;
}
/* Line 1529 of yacc.c.  */
#line 134 "guidoparse.h++"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE guidoarlval;

