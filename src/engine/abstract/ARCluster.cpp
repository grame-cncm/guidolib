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
#include "ARNote.h"

#include "TagParameterFloat.h"
#include "TagParameterStrings.h"

using namespace std;

static const TagParameterMap sARClusterMap (kARClusterParams);

ARCluster::ARCluster() : ARMTParameter(), fHdx(0), fHdy(0), fNoteCount(1), fSingleNote(false)
{
	setupTagParameters(sARClusterMap);
	rangesetting = ONLY;

    fFirstNote = NULL;
    fSecondNote = NULL;
}

ARCluster::ARCluster(const ARCluster *inCopyCluster) : ARMTParameter(-1, inCopyCluster)
{
	setupTagParameters (sARClusterMap);
	rangesetting = ONLY;

    if (inCopyCluster) {
		copyParameters(inCopyCluster->getTagParameters());

        fHdx = inCopyCluster->getahdx();
        fHdy = inCopyCluster->getahdy();
        fNoteCount = inCopyCluster->getNoteCount();
        fSingleNote = inCopyCluster->getIsThereOnlyOneNoteInCluster();

        fFirstNote = inCopyCluster->getFirstNote();
        fSecondNote = inCopyCluster->getSecondNote();
        setVoiceNum(inCopyCluster->getVoiceNum());
    }
}

void ARCluster::setTagParameters (const TagParameterMap& params)
{
	const TagParameterFloat *f = getParameter<TagParameterFloat>(kHdxStr, true);
	if (f) fHdx = f->getValue();
	f = getParameter<TagParameterFloat>(kHdyStr, true);
	if (f) fHdy = f->getValue();
}

void ARCluster::setARNote(ARNote *arNote)
{
    if (fFirstNote == NULL)
        fFirstNote = arNote;
    else if (fSecondNote == NULL) {
        fSecondNote = arNote;
        if (!fSingleNote)
            fNoteCount++;
    }
    else fNoteCount++;
}

