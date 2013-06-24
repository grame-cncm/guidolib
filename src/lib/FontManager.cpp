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
using namespace std;

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice

#include "VGFont.h"
#include "FontManager.h"

FontInfoList FontManager::sFontList;
const VGFont *	FontManager::gFontScriab = 0;
const VGFont *	FontManager::gFontText   = 0;
const char*		FontManager::kDefaultMusicFont = "Guido2";
const char*		FontManager::kDefaultTextFont  = "Times";

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
//FontInfo::~FontInfo() {}
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
// TODO: currently, only text fonts can have different attributes (bold, underline, italic).
// Many text and musical fonts with different sizes can exists.
// In the futur, we could have only one musical font at one size, and scale the context to
// draw musical symbols at any size.

/** \brief Looks for a font in the font list, and returns it. If the font does not exist, it 
	is created and added to the font list.
*/
const VGFont* FontManager::FindOrCreateFont(VGSystem* sys, int size, const string * name, const string * attributesStr )
{
	// - First, we look if a similar font is already in our font list.
	const char * fontNameCStr = name ? name->c_str() : kDefaultMusicFont;
	const char * fontAttribCStr = attributesStr ? attributesStr->c_str() : "";
	
	// - First, we look if a similar font is already in our font list.
	FontInfoList::const_iterator ptr;
	for( ptr = sFontList.begin(); ptr != sFontList.end(); ++ ptr )
	{
		const FontInfo * infos = *ptr;
		if( infos->Compare( size, fontNameCStr, fontAttribCStr ))
			return infos->mFontRef;
	}
	
	// - The font does not exist, we create it.
	int attributes = VGFont::kFontNone;
	if( attributesStr ) {
		if( attributesStr->find("b") != string::npos )	attributes |= VGFont::kFontBold;
		if( attributesStr->find("i") != string::npos )	attributes |= VGFont::kFontItalic;
		if( attributesStr->find("u") != string::npos )	attributes |= VGFont::kFontUnderline;
	}
	
	const VGFont* fontRef = 0;
	if (sys)
		fontRef = sys->CreateVGFont( fontNameCStr, size, attributes );

	if( fontRef )
		sFontList.push_back( new FontInfo( fontRef, size, fontNameCStr, attributesStr ? *attributesStr : "" ) );
	else
		cerr << "Guido error: \"" <<  fontNameCStr << "\" font creation failed !" << endl;
	return fontRef;
}
// --------------------------------------------------------------------------
// TODO: currently, only text fonts can have different attributes (bold, underline, italic).
// Many text and musical fonts with different sizes can exists.
// In the futur, we could have only one musical font at one size, and scale the context to
// draw musical symbols at any size.

/** \brief Looks for a font in the font list, and returns it. If the font does not exist, it 
	is created and added to the font list.
*/
const VGFont* FontManager::FindOrCreateFont( int size, const string * name, const string * attributesStr )
{
	VGSystem * sys = 0;
	if( gGlobalSettings.gDevice)
		sys = gGlobalSettings.gDevice->getVGSystem();
	return FindOrCreateFont (sys, size, name, attributesStr);
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
