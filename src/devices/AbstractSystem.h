#ifndef __AbstractSystem__
#define __AbstractSystem__

/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
class_export AbstractSystem : public VGSystem
{
	public:
					 AbstractSystem() {}
		virtual		~AbstractSystem() {}

		virtual VGDevice*		CreateDisplayDevice( std::ostream& outstream);
		virtual VGDevice*		CreateDisplayDevice();
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight);
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice( );
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;

};

/*! @} */

#endif
