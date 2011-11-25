#ifndef __NullGSystem__
#define __NullGSystem__

/*
	GUIDO Library
	Copyright (C) 2003, 2006	Grame
	
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

#include "NullGFont.h"
#include "NullGDevice.h"
#include "VGSystem.h"

// --------------------------------------------------------------
// 	Null Graphic System class
// --------------------------------------------------------------
/** \brief Generic pure virtual class for manipulating platform 
	independant drawing devices and fonts. 
*/				

class NullGSystem : public VGSystem	
{					
	public:
								 NullGSystem()	{}
		virtual					~NullGSystem()	{}

		// - VGDevice services -------------------------------------------
		virtual VGDevice*		CreateDisplayDevice( )							
									{ return new NullGDevice(this); }	
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight ) 
									{ return new NullGDevice(this, inWidth, inHeight); }	
		virtual VGDevice*		CreateMemoryDevice( const char * inPath )		
									{ return new NullGDevice(this); }
		virtual VGDevice*		CreatePrinterDevice( )							
									{ return new NullGDevice(this); }

		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight )
									{ return new NullGDevice(this, inWidth, inHeight); }	

		// - Font services -----------------------------------------------
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const 
									{ return new NullGFont(faceName, size, properties); }
};

#endif
