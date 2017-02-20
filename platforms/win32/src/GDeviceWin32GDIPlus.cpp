/*
	GUIDO Library
	Copyright (C) 2003--2006  Grame

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

#include "GDeviceWin32GDIPlus.h"
#include "GFontWin32GDIPlus.h"

#include "wingdi.h"	//for AlphaBlend in copyPixels
#pragma warning (disable : 4996 4800)	// for _CRT_SECURE_NO_DEPRECATE

#include <iostream>
using namespace std;

// --------------------------------------------------------------
// constructor for display device
GDeviceWin32GDIPlus::GDeviceWin32GDIPlus( HDC hdc, VGSystem* sys ) 
				:	mWinOriginX(0),	mWinOriginY(0),
					mCurrTextFont(NULL), mCurrMusicFont(NULL),
					mPtCount(0), mPtArray(NULL), 
					mRasterMode(kOpCopy), 				
					mScaleX(1), mScaleY(1),
					mOriginX(0), mOriginY(0), mDPITag(72.0f) 
{ 
	mCurrPenPosX = mCurrPenPosY = 0;

	// guido hack - must be removed asap
	mSys = sys; 
	mBitmap						= 0;
	mGraphics					= new Graphics(hdc);
	mPen1						= new Pen(Color::Black, 1);
	mFillColor1					= new SolidBrush(Color(ALPHA_OPAQUE, 255, 0, 0));
	mBitmapData					= 0;

	mPhysicalWidth  = ::GetDeviceCaps( hdc, HORZRES );
	mPhysicalHeight = ::GetDeviceCaps( hdc, VERTRES );
	MoveTo( 0, 0 );

	// init device's members
	SelectPen(VGColor(0,0,0,ALPHA_OPAQUE), 1);					// opaque black
	SelectFillColor(VGColor(0,0,0,ALPHA_OPAQUE));				// opaque black

	SetFontColor(VGColor(0,0,0,ALPHA_OPAQUE));					// opaque black
	SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	// transparent white
	SetFontAlign( kAlignLeft | kAlignBase );

//	mGraphics->DrawLine(mPen1, 0, 0, 1000, 1000);   ???
}

// --------------------------------------------------------------
// constructor for memory device
GDeviceWin32GDIPlus::GDeviceWin32GDIPlus( int inWidth, int inHeight, VGSystem* sys ) 
				:	mWinOriginX(0),	mWinOriginY(0),
					mCurrTextFont(NULL), mCurrMusicFont(NULL),
					mPtCount(0), mPtArray(NULL),				
					mScaleX(1), mScaleY(1),
					mOriginX(0), mOriginY(0), mPhysicalWidth(inWidth),
					mPhysicalHeight(inHeight), mDPITag(72), mRasterMode(kOpCopy)
{	
	// create default PixelFormat32bppARGB bitmap
	mBitmap				= new Bitmap(inWidth, inHeight, PixelFormat32bppPARGB);
	mGraphics			= new Graphics(mBitmap);
	mPen1				= new Pen(Color::Black, 1);
	mFillColor1			= new SolidBrush(Color(ALPHA_OPAQUE, 255, 0, 0));
	mBitmapData			= new BitmapData;
	MoveTo( 0, 0 );

	mSys = sys; 

	SetFontColor(VGColor(0,0,0,ALPHA_OPAQUE));					// opaque black
	SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	// transparent white
	SetFontAlign( kAlignLeft | kAlignBase );
}

// --------------------------------------------------------------
// Constructor for memory device based on a image file
// The graphics file formats supported by GDI+ are BMP, GIF, JPEG, 
// PNG, TIFF, Exif, WMF, and EMF. 
GDeviceWin32GDIPlus::GDeviceWin32GDIPlus( const char * inPath, VGSystem* sys ) 
				:	mWinOriginX(0),	mWinOriginY(0),
					mCurrTextFont(NULL), mCurrMusicFont(NULL),
					mPtCount(0), mPtArray(NULL),				
					mScaleX(1), mScaleY(1),
					mOriginX(0), mOriginY(0), mDPITag(72), 
					mRasterMode(kOpCopy)
{ 
	//translate image path	
	//std::string s(inPath);
	//std::wstring wPath(s.length(),L' ');
	//std::copy(s.begin(), s.end(), wPath.begin());

	int charCount = (int)strlen( inPath );
	DWORD count = (DWORD)mbstowcs(NULL, inPath, charCount);
	WCHAR * wPath = (WCHAR*) malloc ((count + 1) * sizeof(WCHAR));
	mbstowcs(wPath, inPath, charCount+1);

	// create default PixelFormat32bppARGB bitmap
	mBitmap				= new Bitmap(wPath);
	mGraphics			= new Graphics(mBitmap);
	mPen1				= new Pen(Color::Black, 1);
	mFillColor1			= new SolidBrush(Color(ALPHA_OPAQUE, 255, 0, 0));
	mBitmapData			= new BitmapData;
	MoveTo( 0, 0 );

	mSys = sys; 

	SetFontColor(VGColor(0,0,0,ALPHA_OPAQUE));					// opaque black
	SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	// transparent white
	SetFontAlign( kAlignLeft | kAlignBase );

	mPhysicalWidth = mBitmap->GetWidth();
	mPhysicalHeight = mBitmap->GetHeight();
	free(wPath);
}

// --------------------------------------------------------------
GDeviceWin32GDIPlus::~GDeviceWin32GDIPlus()
{
	if (mBitmapData)	delete mBitmapData;
	delete mFillColor1;
	delete mPen1;
	delete mGraphics;
	if (mBitmap)		delete mBitmap;
}

// --------------------------------------------------------------
bool
GDeviceWin32GDIPlus::IsValid() const
{ 
	return mGraphics != 0;
}

/////////////////////////////////////////////////////////////////
// - Drawing services -------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool
GDeviceWin32GDIPlus::BeginDraw()
{
	// save Gdi+ context
	mGState = mGraphics->Save();

	// save device's state - was VGDevice::SaveDC()
	GState			s;
	s.pen			= mPen;
	s.fillColor		= mFillColor;
	s.textColor		= mTextColor;
	s.textBackColor = mTextBackColor;
	s.scaleX		= mScaleX;
	s.scaleY		= mScaleY;
	s.originX		= mOriginX;
	s.originY		= mOriginY;
	s.WoriginX		= mWinOriginX;
	s.WoriginY		= mWinOriginY;
	s.textAlign		= mTextAlign;
	s.currTextFont	= mCurrTextFont;
	s.currMusicFont = mCurrMusicFont;
	s.rasterMode	= mRasterMode;

	s.currPenPosX	= mCurrPenPosX;	
	s.currPenPosY	= mCurrPenPosY;	

	mStateStack.push( s );

	// Anti-aliasing must be turned on to ensure correct 
	// rendering of strings using the Font object called "Times" (gdi+ bug).
	mGraphics->SetTextRenderingHint(TextRenderingHintAntiAlias);
	mGraphics->SetSmoothingMode(SmoothingModeHighQuality);

	return true;
}
// --------------------------------------------------------------
void
GDeviceWin32GDIPlus::EndDraw()
{
	// restore Gdi+ context
	mGraphics->Restore(mGState);

	// general device's state restore - was RestoreDC()
	GState & s = mStateStack.top();
	
	mPen			= s.pen;
	mFillColor		= s.fillColor;
	mTextColor		= s.textColor;	
	mTextBackColor	= s.textBackColor;	
	mScaleX			= s.scaleX;
	mScaleY			= s.scaleY;
	mOriginX		= s.originX;
	mOriginY		= s.originY;
	mWinOriginX		= s.WoriginX;
	mWinOriginY		= s.WoriginY;
	mTextAlign		= s.textAlign;
	mCurrTextFont	= s.currTextFont;
	mCurrMusicFont	= s.currMusicFont;
	mRasterMode		= s.rasterMode;

	mCurrPenPosX	= s.currPenPosX;	
	mCurrPenPosY	= s.currPenPosY;	

	mStateStack.pop();
}

// --------------------------------------------------------------
void 
GDeviceWin32GDIPlus::MoveTo( float x, float y ) 
{
	mCurrPenPosX = x;
	mCurrPenPosY = y;
}

// --------------------------------------------------------------
void 
GDeviceWin32GDIPlus::LineTo( float x, float y )
{
	mGraphics->DrawLine(mPen1, mCurrPenPosX, mCurrPenPosY, x, y);
	MoveTo( x, y ); 
}

// --------------------------------------------------------------
void 
GDeviceWin32GDIPlus::Line( float x1, float y1, float x2, float y2 )
{
	mGraphics->DrawLine(mPen1, x1, y1, x2, y2);
	MoveTo( x2, y2 ); 
}

// --------------------------------------------------------------
void 
GDeviceWin32GDIPlus::Frame( float left, float top, float right, float bottom )
{
	mGraphics->DrawRectangle(mPen1, left, top, right - left, bottom - top);
	MoveTo( left, top ); 
}

// --------------------------------------------------------------
void		
GDeviceWin32GDIPlus::Arc( float left, float top, float right, float bottom, 
						  float startX, float startY, float endX, float endY )
{ 
  // Set up the arc.
   REAL midX = (left + right) * 0.5f;
   REAL midY = (top + bottom) * 0.5f;
   REAL startAngle = CoordToDegree( startX - midX, startY - midY );
   REAL sweepAngle = CoordToDegree( endX - midX, endY - midY );
   mGraphics->DrawArc(mPen1, left, top, right, bottom, startAngle, sweepAngle);
}

// --------------------------------------------------------------
void			
GDeviceWin32GDIPlus::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	const float xCoords [] = { x1, x2, x3 };
	const float yCoords [] = { y1, y2, y3 };
	Polygon( xCoords, yCoords, 3 );	
}
// --------------------------------------------------------------
void
GDeviceWin32GDIPlus::Polygon( const float * xCoords, const float * yCoords, int count ) 
{ 
	if( count > mPtCount )
	{
		delete [] mPtArray;
		mPtArray = new Point [ count ];
		if( mPtArray )
			mPtCount = count;
		else
			return;
	}

	for( int index = 0; index < count; index ++ )
	{ 
		mPtArray[ index ].X = RInt( xCoords[ index ] );
		mPtArray[ index ].Y = RInt( yCoords[ index ] );
	}
	mGraphics->FillPolygon( mFillColor1, mPtArray, count ); 
}	

// --------------------------------------------------------------
void 
GDeviceWin32GDIPlus::Rectangle( float left, float top, float right, float bottom )
{
	// warning ! hack : w & h are 1 pixel less to match gdi behaviour
    mGraphics->FillRectangle(mFillColor1, left, top, right - left - 1, bottom - top - 1);
}

/////////////////////////////////////////////////////////////////
// - Font services ----------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void GDeviceWin32GDIPlus::SetMusicFont( const VGFont * inObj )	{ mCurrMusicFont = inObj; }
const VGFont * GDeviceWin32GDIPlus::GetMusicFont() const		{ return mCurrMusicFont; }
void GDeviceWin32GDIPlus::SetTextFont( const VGFont * inObj )	{ mCurrTextFont = inObj; }
const VGFont *	GDeviceWin32GDIPlus::GetTextFont() const		{ return mCurrTextFont; }

// --------------------------------------------------------------
// - PEN & BRUSH SERVICES 
// --------------------------------------------------------------
void			
GDeviceWin32GDIPlus::SelectPen( const VGColor & c, float width )
{
	mPen.Set( c, width );
	mPen1->SetColor(Color(c.mAlpha, c.mRed, c.mGreen, c.mBlue));
	mPen1->SetWidth(width);
}

void GDeviceWin32GDIPlus::SelectPenWidth( float width )
{
	//don't re-create pen if same params.
	if (mPen.mWidth == width) return;
	mPen1->SetWidth(width);
}

void GDeviceWin32GDIPlus::SelectPenColor( const VGColor & c )
{
	//don't re-create pen if same params.
	if (mPen.mColor == c) return;
	mPen1->SetColor(Color(c.mAlpha, c.mRed, c.mGreen, c.mBlue));
}

// --------------------------------------------------------------
void			
GDeviceWin32GDIPlus::SelectFillColor( const VGColor & c )
{
	mFillColor.Set( c );
	mFillColor1->SetColor(Color(c.mAlpha, c.mRed, c.mGreen, c.mBlue));
}

// --------------------------------------------------------------
// Save the current pen, select the new one
void
GDeviceWin32GDIPlus::PushPen( const VGColor & inColor, float inWidth )
{
	VGPen tmp;
	tmp.Set(inColor, inWidth); // Pen object cannot be pushed on a vector....
	mPenStack.push( tmp );
	SelectPen( inColor, inWidth );
}

// --------------------------------------------------------------
// Restore the previous pen from the stack
void
GDeviceWin32GDIPlus::PopPen()
{
	VGPen& pen = mPenStack.top();	
	SelectPen(pen.mColor, pen.mWidth);
	mPenStack.pop();
}


void GDeviceWin32GDIPlus::PushPenColor( const VGColor & inColor)
{
	mPenColorStack.push( mPen.mColor );
	SelectPenColor (inColor);
}
void GDeviceWin32GDIPlus::PopPenColor()
{
	SelectPenColor (mPenColorStack.top());
	mPenColorStack.pop();
}
		
void GDeviceWin32GDIPlus::PushPenWidth( float width)
{
	mPenWidthStack.push( mPen.mWidth );
	SelectPenWidth (width);
}
void GDeviceWin32GDIPlus::PopPenWidth()
{
	SelectPenWidth (mPenWidthStack.top());
	mPenWidthStack.pop();
}

// --------------------------------------------------------------
// Save the current brush, select the new one
void
GDeviceWin32GDIPlus::PushFillColor( const VGColor & inColor )
{
	mBrushStack.push( mFillColor );
	SelectFillColor( inColor );
}

// --------------------------------------------------------------
// Restore the previous brush from the stack
void
GDeviceWin32GDIPlus::PopFillColor()
{
	VGColor& brush = mBrushStack.top();
	SelectFillColor( brush );
	mBrushStack.pop();
}

// --------------------------------------------------------------
void			
GDeviceWin32GDIPlus::SetRasterOpMode( VRasterOpMode ROpMode )  
{ 
	mRasterMode = ROpMode; 
} 

/////////////////////////////////////////////////////////////////
// - Bitmap services (bit-block copy methods) -------------------
// 
// These methods don't use GDI+ standard way to display/copy 
// bitmap pixels (ie DrawImage) because of its well-known poor 
// performances (using ColorMatrix and DrawImage to mask source 
// alpha and use the input one). Instead, we use the GDI way which 
// uses AlphaBlend and works well. 
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool
GDeviceWin32GDIPlus::CopyPixels( VGDevice* pSrcDC, float alpha )
{	
	return CopyPixels(int(mOriginX), int(mOriginY), pSrcDC, 
					   int(pSrcDC->GetXOrigin()), int(pSrcDC->GetYOrigin()), 
					   int(pSrcDC->GetWidth()), int(pSrcDC->GetHeight()), alpha );
} 
// part copy
// --------------------------------------------------------------
bool
GDeviceWin32GDIPlus::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, 
							     int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha )
{	
	// get bitmap pixels from input VGDevice
	Bitmap* bitmap = ((GDeviceWin32GDIPlus*)pSrcDC)->GetBitmap();
	// map input bitmap to GDI object
	HBITMAP bm;
	bitmap->GetHBITMAP(Color(255,255,255), &bm);

	// create HDC object to use AlphaBlend
	HDC srcDC = CreateCompatibleDC(NULL);
	SelectObject ( srcDC, bm );

	BLENDFUNCTION bf;
	bf.BlendOp =			AC_SRC_OVER;
    bf.BlendFlags =			0;
    bf.SourceConstantAlpha = BYTE(alpha*255);  
    bf.AlphaFormat =		0;             // ignore source alpha channel

	HDC destDC = mGraphics->GetHDC();
	bool result;
	if (mRasterMode != kOpAnd)
	{
		if (alpha < 0.0){
			bf.SourceConstantAlpha = 255;  
			bf.AlphaFormat =		AC_SRC_ALPHA;             // use source alpha channel
		}
		result = AlphaBlend( destDC, xDest, yDest, nSrcWidth, nSrcHeight, srcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, bf );
	}
	// WARNING ! GDI+ HACK !
	// kOpAnd raster mode (used when highlighting selection) for GDIplus inhibits pixel copy !
	else if (alpha < 0.0)	
	{
//		result = ::BitBlt( destDC, xDest, yDest, nSrcWidth, nSrcHeight, srcDC, xSrc, ySrc, SRCAND );
	}

	mGraphics->ReleaseHDC(destDC);
	DeleteDC(srcDC);
	DeleteObject(bm);
	return result;
}


// stretched copy
// --------------------------------------------------------------
bool			
GDeviceWin32GDIPlus::CopyPixels( int xDest, int yDest, 
							     int dstWidth, int dstHeight, 
							     VGDevice* pSrcDC, float alpha )
{
	// get bitmap pixels from input VGDevice
	Bitmap* bitmap = ((GDeviceWin32GDIPlus*)pSrcDC)->GetBitmap();
	// map input bitmap to GDI object
	HBITMAP bm;
	bitmap->GetHBITMAP(NULL, &bm);
	// create HDC object to use AlphaBlend
	HDC srcDC = CreateCompatibleDC(NULL);
	SelectObject ( srcDC, bm );

	BLENDFUNCTION bf;
	bf.BlendOp =			AC_SRC_OVER;
    bf.BlendFlags =			0;
    bf.SourceConstantAlpha = BYTE(alpha*255);  
    bf.AlphaFormat =		0;             // ignore source alpha channel

	HDC destDC = mGraphics->GetHDC();
	bool result;

	if (mRasterMode != kOpAnd)
	{
		if (alpha < 0.0){
			bf.SourceConstantAlpha = 255;  
			bf.AlphaFormat =		AC_SRC_ALPHA;             // use source alpha channel
		
		}

		result = AlphaBlend( destDC, xDest, yDest, dstWidth, dstHeight, 
		 					 srcDC, int(pSrcDC->GetXOrigin()), int(pSrcDC->GetYOrigin()), 
							 int(pSrcDC->GetWidth()), int(pSrcDC->GetHeight()), bf );

	}
	// WARNING ! GDI+ HACK !
	// kOpAnd raster mode (used when highlighting selection) 
	// for GDIplus inhibits pixel copy !
	else if (alpha < 0.0)	
	{
//		result = ::BitBlt( destDC, xDest, yDest, nSrcWidth, nSrcHeight, srcDC, xSrc, ySrc, SRCAND );
	}

	mGraphics->ReleaseHDC(destDC);
	DeleteDC(srcDC);
	DeleteObject(bm);
	return result;
}


/////////////////////////////////////////////////////////////////
// - Coordinate services ----------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
// Important: Coordinates under Windows95 to 98 are stored as 16bits
// values, so they cannot exceed 32767.
void GDeviceWin32GDIPlus::SetScale( float x, float y )
{
	mScaleX	= x;
	mScaleY	= y;	
	mGraphics->ScaleTransform( x, y );  
}

// --------------------------------------------------------------
void GDeviceWin32GDIPlus::SetOrigin( float x, float y )
{	
	const float prevX	= mOriginX;
	const float prevY	= mOriginY;
	mOriginX			= x;	// voir def dans VGDev.cpp
	mOriginY			= y;
	mWinOriginX += (x - prevX) * GetXScale(); // (JB) test
	mWinOriginY += (y - prevY) * GetYScale();
	mGraphics->TranslateTransform(mWinOriginX, mWinOriginY);
}

// --------------------------------------------------------------
// Called by: GRPage, GRStaff, GRSystem
void GDeviceWin32GDIPlus::OffsetOrigin( float x, float y )
{
	const float prevX = mOriginX;
	const float prevY = mOriginY;
	mOriginX += x;
	mOriginY += y;
	mWinOriginX += (mOriginX - prevX) * GetXScale(); // (JB) test
	mWinOriginY += (mOriginY - prevY) * GetYScale();
	mGraphics->TranslateTransform(x, y);
}

// --------------------------------------------------------------
// Called by:
// GRMusic::OnDraw
// GRPage::getPixelSize
void GDeviceWin32GDIPlus::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * mScaleX - mOriginX);
	*y = (*y * mScaleY - mOriginY);
}

// --------------------------------------------------------------
// Called by:
// GRPage::DPtoLPRect
// GRPage::getVirtualSize
void GDeviceWin32GDIPlus::DeviceToLogical( float * x, float * y ) const	
{
	*x = ( *x + mOriginX ) / mScaleX;
	*y = ( *y + mOriginY ) / mScaleY;
}

// --------------------------------------------------------------
float GDeviceWin32GDIPlus::GetXScale() const		{ return mScaleX; }
float GDeviceWin32GDIPlus::GetYScale() const		{ return mScaleY; }
float GDeviceWin32GDIPlus::GetXOrigin() const		{ return mOriginX; }
float GDeviceWin32GDIPlus::GetYOrigin() const		{ return mOriginY; }

// --------------------------------------------------------------
int	GDeviceWin32GDIPlus::GetWidth() const		{ return mPhysicalWidth; }
int	GDeviceWin32GDIPlus::GetHeight() const		{ return mPhysicalHeight; }

// --------------------------------------------------------------
void GDeviceWin32GDIPlus::NotifySize( int inWidth, int inHeight )
{ 
	// pixmap is updated only if input size differs from actual
	if ( (mPhysicalWidth != inWidth) || (mPhysicalHeight != inHeight) )
	{
		MoveTo(0,0);
		if (mBitmap) {
			delete mBitmap;
			delete mGraphics;
			mBitmap = new Bitmap(inWidth, inHeight, PixelFormat32bppPARGB);
			mGraphics = new Graphics(mBitmap);
		}
		else {
			// Create a RectF object.
			RectF clipRectF(0, 0, float(inWidth), float(inHeight));
			// Set the clipping region with hRegion.
			mGraphics->SetClip(clipRectF);
		}
		// update attributes each time
		mPhysicalWidth  = inWidth;
		mPhysicalHeight = inHeight; 
	}	
}

/////////////////////////////////////////////////////////////////
// - Text and music symbols services ----------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void		
GDeviceWin32GDIPlus::DrawMusicSymbol( float x, float y, unsigned int inSymbolID )
{
	GFontWin32GDIPlus * gdiFont = (GFontWin32GDIPlus *)mCurrMusicFont;
	const char theChar = (char)(gdiFont->GetSymbol( inSymbolID ));	// TODO: correct symbol to ascii conversion

	StringFormat stringFormat;
#if 0
	PointF origin(x-27,y-10);	
	stringFormat.SetLineAlignment(StringAlignmentCenter);
#else
	PointF origin(x-32, y - gdiFont->mAscent);	
	stringFormat.SetLineAlignment(StringAlignmentNear);
#endif

	SolidBrush brush(Color(mTextColor.mAlpha, mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue ));
	WCHAR wstr [] = L"0";
	mbstowcs(wstr, &theChar, 2);
	Status err = mGraphics->DrawString(wstr, 1, gdiFont->GetNativeFont(), origin, &stringFormat, &brush);
}

// --------------------------------------------------------------
void 
GDeviceWin32GDIPlus::DrawString( float x, float y, const char * str, int inCharCount )
{ 
	if( str == 0 ) return;
	if( inCharCount < 0 )  inCharCount = (int)strlen( str );

	GFontWin32GDIPlus * gdiFont = (GFontWin32GDIPlus *)mCurrTextFont;

	// Initialize arguments.
	PointF origin(x, y);
	SolidBrush brush(Color(ALPHA_OPAQUE, mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue ));

//cout << "GDeviceWin32GDIPlus::DrawString" << endl;
//	std::string s(str);
//	std::wstring wstr(s.length(),L' ');
//	std::copy(s.begin(), s.end(), wstr.begin());
//cout << "wstring converted"  << endl;

	WCHAR * wstr = (WCHAR*) malloc ((inCharCount + 1) * sizeof(WCHAR));
	mbstowcs(wstr, str, inCharCount+1);
//cout << "GDeviceWin32GDIPlus::DrawString " << (void*)gdiFont << endl; //->GetNativeFont() << endl;
	Status err = mGraphics->DrawString(wstr, inCharCount, gdiFont->GetNativeFont(), origin, &mFontAlign1, &brush);
//	Status err = mGraphics->DrawString(wstr.data(), inCharCount, gdiFont->GetNativeFont(), origin, &mFontAlign1, &brush);
	free (wstr);

//cout << "GDeviceWin32GDIPlus::DrawString end" << endl;
}

// --------------------------------------------------------------
void 
GDeviceWin32GDIPlus::SetFontColor( const VGColor & inColor )
{ 
	mTextColor = inColor;
}
// --------------------------------------------------------------
VGColor			
GDeviceWin32GDIPlus::GetFontColor() const 
{ 
	return mTextColor; 
}

// --------------------------------------------------------------
void	
GDeviceWin32GDIPlus::SetFontBackgroundColor( const VGColor & inColor )
{
	mTextBackColor = inColor;
}
// --------------------------------------------------------------
VGColor			
GDeviceWin32GDIPlus::GetFontBackgroundColor() const
{ 
	return mTextBackColor; 
}

// --------------------------------------------------------------
void			
GDeviceWin32GDIPlus::SetFontAlign( unsigned int inFlags )
{
	mTextAlign = inFlags;

	// - win32 specific code ----------------------------
	ConvertToWin32Align( inFlags );
	mFontAlign1.SetAlignment(mHFlag);
	mFontAlign1.SetLineAlignment(mVFlag);
}
// --------------------------------------------------------------
unsigned int	
GDeviceWin32GDIPlus::GetFontAlign() const
{ 
	return mTextAlign; 
}

/////////////////////////////////////////////////////////////////
// - Printer informations services ------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void			
GDeviceWin32GDIPlus::SetDPITag( float inDPI ) 
{ 
	mDPITag = inDPI; 
}

// --------------------------------------------------------------
float			
GDeviceWin32GDIPlus::GetDPITag() const 
{ 
	return mDPITag; 
}

// --------------------------------------------------------------
VGSystem *		
GDeviceWin32GDIPlus::getVGSystem() const 
{ 
	return mSys; 
}

// --------------------------------------------------------------
void * GDeviceWin32GDIPlus::GetNativeContext() const	{ return mGraphics; }
Bitmap* GDeviceWin32GDIPlus::GetBitmap() const			{ return mBitmap; }

// --------------------------------------------------------------
void*			
GDeviceWin32GDIPlus::GetBitMapPixels() 
{
//	return mPixels;

	Rect rect(0, 0, mBitmap->GetWidth(), mBitmap->GetHeight() );
	// Lock a rectangular portion of the bitmap for reading.
	mBitmap->LockBits( &rect, ImageLockModeRead, PixelFormat32bppARGB, mBitmapData);
	// Display the hexadecimal value of each pixel in the 5x3 rectangle.
	mPixels = (UINT*)mBitmapData->Scan0;
	return mPixels;	
}

// --------------------------------------------------------------
void
GDeviceWin32GDIPlus::ReleaseBitMapPixels() 
{
	mBitmap->UnlockBits(mBitmapData);
}

// --------------------------------------------------------------
// - Win32 SPECIFIC
// --------------------------------------------------------------
//unsigned int
void
GDeviceWin32GDIPlus::ConvertToWin32Align( unsigned int inAttribute ) 
{
	if (inAttribute & kAlignBottom)		mVFlag = StringAlignmentFar;		
//	case (inAttribute & kAlignCenter)	: mVFlag = StringAlignmentCenter;	break;
	if (inAttribute & kAlignTop)		mVFlag = StringAlignmentNear;		

	if (inAttribute & kAlignRight)		mHFlag = StringAlignmentFar;		
	if (inAttribute & kAlignCenter)		mHFlag = StringAlignmentCenter;	
	if (inAttribute & kAlignLeft)		mHFlag = StringAlignmentNear;		

	
//	return outFlags;
}
// --------------------------------------------------------------
CompositingMode
GDeviceWin32GDIPlus::ConvertToWin32Op( int inOp ) const
{
	switch( inOp )
	{
		case kOpCopy:	return CompositingModeSourceCopy;	break;
		case kOpOr:		return CompositingModeSourceOver;	break;
		case kOpAnd:	//CompositingModeSourceOver;	break;
		case kOpXOr:	
		case kOpInvert:	
		default:		return CompositingModeSourceCopy;
	}
}

// --------------------------------------------------------------
float
GDeviceWin32GDIPlus::CoordToDegree( float x, float y )
{
	const float	kRadToDeg = 180 / 3.14159265359f;
	float outAngle = (float)atan2(x,y) * kRadToDeg;
	if (outAngle < 0)	outAngle += 360;

	return outAngle;
}

