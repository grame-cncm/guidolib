#ifndef GSystemOSX_H
#define GSystemOSX_H

/*
	GUIDO Library
	Copyright (C) 2006	Grame
	
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
