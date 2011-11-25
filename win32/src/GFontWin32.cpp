/*
	GUIDO Library
	Copyright (C) 2006  Grame

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

#include "GFontWin32.h"

// --------------------------------------------------------------
GFontWin32::GFontWin32( HFONT nativeFont, const char * faceName, 
						int size, int properties ) 
						: mNativeFont(nativeFont),
						  mSize(size), mFontProp(properties)
{
	int i=0;
	while (*faceName && (i < 1022)) {
		mName[i++] = *faceName++;
	}
	mName[i] = 0;
}

// --------------------------------------------------------------
GFontWin32::~GFontWin32()
{ 
	HFONT winFont = GetNativeFont();
	if( winFont )
		::DeleteObject( winFont );
}
// --------------------------------------------------------------
void
GFontWin32::GetExtent( const char * s, int inCharCount, float * outWidth, 
					   float * outHeight, VGDevice * context ) const 
{
	HDC hdc;

	if( !(context && context->IsValid()) )
		hdc = ::GetDC( (HWND)NULL ); 
	else 
		hdc = (HDC)(GetContext(context)); 

	HFONT old = (HFONT)::SelectObject( hdc, GetNativeFont() );
		
	SIZE winSize;
//	::GetTextExtentPoint32( GetHDC(), s, inCharCount, &winSize ); 
	::GetTextExtentPoint32( hdc, s, inCharCount, &winSize ); 
	*outWidth	= (float)winSize.cx;
	*outHeight	= (float)winSize.cy;

	::SelectObject( hdc, old );

	if (!context)
		::ReleaseDC( (HWND)NULL, hdc );

}

// --------------------------------------------------------------
void	
GFontWin32::GetExtent( unsigned char c, float * outWidth, 
					   float * outHeight, VGDevice * context ) const
{
	HDC hdc;

	if( !(context && context->IsValid()) )
		hdc = ::GetDC( (HWND)NULL ); 
	else	
		hdc = (HDC)(GetContext(context)); 

	HFONT old = (HFONT)::SelectObject( hdc, GetNativeFont() );
		
	SIZE winSize;
//	::GetTextExtentPoint32( GetHDC(), s, inCharCount, &winSize ); 
	::GetTextExtentPoint32( hdc, (LPCTSTR)&c, 1, &winSize ); 
	*outWidth	= (float)winSize.cx;
	*outHeight	= (float)winSize.cy;

	::SelectObject( hdc, old );

	if (!context)
		::ReleaseDC( (HWND)NULL, hdc );
}
