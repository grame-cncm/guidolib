/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "SVGSystem.h"
#include "SVGDevice.h"
#include "SVGFont.h"

using namespace std;
//______________________________________________________________________________
VGDevice* SVGSystem::CreateDisplayDevice(ostream& outstream, const char* fontfile)
{
	return new SVGDevice (outstream, this, fontfile);
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateDisplayDevice()
{
	return new SVGDevice (cout, this, 0);
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateDisplayDevice(const char* fontfile)
{
	return new SVGDevice (cout, this, fontfile);
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateMemoryDevice( int w, int h)
{
	SVGDevice* device = new SVGDevice (cout, this, 0);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateMemoryDevice( int w, int h, const char* fontfile )
{
	SVGDevice* device = new SVGDevice (cout, this, fontfile);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreateMemoryDevice(const char * inPath)
{
    SVGDevice* device = new SVGDevice (cout, this, 0);
	return device;
}

//______________________________________________________________________________
VGDevice* SVGSystem::CreatePrinterDevice( )							{ return 0; }

//______________________________________________________________________________
VGDevice* SVGSystem::CreateAntiAliasedMemoryDevice( int w, int h )
{
	SVGDevice* device = new SVGDevice (cout, this);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
const VGFont* SVGSystem::CreateVGFont( const char * faceName, int size, int properties ) const
{
	return new SVGFont (faceName, size, properties);
}

