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
#include "ARChordComma.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
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

void ARChordComma::print() const
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


