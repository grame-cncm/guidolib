/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
	if(!mDraw)
		return;
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

void GRSpecial::print(std::ostream& os) const
{
}
