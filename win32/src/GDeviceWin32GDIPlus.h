#ifndef GDeviceWin32GDIPlus_H
#define GDeviceWin32GDIPlus_H

/*
	GUIDO Library
	Copyright (C) 2003 Grame

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

	Windows 95 implementation of VGDevice drawing services. 

	Note: in this version, scaling factors are rounded to integer values, so you may
	loose precision if you use decimal scaling and translation values, especially
	when you're far from the device origin (point (0, 0)). The Win2000 device does not
	have this problem.

*/

#include <stack>
#include <math.h>

#ifdef WIN32
# if defined(_AFXDLL)
#  include <afxwin.h>         // MFC-kernel and standard components
#  include <afxext.h>         // MFC-extensions
#  include <afxdisp.h>        // MFC OLE-autmonation classes
# else	
#  include <windows.h>// without mfc
# endif
# pragma warning (disable : 4275 4251)
#endif

#include <gdiplus.h>
using namespace Gdiplus;

#include "GUIDOExport.h"
#include "VGDevice.h"
#include "VGPen.h"


// --------------------------------------------------------------
class_export  GDeviceWin32GDIPlus : public VGDevice
{

	public:
								GDeviceWin32GDIPlus( HDC hdc, VGSystem* sys );						//display device
								GDeviceWin32GDIPlus( int inWidth, int inHeight, VGSystem* sys );	//memory device
								GDeviceWin32GDIPlus( const char * inPath, VGSystem* sys );			//memory device

		virtual					~GDeviceWin32GDIPlus();

		virtual bool			IsValid() const ;

		// - Drawing services ------------------------------------------------
		virtual bool			BeginDraw();
		virtual void			EndDraw();
		virtual void			InvalidateRect( float left, float top, float right, float bottom ) 
									{}

		// - Standard graphic primitives -------------------------
		virtual void			MoveTo( float x, float y );
		virtual void			LineTo( float x, float y );
		virtual void			Line( float x1, float y1, float x2, float y2 );
		virtual void			Frame( float left, float top, float right, float bottom );
		virtual void			Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY );

		// - Filled surfaces --------------------------------------
		virtual	void			Triangle( float x1, float y1, float x2, float y2, float x3, float y3 );	
		virtual	void			Polygon( const float * xCoords, const float * yCoords, int count );	
		virtual void			Rectangle( float left, float top, float right, float bottom );

		// - Font services ---------------------------------------------------
		virtual	void			SetMusicFont( const VGFont * font );				
		virtual	const VGFont *	GetMusicFont() const;		
		virtual	void			SetTextFont( const VGFont * font );		
		virtual	const VGFont *	GetTextFont() const;

		// - Pen & brush services --------------------------------------------
		virtual	void			SelectPen( const VGColor & inColor, float witdh = 1 ); 
		virtual	void			SelectPenColor( const VGColor & inColor);
		virtual	void			SelectPenWidth( float witdh);

		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();

		virtual	void			SelectFillColor( const VGColor & c );		
		virtual	void			PushPen( const VGColor & inColor, float inWidth = 1 );
		virtual	void			PopPen();
		virtual	void			PushFillColor( const VGColor & inColor );
		virtual	void			PopFillColor();
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode);
		virtual	VRasterOpMode	GetRasterOpMode() const			{ return mRasterMode; }

		// - Bitmap services (bit-block copy methods) --------------------------
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0 ); 
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0 );	
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha = -1.0 );	
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0) 
									{ return false; }


		// - Coordinate services ------------------------------------------------
		virtual	void			SetScale( float x, float y );
        virtual	void			UnsetScale();
		virtual	void			SetOrigin( float x, float y );
		virtual	void			OffsetOrigin( float x, float y );
		virtual	void			LogicalToDevice( float * x, float * y ) const;
		virtual	void			DeviceToLogical( float * x, float * y ) const;	
		virtual float			GetXScale() const;
		virtual	float			GetYScale() const;
		virtual	float			GetXOrigin() const;
		virtual	float			GetYOrigin() const;
		virtual	void			NotifySize( int inWidth, int inHeight );
		virtual	int				GetWidth() const;
		virtual	int				GetHeight() const;

		// - Text and music symbols services -------------------------------------
		virtual void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID );
		virtual	void			DrawString( float x, float y, const char * s, int inCharCount = -1 ); 
		virtual	void			SetFontColor( const VGColor & inColor );
		virtual	VGColor			GetFontColor() const;
		virtual	void			SetFontBackgroundColor( const VGColor & inColor );
		virtual	VGColor			GetFontBackgroundColor() const;
		virtual	void			SetFontAlign( unsigned int inAlign );
		virtual	unsigned int	GetFontAlign() const;

		// - Printer informations services ----------------------------------------
		virtual	void			SetDPITag( float inDPI = 72.0f );
		virtual	float			GetDPITag() const;
		virtual	VGSystem *		getVGSystem() const;

				void*			GetBitMapPixels();
				void			ReleaseBitMapPixels();
				Bitmap*			GetBitmap()	const;

		virtual const char*		GetImageData(const char* & outDataPtr, int& outLength)	{ return 0; }
		virtual void			ReleaseImageData(const char *) const					{}

		// - Data export services ----------------------------------------
