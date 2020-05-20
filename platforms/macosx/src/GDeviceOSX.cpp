/*
  GUIDO Library
	Copyright (C) 2003--2006  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/////////////////////////////////////////////////////////////////
///
/// 	MacOS X Quartz 2D implementation of VGDevice.
///
///	perfs (G3-350): Bach-Inv1.gmn: parse: 240ms, draw: 670ms
/////////////////////////////////////////////////////////////////
  
#include <iostream>
#include <assert.h>

#include <CoreText/CoreText.h>

using namespace std;

#include "GDeviceOSX.h"
#include "GFontOSX.h"
#include "GSystemOSX.h" // guido hack - must be removed asap

// --------------------------------------------------------------
GDeviceOSX::GDeviceOSX(int inWidth, int inHeight, VGSystem* sys)
{
	mPhysicalWidth = inWidth;
	mPhysicalHeight = inHeight;
	
	mCurrTextFont = NULL;
	mCurrMusicFont = NULL;
	mRasterMode = kOpCopy;
//	mSymbolMap = NULL; 
	mScaleX = mScaleY = 1;
	mOriginX = mOriginY = 0;

	
	mDPITag = 72.0f;

	// - Initial state of GDevice
	SetFontColor( VGColor(0,0,0,ALPHA_OPAQUE) );
	SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	// transparent
	SetFontAlign( kAlignLeft | kAlignBase );
	MoveTo(0,0);

	// - GDeviceOSX specific ------------------	
	mColorSpace = ::CGColorSpaceCreateDeviceRGB(); 
	
	// guido hack - must be removed asap
	mSys = sys; 

}

// --------------------------------------------------------------
// process Quartz Init
void GDeviceOSX::Init()
{
	::CGContextSetFillColorSpace(mContext, mColorSpace);
	::CGContextSetStrokeColorSpace(mContext, mColorSpace);

	// - Rendering quality
	::CGContextSetInterpolationQuality(mContext, kCGInterpolationDefault); // kCGInterpolationHigh kCGInterpolationLow 
	::CGContextSetShouldAntialias(mContext, true);
	
	// Quartz device coordinate system has the (0,0) point at (bottom,left) when we expect it to be (top,left) 
	// so we setup an appropriate transformation matrix
#ifdef _USE_QD_COORDINATES_
	::CGContextScaleCTM(mContext, 1, -1 );
	::CGContextTranslateCTM(mContext, 0, -GetHeight());
#endif

	// - Setup Text Servces
	CGAffineTransform m = { 1.f, 0.f, 0.f, -1.f, 0.f, 0.f };
	CGContextSetTextMatrix (mContext, m);
//	CGAffineTransform textCTM = ::CGContextGetTextMatrix(mContext);
//	::CGContextSetTextMatrix(mContext, ::CGAffineTransformScale( textCTM, 1, -1 ));
	::CGContextSetTextDrawingMode(mContext, kCGTextFill );	// kCGTextFillStroke
}

// --------------------------------------------------------------
GDeviceOSX::~GDeviceOSX()
{
	::CGColorSpaceRelease(mColorSpace);	
}

/////////////////////////////////////////////////////////////////
// - Drawing services -------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool GDeviceOSX::BeginDraw()
{
	// - save Quartz context -------------------
	::CGContextSaveGState(mContext);	

	// - save device's context - was SaveDC() --
	GState			s;
	s.pen			= mPen;
	s.fillColor		= mFillColor;
	s.textColor		= mTextColor;
	s.textBackColor = mTextBackColor;
	s.scaleX		= mScaleX;
	s.scaleY		= mScaleY;
	s.originX		= mOriginX;
	s.originY		= mOriginY;
	s.textAlign		= mTextAlign;
	s.currTextFont	= mCurrTextFont;
	s.currMusicFont = mCurrMusicFont;
	s.rasterMode	= mRasterMode;

	mStateStack.push( s );
	return true;
}

// --------------------------------------------------------------
void GDeviceOSX::EndDraw()
{
	// - restore Quartz context ------------------
	::CGContextRestoreGState(mContext); 

	// - restore device's state - was RestoreDC()
	GState & s = mStateStack.top();
	
	mPen			= s.pen;
	mFillColor		= s.fillColor;
	mTextColor		= s.textColor;	
	mTextBackColor	= s.textBackColor;	
	mScaleX			= s.scaleX;
	mScaleY			= s.scaleY;
	mOriginX		= s.originX;
	mOriginY		= s.originY;
	mTextAlign		= s.textAlign;
	mCurrTextFont	= s.currTextFont;
	mCurrMusicFont	= s.currMusicFont;
	mRasterMode		= s.rasterMode;
	
	mStateStack.pop();
}

// --------------------------------------------------------------
void GDeviceOSX::SetRasterOpMode( VRasterOpMode ROpMode)
{ 
	mRasterMode = ROpMode; 
	CGBlendMode mode;
	switch (mRasterMode) {
		case kOpCopy:		mode = kCGBlendModeNormal;
			break;
		case kOpAnd:		mode = kCGBlendModeMultiply;
			break;
		case kOpXOr:		mode = kCGBlendModeNormal;	// ‡ voir !!
			break;
		case kOpInvert:		mode = kCGBlendModeNormal;	// ‡ voir !!
			break;
		case kOpOr:			mode = kCGBlendModeNormal;	// ‡ voir !!
			break;
		default:
			mode = kCGBlendModeNormal;
	}
	::CGContextSetBlendMode(mContext, mode);
}

// --------------------------------------------------------------
void GDeviceOSX::MoveTo( float x, float y )
{
	mCurrPenPos.x = x;
	mCurrPenPos.y = y;
}

// --------------------------------------------------------------
void GDeviceOSX::LineTo( float x, float y )
{
	Line(mCurrPenPos.x, mCurrPenPos.y, x, y);
}

// --------------------------------------------------------------
void GDeviceOSX::Line( float x1, float y1, float x2, float y2 )
{
	::CGContextBeginPath(mContext);
	::CGContextMoveToPoint(mContext, x1, y1); 
	::CGContextAddLineToPoint(mContext, x2, y2);
	::CGContextStrokePath(mContext);

//	mCurrPenPos.x = x2;
//	mCurrPenPos.y = y2;
	MoveTo( x2,y2 );
}

// --------------------------------------------------------------
void GDeviceOSX::Frame( float left, float top, float right, float bottom )
{
	::CGContextBeginPath(mContext);
	::CGContextMoveToPoint(mContext, left, top); 
	::CGContextAddLineToPoint(mContext, left, bottom);
	::CGContextAddLineToPoint(mContext, right, bottom);
	::CGContextAddLineToPoint(mContext, right, top);
	::CGContextAddLineToPoint(mContext, left, top);
	::CGContextStrokePath(mContext);
	
	MoveTo( left,top );
}

// --------------------------------------------------------------
void GDeviceOSX::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY )
{
	// - Save current state
	::CGContextSaveGState(mContext);
	
	// - Preprocess
	const float midX = (left + right) * 0.5f;
	const float midY = (top + bottom) * 0.5f;
	const float width = right - left;
	const float height = bottom - top;

	// - CGContextAddArc does not support ellipses, so we need
	// 	 to scale the device context first.
	const float yScale = height / width;
	::CGContextScaleCTM(mContext, 1, yScale);
	
	// - Draw
	const float angle1 = CoordToDegree( startX - midX, startY - midY );		
	const float angle2 = CoordToDegree( endX - midX, endY - midY );

	//::CGContextAddArc(mContext, midX, midY, width, angle1, angle2, 0);
	::CGContextAddArc(mContext, midX, midY, width, angle1, angle2, 0);
	::CGContextClosePath(mContext);
	::CGContextStrokePath(mContext); 

	// - Restore State
	::CGContextRestoreGState(mContext);
}

// --------------------------------------------------------------
void GDeviceOSX::FrameEllipse( float x, float y, float width, float height)
{
	CGRect rect;
    rect.origin.x = x - width;
    rect.origin.y = y - height;
	rect.size.width = 2*width;
	rect.size.height = 2*height;
	::CGContextAddEllipseInRect(mContext, rect);
    ::CGContextStrokePath(mContext);
}

// --------------------------------------------------------------
void GDeviceOSX::Ellipse( float x, float y, float width, float height, const VGColor& color)
{
	CGRect rect;
    rect.origin.x = x - width;
    rect.origin.y = y - height;
	rect.size.width = 2*width;
	rect.size.height = 2*height;
	PushFillColor(color);
	::CGContextAddEllipseInRect(mContext, rect);
    ::CGContextStrokePath(mContext);
	PopFillColor();
}

// --------------------------------------------------------------
void GDeviceOSX::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	const float xCoords [] = { x1, x2, x3 };
	const float yCoords [] = { y1, y2, y3 };
	
	Polygon(xCoords, yCoords, 3); 
}

// --------------------------------------------------------------
void GDeviceOSX::Polygon( const float * xCoords, const float * yCoords, int count )
{
	::CGContextBeginPath(mContext );
	::CGContextMoveToPoint(mContext, xCoords[ count - 1 ], yCoords[ count - 1 ] ); 

	for( int index = 0; index < count; index ++ )
		::CGContextAddLineToPoint(mContext, xCoords[ index ], yCoords[ index ] );

	::CGContextClosePath(mContext);
//	::CGContextDrawPath(mContext, kCGPathFillStroke);
	::CGContextDrawPath(mContext, kCGPathFill);
}
// --------------------------------------------------------------
void GDeviceOSX::Rectangle( float left, float top, float right, float bottom )
{
	CGRect myRect = ::CGRectMake( left, top, right - left, bottom - top );

	::CGContextBeginPath(mContext);
	::CGContextAddRect(mContext, myRect );
//	::CGContextDrawPath(mContext, kCGPathFillStroke );
	::CGContextDrawPath(mContext, kCGPathFill );
}

/////////////////////////////////////////////////////////////////
// - Font services ----------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void GDeviceOSX::SetMusicFont( const VGFont * inObj )
{
	mCurrMusicFont = inObj;
}

// --------------------------------------------------------------
void GDeviceOSX::SetTextFont( const VGFont* inObj )
{
	if (inObj) {
		mCurrTextFont = inObj;
	}
}

/////////////////////////////////////////////////////////////////
// - Pen & brush services ---------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void GDeviceOSX::SelectPen( const VGColor & c, float width )
{
	SelectPenColor(c);
	SelectPenWidth(width);
/*
	mPen.Set(c, width);	
	float color[4];
	MakeCGColor(c, color);
	::CGContextSetStrokeColor(mContext, color);
	::CGContextSetLineWidth(mContext, width);
*/
}

