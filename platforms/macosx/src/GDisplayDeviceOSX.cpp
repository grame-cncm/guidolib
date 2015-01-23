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

#include "GDisplayDeviceOSX.h"
//#include "GSystemOSX.h"

// --------------------------------------------------------------
//	Onscreen constructor - warning ! default size is 1x1 ! 
//  Notify the new size before using !
GDisplayDeviceOSX::GDisplayDeviceOSX( CGContextRef inContext, VGSystem* sys  )
	:GDeviceOSX(1, 1, sys)
{
	// Enternally received context is "kept" in the object
	mContext = inContext;
	::CGContextRetain(mContext);
	
	Init();
	
	// guido hack - must be removed asap
//	mSys = new GSystemOSX(inContext, NULL, NULL); 
}

// --------------------------------------------------------------
GDisplayDeviceOSX::~GDisplayDeviceOSX()
{
	::CGContextRelease(mContext);
}

// --------------------------------------------------------------
void GDisplayDeviceOSX::NotifySize( int inWidth, int inHeight )
{ 
	if ((mPhysicalWidth != inWidth) || (mPhysicalHeight != inHeight))
	{
		mPhysicalWidth  = inWidth;
		mPhysicalHeight = inHeight;
		
		// Update context
#ifdef _USE_QD_COORDINATES_
		::CGContextTranslateCTM(mContext, 0, -GetHeight());
#endif
	}
}