/*		virtual	void			ExportToFile( const char * inFilePath ) // to be removed
									{}
*/

		static	long	Time2DrawMusic();

	protected:

				// device's state
				struct GState 
				{
					VGPen				pen;
					VGColor				fillColor;
					VGColor				textColor;
					VGColor				textBackColor;
					float				scaleX;
					float				scaleY;
					float				originX;
					float				originY;
					float				WoriginX;
					float				WoriginY;
					unsigned int 		textAlign;
					const VGFont *		currTextFont;
					const VGFont *		currMusicFont;
					VRasterOpMode		rasterMode;
					float				currPenPosX;	//gdi+
					float				currPenPosY;	//gdi+
				};

		/// Returns the platform-specific device context object.
		virtual void *			GetNativeContext() const;	 

		enum { kOpaqueLimit = 128 };

		static int				RInt( float in ) { return (in >= 0) ? (int)floor(in + 0.5f) : (int)ceil(in - 0.5f); }

		// - Implementation		
				void			ConvertToWin32Align( unsigned int inAttribute );		// text aligmement
				CompositingMode	ConvertToWin32Op( int inOp ) const;						// bitmap copy

				float			mWinOriginX;	// win32 cached origin
				float			mWinOriginY;	// win32 cached origin
			
		/// Returns an angle from 0 to 360 
		static	float			CoordToDegree( float x, float y );

	private:


		std::stack<VGPen> 		mPenStack;
		std::stack<VGColor> 	mBrushStack;
		std::stack<GState> 		mStateStack;

				const VGFont *	mCurrTextFont;
				const VGFont *	mCurrMusicFont;
				
				int				mPtCount;		// for polygon
				Gdiplus::Point* mPtArray;		// for polygon

				void*			mPixels;
				BitmapData*		mBitmapData;

				VRasterOpMode	mRasterMode;
			
				// pen & fill color
				VGPen			mPen;
				VGColor			mFillColor;

				// scaling and size
				float			mScaleX;
				float			mScaleY;
				float			mOriginX;
				float			mOriginY;
				int				mPhysicalWidth;
				int				mPhysicalHeight;

				// text and symbol 
				VGColor			mTextColor;
				VGColor			mTextBackColor;
			unsigned int 		mTextAlign;
		
				// printing
				float			mDPITag;

				/// temporary hack - must be removed asap
				VGSystem *		mSys;

				// GDIPlus 
				Bitmap*			mBitmap;
				Graphics*		mGraphics;
				Pen*			mPen1;
				SolidBrush*		mFillColor1;
				float			mCurrPenPosX;
				float			mCurrPenPosY;
				StringFormat	mFontAlign1;
				StringAlignment mHFlag;  
				StringAlignment mVFlag;  

				GraphicsState	mGState;

		std::stack<float> 		mPenWidthStack;
		std::stack<VGColor> 	mPenColorStack;

};

#endif
