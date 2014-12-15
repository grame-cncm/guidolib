/*
  GUIDO Library
  Copyright (C) 2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARCluster.h"

#include "TagParameterList.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"

using namespace std;

ListOfTPLs ARCluster::ltpls(1);

ARCluster::ARCluster() : ARMTParameter(), hdx(0), hdy(0), noteCount(1),
    onlyOneNoteInCluster(false)
{
	rangesetting = ONLY;

    firstNote = NULL;
    secondNote = NULL;
}

ARCluster::ARCluster(const ARCluster *inCopyCluster) : ARMTParameter(-1, inCopyCluster)
{
	rangesetting = ONLY;

    if (inCopyCluster) {
        hdx = inCopyCluster->getahdx();
        hdy = inCopyCluster->getahdy();
        noteCount = inCopyCluster->getNoteCount();
        onlyOneNoteInCluster = inCopyCluster->getIsThereOnlyOneNoteInCluster();

        firstNote = inCopyCluster->getFirstNote();
        secondNote = inCopyCluster->getSecondNote();

        setVoiceNum(inCopyCluster->getVoiceNum());
    }
}

ARCluster::~ARCluster() 
{
}

void ARCluster::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0) {
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("U,hdx,0hs,o;U,hdy,0hs,o");
		CreateListOfTPLs(ltpls, lstrs);
	}

	TagParameterList *rtpl = NULL;
 	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
            // - dx/dy for cluster head only
			TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
			hdx = f->getValue();
			delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
			hdy = f->getValue();
			delete f;
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARCluster::setARNote(ARNote *arNote)
{
    if (firstNote == NULL)
        firstNote = arNote;
    else if (secondNote == NULL) {
        secondNote = arNote;

        if (!onlyOneNoteInCluster)
            noteCount++;
    }
    else
        noteCount++;
}

void ARCluster::printName(std::ostream& os) const
{
    os << "ARCluster";
}

void ARCluster::printGMNName(std::ostream& os) const
{
    os << "\\cluster";
}

void ARCluster::printParameters(std::ostream& os) const
{
    os << "hdx: " << hdx << "; ";
    os << "hdy: " << hdy << "; ";

    ARMusicalTag::printParameters(os);
}