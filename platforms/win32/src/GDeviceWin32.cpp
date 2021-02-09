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

#include "GDeviceWin32.h"
#include "GFontWin32.h"
//#include "wingdi.h"


#define kTimeTableSize 1000
static long fMeanTime[kTimeTableSize];
static int timeindex = 0;

long GDeviceWin32::Time2DrawMusic()
{
	if (!timeindex) return 0;
	int n = timeindex > kTimeTableSize ? kTimeTableSize : timeindex;
	double sum = 0;
	for (int i=0; i<n; i++)
		sum += fMeanTime[i];
	return long(sum / n);
}

// --------------------------------------------------------------
// constructor for display device
GDeviceWin32::GDeviceWin32( HDC hdc, VGSystem* sys )
				:	mWinOriginX(0),	mWinOriginY(0),
					mCurrTextFont(NULL), mCurrMusicFont(NULL),mSavedDC(-1),
					mPtCount(0), mPtArray(NULL), mOwnMemoryDC(false),
					mHDC(hdc), mPixmap(0), mInited(false),
					mMaximizeAxis(true),mRasterMode(kOpCopy),
					mScaleX(1), mScaleY(1),
					mOriginX(0), mOriginY(0),
					mPixels(0), mBrushGDI(0), mPenGDI(0), mDPITag(72.0f)
{

	mPhysicalWidth  = ::GetDeviceCaps( hdc, HORZRES );
	mPhysicalHeight = ::GetDeviceCaps( hdc, VERTRES );

	initialize(sys);
}

// --------------------------------------------------------------
// constructor for memory device
GDeviceWin32::GDeviceWin32( int inWidth, int inHeight, VGSystem* sys )
				:	mWinOriginX(0),	mWinOriginY(0),
					mCurrTextFont(NULL), mCurrMusicFont(NULL),mSavedDC(-1),
					mPtCount(0), mPtArray(NULL), mOwnMemoryDC(true),
					mInited(false),
					mMaximizeAxis(true),mRasterMode(kOpCopy),
					mScaleX(1), mScaleY(1),
					mOriginX(0), mOriginY(0), mPhysicalWidth(inWidth),
					mPhysicalHeight(inHeight),
					mPixels(0), mBrushGDI(0), mPenGDI(0), mDPITag(72)
{
	// - Win32 HDC init ---------
	mHDC = ::CreateCompatibleDC( 0 );
	// if size is nul, default 1x1 mono bitmap is selected into device
	if( inWidth && inHeight ) {
		// - Win32 Bitmap init ------
		BITMAPINFO pbmi;
		pbmi.bmiHeader.biSize			= sizeof( pbmi.bmiHeader );
		pbmi.bmiHeader.biWidth			= inWidth;
		pbmi.bmiHeader.biHeight			= inHeight;
		pbmi.bmiHeader.biPlanes			= 1;
		pbmi.bmiHeader.biBitCount		= 32;
		pbmi.bmiHeader.biCompression    = BI_RGB;
		pbmi.bmiHeader.biSizeImage		= 0; // set to zero for BI_RGB bitmaps.
		pbmi.bmiHeader.biXPelsPerMeter	= 0;
		pbmi.bmiHeader.biYPelsPerMeter	= 0;
		pbmi.bmiHeader.biClrUsed		= 0;
		pbmi.bmiHeader.biClrImportant	= 0;

		mPixmap = ::CreateDIBSection(	NULL,			// handle to DC
										&pbmi,			// bitmap data
										DIB_RGB_COLORS,	// data type indicator
										&mPixels,		// on output: bit values
										NULL,			// handle to file mapping object
										0 );			// offset to bitmap bit values
		HGDIOBJ prev = ::SelectObject( mHDC, mPixmap );
		if( prev ) ::DeleteObject( prev );
	}
	initialize(sys);
}

// --------------------------------------------------------------
// Constructor for memory device based on a image file
// Supported file format: BMP.
GDeviceWin32::GDeviceWin32( const char * inPath, VGSystem* sys )
				:	mWinOriginX(0),	mWinOriginY(0),
					mCurrTextFont(NULL), mCurrMusicFont(NULL),mSavedDC(-1),
					mPtCount(0), mPtArray(NULL), mOwnMemoryDC(true),
					mScaleX(1), mScaleY(1),
					mOriginX(0), mOriginY(0), mPhysicalWidth(0),
					mPhysicalHeight(0), mDPITag(72),
					mRasterMode(kOpCopy), mPixels(0),mBrushGDI(0), mPenGDI(0)
{
	HPALETTE hPalette;
	BITMAP bm;

	// - Win32 HDC init ---------
	mHDC = ::CreateCompatibleDC( NULL );
	//create hBitmap from bmp file
	if( loadBitmapFromBMPFile( (LPTSTR)inPath, &mPixmap, &hPalette ) )
	{
      GetObject( mPixmap, sizeof(BITMAP), &bm );
	  ::SelectObject( mHDC, mPixmap );
	  ::SelectPalette( mHDC, hPalette, FALSE );
	  RealizePalette( mHDC );
	  mPhysicalWidth = bm.bmWidth;
	  mPhysicalHeight = bm.bmHeight;
	}
	initialize(sys);
}

