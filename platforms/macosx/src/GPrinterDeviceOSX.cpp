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

