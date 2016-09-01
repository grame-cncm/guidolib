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

#include "GSystemGtk.h"
#include "GFontGtk.h"
#include "GDeviceGtk.h"

// static instance of VGSystem;
VGSystem* VGSystem::mInstance = NULL;

// --------------------------------------------------------------
GSystemGtk::GSystemGtk(	GdkDrawable * dispDC, GdkDrawable * printDC ) 
		: mDispDC(dispDC), mPrintDC(printDC)													
{
	setupCharEncodingTable();
}
// --------------------------------------------------------------
GSystemGtk::~GSystemGtk()
{
}
// --------------------------------------------------------------
// - VGDevice services ------------------------------------------
// --------------------------------------------------------------
// Creates and returns a pointer to a new display VGDevice 
// which can be used directly to draw on the screen.
VGDevice*
GSystemGtk::CreateDisplayDevice()	
{
	GDeviceGtk * dispDevice = new GDeviceGtk( mDispDC, 500, 500 );
	const GFontGtk * dispFont;
	dispFont = (const GFontGtk*)(CreateVGFont( "Times", 10, VGFont::kFontNone ));
	dispDevice->SetTextFont(dispFont);

	return dispDevice;
}
// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
//	compatible with the application's current screen. This device
// can be used to draw into a bitmap.
VGDevice*
GSystemGtk::CreateMemoryDevice( int inWidth, int inHeight )
{
	GDeviceGtk * memDevice = new GDeviceGtk( mDispDC, inWidth, inHeight );
	const GFontGtk * memFont;
	memFont = (const GFontGtk*)(CreateVGFont( "Arial", 10, VGFont::kFontNone ));
	memDevice->SetTextFont(memFont);

	return memDevice;
}
// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
// compatible with the file (pixmap) located at the specified 
// path.
VGDevice*
GSystemGtk::CreateMemoryDevice( const char * inPath )
{
	// temporaire ...
	return NULL;
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
//	compatible with the application's current screen. This device
// can be used to draw into a bitmap.
VGDevice*
GSystemGtk::CreateAntiAliasedMemoryDevice( int inWidth, int inHeight )
{
	return CreateMemoryDevice(inWidth, inHeight);
}

// --------------------------------------------------------------
// Creates and returns a pointer to a new printer VGDevice.
VGDevice*
GSystemGtk::CreatePrinterDevice( )
{
	return NULL;
}

// --------------------------------------------------------------
// - Font services ----------------------------------------------
// --------------------------------------------------------------
// "width" feature for font is not portable: has been replaced by
// horizontal scaling of device context.
const VGFont*
GSystemGtk::CreateVGFont( const char * faceName, int size, int properties ) const
{

	PangoFontDescription *pangoFontDescr = pango_font_description_new();

	pango_font_description_set_family( pangoFontDescr, faceName );

	int myWeight			= PANGO_WEIGHT_NORMAL;
	PangoStyle myStyle		= PANGO_STYLE_NORMAL;

	if (properties)
	{
		if (properties & VGFont::kFontBold)			myWeight = PANGO_WEIGHT_BOLD; 		
		if (properties & VGFont::kFontItalic)		myStyle	 = PANGO_STYLE_ITALIC;
	}
	
	pango_font_description_set_style( pangoFontDescr, myStyle );								
//	pango_font_description_set_size( pangoFontDescr, height * PANGO_SCALE / 10 );
	pango_font_description_set_size( pangoFontDescr, size * PANGO_SCALE / 10 );
	// + underline ??

//    printf( "ok... where is the font ? %s italic=%d\n", faceName, italic );

	//XXX: GetThe font into the pangocontext
	
	PangoContext* pangoContext = gdk_pango_context_get();
	PangoLanguage*	pangoLanguage = pango_language_from_string( "en-us" );
	
	PangoFontset *fontset = pango_context_load_fontset( pangoContext, 
							    pangoFontDescr, 
							    pangoLanguage );

	GFontGtk * outFont = 0;
	if( fontset )
		outFont = new GFontGtk( pangoFontDescr, faceName, size, properties, 0 );
	else
		g_object_unref( G_OBJECT( pangoFontDescr) );

	return outFont;
}

// --------------------------------------------------------------
// (JB) note: the new GDeviceOSX version directly pick the right glyph in the font, 
// bypassing the terrible "font encoding" issues. Following code should be
// updated the same way.	
void
GSystemGtk::setupCharEncodingTable()
{
#ifdef __WXMAC__
	mFontEncodingConverter.Setup( kTextEncodingWindowsLatin1, kTextEncodingMacRoman ); 
	for( unsigned int symbol = 0; symbol < kMaxMusicalSymbolID; ++ symbol )
	{
		char theChar = (char)symbol;
		char * convStr = mFontEncodingConverter.Convert( &theChar, 1 );
	
		if( convStr )
			mSymbolToChar[ symbol ] = convStr[ 0 ];
		else
			mSymbolToChar[ symbol ] = 0; 
	}
#else
	for( int symbol = 0; symbol < kMaxMusicalSymbolID; ++ symbol )
	{
		mSymbolToChar[ symbol ] = (char)symbol;
	}	
#endif 
}