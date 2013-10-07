/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "Bitmap.h"
#include "NVPoint.h"
#include "NVRect.h"

#include "VGDevice.h"
#include "GUIDOInternal.h"		// for gGlobalSettings.gFeedback


// --------------------------------------------------------------
Bitmap::Bitmap( const char * inName )
{
    VGDevice *tmpDevice = gGlobalSettings.gDevice;

    if (tmpDevice)
    {
        VGSystem *tmpSystem = tmpDevice->getVGSystem();

        if (tmpSystem)
            fDevice = tmpSystem->CreateMemoryDevice(inName);
    }
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
    if (fDevice)
    {
        hdc.SetRasterOpMode(VGDevice::kOpAnd);

        const int width = int(where.right - where.left); 
        const int height = int(where.bottom - where.top);
        
        hdc.CopyPixels(int(where.left), int(where.top), width, height, fDevice);	
    }
}
