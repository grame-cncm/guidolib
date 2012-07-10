#ifndef GDeviceOSX_H
#define GDeviceOSX_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
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

/////////////////////////////////////////////////////////////////
///
/// 	MacOS X Quartz 2D implementation of VGDevice.
///
///		For inherited methods documentation, see VGDevice.h
/////////////////////////////////////////////////////////////////

#include <stack>

#import <QuartzCore/QuartzCore.h>

#include "VGDevice.h"
#include "VGPen.h"
#include "GUIDOExport.h"

CG_EXTERN CGImageRef CGBitmapContextCreateImage(CGContextRef c);
CG_EXTERN CGImageRef CGImageCreateWithImageInRect(CGImageRef image, CGRect rect);

// --------------------------------------------------------------
class_export GDeviceOSX : public VGDevice
{
	public:
								GDeviceOSX(int inWidth, int inHeight, VGSystem* sys);
		virtual					~GDeviceOSX();

		virtual bool			IsValid() const 
									{ return (mContext != 0); }

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
		virtual void			Rectangle( float left,  float top, float right, float bottom );

		// - Font services ---------------------------------------------------
		virtual	void			SetMusicFont( const VGFont * font ); 
		virtual	const VGFont *	GetMusicFont() const 				  
									{ return mCurrMusicFont; }
		virtual	void			SetTextFont( const VGFont * font ); 			
		virtual	const VGFont *	GetTextFont() const				  
									{ return mCurrTextFont; }

		// - Pen & brush services --------------------------------------------
		virtual	void			SelectPen( const VGColor & inColor, 
										   float width );				//was DoSelectPen
		virtual	void			SelectFillColor( const VGColor & c );	//was DoSelectBrush	
		virtual	void			PushPen( const VGColor & inColor, float inWidth ); 
		virtual	void			PopPen(); 
		virtual	void			PushFillColor( const VGColor & inColor ); 
		virtual	void			PopFillColor(); 
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode);
		virtual	VRasterOpMode	GetRasterOpMode() const 
									{ return mRasterMode; }


		// - Bitmap services (bit-block copy methods) --------------------------
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0) __attribute__((weak_import));
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0) __attribute__((weak_import));	
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha = -1.0) __attribute__((weak_import));
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0) __attribute__((weak_import));

		// - Coordinate services ------------------------------------------------
		virtual	void			SetScale( float x, float y );				//was DoSetScale
		virtual	void			SetOrigin( float x, float y );				//was DoSetOrigin
		virtual	void			OffsetOrigin( float x, float y );
		virtual	void			LogicalToDevice( float * x, float * y ) const;
		virtual	void			DeviceToLogical( float * x, float * y ) const;	
		virtual float			GetXScale() const
									{ return mScaleX; }
		virtual	float			GetYScale() const
									{ return mScaleY; }
		virtual	float			GetXOrigin() const
									{ return mOriginX; }
		virtual	float			GetYOrigin() const
									{ return mOriginY; }
		virtual	void			NotifySize( int inWidth, int inHeight )
									{}
		virtual	int				GetWidth() const
									{ return mPhysicalWidth; }
		virtual	int				GetHeight() const
									{ return mPhysicalHeight; }

		// - Text and music symbols services -------------------------------------
		virtual void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID );	//was DrawSymbol
		virtual	void			DrawString( float x, float y, const char * s, int inCharCount );//was PrintAt
		virtual	void			SetFontColor( const VGColor & inColor )		//was DoSetTextColor {}
									{ mTextColor = inColor; }
		virtual	VGColor			GetFontColor() const 
									{ return mTextColor; }
		virtual	void			SetFontBackgroundColor( const VGColor & inColor ) //was DoSetTextBackgroundColor {}
									{ mTextBackColor = inColor; }
		virtual	VGColor			GetFontBackgroundColor() const
									{ return mTextBackColor; }
		virtual	void			SetFontAlign( unsigned int inAlign ) 
									{ mTextAlign = inAlign; }
		virtual	unsigned int	GetFontAlign() const
									{ return mTextAlign; }

		// - Printer informations services ----------------------------------------
		virtual	void			SetDPITag( float inDPI ) 
									{ mDPITag = inDPI; }
		virtual	float			GetDPITag() const 
									{ return 72.0f; }
		virtual void*			GetBitMapPixels()  
									{ return NULL; }
		virtual void			ReleaseBitMapPixels()  
									{};
		virtual	VGSystem *		getVGSystem() const 
									{ return mSys; }

		// - Data export services ----------------------------------------
/*		virtual	void			ExportToFile( const char * inFilePath ) 
									{}
*/		
				void *			GetNativeContext() const;	 

		/// Creates a Carbon color from the input VGColor
		static void				MakeCGColor( const VGColor & inColor, CGFloat outColor[ 4 ] ); 

	protected:

		// - Generic device's state ------------------
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
				unsigned int 		textAlign;
				const VGFont *		currTextFont;
				const VGFont *		currMusicFont;
				VRasterOpMode		rasterMode;
			};

					
		// - Generic VGDevice ------------------
		std::stack<VGPen> 		mPenStack;
		std::stack<VGColor> 	mBrushStack;
		std::stack<GState>		mStateStack;
	
				const VGFont *	mCurrTextFont;
				const VGFont *	mCurrMusicFont;
				
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
		
		// - GDeviceOSX specific ------------------
				float			CoordToDegree( float x, float y );
				void			Init();

		CGColorSpaceRef			mColorSpace;
		CGContextRef 			mContext;
				CGPoint			mCurrPenPos;
			
	public:
		// - VGDevice extension
		virtual	void			SelectPenColor(const VGColor & inColor);
		virtual	void			SelectPenWidth(float witdh);
		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();

	protected:
		std::stack<VGColor> 	mPenColorStack;
		std::stack<float> 		mPenWidthStack;
};

#endif



