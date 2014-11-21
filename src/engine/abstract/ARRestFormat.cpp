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
#include "ARRestFormat.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARRestFormat::ltpls(1);

ARRestFormat::ARRestFormat(ARRestFormat *p_saverf, ARRestFormat *copyrf)
	: ARMTParameter(-1, copyrf)
{
	rangesetting = ARMusicalTag::RANGEDC;
	saverf       = p_saverf;
}


ARRestFormat::~ARRestFormat()
{
}

void ARRestFormat::setTagParameterList(TagParameterList &tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail((""));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >= 0 && rtpl)
    {
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARRestFormat::PrintName(std::ostream & os) const
{
	os << "\\restFormat";
}

void ARRestFormat::PrintParameters(std::ostream &os) const
{
}

void ARRestFormat::print(std::ostream& os) const
{
    os << "ARRestFormat;" << std::endl;
}

