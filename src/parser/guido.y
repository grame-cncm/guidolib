%{

#include <iostream>
#include <sstream>
#include <string>

#include "GuidoParser.h"
#include "guido.h"

#include "guidoparse.hpp"

//int guidoerror(const char*s);
//int	guidowrap()		{ return(1); }

#define YYERROR_VERBOSE
int guidoerror (YYLTYPE* locp, guido::GuidoParser* context, const char*s);
int yylex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);

#define scanner context->fScanner
#define factory context->fFactory

using namespace std;

%}

%pure-parser
%locations
%defines
%error-verbose
%parse-param { guido::GuidoParser* context }
%lex-param { void* scanner  }

%start score

%union {         
	long int		num;
	float			real;
	const char*		token;
	std::string*	str;
	char			c;
}

/*------------------------------ numbers ------------------------------*/
%token NUMBER
%token PNUMBER
%token NNUMBER
%token FLOAT

/*------------------------------ markers ------------------------------*/
%token STARTCHORD
%token ENDCHORD
%token STARTSEQ
%token ENDSEQ
%token STARTPARAM
%token ENDPARAM
%token STARTRANGE
%token ENDRANGE
%token SEP
%token IDSEP

/*------------------------------   tags  ------------------------------*/
%token BAR
%token TAGNAME
%token IDT

/*------------------------------   notes ------------------------------*/
%token DIATONIC
%token CHROMATIC
%token SOLFEGE
%token EMPTYT
%token RESTT
%token DOT
%token DDOT
%token TDOT
%token SHARPT
%token FLATT

/*------------------------------   misc  ------------------------------*/
%token MLS
%token SEC
%token UNIT
%token MULT
%token DIV
%token EQUAL
%token STRING
%token EXTRA

/*------------------------------   types  ------------------------------*/
%type <num> 	NUMBER PNUMBER NNUMBER
%type <real> 	FLOAT floatn
%type <token>	TAGNAME IDT MLS SEC STRING UNIT DIATONIC CHROMATIC SOLFEGE RESTT 
%type <token>	EMPTYT DOT DDOT EXTRA
%type <c>		STARTCHORD ENDCHORD STARTSEQ ENDSEQ STARTPARAM ENDPARAM STARTRANGE ENDRANGE SEP IDSEP BAR 
%type <c>		SHARPT FLATT MULT DIV EQUAL

%type <str>		notename noteid tagname id 
%type <num>		number pnumber nnumber signednumber


%%

//_______________________________________________
score		: STARTCHORD { gd_segmInit (factory); } ENDCHORD			{ gd_segmExit (factory); }
			| STARTCHORD { gd_segmInit (factory); } voicelist ENDCHORD	{ gd_segmExit (factory); }
			| { gd_segmInit (factory); } voice							{ gd_segmExit (factory); }
			;

voicelist	: voice											{ gd_segmAppendSeq (factory); }
			| voicelist SEP voice							{ gd_segmAppendSeq (factory); }
			;

voice		: STARTSEQ { gd_seqInit (factory); } symbols ENDSEQ		{ gd_seqExit( factory); }
			;

symbols		:
			| symbols music									{ gd_seqAppendNote (factory); }
			| symbols tag
			| symbols chord									{ gd_seqAppendChord (factory); }
			;

//_______________________________________________
// tags description
tag			: positiontag									{ gd_tagEnd (factory); }
			| rangetag										{ gd_tagEnd (factory); }
			;

positiontag	: tagid											{ gd_tagAdd (factory); }
			| tagid STARTPARAM tagparams ENDPARAM			{ gd_tagAdd (factory); }
			;

rangetag	: positiontag STARTRANGE { gd_tagRange (factory); } symbols ENDRANGE
			;

tagname		: TAGNAME										{ $$ = new string(context->fText); }
			;

tagid		: tagname										{ gd_tagStart ( factory, $1->c_str(), 0); delete $1; }
			| tagname IDSEP number							{ gd_tagStart ( factory, $1->c_str(),$3); delete $1; }
			| BAR											{ gd_tagStart ( factory, "\\bar", 0); }
			;

