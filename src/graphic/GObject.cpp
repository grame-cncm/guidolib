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

#include "GObject.h"
#include "VGDevice.h"
#include "VGFont.h"

#include "GUIDOInternal.h"	// for gGlobalSettings
#include "FontManager.h"
#include "GuidoDefs.h"		// for LSPACE

#include <iostream>
using namespace std;

// - Static members
int	GObject::sInstanceCount = 0;
float GObject::sSymbolExtentMap [ kMaxMusicalSymbolID ];
NVPoint GObject::sRefposNone;

// ------------------------------------------------------------------------
GObject::GObject()
{
	mGId = (long) this;
	NotifyNewInstance();
}

// --------------------------------------------------------------------------
GObject::GObject( const GObject & in )
{
	mBoundingBox = in.mBoundingBox;
	mPosition = in.mPosition;
	mGId = (long) this;			// not in.mGId 
	NotifyNewInstance();
}

// --------------------------------------------------------------------------
GObject::~GObject()
{
	--sInstanceCount;
}

// --------------------------------------------------------------------------
int GObject::getFontSize() const
{
	return (int)(1.5f * LSPACE); // default?
}

// --------------------------------------------------------------------------
void GObject::setHPosition( float nx )
{
	// Should call setPosition(), or being kept distinct for optimisation ?
	
	mPosition.x = nx;

	// I don't know, wether this
	// should be called that way? 
}

// --------------------------------------------------------------------------
void GObject::setPosition( const NVPoint & inPos )
{
	mPosition = inPos;//  + offset;
}

// --------------------------------------------------------------------------
/** \brief Used with associations to tell associated elements the new position.
*/ 
void GObject::tellPosition(GObject *, const NVPoint &)
{
}

// --------------------------------------------------------------------------
void GObject::GGSOutput() const
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
	const float retval = sSymbolExtentMap[ inSymbol ];
	if (retval > -1)
		return retval;

	float extent = 0;
	float x = 0;
	float y = 0;
	if( gGlobalSettings.gDevice )
		FontManager::gFontScriab->GetExtent( inSymbol, &x, &y, gGlobalSettings.gDevice );
	
	extent = x;
	sSymbolExtentMap[ inSymbol ] = extent;
	return extent;
}

// --------------------------------------------------------------------------
void GObject::NotifyNewInstance()
{ 
 	if( ++ sInstanceCount == 1 )
	{	
		for( int index = 0; index < kMaxMusicalSymbolID; ++ index )
			sSymbolExtentMap[ index ] = -1;
	}
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





