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
#include <ctype.h>

#include "ARStaff.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARStaff::ltpls(1);

ARStaff::ARStaff(const TYPE_TIMEPOSITION & timeposition)
	: ARMTParameter(timeposition)
{
	idi = NULL;
	ids = NULL;
}

ARStaff::ARStaff(int p_staffnumber)
{
	idi = new TagParameterInt(p_staffnumber);
	idi->setIsAuto();
	ids = NULL;
}

ARStaff::ARStaff()
{
	idi = NULL;
	ids = NULL;
}

ARStaff::ARStaff(const ARStaff *stff)
		: ARMTParameter(-1,stff)
{
	const ARStaff &stf = *stff;
	if (stf.idi)
		idi = new TagParameterInt(stf.idi->getValue());
	else
		idi = NULL;

	if (stf.ids)
		ids = new TagParameterString(stf.ids->getValue());
	else 
		ids = NULL;
}

ARStaff::~ARStaff() 
{
	delete idi;
	delete ids;
}

void ARStaff::print() const
{
}

void ARStaff::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ("I,id,,r;U,dy,,o"));
		lstrs.AddTail( ("S,id,,r;U,dy,,o"));
		CreateListOfTPLs(ltpls,lstrs);

	}
	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			idi = TagParameterInt::cast(rtpl->RemoveHead());
			assert(idi);

			if (idi->getValue() < 1)
			{
				GuidoWarn("Staff-Parameter-Value smaller than 1");
				idi->setValue(1);
			}
			mDy = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(mDy);
		}
		else if (ret==1)
		{
			// we have a match with the second parameter ...
			ids = TagParameterString::cast(rtpl->RemoveHead());
			assert(ids);

			mDy = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(mDy);
		}
		delete rtpl;
	}
	else
	{
		// failure ...
	}
	if (idi && idi->getValue() < 0)
		idi->setValue(0);
	tpl.RemoveAll();
}

bool ARStaff::IsStateTag() const
{
	return true;
}

ARMusicalObject * ARStaff::Copy() const
{
	return new ARStaff(this);
}

const char* ARStaff::getStaffID() const
{
	return ids ? ids->getValue() : 0;
}

/** \brief Returns the number if ID, returns -1 if string
*/
int ARStaff::getStaffNumber() const
{ 
	if (idi)		return idi->getValue();
 	else if (ids)	return -1;
  	return 0;
}

void ARStaff::PrintName(std::ostream & os) const
{
	os << "\\staff";
}

void ARStaff::PrintParameters(std::ostream & os) const
{
	if (idi && (idi->TagIsSet() || idi->TagIsSetByAuto()))
		os << "<" << idi->getValue() << ">";
	if (ids && ids->TagIsSet())
		os << "<\"" << ids->getValue() << "\">";
}	
	

