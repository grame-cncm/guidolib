#ifndef __SVGSystem__
#define __SVGSystem__

/*
	GUIDO Library
	Copyright (C) 2011	Grame

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

#include "GUIDOExport.h"

#ifdef WIN32
# pragma warning (disable : 4275 4251)
#endif

#include <iostream>
#include "VGSystem.h"

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/

// --------------------------------------------------------------
class_export SVGSystem : public VGSystem
{
	public:
					 SVGSystem() {}
		virtual		~SVGSystem() {}

		virtual VGDevice*		CreateDisplayDevice( std::ostream& outstream, const char* fontfile=0 );
		virtual VGDevice*		CreateDisplayDevice();
		virtual VGDevice*		CreateDisplayDevice(const char* fontfile=0);
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight);
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight, const char* fontfile=0 );
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice( );
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;

};

/*! @} */

#endif