void GDeviceOSX::SelectPenWidth( float width )
{
	mPen.Set(width);	
	::CGContextSetLineWidth(mContext, width);
}

void GDeviceOSX::SelectPenColor( const VGColor & c)
{
	mPen.Set(c);	
	CGFloat color[4];
	MakeCGColor(c, color);
	::CGContextSetStrokeColor(mContext, color);
}

// --------------------------------------------------------------
void GDeviceOSX::SelectFillColor( const VGColor & c )
{
	mFillColor.Set(c);
	
	CGFloat color[4];
	MakeCGColor(c, color);
	::CGContextSetFillColor(mContext, color);
}

// --------------------------------------------------------------
// Save the current pen, select the new one
void GDeviceOSX::PushPen( const VGColor & inColor, float inWidth )
{
	mPenStack.push( mPen );
	SelectPen( inColor, inWidth );
}

// --------------------------------------------------------------
// Restore the previous pen from the stack
void GDeviceOSX::PopPen()
{
	VGPen & pen = mPenStack.top();
	SelectPen( pen.mColor, pen.mWidth );
	mPenStack.pop();
}

// --------------------------------------------------------------
void GDeviceOSX::PushPenColor( const VGColor & inColor)
{
	mPenColorStack.push(mPen.mColor);
	SelectPenColor (inColor);
}

