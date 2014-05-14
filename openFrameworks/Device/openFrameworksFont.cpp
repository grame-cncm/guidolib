/*
	GUIDO Library
	Copyright (C) 2012	Grame

	openFrameworks Guido interface by Thomas Coffy (c) IRCAM 2014
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2), 
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
*/


#include "openFrameworksFont.h"
#include <ofMain.h>

#include <string>

// --------------------------------------------------------------
// 		openFrameworks implementation of the VGFont class
// --------------------------------------------------------------
openFrameworksFont::openFrameworksFont(const char * faceName, int size, int properties) : fNativeFont(0)
{
	/*
	int style = Font::plain;
	if (properties & kFontBold)			style += Font::bold;
	if (properties & kFontItalic)		style += Font::italic;
	if (properties & kFontUnderline)	style += Font::underlined;
	*/
	
	std::string guido("guido2");
	float scale = (guido == faceName) ? 0.7 : 1.;
	fNativeFont = new ofTrueTypeFont();
	fNativeFont->setGlobalDpi(72);
	string font = faceName;
	if (font == "Times New Roman") // for an obscure reason, the text facename lack file extension
		font += ".ttf";
	cout << "Guido::openFrameworksFont loading " << font << endl;
	if (!fNativeFont->loadFont(font, float(size * scale)))
		cerr << "Error: can not load font : " << font << endl;
	fName = font;
}

openFrameworksFont::~openFrameworksFont()	{ delete fNativeFont; }

// --------------------------------------------------------------
const char * openFrameworksFont::GetName() const	{ return fName.c_str(); }
int openFrameworksFont::GetSize() const			{ return fNativeFont ? int(fNativeFont->getLineHeight()) : 0; }
int openFrameworksFont::GetProperties() const
{
	int properties = kFontNone;
	if (fNativeFont) {
		//if (fNativeFont->isBold()) properties += kFontBold;
		//if (fNativeFont->isItalic()) properties += kFontItalic;
		//if (fNativeFont->isUnderlined()) properties += kFontUnderline;
	}
	return properties;
}

// - Symbol services ---------------------------------------------
void openFrameworksFont::GetExtent( const char * s, int count, float * width, float * height, VGDevice *) const
{
	string text (s, count);
	
	*width = fNativeFont->stringWidth(text);
	*height = fNativeFont->stringHeight(text);
}

void openFrameworksFont::GetExtent( unsigned char c, float * width, float * height,  VGDevice *) const
{
	string text;
	text += wchar_t(c);
	*width = fNativeFont->stringWidth(text);
	*height = fNativeFont->stringHeight(text);
}
