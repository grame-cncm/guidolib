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

#include "TagParameterFloat.h"
#include "TagParameterString.h"


// ===========================================================================
// 		GRSpecial
// ===========================================================================
GRSpecial::GRSpecial(const ARSpecial * arspec, bool p_ownsar)
					: GRARNotationElement(arspec, p_ownsar)
{
	mNeedsSpring = 1;		// ????
}

void GRSpecial::OnDraw(VGDevice & hdc) const
{
	if(!mDraw || !mShow)
		return;
	const ARSpecial * ar = static_cast/*dynamic cast*/<const ARSpecial *>(mAbstractRepresentation);
	
	// try to get the correctly scaled musical font ...
	// TODO: scale the graphic context instead.
    const int fontSize = (int)(((ar->getSize() ? ar->getSize()->getValue() : 1) * 4 * LSPACE) + float(0.5)); // +0.5 to round from float to int.
	const VGFont* myfont = FontManager::FindOrCreateFont( fontSize );
	const VGFont* savedFont = hdc.GetMusicFont();
	hdc.SetMusicFont( myfont );
	hdc.SetFontAlign( VGDevice::kAlignLeft | VGDevice::kAlignBase );

	const unsigned int theSymbol = ar->getDrawChar();

    float specialXOffset = (ar->getDX() ? ar->getDX()->getValue() : 0);
    float specialYOffset = (ar->getDY() ? ar->getDY()->getValue() : 0);
    
    const TagParameterString *tps = ar->getColor();

    const VGColor prevTextColor = hdc.GetFontColor();
    if (tps) {
		unsigned char colRef[4];
        tps->getRGB(colRef);
        hdc.SetFontColor(VGColor(colRef));
    }
	hdc.DrawMusicSymbol((mPosition.x + specialXOffset), (mPosition.y + specialYOffset), theSymbol);
    if (tps) hdc.SetFontColor(prevTextColor);
	hdc.SetMusicFont( savedFont );
}

