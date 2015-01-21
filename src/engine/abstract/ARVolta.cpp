/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>

#include <iostream>
using namespace std;

#include "ARVolta.h"

#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"
#include "TimeUnwrap.h"

ListOfTPLs ARVolta::ltpls(1);

ARVolta::ARVolta()
{
	rangesetting = ONLY;

    mark   = NULL;
    pass   = NULL;
    format = NULL;
}

ARVolta::~ARVolta()
{
    delete mark;
    delete pass;
    delete format;
}

void ARVolta::browse(TimeUnwrap& mapper) const
{
//	std::cout << getRelativeTimePosition() << " ARVolta::browse: " << mPass << std::endl;
	mapper.AtPos (this, TimeUnwrap::kVolta);
}

bool ARVolta::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\voltaEnd",s))
		return true;
	return false;
}

void ARVolta::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty()) {
		ListOfStrings lstrs;
		lstrs.AddTail(/*"S,pass,,r;"*/ "S,mark,,r;S,format,,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >= 0 && rtpl) {
		if (ret == 0) {
			/*delete pass;
			pass = TagParameterString::cast(rtpl->RemoveHead());
			assert(pass);*/
            
			delete mark;
			mark = TagParameterString::cast(rtpl->RemoveHead());
			assert(mark);
            
			delete format;
			format = TagParameterString::cast(rtpl->RemoveHead());
			assert(format);
		}	

		delete rtpl;
	}

	tpl.RemoveAll();
}

void ARVolta::printName(std::ostream& os) const
{
    os << "ARVolta";
}

void ARVolta::printGMNName(std::ostream& os) const
{
    os << "\\volta";
}

void ARVolta::printParameters(std::ostream& os) const
{
    if (mark)
        os << "mark: " << mark->getValue() << "; ";

    if (format)
        os << "format: " << format->getValue() << "; ";

    ARMusicalTag::printParameters(os);
}


