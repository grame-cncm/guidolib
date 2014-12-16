#ifndef GSystemOSX_H
#define GSystemOSX_H

/*
  GUIDO Library
  Copyright (C) 2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#import <QuartzCore/QuartzCore.h>
#include "VGSystem.h"
#include "GUIDOExport.h"

/*!
\addtogroup VGSys Virtual Graphic System
*/

// --------------------------------------------------------------
// 		GSystemOSX class
// --------------------------------------------------------------
/** \brief OSX implementation of the virtual graphic system.
*/				
class_export GSystemOSX : public VGSystem		
{					
	public:
					 GSystemOSX( CGContextRef dispDC, CGContextRef printDC);
		virtual		~GSystemOSX();

		// - VGDevice services -------------------------------------------
		virtual VGDevice*		CreateDisplayDevice();	
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight );
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice( );
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );

		// - Font services -----------------------------------------------
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;

	protected:
		CGContextRef	mDispDC;
		CGContextRef	mPrintDC;
};

/*! @} */

#endif
