/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gddefs.h"

/* --- utility functions: --- */

void gd_fracNorm(long int *a, long int *b) {
	/* normalises fraction a/b */
  long int n, sgn;

  if (*b==0)
	 return;

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

int gd_noteName2pc (const char *name)
{
	std::string note (name);
	if ((note == "c") || (note == "do") || (note == "ut"))	return NOTE_C;
	if (note == "cis")										return NOTE_CIS;
	if ((note == "d") || (note == "re"))					return NOTE_D;
	if (note == "dis")										return NOTE_DIS;
	if ((note == "e") || (note == "mi"))					return NOTE_E;
	if ((note == "f") || (note == "fa"))					return NOTE_F;
	if (note == "fis")										return NOTE_FIS;
	if ((note == "g") || (note == "so") || (note == "sol"))	return NOTE_G;
	if (note == "gis")										return NOTE_GIS;
	if ((note == "a") || (note == "la"))					return NOTE_A;
	if (note == "ais")										return NOTE_AIS;
	if ((note == "h") || (note == "b") || (note == "si") || (note == "ti"))	return NOTE_H;
	if ((note == "_") || (note == "rest"))					return REST;
	if (note == "empty")									return EMPTY;
	if (note == "tab")										return REST;
	std::cerr << "Unknown notename, replaced by rest." << std::endl;
	return REST;
}

//int gd_noteName2pc (const char *name)
//{
//  int res;
//  if ( strcmp(name,"c") == 0
//		 || strcmp(name,"do") == 0
//		 || strcmp(name,"ut") == 0)
//	 res =  NOTE_C;
//  else if (strcmp(name,"cis") == 0 )
//	 res =  NOTE_CIS;
//  else if ( strcmp(name,"d") == 0
//				|| strcmp(name,"re") == 0 )
//	 res =  NOTE_D;
//  else if (strcmp(name,"dis") == 0 )
//	 res =  NOTE_DIS;
//  else if ( strcmp(name,"e") == 0
//				|| strcmp(name,"mi") == 0 )
//	 res =  NOTE_E;
//  else if ( strcmp(name,"f") == 0
//				|| strcmp(name,"fa") == 0 )
//	 res =  NOTE_F;
//  else if (strcmp(name,"fis") == 0 )
//	 res =  NOTE_FIS;
//  else if ( strcmp(name,"g") == 0
//            || strcmp(name,"so") == 0
//            || strcmp(name,"sol") == 0 )
//    res =  NOTE_G;
//  else if (strcmp(name,"gis") == 0 )
//	 res =  NOTE_GIS;
//  else if ( strcmp(name,"a") == 0
//            || strcmp(name,"la") == 0 )
//	 res =  NOTE_A;
//  else if (strcmp(name,"ais") == 0 )
//	 res =  NOTE_AIS;
//  else if ( strcmp(name,"h") == 0
//				|| strcmp(name,"b") == 0
//				|| strcmp(name,"si") == 0
//				|| strcmp(name,"ti") == 0 )
//	 res =  NOTE_H;
//  else if (strcmp(name,"_") == 0 )
//    res =  REST;
//  else if (strcmp(name,"rest") == 0)
//  {
//	  res = REST;
//  }
//  else if (strcmp(name,"empty") == 0)
//  {
//	  res = EMPTY;
//  }
//  else {
//	 res =  REST;
//	  std::cerr << "Unknown notename, replaced by rest." << std::endl;
//	 }
//  return res;
//}

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


