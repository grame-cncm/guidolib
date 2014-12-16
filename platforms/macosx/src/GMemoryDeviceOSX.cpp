/*
  GUIDO Library
	Copyright (C) 2003--2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/////////////////////////////////////////////////////////////////
///
/// 	MacOS X Quartz 2D implementation of VGDevice.
///
///	perfs (G3-350): Bach-Inv1.gmn: parse: 240ms, draw: 670ms
/////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#include "GMemoryDeviceOSX.h" 

// --------------------------------------------------------------
GMemoryDeviceOSX::GMemoryDeviceOSX( int inWidth, int inHeight, VGSystem* sys  )
	:GDeviceOSX(inWidth, inHeight, sys), mOffscreen(0)
{
	mContext = CreateBitmapContext(inWidth, inHeight);	
	Init();
}

// --------------------------------------------------------------
GMemoryDeviceOSX::~GMemoryDeviceOSX()
{
	if (mContext) ::CGContextRelease(mContext);
	delete [] (char *)mOffscreen; 	
}

// --------------------------------------------------------------
void GMemoryDeviceOSX::PutImage (CGImageRef img)
{ 
	size_t h = CGImageGetHeight (img);
	size_t w = CGImageGetWidth (img);
	NotifySize (w, h);
	CGRect rect = ::CGRectMake (0, 0, w, h);
	::CGContextSaveGState (mContext);
	::CGContextTranslateCTM (mContext, 0, h);
	::CGContextScaleCTM (mContext, 1.0f, -1.0f);
	::CGContextDrawImage (mContext, rect, img);
	::CGContextRestoreGState (mContext);
}

// --------------------------------------------------------------
void GMemoryDeviceOSX::NotifySize( int inWidth, int inHeight )
{ 
	// pixmap is updated only if input size differs from actual
	if ((mPhysicalWidth != inWidth) || (mPhysicalHeight != inHeight))
	{
		// delete previous bitmap
		::CGContextRelease(mContext);
		// allocates a new one
		mContext = CreateBitmapContext(inWidth, inHeight);
		
		// update attributes each time
		mPhysicalWidth  = inWidth;
		mPhysicalHeight = inHeight; 
		
		// Update context
		Init();
	}
}

// --------------------------------------------------------------
void* GMemoryDeviceOSX::GetBitMapPixels() const
{
	return ::CGBitmapContextGetData(mContext);
}

// --------------------------------------------------------------
CGContextRef GMemoryDeviceOSX::CreateBitmapContext(int inWidth, int inHeight)
{
	const int componentsPerPixel = 4;
	const int bitsPerComponent	= 8;
	const int bytesPerRow = inWidth * componentsPerPixel;
	
	if (mOffscreen) delete [] (char *)mOffscreen; 
	mOffscreen = new char[inHeight * bytesPerRow];
	if (mOffscreen == NULL)  return NULL;
		
	CGContextRef context = ::CGBitmapContextCreate( mOffscreen, 
													(size_t)inWidth, 
													(size_t)inHeight,
													bitsPerComponent,	
													bytesPerRow, 
													mColorSpace, 
													kCGImageAlphaPremultipliedLast);
	if (context == NULL) {
		delete [] (char *)mOffscreen;
		mOffscreen = 0;
	}
	return context;	
}
