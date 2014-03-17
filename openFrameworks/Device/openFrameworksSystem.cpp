/*
	GUIDO Library
	Copyright (C) 2012	Grame

	openFrameworks Guido interface by Thomas Coffy (c) IRCAM 2014
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2), 
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
*/


#include "openFrameworksFont.h"
#include "openFrameworksDevice.h"
#include "openFrameworksSystem.h"

#include <ofMain.h>

// --------------------------------------------------------------
// 		openFrameworks implementation of the VGSystem class
// --------------------------------------------------------------
VGDevice* openFrameworksSystem::CreateDisplayDevice( )
{
	return new openFrameworksDevice (this);
}

VGDevice* openFrameworksSystem::CreateMemoryDevice( int width, int height )
{
	return new openFrameworksDevice (width, height, this);
}

VGDevice* openFrameworksSystem::CreateMemoryDevice( const char *  )
{
//	FileInputStream file (File (path));
//	if (file.getStatus().failed()) return 0;
//
//	Image img;
//	GIFImageFormat gif;
//	JPEGImageFormat jpg; 
//	PNGImageFormat png;
//	if (gif.canUnderstand (file))
//		img = gif.decodeImage (file);
//	else if (jpg.canUnderstand (file))
//		img = jpg.decodeImage (file);
//	else if (png.canUnderstand (file))
//		Image img = png.decodeImage (file);
//	else return 0;
//
//	Graphics * g = new Graphics(img);
//	return new JuceDevice (g, this);
	return 0;
}

VGDevice* openFrameworksSystem::CreatePrinterDevice( )	{ return 0;	}	// don't know how to create a printer device
VGDevice* openFrameworksSystem::CreateAntiAliasedMemoryDevice( int width, int height )	{ return CreateMemoryDevice(width, height); }

// - Font services -----------------------------------------------
const VGFont*	openFrameworksSystem::CreateVGFont( const char * faceName, int size, int properties ) const
{
	return new openFrameworksFont(faceName, size, properties);
}

