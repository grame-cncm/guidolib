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


#include "JuceDevice.h"
#include "JuceFont.h"

#include "JuceHeader.h"

using namespace juce;

// --------------------------------------------------------------
// static tools
static float CoordToRadian( float x, float y )		{ return (float)atan2( x, y ); }
static Colour Color2JColor (const VGColor & c)		{ return Colour (uint8(c.mRed), c.mGreen, c.mBlue, uint8(c.mAlpha) ); }

// --------------------------------------------------------------
void JuceDevice::initialize()
{
	fXPos = fYPos = 0;
	fXOrigin = fYOrigin = 0; 
	fLineThick = 1.0;
	fXScale = fYScale = 1.0f;
	fDPI = 300.f;
	fCurrentFont = 0;
	fFontAlign   = 0;
	fRasterOpMode = kOpCopy;
	fFillColorStack.push( VGColor(0,0,0) );
	fPenColorStack.push( VGColor(0,0,0) );
}

// --------------------------------------------------------------
JuceDevice::JuceDevice(Graphics * g, VGSystem* sys) 
		  : fGraphics (g), fTextFont(0), fMusicFont(0), fSystem(sys) 
{ 
	if (!g) {
		Image img (Image::ARGB, 10, 10, true);
		fGraphics = new Graphics(img);
		fFreeGraphics = true;
	}
	else fFreeGraphics = false;
	initialize(); 
}

// --------------------------------------------------------------
JuceDevice::JuceDevice(int width, int height, VGSystem* sys) 
		  : fGraphics (0), fTextFont(0), fMusicFont(0), fSystem(sys) 
{ 
	initialize(); 
	Image img (Image::ARGB, width, height, true);
	fGraphics = new Graphics(img);
	fFreeGraphics = true;
}

// --------------------------------------------------------------
JuceDevice::~JuceDevice()
{
	if (fFreeGraphics) delete fGraphics;
}

// - Drawing services ------------------------------------------------
// --------------------------------------------------------------
bool JuceDevice::BeginDraw()	{ 
	initialize ();
	fGraphics->saveState(); 
	return true;
}
void JuceDevice::EndDraw()		{ fGraphics->restoreState(); }
void JuceDevice::InvalidateRect( float /*left*/, float /*top*/, float /*right*/, float /*bottom*/ ) {}

// - Standard graphic primitives -------------------------
void JuceDevice::MoveTo( float x, float y )			{ fXPos = x; fYPos = y; }
void JuceDevice::LineTo( float x, float y )			
			{ fGraphics->drawLine (fXPos, fYPos, x, y); fXPos = x; fYPos = y; }
void JuceDevice::Line( float x1, float y1, float x2, float y2 )
			{ fGraphics->drawLine (x1, y1, x2, y2, fLineThick); }
void JuceDevice::Frame( float left, float top, float right, float bottom )
			{ fGraphics->drawRect (left, top, right-left, bottom-top, fLineThick); }

void JuceDevice::Arc( float left, float top, float right,  float bottom,
					  float startX, float startY, float endX, float endY ) 
{
	const float midX = (left + right) * 0.5f;
	const float midY = (top + bottom) * 0.5f;
	const float width = right - left;
	const float height = bottom - top;
	const float fromRadians = CoordToRadian( startX - midX, startY - midY );	
	const float toRadians = CoordToRadian( endX - midX, endY - midY );
	Path path;
	path.addArc	(left, top, width, height, fromRadians, toRadians, true);
	PathStrokeType st (fLineThick);
	fGraphics->strokePath (path, st);
}

void JuceDevice::FrameEllipse( float x, float y, float width, float height)
{
	float expand = 50.f;
	fGraphics->drawEllipse (x- (width + expand)/2, y-height*1.5 - expand/2, width+expand, height+expand, fLineThick);
}

void JuceDevice::Ellipse( float x, float y, float width, float height, const VGColor& color)
{
	PushFillColor (color);
	fGraphics->fillEllipse (x-width/2, y-height/2, width, height);
	PopFillColor();
}

