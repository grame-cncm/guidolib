/*
	GUIDO Library
	Copyright (C) 2003--2006  Grame

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
