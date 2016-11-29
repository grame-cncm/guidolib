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
#include "AROctava.h"
// #include "ARFactory.h"
#include "TagParameterInt.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"

ListOfTPLs AROctava::ltpls(1);

AROctava::AROctava(AROctava * p_saveoct, AROctava * copyoct)
				: ARMTParameter(-1, copyoct), fSaveoct(p_saveoct)
{
	rangesetting = RANGEDC;
	fOctave = NULL;
	if (copyoct && copyoct->getTPOctava())
	{
		fOctave = TagParameterInt::cast(copyoct->getTPOctava()->getCopy());
	}
}

AROctava::~AROctava()
{
	delete fOctave;
}

int AROctava::getOctava() const
{
	return fOctave ? fOctave->getValue() : 0;
}

void AROctava::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( "I,i,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			fOctave = TagParameterInt::cast(rtpl->RemoveHead());
			assert(fOctave);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void AROctava::printName(std::ostream& os) const
{
    os << "AROctava";
}

void AROctava::printGMNName(std::ostream& os) const
{
    os << "\\octava";
}

void AROctava::printParameters(std::ostream& os) const
{
    if (fOctave)
        os << "i: " << fOctave->getValue() << "; ";

    ARMusicalTag::printParameters(os);
}
