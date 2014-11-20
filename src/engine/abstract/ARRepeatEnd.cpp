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

#include "ARRepeatEnd.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
#include "TimeUnwrap.h"

ListOfTPLs ARRepeatEnd::ltpls(1);

ARRepeatEnd::ARRepeatEnd(ARRepeatBegin * curbeg)
{ 
	repbeg = curbeg;
	numrepeat = 0; 
	rangesetting = RANGEDC;
}

void ARRepeatEnd::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( "I,num,-1,o" );
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
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

			TagParameterInt * tpi = TagParameterInt::cast(rtpl->GetNext(pos));
			assert(tpi);

			if (tpi->pflag != TagParameter::NOTSET)
			{
				numrepeat = tpi->getValue();
				
				if (numrepeat < 0)
					numrepeat = 0;			
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

void ARRepeatEnd::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kRepeatEnd);
}

std::ostream & ARRepeatEnd::operator<<(std::ostream & os) const
{
	os << "\\repeatEnd";
	if (numrepeat != 0)
	{
		os << "<" << numrepeat << ">";
	}
	if (getRange())
		os << "(";

	return os << " ";
}

void ARRepeatEnd::print(std::ostream& os) const
{    
    os << "ARRepeatEnd: numrepeat: " << numrepeat << ";";

    os << std::endl;
}