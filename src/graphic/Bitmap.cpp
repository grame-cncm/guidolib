/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003, 2006  Grame

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

#include "Bitmap.h"
#include "NVPoint.h"
#include "NVRect.h"

#include "VGDevice.h"
#include "GUIDOInternal.h"		// for gGlobalSettings.gFeedback


// --------------------------------------------------------------
Bitmap::Bitmap( const char * inName )
{
	if( gGlobalSettings.gDevice )
		fDevice = gGlobalSettings.gDevice->getVGSystem()->CreateMemoryDevice (inName);
}

// --------------------------------------------------------------
Bitmap::~Bitmap() 
{ 
	delete fDevice; 
}


// --------------------------------------------------------------
float Bitmap::GetProportionalHeight (float width) const
{
	return width * fDevice->GetHeight() / fDevice->GetWidth();
}

// --------------------------------------------------------------
float Bitmap::GetProportionalWidth (float height) const
{
	return height * fDevice->GetWidth() / fDevice->GetHeight();
}

// --------------------------------------------------------------
void Bitmap::OnDrawW( VGDevice & hdc, const NVPoint &where, float width ) const
{
	const float height = GetProportionalHeight(width);
	hdc.SetRasterOpMode( VGDevice::kOpAnd );	
	hdc.CopyPixels( int(where.x), int(where.y), int(width), int(height), fDevice );	
 }

// --------------------------------------------------------------
void Bitmap::OnDrawH( VGDevice & hdc, const NVPoint &where, float height ) const
{
 	if (!fDevice) return;
	const float width = GetProportionalWidth (height);
	hdc.SetRasterOpMode( VGDevice::kOpAnd );	
	hdc.CopyPixels( int(where.x), int(where.y), int(width), int(height), fDevice );	
}

// --------------------------------------------------------------
void Bitmap::OnDraw( VGDevice & hdc, const NVRect & where ) const
{
 	if (!fDevice) return;
	hdc.SetRasterOpMode( VGDevice::kOpAnd );	
	const int width = int(where.right - where.left); 
	const int height = int(where.bottom - where.top);
	hdc.CopyPixels( int(where.left), int(where.top), width, height, fDevice );	
}
