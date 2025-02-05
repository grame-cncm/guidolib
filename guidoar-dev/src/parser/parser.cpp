/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

#include <iostream>

#include "gmnreader.h"
guido::gmnreader * gReader;

#define yyleng			guidoarleng
#define yyin			guidoarin
#define yyrestart		guidoarrestart
#define yy_current_buffer	guidoar_current_buffer
#define yy_delete_buffer	guidoar_delete_buffer
#define yy_scan_string		guidoar_scan_string

//#define yyDebug
#ifdef yyDebug
#define YYDEBUG	1
extern int yydebug;
#endif
#include "guidoparse.c++"
#include "unicode.c++"

using namespace std;

void convert_from_unicode (const char *filename);

namespace guido 
{

static int parse (FILE *fd) 
{
	if (!fd) {
		gReader->error("Invalid file descriptor", 0 );
		return(-1);
	}
	yyin = fd;
#ifdef yyDebug
	yydebug = 1;
#endif
	int res = yyparse();

	yyrestart(yyin);
	BEGIN(INITIAL);
	return res;
}

static int parse (const char *filename) 
{
	int res;
	if( !filename ) return -1; // parse error
	FILE * fd = fopen(filename,"r");
	if (fd == NULL){
		gReader->error("Cannot not open file", 0 );
		return(-1);
	}

	res = parse(fd);
	fclose(fd);
	return res;
}

bool readstring (const char * buffer, gmnreader * r) 
{
	gReader = r;

	if (!*buffer) return false;		// error for empty buffers

	YY_BUFFER_STATE b;
    /*Copy string into new buffer and Switch buffers*/
    b = yy_scan_string (buffer);

    /*Parse the string*/
    int ret = yyparse();

    /*Delete the new buffer*/
    yy_delete_buffer(b);

	BEGIN(INITIAL);
 	return ret==0;
}

bool readfile (FILE* fd, gmnreader * r) 
{
	gReader = r;
	int ret = parse (fd);
 	return ret==0;
}

bool readfile (const char * file, gmnreader * r) 
{
	gReader = r;
	convert_from_unicode (file);
	int ret = parse (file);
 	return ret==0;
}

#ifdef MAIN

class testreader : public gmnreader
{ 
	public:
		void noteInit	(const char *id){ cout << "noteInit " << id << endl; }
		void noteAcc	(int n)			{ cout << "noteAcc " << n << endl; }
		void noteOct	(int n)			{ cout << "noteOct " << n << endl; }
		void noteEnum	(long int n)	{ cout << "noteEnum " << n << endl; }
		void noteDenom	(long int n)	{ cout << "noteDenom " << n << endl; }
		void noteDot	(void)			{ cout << "noteDot " << endl; }
		void noteDdot	(void)			{ cout << "noteDdot " << endl; }
		void noteAbsDur	(long int n)	{ cout << "noteAbsDur " << n << endl; }
		void seqAppendNote (void)		{ cout << "seqAppendNote " << endl; }

		void chordInit	   (void)		{ cout << "chordInit " << endl; }
		void chordInitNote  (void)		{ cout << "chordInitNote " << endl; }
		void chordAppendNote(void)		{ cout << "chordAppendNote " << endl; }
		void seqAppendChord (void)		{ cout << "seqAppendChord " << endl; }

		void seqInit (void)				{ cout << "seqInit " << endl; }
		void seqExit (void)				{ cout << "seqExit " << endl; }

		void segmInit (void)			{ cout << "segmInit " << endl; }
		void segmExit (void)			{ cout << "segmExit " << endl; }
		void segmAppendSeq(void)		{ cout << "segmAppendSeq " << endl; }

		void tagStart	(const char* id, long int no)	{ cout << "tagStart " << id << " - " << no << endl; }
		void tagIntArg	(long int n)	{ cout << "tagIntArg " << n << endl; }
		void tagFloatArg (double r)		{ cout << "tagFloatArg " << r << endl; }
		void tagArgUnit	(const char* unit)	{ cout << "tagArgUnit " << unit << endl; }
		void tagStrArg	(char *s)		{ cout << "tagStrArg " << s << endl; }
		void tagAdd		(void)			{ cout << "tagAdd " << endl; }
		void tagAddArg	(const char *s)	{ cout << "tagAddArg " << s << endl; }
		void tagEnd		(void)			{ cout << "tagEnd " << endl; }

		void tagRange()					{ cout << "tagRange " << endl; }
		int  error (const char* msg, int lineno, int charno) 
			{ 	cerr << msg << " on line " << lineno << " near character " << charno << endl; return 0; }
};


int main (int argc, char * argv[])
{
	if (argc > 1) {
		testreader r;
		return readfile (argv[1], &r) ? 0 : 1;
	}
 	return 0;
}
#endif

} // namespace

