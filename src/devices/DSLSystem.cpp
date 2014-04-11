/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "DSLSystem.h"
#include "DSLDevice.h"
#include "DSLFont.h"

using namespace std;
//______________________________________________________________________________
VGDevice* DSLSystem::CreateDisplayDevice(ostream& outstream)
{
	return new DSLDevice (outstream, this);
}

//______________________________________________________________________________
VGDevice* DSLSystem::CreateDisplayDevice()
{
	return new DSLDevice (cout, this);
}

//______________________________________________________________________________
VGDevice* DSLSystem::CreateMemoryDevice(int w, int h)
{
	DSLDevice* device = new DSLDevice (cout, this);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
VGDevice* DSLSystem::CreateMemoryDevice(const char * inPath)
{
    DSLDevice* device = new DSLDevice (cout, this);
    return device;
}

//______________________________________________________________________________
VGDevice* DSLSystem::CreatePrinterDevice( )							{ return 0; }

//______________________________________________________________________________
VGDevice* DSLSystem::CreateAntiAliasedMemoryDevice( int w, int h )
{
	DSLDevice* device = new DSLDevice (cout, this);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
const VGFont* DSLSystem::CreateVGFont( const char * faceName, int size, int properties ) const
{
	return new DSLFont (faceName, size, properties);
}

