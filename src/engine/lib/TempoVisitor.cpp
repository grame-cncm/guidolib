/*
  GUIDO Library
  Copyright (C) 2016  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <regex>
#include <cctype>

#include "TempoVisitor.h"
#include "ARTempo.h"

using namespace std;

static GuidoDate toGuidoDate (const Fraction& d)
{
	GuidoDate date;
	date.num = d.getNumerator();
	date.denom = d.getDenominator();
	return date;
}

//------------------------------------------------------------------------------
int TempoVisitor::toValue (const string& s) const
{
	try {
	const char * ptr = s.c_str();
	while (*ptr && !std::isdigit(*ptr)) ptr++;
	string num (ptr);
	return std::stoi(num);
	}
	catch (exception e ) {
	return 0;
	}
}

//------------------------------------------------------------------------------
void TempoVisitor::getText (char* dst, const ARTempo* ar) const
{
	int i = 0;
	dst[0] = 0;
	const FormatStringParserResult& s = ar->getTempoMark();
	for (auto e: s) {
		if (e.second == FormatStringParser::kLitteral) {
			const char* ptr = e.first.c_str();
			for (i = 0; (i < 126) && *ptr; i++)
				dst[i] = *ptr++;
			dst[i] = 0;
			break;
		}
	}
	if (i > 0) {
		i--;
		while ((i >= 0) && ((dst[i]==' ') || (dst[i]=='	')))
			dst[i--] = 0;
	}
}

//------------------------------------------------------------------------------
void TempoVisitor::visitIn (ARMusicalTag* obj)
{
	const ARTempo* ar = obj->isARTempo();
	if (ar) {
		GuidoTempo tempo;
		tempo.voice = fCurrentVoice;
		Fraction date = ar->getRelativeTimePosition();
		Fraction dur = ar->getBpmUnit();
		tempo.date = toGuidoDate(date);
		getText (tempo.text, ar);

		if (ar->isNoteEquiv()) {
			tempo.type = kUnitUnitTempo;
			tempo.unit = toGuidoDate(dur);
			tempo.unitvalue = toGuidoDate(ar->getBpmValue());
		}
		else if (ar->hasBpmInfos()) {
			tempo.type = kUnitValueTempo;
			tempo.unit = toGuidoDate(dur);
			tempo.value = int(float(ar->getQpmValue()));
		}
		else {
			tempo.type = kTextualTempo;
			const FormatStringParserResult& s = ar->getTempoMark();
			for (size_t i=0; i < s.size() - 1; i++) {
				if (s[i].second == FormatStringParser::kSpecial) {
					tempo.unit = toGuidoDate(ar->string2Duration(s[i].first.c_str()));
					if ((i < s.size() - 2) && (s[i+2].second == FormatStringParser::kSpecial)) {
						tempo.type = kUnitUnitTempo;
						tempo.unitvalue = toGuidoDate(ar->string2Duration(s[i+2].first.c_str()));
						break;
					}
					else {
						tempo.type = kUnitValueTempo;
						tempo.value = toValue(s[i+1].first);
						break;
					}
				}
			}
		}
		fTempi.push_back (tempo);
	}
}
//------------------------------------------------------------------------------
GuidoTempoList TempoVisitor::getTempi() const
{
	size_t n = fTempi.size();
	GuidoTempoList tempi = new GuidoTempo[n];
	for (size_t i=0; i<n; i++)
		tempi[i] = fTempi[i];
	return tempi;
}


