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

void ARColor::print() const
{
}
void ARColor::PrintName(std::ostream &os) const
{
	os << "\\color";
	
}
void ARColor::PrintParameters(std::ostream &os) const
{
	// TODO: add the alpha color component ?
	os << "<" << colorR << "," 
	   << colorG << "," << colorB << ">";
}

bool ARColor::IsStateTag() const
{
	return true;
}