// --------------------------------------------------------------
void GDeviceWin32::initialize(VGSystem* sys)
{
	SelectPen( VGColor(0,0,0,ALPHA_OPAQUE), 1);		//opaque black
	SelectFillColor( VGColor(0,0,0,ALPHA_OPAQUE) );	//opaque black
	SetFontColor( VGColor(0,0,0) );					//opaque black
	SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	//transp. white
	SetFontAlign( kAlignLeft | kAlignBase );

	// guido hack - must be removed asap
	mSys = sys;
}

// --------------------------------------------------------------
GDeviceWin32::~GDeviceWin32()
{
//	RestoreWinPenAndBrush();
	if (mPixmap)	::DeleteObject(mPixmap);
	delete [] mPtArray;
	if( mOwnMemoryDC ) {
		if( mHDC ) ::DeleteDC( mHDC );
		mHDC = 0;
	}
	if ( mBrushGDI ) ::DeleteObject(mBrushGDI);
	if ( mPenGDI ) ::DeleteObject(mPenGDI);
}

// --------------------------------------------------------------
bool
GDeviceWin32::IsValid() const
{
	return ( mHDC && ::GetDeviceCaps( mHDC, HORZRES ) );
}

/////////////////////////////////////////////////////////////////
// - Drawing services -------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool
GDeviceWin32::BeginDraw()
{
	if( !mInited )
	{
		mInited = true;
		Win32SetupTransformMode();
		Win32ApplyTransform();
//		SetupWinPenAndBrush();
	}

	// save Gdi context
	mSavedDC = ::SaveDC( mHDC );

	// save device's state - was VGDevice::SaveDC()
	GState			s;
	s.pen			= mPen;
	s.fillColor		= mFillColor;
//	s.textColor		= mTextColor;
//	s.textBackColor = mTextBackColor;
	s.scaleX		= mScaleX;
	s.scaleY		= mScaleY;
	s.originX		= mOriginX;
	s.originY		= mOriginY;
	s.WoriginX		= mWinOriginX;
	s.WoriginY		= mWinOriginY;
//	s.textAlign		= mTextAlign;
	s.currTextFont	= mCurrTextFont;
	s.currMusicFont = mCurrMusicFont;
	s.rasterMode	= mRasterMode;

	mStateStack.push( s );
	return true;
}

// --------------------------------------------------------------
void
GDeviceWin32::EndDraw()
{
	// restore Gdi context
	::RestoreDC( mHDC, mSavedDC );
	if ( mBrushGDI ) {
		::DeleteObject(mBrushGDI);
		mBrushGDI = 0;
	}

	if ( mPenGDI ) {
		::DeleteObject(mPenGDI);
		mPenGDI = 0;
	}

	// general device's state restore - was RestoreDC()
	GState & s = mStateStack.top();

	mPen			= s.pen;
	mFillColor		= s.fillColor;
//	mTextColor		= s.textColor;
//	mTextBackColor	= s.textBackColor;
	mScaleX			= s.scaleX;
	mScaleY			= s.scaleY;
	mOriginX		= s.originX;
	mOriginY		= s.originY;
	mWinOriginX		= s.WoriginX;
	mWinOriginY		= s.WoriginY;
//	mTextAlign		= s.textAlign;
	mCurrTextFont	= s.currTextFont;
	mCurrMusicFont	= s.currMusicFont;
	mRasterMode		= s.rasterMode;

	mStateStack.pop();
}

// --------------------------------------------------------------
void GDeviceWin32::InvalidateRect( float left, float top, float right, float bottom )
{
	HWND wnd = ::WindowFromDC(mHDC);
	if (wnd) {
		RECT r;
		r.left   = long(left);
		r.top    = long(top);
		r.right  = long(right);
		r.bottom = long(bottom);
		::InvalidateRect( wnd, &r, false);
	}
}

// --------------------------------------------------------------
void
GDeviceWin32::MoveTo( float x, float y )
{
	::MoveToEx( mHDC, RInt(x), RInt(y), NULL );
}

// --------------------------------------------------------------
void
GDeviceWin32::LineTo( float x, float y )
{
	::LineTo( mHDC, RInt(x), RInt(y) );
}


// --------------------------------------------------------------
void GDeviceWin32::Line( float x1, float y1, float x2, float y2 )
{
	MoveTo( x1, y1 );
	LineTo( x2, y2 );
}

// --------------------------------------------------------------
void GDeviceWin32::FrameEllipse( float x, float y, float width, float height)
{
	float w2 = width / 2;
	float h2 = height / 2;
	::Ellipse(mHDC, int(x - w2), int(y - h2), int(x + w2), int(y + h2));
}
// --------------------------------------------------------------
void GDeviceWin32::Ellipse(float x, float y, float width, float height, const VGColor& color)
{
	float w2 = width / 2;
	float h2 = height / 2;
	PushFillColor(color);
	::Ellipse(mHDC, int(x - w2), int(y - h2), int(x + w2), int(y + h2));
	PopFillColor();
}

// --------------------------------------------------------------
void GDeviceWin32::Frame( float left, float top, float right, float bottom )
{
    MoveTo (left, top);
    LineTo (right, top);
    LineTo (right, bottom);
    LineTo (left, bottom);
    LineTo (left, top);
}

