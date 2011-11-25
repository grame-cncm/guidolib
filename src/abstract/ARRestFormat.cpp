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
#include "ARRestFormat.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARRestFormat::ltpls(1);

ARRestFormat::ARRestFormat(ARRestFormat *p_saverf, ARRestFormat *copyrf)
	: ARMTParameter(-1,copyrf)
{
	rangesetting = ARMusicalTag::RANGEDC;

	posy = NULL;

	saverf = p_saverf;

	if (copyrf && copyrf->getPosy())
	{
		posy = TagParameterFloat::cast(copyrf->getPosy()->getCopy());
	}
}


ARRestFormat::~ARRestFormat()
{
	delete posy;
}

void ARRestFormat::setTagParameterList(TagParameterList &tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( "U,posy,0,o"));
		CreateListOfTPLs(ltpls,lstrs);

	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	posy = NULL;
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			posy = TagParameterFloat::cast(rtpl->RemoveHead()); 
		}

		delete rtpl;
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
	// todo 
	if (posy)
		os << "<posy=\"" << posy->getUnitValue() << posy->getUnit() << "> ";
//		\",size=" 
//		<< size << ",dx=" << dx << "> ";
}

