/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string.h>

#include "SVGSystem.h"
#include "SVGDevice.h"
#include "SVGFont.h"
#include "SVGMapDevice.h"
#include "GUIDOEngine.h"
#include "GuidoFont.h"
#include "TimesFont.h"

using namespace std;
//______________________________________________________________________________
SVGSystem::SVGSystem(const char * font) : fGuidoFont(font)
{
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateDisplayDevice(ostream& outstream, int mappingMode)
{
	if (mappingMode != kNoMapping)
		return new SVGMapDevice(outstream, this, fGuidoFont, mappingMode); // Maps need to be drawn
	else
		return new SVGDevice(outstream, this, fGuidoFont);
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateDisplayDevice()
{
	return new SVGDevice (cout, this, fGuidoFont);
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateMemoryDevice( int w, int h)
{
	SVGDevice* device = new SVGDevice (cout, this, fGuidoFont);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateMemoryDevice(const char * inPath)
{
	SVGDevice* device = new SVGDevice (cout, this, fGuidoFont);
	return device;
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreatePrinterDevice( )							{ return 0; }

//______________________________________________________________________________
VGDevice* SVGSystem::CreateAntiAliasedMemoryDevice( int w, int h )
{
	SVGDevice* device = new SVGDevice (cout, this, 0);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
const VGFont* SVGSystem::CreateVGFont( const char * faceName, int size, int properties ) const
{
#ifdef INDEPENDENTSVG
	// This system try to use embed font metrics
	if(strcmp (kMusicFontStr, faceName) == 0) {
		return new GuidoFont(faceName, size, properties);
	} else {
		//if(strcmp("Times", faceName) == 0) {
		// TODO GGX Add Arial font and bold / italic style ?
			return new TimesFont(faceName, size, properties);
		//} else return NULL;
	}
#endif
	return new SVGFont (faceName, size, properties);
}

