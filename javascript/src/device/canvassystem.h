#ifndef CANVASSYSTEM_H
#define CANVASSYSTEM_H

/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#include "VGSystem.h"

#include <ostream>
#include <string.h>
#include "canvasdevice.h"
#include "GuidoFont.h"
#include "TimesFont.h"
#include "SVGFont.h"


/*!
\addtogroup VGSys Virtual Graphic System
@{
*/

/**
 * \brief The CanvasSystem class use in web context to draw on a html canvas.
 */
class CanvasSystem : public VGSystem
{
	private:
		const char*		fGuidoFontSpec;

	public:
		CanvasSystem(const char* guidofontspec=0);
		virtual		~CanvasSystem() {}

		virtual VGDevice*		CreateDisplayDevice( std::ostream& outstream);
		virtual VGDevice*		CreateDisplayDevice();
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight);
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath);
		virtual VGDevice*		CreatePrinterDevice( );
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;
};

/*! @} */

#endif // CANVASSYSTEM_H
