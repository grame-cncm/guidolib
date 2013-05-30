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

#include "ARSymbol.h"

#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

ListOfTPLs ARSymbol::ltpls(1);

ARSymbol::ARSymbol() : ARMTParameter(), aSize(1)
{
	relativeTimePosition = TYPE_TIMEPOSITION(-1,1);
	duration = DURATION_0;
    aPosition = NULL;
	aFilePath = NULL;
    aBaseFilePath = NULL;
    rangesetting = RANGEDC;
}

ARSymbol::~ARSymbol() 
{
	delete aFilePath;
};

const char *ARSymbol::getTagFormat() const
{
	const char * const outFormat = "S,filePath,,r;F,size,1.0,o;S,position,mid,o";
	return outFormat;
}

void ARSymbol::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs;
		lstrs.AddTail(getTagFormat());	
        ARMusicalTag::CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >= 0 && rtpl)
	{
		if (ret == 0)
		{
			delete aFilePath;
			aFilePath = TagParameterString::cast(rtpl->RemoveHead());
			assert(aFilePath);

            TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
			aSize = f->getValue();
            delete f;

            delete aPosition;
			aPosition = TagParameterString::cast(rtpl->RemoveHead());
			assert(aPosition);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARSymbol::print() const
{
}

void ARSymbol::PrintName(std::ostream &os) const
{
	os << "\\symbol";
}

void ARSymbol::PrintParameters(std::ostream &os) const
{
    if (aFilePath || mDx || mDy)
    {
        os << "<\"";
        if (aFilePath)
        {
            os << aFilePath->getValue();
            if (mDx || mDy)
                os << "\",";
        }

        if (mDx)
        {
            os << mDx->getUnitValue() << mDx->getUnit();
            if (mDy)
                os << "\",";
        }
        if (mDy)
        {
            os << mDy->getUnitValue() << mDy->getUnit();
        }
        os << ">";
    }
}

void ARSymbol::setBaseFilePath(NVstring inBaseFilePath)
{
    aBaseFilePath = inBaseFilePath;
}