/* A Bison parser, made by GNU Bison 3.7.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_GUIDO_GUIDOPARSE_HPP_INCLUDED
# define YY_GUIDO_GUIDOPARSE_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int guidodebug;
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
    NUMBER = 258,                  /* NUMBER  */
    PNUMBER = 259,                 /* PNUMBER  */
    NNUMBER = 260,                 /* NNUMBER  */
    FLOAT = 261,                   /* FLOAT  */
    STARTCHORD = 262,              /* STARTCHORD  */
    ENDCHORD = 263,                /* ENDCHORD  */
    STARTSEQ = 264,                /* STARTSEQ  */
    ENDSEQ = 265,                  /* ENDSEQ  */
    STARTPARAM = 266,              /* STARTPARAM  */
    ENDPARAM = 267,                /* ENDPARAM  */
    STARTRANGE = 268,              /* STARTRANGE  */
    ENDRANGE = 269,                /* ENDRANGE  */
    SEP = 270,                     /* SEP  */
    IDSEP = 271,                   /* IDSEP  */
    BAR = 272,                     /* BAR  */
    TAGNAME = 273,                 /* TAGNAME  */
    IDT = 274,                     /* IDT  */
    DIATONIC = 275,                /* DIATONIC  */
    CHROMATIC = 276,               /* CHROMATIC  */
    SOLFEGE = 277,                 /* SOLFEGE  */
    EMPTYT = 278,                  /* EMPTYT  */
    RESTT = 279,                   /* RESTT  */
    DOT = 280,                     /* DOT  */
    DDOT = 281,                    /* DDOT  */
    TDOT = 282,                    /* TDOT  */
    SHARPT = 283,                  /* SHARPT  */
    FLATT = 284,                   /* FLATT  */
    TAB = 285,                     /* TAB  */
    MLS = 286,                     /* MLS  */
    SEC = 287,                     /* SEC  */
    UNIT = 288,                    /* UNIT  */
    MULT = 289,                    /* MULT  */
    DIV = 290,                     /* DIV  */
    EQUAL = 291,                   /* EQUAL  */
    STRING = 292,                  /* STRING  */
    EXTRA = 293,                   /* EXTRA  */
    ENDVAR = 294,                  /* ENDVAR  */
    VARNAME = 295,                 /* VARNAME  */
    FRETTE = 296                   /* FRETTE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 44 "guido.y"
         
	long int		num;
	float			real;
	const char*		token;
	std::string*	str;
	char			c;
	TagParameter*	param;
	ARMusicalTag*	tag;
//	GuidoParser::ParamsList*	plist;

#line 116 "guidoparse.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int guidoparse (GuidoParser* context);

#endif /* !YY_GUIDO_GUIDOPARSE_HPP_INCLUDED  */
