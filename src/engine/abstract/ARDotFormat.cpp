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
#include "ARDotFormat.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARDotFormat::ltpls(1);

const ARDotFormat defaultARDotFormat(NULL,NULL);


ARDotFormat::ARDotFormat(ARDotFormat *p_savedf,ARDotFormat *copydf)
: ARMTParameter(-1,copydf)
{
	// this probably should be changed to RANGEDC
	// but then we have to take care of saving state-information ....
	rangesetting = ARMusicalTag::RANGEDC;
	savedf = p_savedf;
    
    if (!copydf) {
		TagParameterList tpl;
		setTagParameterList(tpl);
	}
}


ARDotFormat::~ARDotFormat()
{
}

void ARDotFormat::setTagParameterList(TagParameterList &tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("U,dx,0hs,o;U,dy,,o;F,size,1.0,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// this is different from the rest because we have a 
			// "generic" entry that governs DotFormats ...
			delete mDx;
			mDx = TagParameterFloat::cast(rtpl->RemoveHead()); 
			assert(mDx);

			delete mDy;
			mDy = TagParameterFloat::cast(rtpl->RemoveHead()); 
			assert(mDy);

			delete size;
			size = TagParameterFloat::cast(rtpl->RemoveHead()); 
			assert(size);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARDotFormat::printName(std::ostream& os) const
{
    os << "ARDotFormat";
    ARMusicalTag::printName(os);
}

void ARDotFormat::printParameters(std::ostream& os) const
{
    ARMusicalTag::printParameters(os);
}

