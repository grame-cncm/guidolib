// 
// json grammar
// based on the definition published at http://json.org
// D. Fober - Grame 2013
//

%{

#include <iostream>
#include <sstream>
#include <string>

#include "json_parser.h"
#include "json_object.h"
#include "json_element.h"
#include "json_array.h"
#include "json_value.h"

%}


%pure-parser
%locations
%defines
%error-verbose
%parse-param { json::json_parser* context }
%lex-param { void* scanner  }

/*------------------------------ tokens ------------------------------*/
%token INT FLOAT ERR
%token QUOTEDSTRING
%token COMMA COLON OBJSTART OBJEND ARRAYSTART ARRAYEND
%token TOKTRUE TOKFALSE TOKNULL;

/*------------------------------   types  ------------------------------*/
%union
{
	int				num;
	float			real;
	std::string*		str;
	json::json_value*	val;
	json::json_object*	obj;
	json::json_element* elt;
	json::json_array*	tbl;
}

%type <num> 	INT
%type <real>	FLOAT
%type <str>		string
%type <val>		value
%type <obj>		object members
%type <elt>		element
%type <tbl>		array values

%{

typedef void * yyscan_t;

int yyerror (YYLTYPE* locp, json::json_parser* context, const char*s);
int yylex(YYSTYPE* lvalp, YYLTYPE* llocp, void* scanner);
//int lineno(json::json_parser* context);

#define scanner context->fScanner

using namespace std;

%}

%%

//_______________________________________________
// JSON format specification
//_______________________________________________
start		: object						{ context->fJSON = $1; }
			;

object		: OBJSTART OBJEND				{ $$ = new json::json_object(); }
			| OBJSTART members OBJEND		{ $$ = $2; }
			;

members		: element						{ $$ = new json::json_object(); $$->add ($1); }
			| members COMMA element			{ $1->add ($3); $$ = $1; }
			;

element		: string COLON value			{ $$ = new json::json_element($1->c_str(), $3); delete $1; }
			;

array		: ARRAYSTART ARRAYEND			{ $$ = new json::json_array(); }
			| ARRAYSTART values ARRAYEND	{ $$ = $2; }
			;

values		: value							{ $$ = new json::json_array(); $$->add ($1); }
			| values COMMA value			{ $1->add ($3); $$ = $1; }
			;

value		: string						{ $$ = new json::json_string_value($1->c_str()); delete $1; }
			| INT							{ $$ = new json::json_int_value(context->fInt); }
			| FLOAT							{ $$ = new json::json_float_value(context->fFloat); }
			| object						{ $$ = new json::json_object_value($1); }
			| array							{ $$ = new json::json_array_value($1); }
			| TOKTRUE						{ $$ = new json::json_true_value(); }
			| TOKFALSE						{ $$ = new json::json_false_value(); }
			| TOKNULL						{ $$ = new json::json_null_value(); }
			;

string		: QUOTEDSTRING					{ $$ = new string(context->fText); }
			;

%%

using namespace json;

json_object* json_parser::parse()
{
	yyparse (this);
	return fJSON;
}


int yyerror(YYLTYPE* loc, json_parser* context, const char*s) {
	cerr << "error line: " << loc->last_line << " col: " << loc->first_column << ": " << s << endl;
	return 0;
}
