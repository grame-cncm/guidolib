/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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


