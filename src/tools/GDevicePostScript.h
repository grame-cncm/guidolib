#ifndef GDevicePostScript_H
#define GDevicePostScript_H

/*
  GUIDO Library
  Copyright (C) 2003-2006 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/////////////////////////////////////////////////////////////////
///
/// 	PostScript implementation of VGDevice.
///		This device doesn't use a specific PostScript VGFont but
///		rather any suitable platform-dependant VGFont (w32,osx,...)
///
///		For inherited methods documentation, see VGDevice.h
/////////////////////////////////////////////////////////////////

#ifdef WIN32
#pragma warning (disable : 4251 4275) 
#endif

#include <cstdio>
#include <string>
#include <vector>
#include <stack>

#include "GUIDOExport.h"
#include "VGDevice.h"
#include "VGPen.h"

// --------------------------------------------------------------
class_export GDevicePostScript : public VGDevice
{
	public:
								GDevicePostScript( int inWidth, int inHeight,
												   const char * inPSFilePath, 
												   const char * inCreatorStr = "", 
												   const char * inHeaderFilePath = "" );
		virtual					~GDevicePostScript();

		virtual bool			IsValid() const
									{ return true; }	

		// - Drawing services ------------------------------------------------
		virtual bool			BeginDraw();
		virtual void			EndDraw();
		virtual void			InvalidateRect( float left, float top, float right, float bottom ) 
									{}
		
					// - Standard graphic primitives -------------------------
		virtual void			MoveTo( float x, float y );
		virtual void			LineTo( float x, float y );
		virtual void			Line( float x1, float y1, float x2, float y2 );
		virtual void			Frame( float left, float top, float right, float bottom ) 
									{}
		virtual void			Arc( float left,   float top, float right,  float bottom, float startX, float startY, float endX, float endY );

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
		virtual	void			SelectPen( const VGColor & inColor, float width = 1 )  {} 
		virtual	void			SelectFillColor( const VGColor & c )  {} 
		virtual	void			PushPen( const VGColor & inColor, float inWidth = 1 ); 
		virtual	void			PopPen(); 
		virtual	void			PushFillColor( const VGColor & inColor ); 
		virtual	void			PopFillColor(); 
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode)  
									{ mRasterMode = ROpMode; } 
		virtual	VRasterOpMode	GetRasterOpMode() const 
									{ return mRasterMode; }


		// - Bitmap services (bit-block copy methods) --------------------------
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = 0 ) 
									{ return false; }
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = 0 )	
									{ return false; }
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha = 0 ) 
									{ return false; }
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0) 
									{ return false; }

		// - Coordinate services ------------------------------------------------
		virtual	void			SetScale( float x, float y );
		virtual	void			SetOrigin( float x, float y );
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
									{ mPhysicalWidth = inWidth; mPhysicalHeight = inHeight; }
		virtual	int				GetWidth() const
									{ return mPhysicalWidth; }
		virtual	int				GetHeight() const
									{ return mPhysicalHeight; }

		// - Text and music symbols services -------------------------------------
		virtual void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID );
		virtual	void			DrawString( float x, float y, const char * s, int inCharCount = -1 );
		virtual	void			SetFontColor( const VGColor & inColor ); 
		virtual	VGColor			GetFontColor() const 
									{ return mTextColor; }
		virtual	void			SetFontBackgroundColor( const VGColor & inColor ) 
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
									{}
		
		/// temporary hack - must be removed asap
		virtual	VGSystem *		getVGSystem() const 
									{ return NULL; }

		// - Data export services ----------------------------------------
/*		virtual	void			ExportToFile( const char * inFilePath ) // to be removed
									{} */
	protected:

		virtual void *			GetNativeContext() const;	 

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
					float				winOriginX;
					float				winOriginY;
					unsigned int 		textAlign;
					const VGFont *		currTextFont;
					const VGFont *		currMusicFont;
					VRasterOpMode		rasterMode;
				};


	private:
	
				void			SetGrayColor( float inValue ) const;
				void			SetGrayColor( const VGColor & col ) const;

				void			PSMoveTo( float x, float y );
				void			PSAddRectangleToPath( float left, float top, float right, float bottom );
		
				class PSPoint 
				{
					public:
							PSPoint( float ix = 0, float iy = 0 ) : x( ix ), y( iy ) { }
						float x;
						float y;
				};

				// - minimum VGDevice implementation attributes ----

		std::vector<VGPen> 		mPenStack;
		std::vector<VGColor> 	mBrushStack;
		std::vector<GState> 	mStateStack;

		
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
		unsigned int 			mTextAlign;
		
				// printing
				float			mDPITag;
				
				// begin/end Draw utils
				int				mBeginDrawCount;

				/// temporary hack - must be removed asap
//				VGSystem *		mSys;
				
				// - GDevicePostScript specific --------------------
		
				FILE *			GetFile() const { return mFile; }
				void			PSReset();
				void			PSIncludeHeaderFile();
		
				std::string		mPSFilePath;
				std::string		mCreatorStr;
				std::string		mHeaderFilePath;
				bool 			mDrawing;
				FILE *			mFile;

				PSPoint 		mCurrPenPos;
				float			mCurrGrayColor;
				bool			mMoveToSet;     // because moveto is reseted after operations like 'stroke'

	public:
		// - VGDevice extension
		virtual	void			SelectPenColor(const VGColor & inColor) {}
		virtual	void			SelectPenWidth(float witdh) {}

		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();
		
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();

	private:
		std::stack<float> 		mPenWidthStack;
		std::stack<VGColor> 	mPenColorStack;
};


#endif