// --------------------------------------------------------------
void GDeviceWin32::Arc( float left, float top, float right, float bottom,
				   float startX, float startY, float endX, float endY )
{
	::Arc( mHDC, RInt(left), RInt(top), RInt(right), RInt(bottom),
	       RInt(startX), RInt(startY), RInt(endX), RInt(endY));
}

// --------------------------------------------------------------
void GDeviceWin32::Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	const float xCoords [] = { x1, x2, x3 };
	const float yCoords [] = { y1, y2, y3 };

	Polygon( xCoords, yCoords, 3 );
}

// --------------------------------------------------------------
void
GDeviceWin32::Polygon( const float * xCoords, const float * yCoords, int count )
{
	if( count > mPtCount )
	{
		delete [] mPtArray;
		mPtArray = new POINT [ count ];
		if( mPtArray )
			mPtCount = count;
		else
			return;
	}

	for( int index = 0; index < count; index ++ )
	{
		mPtArray[ index ].x = RInt( xCoords[ index ] );
		mPtArray[ index ].y = RInt( yCoords[ index ] );
	}

	// first select a null width pen to avoid outlining
	PushPen(VGColor(0,0,0), 0);
	::Polygon( mHDC, mPtArray, count );
	PopPen();
}

// --------------------------------------------------------------
void
GDeviceWin32::Rectangle( float left, float top, float right, float bottom )
{
	// first select a null width pen to avoid outlining
	PushPen(VGColor(0,0,0), 0);
	::Rectangle( mHDC, RInt(left)+1, RInt(top)+1, RInt(right)+1, RInt(bottom)+1 );
	PopPen();
}

/////////////////////////////////////////////////////////////////
// - Font services ----------------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void
GDeviceWin32::SetMusicFont( const VGFont * inObj )
{
	if (inObj) {
		const GFontWin32* fontDesc = (const GFontWin32*)(inObj);
		::SelectObject( mHDC, fontDesc->GetNativeFont() );
		mCurrMusicFont = fontDesc;
	}
}

// --------------------------------------------------------------
const VGFont *
GDeviceWin32::GetMusicFont() const
{
	return mCurrMusicFont;
}

// --------------------------------------------------------------
void
GDeviceWin32::SetTextFont( const VGFont * inObj )
{
	if (inObj) {
		const GFontWin32* fontDesc = (const GFontWin32*)(inObj);
		::SelectObject( mHDC, fontDesc->GetNativeFont() );
		mCurrTextFont = fontDesc;
	}
}


// --------------------------------------------------------------
const VGFont *
GDeviceWin32::GetTextFont() const
{
	return mCurrTextFont;
}
// --------------------------------------------------------------
// - PEN & BRUSH SERVICES
// --------------------------------------------------------------
void GDeviceWin32::SelectPen( const VGColor & c, float width )
{
	//don't re-create pen if same params.
	if ((mPen.mColor == c) && (mPen.mWidth == width)) return;

	mPen.Set( c, width );

	// - win32 specific code ----------------------------
	// win32 trick: input width of -1 or -2 sets pen to dashed/dotted style.
	COLORREF color = RGB( c.mRed, c.mGreen, c.mBlue );
	if ( width && (c.mAlpha != ALPHA_TRANSPARENT) ) {
		if ( width == -1)
			mPenGDI = ::CreatePen( PS_DASH, RInt( 1 ), color);
		else if ( width == -2)
			mPenGDI = ::CreatePen( PS_DOT, RInt( 1 ), color);
		else
			mPenGDI = ::CreatePen( PS_SOLID, RInt( width ), color);
	}
	else
		mPenGDI = ::CreatePen( PS_NULL, 0, color);

	if( mPenGDI )
	{
		HGDIOBJ prevPen = ::SelectObject( mHDC, mPenGDI );
		if( prevPen )
			::DeleteObject( prevPen );
	}
}

void GDeviceWin32::SelectPenWidth( float width )
{
	//don't re-create pen if same params.
	if (mPen.mWidth == width) return;

	mPen.mWidth = width;
	// - win32 specific code ----------------------------
	// win32 trick: input width of -1 or -2 sets pen to dashed/dotted style.
	COLORREF color = RGB( mPen.mColor.mRed, mPen.mColor.mGreen, mPen.mColor.mBlue );
	mPenGDI = ::CreatePen( PS_SOLID, RInt( width ), color);
	if( mPenGDI )
	{
		HGDIOBJ prevPen = ::SelectObject( mHDC, mPenGDI );
		if( prevPen )
			::DeleteObject( prevPen );
	}
}

void GDeviceWin32::SelectPenColor( const VGColor & c )
{
	//don't re-create pen if same params.
	if (mPen.mColor == c) return;

	mPen.mColor = c;
	COLORREF color = RGB( mPen.mColor.mRed, mPen.mColor.mGreen, mPen.mColor.mBlue );
	mPenGDI = ::CreatePen( PS_SOLID, RInt( mPen.mWidth ), color);

	if( mPenGDI )
	{
		HGDIOBJ prevPen = ::SelectObject( mHDC, mPenGDI );
		if( prevPen )
			::DeleteObject( prevPen );
	}
}

