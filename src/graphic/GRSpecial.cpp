/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2004	Grame

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

#include <iostream>	// debug
using namespace std;

#include "GuidoDefs.h"		// for LSPACE
#include "ARSpecial.h"
#include "VGFont.h"
#include "FontManager.h"
#include "GRSpecial.h"


// ===========================================================================
// 		GRSpecial
// ===========================================================================
GRSpecial::GRSpecial(ARSpecial * arspec, bool p_ownsar)
					: GRARNotationElement(arspec, p_ownsar)
{
	mNeedsSpring = 1;		// ????
}

void GRSpecial::OnDraw(VGDevice & hdc) const
{
	const ARSpecial * ar = static_cast/*dynamic cast*/<ARSpecial *>(mAbstractRepresentation);
	
	// try to get the correctly scaled musical font ...
	// TODO: scale the graphic context instead.
	const int fontSize = (int)((ar->scale * 4 * LSPACE) + float(0.5));	// +0.5 to round from float to int.
	const VGFont* myfont = FontManager::FindOrCreateFont( fontSize );
	hdc.SetMusicFont( myfont );
	hdc.SetFontAlign( VGDevice::kAlignLeft | VGDevice::kAlignBase );

	const unsigned int theSymbol = ar->mDrawChar;
	hdc.DrawMusicSymbol( (mPosition.x + ar->dx), (mPosition.y + ar->dy), theSymbol );
}
