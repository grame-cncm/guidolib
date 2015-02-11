/*
	GUIDO Library
	Copyright (C) 2010	Grame

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

#include <iostream>
#include <math.h>

#include "CairoDevice.h"
#include "VGFont.h"
#include "VGSystem.h"

// -------------------------------------------------------------------
#define c2cc(a)		(a/255.f)					// VGColor to Cairo color (char to double)
#define cc2c(a)		(unsigned char)(a*255)		// Cairo color to VGColor (double to char)


// -------------------------------------------------------------------
CairoDevice::CairoDevice(cairo_t * dev, VGSystem * sys) 
	: fNativeDevice(0), fSystem(sys), fTextFont(0), fMusicFont(0), fFillColor(0,0,0), fPenColor(0,0,0)
{
	fNativeDevice = cairo_reference (dev);
	SetFontColor( VGColor(0,0,0,ALPHA_OPAQUE) );
	SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	// transparent
	SetFontAlign( kAlignLeft | kAlignBase );
}

CairoDevice::CairoDevice(int w, int h, VGSystem * sys) 
	: fNativeDevice(0), fSystem(sys), fTextFont(0), fMusicFont(0), fFillColor(255,255,255), fPenColor(0,0,0)
{
	cairo_surface_t* surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, w, h);
	fNativeDevice = cairo_create (surface);
	cairo_surface_destroy (surface);	
	SetFontColor( VGColor(0,0,0,ALPHA_OPAQUE) );
	SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	// transparent
	SetFontAlign( kAlignLeft | kAlignBase );
}

CairoDevice::~CairoDevice() 
{
	if (fNativeDevice) cairo_destroy (fNativeDevice);
}


// -------------------------------------------------------------------
// - Drawing services -
// -------------------------------------------------------------------
bool CairoDevice::BeginDraw()	{ cairo_save   (fNativeDevice);  return true; }
void CairoDevice::EndDraw()		{ cairo_restore (fNativeDevice); }
void CairoDevice::InvalidateRect( float left, float top, float right, float bottom ) 
{
	cairo_surface_t* surface = cairo_get_target (fNativeDevice);
	cairo_surface_mark_dirty_rectangle (surface, left, top, right-left, bottom-top);
}

// -------------------------------------------------------------------
// - Standard graphic primitives -
// -------------------------------------------------------------------
void CairoDevice::MoveTo( float x, float y )	{ cairo_move_to (fNativeDevice, x, y); }

void CairoDevice::LineTo( float x, float y )	
{ 
	cairo_line_to (fNativeDevice, x, y); 
	cairo_set_source_rgba (fNativeDevice, c2cc(fPenColor.mRed), c2cc(fPenColor.mGreen), c2cc(fPenColor.mBlue), c2cc(fPenColor.mAlpha));
	cairo_stroke (fNativeDevice); 
}

void CairoDevice::Line( float x1, float y1, float x2, float y2 )	
{ 
	MoveTo(x1,y1); 
	LineTo(x2,y2); 
}

void CairoDevice::Frame( float left, float top, float right, float bottom )
{
	cairo_rectangle (fNativeDevice, left, top, right-left, bottom-top);
	cairo_set_source_rgba (fNativeDevice, c2cc(fPenColor.mRed), c2cc(fPenColor.mGreen), c2cc(fPenColor.mBlue), c2cc(fPenColor.mAlpha));
	cairo_stroke (fNativeDevice); 
}

// --------------------------------------------------------------
// Return an angle from 0 to 360
float CairoDevice::CoordToRadian( float x, float y )
{
	return (float)atan2( x, y );
}

void CairoDevice::Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY )
{
	const float midX = (left + right) * 0.5f;
	const float midY = (top + bottom) * 0.5f;
	const float width = right - left;
	const float height = bottom - top;
	
	// - Draw
	const float angle1 = CoordToRadian( startX - midX, startY - midY );		
	const float angle2 = CoordToRadian( endX - midX, endY - midY );
	cairo_save (fNativeDevice);
	cairo_translate (fNativeDevice, midX + width / 2., midY + height / 2.);
	cairo_scale (fNativeDevice, width / 2., height / 2.);
	cairo_arc (fNativeDevice, midX, midY, angle1, angle2, 0);
	cairo_restore (fNativeDevice);	
	cairo_set_source_rgba (fNativeDevice, c2cc(fPenColor.mRed), c2cc(fPenColor.mGreen), c2cc(fPenColor.mBlue), c2cc(fPenColor.mAlpha));
	cairo_stroke (fNativeDevice); 
}

// -------------------------------------------------------------------
// - Filled surfaces -
// The raster op mode for color filling should be specified with SetRasterOpMode() before using one of these.
// -------------------------------------------------------------------
void CairoDevice::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	cairo_move_to (fNativeDevice, x1, y1);
	cairo_line_to (fNativeDevice, x2, y2);
	cairo_line_to (fNativeDevice, x3, y3);
	cairo_line_to (fNativeDevice, x1, y1);
	cairo_set_source_rgba (fNativeDevice, c2cc(fPenColor.mRed), c2cc(fPenColor.mGreen), c2cc(fPenColor.mBlue), c2cc(fPenColor.mAlpha));
	cairo_stroke_preserve (fNativeDevice); 
	cairo_set_source_rgba (fNativeDevice, c2cc(fFillColor.mRed), c2cc(fFillColor.mGreen), c2cc(fFillColor.mBlue), c2cc(fFillColor.mAlpha));
	cairo_fill (fNativeDevice); 
}

void CairoDevice::Polygon( const float * xCoords, const float * yCoords, int count )
{
	if (count <= 2) return;
	cairo_move_to (fNativeDevice, xCoords[0], yCoords[0]);
	for (int i=1; i<count; i++)
		cairo_line_to (fNativeDevice, xCoords[i], yCoords[i]);
	cairo_line_to (fNativeDevice, xCoords[0], yCoords[0]);
	cairo_set_source_rgba (fNativeDevice, c2cc(fPenColor.mRed), c2cc(fPenColor.mGreen), c2cc(fPenColor.mBlue), c2cc(fPenColor.mAlpha));
	cairo_stroke_preserve (fNativeDevice); 
	cairo_set_source_rgba (fNativeDevice, c2cc(fFillColor.mRed), c2cc(fFillColor.mGreen), c2cc(fFillColor.mBlue), c2cc(fFillColor.mAlpha));
	cairo_fill (fNativeDevice); 
}

void CairoDevice::Rectangle( float left,  float top, float right, float bottom )
{
	cairo_rectangle (fNativeDevice, left, top, right-left, bottom-top);
	cairo_set_source_rgba (fNativeDevice, c2cc(fPenColor.mRed), c2cc(fPenColor.mGreen), c2cc(fPenColor.mBlue), c2cc(fPenColor.mAlpha));
	cairo_stroke_preserve (fNativeDevice); 
	cairo_set_source_rgba (fNativeDevice, c2cc(fFillColor.mRed), c2cc(fFillColor.mGreen), c2cc(fFillColor.mBlue), c2cc(fFillColor.mAlpha));
	cairo_fill (fNativeDevice); 
}

// -------------------------------------------------------------------
// - Pen & brush services -
// -------------------------------------------------------------------
void CairoDevice::SelectPen( const VGColor & color, float witdh ) 
{
	SelectPenColor (color);
	SelectPenWidth (witdh);
}
void CairoDevice::PushPen( const VGColor & color, float width )
{
	PushPenColor (color);
	PushPenWidth (width);
}
void CairoDevice::PopPen()
{
	PopPenColor ();
	PopPenWidth ();
}

// -------------------------------------------------------------------
void CairoDevice::SelectFillColor( const VGColor & color )					{ fFillColor  = color; }
void CairoDevice::PushFillColor( const VGColor & color )
{
	double r, g, b, a;
	cairo_pattern_get_rgba (cairo_get_source(fNativeDevice), &r, &g, &b, &a);	
	fFillColorStack.push (VGColor(cc2c(r), cc2c(g), cc2c(b), cc2c(a))); 
	SelectFillColor (color);
}
void CairoDevice::PopFillColor()
{
	SelectFillColor (fFillColorStack.top());
	fFillColorStack.pop();
}

// -------------------------------------------------------------------
void CairoDevice::SelectPenColor( const VGColor & color)					{ fPenColor = color; }
void CairoDevice::PushPenColor( const VGColor & color)
{
	double r, g, b, a;
	cairo_pattern_get_rgba (cairo_get_source(fNativeDevice), &r, &g, &b, &a);	
	fPenColorStack.push (VGColor(cc2c(r), cc2c(g), cc2c(b), cc2c(a))); 
	SelectPenColor (color);
}
void CairoDevice::PopPenColor()
{
	SelectPenColor (fPenColorStack.top());
	fPenColorStack.pop();
}

// -------------------------------------------------------------------
void CairoDevice::SelectPenWidth( float width)
{ 
	cairo_set_line_width (fNativeDevice, width); 
}
void CairoDevice::PushPenWidth( float width)
{
	fPenWidthStack.push( cairo_get_line_width(fNativeDevice) );
	SelectPenWidth( width );	
}
void CairoDevice::PopPenWidth()
{
	SelectPenWidth (fPenWidthStack.top());
	fPenWidthStack.pop();
}

void CairoDevice::SetRasterOpMode( VRasterOpMode ROpMode)		{}
VGDevice::VRasterOpMode	CairoDevice::GetRasterOpMode() const	{ return kOpCopy; }

// -------------------------------------------------------------------
// - Bitmap services (bit-block copy methods) -
// -------------------------------------------------------------------
bool CairoDevice::CopyPixels( VGDevice* pSrcDC, float alpha)
{
	cairo_t* src = (cairo_t*)pSrcDC->GetNativeContext();
	cairo_surface_t* surface = cairo_get_target (src);
	if ( cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE) {
		cairo_set_source_surface (fNativeDevice, surface, pSrcDC->GetWidth(), pSrcDC->GetHeight());
		cairo_paint (fNativeDevice);
	}
	return false;
}

bool CairoDevice::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC,
							  int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
	cairo_t* src = (cairo_t*)pSrcDC->GetNativeContext();
	cairo_surface_t* surface = cairo_get_target (src);
	if ( cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE) {
		cairo_save   (fNativeDevice);
		cairo_save   (src);
		SetOrigin (xDest, yDest);
		pSrcDC->SetOrigin (xSrc, ySrc);
		cairo_set_source_surface (fNativeDevice, surface, nSrcWidth, nSrcHeight);
		cairo_paint (fNativeDevice);
		cairo_restore (src);
		cairo_restore (fNativeDevice);
	}
	return false;
}

bool CairoDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha)
{
	return false;
}

bool CairoDevice::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC,
							  int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha)
{
	return false;
}

// -------------------------------------------------------------------
// - Coordinate services -
// -------------------------------------------------------------------
void CairoDevice::SetScale( float x, float y )		{ cairo_scale(fNativeDevice, x, y); }
void CairoDevice::OffsetOrigin( float x, float y )	{ cairo_translate(fNativeDevice, x, y); }
void CairoDevice::SetOrigin( float x, float y )	
{ 
	cairo_matrix_t matrix;
	cairo_get_matrix (fNativeDevice, &matrix);
	matrix.x0 = x;
	matrix.y0 = y;
	cairo_set_matrix (fNativeDevice, &matrix);
}

void CairoDevice::LogicalToDevice( float * x, float * y ) const	
{ 
	double dx, dy;
	cairo_user_to_device(fNativeDevice, &dx, &dy);
	*x = dx;
	*y = dy;
}
void CairoDevice::DeviceToLogical( float * x, float * y ) const	
{ 
	double dx, dy;
	cairo_device_to_user(fNativeDevice, &dx, &dy); 
	*x = dx;
	*y = dy;
}

float CairoDevice::GetXScale() const
{
	cairo_matrix_t matrix;
	cairo_get_matrix (fNativeDevice, &matrix);
	return matrix.xx;
}

float CairoDevice::GetYScale() const
{
	cairo_matrix_t matrix;
	cairo_get_matrix (fNativeDevice, &matrix);
	return matrix.yy;
}

float CairoDevice::GetXOrigin() const
{
	cairo_matrix_t matrix;
	cairo_get_matrix (fNativeDevice, &matrix);
	return matrix.x0;
}

float CairoDevice::GetYOrigin() const
{
	cairo_matrix_t matrix;
	cairo_get_matrix (fNativeDevice, &matrix);
	return matrix.y0;
}

void CairoDevice::NotifySize( int w, int h )
{
	cairo_surface_t* surface = cairo_get_target (fNativeDevice);
	if ( cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE) {
		cairo_surface_t* surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, w, h);
		if (fNativeDevice) cairo_destroy (fNativeDevice);
		fNativeDevice = cairo_create (surface);
		cairo_surface_destroy (surface);
	}
}

int CairoDevice::GetWidth() const
{
	cairo_surface_t* surface = cairo_get_target (fNativeDevice);
	if ( cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE)
		return cairo_image_surface_get_width (surface);
	return 1;
}
int CairoDevice::GetHeight() const
{
	cairo_surface_t* surface = cairo_get_target (fNativeDevice);
	if ( cairo_surface_get_type (surface) == CAIRO_SURFACE_TYPE_IMAGE)
		return cairo_image_surface_get_height (surface);
	return 1;
}


// -------------------------------------------------------------------
// - Font services -
// -------------------------------------------------------------------
void CairoDevice::SelectFont( const VGFont * font )
{
	cairo_font_slant_t slant = font->GetProperties() & VGFont::kFontItalic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL;
	cairo_font_weight_t weight = font->GetProperties() & VGFont::kFontBold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL;
	cairo_select_font_face (fNativeDevice, font->GetName(), slant, weight);
	cairo_set_font_size (fNativeDevice, font->GetSize());
}
void CairoDevice::SetMusicFont( const VGFont * font )
{
	SelectFont (font);
	fMusicFont = font;
}
void CairoDevice::SetTextFont( const VGFont * font )
{
	SelectFont (font);
	fTextFont = font;
}

// -------------------------------------------------------------------
// - Text and music symbols services -
// -------------------------------------------------------------------
const char* to_utf8 (int n, int& outcount)
{
	static char utf[5];
	if (n < 128) {
		utf[0] = n;
		utf[1] = 0;
		outcount = 1;
	}
	else if (n < 2048) {
		utf[0] = 192 + (n / 64);
		utf[1] = 128 + (n % 64);
		utf[2] = 0;
		outcount = 2;
	}
	else utf[0] = 0;		// unexpected
	return utf;
}

void CairoDevice::DrawMusicSymbol(float x, float y, unsigned int symbol )
{
	int n; const char* str = to_utf8(symbol,n);
	ShowText (fMusicFont, x, y, str, n);
}

void CairoDevice::DrawString( float x, float y, const char * s, int charCount )
{
	ShowText (fTextFont, x, y, s, charCount);
}

void CairoDevice::ShowText( const VGFont* font, float x, float y, const char * s, int charCount )
{
	// - Calculate string dimensions
	float w = 0, h = 0;
	float baseline = 0;
	font->GetExtent( s, charCount, &w, &h, this );

	// - Perform text alignement
	if( fTextAlign != ( kAlignLeft | kAlignBase )) {
		if( fTextAlign & kAlignBottom )	// Vertical align
			y -= baseline; 
		else if( fTextAlign & kAlignTop )
			y += h - baseline;

		if( fTextAlign & kAlignRight )	// Horizontal align
			x -= w;
		else if( fTextAlign & kAlignCenter )
			x -= (w * 0.5);
	}

	// - Draw text background
//	PushPen( fTextBackground, 1 );
//	PushFillColor( fTextBackground );
//	Rectangle( x, y, x + w, y - h/2 );
//	PopFillColor();
//	PopPen();

	cairo_set_source_rgba (fNativeDevice, c2cc(fTextColor.mRed), c2cc(fTextColor.mGreen), c2cc(fTextColor.mBlue), c2cc(fTextColor.mAlpha));
//std::cout << "CairoDevice::ShowText2 " << s << " x/y: " << x << "/" << y << std::endl;
	cairo_move_to (fNativeDevice, x, y);
	cairo_show_text(fNativeDevice, s);
	cairo_move_to (fNativeDevice, x+w, y);
}

void CairoDevice::SetFontColor( const VGColor & color )				{ fTextColor = color; }
VGColor CairoDevice::GetFontColor() const							{ return fTextColor; }
void CairoDevice::SetFontBackgroundColor( const VGColor & color )	{ fTextBackground = color; }
VGColor CairoDevice::GetFontBackgroundColor() const					{ return fTextBackground; }
void CairoDevice::SetFontAlign( unsigned int align )				{ fTextAlign = align; } 
unsigned int CairoDevice::GetFontAlign() const						{ return fTextAlign; }

// -------------------------------------------------------------------
void CairoDevice::SetDPITag( float inDPI )	{}
float CairoDevice::GetDPITag() const		{ return 72.0f; }
void* CairoDevice::GetBitMapPixels()		{ return 0; }
void CairoDevice::ReleaseBitMapPixels()		{}