// --------------------------------------------------------------
void
GDeviceWin32::SelectFillColor( const VGColor & c )
{
	mFillColor.Set( c );
	if( c.mAlpha == ALPHA_TRANSPARENT ) {
		mBrushGDI = (HBRUSH)::GetStockObject( HOLLOW_BRUSH );
	}
	else {
		mBrushGDI = ::CreateSolidBrush( RGB( c.mRed, c.mGreen, c.mBlue ));
	}

	if( mBrushGDI ) {
		HGDIOBJ  prevBrush = ::SelectObject( mHDC, mBrushGDI );
		if( prevBrush ) ::DeleteObject( prevBrush );
	}
}

// --------------------------------------------------------------
// Save the current pen, select the new one
void GDeviceWin32::PushPen( const VGColor & inColor, float inWidth )
{
	mPenStack.push( mPen );
	SelectPen( inColor, inWidth );
}

// --------------------------------------------------------------
// Restore the previous pen from the stack
void GDeviceWin32::PopPen()
{
	VGPen & pen = mPenStack.top();
	SelectPen( pen.mColor, pen.mWidth );
	mPenStack.pop();
}

void GDeviceWin32::PushPenColor( const VGColor & inColor)
{
	mPenColorStack.push( mPen.mColor );
	SelectPenColor (inColor);
}
void GDeviceWin32::PopPenColor()
{
	SelectPenColor (mPenColorStack.top());
	mPenColorStack.pop();
}
		
void GDeviceWin32::PushPenWidth( float width)
{
	mPenWidthStack.push( mPen.mWidth );
	SelectPenWidth (width);
}
void GDeviceWin32::PopPenWidth()
{
	SelectPenWidth (mPenWidthStack.top());
	mPenWidthStack.pop();
}

// --------------------------------------------------------------
// Save the current brush, select the new one
void
GDeviceWin32::PushFillColor( const VGColor & inColor )
{
	mBrushStack.push( mFillColor );
	if( inColor != mFillColor )
		SelectFillColor( inColor );
}

// --------------------------------------------------------------
// Restore the previous brush from the stack
void
GDeviceWin32::PopFillColor()
{
	VGColor & brush = mBrushStack.top();
	SelectFillColor( brush );
	mBrushStack.pop();
}

// --------------------------------------------------------------
void
GDeviceWin32::SetRasterOpMode( VRasterOpMode ROpMode )
{
	mRasterMode = ROpMode;
}

/////////////////////////////////////////////////////////////////
// - Bitmap services (bit-block copy methods) -------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
bool
GDeviceWin32::CopyPixels( VGDevice* pSrcDC, float alpha )
{
	return CopyPixels( int(mOriginX), int(mOriginY), pSrcDC,
					   int(pSrcDC->GetXOrigin()), int(pSrcDC->GetYOrigin()),
					   int(pSrcDC->GetWidth()), int(pSrcDC->GetHeight()), alpha );

}
// --------------------------------------------------------------
bool
GDeviceWin32::CopyPixels( int xDest, int yDest, VGDevice* pSrcDC,
							int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha )
{
	GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(pSrcDC);
	if (!pSrcDC) return false;

	if((alpha < 0.0) || (alpha >= 1.0))	// default: opaque copy
	{
		const DWORD mode = ConvertToWin32Op(GetRasterOpMode());
		return ::BitBlt( mHDC, xDest, yDest, nSrcWidth, nSrcHeight,
			             (HDC)(pSrcDC_Win32->GetNativeContext()), xSrc, ySrc, mode ) ? true : false;
	}
	else				// alpha copy
	{
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = BYTE(alpha*255);		// half of 0xff = 50% transparency
		bf.AlphaFormat = 0;						// ignore source alpha channel

		return AlphaBlend(
			mHDC,            // handle to destination DC
			xDest,								// x-coord of upper-left corner
			yDest,								// y-coord of upper-left corner
			nSrcWidth,							// destination width
			nSrcHeight,							// destination height
			(HDC)(pSrcDC_Win32->GetNativeContext()),                  // handle to source DC
			xSrc,								// x-coord of upper-left corner
			ySrc,								// y-coord of upper-left corner
			nSrcWidth,							// source width
			nSrcHeight,							// source height
			bf									// alpha-blending function
		) ? true : false;
	}
}

// --------------------------------------------------------------
bool
GDeviceWin32::CopyPixels( int xDest, int yDest,
							int dstWidth, int dstHeight,
							VGDevice* pSrcDC, float alpha )
{
	GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(pSrcDC);
	if (!pSrcDC) return false;

	if((alpha < 0.0) || (alpha >= 1.0))	// default: opaque copy
	{
		const DWORD mode = ConvertToWin32Op(GetRasterOpMode());
		return ::StretchBlt( mHDC, xDest, yDest, dstWidth, dstHeight,
							 (HDC)(pSrcDC_Win32->GetNativeContext()), 
							 int(pSrcDC->GetXOrigin()), int(pSrcDC->GetYOrigin()), int(pSrcDC->GetWidth()),
							 int(pSrcDC->GetHeight()), mode ) ? true : false;
	}
	else				// alpha copy
	{
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = BYTE(alpha*255);		// half of 0xff = 50% transparency
		bf.AlphaFormat = 0;						// ignore source alpha channel

		return AlphaBlend(
			mHDC,            // handle to destination DC
			xDest,								// x-coord of upper-left corner
			yDest,								// y-coord of upper-left corner
			dstWidth,							// destination width
			dstHeight,							// destination height
			(HDC)(pSrcDC_Win32->GetNativeContext()),                  // handle to source DC
			(int)pSrcDC->GetXOrigin(),				// x-coord of upper-left corner
			(int)pSrcDC->GetYOrigin(),				// y-coord of upper-left corner
			(int)pSrcDC->GetWidth(),					// source width
			(int)pSrcDC->GetHeight(),				// source height
			bf									// alpha-blending function
		) ? true : false;
	}
}

