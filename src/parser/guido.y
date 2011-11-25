%{

#include <string>
#include <iostream>

#include "guido.h"

#include "guidoparse.hxx"
#include "guidolex.cxx"

int guidoerror(const char*s);
int	guidowrap()		{ return(1); }


//#define parseDebug

#ifdef parseDebug
#define debug(msg)		cout << msg << endl;
#define vdebug(msg,v)	cout << msg << " " << v << endl;
#else
#define debug(msg)
#define vdebug(msg, v)
#endif

#define YYERROR_VERBOSE


using namespace std;

%}


//%pure_parser

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
score		: STARTCHORD { GD_INIT_SEGM } ENDCHORD				{ GD_EXIT_SEGM }
			| STARTCHORD { GD_INIT_SEGM } voicelist ENDCHORD	{ GD_EXIT_SEGM }
			| { GD_INIT_SEGM } voice							{ GD_EXIT_SEGM }
			;

voicelist	: voice											{ GD_APP_SEQ }
			| voicelist SEP voice							{ GD_APP_SEQ }
			;

voice		: STARTSEQ { GD_INIT_SEQ } symbols ENDSEQ		{ GD_EXIT_SEQ }
			;

symbols		:
			| symbols music									{  GD_APP_NT  }
			| symbols tag
			| symbols chord									{  GD_APP_CH  }
			;

//_______________________________________________
// tags description
tag			: positiontag									{ GD_TAG_END }
			| rangetag										{ GD_TAG_END }
			;

positiontag	: tagid											{ GD_TAG_ADD }
			| tagid STARTPARAM tagparams ENDPARAM			{ GD_TAG_ADD }
			;

rangetag	: positiontag STARTRANGE { GD_TAG_RANGE } symbols ENDRANGE
			;

tagname		: TAGNAME										{ $$ = new string(guidotext); }
			;

tagid		: tagname										{ GD_TAG_START($1->c_str(), 0); delete $1; }
			| tagname IDSEP number							{ GD_TAG_START($1->c_str(),$3); delete $1; }
			| BAR											{ GD_TAG_START("\\bar", 0); }
			;

tagarg		: signednumber									{ GD_TAG_NARG($1) }
			| floatn										{ GD_TAG_RARG($1) }
			| signednumber UNIT								{ GD_TAG_NARG($1); GD_TAG_ARG_UNIT(guidotext) }
			| floatn UNIT									{ GD_TAG_RARG($1); GD_TAG_ARG_UNIT(guidotext) }
			| STRING										{ GD_TAG_SARG(guidotext) }
			| id											{ GD_TAG_TARG($1->c_str()); delete $1; }
			;

tagparam	: tagarg										{ GD_TAG_ADD_ARG(""); }
			| id EQUAL tagarg								{ GD_TAG_ADD_ARG($1->c_str()); delete $1; }
			;

tagparams	: tagparam
			| tagparams SEP tagparam
			;

//_______________________________________________
// chord description

chord		: STARTCHORD { GD_INIT_CH } chordsymbols ENDCHORD
			;

chordsymbols: { GD_CH_INIT_NT } tagchordsymbol
			| chordsymbols SEP { GD_CH_INIT_NT } tagchordsymbol
			;

tagchordsymbol: chordsymbol
			| taglist chordsymbol
			| chordsymbol taglist
			| taglist chordsymbol taglist
			;

chordsymbol	: music											{ GD_CH_APP_NT }
			| rangechordtag	
			;

rangechordtag : positiontag  STARTRANGE { GD_TAG_RANGE } tagchordsymbol ENDRANGE { GD_TAG_END }
			;

taglist		: positiontag							{ GD_TAG_END }
			| taglist positiontag					{ GD_TAG_END }
			;

//_______________________________________________
// notes description

music		: note
			| rest
			;

rest		: RESTT { GD_NT("_") } duration dots
			| RESTT { GD_NT("_") } STARTPARAM NUMBER ENDPARAM duration dots
			;

note		: noteid octave duration dots
			| noteid accidentals octave duration dots
			;

noteid		: notename
			| notename STARTPARAM NUMBER ENDPARAM
			;
			
notename	: DIATONIC								{ GD_NT(guidotext) }
			| CHROMATIC								{ GD_NT(guidotext) }
			| SOLFEGE								{ GD_NT(guidotext) }
			| EMPTYT								{ GD_NT(guidotext) }
			;		

accidentals	: accidental
			| accidentals accidental
			;

accidental	: SHARPT								{  GD_SH_NT  }
			| FLATT									{  GD_FL_NT  }
			;

octave		:
			| signednumber							{ GD_OCT_NT($1) }
			;

duration	:
			| MULT number DIV number				{  GD_ENUM_NT($2); GD_DENOM_NT($4) }
			| MULT number							{  GD_ENUM_NT($2)  }
			| MULT number MLS						{  GD_ABSDUR_NT($2)  }
			| DIV number							{  GD_DENOM_NT($2) }
			;

dots		:
			| DOT									{  GD_DOT_NT  }
			| DDOT									{  GD_DDOT_NT }
			| TDOT									{  GD_TDOT_NT }
			;

//_______________________________________________
// misc
id			: IDT									{ $$ = new string(guidotext); }
			;
number		: NUMBER								{ $$ = atol(guidotext); }
			;
pnumber		: PNUMBER								{ $$ = atol(guidotext); }
			;
nnumber		: NNUMBER								{ $$ = atol(guidotext); }
			;
floatn		: FLOAT									{ $$ = atof(guidotext); }

			;
signednumber: number								{ $$ = $1; }
			| pnumber								{ $$ = $1; } 
			| nnumber								{ $$ = $1; }
			;
%%

int guidoerror(const char*s) {
	extern long int lnr;
	YY_FLUSH_BUFFER;
	lnr = guidolineno;
	return ERROR(s);
}
