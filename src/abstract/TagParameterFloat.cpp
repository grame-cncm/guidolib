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

#include <ctype.h> // for isalpha, isdigit (must keep the ".h" ) 
#include <cstring>
#include <cstdlib>

#include "TagParameterFloat.h"
#include "ARUnits.h"
#include "gmntools.h" // for gd_convertUnits
#include "GRDefine.h"
#include "secureio.h"


TagParameterFloat::TagParameterFloat(const TagParameterFloat & tpf)
	: TagParameter(tpf)
{
	if (kUnitLen) strncpy (fUnit, tpf.fUnit, kUnitLen);
	fUnittag = tpf.fUnittag;
	fValue = tpf.fValue;
}

TagParameterFloat::TagParameterFloat(float theFloat)
	: fValue(theFloat)
{
	fUnittag = 0;
	fUnit[0] = 0;
}

void  TagParameterFloat::set( const TagParameterFloat & in )
{
	TagParameter::set( in );
	if (kUnitLen) strncpy(fUnit, in.fUnit, kUnitLen);
	fUnittag = in.fUnittag;
	fValue = in.fValue;
}

void 
TagParameterFloat::reset(float inFloatValue, const char * inUnit)
{
	fUnittag = false;
	fValue = inFloatValue;
	fUnit[0] = '\0'; 

	if(!strncmp(fUnit, inUnit, kUnitLen)) {
		if (kUnitLen) strncpy(fUnit, inUnit, kUnitLen);
		fUnit[2] = 0;
		fUnittag = true;
	}
}

const TYPE_FLOATPARAMETER  TagParameterFloat::getValue(float curLSPACE) const
{
	if (fUnittag)
	{
		char tmpunit[kUnitLen];
		if (kUnitLen) strncpy(tmpunit, fUnit, kUnitLen);
		if (tmpunit[0] == 0)
		{
			// no unit is given -> then
			// we take the unit specified by the units-tag ...
			if (kUnitLen) strncpy(tmpunit, ARUnits::getUnit().c_str(), kUnitLen);
		}
		if (!strcmp(tmpunit,"hs"))
			return ((float) (fValue * curLSPACE * 0.5f));
		
		const float tmpval = (float)gd_convertUnits((double) fValue, tmpunit, "cm" );
		return (tmpval * kCmToVirtual);
	}
	return fValue;
}

void TagParameterFloat::setValue(const char * p)
{
	// this is a difficult function.
	// It must set the parameter and do it a unit-transform, if necessary ...
	// first extract the unit.
	const size_t length = strlen(p);
	if (length == 0)  {  
		fValue = 0;
		return;
	}

	size_t i = length - 1;
	while ( !isdigit((int) p[i]) ) --i;

	// - Allocate a temp string table. It tryes to do it on the stack
	const size_t kStackStrLen = 64;
	char tmpStack [ kStackStrLen ];		// stack temp string
	char * tmpHeap = 0;					// heap temp string

	char * tmp = 0;						// work string
	if(( i + 2 ) >= kStackStrLen )
	{
		tmpHeap = new char [ i + 2 ];	// Too long to convert on the stack, we use allocate a string on the heap
		tmp = tmpHeap;
	}									
	else tmp = tmpStack;	
			
	// - Copy required chars
	strncpy( tmp, p, i+2 );
	// - Convert them to float
	fValue = (float) atof(tmp);
 	delete [] tmpHeap;

	// we need to to a conversion of units ...
	if (i != length - 1)
	{
		if (kUnitLen) strncpy(fUnit, &p[i+1], kUnitLen);
		fUnit[2] = 0;
		fUnittag = true;
	}
}

void TagParameterFloat::setUnit(const char * un)
{
	// one should probable check the unit
	// here -> if no valid unit is given
	// use cm/inches or whatever ...
	if (kUnitLen) strncpy(fUnit,un,kUnitLen);
	fUnit[2] = 0;
	fUnittag = true;
}

bool TagParameterFloat::copyValue(const TagParameter * tp)
{
	const TagParameterFloat * tpf = TagParameterFloat::cast(tp);
	if (tpf)
	{
		if (!fUnittag && tpf->fUnit[0] != 0)
			return false;
	
		if (fUnittag && kUnitLen)
			strncpy(fUnit, tpf->fUnit,kUnitLen);
		else
			fUnit[0] = '\0';
		fValue = tpf->fValue;		
	}
	else return false;
	return true;
}

TYPE_FLOATPARAMETER TagParameterFloat::convertValue (float value, const char * unit, float curLSPACE)
{
	const char * tmp = unit;
	if (!tmp[0]) {
		// no unit is given -> then we take the unit specified by the units-tag ...
		tmp = ARUnits::getUnit().c_str();
	}
	if (!strcmp(tmp, "hs"))
		return value * curLSPACE * 0.5f;

	const float tmpval = (float)gd_convertUnits (value, tmp, "cm" );
	return tmpval * kCmToVirtual;
}
