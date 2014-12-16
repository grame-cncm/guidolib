#ifndef __CairoDevice__
#define __CairoDevice__

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

#include <stack>
#include <cairo.h>

#include "VGDevice.h"
#include "VGColor.h"

class VGFont;
class VGSystem;


// --------------------------------------------------------------
// 		A Cairo based graphic device
// --------------------------------------------------------------
class CairoDevice : public VGDevice
{
	private:
		std::stack<VGColor> 	fFillColorStack;
		std::stack<VGColor> 	fPenColorStack;
		std::stack<double> 		fPenWidthStack;

		cairo_t *			fNativeDevice;
		VGSystem *			fSystem;
		const VGFont*		fTextFont;
		const VGFont*		fMusicFont;
		VGColor				fFillColor;
		VGColor				fPenColor;
		VGColor				fTextColor;
		VGColor				fTextBackground;
		unsigned int		fTextAlign;
		
	public:
					 CairoDevice (cairo_t * dev, VGSystem * sys=0);
					 CairoDevice (int width, int height, VGSystem *sys=0);
		virtual		~CairoDevice();

		/// Returns the ability of the current VGdevice to be drawn into.
		virtual bool			IsValid() const		{ return fNativeDevice != 0; }

		// - Drawing services ------------------------------------------------
		virtual bool			BeginDraw();
		virtual void			EndDraw();
		virtual void			InvalidateRect( float left, float top, float right, float bottom );

		// - Standard graphic primitives -------------------------
		virtual void			MoveTo( float x, float y );
		virtual void			LineTo( float x, float y );
		virtual void			Line( float x1, float y1, float x2, float y2 );
		virtual void			Frame( float left, float top, float right, float bottom );
		virtual void			Arc( float left,   float top, float right,  float bottom,
									 float startX, float startY, float endX,   float endY );

		// - Filled surfaces --------------------------------------
		// The raster op mode for color filling should be specified with SetRasterOpMode() before using one of these.
		virtual	void			Triangle( float x1, float y1, float x2, float y2, float x3, float y3 );
		virtual	void			Polygon( const float * xCoords, const float * yCoords, int count );
		virtual void			Rectangle( float left,  float top, float right, float bottom );

		// - Font services ---------------------------------------------------
		virtual	void			SetMusicFont( const VGFont * font );
		virtual	void			SetTextFont( const VGFont * font );
		virtual	const VGFont *	GetMusicFont() const	{ return fMusicFont; }
		virtual	const VGFont *	GetTextFont() const		{ return fTextFont; }

		// - Pen & brush services --------------------------------------------
		virtual	void			SelectPen( const VGColor & inColor, float witdh );
		virtual	void			SelectFillColor( const VGColor & c );
		virtual	void			PushPen( const VGColor & inColor, float inWidth );
		virtual	void			PopPen();
		virtual	void			PushFillColor( const VGColor & inColor );
		virtual	void			PopFillColor();
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode);
		virtual	VRasterOpMode	GetRasterOpMode() const;


		// - Bitmap services (bit-block copy methods) --------------------------
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0);
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC,
											int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0);
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight,
											VGDevice* pSrcDC, float alpha = -1.0);
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC,
											int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0);

		// - Coordinate services ------------------------------------------------
		virtual	void			SetScale( float x, float y );
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
		virtual	void			DrawString( float x, float y, const char * s, int inCharCount );
		virtual	void			SetFontColor( const VGColor & inColor );
		virtual	VGColor			GetFontColor() const;
		virtual	void			SetFontBackgroundColor( const VGColor & inColor );
		virtual	VGColor			GetFontBackgroundColor() const;
		virtual	void			SetFontAlign( unsigned int inAlign );
		virtual	unsigned int	GetFontAlign() const;

		// - Printer informations services ----------------------------------------
		virtual	void			SetDPITag( float inDPI );
		virtual	float			GetDPITag() const;
		virtual void*			GetBitMapPixels();
		virtual void			ReleaseBitMapPixels();
		virtual const char*		GetImageData(const char* & outDataPtr, int& outLength) { return 0; }
		virtual void			ReleaseImageData(const char *) const  {}


		/// temporary hack - must be removed asap
		virtual	VGSystem *		getVGSystem() const				{ return fSystem; }
		/// Returns the platform-specific device context object.
		virtual void *			GetNativeContext() const		{ return fNativeDevice; }

		// - VGDevice extension --------------------------------------------
		virtual	void			SelectPenColor( const VGColor & inColor);
		virtual	void			SelectPenWidth( float witdh);
		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();		
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();

	private:
		float		CoordToRadian( float x, float y );
		void		SelectFont( const VGFont * font );
		void		ShowText( const VGFont* font, float x, float y, const char * s, int count );
};

#endif
