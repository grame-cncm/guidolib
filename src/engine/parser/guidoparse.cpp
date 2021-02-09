/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 1

/* Substitute the variable and function names.  */
#define yyparse guidoparse
#define yylex   guidolex
#define yyerror guidoerror
#define yylval  guidolval
#define yychar  guidochar
#define yydebug guidodebug
#define yynerrs guidonerrs
#define yylloc guidolloc

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
     IDT = 274,
     DIATONIC = 275,
     CHROMATIC = 276,
     SOLFEGE = 277,
     EMPTYT = 278,
     RESTT = 279,
     DOT = 280,
     DDOT = 281,
     TDOT = 282,
     SHARPT = 283,
     FLATT = 284,
     MLS = 285,
     SEC = 286,
     UNIT = 287,
     MULT = 288,
     DIV = 289,
     EQUAL = 290,
     STRING = 291,
     EXTRA = 292,
     ENDVAR = 293,
     VARNAME = 294
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
#define IDT 274
#define DIATONIC 275
#define CHROMATIC 276
#define SOLFEGE 277
#define EMPTYT 278
#define RESTT 279
#define DOT 280
#define DDOT 281
#define TDOT 282
#define SHARPT 283
#define FLATT 284
#define MLS 285
#define SEC 286
#define UNIT 287
#define MULT 288
#define DIV 289
#define EQUAL 290
#define STRING 291
#define EXTRA 292
#define ENDVAR 293
#define VARNAME 294




/* Copy the first part of user declarations.  */
#line 1 "guido.y"


#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifdef TEST
#include "tests/GuidoParserTest.h"
#else
#include "GuidoParser.h"
#endif
#include "gddefs.h"

#ifdef WIN32
# pragma warning (disable : 4267 4005)
#endif

#include "guidoparse.hpp"

#define YYERROR_VERBOSE
int guidoerror (YYLTYPE* locp, GuidoParser* context, const char*s);
int varerror (int line, int column, GuidoParser* p, const char* varname);
int yylex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);

#define scanner context->fScanner

class TagParameter;
class ARMusicalTag;

