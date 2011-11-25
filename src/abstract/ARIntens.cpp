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
#include "ARIntens.h"

#include "GRDefine.h"

#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "TimeUnwrap.h"

ListOfTPLs ARIntens::ltpls(1);

ARIntens::ARIntens(const char * txt, float val)
	: ARMTParameter(), value(val)
{
	mIntensText = txt;
}

void ARIntens::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "S,type,,r;F,value,-1.0,o"));
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

			TagParameterString * tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);

			mIntensText = tps->getValue();

			TagParameterFloat * tpf = TagParameterFloat::cast(rtpl->GetNext(pos));
			assert(tpf);

			if (tpf->pflag != TagParameter::NOTSET)
			{
				value = tpf->getValue();
			}
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

// --------------------------------------------------------------------------
void ARIntens::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kIntens);
}

void ARIntens::print() const
{
}

void ARIntens::PrintName(std::ostream &os) const
{
		os << "\\intens";
}

void ARIntens::PrintParameters(std::ostream &os) const
{
	os << "<\"" << mIntensText << "\"";
	if (value != -1.0)
	{
		os << "," << value * kVirtualToCm << "cm";
	}
	os << ">";
}



