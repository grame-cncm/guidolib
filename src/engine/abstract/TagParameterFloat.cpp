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

#include <ctype.h> // for isalpha, isdigit (must keep the ".h" ) 
#include <cstring>
#include <cstdlib>

#include <string>
#include <regex>

#include "TagParameterFloat.h"
#include "ARUnits.h"
#include "gmntools.h" // for gd_convertUnits
#include "GRDefine.h"
#include "secureio.h"

using namespace std;

TagParameterFloat::TagParameterFloat(const TagParameterFloat & tpf)
	: TagParameter(tpf)
{
	fUnittag = tpf.fUnittag;
	fValue = tpf.fValue;
	fIsDuration = tpf.fIsDuration;
	fDuration = tpf.fDuration;
	fUnit = tpf.getUnit();
}

TagParameterFloat::TagParameterFloat( bool unittag ) : fUnittag(unittag), fValue(0.f)	{}
TagParameterFloat::TagParameterFloat(float val)		 : fUnittag(false), fValue(val)		{}

void  TagParameterFloat::set( const TagParameterFloat & in )
{
	TagParameter::set( in );
	fUnit = in.getUnit();
	fUnittag = in.fUnittag;
	fValue = in.fValue;
	fIsDuration = in.fIsDuration;
	fDuration = in.fDuration;
}


void TagParameterFloat::print(std::ostream& out)
{
	TagParameter::print (out);
	const char* u = TagIsUnitTag() ? getUnit() : "";
	if (fIsDuration) {
		out << std::string(fDuration);
	}
	else out << getValue() << u;
}

void TagParameterFloat::reset(float inFloatValue, const char * inUnit)
{
	fIsDuration = false;
	fDuration = Fraction(0,1);
	fValue = inFloatValue;
	string u (inUnit);
	if (u.size()) {
		fUnit = u;
		fUnittag = true;
	}
	else {
		fUnit.clear();
		fUnittag = false;
	}
}

const TYPE_FLOATPARAMETER TagParameterFloat::getValue(float curLSPACE) const
{
	if (fIsDuration)
		return (float) fDuration;
	if (fUnittag) {
		string tmpunit;
		if (fUnit.empty()) {
			// no unit is given -> then
			// we take the unit specified by the units-tag ...
			tmpunit = ARUnits::getUnit();
		}
		else tmpunit = fUnit;
		if (tmpunit == "hs")
			return ((float) (fValue * curLSPACE * 0.5f));
		
        double result;
		bool conversionOk = gd_convertUnits((double) fValue, tmpunit.c_str(), "cm", result);
		
        if (conversionOk)
            return ((float)result * kCmToVirtual);
        else
        {
            // Failure. Thing is that getValue should returns an errorCode, 
            // and not directly the value (like convertValue).
        }
	}
	return fValue;
}

static bool parseDots (int dots, Fraction& value)
{
	if (!dots) return true;
	Fraction add = value;
	for (int i=0; i<dots; i++) {
		add = add * Frac_1_2;
		value += add;
	}
	return true;
}

static bool parseDurationString(const std::string& val, Fraction& out)
{
	// syntax: [sign]num/den[.]* (dot(s) optional)
	std::regex e("^\\s*([+-]?)(\\d+)/(\\d+)(\\.*)\\s*$");
	std::smatch match;
	if (!std::regex_search(val, match, e))
		return false;

	long num = std::stol(match[2]);
	long den = std::stol(match[3]);
	if (den == 0) return false;
	int dots = (int)match[4].str().size();
	out = Fraction((int)num, (int)den);
	parseDots(dots, out);
	if (match[1].str() == "-")
		out = out * -1;
	return true;
}

void TagParameterFloat::setValue(const char * p)
{
	string val = p;
	string exp = "^([1-9][0-9]*)(cm|mm|hs|in|m|pc|pt)$";
	std::regex e (exp);
	std::smatch match;
	
	fUnit.clear();
	fIsDuration = false;
	fDuration = Fraction(0,1);
	if (regex_search (val, match, e)) {
		string vstr = match[1];
		fValue = (float)atof(vstr.c_str());
		fUnit = match[2];
		fUnittag = true;
	}
	else {
		Fraction dur;
		if (parseDurationString(val, dur)) {
			fDuration = dur;
			fIsDuration = true;
			fUnittag = false;
			fValue = (float) dur;
		}
		else fValue = (float)atof(p);
	}
}

void TagParameterFloat::setUnit(const char * unit)
{
	fUnit = unit;
	fUnittag = true;
}

bool TagParameterFloat::copyValue(const TagParameter * tp)
{
	const TagParameterFloat * tpf = TagParameterFloat::cast(tp);
	if (!tpf) return false;

	fIsDuration = tpf->fIsDuration;
	fDuration = tpf->fDuration;
	if (fUnittag)
		fUnit = tpf->getUnit();
	else if (tpf->getUnit()[0] != 0)
		return false;
	else fUnit.clear();
	fValue = tpf->fValue;		
	return true;
}

bool TagParameterFloat::convertValue(float fromValue, double &toValue, const char * unit, float curLSPACE)
{
    bool returnValue = false;

	const char *tmp = unit;
	if (!tmp[0])  {
		// no unit is given -> then we take the unit specified by the units-tag ...
		tmp = ARUnits::getUnit();
	}
	if (!strcmp(tmp, "hs"))  {
        returnValue = true;
        toValue = fromValue * curLSPACE * 0.5f;
    }
    else {
        double result;
        returnValue = gd_convertUnits (fromValue, tmp, "cm", result);
        if (returnValue)  toValue = result * kCmToVirtual;
    }
    return returnValue;
}
