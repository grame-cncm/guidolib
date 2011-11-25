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
#include <cstring>
#include "ARCrescendo.h"


void ARCrescendo::print() const 
{
}

bool ARCrescendo::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;

	if ((getRange() == false ) && !strcmp("\\crescEnd",s))
		return true;
	return false;
}

void ARCrescendo::PrintName(std::ostream & os) const
{
	if (getRange())
		os << "\\cresc";
	else
		os << "\\crescBegin";
}

