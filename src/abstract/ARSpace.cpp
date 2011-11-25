/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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


