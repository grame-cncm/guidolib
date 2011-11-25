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
#include "TagParameterList.h"
#include "ARCrescEnd.h"


// Parameters for crescBegin (as spec tells us ...)
// ... 
void ARCrescEnd::setTagParameterList(TagParameterList & tpl)
{
	// the end-tags do not have Parameters!
	tpl.RemoveAll();
}

void ARCrescEnd::PrintName(std::ostream & os) const
{ 
	os << " //crescEnd"; 
}

