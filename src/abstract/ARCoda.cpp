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

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include <iostream>
#include "ARCoda.h"
#include "TimeMapper.h"

ARCoda::ARCoda() : ARJump ("[coda] Coda") {}
ARCoda::~ARCoda()   {}
void ARCoda::PrintName( std::ostream & os ) const  { os << "\\coda"; }

void ARCoda::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kCoda);
}
