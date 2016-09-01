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

#include "GUIDOInternal.h"

#include "VGDevice.h"
#include "SVGDevice.h"


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

    isSVGDevice = false;

    if (fDevice)
    {
        SVGDevice *device = dynamic_cast<SVGDevice *>(fDevice);
        if (device)
            isSVGDevice = true;    
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
    float propHeight = 0;

    if (fDevice)
	    propHeight = width * fDevice->GetHeight() / fDevice->GetWidth();

    return propHeight;
}

// --------------------------------------------------------------
float Bitmap::GetProportionalWidth (float height) const
{
    float propWidth = 0;

    if (fDevice)
	    propWidth = height * fDevice->GetWidth() / fDevice->GetHeight();

    return propWidth;
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
