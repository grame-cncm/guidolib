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
#include <iostream>
using namespace std;

#include "GSystemGL.h"
#include "GFontGL.h"
#include "GDeviceGL.h"


#include "FTGL/ftgl.h"
//#include "FTGLExtrdFont.h"
//#include "FTGLOutlineFont.h"
//#include "FTGLPolygonFont.h"
//#include "FTGLTextureFont.h"
//#include "FTGLPixmapFont.h"
//#include "FTGLBitmapFont.h"

#if defined(__APPLE__) || defined(MACOSX)
#define SysFontPath		"/Library/Fonts/"
#elif defined(linux)
#define SysFontPath		"/usr/X11R6/lib/X11/fonts/TTF/"
#elif defined(WIN32)
#define SysFontPath		"c:\\windows\\fonts\\"
#else
#endif

// --------------------------------------------------------------
GSystemGL::GSystemGL( int fontType ) : fFontType(fontType) 									
{
	// Here, we should study differents cases 
	// from the required charSet type 
	SetupCharEncodingTable();

}
// --------------------------------------------------------------
GSystemGL::~GSystemGL()
{
}
// --------------------------------------------------------------
// - VGDevice services ------------------------------------------
// --------------------------------------------------------------
// Creates and returns a pointer to a new display VGDevice 
// which can be used directly to draw on the screen.
VGDevice*
GSystemGL::CreateDisplayDevice()	
{
	GDeviceGL * dispDevice = new GDeviceGL(this);

	return dispDevice;
}
// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
//	compatible with the application's current screen. This device
// can be used to draw into a bitmap.
VGDevice*
GSystemGL::CreateMemoryDevice( int inWidth, int inHeight )
{
	GDeviceGL * memDevice = new GDeviceGL(this);

	return memDevice;
}
// --------------------------------------------------------------
VGDevice*
GSystemGL::CreateAntiAliasedMemoryDevice( int inWidth, int inHeight )
{
	return CreateMemoryDevice(inWidth, inHeight);
}
// --------------------------------------------------------------
// Creates and returns a pointer to a new memory VGDevice
// compatible with the file (pixmap) located at the specified 
// path.
VGDevice*
GSystemGL::CreateMemoryDevice( const char * inPath )
{
	// temporaire ...
	return NULL;

}

// --------------------------------------------------------------
// Creates and returns a pointer to a new printer VGDevice.
VGDevice*
GSystemGL::CreatePrinterDevice( )
{
	GDeviceGL * printDevice = new GDeviceGL(this);
	return NULL;
}

// --------------------------------------------------------------
// - Font services ----------------------------------------------
// --------------------------------------------------------------
// "width" feature for font is not portable: has been replaced by
// horizontal scaling of device context.
const VGFont*
GSystemGL::CreateVGFont( const char * faceName, int size, 
						 int properties ) const
{
	if( faceName == 0 ) return 0;

	FTFont * font = SetupFont (faceName, size );
	
	// Properties translation ?

	if( !font ) {
		string sysFont(SysFontPath);
		sysFont += faceName;
		font = SetupFont( sysFont.c_str(), size );		
	}
	return new GFontGL( font, faceName, size, properties, fSymbolTable );
	
}

// ----------------------------------------------------------------------------
FTFont* 
GSystemGL::SetupFont( const char * inPath, int inFontSize ) const
{
	FTFont * font = 0;

	char faceName2[512];
	strcpy(faceName2, inPath);

#ifdef WIN32 
	if( !strcmp(inPath,"Times") )
		strcpy(faceName2, "times.ttf");	
#endif

	switch (fFontType) {
		case kPixmapFont:
			font = new FTGLPixmapFont((const char*)faceName2);
			break;
		case kBitmapFont:
			font = new FTGLBitmapFont((const char*)faceName2);
			break;
		case kOutlineFont:
			font = new FTGLOutlineFont((const char*)faceName2);
			break;
		case kPolygonFont:
			font = new FTGLPolygonFont((const char*)faceName2);
            glEnable( GL_TEXTURE_2D);
//          glBindTexture(GL_TEXTURE_2D, textureID);
			glDisable( GL_BLEND);
			break;
		case kExtrudeFont:
			font = new FTGLExtrdFont((const char*)faceName2);
			break;
		case kTextureFont:
			font = new FTGLTextureFont((const char*)faceName2);
			glEnable( GL_TEXTURE_2D);
			glDisable( GL_DEPTH_TEST);
			break;
	}

	if( !font || font->Error()) {
		cerr <<  "Failed to open font " << inPath << endl;
		delete font;
		return 0;
	}
	else {
//		font->Depth(20);   // extrusion distance for the font. Only for FTGLExtrdFont
		int cmc = font->CharMapCount();
		FT_Encoding encoding = ft_encoding_unicode;
		FT_Encoding* cml = font->CharMapList();
		for (int i=0; i<cmc; i++) {
			if (i==0) encoding = cml[i];
			if (cml[i] == ft_encoding_latin_1) {
				encoding = ft_encoding_latin_1;
				break;
			}
		}
//cout << "GSystemGL::SetupFont " <<  faceName2 << ": " << (encoding == ft_encoding_latin_1 ? "ft_encoding_latin_1" : (encoding == ft_encoding_unicode ? "ft_encoding_unicode" : "unknown")) << endl;
		font->CharMap(encoding);
		font->FaceSize(inFontSize);
	}
	return font;
}

