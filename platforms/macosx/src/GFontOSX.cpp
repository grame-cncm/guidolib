/*
  GUIDO Library
	Copyright (C) 2003--2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GFontOSX.h"

// --------------------------------------------------------------
GFontOSX::GFontOSX(const char * faceName, int size, int properties) 
			:  mName(faceName), mSize(size), mFontProp(properties)
{
#ifdef JG_CTFONT_DEF
	fCTName = CFStringCreateWithCString(0, faceName, kCFStringEncodingUTF8);
	fCTFont = CTFontCreateWithName( fCTName, mSize, 0);
    fCGFont = CTFontCopyGraphicsFont(fCTFont, NULL);

    CFStringRef keys[1] = { kCTFontAttributeName };
    CTFontRef values[1] = { fCTFont };
    fCTFontDictionary = CFDictionaryCreate(NULL, (const void **) keys, (const void **) values, 1, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
#endif
}

// --------------------------------------------------------------
GFontOSX::~GFontOSX()
{
#ifdef JG_CTFONT_DEF
    CFRelease (fCTFontDictionary);
    CFRelease (fCGFont);
	CFRelease (fCTFont);
	CFRelease (fCTName);
#endif
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

#ifdef JG_CTFONT_TEXT
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

#else
	float fooBaseLine;
	CGContextRef dc = (CGContextRef)( GetContext(context) );
	::CGContextSaveGState( dc );
	::CGContextSelectFont( dc, GetName(), GetSize(), kCGEncodingMacRoman  );
	GetQuartzTextDims( s, inCharCount, outWidth, outHeight, &fooBaseLine, dc );
	::CGContextRestoreGState( dc );
#endif
}

// --------------------------------------------------------------
void GFontOSX::GetExtent( int c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	if (!c) {
		*outWidth  = 0;
		*outHeight = 0;
		return;
	}

#ifdef SMUFL
	CGGlyph glyphs[1];
	CGRect  boundingRects[1];
	UniChar ustr[1];
	ustr[0] = c;
	bool done = CTFontGetGlyphsForCharacters(fCTFont, ustr, glyphs, 1);
	if (!done)
		std::cerr << "GFontOSX::GetExtent error in CTFontGetGlyphsForCharacters with char '" << c << "'" << std::endl;
	CGRect rect = CTFontGetBoundingRectsForGlyphs (fCTFont, kCTFontOrientationHorizontal, glyphs, boundingRects, 1);

	*outWidth  = float(rect.size.width);
    *outHeight = float(mSize); // float(rect.size.height);

#else
	float fooBaseLine;	
	CGContextRef dc = (CGContextRef)( GetContext(context) );
	::CGContextSaveGState(dc);
	::CGContextSelectFont( dc, GetName(), GetSize(), kCGEncodingMacRoman  );
	const CGGlyph glyph = c;

	GetQuartzGlyphDims( &glyph, 1, outWidth, outHeight, &fooBaseLine, dc );
	if (c == 139) *outWidth = 4.0;		// ????????????????
	::CGContextRestoreGState( dc );
#endif
}

#if !defined (SMUFL) || !defined (JG_CTFONT_TEXT)
// --------------------------------------------------------------
// Q: How do I measure the width of my text before drawing it with Core Graphics?
// A: First call CGContextGetTextPosition to find the current text position. Then, set the 
// text drawing mode to kCGTextInvisible using CGContextSetTextDrawingMode and draw the text. 
// Finally, call CGContextGetTextPosition again to determine the final text position. Take 
// the difference between the starting and ending positions to find the width of your text.
// TODO: calculate exact baseline. Currently is only an approximation.
void GFontOSX::GetQuartzTextDims( const char * inString, int inCharCount, float * outWidth, 
							 float * outHeight, float * outBaseline, CGContextRef dc ) const
{
	CGPoint oldTextPos = ::CGContextGetTextPosition( dc );
	::CGContextSetTextDrawingMode( dc, kCGTextInvisible );


	::CGContextShowText( dc, inString, size_t(inCharCount));
	CGPoint newTextPos = ::CGContextGetTextPosition( dc );

	*outWidth = newTextPos.x - oldTextPos.x;
	
	// - Approximation
	*outHeight = (GetSize() /** GetYScale()*/);
	*outBaseline = (*outHeight /** GetYScale()*/ * 0.2f); // Approx

	//	cout << "guido quartz, GetQuartzTextDims width = " << (*outWidth) << " x " << (*outHeight) << endl;	// DEBUG
	
	// - Restore previous state
	::CGContextSetTextPosition( dc, oldTextPos.x, oldTextPos.y );
	::CGContextSetTextDrawingMode( dc, kCGTextFill );	// kCGTextFillStroke
}

// --------------------------------------------------------------
void GFontOSX::GetQuartzGlyphDims( const CGGlyph * inGlyphs, int inGlyphCount, float * outWidth,
							   float * outHeight, float * outBaseline, CGContextRef dc ) const
{
//	CGContextRef dc = GetDC();
	CGPoint oldTextPos = ::CGContextGetTextPosition( dc );
	::CGContextSetTextDrawingMode( dc, kCGTextInvisible );

	::CGContextShowGlyphs( dc, inGlyphs, (size_t)inGlyphCount );
	CGPoint newTextPos = ::CGContextGetTextPosition( dc );

	*outWidth = newTextPos.x - oldTextPos.x;
	
	// - Approximation
	*outHeight = GetSize();
	*outBaseline = (*outHeight * 0.2f); // Approx

	// - Restore previous state
	::CGContextSetTextPosition( dc, oldTextPos.x, oldTextPos.y );
	::CGContextSetTextDrawingMode( dc, kCGTextFill );	// kCGTextFillStroke
}
#endif
