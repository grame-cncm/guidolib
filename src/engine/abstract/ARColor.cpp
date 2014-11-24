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

#include "ARColor.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARColor::ltpls(1);

void ARColor::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			
			// Was:
			// ( "F,red,0.0,r;F,green,0.0,r;F,blue,0.0,r" ));
			
			// (JB) proposal change: alpha component support (transparency): 
			( "F,red,0.0,r;F,green,0.0,r;F,blue,0.0,r;F,alpha,0.0,o"));
		
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
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

			TagParameterFloat * tpf;
			tpf = TagParameterFloat::cast( rtpl->GetNext(pos));
			assert(tpf);
			colorR = tpf->getValue();

			tpf = TagParameterFloat::cast( rtpl->GetNext(pos));
			assert(tpf);
			colorG = tpf->getValue();

			tpf = TagParameterFloat::cast( rtpl->GetNext(pos));
			assert(tpf);
			colorB = tpf->getValue();

			// - Optional Alpha component.
			tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
			if( tpf )
				colorA = tpf->getValue();
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

bool ARColor::IsStateTag() const
{
	return true;
}

void ARColor::printName(std::ostream& os) const
{
    os << "ARColor";
    ARMusicalTag::printName(os);
	
}
void ARColor::printParameters(std::ostream& os) const
{
    os << "red: "   << colorR << "; ";
    os << "green: " << colorG << "; ";
    os << "blue: "  << colorB << "; ";
    os << "alpha: " << colorA << "; ";

    ARMusicalTag::printParameters(os);
}
