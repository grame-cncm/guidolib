/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include <string.h>

#include "GuidoDefs.h"	// for gGlobalSettings.gDevice
#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "VGFont.h"
#include "FontManager.h"
#include "ARFontAble.h"

using namespace std;

FontInfoList FontManager::sFontList;
const VGFont *	FontManager::gFontScriab = 0;
const VGFont *	FontManager::gFontText   = 0;
string	FontManager::kDefaultMusicFont = kMusicFontStr;
string	FontManager::kDefaultTextFont  = "Times";

#ifdef SMUFL
const char* kMusicFontStr = "Bravura";
#else
const char* kMusicFontStr = "Guido2";
#endif

// ===========================================================================
// 		FontInfo
// ===========================================================================
/** \brief If the Name is set to NULL, the standard GUIDO-Font is used.
*/
FontInfo::FontInfo( const VGFont * inFontRef, int size, const string& name, const string& attributes )
{
	mFontRef = inFontRef;
	mSize = size;
	if( name.size() )
		mName = name;
	else
		mName = FontManager::kDefaultMusicFont;	
	mAttrib = attributes;
}
FontInfo::~FontInfo() 	{ delete mFontRef; }


/** \brief Compares if the entry matches the given text.
*/
bool FontInfo::Compare( int size, const string& name, const string& attributes) const
{
	return (size == mSize)	&& (name == mName)	&& (attributes == mAttrib);
}

// these static global classes are defined to trigger FontManager destructor at library exit
// otherwise the memory leaks
static FontManager gFontManager;

// ===========================================================================
// 		FontManager
// --------------------------------------------------------------------------
FontManager::~FontManager()	{ ReleaseAllFonts(); }


// --------------------------------------------------------------------------
const VGFont * FontManager::GetTextFont( const ARFontAble* ar, float lspace, unsigned int& textalign)
{
	const VGFont * font = gFontText;
	if (ar) {
		int size = int(ar->getFSize() * lspace / LSPACE);
		if (size == 0)
			size = (int)(1.5f * lspace);
		string name = ar->getFont();
		const char* attr = ar->getTextAttributes();
		const char* tf = ar->getTextFormat();
		if (tf && (strlen(tf) == 2)) {
			unsigned int align = 0;
			switch (tf[0]) {
				case 'l':	align = VGDevice::kAlignLeft; break;
				case 'c':	align = VGDevice::kAlignCenter; break;
				case 'r':	align = VGDevice::kAlignRight; break;
			}

			switch (tf[1]) {
				case 't':	align += VGDevice::kAlignTop; break;
				case 'c':	align += VGDevice::kAlignBase; break;
				case 'b':	align += VGDevice::kAlignBottom; break;
			}
			textalign = align;
		}
		
		font= FontManager::FindOrCreateFont( size, (name.empty() ? kDefaultTextFont : name).c_str(), attr );
	}
	return font;
}


// --------------------------------------------------------------------------
// TODO: currently, only text fonts can have different attributes (bold, underline, italic).
// Many text and musical fonts with different sizes can exists.
// In the futur, we could have only one musical font at one size, and scale the context to
// draw musical symbols at any size.

/** \brief Looks for a font in the font list, and returns it. If the font does not exist, it 
	is created and added to the font list.
*/
const VGFont* FontManager::FindOrCreateFont(VGSystem* sys, int size, const char* name, const char * attributes )
{
	// - First, we look if a similar font is already in our font list.
	const char * fontName = name ? name : kDefaultMusicFont.c_str();
	const char * fontAttributes = attributes ? attributes : "";
	
	// - First, we look if a similar font is already in our font list.
	for( auto infos: sFontList ) {
		if( infos->Compare( size, fontName, fontAttributes ))
			return infos->mFontRef;
	}

	// - The font does not exist, we create it.
	int attrib = VGFont::kFontNone;
	string attStr (fontAttributes);
	if( attStr.find("b") != string::npos )	attrib |= VGFont::kFontBold;
	if( attStr.find("i") != string::npos )	attrib |= VGFont::kFontItalic;
	if( attStr.find("u") != string::npos )	attrib |= VGFont::kFontUnderline;

	const VGFont* fontRef = 0;
	if (sys) 		fontRef = sys->CreateVGFont( fontName, size, attrib );
	if( fontRef ) 	sFontList.push_back( new FontInfo( fontRef, size, fontName, fontAttributes ) );
	else 			cerr << "Guido error: \"" <<  fontName << "\" font creation failed !" << endl;
	return fontRef;
}

// --------------------------------------------------------------------------
/** \brief Looks for a font in the font list, and returns it. If the font does not exist, it
	is created and added to the font list.
*/
const VGFont* FontManager::FindOrCreateFont( int size, const char* name, const char* attributes )
{
	VGSystem * sys = 0;
	if( gGlobalSettings.gDevice)
		sys = gGlobalSettings.gDevice->getVGSystem();
	const VGFont* font = FindOrCreateFont (sys, size, name, attributes);
	if (!font) {											// font not found
		
		if (kMusicFontStr != name) 							// if not the music font
			font = FindOrCreateFont (sys, size, kDefaultTextFont.c_str(), attributes);
	}
	return font;
}

// --------------------------------------------------------------------------
/** \brief Destroy all fonts of the font list.
*/
void FontManager::ReleaseAllFonts()
{
	FontInfoList::iterator ptr;
	for( ptr = sFontList.begin(); ptr != sFontList.end(); ++ ptr )
	{
		const FontInfo * fontInfos = *ptr;
		delete fontInfos;
	}
	sFontList.clear();
	gFontScriab = gFontText = 0;
}

// --------------------------------------------------------------------------
/** \brief computes the width of a symbols strings
 
	Input string is expected to contain only numbers or '+' signs
	Used by GRMeter and GRSingleRest (for multimeasures rest)
*/
float FontManager::ComputeSymbolsStrWidth (VGDevice* hdc, const string& str, float spacing)
{
	float width = 0;
	if (!hdc)	return 0;

#ifdef SMUFL
	const char* ptr = str.c_str();
	while (*ptr) {
		int symbol = (*ptr == '+') ? kMeterPlusSymbol : *ptr + kMeter0Symbol - '0';
		float w, h;
		FontManager::gFontScriab->GetExtent(symbol, &w, &h, hdc);
		width += w + spacing;
		ptr++;
	}
#else
	float h;
	FontManager::gFontScriab->GetExtent (str.c_str(),  int(str.size()), &width, &h, hdc);
#endif
	return width;
}
