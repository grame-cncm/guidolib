/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <cstdlib>
#include <cstring>

#include "gmntools.h"

#include <map>
#include <string>
using namespace std;

static map<string, T_REAL> unit2mmMap;

const double kGuidoDpi = 72.27;

//----------------------------------------------
static void unit2mmMapInit()
{
	unit2mmMap["mm"] = 1.0;
	unit2mmMap["cm"] = 10.0;
	unit2mmMap["m"]  = 1000.0;
	unit2mmMap["in"] = 25.4;
	unit2mmMap["pt"] = 25.4 / kGuidoDpi;
	unit2mmMap["pc"] = 25.4 / kGuidoDpi * 12.0;
}

static T_REAL unit2mm(const char * unit)
{
	if (unit2mmMap.size() == 0)
		unit2mmMapInit();
	map<string, T_REAL>::const_iterator found = unit2mmMap.find (unit);
	return (found == unit2mmMap.end()) ? 0.0 : found->second;
}


/* conversion between units (advanced guido) */
/* note: hs and rl are relative units dependent on
         the context; consequently, they cannot be converted
         using this function - see Advanced GUIDO Spec */
T_REAL gd_convertUnits(T_REAL value, CONST_T_STRING fromUnit, CONST_T_STRING toUnit) 
{
#if 1
	if(strcmp(fromUnit,toUnit)==0)
		return value;

	T_REAL f1 = unit2mm(fromUnit);
	T_REAL f2 = unit2mm(toUnit);
	if (!f2)	value = REAL_INVALID;	// invalid destination unit
	else		value *= f1 / f2;
	return value;

#else
  /* conversion required? */
  if(strcmp(fromUnit,toUnit)==0) {
    return value;
  }
  
  /* normalise to mm ('cause we are metric!) */
  if(strcmp(fromUnit,"cm")==0)
    value *= 10.0;
  else if(strcmp(fromUnit,"m")==0)
    value *= 1000.0;
  else if(strcmp(fromUnit,"in")==0)
    value *= 25.4;
  else if(strcmp(fromUnit,"pt")==0)
    value *= 25.4 / kGuidoDpi;
  else if(strcmp(fromUnit,"pc")==0)
    value *= 25.4 / kGuidoDpi * 12.0;

  /* convert from mm to toUnit */
  if(strcmp(toUnit,"mm")==0)
	;
//    return(value);
  else if(strcmp(toUnit,"cm")==0)
    value /= 10.0;
  else if(strcmp(toUnit,"m")==0)
    value /= 1000.0;
  else if(strcmp(toUnit,"in")==0)
    value /= 25.4;
  else if(strcmp(toUnit,"pt")==0)
    value /= 25.4 * kGuidoDpi;
  else if(strcmp(toUnit,"pc")==0)
    value /= 25.4 * kGuidoDpi /12.0;
  else
    value = REAL_INVALID;
  return value;
#endif
}