// ----------------------------------------------------------------------------
void 
GSystemGL::SetupCharEncodingTable()
{
	for(int i = 0; i <= 255; ++i )	fSymbolTable[i] = i;
/*
	fSymbolTable[ kNoneSymbol ]				= 0;
	fSymbolTable[ kEmptySymbol ]			= 32;

	fSymbolTable[ kSharpSymbol ]			= 35;
	fSymbolTable[ kDSharpSymbol ]			= 194;
	fSymbolTable[ kFlatSymbol ]				= 98;
	fSymbolTable[ kDFlatSymbol ]			= 178;
	fSymbolTable[ kNaturalSymbol ]			= 110;
	
	fSymbolTable[ kNoteDotSymbol ]			= 46;
	fSymbolTable[ kStaffLineSymbol ]		= 89;
	fSymbolTable[ kStaffSymbol ]			= 61;
	fSymbolTable[ kFinishBarSymbol ]		= 92;
	fSymbolTable[ kLedgerLineSymbol ]		= 95; //59;
	fSymbolTable[ kBembelSymbol ]			= 168;

	fSymbolTable[ kAccoladeSymbol ]			= 171;
	fSymbolTable[ kRepeatEndSymbol ]		= 125;

	fSymbolTable[ kStemUp1Symbol ]			= 120;
	fSymbolTable[ kStemUp2Symbol ]			= 120;
	fSymbolTable[ kStemDown1Symbol ]		= 96;
	fSymbolTable[ kStemDown2Symbol ]		= 177;

	fSymbolTable[ kBarGSSymbol ]			= 101;
	fSymbolTable[ kBarSymbol ]				= 101;

	fSymbolTable[ kClefViolin ]				= 38;
	fSymbolTable[ kClefBass ]				= 63;
	fSymbolTable[ kClefBratsche ]			= 66;

	fSymbolTable[ kP4Symbol ]				= 165;
	fSymbolTable[ kP8Symbol ]				= 97;
	fSymbolTable[ kP16Symbol ]				= 64;
	fSymbolTable[ kP32Symbol ]				= 263;
	fSymbolTable[ kP64Symbol ]				= 108;
	fSymbolTable[ kP128Symbol ]				= 47;

	fSymbolTable[ kIntensFSymbol ]			= 102;
	fSymbolTable[ kIntensFFSymbol ]			= 166;
	fSymbolTable[ kIntensFFFSymbol ]		= 169;
	fSymbolTable[ kIntensFFFFSymbol ]		= 115;
	fSymbolTable[ kIntensPSymbol ]			= 112;
	fSymbolTable[ kIntensPPSymbol ]			= 185;
	fSymbolTable[ kIntensMFSymbol ]			= 70;
	fSymbolTable[ kIntensMPSymbol ]			= 80;
	fSymbolTable[ kIntensSFSymbol ]			= 83;

	fSymbolTable[ kFlag8UpSymbol ]			= 106;
	fSymbolTable[ kFlag8DownSymbol ]		= 74;
	fSymbolTable[ kFlag16UpSymbol ]			= 107;
	fSymbolTable[ kFlag16DownSymbol ]		= 75;
	fSymbolTable[ kFlag32UpSymbol ]			= 275;
	fSymbolTable[ kFlag32DownSymbol ]		= 276;
	fSymbolTable[ kFlag64UpSymbol ]			= 0;		// Not in font, use the Flag32 !
	fSymbolTable[ kFlag64DownSymbol ]		= 0;		// Not in font, use the Flag32 !

	fSymbolTable[ kStaccatoSymbol ]			= 78;
	fSymbolTable[ kAccentSymbol ]			= 62;
	fSymbolTable[ kMarcatoUpSymbol ]		= 94;
	fSymbolTable[ kMarcatoDownSymbol ]		= 118;
	fSymbolTable[ kTenutoSymbol ]			= 58;
	fSymbolTable[ kFermataUpSymbol ]		= 117;
	fSymbolTable[ kFermataDownSymbol ]		= 85;
	fSymbolTable[ kBreathMarkSymbol ]		= 44;

	fSymbolTable[ kCSymbol ]				= 99;
	fSymbolTable[ kC2Symbol ]				= 67;
	
	fSymbolTable[ kRepeatBeginSymbol ]		= 93;

	fSymbolTable[ kHalfDiamondHeadSymbol ]	= 79;
	fSymbolTable[ kFullDiamondHeadSymbol ]	= 81;
	fSymbolTable[ kLongaHeadSymbol ]		= 87;

	fSymbolTable[ kWholeNoteHeadSymbol ]	= 119;
	fSymbolTable[ kHalfNoteHeadSymbol ]		= 69;
	fSymbolTable[ kFullHeadSymbol ]			= 88;
	fSymbolTable[ kXHeadSymbol ]			= 191;
	fSymbolTable[ kErrorHeadSymbol ]		= 191;

	fSymbolTable[ kMordSymbol ]				= 109;
	fSymbolTable[ kCodaSymbol ]				= 241;
	fSymbolTable[ kSegnoSymbol ]			= 37;
*/
}
