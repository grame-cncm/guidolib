 //
 // json lexical elements
 // based on the definition published at http://json.org
 // D. Fober - Grame 2013
 //

%option reentrant
%option bison-locations
%option never-interactive
%option noyywrap
%option nounput
%option yylineno
%option yymore

%{

#include <iostream>
#include "json_parser.h"
#include "json_object.h"
#include "json_element.h"
#include "json_array.h"
#include "json_value.h"
#include "jsonparse.hpp"

using namespace std;

#define YY_EXTRA_TYPE json::json_parser*
#define YY_USER_ACTION yylloc->last_line = yylineno; yylloc->first_column += strlen(yytext);

#define YY_INPUT(buf,result,max_size)   \
   {                                       \
	  char c;                              \
      yyextra->fStream->get( c );                 \
	  if (yyextra->fStream->eof())               \
         result = YY_NULL;                 \
      else {                               \
         buf[0] = c;                       \
         result = 1;                       \
      }                                    \
   }

#define YY_NO_UNISTD_H

// ----------------------------------------------------------------------------------------

%}

 /* for quoted strings */
%x QUOTEDSTR

DIGIT		[0-9]
NNDIGIT		[1-9]
HEXDIGIT	[0-9a-fA-F]

SPACE		[ \t]
EOL			[\x0a\x0d]
 
QUOTE		"\\\""
REVSOL		"\\\\"
SOLIDUS		"\\/"
BACKSP		"\\b"
FF			"\\f"
NL			"\\n"
CR			"\\r"
TAB			"\\t"
 /* HEXMARK		"\\u"*/
HEXMARK		"\\u"{HEXDIGIT}{HEXDIGIT}{HEXDIGIT}{HEXDIGIT}

%%

 /* ----------------------------------------- */
 /*	digits */
 /* ----------------------------------------- */
{DIGIT}										yyextra->fInt = atoi(yytext); return INT;
{NNDIGIT}{DIGIT}+							yyextra->fInt = atoi(yytext); return INT;
"-"{DIGIT}									yyextra->fInt = atoi(yytext); return INT;
"-"{NNDIGIT}{DIGIT}+						yyextra->fInt = atoi(yytext); return INT;

[-]*{DIGIT}+"."{DIGIT}*						yyextra->fFloat = atof(yytext); return FLOAT;
[-]*{DIGIT}+"."{DIGIT}+[eE][-+]?{DIGIT}+ 	yyextra->fFloat = atof(yytext); return FLOAT;
[-]*{DIGIT}+[eE][-+]?{DIGIT}+				yyextra->fFloat = atof(yytext); return FLOAT;

"{"				return OBJSTART;
"}"				return OBJEND;

"["				return ARRAYSTART;
"]"				return ARRAYEND;

","				return COMMA;
":"				return COLON;

"true"			return TOKTRUE;
"false"			return TOKFALSE;
"null"			return TOKNULL;

 /* ----------------------------------------- */
 /*	quoted strings */
 /* ----------------------------------------- */


"\"\""			yyextra->fText = ""; return QUOTEDSTRING;

"\""							BEGIN QUOTEDSTR;
 /*<QUOTEDSTR>([^\"\\]|{QUOTE}|{REVSOL}|{SOLIDUS}|{BACKSP}|{FF}|{NL}|{CR}|{TAB}|{HEXMARK})*		yyextra->fText = yytext; return QUOTEDSTRING; */
<QUOTEDSTR>([^\"\\]|{QUOTE}|{REVSOL}|{SOLIDUS}|{BACKSP}|{FF}|{NL}|{CR}|{TAB}|{HEXMARK})*		yyextra->fText = yytext; return QUOTEDSTRING;
<QUOTEDSTR>\"					BEGIN INITIAL;
 
 
{SPACE}+		;   /* eat up space */

{EOL}			yylloc->first_column=0; /* ignore */

.				yyextra->fText = yytext; return ERR;

%%

void json::json_parser::initScanner()
{
	yylex_init(&fScanner);
	yyset_extra(this, fScanner);
}

void json::json_parser::destroyScanner()
{
	yylex_destroy(fScanner);
}


