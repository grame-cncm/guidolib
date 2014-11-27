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
#include "TagParameterInt.h"
#include "ListOfStrings.h"

ListOfTPLs ARSymbol::ltpls(1);

ARSymbol::ARSymbol() : ARMTParameter(), size(1)
{
    rangesetting = RANGEDC;
	relativeTimePosition = TYPE_TIMEPOSITION(-1, 1);
	setDuration( DURATION_0 );

    position       = 0;
    filePath       = 0;
    width          = 0;
    height         = 0;
    currentARMusic = 0;
}

ARSymbol::~ARSymbol() 
{
	delete filePath;
    delete position;
    delete width;
    delete height;
};

const char *ARSymbol::getTagFormat() const
{
	const char * const outFormat = "S,file,,r;F,size,1.0,o;S,position,mid,o;I,w,,o;I,h,,o";
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
			delete filePath;
			filePath = TagParameterString::cast(rtpl->RemoveHead());
			assert(filePath);

            TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
			size = f->getValue();
            delete f;

            delete position;
			position = TagParameterString::cast(rtpl->RemoveHead());
			assert(position);

            delete width;
            width = TagParameterInt::cast(rtpl->RemoveHead());
            assert(width);

            delete height;
            height = TagParameterInt::cast(rtpl->RemoveHead());
            assert(height);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARSymbol::printName(std::ostream& os) const
{
    os << "ARSymbol";
}

void ARSymbol::printGMNName(std::ostream& os) const
{
    os << "\\symbol";
}

void ARSymbol::printParameters(std::ostream& os) const
{
    if (filePath)
        os << "file: \"" << filePath->getValue() << "\"; ";

    if (position)
        os << "position: " << position->getValue() << "; ";

    if (width)
        os << "w: " << width->getValue() << "; ";

    if (height)
        os << "h: " << height->getValue() << "; ";

    ARMusicalTag::printParameters(os);
}