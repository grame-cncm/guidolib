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

#include "ARBarFormat.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
//#include "TagParameterFloat.h"
#include "GRDefine.h"

using namespace std;


ARBarFormat::ARBarFormat(const ARBarFormat & barfrmt)
{
	setupTagParameters (gMaps->sARBarFormatMap);
	fStyle = barfrmt.getStyle();
	fRanges = barfrmt.getRanges();
}

ARBarFormat::ARBarFormat()
{
	setupTagParameters (gMaps->sARBarFormatMap);
	fStyle = kStyleStaff;
}

ARBar::TRanges ARBarFormat::getRanges (const NVstring &str)
{
	ARBar::TRanges ranges;
	size_t pos = -1;
	do {
		pos += 1;
		size_t sep = str.find(":", pos);
		string range = str.substr(pos, sep);

		size_t dashPos = range.find("-", 0);
		int start=0, end=0;
		if (dashPos == string::npos) {
			stringstream s(range);
			s >> start;
			end = start;
		}
		else {
			string startStr = range.substr(0, dashPos);
			stringstream s1(startStr);
			s1 >> start;

			std::string endStr = range.substr(dashPos+1);
			std::stringstream s2(endStr);
			s2 >> end;
		}
		if (start && end) ranges.push_back(make_pair(start, end));
		else break;
		pos = sep;
	} while (pos != string::npos);
	return ranges;
}

void ARBarFormat::setTagParameters (const TagParameterMap& params)
{
    const TagParameterString* p = getParameter<TagParameterString>(kStyleStr);
	if (p) {
		const string str = p->getValue();
		if (str == "system" )
			fStyle = kStyleSystem;
		else if (str == "staff" )
			fStyle = kStyleStaff;
		else cerr << "\\barFormat: unknown style " << str << endl;
	}
	
	p = getParameter<TagParameterString>(kRangeStr);
	if (p)
		fRanges = getRanges(p->getValue());
}

