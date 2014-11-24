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

void ARStaff::printName(std::ostream& os) const
{
    os << "ARStaff";
    ARMusicalTag::printName(os);
}

void ARStaff::printParameters(std::ostream& os) const
{
    if (idi)
        os << "id: " << idi->getValue() << ";";

    ARMusicalTag::printParameters(os);
}	
	

