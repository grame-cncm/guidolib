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
				: ARMTParameter(-1, copyoct), saveoct(p_saveoct)
{
	rangesetting = RANGEDC;
	noct = NULL;
	if (copyoct && copyoct->getTPOctava())
	{
		noct = TagParameterInt::cast(copyoct->getTPOctava()->getCopy());
	}
}

AROctava::~AROctava()
{
	delete noct;
}

int AROctava::getOctava() const
{
	return noct ? noct->getValue() : 0;
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
			noct = TagParameterInt::cast(rtpl->RemoveHead());
			assert(noct);

		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();

}

void AROctava::PrintName(std::ostream & os) const
{
	os << "\\oct";
}

void AROctava::PrintParameters(std::ostream & os) const
{
	int value = 0;
	if (noct)
		value = noct->getValue();
	os << "<i=" << value << ">";
}

void AROctava::print(std::ostream& os) const
{
    os << "AROctava: ";

    if (noct)
        os << "i: " << noct->getValue() << "; ";

    os << std::endl;
}
