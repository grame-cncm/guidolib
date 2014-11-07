#ifndef __AbstractDevice__
#define __AbstractDevice__

/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOExport.h"

#ifdef WIN32
# pragma warning (disable : 4275 4251)
#endif

#include "VGDevice.h"
#include <ostream>
#include <fstream>
#include <string>

/*!
\addtogroup VGSys Virtual Graphic System
@{
*/

class AbstractSystem;
// --------------------------------------------------------------
// 		AbstractDevice class
// --------------------------------------------------------------
/** \brief An abstract drawing device, for use with higher level devices.
 ** outputs commands and their passed parameters
 ** represents images, colors and fonts via their important parameters
 ** more comments on this below
*/
class_export AbstractDevice : public VGDevice
{
	AbstractSystem*		fSystem;
	std::ostream&	fStream;
	std::string		fSpace;
    int fWidth;
    int fHeight;
    const VGFont * fMusicFont;
    const VGFont * fTextFont;
    VRasterOpMode fOpMode;
    float fXScale;
    float fYScale;
    float fXOrigin;
    float fYOrigin;
    unsigned int fFontAlign;
    float fDPI;
    VGColor fFontColor;
    VGColor fFontBackgroundColor;
	
	// color is written in the abstract device as four values
	// from 0 to 255 that represent ARGB
	void		writeColor (const VGColor& color) const;
	// formatted images (bitmaps) are written in the form:
	// Width Height Mimetype Length(inBytes) bytes
	// for example
	// 20 30 image/png 40 #@#%!@!@%%
	void		writeFormattedImage (VGDevice* pSrcDC) const;
	// translates RasterOpMode enum to a human readable string
	void		writeRasterOpModeToString(VRasterOpMode mode) const;
	// writes the font's name, size and properties
	// properties are an integer corresponding to Guido's internal
	// properties representation
	void		writeFont(const VGFont *font) const;
	public:
				 AbstractDevice(std::ostream& outstream, AbstractSystem* system);
		virtual	~AbstractDevice();

		/// Returns the ability of the current VGdevice to be drawn into.
		virtual bool			IsValid() const		{ return true; }


		// - Drawing services ------------------------------------------------
		virtual bool	BeginDraw();
		virtual void	EndDraw();

		virtual void	InvalidateRect( float left, float top, float right, float bottom );

		// - Standard graphic primitives -------------------------
		virtual void	MoveTo( float x, float y );
		virtual void	LineTo( float x, float y );
		virtual void	Line( float x1, float y1, float x2, float y2 );
		virtual void	Frame( float left, float top, float right, float bottom );
		virtual void	Arc( float left,   float top, float right,  float bottom,
									 float startX, float startY, float endX,   float endY );

		// - Filled surfaces --------------------------------------
		virtual	void	Triangle( float x1, float y1, float x2, float y2, float x3, float y3 );
		virtual	void	Polygon( const float * xCoords, const float * yCoords, int count );
		virtual void	Rectangle( float left,  float top, float right, float bottom );

		// - Font services ---------------------------------------------------
		virtual	void			SetMusicFont( const VGFont * font );
		virtual	const VGFont *	GetMusicFont() const;
		virtual	void			SetTextFont( const VGFont * font );
		virtual	const VGFont *	GetTextFont() const;

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
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight,
											VGDevice* pSrcDC, float alpha = -1.0);
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight,
											VGDevice* pSrcDC, int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);

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

		virtual void*			GetBitMapPixels()		{ return 0; }
		virtual void			ReleaseBitMapPixels()	{}
		virtual const char*		GetImageData(const char* & outDataPtr, int& outLength)	{ return 0; };
		virtual void			ReleaseImageData(const char *) const					{}

		/// temporary hack - must be removed asap
		virtual	VGSystem *		getVGSystem() const		{ return (VGSystem *)fSystem; }

		// - Data export services ----------------------------------------
		virtual void *			GetNativeContext() const	{ return 0; }


		// - VGDevice extension --------------------------------------------
		virtual	void			SelectPenColor( const VGColor & inColor);
		virtual	void			SelectPenWidth( float witdh);
		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();		
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();
};

/*! @} */

#endif
