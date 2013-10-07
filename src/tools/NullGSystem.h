#ifndef __NullGSystem__
#define __NullGSystem__

/*
  GUIDO Library
  Copyright (C) 2003-2006 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