using namespace std;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 44 "guido.y"
{         
	long int		num;
	float			real;
	const char*		token;
	std::string*	str;
	char			c;
	TagParameter*	param;
	ARMusicalTag*	tag;
//	GuidoParser::ParamsList*	plist;
}
/* Line 193 of yacc.c.  */
#line 227 "guidoparse.cpp"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 252 "guidoparse.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
    YYLTYPE yyls;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   126

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  50
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNRULES -- Number of states.  */
#define YYNSTATES  140

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   294

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    13,    14,    19,    20,
      23,    25,    29,    30,    35,    36,    39,    42,    45,    48,
      50,    53,    58,    63,    68,    70,    72,    74,    76,    81,
      82,    88,    90,    92,    96,    98,   100,   102,   105,   108,
     110,   112,   114,   116,   120,   122,   126,   127,   132,   133,
     136,   137,   142,   144,   147,   150,   154,   156,   158,   159,
     165,   167,   170,   172,   174,   175,   180,   181,   189,   194,
     200,   202,   207,   209,   211,   213,   215,   217,   220,   222,
     224,   225,   227,   228,   233,   236,   240,   243,   244,   246,
     248,   250,   252,   254,   256,   258,   260,   262,   264
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      41,     0,    -1,    42,    -1,    50,    42,    -1,    -1,     7,
      43,     8,    -1,    -1,     7,    44,    46,     8,    -1,    -1,
      45,    47,    -1,    47,    -1,    46,    15,    47,    -1,    -1,
       9,    48,    49,    10,    -1,    -1,    49,    72,    -1,    49,
      53,    -1,    49,    62,    -1,    49,    52,    -1,    51,    -1,
      50,    51,    -1,    52,    35,    36,    38,    -1,    52,    35,
      89,    38,    -1,    52,    35,    88,    38,    -1,    39,    -1,
      54,    -1,    55,    -1,    58,    -1,    58,    11,    61,    12,
      -1,    -1,    54,    13,    56,    49,    14,    -1,    18,    -1,
      57,    -1,    57,    16,    85,    -1,    17,    -1,    89,    -1,
      88,    -1,    89,    32,    -1,    88,    32,    -1,    36,    -1,
      84,    -1,    52,    -1,    59,    -1,    84,    35,    59,    -1,
      60,    -1,    61,    15,    60,    -1,    -1,     7,    63,    64,
       8,    -1,    -1,    65,    67,    -1,    -1,    64,    15,    66,
      67,    -1,    68,    -1,    71,    68,    -1,    68,    71,    -1,
      71,    68,    71,    -1,    72,    -1,    69,    -1,    -1,    54,
      13,    70,    67,    14,    -1,    54,    -1,    71,    54,    -1,
      76,    -1,    73,    -1,    -1,    24,    74,    82,    83,    -1,
      -1,    24,    75,    11,     3,    12,    82,    83,    -1,    77,
      81,    82,    83,    -1,    77,    79,    81,    82,    83,    -1,
      78,    -1,    78,    11,     3,    12,    -1,    20,    -1,    21,
      -1,    22,    -1,    23,    -1,    80,    -1,    79,    80,    -1,
      28,    -1,    29,    -1,    -1,    89,    -1,    -1,    33,    85,
      34,    85,    -1,    33,    85,    -1,    33,    85,    30,    -1,
      34,    85,    -1,    -1,    25,    -1,    26,    -1,    27,    -1,
      19,    -1,     3,    -1,     4,    -1,     5,    -1,     6,    -1,
      85,    -1,    86,    -1,    87,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   119,   119,   120,   123,   123,   124,   124,   125,   125,
     128,   129,   132,   132,   135,   136,   137,   138,   139,   148,
     149,   152,   153,   154,   157,   162,   163,   166,   167,   170,
     170,   173,   176,   177,   178,   181,   182,   183,   184,   185,
     186,   187,   194,   195,   198,   199,   205,   205,   208,   208,
     209,   209,   212,   213,   214,   215,   218,   219,   222,   222,
     225,   226,   232,   233,   236,   236,   237,   237,   240,   241,
     244,   245,   248,   249,   250,   251,   254,   255,   258,   259,
     262,   263,   266,   267,   268,   269,   270,   273,   274,   275,
     276,   281,   283,   285,   287,   289,   292,   293,   294
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMBER", "PNUMBER", "NNUMBER", "FLOAT",
  "STARTCHORD", "ENDCHORD", "STARTSEQ", "ENDSEQ", "STARTPARAM", "ENDPARAM",
  "STARTRANGE", "ENDRANGE", "SEP", "IDSEP", "BAR", "TAGNAME", "IDT",
  "DIATONIC", "CHROMATIC", "SOLFEGE", "EMPTYT", "RESTT", "DOT", "DDOT",
  "TDOT", "SHARPT", "FLATT", "MLS", "SEC", "UNIT", "MULT", "DIV", "EQUAL",
  "STRING", "EXTRA", "ENDVAR", "VARNAME", "$accept", "gmn", "score", "@1",
  "@2", "@3", "voicelist", "voice", "@4", "symbols", "variables",
  "vardecl", "varname", "tag", "positiontag", "rangetag", "@5", "tagname",
  "tagid", "tagarg", "tagparam", "tagparams", "chord", "@6",
  "chordsymbols", "@7", "@8", "tagchordsymbol", "chordsymbol",
  "rangechordtag", "@9", "taglist", "music", "rest", "@10", "@11", "note",
  "noteid", "notename", "accidentals", "accidental", "octave", "duration",
  "dots", "id", "number", "pnumber", "nnumber", "floatn", "signednumber", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    41,    43,    42,    44,    42,    45,    42,
      46,    46,    48,    47,    49,    49,    49,    49,    49,    50,
      50,    51,    51,    51,    52,    53,    53,    54,    54,    56,
      55,    57,    58,    58,    58,    59,    59,    59,    59,    59,
      59,    59,    60,    60,    61,    61,    63,    62,    65,    64,
      66,    64,    67,    67,    67,    67,    68,    68,    70,    69,
      71,    71,    72,    72,    74,    73,    75,    73,    76,    76,
      77,    77,    78,    78,    78,    78,    79,    79,    80,    80,
      81,    81,    82,    82,    82,    82,    82,    83,    83,    83,
      83,    84,    85,    86,    87,    88,    89,    89,    89
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     0,     4,     0,     2,
       1,     3,     0,     4,     0,     2,     2,     2,     2,     1,
       2,     4,     4,     4,     1,     1,     1,     1,     4,     0,
       5,     1,     1,     3,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     3,     1,     3,     0,     4,     0,     2,
       0,     4,     1,     2,     2,     3,     1,     1,     0,     5,
       1,     2,     1,     1,     0,     4,     0,     7,     4,     5,
       1,     4,     1,     1,     1,     1,     1,     2,     1,     1,
       0,     1,     0,     4,     2,     3,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       8,     4,    24,     0,     2,     0,     8,    19,     0,     0,
       0,     1,    12,     9,     3,    20,     0,     5,     0,    10,
      14,    92,    93,    94,    95,     0,    96,    97,    98,     0,
       0,     7,     0,     0,    21,    23,    22,    11,    46,    13,
      34,    31,    72,    73,    74,    75,    64,    18,    16,    25,
      26,    32,    27,    17,    15,    63,    62,    80,    70,    48,
      82,     0,    29,     0,     0,    78,    79,    80,    76,    82,
      81,     0,     0,     0,     0,     0,    87,     0,    14,    33,
      91,    39,    41,    42,    44,     0,    40,    36,    35,    77,
      82,    87,     0,    47,    50,    60,    49,    52,    57,     0,
      56,    84,    86,    88,    89,    90,    65,     0,     0,    28,
       0,     0,    38,    37,    87,    68,    71,     0,    58,    60,
      54,    61,    53,    85,     0,    82,    30,    45,    43,    40,
      69,    51,     0,    61,    55,    83,    87,     0,    67,    59
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     9,    10,     5,    18,    13,    20,    33,
       6,     7,    82,    48,    95,    50,    78,    51,    52,    83,
      84,    85,    53,    59,    72,    73,   117,    96,    97,    98,
     132,    99,   100,    55,    60,    61,    56,    57,    58,    67,
      68,    69,    76,   106,    86,    26,    27,    28,    87,    88
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -106
static const yytype_int8 yypact[] =
{
       6,    20,  -106,    51,  -106,    48,     6,  -106,    31,    67,
      48,  -106,  -106,  -106,  -106,  -106,    13,  -106,    35,  -106,
    -106,  -106,  -106,  -106,  -106,    21,  -106,  -106,  -106,    42,
      55,  -106,    48,    61,  -106,  -106,  -106,  -106,  -106,  -106,
    -106,  -106,  -106,  -106,  -106,  -106,    78,  -106,  -106,    86,
    -106,    85,    92,  -106,  -106,  -106,  -106,    58,    93,  -106,
      36,    94,  -106,   103,     5,  -106,  -106,    58,  -106,    36,
    -106,   104,    40,    74,   103,   103,    47,   106,  -106,  -106,
    -106,  -106,  -106,  -106,  -106,    11,    75,    79,    80,  -106,
      36,    47,   101,  -106,  -106,   102,  -106,    22,  -106,    74,
    -106,    24,  -106,  -106,  -106,  -106,  -106,   105,    14,  -106,
       5,     5,  -106,  -106,    47,  -106,  -106,    74,  -106,  -106,
      22,   102,    22,  -106,   103,    36,  -106,  -106,  -106,  -106,
    -106,  -106,    74,  -106,    22,  -106,    47,   100,  -106,  -106
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -106,  -106,   110,  -106,  -106,  -106,  -106,    10,  -106,    41,
    -106,   112,     0,  -106,   -32,  -106,  -106,  -106,  -106,     9,
      12,  -106,  -106,  -106,  -106,  -106,  -106,  -105,    25,  -106,
    -106,   -75,   -31,  -106,  -106,  -106,  -106,  -106,  -106,  -106,
      54,    56,   -65,   -84,    15,   -60,  -106,  -106,   109,   -11
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -67
static const yytype_int16 yytable[] =
{
       8,    49,    54,    79,    91,    30,     8,   115,    21,    22,
      23,    24,   131,     1,   101,   102,    21,    22,    23,    24,
      19,    38,   120,   109,    80,   114,   110,   137,   126,    -6,
     130,    40,    41,    47,    42,    43,    44,    45,    46,    40,
      41,    81,    37,    31,     2,     2,    70,   134,    93,    25,
      32,    11,   138,     2,   123,    94,    70,    12,   124,    34,
     136,    21,    22,    23,   135,   119,    16,   121,    38,    74,
      75,    39,   103,   104,   105,    17,    49,    54,    40,    41,
      35,    42,    43,    44,    45,    46,    65,    66,   133,   -66,
     119,    40,    41,    36,    42,    43,    44,    45,    46,    62,
       2,    63,   133,    64,    71,    77,    21,    92,    47,   107,
     111,   112,   113,   116,   139,   118,    14,   125,    15,   108,
     128,    89,   127,    90,   122,    29,   129
};

static const yytype_uint8 yycheck[] =
{
       0,    33,    33,    63,    69,    16,     6,    91,     3,     4,
       5,     6,   117,     7,    74,    75,     3,     4,     5,     6,
      10,     7,    97,    12,    19,    90,    15,   132,    14,     9,
     114,    17,    18,    33,    20,    21,    22,    23,    24,    17,
      18,    36,    32,     8,    39,    39,    57,   122,     8,    36,
      15,     0,   136,    39,    30,    15,    67,     9,    34,    38,
     125,     3,     4,     5,   124,    97,    35,    99,     7,    33,
      34,    10,    25,    26,    27,     8,   108,   108,    17,    18,
      38,    20,    21,    22,    23,    24,    28,    29,   120,    11,
     122,    17,    18,    38,    20,    21,    22,    23,    24,    13,
      39,    16,   134,    11,    11,    11,     3,     3,   108,     3,
      35,    32,    32,    12,    14,    13,     6,    12,     6,    78,
     111,    67,   110,    67,    99,    16,   111
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,    39,    41,    42,    45,    50,    51,    52,    43,
      44,     0,     9,    47,    42,    51,    35,     8,    46,    47,
      48,     3,     4,     5,     6,    36,    85,    86,    87,    88,
      89,     8,    15,    49,    38,    38,    38,    47,     7,    10,
      17,    18,    20,    21,    22,    23,    24,    52,    53,    54,
      55,    57,    58,    62,    72,    73,    76,    77,    78,    63,
      74,    75,    13,    16,    11,    28,    29,    79,    80,    81,
      89,    11,    64,    65,    33,    34,    82,    11,    56,    85,
      19,    36,    52,    59,    60,    61,    84,    88,    89,    80,
      81,    82,     3,     8,    15,    54,    67,    68,    69,    71,
      72,    85,    85,    25,    26,    27,    83,     3,    49,    12,
      15,    35,    32,    32,    82,    83,    12,    66,    13,    54,
      71,    54,    68,    30,    34,    12,    14,    60,    59,    84,
      83,    67,    70,    54,    71,    85,    82,    67,    83,    14
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (&yylloc, context, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, scanner)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, Location, context); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, GuidoParser* context)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, context)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    GuidoParser* context;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (context);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, GuidoParser* context)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, context)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    GuidoParser* context;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, context);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, GuidoParser* context)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, context)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    GuidoParser* context;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , context);
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, context); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, GuidoParser* context)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, context)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    GuidoParser* context;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (context);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (GuidoParser* context);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (GuidoParser* context)
#else
int
yyparse (context)
    GuidoParser* context;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;
