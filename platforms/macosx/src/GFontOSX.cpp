/*
  GUIDO Library
	Copyright (C) 2003--2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <cmath>

#include "GFontOSX.h"

static CTFontDescriptorRef createCTFontDescriptor(const char * faceName, int size, int properties);

// --------------------------------------------------------------
GFontOSX::GFontOSX(const char * faceName, int size, int properties) 
			:  mName(faceName), mSize(size), mFontProp(properties)
{
    // Create a font descriptor, assuming faceName is a font family name or concrete font name.
    CTFontDescriptorRef fontDescriptor = createCTFontDescriptor(faceName, size, properties);
    // Create a font reference that best matches the given font descriptor.
    fCTFont = CTFontCreateWithFontDescriptor(fontDescriptor, size, NULL);
    fCGFont = CTFontCopyGraphicsFont(fCTFont, NULL);

    CFStringRef keys[1] = { kCTFontAttributeName };
    CTFontRef values[1] = { fCTFont };
    fCTFontDictionary = CFDictionaryCreate(NULL, (const void **) keys, (const void **) values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
}

// --------------------------------------------------------------
GFontOSX::~GFontOSX()
{
    CFRelease (fCTFontDictionary);
    CFRelease (fCGFont);
	CFRelease (fCTFont);
}

CGGlyph  GFontOSX::GetGlyph( unsigned int inSymbolID ) {
#ifdef SMUFL
    CGGlyph glyphs[1] = { 0 };
    UniChar ustr[1] = { (UniChar)inSymbolID };
    bool done = CTFontGetGlyphsForCharacters(fCTFont, ustr, glyphs, 1);
    if (done) {
        return glyphs[0];
    } else {
        return 0;
    }
#else
    return inSymbolID;
#endif
}

// --------------------------------------------------------------
void GFontOSX::GetExtent( const char * s, int inCharCount, float * outWidth, 
					 float * outHeight, VGDevice * context ) const 
{
	if( s == 0 ) return;
	if( inCharCount < 0 )
		inCharCount = (int)strlen( s );

    CFStringRef string = CFStringCreateWithBytes(NULL, (const UInt8 *)s, inCharCount, kCFStringEncodingUTF8, false);
    CFIndex length = CFStringGetLength(string);
    CFRange range = CFRangeMake(0, length);
    CFAttributedStringRef attrString = CFAttributedStringCreate(NULL, string, fCTFontDictionary);
    CTFramesetterRef fsetter = CTFramesetterCreateWithAttributedString (attrString);
    CFRange fitRange;
    CGSize frameSize = CTFramesetterSuggestFrameSizeWithConstraints (fsetter,
                                                                     range,
                                                                     NULL,  // frame attributes
                                                                     CGSizeMake (CGFLOAT_MAX, CGFLOAT_MAX), // mSize+1
                                                                     &fitRange);
    *outWidth = ceil(frameSize.width);
    *outHeight = mSize; // frameSize.height;
    CFRelease(fsetter);
    CFRelease(attrString);
    CFRelease(string);
}

// --------------------------------------------------------------
void GFontOSX::GetExtent( int c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	if (!c) {
		*outWidth  = 0;
		*outHeight = 0;
		return;
	}

	CGGlyph glyphs[1];
	UniChar ustr[1];
	ustr[0] = c;
	bool done = CTFontGetGlyphsForCharacters(fCTFont, ustr, glyphs, 1);
	if (!done)
		std::cerr << "GFontOSX::GetExtent error in CTFontGetGlyphsForCharacters with char '" << c << "'" << std::endl;
    double advance = CTFontGetAdvancesForGlyphs(fCTFont, kCTFontOrientationHorizontal, glyphs, NULL, 1);
    *outWidth = advance;
    *outHeight = float(mSize); // float(rect.size.height);

    if (c == 139) {
        *outWidth = 4.0;        // ????????????????
    }
}

// Creates a font descriptor.
// The client becomes the owner of the font descriptor and should release it.
static CTFontDescriptorRef createCTFontDescriptor(const char * faceName, int size, int properties) {
    CFStringRef fontName = CFStringCreateWithCString(NULL, faceName, kCFStringEncodingUTF8);
    CTFontDescriptorRef fontDescriptor = CTFontDescriptorCreateWithNameAndSize(fontName, size);
    if (properties) {
        CTFontSymbolicTraits traits = 0;
        if (properties & 1) {
            traits |= kCTFontTraitBold;
        }
        if (properties & 2) {
            traits |= kCTFontTraitItalic;
        }
        CTFontDescriptorRef other = CTFontDescriptorCreateCopyWithSymbolicTraits(fontDescriptor, traits, traits);
        if (other) {
            CFRelease(fontDescriptor);
            fontDescriptor = other;
        }
    }
    CFRelease(fontName);
    return fontDescriptor;
}
