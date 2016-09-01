/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "AbstractSystem.h"
#include "AbstractDevice.h"
#include "AbstractFont.h"

using namespace std;
//______________________________________________________________________________
VGDevice* AbstractSystem::CreateDisplayDevice(ostream& outstream)
{
	return new AbstractDevice (outstream, this);
}

//______________________________________________________________________________
VGDevice* AbstractSystem::CreateDisplayDevice()
{
	return new AbstractDevice (cout, this);
}

//______________________________________________________________________________
VGDevice* AbstractSystem::CreateMemoryDevice(int w, int h)
{
	AbstractDevice* device = new AbstractDevice (cout, this);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
VGDevice* AbstractSystem::CreateMemoryDevice(const char * inPath)
{
    AbstractDevice* device = new AbstractDevice (cout, this);
    return device;
}

//______________________________________________________________________________
VGDevice* AbstractSystem::CreatePrinterDevice( )							{ return 0; }

//______________________________________________________________________________
VGDevice* AbstractSystem::CreateAntiAliasedMemoryDevice( int w, int h )
{
	AbstractDevice* device = new AbstractDevice (cout, this);
	device->NotifySize( w, h );
	return device;
}

//______________________________________________________________________________
const VGFont* AbstractSystem::CreateVGFont( const char * faceName, int size, int properties ) const
{
	return new AbstractFont (faceName, size, properties);
}