// --------------------------------------------------------------
bool
GDeviceWin32::CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight,
						  VGDevice* pSrcDC, int xSrc, int ySrc, int srcWidth, int srcHeight, float alpha )
{
	GDeviceWin32* pSrcDC_Win32 = (GDeviceWin32*)(pSrcDC);
	if (!pSrcDC) return false;

	if((alpha < 0.0) || (alpha >= 1.0))	// default: opaque copy
	{
		const DWORD mode = ConvertToWin32Op(GetRasterOpMode());
		return ::StretchBlt( mHDC, xDest, yDest, dstWidth, dstHeight,
							 (HDC)(pSrcDC_Win32->GetNativeContext()), xSrc,
							 ySrc, srcWidth, srcHeight, mode ) ? true : false;
	}
	else				// alpha copy
	{
		BLENDFUNCTION bf;
		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = BYTE(alpha*255);		// half of 0xff = 50% transparency
		bf.AlphaFormat = 0;						// ignore source alpha channel

		return AlphaBlend(
			mHDC,            // handle to destination DC
			xDest,								// x-coord of upper-left corner
			yDest,								// y-coord of upper-left corner
			dstWidth,							// destination width
			dstHeight,							// destination height
			(HDC)(pSrcDC_Win32->GetNativeContext()),                  // handle to source DC
			xSrc,								// x-coord of upper-left corner
			ySrc,								// y-coord of upper-left corner
			srcWidth,							// source width
			srcHeight,							// source height
			bf									// alpha-blending function
		) ? true : false;
	}
}

/////////////////////////////////////////////////////////////////
// - Coordinate services ----------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
// Important: Coordinates under Windows95 to 98 are stored as 16bits
// values, so they cannot exceed 32767.
void
GDeviceWin32::SetScale( float x, float y )
{
	mScaleX	= x;
	mScaleY	= y;
	Win32ApplyTransform();
}

// --------------------------------------------------------------
void
GDeviceWin32::SetOrigin( float x, float y )
{
	const float prevX	= mOriginX;
	const float prevY	= mOriginY;
	mOriginX			= x;	// voir def dans VGDev.cpp
	mOriginY			= y;
	mWinOriginX += (x - prevX) * GetXScale(); // (JB) test
	mWinOriginY += (y - prevY) * GetYScale();
//	mWinOriginX += (x - prevX) ; // (JB) test
//	mWinOriginY += (y - prevY) ;
	::SetViewportOrgEx( mHDC, RInt( mWinOriginX ), RInt( mWinOriginY ), NULL );
}
// --------------------------------------------------------------
// Called by: GRPage, GRStaff, GRSystem
void
GDeviceWin32::OffsetOrigin( float x, float y )
{
//	const float prevX = mOriginX;
//	const float prevY = mOriginY;
	mOriginX += x;
	mOriginY += y;
	mWinOriginX += (x) * GetXScale(); // (JB) test
	mWinOriginY += (y) * GetYScale();
	::SetViewportOrgEx( mHDC, RInt( mWinOriginX ), RInt( mWinOriginY ), NULL );
}

// --------------------------------------------------------------
// Called by:
// GRMusic::OnDraw
// GRPage::getPixelSize
void
GDeviceWin32::LogicalToDevice( float * x, float * y ) const
{
	*x = (*x * mScaleX - mOriginX);
	*y = (*y * mScaleY - mOriginY);
}

// --------------------------------------------------------------
// Called by:
// GRPage::DPtoLPRect
// GRPage::getVirtualSize
void
GDeviceWin32::DeviceToLogical( float * x, float * y ) const
{
	*x = ( *x + mOriginX ) / mScaleX;
	*y = ( *y + mOriginY ) / mScaleY;
}

// --------------------------------------------------------------
float
GDeviceWin32::GetXScale() const
{
	return mScaleX;
}
// --------------------------------------------------------------
float
GDeviceWin32::GetYScale() const
{
	return mScaleY;
}
// --------------------------------------------------------------
float
GDeviceWin32::GetXOrigin() const
{
	return mOriginX;
}
// --------------------------------------------------------------
float
GDeviceWin32::GetYOrigin() const
{
	return mOriginY;
}

