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


#include "openFrameworksDevice.h"
#include "openFrameworksFont.h"
#include <ofMain.h>

// --------------------------------------------------------------
// static tools
static float CoordToRadian( float x, float y )		{ return (float)atan2( x, y ); }
static ofColor Color2ofColor (const VGColor & c)		{ return ofColor (c.mRed, c.mGreen, c.mBlue, c.mAlpha ); }

// --------------------------------------------------------------
void openFrameworksDevice::initialize()
{
	fXPos = fYPos = 0;
	fXOrigin = fYOrigin = 0; 
	fLineThick = 1.0;
	fXScale = fYScale = 1.0f;
	fDPI = 72.f;
	fCurrentFont = 0;
	fFontAlign   = 0;
	fRasterOpMode = kOpCopy;
	fFillColorStack.push( VGColor(0,0,0) );
	fPenColorStack.push( VGColor(0,0,0) );
}

// --------------------------------------------------------------
openFrameworksDevice::openFrameworksDevice(VGSystem* sys) 
		  : fTextFont(0), fMusicFont(0), fSystem(sys) 
{ 
	initialize(); 
}

// --------------------------------------------------------------
openFrameworksDevice::openFrameworksDevice(int width_, int height_, VGSystem* sys) 
		  : fTextFont(0), fMusicFont(0), fSystem(sys) 
{ 
	initialize(); 
	fWidth = width_;
	fHeight = height_;

	cout << "openFrameworksDevice: allocating FBO: " << fWidth << "x" << fHeight << endl;

	ofClear(255,255,255, 0);
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glClear (GL_COLOR_BUFFER_BIT);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable( GL_DEPTH_TEST );
	glEnable(GL_MULTISAMPLE_ARB);

	drawCache.allocate(fWidth, fHeight, GL_RGBA, 8);
	drawCache.begin();
	ofClear(255,255,255, 0);

	drawCache.end();
}

// --------------------------------------------------------------
openFrameworksDevice::~openFrameworksDevice()
{
}

// - Drawing services ------------------------------------------------
// --------------------------------------------------------------
bool openFrameworksDevice::BeginDraw()	{ 
	ofPushStyle();
	drawCache.begin();
	return true;
}
void openFrameworksDevice::EndDraw()		{ 
	drawCache.end();
	ofPopStyle(); 
}
void openFrameworksDevice::InvalidateRect( float /*left*/, float /*top*/, float /*right*/, float /*bottom*/ ) {}

// - Standard graphic primitives -------------------------
void openFrameworksDevice::MoveTo( float x, float y )			{ fXPos = x; fYPos = y; }
void openFrameworksDevice::LineTo( float x, float y ) {
	ofFill();
	ofLine(fXPos, fYPos, x, y);
	fXPos = x; fYPos = y;
}
void openFrameworksDevice::Line( float x1, float y1, float x2, float y2 ) {
	ofFill();
	ofLine(x1, y1, x2, y2);
}
void openFrameworksDevice::Frame( float left, float top, float right, float bottom ) {
	ofNoFill();
	ofSetLineWidth(fLineThick);
	ofRect (left, top, right-left, bottom-top);
}

void openFrameworksDevice::Arc( float left, float top, float right,  float bottom,
					  float startX, float startY, float endX, float endY ) 
{
	const float midX = (left + right) * 0.5f;
	const float midY = (top + bottom) * 0.5f;
	const float width = right - left;
	const float height = bottom - top;
	const float fromRadians = CoordToRadian( startX - midX, startY - midY );	
	const float toRadians = CoordToRadian( endX - midX, endY - midY );
	ofPath path;
	path.arc(left, top, width, height, fromRadians, toRadians, true);
	path.setFilled(true);
	path.draw();
}

// - Filled surfaces --------------------------------------
void openFrameworksDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 ) 
{
	ofFill();
	ofTriangle(x1, y1, 0, x2, y2, 0, x3, y3, 0);
}

void openFrameworksDevice::Polygon( const float * xCoords, const float * yCoords, int count ) 
{
	if (count < 2) return;
	
	ofPath path;
		
	path.newSubPath();
	path.moveTo(xCoords[0], yCoords[0]);
	for (int i = 1; i < count; i++)
		path.lineTo (xCoords[i], yCoords[i]);
	path.setFilled(true);
	path.draw();
}