// --------------------------------------------------------------
void GDeviceOSX::PopPenColor()
{
	SelectPenColor(mPenColorStack.top());
	mPenColorStack.pop();
}

// --------------------------------------------------------------
void GDeviceOSX::PushPenWidth( float width)
{
	mPenWidthStack.push(mPen.mWidth);
	SelectPenWidth (width);
}

// --------------------------------------------------------------
void GDeviceOSX::PopPenWidth()
{
	SelectPenWidth(mPenWidthStack.top());
	mPenWidthStack.pop();
}

// --------------------------------------------------------------
// Save the current brush, select the new one
void
GDeviceOSX::PushFillColor( const VGColor & inColor )
{
	mBrushStack.push( mFillColor );
	if( inColor != mFillColor )
		SelectFillColor( inColor );
}

// --------------------------------------------------------------
// Restore the previous brush from the stack
void
GDeviceOSX::PopFillColor()
{
	VGColor & brush = mBrushStack.top();
	SelectFillColor( brush );
	mBrushStack.pop();
}

/////////////////////////////////////////////////////////////////
// - Bitmap services (bit-block copy methods) -------------------
/////////////////////////////////////////////////////////////////

// Quartz Doc : After drawing in a bitmap context, you can easily 
// transfer the bitmap image to another Quartz context of any type. 
// To maintain device independence, copying an image is a drawing 
// operation and not a blitting operation. There are two steps:

