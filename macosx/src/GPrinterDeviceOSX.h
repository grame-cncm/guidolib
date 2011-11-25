#ifndef GPrinterDeviceOSX_H
#define GPrinterDeviceOSX_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003--2006  Grame

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

/////////////////////////////////////////////////////////////////
///
/// 	MacOS X Quartz 2D implementation of VGDevice.
///
/////////////////////////////////////////////////////////////////
#include "GDeviceOSX.h"

// --------------------------------------------------------------
class_export GPrinterDeviceOSX : public GDeviceOSX
{
	public:
								//	Printer constructor 
								GPrinterDeviceOSX(CGContextRef printContext, VGSystem* sys, int inWidth = 0, int inHeight = 0);		
		virtual					~GPrinterDeviceOSX();

		// - Generic services -----------------------------------------
		virtual void			NotifySize( int inWidth, int inHeight );
	
};
			
#endif