// --------------------------------------------------------------
void
GDeviceWin32::NotifySize( int inWidth, int inHeight )
{
	// pixmap is updated only if input size differs from actual
	if ( (mPhysicalWidth != inWidth) || (mPhysicalHeight != inHeight) )
	{
		// delete previous bitmap
		if (mOwnMemoryDC) {
			HBITMAP memBM = 0;
			if( mPixmap )
			{
				memBM = CreateCompatibleBitmap ( mHDC, 1, 1 );
				SelectObject ( mHDC, memBM );
				::DeleteObject( mPixmap );
			}

		// re-init bitmap
			BITMAPINFO pbmi;
			pbmi.bmiHeader.biSize			= sizeof( pbmi.bmiHeader );
			pbmi.bmiHeader.biWidth			= inWidth;
			pbmi.bmiHeader.biHeight			= inHeight;
			pbmi.bmiHeader.biPlanes			= 1;
			pbmi.bmiHeader.biBitCount		= 32;
			pbmi.bmiHeader.biCompression    = BI_RGB;
			pbmi.bmiHeader.biSizeImage		= 0; // set to zero for BI_RGB bitmaps.
			pbmi.bmiHeader.biXPelsPerMeter	= 0;
			pbmi.bmiHeader.biYPelsPerMeter	= 0;
			pbmi.bmiHeader.biClrUsed		= 0;
			pbmi.bmiHeader.biClrImportant	= 0;

			mPixmap = ::CreateDIBSection(	NULL,			// handle to DC
											&pbmi,			// bitmap data
											DIB_RGB_COLORS,	// data type indicator
											&mPixels,		// on output: bit values
											NULL,			// handle to file mapping object
											0 );			// offset to bitmap bit values

			::SelectObject( mHDC, mPixmap );
			if (memBM) ::DeleteObject( memBM );
		}

		// update attributes
		mPhysicalWidth  = inWidth;
		mPhysicalHeight = inHeight;
	}
}

// --------------------------------------------------------------
int
GDeviceWin32::GetWidth() const
{
	return mPhysicalWidth;
}

// --------------------------------------------------------------
int
GDeviceWin32::GetHeight() const
{
	return mPhysicalHeight;
}

/////////////////////////////////////////////////////////////////
// - Text and music symbols services ----------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void
GDeviceWin32::DrawMusicSymbol( float x, float y, unsigned int inSymbolID )
{
	//LARGE_INTEGER counter1, counter2;
	//QueryPerformanceCounter (&counter1);

	SetMusicFont(mCurrMusicFont);
	const char theChar = (char)inSymbolID;	// TODO: correct symbol to ascii conversion
	::TextOut( mHDC, RInt( x ), RInt( y ), &theChar, 1 );

	//QueryPerformanceCounter (&counter2);
	//LONGLONG diff = counter2.QuadPart - counter1.QuadPart;
	//fMeanTime[timeindex++ % kTimeTableSize] = long(diff);
}

// --------------------------------------------------------------
void
GDeviceWin32::DrawString( float x, float y, const char * s, int inCharCount )
{
	if( s == 0 ) return;
	SetTextFont(mCurrTextFont);
	if( inCharCount < 0 )
		inCharCount = (int)strlen( s );
	::TextOut( mHDC, RInt( x ), RInt( y ), s, inCharCount );
}

// --------------------------------------------------------------
void
GDeviceWin32::SetFontColor( const VGColor & inColor )
{
//	mTextColor = inColor;

	// - win32 specific code ----------------------------
	::SetTextColor( mHDC, RGB( inColor.mRed, inColor.mGreen, inColor.mBlue ));
}
// --------------------------------------------------------------
VGColor
GDeviceWin32::GetFontColor() const
{
	//COLORREF c = GetTextColor( mHDC ); // COLORREF mask is 0x00bbggrr
	unsigned char c = (unsigned char)GetTextColor( mHDC ); // COLORREF mask is 0x00bbggrr
	return VGColor ( c & 0xff, c & 0xff00, c & 0xff0000);
}

// --------------------------------------------------------------
void
GDeviceWin32::SetFontBackgroundColor( const VGColor & inColor )
{
//	mTextBackColor = inColor;

	// - win32 specific code ----------------------------
	if( inColor.mAlpha == ALPHA_TRANSPARENT )
		::SetBkMode( mHDC, TRANSPARENT );
	else
		::SetBkMode( mHDC, OPAQUE );
	::SetBkColor(mHDC, RGB( inColor.mRed, inColor.mGreen, inColor.mBlue ));
}
// --------------------------------------------------------------
VGColor
GDeviceWin32::GetFontBackgroundColor() const
{
	int mode = GetBkMode(mHDC);
//	COLORREF c = GetBkColor(mHDC);
	unsigned char c = (unsigned char)GetTextColor( mHDC ); // COLORREF mask is 0x00bbggrr
	return VGColor ( c & 0xff, c & 0xff00, c & 0xff0000, mode == TRANSPARENT ? ALPHA_TRANSPARENT : ALPHA_OPAQUE);
}

// --------------------------------------------------------------
void
GDeviceWin32::SetFontAlign( unsigned int inFlags )
{
//	mTextAlign = inFlags;

	// - win32 specific code ----------------------------
	::SetTextAlign( mHDC, ConvertToWin32Align( inFlags ));
}
// --------------------------------------------------------------
unsigned int
GDeviceWin32::GetFontAlign() const
{
	return ::GetTextAlign(mHDC);
}

/////////////////////////////////////////////////////////////////
// - Printer informations services ------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
void
GDeviceWin32::SetDPITag( float inDPI )
{
	mDPITag = inDPI;
}

