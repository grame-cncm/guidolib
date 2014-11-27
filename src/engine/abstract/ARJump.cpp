/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>
#include <iostream>
using namespace std;

#include "ARJump.h"

#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs ARJump::ltpls(1);

ARJump::ARJump(string mark) {
	FormatStringParser p;
	p.parse (mark.c_str(), mMark);
}

ARJump::~ARJump()   {}

void ARJump::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty()) {
		ListOfStrings lstrs;
		lstrs.AddTail( "S,m,,o;I,id,0,o" );
		CreateListOfTPLs(ltpls,lstrs);
	}
	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if( ret >= 0 && rtpl ) {
		if( ret == 0 ) {
			TagParameterString * tps = TagParameterString::cast(rtpl->RemoveHead());
			if (tps && strlen(tps->getValue())) {
				FormatStringParser p;
				mMark.clear();
				p.parse (tps->getValue(), mMark);
			}
			delete tps;

			TagParameterInt * tpi = TagParameterInt::cast(rtpl->RemoveHead());
			if (tpi) mID = tpi->getValue();
			delete tpi;
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

void ARJump::printName(std::ostream& os) const
{
    os << "ARJump";
}

void ARJump::printGMNName(std::ostream& os) const
{
    os << "\\jump";
}

void ARJump::printParameters(std::ostream& os) const
{
    /* TODO */

    ARMusicalTag::printParameters(os);
}


