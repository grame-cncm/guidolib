#ifndef GFontWin32GDIPlus_H
#define GFontWin32GDIPlus_H

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
#  include <afxwin.h>         // MFC-kernel and standard components
#  include <afxext.h>         // MFC-extensions
#  include <afxdisp.h>        // MFC OLE-autmonation classes
# else	
#  include <windows.h>// without mfc
# endif
# pragma warning (disable : 4275 4251)
#endif

#include "VGFont.h"
#include "GUIDOExport.h"

#include <gdiplus.h>
#include <string>
using namespace Gdiplus;


// --------------------------------------------------------------
// 		GFontWin32GDIPlus class
// --------------------------------------------------------------
/** \brief Win32 GDI+ implementation of the VGFont device-independant font class.

	Using VGFont in the ScoreProcessingLib:

	The VGSystem child-object used by the client app is responsible 
	for allocating a new VGFont child-object and associating it with 
	the appropriate VGdevices. 

*/		
class_export GFontWin32GDIPlus : public VGFont		
{
	public:
								GFontWin32GDIPlus(	Font* nativeFont, const char* faceName, 
													int size, int properties,
													const unsigned int * charSet );
								virtual	~GFontWin32GDIPlus();

		virtual	const char*		GetName() const			{ return mName.c_str(); }
		virtual	int 			GetSize() const			{ return mSize; }
		virtual	int				GetProperties() const	{ return mFontProp; }

				Font*			GetNativeFont() const	{ return mNativeFont; } 
		inline unsigned int			GetSymbol( unsigned int inSymbolID ) const
									{ return inSymbolID; }

		// - Musical symbol services -------------------------------------------
		/// Computes the width and height of the input string using the current font
		virtual	void			GetExtent( const char * s, int inCharCount, 
										   float * outWidth, float * outHeight,
										   VGDevice * context = 0) const;
		/// Computes the width and height of the input character using the 
		/// current font capabilities in the input VGDevice
		virtual	void			GetExtent( unsigned char c, float * outWidth, float * outHeight,
										   VGDevice * context = 0) const;
				float		mAscent;
				float		mEmHeight;
				float		mDescent;

	protected:
				Font*			mNativeFont;
				std::string		mName;
				int				mSize;
				int				mFontProp;

		const unsigned int *	fSymbolTable;
};


#endif