void openFrameworksDevice::Rectangle( float left,  float top, float right, float bottom )	
{
	ofFill();
	ofRect(left, top, right-left, bottom-top);
}

// - Pen & brush services --------------------------------------------
void openFrameworksDevice::SelectPen( const VGColor & color, float witdh ) 
{
	ofSetColor (Color2ofColor(color));
	ofSetLineWidth (witdh);
}
void openFrameworksDevice::PushPen( const VGColor & color, float width )
{
	ofPushStyle();
	ofSetColor (Color2ofColor(color));
	ofSetLineWidth (width);
}
void openFrameworksDevice::PopPen()
{
	ofPopStyle();
}

void openFrameworksDevice::SelectFillColor( const VGColor & color )
{
	ofFill();
	fFillColor = color;
	ofSetColor (Color2ofColor(color));
}
void openFrameworksDevice::PushFillColor( const VGColor & color )
{
	fFillColorStack.push( color );
	SelectFillColor( color );
}
void openFrameworksDevice::PopFillColor()
{
	fFillColorStack.pop();
	SelectFillColor( fFillColorStack.top() );
}


void openFrameworksDevice::SetRasterOpMode( VRasterOpMode ROpMode)		{ fRasterOpMode = ROpMode; }
VGDevice::VRasterOpMode	openFrameworksDevice::GetRasterOpMode() const		{ return fRasterOpMode; }

// - Bitmap services (bit-block copy methods) --------------------------
//>>>>>>>>>>>>>>>> todo
bool openFrameworksDevice::CopyPixels( VGDevice* /*pSrcDC*/, float /*alpha*/)
{
	return false;
}

bool openFrameworksDevice::CopyPixels( int /*xDest*/, int /*yDest*/, VGDevice* /*pSrcDC*/, int /*xSrc*/, int /*ySrc*/,
							 int /*srcWidth*/, int /*srcHeight*/, float /*alpha*/)
{
	return false;
}

bool openFrameworksDevice::CopyPixels( int /*xDest*/, int /*yDest*/, int /*dstWidth*/, int /*dstHeight*/,
							 VGDevice* /*pSrcDC*/, float /*alpha*/)
{
	return false;
}

bool openFrameworksDevice::CopyPixels( int /*xDest*/, int /*yDest*/, int /*dstWidth*/, int /*dstHeight*/,
							 VGDevice* /*pSrcDC*/, int /*xSrc*/, int /*ySrc*/,
							 int /*srcWidth*/, int /*srcHeight*/, float /*alpha*/)
{
	return false;
}
//>>>>>>>>>>>>>>>> todo


// - Coordinate services ------------------------------------------------
void openFrameworksDevice::SetOrigin( float x, float y )	
{ 
	//cout << "openFrameworksDevice::SetOrigin:" << x << " " << y << endl;
	ofTranslate(x-fXOrigin, y-fYOrigin);
	ofTranslate(x, y);
	fXOrigin = x; fYOrigin = y; 
}
void openFrameworksDevice::OffsetOrigin( float x, float y )
{ 
	//cout << "openFrameworksDevice::OffsetOrigin" << x << " " << y <<  endl;
	ofTranslate(x, y);
	fXOrigin += x; fYOrigin += y; 
}

float openFrameworksDevice::GetXOrigin() const				{ return fXOrigin; }
float openFrameworksDevice::GetYOrigin() const				{ return fYOrigin; }

void openFrameworksDevice::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * fXScale - fXOrigin);
	*y = (*y * fYScale - fYOrigin);
}

void openFrameworksDevice::DeviceToLogical( float * x, float * y ) const
{
	*x = ( *x + fXOrigin ) / fXScale;
	*y = ( *y + fYOrigin ) / fYScale;
}

void openFrameworksDevice::SetScale( float x, float y )	
{ 
	//cout << "openFrameworksDevice::SetScale " << x << " " << y << endl;
	ofScale(x, y);
	fXScale = x;
	fYScale = y;
}
float openFrameworksDevice::GetXScale() const				{ return fXScale; }
float openFrameworksDevice::GetYScale() const				{ return fYScale; }

