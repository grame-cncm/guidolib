#ifndef GSystemWin32_H
#define GSystemWin32_H

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

#ifdef WIN32
# if defined(_AFXDLL) // || defined(_WINDLL)	// using mfc
#  include <afx.h>
# else	
#  include <windows.h>// without mfc
# endif
# pragma warning (disable : 4275) 
#endif

#include "GUIDOExport.h"
#include "VGSystem.h"


/*!
\addtogroup VGSys Virtual Graphic System
*/

// --------------------------------------------------------------
// 		GSystemWin32 class
// --------------------------------------------------------------
/*!
	\brief  Win32 implementation of the virtual graphic system 
*/				
class_export GSystemWin32 : public VGSystem		
{					
	public:
								GSystemWin32( HDC dispDC, HDC printDC );
		virtual					~GSystemWin32();

		// - VGDevice services -------------------------------------------
		virtual VGDevice*		CreateDisplayDevice();	
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight );
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice( );
		virtual	VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );

		// - Font services -----------------------------------------------
		virtual const VGFont*	CreateVGFont( const char * faceName, int size,  int properties ) const;

		/// Win32 specific - Sets the printing HDC in typical case it can be set when constructor is called 
				void			SetPrinterDC(HDC printDC);

	protected:
				HDC				mDispDC;
				HDC				mPrintDC;
};

/*! @} */

#endif
