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

ARCluster::ARCluster() : ARMTParameter(), adx(0), ady(0), ahdx(0), ahdy(0), aSize(1.0), aColor(NULL), aNoteCount(1),
    aOnlyOneNoteInCluster(false)
{
	rangesetting = ONLY;

    for(int i = 0; i <= 1; i++)
    {
        aFirstNote[i] = 0;
        aSecondNote[i] = 0;
    }
}

ARCluster::ARCluster(ARCluster *inCopyCluster) : ARMTParameter()
{
	rangesetting = ONLY;

    if (inCopyCluster)
    {
        adx = inCopyCluster->getadx();
        ady = inCopyCluster->getady();
        ahdx = inCopyCluster->getahdx();
        ahdy = inCopyCluster->getahdy();
        aSize = inCopyCluster->getSize();
        aColor = inCopyCluster->getColor();
        aNoteCount = inCopyCluster->getNoteCount();
        aOnlyOneNoteInCluster = inCopyCluster->getIsThereOnlyOneNoteInCluster();
        for(int i = 0; i <= 1; i++)
        {
            aFirstNote[i] = inCopyCluster->aFirstNote[i];
            aSecondNote[i] = inCopyCluster->aSecondNote[i];
        }
    }
    else
    {
        //Failure
    }
}

ARCluster::~ARCluster() 
{
}

void ARCluster::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,color,,o;F,size,1.0,o;U,hdx,0hs,o;U,hdy,0hs,o;U,dx,0hs,o;U,dy,0hs,o");
		CreateListOfTPLs(ltpls, lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
            aColor = TagParameterString::cast(rtpl->RemoveHead());

            TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
			aSize = f->getValue();
			delete f;

            // - dx/dy for cluster head only
			f = TagParameterFloat::cast(rtpl->RemoveHead());
			ahdx = f->getValue();
			delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
			ahdy = f->getValue();
			delete f;

            // - dx/dy for entire cluster
            f = TagParameterFloat::cast(rtpl->RemoveHead());
			adx = f->getValue();
			delete f;

            f = TagParameterFloat::cast(rtpl->RemoveHead());
			ady = f->getValue();
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

void ARCluster::PrintName(std::ostream & os) const
{
	os << "\\cluster";
}

void ARCluster::PrintParameters(std::ostream & os) const
{
	/*if (!num) return;
	
	os << "<i=\"" << num->getValue() << "\">";*/
}

void ARCluster::setNotePitchAndOctave(int inPitch, int inOctave)
{
    if (inPitch != 0)
    {
        if (aFirstNote[0] == 0)
        {
            aFirstNote[0] = inPitch;
            aFirstNote[1] = inOctave;
        }
        else if (aSecondNote[0] == 0)
        {
            aSecondNote[0] = inPitch;
            aSecondNote[1] = inOctave;

            if (!aOnlyOneNoteInCluster)
                aNoteCount++;
        }
        else
            aNoteCount++;
    }
}