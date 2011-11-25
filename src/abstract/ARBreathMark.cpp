/*
	GUIDO Library
	Copyright (C) 2004  Grame

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
#include "ARBreathMark.h"

// #include "TagParameterInt.h"
// #include "ListOfStrings.h"
#include "TagParameterList.h"

// ListOfTPLs ARBreathMark::ltpls(1);

ARBreathMark::ARBreathMark()
{
}

ARBreathMark::~ARBreathMark()
{

}

void ARBreathMark::setTagParameterList(TagParameterList & tpl)
{
/*	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( "" );
		CreateListOfTPLs(ltpls,lstrs);
	}
*/

	tpl.RemoveAll();
}

void ARBreathMark::PrintName( std::ostream & os ) const
{
	os << "\\breathMark";
}

/*
void ARBreathMark::PrintParameters(std::ostream & os) const
{
}*/

