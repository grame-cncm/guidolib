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

void ARTStem::printName(std::ostream& os) const
{
    os << "ARTStem";

	switch (mStemState) {
		case UP:
            os << " (stemsUp)";
            break;
		case DOWN:
            os << " (stemsDown)";
            break;
		case AUTO:
            os << " (stemsAuto)";
            break;
		case OFF:
            os << " (stemsOff)";
            break;
	}

    ARMusicalTag::printName(os);
}

void ARTStem::printParameters(std::ostream& os) const
{
    if (mTpfLength)
        os << "length: " << mTpfLength->getValue() << ";";

    ARMusicalTag::printParameters(os);
}
