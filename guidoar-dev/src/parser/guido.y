%{

#include <string>
#include <iostream>
#include <vector>
#include <assert.h>

#include "guidoelement.h"
#include "gmnreader.h"
#include "guidorational.h"

#include "guidoparse.h++"
#include "guidolex.c++"

int guidoarerror(const char*s);
int	guidoarwrap()		{ return(1); }

extern guido::gmnreader* gReader;

static void vadd (std::vector<guido::Sguidoelement>* v1, std::vector<guido::Sguidoelement>* v2)
{
	for (std::vector<guido::Sguidoelement>::iterator i = v2->begin(); i != v2->end(); i++)
		v1->push_back(*i);
}

//#define parseDebug

#ifdef parseDebug
#define debug(msg)		cout << msg << endl;
#define vdebug(msg,v)	cout << msg << " " << v << endl;
#else
#define debug(msg)
#define vdebug(msg, v)
#endif

#define addElt(to,elt)	(to)->push(*elt); delete elt

using namespace std;

namespace guido
{

%}


//%pure_parser

%union {         
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
%token ID

/*------------------------------   notes ------------------------------*/
%token DIATONIC
%token CHROMATIC
%token SOLFEGE
%token EMPTY
%token REST
%token DOT
%token DDOT
%token SHARP
%token FLAT

/*------------------------------   misc  ------------------------------*/
%token MLS
%token SEC
%token UNIT
%token MULT
%token DIV
%token EQUAL
%token STRING
%token EXTRA

%type <num> 	NUMBER PNUMBER NNUMBER
%type <real> 	FLOAT floatn
%type <token>	TAGNAME ID MLS SEC STRING UNIT DIATONIC CHROMATIC SOLFEGE REST 
%type <token>	EMPTY DOT DDOT EXTRA
%type <c>		STARTCHORD ENDCHORD STARTSEQ ENDSEQ STARTPARAM ENDPARAM STARTRANGE ENDRANGE SEP IDSEP BAR 
%type <c>		SHARP FLAT MULT DIV EQUAL

%type <str>		notename noteid tagname id 
%type <num>		dots accidentals accidental octave number pnumber nnumber signednumber
%type <r>		duration
%type <elt>		score voice note rest music chord  tag tagid positiontag rangetag rangechordtag 
%type <attr>	tagarg tagparam
%type <velt>	symbols voicelist chordsymbol chordsymbols taglist tagchordsymbol
%type <vattr>	tagparams

%%

start		: score				{delete $1;}
		;

//_______________________________________________
score		: STARTCHORD ENDCHORD							{ debug("new score"); $$ = gReader->newScore(); }
			| STARTCHORD voicelist ENDCHORD					{ debug("score voicelist"); $$ = gReader->newScore(); addElt(*$$, $2); }
			| voice											{ debug("score voice"); $$ = gReader->newScore(); addElt(*$$, $1); }
			;

voicelist	: voice											{ debug("new voicelist"); $$ = new vector<Sguidoelement>; $$->push_back (*$1); delete $1; }
			| voicelist SEP voice							{ debug("add voicelist"); $$ = $1; $$->push_back (*$3); delete $3; }
			;

voice		: STARTSEQ symbols ENDSEQ						{ debug("new voice"); $$ = gReader->newVoice(); addElt(*$$, $2); }
			;

symbols		:												{ debug("new symbols"); $$ = new vector<Sguidoelement>; }
			| symbols music									{ debug("add music"); $$ = $1; $$->push_back(*$2); delete $2; }
			| symbols tag									{ debug("add tag"); $$ = $1; $$->push_back(*$2); delete $2; }
			| symbols chord									{ debug("add chord"); $$ = $1; $$->push_back(*$2); delete $2; }
			;

//_______________________________________________
// tags description
tag			: positiontag									{ debug("position tag "); $$ = $1; }
			| rangetag										{ debug("range tag "); $$ = $1; }
			;

positiontag	: tagid											{ debug("new position tag "); $$ = $1; }
			| tagid STARTPARAM tagparams ENDPARAM			{ debug("new tag + params"); $$ = $1; (*$1)->add (*$3); delete $3; }
			;

rangetag	: positiontag  STARTRANGE symbols ENDRANGE		{ debug("new range tag "); $$ = $1; (*$1)->push (*$3); delete $3; }
			;

tagname		: TAGNAME										{ debug("tag name "); $$ = new string(guidoartext); }
			;

tagid		: tagname										{ vdebug("new tag", *$1); $$ = gReader->newTag(*$1, 0); delete $1; if (!$$) { guidoarerror("unknown tag"); YYERROR;} }
			| tagname IDSEP NUMBER							{ debug("new tag::id"); $$ = gReader->newTag(*$1, $2); delete $1; if (!$$) { guidoarerror("unknown tag"); YYERROR;} }
			| BAR											{ debug("new bar"); $$ = gReader->newTag("bar", 0); }
			;

tagarg		: signednumber									{ debug("new signednumber arg"); $$ = gReader->newAttribute($1); }
			| floatn										{ debug("new FLOAT arg"); $$ = gReader->newAttribute($1); }
			| signednumber UNIT								{ debug("new signednumber UNIT arg"); $$ = gReader->newAttribute($1); (*$$)->setUnit(guidoartext); }
			| floatn UNIT									{ debug("new FLOAT UNIT arg"); $$ = gReader->newAttribute($1); (*$$)->setUnit(guidoartext); }
			| STRING										{ debug("new STRING arg"); $$ = gReader->newAttribute(guidoartext, true); }
			| id											{ debug("new ID arg"); $$ = gReader->newAttribute(*$1, false); delete $1; }
			;

tagparam	: tagarg										{ $$ = $1; }
			| id EQUAL tagarg								{ $$ = $3; (*$3)->setName(*$1); delete $1; }
			;

tagparams	: tagparam										{ $$ = new vector<Sguidoattribute>; $$->push_back(*$1); delete $1; }
			| tagparams SEP tagparam						{ $$ = $1; $$->push_back(*$3); delete $3; }
			;


//_______________________________________________
// chord description

chord		: STARTCHORD chordsymbols ENDCHORD				{ debug("new chord"); $$ = gReader->newChord(); (*$$)->push(*$2); delete $2; }
			;

chordsymbols: tagchordsymbol								{ $$ = new vector<Sguidoelement>; vadd($$, $1); delete $1; }
			| chordsymbols SEP tagchordsymbol				{ $$ = $1; vadd($$, $3); delete $3; }
			;

tagchordsymbol: chordsymbol									{ $$ = $1;}
			| taglist chordsymbol							{ $$ = $1; vadd($$, $2); delete $2; }
			| chordsymbol taglist							{ $$ = $1; vadd($$, $2); delete $2; }
			| taglist chordsymbol taglist					{ $$ = $1; vadd($$, $2); delete $2; vadd($$, $3); delete $3; }
			;

chordsymbol	: music											{ $$ = new vector<Sguidoelement>; $$->push_back(*$1); delete $1; }
			| rangechordtag									{ $$ = new vector<Sguidoelement>; $$->push_back(*$1); delete $1; }
			;

rangechordtag : positiontag  STARTRANGE tagchordsymbol ENDRANGE	{ debug("range chord tag"); $$ = $1; (*$$)->push(*$3); delete $3; }
			;

taglist		: positiontag									{ debug("new taglist 1"); $$ = new vector<Sguidoelement>; $$->push_back(*$1); delete $1; }
			| taglist positiontag							{ debug("new taglist 2"); $$ = $1; $$->push_back(*$2); delete $2; }
			;

//_______________________________________________
// notes description

music		: note											{ $$ = $1; }
			| rest											{ $$ = $1; }
			;

rest		: REST duration	dots							{ debug("new rest 1"); $$ = gReader->newRest($2, $3); delete $2; }
			| REST STARTPARAM NUMBER ENDPARAM duration dots	{ debug("new rest 2"); $$ = gReader->newRest($5, $6); delete $5; }
			;

note		: noteid octave duration dots				{ debug("new note v1"); $$ = gReader->newNote(*$1, 0, $2, $3, $4); delete $1; delete $3; }
			| noteid accidentals octave duration dots	{ debug("new note v2"); $$ = gReader->newNote(*$1, $2, $3, $4, $5); delete $1; delete $4; }
			;

noteid		: notename									{ vdebug("notename", *$1); $$ = $1; }
			| notename STARTPARAM NUMBER ENDPARAM		{ $$ = $1; }
			;
			
notename	: DIATONIC								{ debug("new diatonic note"); $$ = new string(guidoartext); }
			| CHROMATIC								{ debug("new chromatic note"); $$ = new string(guidoartext); }
			| SOLFEGE								{ debug("new solfege note"); $$ = new string(guidoartext); }
			| EMPTY									{ debug("new empty note"); $$ = new string(guidoartext); }
			;		

accidentals	: accidental							{ debug("accidental"); $$ = $1; }
			| accidentals accidental				{ debug("accidentals"); $$ = $1 + $2; }
			;

accidental	: SHARP									{ debug("sharp"); $$ = 1; }
			| FLAT									{ debug("flat"); $$ = -1; }
			;

octave		:										{ debug("no octave"); $$ = -1000; }					// implicit duration
			| signednumber							{ debug("octave"); $$ = $1; }
			;

duration	:										{ debug("implicit duration"); $$ = new rational(-1, 1); }	// implicit duration
			| MULT number DIV number				{ debug("duration ./."); $$ = new rational($2, $4); }
			| MULT number							{ debug("duration *"); $$ = new rational($2, 1); }
			| DIV number							{ debug("duration /"); $$ = new rational(1, $2); }
			;

dots		:										{ debug("dots 0"); $$ = 0; }
			| DOT									{ debug("dots 1"); $$ = 1; }
			| DDOT									{ debug("dots 2"); $$ = 2; }
			;

//_______________________________________________
// misc

id			: ID									{ $$ = new string(guidoartext); }
			;
number		: NUMBER								{ vdebug("NUMBER", guidoartext); $$ = atol(guidoartext); }
			;
pnumber		: PNUMBER								{ vdebug("NUMBER", guidoartext); $$ = atol(guidoartext); }
			;
nnumber		: NNUMBER								{ vdebug("NUMBER", guidoartext); $$ = atol(guidoartext); }
			;
floatn		: FLOAT									{ $$ = atof(guidoartext); }
			;
signednumber: number								{ $$ = $1; }
			| pnumber								{ $$ = $1; } 
			| nnumber								{ $$ = $1; }
			;
%%

} // namespace

int guidoarerror(const char*s) {
	YY_FLUSH_BUFFER;
	return gReader->error(s, guidoarlineno);
}
