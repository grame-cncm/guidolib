/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GObject.h"
#include "VGDevice.h"
#include "VGFont.h"

#include "GUIDOInternal.h"	// for gGlobalSettings
#include "FontManager.h"
#include "GuidoDefs.h"		// for LSPACE

#include <iostream>
using namespace std;

// - Static members
//int	GObject::sInstanceCount = 0;
std::map<ConstMusicalSymbolID, float> GObject::sSymbolExtentMap;
NVPoint GObject::sRefposNone;


// --------------------------------------------------------------------------
GObject::GObject( const GObject & in )
{
	mBoundingBox = in.mBoundingBox;
	mPosition = in.mPosition;
}

// --------------------------------------------------------------------------
/** \brief Used with associations to tell associated elements the new position.
*/ 
void GObject::tellPosition(GObject *, const NVPoint &)
{
}

// --------------------------------------------------------------------------
/** \brief Returns the width of a musical symbol.

	When GetSymbolExtent() is called for the first time for a given symbol, 
	it may extract the required width from a musical font. Before beeing 
	returned, the result is stored in a cache table to make next calls faster.

	TODO: also return the height of the symbols. (the width is not enough 
	to calculate a bounding box)
*/
float GObject::GetSymbolExtent( unsigned int inSymbol )
{
	float val = sSymbolExtentMap[ inSymbol ];
	if (val) return val;

	float x = 0;
	float y = 0;
	if( gGlobalSettings.gDevice )
		FontManager::gFontScriab->GetExtent( inSymbol, &x, &y, gGlobalSettings.gDevice );
	
	sSymbolExtentMap[ inSymbol  ] = x;
	return x;
}

// --------------------------------------------------------------------------
bool GObject::positionIsOnStaffLine( float inPositionY, float inLSpace )
{
	// determine, wether the note is on a line or not ...
	const int n = (int)( inPositionY / ( inLSpace * 0.5f ));

	//if ( (n / 2) * 2 == n ) // if even ?
	return (( n | 1 ) != n ); // if even
	
}

// --------------------------------------------------------------------------
/** \brief For debugging purpose only
*/
void GObject::DrawBoundingBox( VGDevice & hdc, const VGColor & inBrushColor ) const
{
	hdc.PushPen( inBrushColor, 4);
	NVRect r(mBoundingBox);
	r += mPosition;
	r += getOffset();
    hdc.Line (r.left, r.top, r.right, r.top);
    hdc.Line (r.right, r.top, r.right, r.bottom);
    hdc.Line (r.right, r.bottom, r.left, r.bottom);
    hdc.Line (r.left, r.bottom, r.left, r.top);
	hdc.PopPen();
}





