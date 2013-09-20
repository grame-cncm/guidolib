%{

#include <iostream>
#include <sstream>
#include <string>

#include "GuidoParser.h"
#include "guido.h"

#include "guidoparse.hpp"

#define YYERROR_VERBOSE
int guidoerror (YYLTYPE* locp, guido::GuidoParser* context, const char*s);
int yylex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);

#define scanner context->fScanner

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
score		: STARTCHORD { context->segmInit (); } ENDCHORD				{ context->segmExit (); }
			| STARTCHORD { context->segmInit (); } voicelist ENDCHORD	{ context->segmExit (); }
			| { context->segmInit (); } voice							{ context->segmExit (); }
			;

voicelist	: voice													{ context->segmAppendSeq (); }
			| voicelist SEP voice									{ context->segmAppendSeq (); }
			;

voice		: STARTSEQ { context->seqInit (); } symbols ENDSEQ		{ context->seqExit( ); }
			;

symbols		:
			| symbols music									{ context->appendNote (); }
			| symbols tag
			| symbols chord									{ context->seqAppendChord (); }
			;

//_______________________________________________
// tags description
tag			: positiontag									{ context->tagEnd (); }
			| rangetag										{ context->tagEnd (); }
			;

positiontag	: tagid											{ context->tagAdd (); }
			| tagid STARTPARAM tagparams ENDPARAM			{ context->tagAdd (); }
			;

rangetag	: positiontag STARTRANGE { context->tagRange (); } symbols ENDRANGE
			;

tagname		: TAGNAME										{ $$ = new string(context->fText); }
			;

tagid		: tagname										{ context->tagStart ( $1->c_str(), 0); delete $1; }
			| tagname IDSEP number							{ context->tagStart ( $1->c_str(),$3); delete $1; }
			| BAR											{ context->tagStart ( "\\bar", 0); }
			;

tagarg		: signednumber									{ context->tagIntArg   ($1) }
			| floatn										{ context->tagFloatArg ($1) }
			| signednumber UNIT								{ context->tagIntArg   ($1); context->tagArgUnit (context->fText.c_str() ) }
			| floatn UNIT									{ context->tagFloatArg ($1); context->tagArgUnit (context->fText.c_str() ) }
			| STRING										{ context->tagStrArg   (context->fText.c_str() ) }
			| id											{ /* unused */ delete $1; }
			;

tagparam	: tagarg										{ context->tagAddArg ( ""); }
			| id EQUAL tagarg								{ context->tagAddArg ( $1->c_str()); delete $1; }
			;

tagparams	: tagparam
			| tagparams SEP tagparam
			;

//_______________________________________________
// chord description

chord		: STARTCHORD { context->chordInit (); } chordsymbols ENDCHORD
			;

chordsymbols: { context->chordInitNote (); } tagchordsymbol
			| chordsymbols SEP { context->chordInitNote ();} tagchordsymbol
			;

tagchordsymbol: chordsymbol
			| taglist chordsymbol
			| chordsymbol taglist
			| taglist chordsymbol taglist
			;

chordsymbol	: music									{ context->appendNote ();  }
			| rangechordtag	
			;

rangechordtag : positiontag  STARTRANGE { context->tagRange (); } tagchordsymbol ENDRANGE { context->tagEnd (); }
			;

taglist		: positiontag							{ context->tagEnd (); }
			| taglist positiontag					{ context->tagEnd (); }
			;

//_______________________________________________
// notes description

music		: note
			| rest
			;

rest		: RESTT { context->noteInit ( "_" ); } duration dots
			| RESTT { context->noteInit ( "_" ); } STARTPARAM NUMBER ENDPARAM duration dots
			;

note		: noteid octave duration dots
			| noteid accidentals octave duration dots
			;

noteid		: notename
			| notename STARTPARAM NUMBER ENDPARAM
			;
			
notename	: DIATONIC								{ context->noteInit ( context->fText.c_str() ); }
			| CHROMATIC								{ context->noteInit ( context->fText.c_str() ); }
			| SOLFEGE								{ context->noteInit ( context->fText.c_str() ); }
			| EMPTYT								{ context->noteInit ( context->fText.c_str() ); }
			;		

accidentals	: accidental
			| accidentals accidental
			;

accidental	: SHARPT								{  context->noteAcc (SHARP); }
			| FLATT									{  context->noteAcc (FLAT); }
			;

octave		:
			| signednumber							{ context->noteOct ($1); }
			;

duration	:
			| MULT number DIV number				{  context->noteEnum ($2); context->noteDenom ($4); }
			| MULT number							{  context->noteEnum ($2);  }
			| MULT number MLS						{  context->noteAbsDur($2);  }
			| DIV number							{  context->noteDenom ($2); }
			;

dots		:
			| DOT									{  context->noteDot  ();  }
			| DDOT									{  context->noteDdot (); }
			| TDOT									{  context->noteTdot (); }
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

extern int	gParseErrorLine;
int guidoerror(YYLTYPE* loc, guido::GuidoParser* p, const char*s) {
	gParseErrorLine = loc->last_line;		// for backward compatibility only
	p->setErrorLoc (loc->last_line, loc->first_column);
	cerr << "error line: " << loc->last_line << " col: " << loc->first_column << ": " << s << endl;
	return 0;
}


namespace guido 
{

ARHandler GuidoParser::parse()
{
	fzaehlerSet = 0;
	faccidentals = 0;
	fndots = 0;
	fnt_enumSet = false;
	fnt_enum =0;
	fnt_denom =1;
	fErrorLine = fErrorColumn = 0;
	yyparse (this);
	return (fErrorLine == 0) ? GuidoFactoryCloseMusic (fFactory) : 0;
}
}