tagarg		: signednumber									{ gd_tagIntArg   (factory, $1) }
			| floatn										{ gd_tagFloatArg (factory, $1) }
			| signednumber UNIT								{ gd_tagIntArg   (factory, $1); gd_tagArgUnit (factory, context->fText.c_str() ) }
			| floatn UNIT									{ gd_tagFloatArg (factory,$1);  gd_tagArgUnit (factory, context->fText.c_str() ) }
			| STRING										{ gd_tagStrArg   (factory, context->fText.c_str() ) }
			| id											{ /* unused */ delete $1; }
			;

tagparam	: tagarg										{ gd_tagAddArg ( factory, ""); }
			| id EQUAL tagarg								{ gd_tagAddArg ( factory, $1->c_str()); delete $1; }
			;

tagparams	: tagparam
			| tagparams SEP tagparam
			;

//_______________________________________________
// chord description

chord		: STARTCHORD { gd_chordInit ( factory ); } chordsymbols ENDCHORD
			;

chordsymbols: { gd_chordInitNote ( factory ); } tagchordsymbol
			| chordsymbols SEP { gd_chordInitNote (factory);} tagchordsymbol
			;

tagchordsymbol: chordsymbol
			| taglist chordsymbol
			| chordsymbol taglist
			| taglist chordsymbol taglist
			;

chordsymbol	: music									{ gd_chordAppendNote (factory);  }
			| rangechordtag	
			;

rangechordtag : positiontag  STARTRANGE { gd_tagRange (factory); } tagchordsymbol ENDRANGE { gd_tagEnd (factory); }
			;

taglist		: positiontag							{ gd_tagEnd (factory); }
			| taglist positiontag					{ gd_tagEnd (factory); }
			;

//_______________________________________________
// notes description

music		: note
			| rest
			;

rest		: RESTT { gd_noteInit ( factory, "_" ); } duration dots
			| RESTT { gd_noteInit ( factory, "_" ); } STARTPARAM NUMBER ENDPARAM duration dots
			;

note		: noteid octave duration dots
			| noteid accidentals octave duration dots
			;

noteid		: notename
			| notename STARTPARAM NUMBER ENDPARAM
			;
			
notename	: DIATONIC								{ gd_noteInit ( factory, context->fText.c_str() ); }
			| CHROMATIC								{ gd_noteInit ( factory, context->fText.c_str() ); }
			| SOLFEGE								{ gd_noteInit ( factory, context->fText.c_str() ); }
			| EMPTYT								{ gd_noteInit ( factory, context->fText.c_str() ); }
			;		

accidentals	: accidental
			| accidentals accidental
			;

accidental	: SHARPT								{  gd_noteAcc (factory, SHARP); }
			| FLATT									{  gd_noteAcc (factory, FLAT); }
			;

octave		:
			| signednumber							{ gd_noteOct (factory, $1); }
			;

duration	:
			| MULT number DIV number				{  gd_noteEnum (factory, $2); gd_noteDenom (factory, $4); }
			| MULT number							{  gd_noteEnum (factory, $2);  }
			| MULT number MLS						{  gd_noteAbsDur(factory, $2);  }
			| DIV number							{  gd_noteDenom (factory, $2); }
			;

dots		:
			| DOT									{  gd_noteDot  (factory);  }
			| DDOT									{  gd_noteDdot (factory); }
			| TDOT									{  gd_noteTdot (factory); }
			;

//_______________________________________________
// misc
id			: IDT									{ $$ = new string(context->fText); }
			;
number		: NUMBER								{ $$ = atol(context->fText.c_str() ); }
			;
pnumber		: PNUMBER								{ $$ = atol(context->fText.c_str() ); }
			;
nnumber		: NNUMBER								{ $$ = atol(context->fText.c_str() ); }
			;
floatn		: FLOAT									{ $$ = atof(context->fText.c_str() ); }

			;
signednumber: number								{ $$ = $1; }
			| pnumber								{ $$ = $1; } 
			| nnumber								{ $$ = $1; }
			;
%%

// int guidoerror(const char*s) {
// 	extern long int lnr;
// 	YY_FLUSH_BUFFER;
// 	lnr = guidolineno;
// 	return ERROR(s);
// }

int guidoerror(YYLTYPE* loc, guido::GuidoParser* context, const char*s) {
//	cerr << "error line: " << loc->last_line + context->fLine << " col: " << loc->first_column << ": " << s << endl;
	return 0;
}


namespace guido 
{

ARHandler GuidoParser::parse()
{
	yyparse (this);
	return GuidoFactoryCloseMusic (fFactory);
}
}

