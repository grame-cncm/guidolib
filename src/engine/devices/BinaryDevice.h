#ifndef __BinaryDevice__
#define __BinaryDevice__

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

class BinarySystem;
// --------------------------------------------------------------
// 		BinaryDevice class
// --------------------------------------------------------------
/** \brief An abstract drawing device, for use with higher level devices.
 ** outputs commands and their passed parameters in a binary storage format
*/
class_export BinaryDevice : public VGDevice
{
	BinarySystem*		fSystem;
	std::ostream&	fStream;
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
	
	void		writeColor (const VGColor& color) const;
	void		writeString (const char *str) const;
	void		writeFormattedImage (VGDevice* pSrcDC) const;
	void		writeRasterOpModeToString(VRasterOpMode mode) const;
	void		writeFont(const VGFont *font) const;
	public:
				 BinaryDevice(std::ostream& outstream, BinarySystem* system);
		virtual	~BinaryDevice();

		/// Returns the ability of the current VGdevice to be drawn into.
		virtual bool			IsValid() const		{ return true; }


		// - Drawing services ------------------------------------------------
		// 0
		virtual bool	BeginDraw();
		// 1
		virtual void	EndDraw();
		// 2
		virtual void	InvalidateRect( float left, float top, float right, float bottom );

		// - Standard graphic primitives -------------------------
		// 3
		virtual void	MoveTo( float x, float y );
		// 4
		virtual void	LineTo( float x, float y );
		// 5
		virtual void	Line( float x1, float y1, float x2, float y2 );
		// 6
		virtual void	Frame( float left, float top, float right, float bottom );
		// 7
		virtual void	Arc( float left,   float top, float right,  float bottom,
									 float startX, float startY, float endX,   float endY );

		// - Filled surfaces --------------------------------------
		// 8
		virtual	void	Triangle( float x1, float y1, float x2, float y2, float x3, float y3 );
		// 9
		virtual	void	Polygon( const float * xCoords, const float * yCoords, int count );
		// 10
		virtual void	Rectangle( float left,  float top, float right, float bottom );

		// - Font services ---------------------------------------------------
		// 11
		virtual	void			SetMusicFont( const VGFont * font );
		// 12
		virtual	const VGFont *	GetMusicFont() const;
		// 13
		virtual	void			SetTextFont( const VGFont * font );
		// 14
		virtual	const VGFont *	GetTextFont() const;

		// - Pen & brush services --------------------------------------------
		// 15
		virtual	void			SelectPen( const VGColor & inColor, float witdh );
		// 16
		virtual	void			SelectFillColor( const VGColor & c );
		// 17
		virtual	void			PushPen( const VGColor & inColor, float inWidth );
		// 18
		virtual	void			PopPen();
		// 19
		virtual	void			PushFillColor( const VGColor & inColor );
		// 20
		virtual	void			PopFillColor();
		// 21
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode);
		// 22
		virtual	VRasterOpMode	GetRasterOpMode() const;


		// - Bitmap services (bit-block copy methods) --------------------------
		// 23
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0);
		// 24
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);
		// 25
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight,
											VGDevice* pSrcDC, float alpha = -1.0);
		// 26
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight,
											VGDevice* pSrcDC, int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);

		// - Coordinate services ------------------------------------------------
		// 27
		virtual	void			SetScale( float x, float y );
		// 28
		virtual	void			SetOrigin( float x, float y );
		//29 
		virtual	void			OffsetOrigin( float x, float y );
		// 30
		virtual	void			LogicalToDevice( float * x, float * y ) const;
		// 31
		virtual	void			DeviceToLogical( float * x, float * y ) const;

		// 32
		virtual float			GetXScale() const;
		// 33
		virtual	float			GetYScale() const;
		// 34
		virtual	float			GetXOrigin() const;
		// 35
		virtual	float			GetYOrigin() const;

		// 36
		virtual	void			NotifySize( int inWidth, int inHeight );
		// 37
		virtual	int				GetWidth() const;
		// 38
		virtual	int				GetHeight() const;


		// - Text and music symbols services -------------------------------------
		// 39
		virtual void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID );
		// 40
		virtual	void			DrawString( float x, float y, const char * s, int inCharCount );

		// 41
		virtual	void			SetFontColor( const VGColor & inColor );
		// 42
		virtual	VGColor			GetFontColor() const;
		// 43
		virtual	void			SetFontBackgroundColor( const VGColor & inColor );
		// 44
		virtual	VGColor			GetFontBackgroundColor() const;
		// 45
		virtual	void			SetFontAlign( unsigned int inAlign );
		// 46
		virtual	unsigned int	GetFontAlign() const;

		// - Printer informations services ----------------------------------------
		// 47
		virtual	void			SetDPITag( float inDPI );
		// 48
		virtual	float			GetDPITag() const;

		// ----------------------- never needed in abstract device ------------------
		virtual void*			GetBitMapPixels()		{ return 0; }
		virtual void			ReleaseBitMapPixels()	{}
		virtual const char*		GetImageData(const char* & outDataPtr, int& outLength)	{ return 0; };
		virtual void			ReleaseImageData(const char *) const					{}

		/// temporary hack - must be removed asap
		virtual	VGSystem *		getVGSystem() const		{ return (VGSystem *)fSystem; }

		// - Data export services ----------------------------------------
		virtual void *			GetNativeContext() const	{ return 0; }
		// ----------------------- end "never needed in abstract device" ------------------


		// - VGDevice extension --------------------------------------------
		// 49
		virtual	void			SelectPenColor( const VGColor & inColor);
		// 50
		virtual	void			SelectPenWidth( float witdh);
		// 51
		virtual	void			PushPenColor( const VGColor & inColor);
		// 52
		virtual	void			PopPenColor();		
		// 53
		virtual	void			PushPenWidth( float width);
		// 54
		virtual	void			PopPenWidth();
};

/*! @} */

#endif
