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


#include <cairo.h>
#include <string.h>

#include "CairoFont.h"
#include "VGDevice.h"

// --------------------------------------------------------------
CairoFont::CairoFont(const char* faceName, int size, int properties, const unsigned int * charSet) 
				: fName(faceName),
				  fSize(size), fProperties(properties), fSymbolTable(charSet)
{}

CairoFont::~CairoFont() {}


// --------------------------------------------------------------
void CairoFont::SelectFont( cairo_t * cr ) const
{
	cairo_font_slant_t slant = GetProperties() & VGFont::kFontItalic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL;
	cairo_font_weight_t weight = GetProperties() & VGFont::kFontBold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL;
	cairo_select_font_face (cr, GetName(), slant, weight);
	cairo_set_font_size (cr, GetSize());
}


// --------------------------------------------------------------
void CairoFont::GetExtent( const char * s, float * outWidth, float * outHeight, cairo_t * context ) const
{
        bool newcontext = false;
	if (!context) {
        	cairo_surface_t* surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 1000, 1000);
                context = cairo_create (surface);
		cairo_surface_destroy (surface);	
		newcontext = true;
		std::cerr << "Your device has no native context.  Probably due to mixing and matching cairo with another device. Creating an ad hoc context and crossing fingers." << std::endl;
	}
	SelectFont (context);

//	cairo_matrix_t matrix;
//	cairo_get_matrix (context, &matrix);

	cairo_text_extents_t extents;
	cairo_text_extents (context, s, &extents);
	*outWidth	= extents.width;
	*outHeight	= extents.height;
//	*outWidth	= extents.width - extents.x_advance;
//	*outHeight	= extents.height - extents.y_advance;
//	*outWidth	= (extents.width - extents.x_advance) * matrix.xx;
//	*outHeight	= (extents.height - extents.y_advance) * matrix.yy;
//std::cout << "CairoFont::GetExtent -" << s << "- w/h " << *outWidth << "/" << *outHeight 
//	<< " - adv x/y " << extents.x_advance << "/" << extents.y_advance << std::endl;
        if (newcontext)
                cairo_destroy(context);
}

void CairoFont::GetExtent( const char * s, int charCount, float * outWidth, float * outHeight, VGDevice * dev ) const
{
	if (s[charCount] == 0)
		GetExtent (s, outWidth, outHeight, (cairo_t *)dev->GetNativeContext());
	else {
		char* str = new char[charCount+1];
		memcpy (str, s, charCount);
		str[charCount] = 0;
		GetExtent (str, outWidth, outHeight, (cairo_t *)dev->GetNativeContext());
		delete str;
	}
}

void CairoFont::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * dev ) const
{
	extern const char* to_utf8 (int n, int& outcount);
	int n;
	const char * str = to_utf8(c, n);
	return GetExtent (str, outWidth, outHeight, (cairo_t *)dev->GetNativeContext());
}
