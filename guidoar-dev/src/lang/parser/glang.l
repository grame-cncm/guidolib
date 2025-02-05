%{

#include <stdio.h>

#define YY_NO_UNISTD_H

static int nested;
static int chord;

%}

%option yylineno
%option never-interactive

%x CMNTLN
%x COMMENT
%S SCORE

DIGIT	[0-9]
ID		[a-z_A-Z][a-z_A-Z0-9]*
LETTER	[a-zA-Z]
SPACE	[ \t\x0a\x0d]
EOL		[\x0a\x0d]
SCCHAR	[^{}]

%%

"%"					BEGIN CMNTLN;
"//"				BEGIN CMNTLN;
<CMNTLN>.*	
<CMNTLN>{EOL}+		BEGIN INITIAL;

<COMMENT>.|{EOL}*
<COMMENT>"/*"		nested++; 
<COMMENT>"*/"		if (nested==0) { BEGIN INITIAL; } else { nested--; }
"/*"				nested=0; BEGIN COMMENT;

"("					return GROUPSTART;
")"					return GROUPEND;
":"					return SEQ;
"/"					return PAR;
"-|"				return HEAD;
"|-"				return TAIL;
"-/"                return TOP;
"/-"                return BOTTOM;

"@"					return APPLY;
"#"					return ABSTRACT;
"."					return ASEP;
"="					return EQ;

<SCORE>"{"			chord++; yymore(); 
<SCORE>"}"			if (chord==0) { BEGIN INITIAL; return GMN; } else { chord--; yymore(); }
<SCORE>{EOL}*		yymore();
<SCORE>{SCCHAR}*	yymore();
"{"					chord=0; yymore(); BEGIN SCORE;

{ID}				return IDENT;

{SPACE}+			/* eat up space */
.					/* printf("glang extra text is : %s\n", yytext); */

