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

#include "GFontGtk.h"
#include "gdk/gdk.h"
#include "math.h"
// --------------------------------------------------------------
GFontGtk::GFontGtk( PangoFontDescription* nativeFont, const char * faceName, int size, int properties, const unsigned int * charSet ) 
				: mNativeFont(nativeFont), mName(faceName),
				  mSize(size), mFontProp(properties), mSymbolTable(charSet)
{
}

// --------------------------------------------------------------
GFontGtk::~GFontGtk()
{ 
	//delete native font
}
// --------------------------------------------------------------
void
GFontGtk::GetExtent( const char * s, int inCharCount, float * outWidth, float * outHeight, VGDevice * context ) const 
{
	PangoContext* pangoContext = gdk_pango_context_get();

	PangoLayout *layout = pango_layout_new( pangoContext );
	int intWidth, intHeight;

	pango_layout_set_text( layout, s, inCharCount );
	pango_layout_set_font_description( layout, (PangoFontDescription *) GetNativeFont() );

	pango_layout_get_pixel_size(layout, &intWidth, &intHeight );

	g_object_unref( G_OBJECT( layout ) );
	//XXX: scaling ?
	*outWidth = intWidth / PANGO_SCALE;
	*outHeight = intHeight / PANGO_SCALE; 
}

// --------------------------------------------------------------
void	
GFontGtk::GetExtent( char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	const char theChar = (const char)Symbol( c );

	PangoContext* pangoContext = gdk_pango_context_get();
	
	PangoLayout *layout = pango_layout_new( pangoContext );
	int intWidth, intHeight;

	PangoFontDescription *mydescr = pango_font_description_copy( (PangoFontDescription *) GetNativeFont() );
	pango_font_description_set_size( mydescr, GetSize() * PANGO_SCALE * context->GetYScale() );

	pango_layout_set_font_description( layout, (PangoFontDescription *) mydescr );
	pango_layout_set_text( layout, &theChar, 1 );
	pango_layout_get_pixel_size( layout, &intWidth, &intHeight );

	g_object_unref( G_OBJECT( layout ) );

	*outWidth = lrintf( (float)intWidth / context->GetXScale() );
	*outHeight = lrintf( (float) intHeight / context->GetYScale() );
	
}


