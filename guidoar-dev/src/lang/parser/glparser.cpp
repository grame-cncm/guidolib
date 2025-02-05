/*
  GUIDO Library
  Copyright (C) 2008  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4786 4244 4018 4065)
# pragma warning (disable : 4996)
# pragma warning (disable : 4102)
#endif

//#define GLDEBUG

#ifdef GLDEBUG
# define YYDEBUG	1
#endif
#include "glangparse.cpp"

#include <iostream>
#include "glparser.h"

#define yyin				glangin
#define yyrestart			glangrestart
#define yy_delete_buffer	glang_delete_buffer
#define yy_scan_string		glang_scan_string

#ifdef GLDEBUG
  extern int yydebug;
# define pdebug	yydebug = 1
#else
# define pdebug
#endif

guidolang::glangreader * gGLReader;

namespace guidolang 
{

int glparser::parse (FILE *fd) 
{
	if (!fd) {
		gGLReader->error("Invalid file descriptor", 0 );
		return(-1);
	}
	yyin = fd;
	pdebug;

	glanglineno = 0;
	int res = yyparse();
	yyrestart(yyin);
	BEGIN(INITIAL);
	return res;
}

int glparser::parse (const char *filename) 
{
	int res;
	if( !filename ) return -1; // parse error
	FILE * fd = fopen(filename,"r");
	if (fd == NULL){
		gGLReader->error("Cannot not open file", 0 );
		return(-1);
	}
	pdebug;
	res = parse(fd);
	fclose(fd);
	return res;
}

bool glparser::readstring (const char * buffer, glangreader * r) 
{
	gGLReader = r;
	if (!*buffer) return false;		// error for empty buffers

	YY_BUFFER_STATE b;
    b = yy_scan_string (buffer);	// Copy string into new buffer and Switch buffers
	pdebug;
	glanglineno = 1;
    int ret = yyparse();			// parse the string
    yy_delete_buffer(b);			// delete the new buffer
	BEGIN(INITIAL);					// and return to INITIAL state
 	return ret==0;
}

bool glparser::readfile (FILE* fd, glangreader * r) 
{
	gGLReader = r;
	return parse (fd) == 0;
}

bool glparser::readfile (const char * file, glangreader * r) 
{
	gGLReader = r;
	return parse (file) == 0;
}

} // namespace

#ifdef MAIN

using namespace std;
using namespace guidolang;

#define catOp(str,a,b,c)	*str+="("; *str+=*a; *str+=b; *str+=*c; *str+=")" 

class testreader : public glangreader
{ 
	public:
		virtual void	 newIDExpr			(const char *id, SGLExpr* e)	
			{  cout << "new identified expression: " << id << " = " << *e << endl; }

		virtual SGLExpr* newNamedExpr		(const char *name)	
			{  cout << "newNamedExpr: " << name << endl; return new SGLExpr(name); }

		virtual SGLExpr* newGroupExpr		(SGLExpr* e)	
			{  cout << "newGroupedExpr: " << *e << endl; SGLExpr* str = new string; *str+="<"; *str+=*e; *str+=">";return str; }

		virtual SGLExpr* newScoreExpr		(const char *gmn)	
			{  return new SGLExpr(gmn); }

		virtual SGLExpr* newBinaryExpr		(const char * name, SGLExpr* e1, SGLExpr* e2)	
			{ SGLExpr* str = new string(); catOp(str, e1, name, e2); cout << "newBinaryExpr: " << *str << endl; return str; }

		virtual SGLExpr* newAbstractExpr	(const char * name, SGLExpr* e1, SGLExpr* e2)	
			{ SGLExpr* str = new string(); catOp(str, e1, name, e2); cout << "newAbstractExpr: " << *str << endl; return str; }

		int  error (const char* msg, int lineno) 
			{ 	cerr << msg << " on line " << lineno << endl; return 0; }
};


int main (int argc, char * argv[])
{
	if (argc > 1) {
		testreader r;
		glparser glp;
		return glp.readfile (argv[1], &r) ? 0 : 1;
	}
 	return 0;
}
#endif

