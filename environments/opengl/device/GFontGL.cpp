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

#include "GFontGL.h"
#include "FTGL/ftgl.h"

//#include "FTFont.h"

// --------------------------------------------------------------
GFontGL::GFontGL( FTFont* nativeFont, const char * faceName, int size, int properties, const unsigned int * charSet ) 
				: mNativeFont(nativeFont), mName(faceName),
				  mSize(size), mFontProp(properties), fSymbolTable(charSet)
{
}

// --------------------------------------------------------------
GFontGL::~GFontGL()
{ 
	//delete native font
}

// --------------------------------------------------------------
void
GFontGL::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const 
{
	float x1, y1, z1, x2, y2, z2;
	if (!s) return;
	
	FTFont * font = GetNativeFont();
	if (!font) return;

	font->BBox( s, x1, y1, z1, x2, y2, z2);
	*outWidth = x2 - x1;
	*outHeight = x2 - x1;
}

// --------------------------------------------------------------
void	
GFontGL::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	float x1, y1, z1, x2, y2, z2;
	char str[2];
	FTFont * font = GetNativeFont();
	str[0] = c;
	str[1] = 0;
	font->BBox( str, x1, y1, z1, x2, y2, z2);
	*outWidth = x2 - x1;
	*outHeight = x2 - x1;
}


