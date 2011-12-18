/*
	GUIDO Library
	Copyright (C) 2011	Grame

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
VGDevice* SVGSystem::CreateMemoryDevice( const char * inPath)		{ return 0; }
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

