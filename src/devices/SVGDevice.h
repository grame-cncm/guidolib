#ifndef __SVGDevice__
#define __SVGDevice__

/*
	GUIDO Library
	Copyright (C) 2011	Grame

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

//______________________________________________________________________________
/*!
\internal
\brief to be used in place of std::endl
	to provide a correct indentation of the xml output.
*/
class svgendl {
	private:
		int fIndent;
	public:
				 svgendl() : fIndent(0) {}
		virtual ~svgendl() {}

		//! increase the indentation
		svgendl& operator++ (int)  { fIndent++; return *this; }
		//! decrease the indentation
		svgendl& operator-- (int)  { fIndent--; return *this; }
		//! reset the indentation to none
		void print(std::ostream& os) const;
};
std::ostream& operator<< (std::ostream& os, const svgendl& eol);


class SVGSystem;
// --------------------------------------------------------------
// 		SVGDevice class
// --------------------------------------------------------------
/** \brief A SVG based drawing device.
*/
class_export SVGDevice : public VGDevice
{
	SVGSystem*		fSystem;
	const char*		fGuidoFontFile;
	std::ostream&	fStream;
	svgendl			fEndl;
	int				fWidth, fHeight;
	const VGFont *	fMusicFont;
	const VGFont *	fTextFont;
	VRasterOpMode	fOpMode;
	float			fXScale, fYScale, fXOrigin, fYOrigin, fXPos, fYPos;
	VGColor			fFontColor, fFontBackgroundColor;
	int				fFontAlign;
	float			fDPI;
	
	bool		fPushedPen, fPushedPenColor, fPushedPenWidth, fPushedFill, fScaled, fOffset;
	int			fCurrFont;
	VGColor	*	fPendingStrokeColor;
	bool		fBeginDone;
	
	enum		{ kNoFont, kMusicFont, kTextFont };

	void		print (std::ostream& out, const VGColor& color) const;
	void		printFont (std::ostream& out, const char* file) const;
	void		getsvgfont (const char* ptr, std::string& str) const;
	float		alpha2float (const VGColor& color) const;
	void		selectfont (int font);
	void		checkfont ();
	void		closegroup ();
	const char* align2str (int align) const;
	const char* baseline2str (int align) const;
	
	public:
		enum	{ kSVGSizeDivider = 8 };		// used to compute the svg view size GuidoSVGExport and GuidoGetSVGMap
				 SVGDevice(std::ostream& outstream, SVGSystem* system, const char* guidofontfile=0);
		virtual	~SVGDevice();

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
