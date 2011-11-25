%{

#include <stdlib.h>

#define YY_NO_UNISTD_H
extern YYSTYPE guidolval;

static int nested;

static  char* unescape (char * str) {
	int n = strlen(str);
	for (int i=0, j=0; i <= n; i++) {		
		if (str[i] == '\\') {
			if (str[i+1] == '\\') {
				str[j++] = str[i++];
			}
			else if (str[i+1] != '"') {
				str[j++] = str[i];
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

%option yylineno
%option never-interactive

%x CMNTLN
%x COMMENT
%S PARAM

DIGIT	[0-9]
ID		[$a-z_A-Z][a-z_A-Z0-9]*
LETTER	[a-zA-Z]
NOTE	[abcdefgh]
SPACE	[ \t\x0a\x0d]
EOL		[\x0a\x0d]

%%
{DIGIT}+					return NUMBER;
"+"{DIGIT}+					return PNUMBER;
"-"{DIGIT}+					return NNUMBER;

{DIGIT}*"."{DIGIT}+			return FLOAT;
"+"{DIGIT}*"."{DIGIT}+		return FLOAT;
"-"{DIGIT}*"."{DIGIT}+		return FLOAT;

{SPACE}*"%"			BEGIN CMNTLN;
<CMNTLN>.
<CMNTLN>{EOL}+		BEGIN INITIAL;

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

"ms"				return MLS;
"s"					return SEC;
"m"|"cm"|"mm"|"in"|"pt"|"pc"|"hs"|"rl"		return UNIT;

"<"					BEGIN PARAM; return STARTPARAM;
<PARAM>{ID}			return IDT;
<PARAM>">"			BEGIN INITIAL; return ENDPARAM;

"\\"{ID}			return TAGNAME;

"do"|"re"|"mi"|"fa"|"sol"|"la"|"si"|"ti"	return SOLFEGE;
"cis"|"dis"|"fis"|"gis"|"ais"				return CHROMATIC;
{NOTE}				return DIATONIC;
"empty"				return EMPTYT;
"_"					return RESTT;

\"(\\\\|\\.|[^\\\"])*\"   unescape(yytext); unquote(yytext); return STRING;

{SPACE}+			/* eat up space */
.				printf("extra text is : %s\n", yytext); return EXTRA;

