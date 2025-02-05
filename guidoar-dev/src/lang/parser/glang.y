%{

#include <iostream>
#include <string>
#include "glangreader.h"

#include "glangparse.hpp"
#include "glanglex.cpp"

int glangerror(const char*s);

//#define parseDebug
#ifdef parseDebug
#define debug(msg)		std::cout << msg << " -" << glangtext << "-" << std::endl;
#else
#define debug(msg)
#endif

#define clean(a,b,c)	delete a; delete b; delete c

extern guidolang::glangreader* gGLReader;
using namespace guidolang;

%}

//%pure_parser		/* reentrancy support (but generates incorrect code) */

%start program

%union {         
	SGLExpr*  exprPtr;
	std::string * strPtr;
}

%type <exprPtr> 	expr group 
%type <strPtr>		name op asep

/*------------------------- tokens  and precedence -----------------------*/
%token GMN 
%token GROUPSTART 
%token GROUPEND 
%token ASEP 
%token EQ IDENT 

%left  BOTTOM
%left TOP
%left TAIL
%left HEAD
%left PAR
%left SEQ
%left APPLY
%right ABSTRACT

/*
	there are actually 7 shift/reduce
*/
%expect 7

%%

//__________________________________________________________________________
program		: deflist						{ }

deflist		: name EQ expr					{ debug("ident expr");		gGLReader->newIDExpr($1->c_str(), $3); delete $1; delete $3; }
			| deflist name EQ expr			{ debug("deflist...");		gGLReader->newIDExpr($2->c_str(), $4); delete $2; delete $4; }


expr		: GMN							{ debug("score expr");		$$ = gGLReader->newScoreExpr(glangtext); 
																			 if (!$$) { glangerror("Error while parsing gmn code"); YYERROR; } }
			| expr op expr					{ debug("op expr");			$$ = gGLReader->newBinaryExpr($2->c_str(),$1,$3); clean($1,$2,$3); }
			| ABSTRACT expr asep expr		{ debug("abstract expr");	$$ = gGLReader->newAbstractExpr($3->c_str(),$2,$4); clean($2,$3,$4); }
			| name							{ debug("named expr");		$$ = gGLReader->newNamedExpr($1->c_str()); delete $1;
																			 if (!$$) { glangerror("Undefined named expression used"); YYERROR; } }
			| group							{ debug("group expr");		$$ = gGLReader->newGroupExpr($1); delete $1; }
			;

group		: GROUPSTART expr GROUPEND		{ debug("group expr"); $$ = $2; }

op			: SEQ							{ debug("seq");		$$ = new std::string (glangtext); }
			| PAR							{ debug("par");		$$ = new std::string (glangtext); }
			| HEAD							{ debug("head");	$$ = new std::string (glangtext); }
			| TAIL							{ debug("tail");	$$ = new std::string (glangtext); }
			| TOP							{ debug("top");		$$ = new std::string (glangtext); }
			| BOTTOM						{ debug("bottom");	$$ = new std::string (glangtext); }
			| APPLY							{ debug("apply");	$$ = new std::string (glangtext); }
			
asep		: ASEP							{ debug("asep");	$$ = new std::string (glangtext); }
name		: IDENT							{ debug("name");	$$ = new std::string (glangtext); }

%%

int	glangwrap()		{ return(1); }

int glangerror(const char* s) {
	YY_FLUSH_BUFFER;
	return gGLReader->error(s, glanglineno);
	return 0;
}

