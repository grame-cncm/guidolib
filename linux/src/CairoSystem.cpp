/*
	GUIDO Library
	Copyright (C) 2010	Grame

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

#include <iostream>

#include "CairoSystem.h"
#include "CairoDevice.h"
#include "CairoFont.h"

// --------------------------------------------------------------
// 		A Cairo based system
// --------------------------------------------------------------
CairoSystem::CairoSystem (cairo_t * dev) : fDevice(dev) {}

// - VGDevice services -------------------------------------------
VGDevice* CairoSystem::CreateDisplayDevice()
{
	return new CairoDevice (fDevice, this);
}

VGDevice* CairoSystem::CreateMemoryDevice( int width, int height )
{
	return new CairoDevice (width, height, this);
}

VGDevice* CairoSystem:: CairoSystem::CreateMemoryDevice( const char * inPath )
{
	return 0;		// not yet supported, should be ok with png surfaces
}

VGDevice* CairoSystem::CreatePrinterDevice( )
{
	return new CairoDevice (fDevice, this);
}

VGDevice* CairoSystem::CreateAntiAliasedMemoryDevice( int width, int height )
{
	return new CairoDevice (width, height, this);
}

// - Font services -----------------------------------------------
const VGFont* CairoSystem::CreateVGFont( const char * faceName, int size, int properties ) const
{
	return new CairoFont (faceName, size, properties);
}

