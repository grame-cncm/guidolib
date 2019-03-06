/*
  GUIDO Library
	Copyright (C) 2003--2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
    if (faceName == NULL) {
        return NULL;
    }
    CFStringRef fontName = CFStringCreateWithCString(NULL, faceName, kCFStringEncodingUTF8);
    CTFontDescriptorRef fontDescriptor = CTFontDescriptorCreateWithNameAndSize(fontName, size);
    if (properties) {
        CTFontSymbolicTraits traits = 0;
        if (properties & 1) {
            traits |= kCTFontTraitBold;
        }
        if (properties & 2) {
            traits |= kCTFontTraitItalic;
        }
        CTFontDescriptorRef other = CTFontDescriptorCreateCopyWithSymbolicTraits(fontDescriptor, traits, traits);
        if (other) {
            CFRelease(fontDescriptor);
            fontDescriptor = other;
        }
    }
    // Does not work: CFTypeRef attribute = CTFontDescriptorCopyAttribute(fontDescriptor, kCTFontNameAttribute); // CFStringRef replacement = (CFStringRef)attribute; // Times -> Times, not TimesNewRomanPSMT.
    // kCTFontNameAttribute is wrong. We need postscriptName.
    // In UIFontDescriptor/NSFontDescriptor there is a method for it.
    // But in CoreText there seems to be no way to get it from CTFontDescriptor.
    // So we get it from CTFont.
    CTFontRef font = CTFontCreateWithFontDescriptor(fontDescriptor, size, NULL);
    GFontOSX *ret;
    if (font) {
        CFStringRef replacement = CTFontCopyPostScriptName(font);
        CFIndex len = CFStringGetLength(replacement);
        CFIndex max = CFStringGetMaximumSizeForEncoding(len, kCFStringEncodingUTF8);
        char buffer[max+1];
        CFStringGetCString(replacement, buffer, max+1, kCFStringEncodingUTF8);
        // FIXME: we should pass font in the constructor.
        ret = new GFontOSX (buffer, size, properties);
        CFRelease(replacement);
        CFRelease(font);
    } else {
        ret = new GFontOSX (faceName, size, properties);
    }
    CFRelease(fontDescriptor);
    CFRelease(fontName);
    return ret;
}