// - Create a Quartz image from the bitmap. In Mac OS X v10.4 and later, 
// you can use the function CGBitmapContextCreateImage.
// - Draw the image in the destination context using the function 
// CGContextDrawImage.
// --------------------------------------------------------------
bool
GDeviceOSX::CopyPixels( VGDevice* pSrcDC, float alpha) 
{ 
	return CopyPixels( int(mOriginX), int(mOriginY), pSrcDC, 
					   int(pSrcDC->GetXOrigin()), int(pSrcDC->GetYOrigin()),
					   pSrcDC->GetWidth(), pSrcDC->GetHeight(), alpha );
}

// --------------------------------------------------------------
bool
GDeviceOSX::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, 
						int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha )	
{ 
	GDeviceOSX* pSrcDC_OSX = (GDeviceOSX*)(pSrcDC);
	CGContextRef srcContext = (CGContextRef)( pSrcDC_OSX->GetNativeContext() );
	
	yDest = mPhysicalHeight - yDest;
	
	CGImageRef srcPixmap = ::CGBitmapContextCreateImage( srcContext );
	CGRect srcRect = ::CGRectMake( xSrc, ySrc, nSrcWidth, nSrcHeight );
	CGImageRef srcRectPixmap = ::CGImageCreateWithImageInRect( srcPixmap, srcRect );
	CGRect destRect = ::CGRectMake( xDest, yDest, nSrcWidth, -nSrcHeight );
	
	// We need to setup the desination context in Quartz default coordinate system before doing the copy
#ifdef _USE_QD_COORDINATES_
	::CGContextScaleCTM(mContext, 1, -1 );
	::CGContextTranslateCTM(mContext, 0, -GetHeight());
#endif
	::CGContextSetAlpha(mContext, alpha); 
	
	::CGContextDrawImage(mContext, destRect, srcRectPixmap);
	
#ifdef _USE_QD_COORDINATES_
	::CGContextScaleCTM(mContext, 1, -1 );
	::CGContextTranslateCTM(mContext, 0, -GetHeight());
#endif
	::CGContextSetAlpha(mContext, 1.0); 
	
	::CGImageRelease(srcPixmap);
	::CGImageRelease(srcRectPixmap);
	return true; 
}

// --------------------------------------------------------------
bool
GDeviceOSX::CopyPixels( int xDest, int yDest, 
						int dstWidth, int dstHeight, 
						VGDevice* pSrcDC, float alpha ) 
{ 
	GDeviceOSX* pSrcDC_OSX = (GDeviceOSX*)(pSrcDC);
	
	yDest = mPhysicalHeight - yDest;

	CGContextRef srcContext = (CGContextRef)( pSrcDC_OSX->GetNativeContext() );
	CGImageRef srcPixmap = ::CGBitmapContextCreateImage( srcContext );
	CGRect destRect = ::CGRectMake( xDest, yDest, dstWidth, -dstHeight );
	
	// We need to setup the desination context in Quartz default coordinate system before doing the copy
#ifdef _USE_QD_COORDINATES_
	::CGContextScaleCTM(mContext, 1, -1 );
	::CGContextTranslateCTM(mContext, 0, -GetHeight());
#endif
	::CGContextSetAlpha(mContext, alpha); 
	
	::CGContextDrawImage (mContext, destRect, srcPixmap);
	
#ifdef _USE_QD_COORDINATES_
	::CGContextScaleCTM(mContext, 1, -1 );
	::CGContextTranslateCTM(mContext, 0, -GetHeight());
#endif
	::CGContextSetAlpha(mContext, 1.0); 
	
	::CGImageRelease(srcPixmap);
	return true; 
}