/* Location data for the look-ahead symbol.  */
YYLTYPE yylloc;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;

  /* The location stack.  */
  YYLTYPE yylsa[YYINITDEPTH];
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;
  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[2];

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;
#if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 0;
#endif

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);
	yyls = yyls1;
	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);
	YYSTACK_RELOCATE (yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 123 "guido.y"
    { context->segmInit (); ;}
    break;

  case 5:
#line 123 "guido.y"
    { context->segmExit (); ;}
    break;

  case 6:
#line 124 "guido.y"
    { context->segmInit (); ;}
    break;

  case 7:
#line 124 "guido.y"
    { context->segmExit (); ;}
    break;

  case 8:
#line 125 "guido.y"
    { context->segmInit (); ;}
    break;

  case 9:
#line 125 "guido.y"
    { context->segmExit (); ;}
    break;

  case 10:
#line 128 "guido.y"
    { context->segmAppendSeq (); ;}
    break;

  case 11:
#line 129 "guido.y"
    { context->segmAppendSeq (); ;}
    break;

  case 12:
#line 132 "guido.y"
    { context->seqInit (); ;}
    break;

  case 13:
#line 132 "guido.y"
    { context->seqExit( ); ;}
    break;

  case 15:
#line 136 "guido.y"
    { context->appendNote (); ;}
    break;

  case 17:
#line 138 "guido.y"
    { context->seqAppendChord (); ;}
    break;

  case 18:
#line 139 "guido.y"
    { bool ret = context->variableSymbols ((yyvsp[(2) - (2)].str)->c_str()); 
															  if (!ret) varerror ((yylsp[(2) - (2)]).last_line, (yylsp[(2) - (2)]).first_column, context, (yyvsp[(2) - (2)].str)->c_str());
															  delete (yyvsp[(2) - (2)].str);
															  if (!ret)  YYABORT;
															;}
    break;

  case 21:
#line 152 "guido.y"
    { context->variableDecl ((yyvsp[(1) - (4)].str)->c_str(), context->fText.c_str(), GuidoParser::kString); delete (yyvsp[(1) - (4)].str); ;}
    break;

  case 22:
#line 153 "guido.y"
    { context->variableDecl ((yyvsp[(1) - (4)].str)->c_str(), context->fText.c_str(), GuidoParser::kInt); delete (yyvsp[(1) - (4)].str); ;}
    break;

  case 23:
#line 154 "guido.y"
    { context->variableDecl ((yyvsp[(1) - (4)].str)->c_str(), context->fText.c_str(), GuidoParser::kFloat); delete (yyvsp[(1) - (4)].str); ;}
    break;

  case 24:
#line 157 "guido.y"
    { (yyval.str) = new string(context->fText); ;}
    break;

  case 25:
#line 162 "guido.y"
    { context->tagEnd (); ;}
    break;

  case 26:
#line 163 "guido.y"
    { context->tagEnd (); ;}
    break;

  case 27:
#line 166 "guido.y"
    { context->tagAdd (); ;}
    break;

  case 28:
#line 167 "guido.y"
    { context->tagAdd (); ;}
    break;

  case 29:
#line 170 "guido.y"
    { context->tagRange (); ;}
    break;

  case 31:
#line 173 "guido.y"
    { (yyval.str) = new string(context->fText); ;}
    break;

  case 32:
#line 176 "guido.y"
    { context->tagStart ( (yyvsp[(1) - (1)].str)->c_str(), 0); delete (yyvsp[(1) - (1)].str); ;}
    break;

  case 33:
#line 177 "guido.y"
    { context->tagStart ( (yyvsp[(1) - (3)].str)->c_str(),(yyvsp[(3) - (3)].num)); delete (yyvsp[(1) - (3)].str); ;}
    break;

  case 34:
#line 178 "guido.y"
    { context->tagStart ( "\\bar", 0); ;}
    break;

  case 35:
#line 181 "guido.y"
    { (yyval.param) = context->intParam   ((yyvsp[(1) - (1)].num)) ;}
    break;

  case 36:
#line 182 "guido.y"
    { (yyval.param) = context->floatParam ((yyvsp[(1) - (1)].real)) ;}
    break;

  case 37:
#line 183 "guido.y"
    { (yyval.param) = context->intParam   ((yyvsp[(1) - (2)].num), context->fText.c_str() ) ;}
    break;

  case 38:
#line 184 "guido.y"
    { (yyval.param) = context->floatParam ((yyvsp[(1) - (2)].real), context->fText.c_str() ) ;}
    break;

  case 39:
#line 185 "guido.y"
    { (yyval.param) = context->strParam   (context->fText.c_str() ) ;}
    break;

  case 40:
#line 186 "guido.y"
    { /* unused */ (yyval.param) = 0; delete (yyvsp[(1) - (1)].str); ;}
    break;

  case 41:
#line 187 "guido.y"
    { (yyval.param) = context->varParam ((yyvsp[(1) - (1)].str)->c_str() ); 
															  if (!(yyval.param)) varerror ((yylsp[(1) - (1)]).last_line, (yylsp[(1) - (1)]).first_column, context, (yyvsp[(1) - (1)].str)->c_str());
															  delete (yyvsp[(1) - (1)].str);
															  if (!(yyval.param)) YYABORT; 
															;}
    break;

  case 42:
#line 194 "guido.y"
    { (yyval.param) = (yyvsp[(1) - (1)].param); ;}
    break;

  case 43:
#line 195 "guido.y"
    { if((yyvsp[(3) - (3)].param)) context->setParamName ((yyvsp[(3) - (3)].param), (yyvsp[(1) - (3)].str)->c_str()); (yyval.param) = (yyvsp[(3) - (3)].param); delete (yyvsp[(1) - (3)].str); ;}
    break;

  case 44:
#line 198 "guido.y"
    { context->tagParameter ((yyvsp[(1) - (1)].param)); ;}
    break;

  case 45:
#line 199 "guido.y"
    { context->tagParameter ((yyvsp[(3) - (3)].param)) ; ;}
    break;

  case 46:
#line 205 "guido.y"
    { context->chordInit (); ;}
    break;

  case 48:
#line 208 "guido.y"
    { context->chordInitNote (); ;}
    break;

  case 50:
#line 209 "guido.y"
    { context->chordInitNote ();;}
    break;

  case 56:
#line 218 "guido.y"
    { context->appendNote ();  ;}
    break;

  case 58:
#line 222 "guido.y"
    { context->tagRange (); ;}
    break;

  case 59:
#line 222 "guido.y"
    { context->tagEnd (); ;}
    break;

  case 60:
#line 225 "guido.y"
    { context->tagEnd (); ;}
    break;

  case 61:
#line 226 "guido.y"
    { context->tagEnd (); ;}
    break;

  case 64:
#line 236 "guido.y"
    { context->noteInit ( "_" ); ;}
    break;

  case 66:
#line 237 "guido.y"
    { context->noteInit ( "_" ); ;}
    break;

  case 72:
#line 248 "guido.y"
    { context->noteInit ( context->fText.c_str() ); ;}
    break;

  case 73:
#line 249 "guido.y"
    { context->noteInit ( context->fText.c_str() ); ;}
    break;

  case 74:
#line 250 "guido.y"
    { context->noteInit ( context->fText.c_str() ); ;}
    break;

  case 75:
#line 251 "guido.y"
    { context->noteInit ( context->fText.c_str() ); ;}
    break;

  case 78:
#line 258 "guido.y"
    {  context->noteAcc (SHARP); ;}
    break;

  case 79:
#line 259 "guido.y"
    {  context->noteAcc (FLAT); ;}
    break;

  case 81:
#line 263 "guido.y"
    { context->noteOct ((yyvsp[(1) - (1)].num)); ;}
    break;

  case 83:
#line 267 "guido.y"
    {  context->noteEnum ((yyvsp[(2) - (4)].num)); context->noteDenom ((yyvsp[(4) - (4)].num)); ;}
    break;

  case 84:
#line 268 "guido.y"
    {  context->noteEnum ((yyvsp[(2) - (2)].num));  ;}
    break;

  case 85:
#line 269 "guido.y"
    {  context->noteAbsDur((yyvsp[(2) - (3)].num));  ;}
    break;

  case 86:
#line 270 "guido.y"
    {  context->noteDenom ((yyvsp[(2) - (2)].num)); ;}
    break;

  case 88:
#line 274 "guido.y"
    {  context->noteDot  ();  ;}
    break;

  case 89:
#line 275 "guido.y"
    {  context->noteDdot (); ;}
    break;

  case 90:
#line 276 "guido.y"
    {  context->noteTdot (); ;}
    break;

  case 91:
#line 281 "guido.y"
    { (yyval.str) = new string(context->fText); ;}
    break;

  case 92:
#line 283 "guido.y"
    { (yyval.num) = atol(context->fText.c_str() ); ;}
    break;

  case 93:
#line 285 "guido.y"
    { (yyval.num) = atol(context->fText.c_str() ); ;}
    break;

  case 94:
#line 287 "guido.y"
    { (yyval.num) = atol(context->fText.c_str() ); ;}
    break;

  case 95:
#line 289 "guido.y"
    { (yyval.real) = atof(context->fText.c_str() ); ;}
    break;

  case 96:
#line 292 "guido.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); ;}
    break;

  case 97:
#line 293 "guido.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); ;}
    break;

  case 98:
#line 294 "guido.y"
    { (yyval.num) = (yyvsp[(1) - (1)].num); ;}
    break;


/* Line 1267 of yacc.c.  */
#line 1965 "guidoparse.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (&yylloc, context, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (&yylloc, context, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, context, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, &yylloc, context);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[0] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, context);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the look-ahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, context, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, context);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, context);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 296 "guido.y"

#ifdef TEST
int	gParseErrorLine = 0;
#else
extern int	gParseErrorLine;
#endif

static int _error(int line, int column, GuidoParser* p, const char* msg) {
	p->parseError (line, column, msg);
/*
	gParseErrorLine = line;		// for backward compatibility only
	p->setError (line, column, msg);
	cerr << "error line: " << line << " col: " << column << ": " << msg << endl;
*/
	return 0;
}

int varerror(int line, int column, GuidoParser* p, const char* varname) {
	string msg = "unknown variable ";
	msg += varname;
	return _error (line, column, p, msg.c_str());
}

int guidoerror(YYLTYPE* loc, GuidoParser* p, const char*s) {
	return _error (loc->last_line, loc->first_column, p, s);
}

int GuidoParser::_yyparse()		{ return yyparse (this); }

