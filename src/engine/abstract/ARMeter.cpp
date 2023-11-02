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

#include <iostream>
#include <sstream>
#include <regex>

#include "ARMeter.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

using namespace std;


//--------------------------------------------------------------------------------------
ARMeter::ARMeter() : fMeterDuration(0,1), fSingleUnit(true), fGroupComplex(false)
{
	setupTagParameters (gMaps->sARMeterMap);
	fType = NONE;
	fAutoBarlines    = true;
	fAutoMeasuresNum = kNoAutoMeasureNum;
}

//--------------------------------------------------------------------------------------
ARMeter::ARMeter(int num, int dnum) : fMeterDuration(num, dnum), fSingleUnit(true)
{
	setupTagParameters (gMaps->sARMeterMap);
	
    std::stringstream bufferSStream;
    bufferSStream << fMeterDuration;
	fMeterName = bufferSStream.str().c_str();
	fType = NUMERIC;
	fAutoBarlines    = true;
	fAutoMeasuresNum = kNoAutoMeasureNum;
}

//--------------------------------------------------------------------------------------
// converts a meter string in the form "n[/d]" to a fraction
// warning: the [/d] part is optional are when not present, the denominator is set to 0
Fraction ARMeter::str2meter(string str)  const
{
	size_t n = str.find("/");
	int num = 0;
	int dnum = 0;
	if (n == string::npos)
		num = std::stoi(str);
	else {
		num = std::stoi(str.substr(0,n));
		dnum = std::stoi(str.substr(n+1));
	}
	return Fraction (num, dnum);
}

//--------------------------------------------------------------------------------------
// check if the meters set has a single unit (denominator)
bool ARMeter::singleUnit (const std::vector<Fraction>& meters)  const
{
	int unit = 0;
	for (size_t i=0; i < meters.size(); i++) {
		int d = meters[i].getDenominator();
		if (!d) continue;		// unit is implicit
		if (!unit) unit = d;
		else if (d != unit) return false;
	}
	return true;
}

//--------------------------------------------------------------------------------------
// parse a meters string in the form "n[/d] [+] n[/d] [+] ... [+] n[/d]"
// where parts in brackets [] are optionnal
// note that the method is relaxed regarding extra characters that are actually ignored
const vector<Fraction> ARMeter::finalizeMeters(const std::vector<Fraction>& meters)  const
{
	vector<Fraction> out;
	int n = int(meters.size() - 1);
	int lastdnum = 4;		// default denominator
	while (n >= 0) {
		Fraction m = meters[n--];
		int d = m.getDenominator();
		if (!d) m.setDenominator(lastdnum);
		else lastdnum = d;
		out.insert (out.begin(), m);
	}
	return out;
}

//--------------------------------------------------------------------------------------
// parse a meters string in the form "n[/d] [+] n[/d] [+] ... [+] n[/d]"
// where parts in brackets [] are optionnal
// note that the method is relaxed regarding extra characters that are actually ignored
const vector<Fraction> ARMeter::parseMeters(std::string str)  const
{
	vector<Fraction> meters;
	string exp = "([1-9][0-9]*([ 	]*/[1-9][0-9]*){0,1})+";
	std::regex e (exp);
	smatch m;
	while (regex_search (str, m, e, regex_constants::match_any)) {
		string match = *m.begin();
		meters.push_back(str2meter (match));
		str = m.suffix().str();
	}
	if (meters.size()) meters = finalizeMeters (meters);
	return meters;
}

//--------------------------------------------------------------------------------------
Fraction ARMeter::metersDuration (const std::vector<Fraction>& meters)  const
{
	Fraction out (0,1);
	if (fSingleUnit) {		// intended to avoid fraction normalization
		int n = 0, d = 1;
		for (size_t i=0; i < meters.size(); i++) {
			n += meters[i].getNumerator();
			d = meters[i].getDenominator();
		}
		out.set (n, d);
	}
	else {
		for (size_t i=0; i<meters.size(); i++)
			out += meters[i];
		out.normalize();
	}
	return out;
}

 //--------------------------------------------------------------------------------------
 void ARMeter::setMeter (const string& meter)
 {
	fMeterName = meter;
	if ((meter == "C") || (meter == "c")) {
		fType = C;
		fMetersVector.push_back(Fraction(4,4));
	}
	else if ((meter == "C/") || (meter == "c/")) {
		fType = C2;
		fMetersVector.push_back(Fraction(2,2));
	}
	else if ((meter == "") || !isNumeric(meter)) {
		fType = NONE;
		fMetersVector.push_back(Fraction(4,4));
	}
	else {
		fType       = NUMERIC;
		fMetersVector	= parseMeters(meter);
		fSingleUnit		= singleUnit (fMetersVector);
		fMeterDuration	= metersDuration(fMetersVector);
	}
	if (fMetersVector.empty()) {
		fType = NONE;
		fMetersVector.push_back(Fraction(4,4));
	}
}

//--------------------------------------------------------------------------------------
void ARMeter::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString* p = getParameter<TagParameterString>(kTypeStr);
	if (p) setMeter (p->getValue());
	else return;

	string am = getParameter<TagParameterString>(kAutoMeasNumStr, true)->getValue();
	if (am == "on")				fAutoMeasuresNum = kAutoMeasureNum;
	else if (am == "page")		fAutoMeasuresNum = kAutoMeasureNumPage;
	else if (am == "system")	fAutoMeasuresNum = kAutoMeasureNumSystem;

	fHidden			= getParameter<TagParameterString>(kHiddenStr, true)->getBool();
	fAutoBarlines	= getParameter<TagParameterString>(kAutoBarlinesStr, true)->getBool();
	fGroupComplex	= getParameter<TagParameterString>(kGroupStr, true)->getBool();
	fMeterDuration	= metersDuration(fMetersVector);
}

//--------------------------------------------------------------------------------------
bool ARMeter::isNumeric	(const std::string& meter)  const
{
	const char* ptr = meter.c_str();
	while (*ptr) {
		int c = *ptr++;
		if (!isdigit(c) && !isblank(c) && (c != '+') && (c != '/')) return false;
	}
	return true;
}


