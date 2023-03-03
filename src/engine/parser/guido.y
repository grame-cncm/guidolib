%{

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

%}

%define api.pure
%locations
%defines
%define parse.error verbose
%parse-param { GuidoParser* context }
%lex-param { void* scanner  }

%start gmn

%union {         
	long int		num;
	float			real;
	const char*		token;
	std::string*	str;
	char			c;
	TagParameter*	param;
	ARMusicalTag*	tag;
//	GuidoParser::ParamsList*	plist;
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
%token TAB

/*------------------------------   misc  ------------------------------*/
%token MLS
%token SEC
%token UNIT
%token MULT
%token DIV
%token EQUAL
%token STRING
%token EXTRA
%token ENDVAR
%token VARNAME
%token FRETTE


/*------------------------------   types  ------------------------------*/
%type <num> 	NUMBER PNUMBER NNUMBER
%type <real> 	FLOAT floatn
%type <token>	TAGNAME IDT MLS SEC STRING UNIT DIATONIC CHROMATIC SOLFEGE RESTT 
%type <token>	EMPTYT DOT DDOT EXTRA
%type <c>		STARTCHORD ENDCHORD STARTSEQ ENDSEQ STARTPARAM ENDPARAM STARTRANGE ENDRANGE SEP IDSEP BAR 
%type <c>		SHARPT FLATT MULT DIV EQUAL

%type <str>		notename noteid tagname id varname 
%type <num>		number pnumber nnumber signednumber string
%type<param>	tagarg tagparam tab
// %type<plist>	tagparams

%%

//_______________________________________________
gmn			: score
			| variables score
			;

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
			| symbols varname								{ bool ret = context->variableSymbols ($2->c_str()); 
															  if (!ret) varerror (@2.last_line, @2.first_column, context, $2->c_str());
															  delete $2;
															  if (!ret)  YYABORT;
															}
			;

//_______________________________________________
// variables - introduced on sept. 15 2020 by DF
variables 	: vardecl
			| variables vardecl
			;

vardecl 	: varname EQUAL STRING ENDVAR					{ context->variableDecl ($1->c_str(), context->fText.c_str(), GuidoParser::kString); delete $1; }
			| varname EQUAL signednumber ENDVAR				{ context->variableDecl ($1->c_str(), context->fText.c_str(), GuidoParser::kInt); delete $1; }
			| varname EQUAL floatn ENDVAR					{ context->variableDecl ($1->c_str(), context->fText.c_str(), GuidoParser::kFloat); delete $1; }
			;

varname		: VARNAME										{ $$ = new string(context->fText); }
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

tagarg		: signednumber									{ $$ = context->intParam   ($1); }
			| floatn										{ $$ = context->floatParam ($1); }
			| signednumber UNIT								{ $$ = context->intParam   ($1, context->fText.c_str() ); }
			| floatn UNIT									{ $$ = context->floatParam ($1, context->fText.c_str() ); }
			| STRING										{ $$ = context->strParam   (context->fText.c_str() ); }
			| id											{ /* unused */ $$ = 0; delete $1; }
			| varname										{ $$ = context->varParam ($1->c_str() ); 
															  if (!$$) varerror (@1.last_line, @1.first_column, context, $1->c_str());
															  delete $1;
															  if (!$$) YYABORT; 
															}
			;

tagparam	: tagarg										{ $$ = $1; }
			| id EQUAL tagarg								{ if($3) context->setParamName ($3, $1->c_str()); $$ = $3; delete $1; }
			;

tagparams	: tagparam										{ context->tagParameter ($1); }
			| tagparams SEP tagparam						{ context->tagParameter ($3) ; } 
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
			| tab duration dots
			;

noteid		: notename
			| notename STARTPARAM NUMBER ENDPARAM
			;
			
notename	: DIATONIC								{ context->noteInit ( context->fText.c_str() ); }
			| CHROMATIC								{ context->noteInit ( context->fText.c_str() ); }
			| SOLFEGE								{ context->noteInit ( context->fText.c_str() ); }
			| EMPTYT								{ context->noteInit ( context->fText.c_str() ); }
			;		

tab			: string FRETTE 						{ context->tabInit ( $1, context->fText.c_str() ); }
			;

string		: TAB 									{ $$ = atoi( &context->fText.c_str()[1]); }
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