// --------------------------------------------------------------
bool
GDeviceOSX::CopyPixels( int xDest, int yDest, 
						int dstWidth, int dstHeight, 
						VGDevice* pSrcDC, 
						int xSrc, int ySrc, 
						int nSrcWidth, int nSrcHeight, float alpha) 
{ 
	return false; 
} 

/////////////////////////////////////////////////////////////////
// - Coordinate services ----------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceOSX::SetScale( float x, float y )
{
	mScaleX *= x;
	mScaleY *= y;
	
	// - GDeviceOSX specific ------------------
	::CGContextScaleCTM(mContext, (x ), (y ));
}

// --------------------------------------------------------------
// Called by: GRPage, GRStaff, GRSystem
void				
GDeviceOSX::SetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX = x;
	mOriginY = y;

	// - GDeviceOSX specific / was DoSetOrigin -----------------
	::CGContextTranslateCTM(mContext, (x - prevX), (y - prevY));
}

// --------------------------------------------------------------
// Called by: GRPage, GRStaff, GRSystem
void				
GDeviceOSX::OffsetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX += x;
	mOriginY += y;

	// - GDeviceOSX specific / was DoSetOrigin -----------------
	::CGContextTranslateCTM(mContext, (mOriginX - prevX), (mOriginY - prevY));
}

// --------------------------------------------------------------
// Called by:
// GRMusic::OnDraw
// GRPage::getPixelSize
void			
GDeviceOSX::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * mScaleX - mOriginX);
	*y = (*y * mScaleY - mOriginY);
}

// --------------------------------------------------------------
// Called by:
// GRPage::DPtoLPRect
// GRPage::getVirtualSize
void			
GDeviceOSX::DeviceToLogical( float * x, float * y ) const	
{
	*x = ( *x + mOriginX ) / mScaleX;
	*y = ( *y + mOriginY ) / mScaleY;
}

/////////////////////////////////////////////////////////////////
// - Text and music symbols services ----------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void GDeviceOSX::DrawMusicSymbol( float x, float y, unsigned int inSymbolID )
{
	GFontOSX * macFont = (GFontOSX *)mCurrMusicFont;
	assert(macFont);
	CGGlyph glyph = macFont->GetGlyph( inSymbolID );

	// - Calculate string dimensions		
	// - Perform text alignement. TODO: use precalculated values.
	if( mTextAlign != kAlignBaseLeft )
	{	
		float w = 0;
		float h = 0;
		float baseline = 0;
        mCurrMusicFont->GetExtent( inSymbolID, &w, &h, this );
		if( mTextAlign & kAlignBottom )	// Vertical align
			y -= baseline; 
		else if( mTextAlign & kAlignTop )
			y += h - baseline;

		if( mTextAlign & kAlignRight )	// Horizontal align
			x -= w;
		else if( mTextAlign & kAlignCenter )
			x -= (w * float(0.5));
	}

    CGPoint pointInUserCoordinates = CGPointMake(x, y);
    CGAffineTransform textMatrix = CGContextGetTextMatrix(mContext);
    CGAffineTransform inverse = CGAffineTransformInvert(textMatrix);
    CGPoint pointInTextCoordinates = CGPointApplyAffineTransform(pointInUserCoordinates, inverse);

	// - Draw text
	PushFillColor( VGColor(mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue, mTextColor.mAlpha) );
    CTFontDrawGlyphs(macFont->GetCTFont(), &glyph, &pointInTextCoordinates, 1, mContext);
	PopFillColor();
}

