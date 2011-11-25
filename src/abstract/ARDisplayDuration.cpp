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
#include <string.h>

#include "ARDisplayDuration.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARDisplayDuration::ltpls(1);

ARDisplayDuration::ARDisplayDuration(const ARDisplayDuration * dspdur)
						: ARMTParameter(-1,dspdur)
{
	num = denom = ndots = 0;
	
	if (dspdur->num)
		num = TagParameterInt::cast(dspdur->num->getCopy());

	if (dspdur->denom)
		denom = TagParameterInt::cast(dspdur->denom->getCopy());

	if (dspdur->ndots)
		ndots = TagParameterInt::cast(dspdur->ndots->getCopy());
}

ARDisplayDuration::~ARDisplayDuration()
{
	delete num;
	delete denom;
	delete ndots;
}

void ARDisplayDuration::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			( "I,n,,r;I,d,,r;I,ndots,0,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// GuidoPos pos = rtpl->GetHeadPosition();

			num = TagParameterInt::cast(rtpl->RemoveHead());
			assert(num);

			denom = TagParameterInt::cast(rtpl->RemoveHead());
			assert(denom);

			dur.setNumerator(num->getValue());
			dur.setDenominator(denom->getValue());

			ndots = TagParameterInt::cast(rtpl->RemoveHead());
			assert(ndots);

			dots = ndots->getValue();
		}

		delete rtpl;
	}
		
	
	tpl.RemoveAll();
}

ARMusicalObject *ARDisplayDuration::Copy() const
{
	return new ARDisplayDuration(this);
}

bool ARDisplayDuration::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;

	if (!getRange() && !strcmp("\\dispDurEnd",s))
		return 1;
	return 0;
}

void ARDisplayDuration::PrintName(std::ostream & os) const
{
	os << "\\dispDur";
	if (!getRange())
		os << "Begin";
}

void ARDisplayDuration::PrintParameters(std::ostream & os) const
{

	os << "<" << dur.getNumerator() <<
		"," << dur.getDenominator() <<
		"," << dots << ">";
}

void ARDisplayDuration::setDisplayDuration(const TYPE_DURATION & tmp)
{
	dots = 0;
	TYPE_DURATION mydur (tmp);
	mydur.normalize();
	if (mydur.getNumerator() == 1)
	{
		dots = 0;
	}
	else if (mydur.getNumerator() == 3
		&& mydur.getDenominator() > 1)
	{
		dots = 1;
		mydur.setNumerator(1);
		int denom = mydur.getDenominator();
		mydur.setDenominator(denom/2);
	}	
	else if (mydur.getNumerator() == 7
		&& mydur.getDenominator() > 1)
	{
		dots = 2;
		mydur.setNumerator(1);
		int denom = mydur.getDenominator();
		mydur.setDenominator(denom/4);
	}
	else
	{
		// ASSERT(FALSE);
		//TRACE("dispduration of unknown length");
	}
	dur = mydur;		
}
