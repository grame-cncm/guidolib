#ifndef GSystemWin32GDIPlus_H
#define GSystemWin32GDIPlus_H

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
# if defined(_AFXDLL)
#  include <afx.h>
# else	
#  include <windows.h>// without mfc
# endif
# pragma warning (disable : 4275 4251)
#endif

#include "GUIDOExport.h"
#include "VGSystem.h"
//#include "MusicalSymbols.h"	// kMaxMusicalSymbolID and other constants definitions


// --------------------------------------------------------------
// 		GSystemWin32GDIPlus class
// --------------------------------------------------------------
/** Win32 implementation of the graphic system used for manipulating 
	platform independant drawing devices and fonts. 

	A VGSystem object (for Virtual Guido drawing System) is the highest
	graphical abstraction whose child-object can be used by a client app 
	to obtain a platform dependant graphical system able to perform guido 
	drawing or printing operations (like double-buffered display on the 
	current screen or printing). To do so, the client app simply needs to
	intantiate an object of the appropriate platform dependant VGSystem 
	derived class using the correct platform dependant parameters.

	A VGSystem object deals mainly with two kinds of graphical objects:

		- VGDevice objects, which can be display devices, memory devices
		or printer devices;
		- VGFont objects, used to describe device independant fonts (for
		music or text).

	Using VGSystem in the ScoreProcessingLib:

	VGSystem is the only object the client app should be able to manipulate 
	in order to use any graphical capability of the ScoreProcessingLib (ie 
	displaying, offscreen drawing and printing).

	To do so, the client should instantiate a VGSystem child-class object 
	with every platform dependant data and tell the ScoreProcessingLib to 
	use it. The VGSystem child-class instance is then responsible for 
	allocating the necessary VGDevices and VGFont.
*/				

class_export GSystemWin32GDIPlus : public VGSystem		
{					
	public:
								 GSystemWin32GDIPlus( HDC dispDC, HDC printDC );
		virtual					~GSystemWin32GDIPlus();

		// - VGDevice services -------------------------------------------

		/// Creates and returns a pointer to a new display VGDevice 
		///	which can be used directly to draw on the screen.
		virtual VGDevice*		CreateDisplayDevice();	

		/// Creates and returns a pointer to a new memory VGDevice
		///	compatible with the application's current screen. This device
		/// can be used to draw into a bitmap.
		virtual VGDevice*		CreateMemoryDevice( int inWidth, 
													int inHeight );
		/// Creates and returns a pointer to a new memory VGDevice
		///	compatible with the file (pixmap) located at the specified 
		/// path.
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );

		/// Creates and returns a pointer to a new printer VGDevice.
		virtual VGDevice*		CreatePrinterDevice( );

		virtual	VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );

		// - Font services -----------------------------------------------
		/// Creates and returns a pointer to a new VGFont using the 
		///	specified parameters. 
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;

		/// Windows specific - Sets the printing HDC in typical case 
		/// it can be set when constructor is called 
				void			SetPrinterDC(HDC printDC);

		static void			Start();
		static void			ShutDown();

	protected:
				//charset setup
				void			SetupCharEncodingTable();
//		unsigned int			fSymbolTable [kMaxMusicalSymbolID];

				// GDIPlus init
		static  ULONG_PTR		mGdiplusToken;
				HDC				mDispDC;
				HDC				mPrintDC;
};

#endif
