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

#include "ARSpace.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
//#include "../parser/gmntools.h"
#include "GuidoDefs.h"

ListOfTPLs ARSpace::ltpls(1);

void ARSpace::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( "U,dd,,r"));
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
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
			assert(tpf);

			val = tpf->getValue();
		}
		delete rtpl;

	}
	else
	{
		// failure.
	}
	tpl.RemoveAll();
	
}

void ARSpace::PrintName(std::ostream  &os) const
{
	os << "\\space";
}

void ARSpace::PrintParameters(std::ostream & os) const
{
	os << "<dd=" << val * kVirtualToCm << "cm> ";
}


