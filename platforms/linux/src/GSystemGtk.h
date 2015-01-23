#ifndef GSystemGtk_H
#define GSystemGtk_H

/*
	GUIDO Library
	Copyright (C) 2005-2007	Grame
	
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

#include "VGSystem.h"
#include "MusicalSymbols.h"	// kMaxMusicalSymbolID def. only

#include "gdk/gdk.h"

/*!
\addtogroup VGSys Virtual Graphic System
*/

// --------------------------------------------------------------
// 		GSystemGtk class
// --------------------------------------------------------------
/** \brief GTK implementation of the virtual graphic system
*/				

class GSystemGtk : public VGSystem		
{					
	public:
	
								GSystemGtk( GdkDrawable * dispDC, GdkDrawable * printDC );
		virtual					~GSystemGtk();

		// - VGDevice services -------------------------------------------
		virtual VGDevice*		CreateDisplayDevice();	
		virtual VGDevice*		CreateMemoryDevice( int inWidth, int inHeight );
		virtual	VGDevice*		CreateMemoryDevice( const char * inPath );
		virtual VGDevice*		CreatePrinterDevice( );
		virtual VGDevice*		CreateAntiAliasedMemoryDevice( int inWidth, int inHeight );
		
		// - Font services -----------------------------------------------
		virtual const VGFont*	CreateVGFont( const char * faceName, int size, int properties ) const;

	protected:
			void				setupCharEncodingTable();

#if defined(__WXMAC__)
		FontEncodingConverter	mFontEncodingConverter;
#endif			
			char				mSymbolToChar [ kMaxMusicalSymbolID ];
			GdkDrawable * 		mDispDC;
			GdkDrawable * 		mPrintDC;

};

/*! @} */

#endif
