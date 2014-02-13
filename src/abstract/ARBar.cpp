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
// #include "ARFactory.h"
#include "ARBar.h"
#include "ARMusicalTag.h"

#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARBar::ltpls(1);

ARBar::ARBar(const TYPE_TIMEPOSITION &timeposition)
	: ARMTParameter(timeposition)
{
	barnumber = -1; // not specified
	measureNumber = 0;
}


ARBar::ARBar() : ARMTParameter()
{
	barnumber = -1; // not specified
	measureNumber = 0;
}   

ARBar::~ARBar() // does nothing
{
}

void ARBar::print() const
{
}

void ARBar::PrintName(std::ostream &os) const
{
	os << "\\bar";
}

void ARBar::PrintParameters(std::ostream &os) const
{
	if (barnumber!=-1)
	{
		os << "<" << barnumber << ">";
	}
}

void ARBar::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"I,number,-1,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterInt * tpi =  TagParameterInt::cast(rtpl->GetNext(pos));
			assert(tpi);

			if (tpi->pflag != TagParameter::NOTSET)
			{
				barnumber = tpi->getValue();
			}
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();

  }


