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
#include "ARSystemFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARSystemFormat::ltpls(1);

ARSystemFormat::ARSystemFormat()
{
}

ARSystemFormat::~ARSystemFormat() 
{
}

void ARSystemFormat::setTagParameterList(TagParameterList &tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("U,dx,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb
			// GuidoPos pos = rtpl->GetHeadPosition();

			mDx = TagParameterFloat::cast(rtpl->RemoveHead());
		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARSystemFormat::PrintName(std::ostream & os) const
{
	os << "\\systemFormat";
}

void ARSystemFormat::PrintParameters(std::ostream & ) const
{
	// todo !
}

void ARSystemFormat::print(std::ostream& os) const
{
    os << "ARSystemFormat: ";

    if (mDx)
        os << "dx: " << mDx->getValue() << ";";

     os << std::endl;
}