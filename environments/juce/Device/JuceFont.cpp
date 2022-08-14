/*
	GUIDO Library
	Copyright (C) 2012	Grame
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2), 
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
*/


#include "JuceFont.h"
#include "JuceHeader.h"

#include <string>
using namespace juce;

// --------------------------------------------------------------
// 		Juce implementation of the VGFont class
// --------------------------------------------------------------
JuceFont::JuceFont(const char * faceName, int size, int properties) : fNativeFont(0)
{
	int style = Font::plain;
	if (properties & kFontBold)			style += Font::bold;
	if (properties & kFontItalic)		style += Font::italic;
	if (properties & kFontUnderline)	style += Font::underlined;
	
	std::string guido("Guido2");
	float scale = (guido == faceName) ? 2.49 : 1.;
	fNativeFont = new Font(faceName, float(size * scale), style);
	fName = faceName;
}

JuceFont::~JuceFont()	{ delete fNativeFont; }

// --------------------------------------------------------------
const char * JuceFont::GetName() const	{ return fName.c_str(); }
int JuceFont::GetSize() const			{ return fNativeFont ? int(fNativeFont->getHeight()) : 0; }
int JuceFont::GetProperties() const
{
	int properties = kFontNone;
	if (fNativeFont) {
		if (fNativeFont->isBold()) properties += kFontBold;
		if (fNativeFont->isItalic()) properties += kFontItalic; 
		if (fNativeFont->isUnderlined()) properties += kFontUnderline;
	}
	return properties;
}

// - Symbol services ---------------------------------------------
void JuceFont::GetExtent( const char * s, int count, float * width, float * height, VGDevice *) const
{
	auto text = String::fromUTF8 (s, count);
	*width = fNativeFont->getStringWidthFloat (text);
	*height = fNativeFont->getAscent() + fNativeFont->getDescent();
}

void JuceFont::GetExtent( int c, float * width, float * height,  VGDevice *) const
{
	String text;
	text += wchar_t(c);
	*width = fNativeFont->getStringWidthFloat (text);
	*height = fNativeFont->getAscent() + fNativeFont->getDescent();
}
