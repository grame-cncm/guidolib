#ifndef __GDeviceGL__
#define __GDeviceGL__

/*
	GUIDO Library
	Copyright (C) 2005,2006  Grame

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
///	OpenGL implementation of VGDevice. (work in progress).
///		
///	Requirement:
///	- FTGL library:			http://homepages.paradise.net.nz/henryj/code/index.html#FTGL
///	- FreeType library:		http://www.freetype.org
///	- OpenGL:				http://www.opengl.org
///
///	For inherited methods documentation, see VGDevice.h
/////////////////////////////////////////////////////////////////

#include <vector>

#include "VGDevice.h"
#include "VGPen.h"

#ifdef WIN32
#include <windows.h>
#endif

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// --------------------------------------------------------------
class GDeviceGL : public VGDevice
{
	public:
								 GDeviceGL(VGSystem* sys);					// Clean constructor
								 GDeviceGL(int fontType, VGSystem* sys);	// Guido Hack constructor
		virtual					~GDeviceGL();

		virtual bool			IsValid() const;

		// - Drawing services ------------------------------------------------
		virtual bool			BeginDraw();	//was DoBeginDraw
		virtual void			EndDraw();		//was DoEndDraw
		virtual void			InvalidateRect( float left, float top, float right, float bottom );

					// - Standard graphic primitives -------------------------
		virtual void			MoveTo( float x, float y );
		virtual void			LineTo( float x, float y );
		virtual void			Line( float x1, float y1, float x2, float y2 );
		virtual void			Frame( float left, float top, float right, float bottom );
		virtual void			Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY );
		virtual void			FrameEllipse( float x, float y, float width, float height);

					// - Filled surfaces --------------------------------------
		virtual void			Ellipse( float x, float y, float width, float height, const VGColor& color);
		virtual	void			Triangle( float x1, float y1, float x2, float y2, float x3, float y3 );
		virtual	void			Polygon( const float * xCoords, const float * yCoords, int count );	
		virtual void			Rectangle( float left,  float top, float right, float bottom );

		// - Font services ---------------------------------------------------
		virtual	void			SetMusicFont( const VGFont * font );		//was SelectFont
		virtual	const VGFont *	GetMusicFont() const ;				  
		virtual	void			SetTextFont( const VGFont * font );
		virtual	const VGFont *	GetTextFont() const	;

		// - Pen & brush services --------------------------------------------
		virtual	void			SelectPen( const VGColor & inColor, float width );
		virtual	void			SelectFillColor( const VGColor & c );
		virtual	void			PushPen( const VGColor & inColor,  float inWidth ); 
		virtual	void			PopPen(); 
		virtual	void			PushFillColor( const VGColor & inColor ); 
		virtual	void			PopFillColor(); 
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode)  ;
		virtual	VRasterOpMode	GetRasterOpMode() const 
									{ return mRasterMode; }

		// - Bitmap services (bit-block copy methods) --------------------------
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0) ;
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0)	;
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha = -1.0) ;
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0);	

		// - Coordinate services ------------------------------------------------
		virtual	void			SetScale( float x, float y );			//was DoSetScale
		virtual	void			SetOrigin( float x, float y );			//was DoSetOrigin
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
		virtual void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID );	//was DrawSymbol
		virtual	void			DrawString( float x, float y, const char * s, int inCharCount );
		virtual	void			SetFontColor( const VGColor & inColor );	//was DoSetTextColor
		virtual	VGColor			GetFontColor() const ;
		virtual	void			SetFontBackgroundColor( const VGColor & inColor ) ;
		virtual	VGColor			GetFontBackgroundColor() const;
		virtual	void			SetFontAlign( unsigned int inAlign ); 
		virtual	unsigned int	GetFontAlign() const;

		// - Printer informations services ----------------------------------------
		virtual	void			SetDPITag( float inDPI ) ;
		virtual	float			GetDPITag() const ;
		virtual	VGSystem *		getVGSystem() const ;

		virtual void*			GetBitMapPixels();
		virtual void			ReleaseBitMapPixels();


		/// Gives the current device data and returns the data associated mime type.
		virtual const char*		GetImageData(const char* & outDataPtr, int& outLength)	{ return 0; }
		/// Release the pointer returned by GetImageData
		virtual void			ReleaseImageData(const char *) const					{ }


		// - VGDevice extension --------------------------------------------
		virtual	void			SelectPenColor( const VGColor & inColor)	{}
		virtual	void			SelectPenWidth( float witdh)				{}
		virtual	void			PushPenColor( const VGColor & inColor)		{}
		virtual	void			PopPenColor()								{}
		virtual	void			PushPenWidth( float width)					{}
		virtual	void			PopPenWidth()								{}


		// - Data export services ----------------------------------------
/*		virtual	void			ExportToFile( const char * inFilePath ) 
									{}
*/
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
			unsigned int 		textAlign;
			const VGFont *		currTextFont;
			const VGFont *		currMusicFont;
			VRasterOpMode		rasterMode;
		};


	private:
		// - Generic device's attributes -------------------------------	
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
			unsigned int 		mTextAlign;
				// printing
				float			mDPITag;
				// begin/end Draw utils
				int				mBeginDrawCount;

				/// temporary hack - must be removed asap
				VGSystem *		mSys;		

		// - OpenGL specific attributes -------------------------------	
				void			TraceRect();
				void			TracePos(const char * msg="TracePos") const;
				void			AAliasing ();

				float			getX (float x) const				{ return x; }
				float			getY (float y) const				{ return -y; }
				float			mCurrPenPosX, mCurrPenPosY;
				bool 			mDrawing;
};


#endif