// - Filled surfaces --------------------------------------
void JuceDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 ) 
{
	Path path;
	path.addTriangle (x1, y1, x2, y2, x3, y3);
	fGraphics->fillPath (path);
}

void JuceDevice::Polygon( const float * xCoords, const float * yCoords, int count ) 
{
	if (count < 2) return;
	
	Path path;
	path.startNewSubPath (xCoords[0], yCoords[0]);
	for (int i = 1; i < count; i++)
		path.lineTo (xCoords[i], yCoords[i]);
	path.closeSubPath ();
	fGraphics->fillPath (path);
}

void JuceDevice::Rectangle( float left,  float top, float right, float bottom )	
			{ fGraphics->fillRect (left, top, right-left, bottom-top); }

// - Pen & brush services --------------------------------------------
void JuceDevice::SelectPen( const VGColor & color, float witdh ) 
{
	SelectPenColor (color);
	SelectPenWidth (witdh);
}
void JuceDevice::PushPen( const VGColor & color, float width )
{
	PushPenColor (color);
	PushPenWidth (width);
}
void JuceDevice::PopPen()
{
	PopPenColor ();
	PopPenWidth ();
}

void JuceDevice::SelectFillColor( const VGColor & color )
{
	fFillColor = color;
	fGraphics->setColour (Color2JColor(color));
}
void JuceDevice::PushFillColor( const VGColor & color )
{
	fFillColorStack.push( color );
	SelectFillColor( color );
}
void JuceDevice::PopFillColor()
{
	fFillColorStack.pop();
	SelectFillColor( fFillColorStack.top() );
}


void JuceDevice::SetRasterOpMode( VRasterOpMode ROpMode)		{ fRasterOpMode = ROpMode; }
VGDevice::VRasterOpMode	JuceDevice::GetRasterOpMode() const		{ return fRasterOpMode; }

// - Bitmap services (bit-block copy methods) --------------------------
//>>>>>>>>>>>>>>>> todo
bool JuceDevice::CopyPixels( VGDevice* /*pSrcDC*/, float /*alpha*/)
{
	return false;
}

bool JuceDevice::CopyPixels( int /*xDest*/, int /*yDest*/, VGDevice* /*pSrcDC*/, int /*xSrc*/, int /*ySrc*/,
							 int /*srcWidth*/, int /*srcHeight*/, float /*alpha*/)
{
	return false;
}

bool JuceDevice::CopyPixels( int /*xDest*/, int /*yDest*/, int /*dstWidth*/, int /*dstHeight*/,
							 VGDevice* /*pSrcDC*/, float /*alpha*/)
{
	return false;
}

bool JuceDevice::CopyPixels( int /*xDest*/, int /*yDest*/, int /*dstWidth*/, int /*dstHeight*/,
							 VGDevice* /*pSrcDC*/, int /*xSrc*/, int /*ySrc*/,
							 int /*srcWidth*/, int /*srcHeight*/, float /*alpha*/)
{
	return false;
}
//>>>>>>>>>>>>>>>> todo


// - Coordinate services ------------------------------------------------
void JuceDevice::SetOrigin( float x, float y )	
{ 
	AffineTransform transform = AffineTransform::translation (x-fXOrigin, y-fYOrigin);
	fGraphics->addTransform (transform);
	fXOrigin = x; fYOrigin = y; 
}
void JuceDevice::OffsetOrigin( float x, float y )
{ 
	AffineTransform transform = AffineTransform::translation (x, y);
	fGraphics->addTransform (transform);
	fXOrigin += x; fYOrigin += y; 
}

float JuceDevice::GetXOrigin() const				{ return fXOrigin; }
float JuceDevice::GetYOrigin() const				{ return fYOrigin; }

void JuceDevice::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * fXScale - fXOrigin);
	*y = (*y * fYScale - fYOrigin);
}

