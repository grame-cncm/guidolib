#ifndef GDisplayDeviceOSX_H
#define GDisplayDeviceOSX_H

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
/////////////////////////////////////////////////////////////////


#include "GDeviceOSX.h"

// --------------------------------------------------------------
class_export GDisplayDeviceOSX : public GDeviceOSX
{
	public:
								
						GDisplayDeviceOSX (CGContextRef inContext, VGSystem* sys);
		virtual			~GDisplayDeviceOSX();
		
		virtual void	NotifySize( int inWidth, int inHeight );
};			

#endif



