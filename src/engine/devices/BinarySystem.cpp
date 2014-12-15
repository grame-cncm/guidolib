/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "BinarySystem.h"
#include "BinaryDevice.h"
#include "BinaryFont.h"

using namespace std;
//______________________________________________________________________________
VGDevice* BinarySystem::CreateDisplayDevice(ostream& outstream)
{
	return new BinaryDevice (outstream, this);
}

//______________________________________________________________________________
VGDevice* BinarySystem::CreateDisplayDevice()
{
	return new BinaryDevice (cout, this);
}

//______________________________________________________________________________
VGDevice* BinarySystem::CreateMemoryDevice(int w, int h)
{
	BinaryDevice* device = new BinaryDevice (cout, this);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
VGDevice* BinarySystem::CreateMemoryDevice(const char * inPath)
{
    BinaryDevice* device = new BinaryDevice (cout, this);
    return device;
}

//______________________________________________________________________________
VGDevice* BinarySystem::CreatePrinterDevice( )							{ return 0; }

//______________________________________________________________________________
VGDevice* BinarySystem::CreateAntiAliasedMemoryDevice( int w, int h )
{
	BinaryDevice* device = new BinaryDevice (cout, this);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
const VGFont* BinarySystem::CreateVGFont( const char * faceName, int size, int properties ) const
{
	return new BinaryFont (faceName, size, properties);
}