// --------------------------------------------------------------
float
GDeviceWin32::GetDPITag() const
{
	return mDPITag;
}

// --------------------------------------------------------------
VGSystem *
GDeviceWin32::getVGSystem() const
{
	return mSys;
}

// --------------------------------------------------------------
void *
GDeviceWin32::GetNativeContext() const
{
	return mHDC;
}

// --------------------------------------------------------------
void*
GDeviceWin32::GetBitMapPixels()
{
	return mPixels;
}

// --------------------------------------------------------------
void
GDeviceWin32::ReleaseBitMapPixels()
{
}

/////////////////////////////////////////////////////////////////
// - Data export services ---------------------------------------
/////////////////////////////////////////////////////////////////
// --------------------------------------------------------------
/*
void
GDeviceWin32::ExportToFile( const char * inFilePath )
{
		HBITMAP bmp = CreateBitmap(
		  mPhysicalWidth,
		  mPhysicalHeight,
		  1,
		  32,
		  mPixels
		);

		PBITMAPINFO bmi = createBitmapInfoStruct( 0, bmp );
		saveBitmapToBMPFile( 0, inFilePath, bmi, bmp, mHDC );
}
*/

// --------------------------------------------------------------
// - Win32 SPECIFIC
// --------------------------------------------------------------
unsigned int
GDeviceWin32::ConvertToWin32Align( unsigned int inAttribute ) const
{
	unsigned int outFlags = 0;

	if( inAttribute & kAlignBase )		outFlags |= TA_BASELINE;
	if( inAttribute & kAlignBottom )	outFlags |= TA_BOTTOM;
	if( inAttribute & kAlignTop )		outFlags |= TA_TOP;
	if( inAttribute & kAlignCenter )	outFlags |= TA_CENTER;
	if( inAttribute & kAlignLeft )		outFlags |= TA_LEFT;
	if( inAttribute & kAlignRight )		outFlags |= TA_RIGHT;

	return outFlags;
}

// --------------------------------------------------------------
unsigned long
GDeviceWin32::ConvertToWin32Op( int inOp ) const
{
	switch( inOp )
	{
		case kOpCopy:	return SRCCOPY;		break;
		case kOpAnd:	return SRCAND;		break;
		case kOpXOr:	return SRCINVERT; 	break;
		case kOpInvert:	return DSTINVERT;	break;
		case kOpOr:		return SRCPAINT;	break;
		default:		return SRCCOPY;
	}
}

// --------------------------------------------------------------
void
GDeviceWin32::Win32SetupTransformMode()
{
	// returns 0 on failure
	int result = ::SetMapMode( mHDC, MM_ANISOTROPIC );
	result = ::SetGraphicsMode( mHDC, GM_COMPATIBLE );
}

// --------------------------------------------------------------
bool GDeviceWin32::Win32ApplyTransform()
{
	const float xScale = GetXScale();
	const float yScale = GetYScale();
	float width, height, newWidth, newHeight;

	// - Original code, the standard way to calculate new size.
	width = (float)GetWidth();
	height = (float)GetHeight();
	newWidth = width * xScale;
	newHeight = height * yScale;

	// Win95 : Coordinates can't be larger than 16 bit.
	if( mMaximizeAxis || width > 32000 || height > 32000
						|| newWidth > 32000 || newHeight > 32000 )
	{
		// Then, use the largest extent possible, but under 32000
		if( xScale <= 1 ) {
			width = 32000;
			newWidth = width * xScale;
		} else {
			newWidth = 32000;
			width = newWidth / xScale;
		}

		if( yScale <= 1 ) {
			height = 32000;
			newHeight = height * yScale;
		} else {
			newHeight = 32000;
			height = newHeight / yScale;
		}
	}

	BOOL result;
	// OK for everything but print preview ->
	result = ::SetWindowExtEx( mHDC, RInt( width ), RInt( height ), NULL );
	result = ::SetViewportExtEx( mHDC, RInt( newWidth ), RInt( newHeight ), NULL );
	return true;
}

//----------------------------------------------------------------
// see http://support.microsoft.com/kb/q158898/
bool GDeviceWin32::loadBitmapFromBMPFile( LPTSTR szFileName, HBITMAP *phBitmap,
										  HPALETTE *phPalette )
{

   BITMAP  bm;

   *phBitmap = NULL;
   *phPalette = NULL;

   // Use LoadImage() to get the image loaded into a DIBSection
   *phBitmap = (HBITMAP)LoadImage( NULL, szFileName, IMAGE_BITMAP, 0, 0,
               LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
   if( *phBitmap == NULL )
     return FALSE;

   // Get the color depth of the DIBSection
   GetObject(*phBitmap, sizeof(BITMAP), &bm );
   // If the DIBSection is 256 color or less, it has a color table
   if( ( bm.bmBitsPixel * bm.bmPlanes ) <= 8 )
   {
	   HDC           hMemDC;
	   HBITMAP       hOldBitmap;
	   RGBQUAD       rgb[256];
	   LPLOGPALETTE  pLogPal;
	   WORD          i;

	   // Create a memory DC and select the DIBSection into it
	   hMemDC = CreateCompatibleDC( NULL );
	   hOldBitmap = (HBITMAP)SelectObject( hMemDC, *phBitmap );
	   // Get the DIBSection's color table
	   GetDIBColorTable( hMemDC, 0, 256, rgb );
	   // Create a palette from the color tabl
	   pLogPal = (LOGPALETTE *)malloc( sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY)) );
	   pLogPal->palVersion = 0x300;
	   pLogPal->palNumEntries = 256;
	   for(i=0;i<256;i++)
	   {
		 pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
		 pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
		 pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
		 pLogPal->palPalEntry[i].peFlags = 0;
	   }
	   *phPalette = CreatePalette( pLogPal );
	   // Clean up
	   free( pLogPal );
	   SelectObject( hMemDC, hOldBitmap );
	   DeleteDC( hMemDC );
   }
   else   // It has no color table, so use a halftone palette
   {
	   HDC    hRefDC;

	   hRefDC = GetDC( NULL );
	   *phPalette = CreateHalftonePalette( hRefDC );
	   ReleaseDC( NULL, hRefDC );
   }
   return TRUE;

}

