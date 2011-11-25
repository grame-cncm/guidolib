/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

*/

/*
	changes log:
	03/07/15 jk: added unicode file support, for unicode files produced by the Sibelius2GUIDO Plugin
*/

//#define yyDebug
#ifdef yyDebug
#define YYDEBUG	1
extern int yydebug;
#endif

#ifdef WIN32
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning (disable : 4267 4018 4244 4065)
#else
static void yyunput(int, char*) __attribute__((unused));
#endif

#include "guido.h"
#include "guidoparse.cxx"

/*
extern "C" {
YY_BUFFER_STATE guido_scan_string YY_PROTO(( const char *yy_str ));
}
*/

int gd_error(const char *msg);


/* --- initialisation, has to be called before  any other gd_*  function is called --- */

void gd_init(void) {
	gd_imp_init();
	setlocale(LC_NUMERIC, "C");
}

void gd_exit(void)
{
	setlocale(LC_NUMERIC, 0);
	gd_imp_exit();
}


/* --- parser interface: --- */
int parse_mode;

long int lnr = 1;
long int cnr = 0;
long int cmnt_level;

static int checkUnicode(FILE *fd)
{
	rewind( fd );
	if ( fd ) {
		int c = fgetc( fd );
		if( c == 0xff || c == 0xfe) {
			c = fgetc( fd );
			if( c == 0xfe || c == 0xff )
				return 1;
		}
	}
	return 0;
}

static void uniconv(const char *filename)
{
	FILE * fd = fopen(filename,"rb");	// open file
	if (!fd) return;

	if (checkUnicode (fd)) {
		fseek(fd, 0, SEEK_END);
		long int len = ftell(fd);
		char * content = new char[len+1];
		if( content ) {
			rewind( fd );
			fread (content, 1, len, fd);
			fclose(fd);

			fd = fopen(filename,"wt");
			if (fd) {
				int i;
				for( i = 0; i < len; i++ ) {
					if( content[i] > 0 )
						fputc(content[i], fd);
				}
				fclose( fd );
			}
			delete [] content;
		}
	}
	else fclose(fd);
}

int gd_parse_buffer(const char *buffer)
{
	lnr = 1;
	cnr = 0;

	int res;
	if (!*buffer) return -1;		// error for empty buffers

	gd_init();
	YY_BUFFER_STATE b;
    /*Copy string into new buffer and Switch buffers*/
    b = guido_scan_string (buffer);

    /*Parse the string*/
	guidolineno = 1;
    res = guidoparse();

    /*Delete the new buffer*/
    guido_delete_buffer(b);

	BEGIN(INITIAL);

	guidorestart(guidoin);
#if !defined(linux) && !defined(WIN32)
	guidolex_destroy();
#endif
//	fclose(guidoin);
	gd_exit();

	return res;
}

int gd_parse(const char *filename, int mode)
{
	lnr = 1;
	cnr = 0;

	int res;
#ifdef yyDebug
	yydebug = 1;
#endif

	gd_init();
	uniconv(filename);
	if((guidoin= fopen(filename,"r")) == NULL){
		gd_error("Could not open file");
		return -1;
	}

	guidolineno = 1;
	res = guidoparse();
	guidorestart(guidoin);
#if !defined(linux) && !defined(WIN32)
	guidolex_destroy();
#endif
	fclose(guidoin);
	gd_exit();
	return res;
}


/* --- utility functions: --- */

void gd_fracNorm(long int *a, long int *b) {
	/* normalises fraction a/b */
  long int n, sgn;

  if (*b==0)
	 return;

  /// multiplication can cause range overflow errors!
  /*
  if ((*a) * (*b) < 0)
	 sgn = -1;
  else
	 sgn = +1;
   */
  if( ((*a) > 0 && (*b) > 0) ||
      ((*a) < 0 && (*b) < 0) )
      sgn = 1;
  else
      sgn = -1;

  *a = labs(*a);
  *b = labs(*b);
  if (*a != 0) {
    for (n=2; n <= *b; n++)
		while (*a % n == 0 && *b % n == 0) {
	*a /= n;
	*b /= n;
	}
	 }
  else
	 *b = 1;
  *a = sgn * (*a);
}

void gd_fracAdd(long int *a, long int *b,
					 const long int c, const long int d) {
  /* sets a/b := a/b+c/d, normalised */
  /* normalise a/b, c/d before using this function */
  if (*b==0 || d==0)
	 return;

  *a = (*a) *d + c* (*b);
  *b = (*b) *d;
  gd_fracNorm(a,b);
}


int gd_fracCmp(long int a, long int b,
					long int c, long int d) {
  /* returns +1 if a/b > c/d, 0 if a/b=c/d, -1 else */
  /* normalise a/b, c/d before using this function */
  long int de;

  if (b==0 || d==0)
	 return(0);

  de = a*d-c*b;
  if (de>0) return(1);
  else if (de<0) return(-1);
  else return(0);
}


/* --- action functions: --- */

int gd_noteName2pc (const char *name) {
  int res;
  if ( strcmp(name,"c") == 0
		 || strcmp(name,"do") == 0
		 || strcmp(name,"ut") == 0)
	 res =  NOTE_C;
  else if (strcmp(name,"cis") == 0 )
	 res =  NOTE_CIS;
  else if ( strcmp(name,"d") == 0
				|| strcmp(name,"re") == 0 )
	 res =  NOTE_D;
  else if (strcmp(name,"dis") == 0 )
	 res =  NOTE_DIS;
  else if ( strcmp(name,"e") == 0
				|| strcmp(name,"mi") == 0 )
	 res =  NOTE_E;
  else if ( strcmp(name,"f") == 0
				|| strcmp(name,"fa") == 0 )
	 res =  NOTE_F;
  else if (strcmp(name,"fis") == 0 )
	 res =  NOTE_FIS;
  else if ( strcmp(name,"g") == 0
            || strcmp(name,"so") == 0
            || strcmp(name,"sol") == 0 )
    res =  NOTE_G;
  else if (strcmp(name,"gis") == 0 )
	 res =  NOTE_GIS;
  else if ( strcmp(name,"a") == 0
            || strcmp(name,"la") == 0 )
	 res =  NOTE_A;
  else if (strcmp(name,"ais") == 0 )
	 res =  NOTE_AIS;
  else if ( strcmp(name,"h") == 0
				|| strcmp(name,"b") == 0
				|| strcmp(name,"si") == 0
				|| strcmp(name,"ti") == 0 )
	 res =  NOTE_H;
  else if (strcmp(name,"_") == 0 )
    res =  REST;
  else if (strcmp(name,"rest") == 0)
  {
	  res = REST;
  }
  else if (strcmp(name,"empty") == 0)
  {
	  res = EMPTY;
  }
  else {
	 res =  REST;
	 gd_error ("Unknown notename, replaced by rest.");
	 }
  return(res);
}

const char * gd_pc2noteName(int pitch)
{
  switch (pitch)
    {
    case REST:
      return "_";
    case NOTE_C:
      return "c";
    case NOTE_D:
      return "d";
    case NOTE_E:
      return "e";
    case NOTE_F:
      return "f";
    case NOTE_G:
      return "g";
    case NOTE_A:
      return "a";
    case NOTE_H:
      return "h";
    case NOTE_CIS:
      return "cis";
    case NOTE_DIS:
      return "dis";
    case NOTE_FIS:
      return "fis";
    case NOTE_GIS:
      return "gis";
    case NOTE_AIS:
      return "ais";
    }
  return "";
}