void JuceDevice::DeviceToLogical( float * x, float * y ) const
{
	*x = ( *x + fXOrigin ) / fXScale;
	*y = ( *y + fYOrigin ) / fYScale;
}

void JuceDevice::SetScale( float x, float y )	
{ 
//	AffineTransform transform = AffineTransform::scale (x*1/fXScale, y*1/fYScale);
//	fGraphics->addTransform (transform);
	fGraphics->addTransform (AffineTransform::scale (x, y));
	fXScale = x;
	fYScale = y;
}
float JuceDevice::GetXScale() const				{ return fXScale; }
float JuceDevice::GetYScale() const				{ return fYScale; }

void JuceDevice::NotifySize( int width, int height ) { fWidth = width; fHeight = height; }
int JuceDevice::GetWidth() const				{ return fWidth; }
int JuceDevice::GetHeight() const				{ return fHeight; }


// - Font services ---------------------------------------------------
void JuceDevice::SetMusicFont( const VGFont * font )	{ 
	if (!font) return;
	fMusicFont = font;
	if (fCurrentFont != font) {
		fGraphics->setFont(static_cast<const JuceFont*>(font)->NativeFont()); 
		fCurrentFont = font;
	}
}
const VGFont *	JuceDevice::GetMusicFont() const		{ return fMusicFont; }
void JuceDevice::SetTextFont( const VGFont * font )		{ 
	if (!font) return;
	fTextFont = font;
	if (fCurrentFont != font) {
		fGraphics->setFont(static_cast<const JuceFont*>(font)->NativeFont());
		fCurrentFont = font;
	}
}
const VGFont *	JuceDevice::GetTextFont() const			{ return fTextFont; }

// - Text and music symbols services -------------------------------------
void JuceDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID ) 
{
	String text;
	text += wchar_t(inSymbolID);
	fGraphics->setColour (Color2JColor(fFontColor));
	fGraphics->drawSingleLineText (text, int(x), int(y));
	fGraphics->setColour (Color2JColor(fFillColor));
}

void JuceDevice::DrawString( float x, float y, const char * s, int inCharCount ) 
{
	float w, h; 
	fTextFont->GetExtent (s, inCharCount, &w, &h, this);
	if (fFontAlign & kAlignCenter)
		x -= w/2;
	else if (fFontAlign & kAlignRight)
		x -= w;

	String text (s, inCharCount);
	fGraphics->setColour (Color2JColor(fFontColor));
	fGraphics->drawSingleLineText (text, int(x), int(y));
	fGraphics->setColour (Color2JColor(fFillColor));
}

void JuceDevice::SetFontColor( const VGColor & c )			{ fFontColor = c; }
VGColor JuceDevice::GetFontColor() const					{ return fFontColor; }
void JuceDevice::SetFontBackgroundColor( const VGColor & c ){ fFontBackgroundColor = c; }
VGColor JuceDevice::GetFontBackgroundColor() const			{ return fFontBackgroundColor; }
void JuceDevice::SetFontAlign( unsigned int align )			{ fFontAlign = align; }
unsigned int JuceDevice::GetFontAlign() const				{ return fFontAlign; }

// - Printer informations services ----------------------------------------
void JuceDevice::SetDPITag( float inDPI )				{ fDPI = inDPI; }
float JuceDevice::GetDPITag() const						{ return fDPI; }

// - VGDevice extension --------------------------------------------
void JuceDevice::SelectPenColor( const VGColor & color)	{ fPenColor = color; }
void JuceDevice::PushPenColor( const VGColor & color)	{ fPenColor = color; fPenColorStack.push(color); }
void JuceDevice::PopPenColor()							{ fPenColorStack.pop(); fPenColor = fPenColorStack.top();  }

void JuceDevice::SelectPenWidth( float width)			{ fLineThick = width; }
void JuceDevice::PushPenWidth( float width)				{ fPenWidthStack.push(width); fLineThick = width; }
void JuceDevice::PopPenWidth()							{ fLineThick = fPenWidthStack.top(); fPenWidthStack.pop(); }