//The following example code defines a function that initializes the remaining
//structures, retrieves the array of palette indices, opens the file, copies the data, and closes the file.
//-----------------------------------------------------------------------
PBITMAPINFO GDeviceWin32::createBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // Retrieve the bitmap color format, width, and height.
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
	{
        //errhandler("GetObject", hwnd);
	}


    // Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    // Allocate memory for the BITMAPINFO structure. (This structure
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    // data structures.)

     if (cClrBits != 24)
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                    sizeof(BITMAPINFOHEADER) +
                    sizeof(RGBQUAD) * (1<< cClrBits));

     // There is no RGBQUAD array for the 24-bit-per-pixel format.

     else
         pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                    sizeof(BITMAPINFOHEADER));

    // Initialize the fields in the BITMAPINFO structure.

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits);

    // If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color
    // indices and store the result in biSizeImage.
    // For Windows NT, the width must be DWORD aligned unless
    // the bitmap is RLE compressed. This example shows this.
    // For Windows 95/98/Me, the width must be WORD aligned unless the
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight;
    // Set biClrImportant to 0, indicating that all of the
    // device colors are important.
     pbmi->bmiHeader.biClrImportant = 0;

	 pbmi->bmiHeader.biHeight = pbmi->bmiHeader.biHeight;

     return pbmi;
 }

//-----------------------------------------------------------------------
void GDeviceWin32::saveBitmapToBMPFile( HWND hwnd, LPCTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
 {
    HANDLE hf;                  // file handle
    BITMAPFILEHEADER hdr;       // bitmap file-header
    PBITMAPINFOHEADER pbih;     // bitmap info-header
    LPBYTE lpBits;              // memory pointer
    DWORD dwTotal;              // total count of bytes
    DWORD cb;                   // incremental count of bytes
    BYTE *hp;                   // byte pointer
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits)
	{
         //errhandler("GlobalAlloc", hwnd);
	}

    // Retrieve the color table (RGBQUAD array) and the bits
    // (array of palette indices) from the DIB.
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi,
        DIB_RGB_COLORS))
    {
        //errhandler("GetDIBits", hwnd);
    }

	pbi->bmiHeader.biHeight = -pbi->bmiHeader.biHeight;

    // Create the .BMP file.
    hf = CreateFile(pszFile,
                   GENERIC_READ | GENERIC_WRITE,
                   (DWORD) 0,
                    NULL,
                   CREATE_ALWAYS,
                   FILE_ATTRIBUTE_NORMAL,
                   (HANDLE) NULL);
    if (hf == INVALID_HANDLE_VALUE)
	{
        //errhandler("CreateFile", hwnd);
	}
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
    // Compute the size of the entire file.
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +
                 pbih->biSize + pbih->biClrUsed
                 * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
                    pbih->biSize + pbih->biClrUsed
                    * sizeof (RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file.
    if ( !WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER),
        (LPDWORD) &dwTmp,  NULL) )
    {
       //errhandler("WriteFile", hwnd);
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
                  + pbih->biClrUsed * sizeof (RGBQUAD),
                  (LPDWORD) &dwTmp, (NULL)) )
	{
        //errhandler("WriteFile", hwnd);
	}

    // Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
	{
           //errhandler("WriteFile", hwnd);
	}

    // Close the .BMP file.
     if (!CloseHandle(hf))
	 {
           //errhandler("CloseHandle", hwnd);
	 }

    // Free memory.
    GlobalFree((HGLOBAL)lpBits);
}

//Load any kind of image using OLE
//http://www.codeguru.com/Cpp/G-M/bitmap/article.php/c4935/
//----------------------------------------------------------------
HBITMAP GDeviceWin32::loadAnImage(char* FileName)
{
/*    WCHAR wpath[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, FileName, -1, wpath, MAX_PATH);

    IPicture* pPic;
    OleLoadPicturePath(wpath, NULL, NULL, NULL, IID_IPicture,(LPVOID*)&pPic);

	HBITMAP hPic = NULL;
    pPic->get_Handle((UINT*)&hPic);

    HBITMAP hPicRet = (HBITMAP)CopyImage(hPic, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);

    pPic->Release();
    return hPicRet;*/
	return 0;

}

HDC GDeviceWin32::getHDC() const
{
	return (HDC)GetNativeContext();
}
