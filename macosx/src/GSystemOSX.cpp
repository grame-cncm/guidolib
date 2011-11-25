/*
	GUIDO Library
	Copyright (C) 2006  Grame

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

#include "GSystemOSX.h"
#include "GFontOSX.h"
#include "GDisplayDeviceOSX.h"
#include "GMemoryDeviceOSX.h"
#include "GPrinterDeviceOSX.h"

//#include "DebugDevice.h"

using namespace std;
// --------------------------------------------------------------
GSystemOSX::GSystemOSX(	CGContextRef dispDC, CGContextRef printDC)
					: mDispDC(dispDC), mPrintDC(printDC)
{
	if (mDispDC)
		::CGContextRetain(mDispDC);
	if (mPrintDC)
		::CGContextRetain(mPrintDC);
}

// --------------------------------------------------------------
GSystemOSX::~GSystemOSX()
{
	if (mDispDC)	::CGContextRelease(mDispDC);
	if (mPrintDC)	::CGContextRelease(mPrintDC);
}

// --------------------------------------------------------------
// - VGDevice services ------------------------------------------
// --------------------------------------------------------------
// Creates and returns a pointer to a new display VGDevice 
// which can be used directly to draw on the screen.
VGDevice* GSystemOSX::CreateDisplayDevice()	
{
//	return new DebugDevice( new GDisplayDeviceOSX(mDispDC, this));
	return new GDisplayDeviceOSX(mDispDC, this);
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
// compatible with the application's current screen. This device
// can be used to draw into a bitmap.
VGDevice* GSystemOSX::CreateMemoryDevice( int inWidth, int inHeight )
{
	return (inHeight < 0) 
		? new GMemoryDeviceOSX(inWidth, -inHeight, this)
		: new GMemoryDeviceOSX(inWidth, inHeight, this);
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
//	compatible with the application's current screen. This device
// can be used to draw into a bitmap.
VGDevice* GSystemOSX::CreateAntiAliasedMemoryDevice( int inWidth, int inHeight )
{
	return CreateMemoryDevice(inWidth, inHeight);
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
// compatible with the file (pixmap) located at the specified 
// path.
VGDevice* GSystemOSX::CreateMemoryDevice( const char * inPath )
{
	GMemoryDeviceOSX * dev = new GMemoryDeviceOSX(1, 1, this);

	CFStringRef filePath = ::CFStringCreateWithCString (kCFAllocatorDefault, inPath, kCFStringEncodingMacRoman);
	bool isDirectory = false;
	CFURLRef url = ::CFURLCreateWithFileSystemPath (kCFAllocatorDefault, filePath, kCFURLPOSIXPathStyle, isDirectory);
	CGDataProviderRef source = ::CGDataProviderCreateWithURL (url);
	if (source) {
		bool shouldInterpolate = true;
		CGImageRef img = CGImageCreateWithPNGDataProvider (source, NULL, shouldInterpolate, kCGRenderingIntentDefault);
		if (img) {
			if (dev) dev->PutImage (img);
			::CGImageRelease (img);
		}
		::CGDataProviderRelease (source);
	}
    ::CFRelease (url);
    ::CFRelease (filePath);
	return dev;
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new printer VGDevice.
VGDevice* GSystemOSX::CreatePrinterDevice( )
{
	return new GPrinterDeviceOSX(mPrintDC, this);
}

// --------------------------------------------------------------
// - Font services ----------------------------------------------
// --------------------------------------------------------------
const VGFont* GSystemOSX::CreateVGFont( const char * faceName, int size, int properties ) const
{
	return new GFontOSX (faceName, size, properties);;
}


