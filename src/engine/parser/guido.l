%option reentrant
%option bison-locations
%option never-interactive
%option noyywrap
%option nounput
%option yylineno
%option yymore

%{


#ifdef WIN32
# pragma warning (disable : 4267 4005)
#endif


#include <iostream>
#include <stdlib.h>

#ifdef TEST
#include "tests/GuidoParserTest.h"
#else
#include "GuidoParser.h"
#endif
#include "guidoparse.hpp"

using namespace std;

#define YY_EXTRA_TYPE GuidoParser*
#define YY_USER_ACTION yylloc->last_line = yylineno; yylloc->first_column += strlen(yytext);

#define YY_INPUT(buf,result,max_size)  \
   {                                   \
	char c;                            \
	if (!yyextra->get(c) )             \
		result =  YY_NULL;             \
      else {                               \
         buf[0] = c;                       \
         result = 1;                       \
      }                                    \
   }

static int nested;

#define YY_NO_UNISTD_H
#define register		// this is to avoid deprecated register declarations

static  char* unescape (char * str) {
	int n = strlen(str);
	for (int i=0, j=0; i <= n; i++) {		
		if (str[i] == '\\') {
			switch (str[i+1]) {
				case '\\':
					str[j++] = str[i++];
					break;
				case 'n':
					str[j++] = '\n';
					i++;
					break;
				case '"':
				case '\'':
					break;

				case ';':
				case ' ':
					str[j++] = str[++i];
					break;
				
				default:
					str[j++] = str[i];
					break;
			}
		}
		else str[j++] = str[i];
	}
	return str;
}

static  char* unquote (char * str) {
	int n = strlen(str);
	for (int i=0; i < n; i++)
		str[i] = str[i+1];
	str[n-2] = 0;
	return str;
}

%}

%x CMNTLN
%x COMMENT
%S PARAM
%S VAR

DIGIT	[0-9]
ID		[a-z_A-Z][a-z_A-Z0-9]*
VARCHAR	[a-z_A-Z0-9]
LETTER	[a-zA-Z]
NOTE	[abcdefgh]
SPACE	[ \t]
EOL		[\x0a\x0d]

%%
{DIGIT}+					yyextra->fText = yytext; return NUMBER;
"+"{DIGIT}+					yyextra->fText = yytext; return PNUMBER;
"-"{DIGIT}+					yyextra->fText = yytext; return NNUMBER;

{DIGIT}*"."{DIGIT}+			yyextra->fText = yytext; return FLOAT;
"+"{DIGIT}*"."{DIGIT}+		yyextra->fText = yytext; return FLOAT;
"-"{DIGIT}*"."{DIGIT}+		yyextra->fText = yytext; return FLOAT;

{SPACE}*"%"			BEGIN CMNTLN;
<CMNTLN>.
<CMNTLN>{EOL}+		yylloc->first_column=0; BEGIN INITIAL;

{SPACE}*"(*"		nested=1; BEGIN COMMENT;
<COMMENT>.|{EOL}*
<COMMENT>"(*"		nested++;
<COMMENT>"*)"		if (--nested==0) BEGIN INITIAL;

"{"					return STARTCHORD;
"}"					return ENDCHORD;
","					return SEP;
":"					return IDSEP;
"["                 return STARTSEQ;
"]"                 return ENDSEQ;
"("                 return STARTRANGE;
")"                 return ENDRANGE;
"|"					return BAR;

"."					return DOT;
".."				return DDOT;
"..."				return TDOT;
"#"					return SHARPT;
"&"					return FLATT;
"*"					return MULT;
"/"					return DIV;
"="					return EQUAL;
";"					return ENDVAR;			/* end of variable declaration */


"ms"				return MLS;
"s"					return SEC;
"m"|"cm"|"mm"|"in"|"pt"|"pc"|"hs"|"rl"		yyextra->fText = yytext; return UNIT;


"<"					BEGIN PARAM; return STARTPARAM;
<PARAM>{ID}			yyextra->fText = yytext; return IDT;
<PARAM>">"			BEGIN INITIAL; return ENDPARAM;

"\\"{ID}			yyextra->fText = yytext; return TAGNAME;

"$"{ID}				yyextra->fText = yytext; return VARNAME;


"do"|"re"|"mi"|"fa"|"sol"|"la"|"si"|"ti"	yyextra->fText = yytext; return SOLFEGE;
"cis"|"dis"|"fis"|"gis"|"ais"				yyextra->fText = yytext; return CHROMATIC;
{NOTE}				yyextra->fText = yytext; return DIATONIC;
"empty"				yyextra->fText = yytext; return EMPTYT;
"s"[1-6]			yyextra->fText = yytext; return TAB;
"_"					return RESTT;

\"(\\\\|\\.|[^\\\"])*\"   unescape(yytext); unquote(yytext); yyextra->fText = yytext; return STRING;
\'(\\\\|\\.|[^\\\'])*\'   unescape(yytext); unquote(yytext); yyextra->fText = yytext; return STRING;
":"(\\[: ]|[^:\\\n])*":"    unescape(yytext); unquote(yytext); yyextra->fText = yytext; return FRETTE;

{SPACE}+			/* eat up space */

{EOL}				yylloc->first_column=1; /* ignore */

.					fprintf(stderr, "extra text is : %s\n", yytext); return EXTRA;

%%

void GuidoParser::initScanner()
{
	yylex_init(&fScanner);
	yyset_extra(this, fScanner);
}

void GuidoParser::destroyScanner()
{
	yylex_destroy(fScanner);
}
