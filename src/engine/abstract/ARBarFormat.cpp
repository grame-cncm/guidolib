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

#include "ARBarFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "GRDefine.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARBarFormat::ltpls(1);

ARBarFormat::ARBarFormat(const ARBarFormat & barfrmt)
{
	fStyle = barfrmt.getStyle();
	fRanges = barfrmt.getRanges();
}

ARBarFormat::~ARBarFormat()	{}
ARBarFormat::ARBarFormat()	{ fStyle = kStyleStaff; }


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

void ARBarFormat::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,style,staff,o;S,range,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * style = TagParameterString::cast(rtpl->RemoveHead());
			assert(style);
			TagParameterString * range = TagParameterString::cast(rtpl->RemoveHead());
			assert(range);

			if ( style->TagIsSet() ) {
				const NVstring & str = style->getValue();
				if (str == "system" )
					fStyle = kStyleSystem;
				else if (str == "staff" )
					fStyle = kStyleStaff;
				else cerr << "\\barFormat: unknown style " << str << endl;
			}
			if ( range->TagIsSet() )
				fRanges = getRanges(range->getValue());
			delete style;
			delete range;
		}

		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARBarFormat::printName(std::ostream& os) const
{
    os << "ARBarFormat";
}

void ARBarFormat::printGMNName(std::ostream& os) const
{
    os << "\\barFormat";
}

void ARBarFormat::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}