void GDeviceOSX::DrawString( float x, float y, const char * s, int inCharCount )
{
    // - Manage character encoding
    const char * convStr = s;
    if( convStr == 0 ) return;
    
    // Create an attributed string
    GFontOSX *font = (GFontOSX *)mCurrTextFont;
    CFDictionaryRef attributes = font->GetCTFontDictionary();
    CFStringRef string = CFStringCreateWithBytes(NULL, (const UInt8 *)s, inCharCount, kCFStringEncodingUTF8, false);
    CFAttributedStringRef attributedOverlayText = CFAttributedStringCreate(kCFAllocatorDefault, string, attributes);

    ///Create framesetter with the attributed text
    CTFramesetterRef framesetter = CTFramesetterCreateWithAttributedString(attributedOverlayText);
    CGSize suggestedSize = CTFramesetterSuggestFrameSizeWithConstraints(framesetter, /* Framesetter */
                                                                        CFRangeMake(0, 0), /* String range (entire string) */
                                                                        NULL, /* Frame attributes */
                                                                        CGSizeMake(CGFLOAT_MAX, CGFLOAT_MAX), /* Constraints (CGFLOAT_MAX indicates unconstrained) */
                                                                        NULL /* Gives the range of string that fits into the constraints, doesn't matter in your situation */
                                                                        );
    CFRelease(attributedOverlayText);
    CFRelease(framesetter);
    
    /// Set text alignment (paragraph style)
    CTTextAlignment alignment = kCTTextAlignmentLeft;
    
    // - Perform text alignement
    float baseline = 0.0, h = suggestedSize.height, w = suggestedSize.width;

    if( mTextAlign != ( kAlignLeft | kAlignBase )) {
        if( mTextAlign & kAlignBottom )	// Vertical align
            y -= baseline;      // was y -= baseline;
        else if( mTextAlign & kAlignTop )
            y += 0.5*h - baseline;    // was  y += h - baseline
        
        if( mTextAlign & kAlignRight )	// Horizontal align
        {
            alignment = kCTTextAlignmentRight;
            x -= w;
        }
        else if( mTextAlign & kAlignCenter )
        {
            alignment = kCTTextAlignmentCenter;
            x -= (w * 0.5);
        }else if( mTextAlign & kAlignLeft )
        {
            alignment = kCTTextAlignmentLeft;
        }
    }

    // Second pass of frame setting with paragraph style.

    CTParagraphStyleSetting settings[] = {kCTParagraphStyleSpecifierAlignment, sizeof(alignment), &alignment};
    CTParagraphStyleRef paragraphStyle = CTParagraphStyleCreate(settings, sizeof(settings) / sizeof(settings[0]));
    CFMutableDictionaryRef mutableAttributes = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 2, attributes);
    CFDictionarySetValue(mutableAttributes, kCTParagraphStyleAttributeName, paragraphStyle);
    attributedOverlayText = CFAttributedStringCreate(kCFAllocatorDefault, string, mutableAttributes);
    framesetter = CTFramesetterCreateWithAttributedString(attributedOverlayText);

    // Core Text changes the state of the context, so save it
    CGContextSaveGState(mContext);
    
    /// Draw attributed text using CTFrameDraw
    CGPathRef path = CGPathCreateWithRect(CGRectMake(x, y, suggestedSize.width, suggestedSize.height), NULL);
    CTFrameRef frame = CTFramesetterCreateFrame(framesetter, CFRangeMake(0, 0), path, NULL);
    CTFrameDraw(frame, mContext);
    CFRelease(frame);
    CFRelease(path);
    CFRelease(framesetter);
    CFRelease(attributedOverlayText);
    CFRelease(mutableAttributes);
    CFRelease(paragraphStyle);

    // Restore the state of the contexte
    CGContextRestoreGState(mContext);
}

// --------------------------------------------------------------
void *			
GDeviceOSX::GetNativeContext() const
{
	return mContext; 
}

// --------------------------------------------------------------
void			
GDeviceOSX::MakeCGColor( const VGColor & inColor, CGFloat outColor[ 4 ] )
{
	const float conv = 1.0f / 255.0f;

	outColor[ 0 ] = float(inColor.mRed) * conv;
	outColor[ 1 ] = float(inColor.mGreen) * conv;
	outColor[ 2 ] = float(inColor.mBlue) * conv;
	outColor[ 3 ] = float(inColor.mAlpha) * conv;	
}

// --------------------------------------------------------------
// Return an angle from 0 to 360
float 
GDeviceOSX::CoordToDegree( float x, float y )
{
	const float kRadToDeg = 180 / 3.14159265359f;
	float outAngle = (float)atan2( x, y ) * kRadToDeg;
	if( outAngle < 0 )
		outAngle += 360;

	return outAngle;
}

