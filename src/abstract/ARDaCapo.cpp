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
#include "ARDaCapo.h"
#include "TimeUnwrap.h"

ARDaCapo::ARDaCapo() : ARJump ("D.C.") {}
ARDaCapo::~ARDaCapo()   {}
void ARDaCapo::PrintName( std::ostream & os ) const  { os << "\\daCapo"; }

void ARDaCapo::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDaCapo);
}

ARDaCapoAlFine::ARDaCapoAlFine() : ARJump ("D.C. al Fine") {}
ARDaCapoAlFine::~ARDaCapoAlFine()   {}
void ARDaCapoAlFine::PrintName( std::ostream & os ) const  { os << "\\daCapoAlFine"; }

void ARDaCapoAlFine::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kDaCapoAlFine);
}
