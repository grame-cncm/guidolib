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
#include "ARChordComma.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "TimeUnwrap.h"
#include "ListOfStrings.h"

ListOfTPLs ARChordComma::ltpls(1);


ARChordComma::ARChordComma(const ARChordComma * cc)
				: ARMTParameter(-1,cc)
{
}

ARChordComma::ARChordComma(): ARMTParameter()
{
}   

ARChordComma::~ARChordComma()
{
}

ARMusicalObject * ARChordComma::Copy() const
{
	return new ARChordComma(this);
}

void ARChordComma::print(int &indent) const
{
	std::cout << "ARChordComma" << '\n';
}

void ARChordComma::PrintName(std::ostream & os) const
{
	os << "\\chordComma";
}

void ARChordComma::PrintParameters(std::ostream & ) const
{
}


void ARChordComma::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kChordComma);
}


void ARChordComma::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}


