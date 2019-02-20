#ifndef GFontWin32_H
#define GFontWin32_H

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
# pragma warning (disable : 4275 4251) 
#endif

#include "GUIDOExport.h"
#include "VGFont.h"
#include <string>


// --------------------------------------------------------------
// 		GFontWin32 class
// --------------------------------------------------------------
/** \brief Win32 implementation of the VGFont device-independant font class.
*/		
class_export GFontWin32 : public VGFont		
{
	public:
								GFontWin32( HFONT nativeFont, 
											const char* faceName, 
											int size, 
											int properties );
								virtual	~GFontWin32();


		// - VGFont services -------------------------------------------
		virtual	const char*		GetName() const
									{ return mName.c_str(); }
		virtual	int 			GetSize() const
									{ return mSize; }
		virtual	int				GetProperties() const
									{ return mFontProp; }

		// - Symbol services -------------------------------------------
		virtual	void			GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context = 0) const;
		virtual	void			GetExtent( int c, float * outWidth, float * outHeight, VGDevice * context = 0) const;

		// - Win32 specific services -----------------------------------
				HFONT			GetNativeFont() const
									{ return mNativeFont; } 

	
	protected:

				HFONT			mNativeFont;
				std::string		mName;
				int				mSize;
				int				mFontProp;
};


#endif
