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

#include <iostream>
#include <sstream>
#include <regex>
#include <cctype>

#include "secureio.h"

#include "ARMeter.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

using namespace std;

ListOfTPLs ARMeter::ltpls(1);

//--------------------------------------------------------------------------------------
ARMeter::ARMeter() : fMeterDuration(0,1), fSingleUnit(true), fGroupComplex(false)
{
	fType = NONE;
	autoBarlines    = true;
	autoMeasuresNum = false;
}

//--------------------------------------------------------------------------------------
ARMeter::ARMeter(int num, int dnum) : fMeterDuration(num, dnum), fSingleUnit(true)
{
    std::stringstream bufferSStream;
    bufferSStream << fMeterDuration;
	mMeterName = bufferSStream.str().c_str();
	fType = NUMERIC;
	autoBarlines    = true;
	autoMeasuresNum = false;
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
	int n = meters.size() - 1;
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
	meters = finalizeMeters (meters);
//cerr << "ARMeter::parseMeters" << endl;
//for (size_t i=0; i<meters.size(); i++) {
//cerr << " -> " << meters[i] << endl;
//}
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
void ARMeter::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,type,4/4,r;F,size,1.0,o;S,autoBarlines,on,o;S,autoMeasuresNum,off,o;S,group,off,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString * tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);
			mMeterName = tps->getValue();

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
			assert(tpf);
			size = tpf->getValue();

			tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);
			autoBarlines = tps->getBool();

			tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);
			std::string on("on");
			std::string page("page");
            std::string system("system");
			const char* automeasures = tps->getValue();
			if (on == automeasures)
				autoMeasuresNum = kAutoMeasureNum;
			else if (page == automeasures)
				autoMeasuresNum = kAutoMeasureNumPage;
            else if (system == automeasures)
                autoMeasuresNum = kAutoMeasureNumSystem;
			else
				autoMeasuresNum = kNoAutoMeasureNum;

			tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);
			fGroupComplex =  tps->getBool();

            /**** Meter type analysis ****/
            if ((mMeterName == "C") || (mMeterName == "c")) {
                fType = C;
				fMetersVector.push_back(Fraction(4,4));
            }
            else if ((mMeterName == "C/") || (mMeterName == "c/")) {
                fType = C2;
				fMetersVector.push_back(Fraction(2,2));
            }
            else if ((mMeterName == "") || !isNumeric(mMeterName)) {
                fType = NONE;
				fMetersVector.push_back(Fraction(4,4));
            }
            else {
                fType       = NUMERIC;
				fMetersVector	= parseMeters(mMeterName);
				fSingleUnit		= singleUnit (fMetersVector);
				fMeterDuration	= metersDuration(fMetersVector);
            }
		}
		delete rtpl;
	}
	else {
		fType = NONE;
		fMetersVector.push_back(Fraction(4,4));
	}

	fMeterDuration = metersDuration(fMetersVector);
    tpl.RemoveAll();
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

//--------------------------------------------------------------------------------------
bool ARMeter::IsStateTag() const					{ return true; }

void ARMeter::printName(std::ostream& os) const		{ os << "ARMeter"; }
void ARMeter::printGMNName(std::ostream& os) const	{ os << "\\meter"; }

void ARMeter::printParameters(std::ostream& os) const
{
    os << "name: \"" << mMeterName << "\"; ";
    os << "autoBarlines: " << (autoBarlines ? "true" : "false") << "; ";
    os << "autoMeasuresNum: " << (autoMeasuresNum ? "true" : "false") << "; ";

    ARMusicalTag::printParameters(os);
}
