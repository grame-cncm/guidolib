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

#include "GPrinterDeviceOSX.h"

// --------------------------------------------------------------
//	Printer constructor 
GPrinterDeviceOSX::GPrinterDeviceOSX( CGContextRef printContext, VGSystem* sys, int inWidth, int inHeight )
	:GDeviceOSX(inWidth, inHeight, sys)
{
	
	mContext = printContext;
	::CGContextRetain(mContext);

	Init();
}

// --------------------------------------------------------------
GPrinterDeviceOSX::~GPrinterDeviceOSX()
{
	// (Important for printing tasks) restore the text matrix.
	CGAffineTransform textCTM = ::CGContextGetTextMatrix(mContext);
	::CGContextSetTextMatrix(mContext, ::CGAffineTransformScale( textCTM, 1, -1 ));
	::CGContextRelease(mContext);
}


// --------------------------------------------------------------
void
GPrinterDeviceOSX::NotifySize( int inWidth, int inHeight )
{ 
	
	if ((mPhysicalWidth != inWidth) || (mPhysicalHeight != inHeight))
	{
		mPhysicalWidth  = inWidth;
		mPhysicalHeight = inHeight;
		
		// Update context
		::CGContextTranslateCTM(mContext, 0, -GetHeight());
	}	

}

