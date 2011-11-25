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

#include "GFontOSX.h"

// --------------------------------------------------------------
GFontOSX::GFontOSX(const char * faceName, int size, int properties) 
			:  mName(faceName), mSize(size), mFontProp(properties)
{
}

// --------------------------------------------------------------
GFontOSX::~GFontOSX()
{}

// --------------------------------------------------------------
void GFontOSX::GetExtent( const char * s, int inCharCount, float * outWidth, 
					 float * outHeight, VGDevice * context ) const 
{
	if( s == 0 ) return;
	if( inCharCount < 0 )
		inCharCount = (int)strlen( s );

	float fooBaseLine;
	CGContextRef dc = (CGContextRef)( GetContext(context) );
	::CGContextSaveGState( dc );
	::CGContextSelectFont( dc, GetName(), GetSize(), kCGEncodingMacRoman  );
	GetQuartzTextDims( s, inCharCount, outWidth, outHeight, &fooBaseLine, dc );
	::CGContextRestoreGState( dc );
}

// --------------------------------------------------------------
void	
GFontOSX::GetExtent( unsigned char c, float * outWidth, float * outHeight, VGDevice * context ) const
{
	float fooBaseLine;	
	CGContextRef dc = (CGContextRef)( GetContext(context) );
	::CGContextSaveGState(dc);
	::CGContextSelectFont( dc, GetName(), GetSize(), kCGEncodingMacRoman  );
	const CGGlyph glyph = c;

	GetQuartzGlyphDims( &glyph, 1, outWidth, outHeight, &fooBaseLine, dc );
	if (c == 139) *outWidth = 4.0;		// ????????????????
	::CGContextRestoreGState( dc );
}

// --------------------------------------------------------------
// Q: How do I measure the width of my text before drawing it with Core Graphics?
// A: First call CGContextGetTextPosition to find the current text position. Then, set the 
// text drawing mode to kCGTextInvisible using CGContextSetTextDrawingMode and draw the text. 
// Finally, call CGContextGetTextPosition again to determine the final text position. Take 
// the difference between the starting and ending positions to find the width of your text.
// TODO: calculate exact baseline. Currently is only an approximation.
void			
GFontOSX::GetQuartzTextDims( const char * inString, int inCharCount, float * outWidth, 
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
void			
GFontOSX::GetQuartzGlyphDims( const CGGlyph * inGlyphs, int inGlyphCount, float * outWidth,
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

