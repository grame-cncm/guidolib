/*
	GUIDO Library
	Copyright (C) 2003  Grame

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

	Note: requires windows 2000 or later.

	DLL or compiled with this file won't run on versions
    earlier than windows 2000
*/

//***************************************************************//
//**************** WARNING - WARNING - WARNING ******************//
//																 //
//		 			   UN-MAINTAINED DEVICE						 //
//				Doesn't match the new VGDevice API				 //
//																 //
//						-- Grame 2007 --						 //
//***************************************************************//


#define _WIN32_WINNT 0x0500 

#include "GDeviceWin2000.h"
#include "wingdi.h"

// #include <afxwin.h>
// --------------------------------------------------------------
GDeviceWin2000::GDeviceWin2000( HDC hdc, int inWidth, int inHeight ) 
					: GDeviceWin32( hdc, inWidth, inHeight )
{ 
	mNullPen = ::CreatePen( PS_NULL, 1, RGB( 0, 0, 0 ));
	mMustDeletePen = false;
}

// --------------------------------------------------------------
GDeviceWin2000::~GDeviceWin2000()
{ 
}

// --------------------------------------------------------------
void 
GDeviceWin2000::Win32SetupTransformMode()
{
	::SetMapMode( GetHDC(), MM_ANISOTROPIC );
	::SetGraphicsMode( GetHDC(), GM_ADVANCED );
}

// --------------------------------------------------------------
bool
GDeviceWin2000::Win32ApplyTransform()
{
	// - Advanced GDI
	XFORM matrix; 
	matrix.eM11 = GetXScale(); 
	matrix.eM12 = 0.0f; 
	matrix.eM21 = 0.0f; 
	matrix.eM22 = GetYScale();
	matrix.eDx  = mWinOriginX; 
	matrix.eDy  = mWinOriginY; 
	const int result = ::SetWorldTransform( GetHDC(), &matrix );
	return (result != 0);
}

// --------------------------------------------------------------
void			
GDeviceWin2000::DoSetScale( float prevX, float prevY, float newX, float newY )
{
	Win32ApplyTransform();
}

// --------------------------------------------------------------
void				
GDeviceWin2000::DoSetOrigin( float prevX, float prevY, float newX, float newY )
{
	mWinOriginX += (newX - prevX) * GetXScale();
	mWinOriginY += (newY - prevY) * GetYScale();

	Win32ApplyTransform();
}
// --------------------------------------------------------------
void 
GDeviceWin2000::SetupWinPenAndBrush()
{
	// - Setup pens
	HPEN newPen = (HPEN)::GetStockObject( DC_PEN );
	mOriginalPen = (HPEN)::SelectObject( GetHDC(), newPen );

//	DWORD err = ::GetLastError();
	
//	mIsStockPen = true;
//	mIsMyPen = false;

	// - Setup brushes
	HBRUSH newBrush = (HBRUSH)::GetStockObject( BLACK_BRUSH );
	mOriginalBrush = (HBRUSH)::SelectObject( GetHDC(), newBrush );

//	::SelectObject( GetHDC(), GetStockObject( DC_BRUSH ));	// <- new
//	::SetDCBrushColor( GetHDC(), RGB( 255, 255, 255 ));		// <- new
}

// --------------------------------------------------------------
void 
GDeviceWin2000::RestoreWinPenAndBrush()
{
	// - Restore the original pen
	if( mOriginalPen )
	{
		HPEN prevPen = (HPEN)::SelectObject( GetHDC(), mOriginalPen );
		if( prevPen && mMustDeletePen )
			::DeleteObject( prevPen );
	}

	if( mNullPen )	::DeleteObject( mNullPen );

	// - Restore the original brush
	if( mOriginalBrush )
		::SelectObject( GetHDC(), mOriginalBrush );
}

// --------------------------------------------------------------
void			
GDeviceWin2000::SelectPen( const VGColor & c, float width )
{
	bool deletePrevPen = mMustDeletePen;
	
	HPEN prevPen = 0;
	if( c.mAlpha == ALPHA_TRANSPARENT )
	{
		// - Select our transparent pen.
		prevPen = (HPEN)::SelectObject( GetHDC(), mNullPen );
//		DWORD err = ::GetLastError();

		mMustDeletePen = false;
	}
	else 
	{
		const int newWidth = RInt( width );
		if( newWidth == 1 )
		{
			// - Use the standard dc pen (Win2000 and later)
			HPEN newPen = (HPEN)::GetStockObject( DC_PEN );
			prevPen = (HPEN)::SelectObject( GetHDC(), newPen );
			::SetDCPenColor( GetHDC(), RGB( c.mRed, c.mGreen, c.mBlue ));
			
			mMustDeletePen = false;
		}
		else
		{
			HPEN newPen = ::CreatePen( PS_SOLID, newWidth, RGB( c.mRed, c.mGreen, c.mBlue ));	
			prevPen = (HPEN)::SelectObject( GetHDC(), newPen );

			mMustDeletePen = true;
		}
	}
	
	if( prevPen && deletePrevPen )
		::DeleteObject( prevPen );	
}

// --------------------------------------------------------------
void			
GDeviceWin2000::SelectBrush( const VGColor & c )
{
	if( c.mAlpha == ALPHA_TRANSPARENT ) 
	{
		// - Here we use the transparent (null) stock brush.
		HBRUSH newBrush = (HBRUSH)::GetStockObject( HOLLOW_BRUSH );
		::SelectObject( GetHDC(), newBrush );
	}
	else
	{
		// - We just have to select our brush and set its color.
		// ( Win2000 and later only)
		HBRUSH newBrush = (HBRUSH)::GetStockObject( DC_BRUSH );
		::SelectObject( GetHDC(), newBrush );
		::SetDCBrushColor( GetHDC(), RGB( c.mRed, c.mGreen, c.mBlue ));
	}	
}