void openFrameworksDevice::NotifySize( int width, int height ) { 
	fWidth = width; fHeight = height;

	//cout << "openFrameworksDevice::NotifySize: allocating FBO: " << fWidth << "x" << fHeight << endl;
	drawCache.allocate(fWidth, fHeight, GL_RGBA, 8);
	drawCache.begin();
	ofClear(255,255,255, 0);
	drawCache.end();
}
int openFrameworksDevice::GetWidth() const				{ return fWidth; }
int openFrameworksDevice::GetHeight() const				{ return fHeight; }


// - Font services ---------------------------------------------------
void openFrameworksDevice::SetMusicFont( const VGFont * font )	{ 
	fMusicFont = font;
	if (fCurrentFont != font) {
		//fGraphics->setFont(static_cast<const openFrameworksFont*>(font)->NativeFont()); 
		fCurrentFont = font;
	}
}
const VGFont *	openFrameworksDevice::GetMusicFont() const		{ return fMusicFont; }
void openFrameworksDevice::SetTextFont( const VGFont * font )		{ 
	fTextFont = font; 
	if (fCurrentFont != font) {
		//fGraphics->setFont(static_cast<const openFrameworksFont*>(font)->NativeFont());
		fCurrentFont = font;
	}
}
const VGFont *	openFrameworksDevice::GetTextFont() const			{ return fTextFont; }

// - Text and music symbols services -------------------------------------
void openFrameworksDevice::DrawMusicSymbol(float x, float y, unsigned int inSymbolID ) 
{
	string text;
	text += wchar_t(inSymbolID);

	ofSetColor(Color2ofColor(fFontColor));
	ofTrueTypeFont* f = (ofTrueTypeFont*)(&static_cast<const openFrameworksFont*>(fCurrentFont)->NativeFont());
	f->drawString (text, int(x), int(y));
	ofSetColor( Color2ofColor(fFillColor));
}

void openFrameworksDevice::DrawString( float x, float y, const char * s, int inCharCount ) 
{
	float w, h; 
	fTextFont->GetExtent (s, inCharCount, &w, &h, this);
	if (fFontAlign & kAlignCenter)
		x -= w/2;
	else if (fFontAlign & kAlignRight)
		x -= w;

	string text (s, inCharCount);

	ofSetColor(Color2ofColor(fFontColor));
	ofTrueTypeFont* f = (ofTrueTypeFont*)(&static_cast<const openFrameworksFont*>(fCurrentFont)->NativeFont());
	f->drawString (text, int(x), int(y));
	ofSetColor( Color2ofColor(fFillColor));
}

void openFrameworksDevice::SetFontColor( const VGColor & c )			{ fFontColor = c; }
VGColor openFrameworksDevice::GetFontColor() const					{ return fFontColor; }
void openFrameworksDevice::SetFontBackgroundColor( const VGColor & c ){ fFontBackgroundColor = c; }
VGColor openFrameworksDevice::GetFontBackgroundColor() const			{ return fFontBackgroundColor; }
void openFrameworksDevice::SetFontAlign( unsigned int align )			{ fFontAlign = align; }
unsigned int openFrameworksDevice::GetFontAlign() const				{ return fFontAlign; }

// - Printer informations services ----------------------------------------
void openFrameworksDevice::SetDPITag( float inDPI )				{ fDPI = inDPI; }
float openFrameworksDevice::GetDPITag() const					{ return fDPI; }

// - VGDevice extension --------------------------------------------
void openFrameworksDevice::SelectPenColor( const VGColor & color)	{ fPenColor = color; }
void openFrameworksDevice::PushPenColor( const VGColor & color)	{ fPenColor = color; fPenColorStack.push(color); }
void openFrameworksDevice::PopPenColor()				{ fPenColorStack.pop(); fPenColor = fPenColorStack.top();  }

void openFrameworksDevice::SelectPenWidth( float width)			{ fLineThick = width; ofSetLineWidth(fLineThick);}
void openFrameworksDevice::PushPenWidth( float width)			{ fPenWidthStack.push(width); fLineThick = width; }
void openFrameworksDevice::PopPenWidth()				{ fLineThick = fPenWidthStack.top(); fPenWidthStack.pop(); }

