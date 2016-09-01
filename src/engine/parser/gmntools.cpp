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

//----------------------------------------------
static T_REAL unit2mm(const char * unit)
{
	if (unit2mmMap.size() == 0)
		unit2mmMapInit();
	map<string, T_REAL>::const_iterator found = unit2mmMap.find(unit);
	return (found == unit2mmMap.end()) ? 0.0 : found->second;
}


/* Conversion between units (advanced guido) */
/* note: hs and rl are relative units dependent on
         the context; consequently, they cannot be converted
         using this function - see Advanced GUIDO Spec */
bool gd_convertUnits(T_REAL fromValue, CONST_T_STRING fromUnit, CONST_T_STRING toUnit, double &toValue) 
{
    bool returnValue = false;

	if (strcmp(fromUnit, toUnit) == 0)
	{
        toValue  = fromValue;
        returnValue = true;
    }

    T_REAL f1 = unit2mm(fromUnit);
    T_REAL f2 = unit2mm(toUnit);

    if (f2)
    {
        fromValue  *= f1 / f2;
        toValue     = fromValue;
        returnValue = true;
    }

    return returnValue;
}