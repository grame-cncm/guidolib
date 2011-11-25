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
#include "ARTStem.h"
#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "ListOfStrings.h"

ListOfTPLs ARTStem::ltpls(1);

ARTStem::ARTStem(int st , ARTStem * p_savestem, ARTStem * p_copystem ) 
									: ARMTParameter(-1,p_copystem)
{
	rangesetting = RANGEDC;
	mStemState = (ARTStem::STEMSTATE) st;
	mSaveStem = p_savestem;
	mTpfLength = NULL;

	if (p_copystem)
	{
		mStemState = p_copystem->getStemState();
		if (p_copystem->getLength())
			mTpfLength = TagParameterFloat::cast(
			p_copystem->getLength()->getCopy());
	}
}


ARTStem::ARTStem(const ARTStem * tstem) : ARMTParameter(-1,tstem)
{
	mSaveStem = NULL;
	mStemState = tstem->mStemState;
	mTpfLength = NULL;
	if (tstem->mTpfLength)
	{
		mTpfLength = TagParameterFloat::cast
						(tstem->mTpfLength->getCopy());
	}
}

ARTStem::~ARTStem()
{
	delete mTpfLength;
}

ARMusicalObject * ARTStem::Copy() const
{
	return new ARTStem(this);
}

void ARTStem::PrintName(std::ostream & os) const
{
	switch (mStemState)
	{
		case UP: 	os << "\\stemsUp";		break;
		case DOWN: 	os << "\\stemsDown";	break;
		case AUTO: 	os << "\\stemsAuto";	break;
		case OFF: 	os << "\\stemsOff";		break;
	}
}

void ARTStem::PrintParameters(std::ostream & os) const
{
	if (mTpfLength && mTpfLength->TagIsSet())
	{
		os << "<" << mTpfLength->getUnitValue() <<
			mTpfLength->getUnit() << ">";
	}
}

void ARTStem::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "U,length,7.0,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	const int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb
			//GuidoPos pos = rtpl->GetHeadPosition();

			mTpfLength = TagParameterFloat::cast( rtpl->RemoveHead());
		}
		delete rtpl;
	}
	else
	{
		// failure...
	}

	tpl.RemoveAll();
}
