#ifndef NullGDevice_H
#define NullGDevice_H

/*
  GUIDO Library
  Copyright (C) 2004-2006 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/////////////////////////////////////////////////////////////////
///
/// This VGDevice is the most basic child object that can be used 
/// to build almost nothing but time-graphic mapping & common 
/// graphical objects dummy initialization. Thus, it shouldn't be 
/// used as a real VGDevice but more as the basic x-platform  
/// "template" VGDevice implementation.
///
/// For inherited methods documentation, see VGDevice.h
/////////////////////////////////////////////////////////////////

#include "VGDevice.h"
#include "NullGFont.h"

// --------------------------------------------------------------
class NullGDevice : public VGDevice
{
	public:
								 NullGDevice(VGSystem * sys) 
													: mSys(sys), mCurrTextFont("textFont", 10, 0), mCurrMusicFont("musicFont", 10, 0)  {}
								 NullGDevice(VGSystem * sys, int inWidth, int inHeight ) 
													: mSys(sys), mCurrTextFont("textFont", 10, 0), mCurrMusicFont("musicFont", 10, 0)  {}
		virtual					~NullGDevice() {}

		virtual bool			IsValid() const { return true; }

		// - Drawing services ------------------------------------------------		
		virtual bool			BeginDraw()		{ return true; }
		virtual void			EndDraw()		{}
		virtual void			InvalidateRect( float left, float top, float right, float bottom ) {}

					// - Standard graphic primitives -------------------------
		virtual void			MoveTo( float x, float y )		{}
		virtual void			LineTo( float x, float y )		{}
		virtual void			Line( float x1, float y1, float x2, float y2 )				{}
		virtual void			Frame( float left, float top, float right, float bottom )	{}
		virtual void			Arc( float left, float top, float right, float bottom, float startX, float startY, float endX, float endY ) {}

					// - Filled surfaces --------------------------------------
		virtual	void			Triangle( float x1, float y1, float x2, float y2, float x3, float y3 )	{}
		virtual	void			Polygon( const float * xCoords, const float * yCoords, int count )		{}
		virtual void			Rectangle( float left,  float top, float right, float bottom )			{}

		// - Font services ---------------------------------------------------
		virtual	void			SetMusicFont( const VGFont * font )		{}
		virtual	const VGFont *	GetMusicFont() const					{ return &mCurrMusicFont; }
		virtual	void			SetTextFont( const VGFont * font )		{}
		virtual	const VGFont *	GetTextFont() const						{ return &mCurrTextFont; }

		// - Pen & brush services --------------------------------------------
		virtual	void			SelectPen( const VGColor & inColor, float width )	{}
		virtual	void			SelectFillColor( const VGColor & c )				{}
		virtual	void			PushPen( const VGColor & inColor, float inWidth )	{}
		virtual	void			PopPen()											{}
		virtual	void			PushFillColor( const VGColor & inColor )			{}
		virtual	void			PopFillColor()										{}
		virtual	void			SetRasterOpMode( VRasterOpMode ROpMode)				{}
		virtual	VRasterOpMode	GetRasterOpMode() const								{ return kOpCopy; }

		// - Bitmap services (bit-block copy methods) --------------------------
		virtual bool			CopyPixels( VGDevice* pSrcDC, float alpha = -1.0 )		{ return true; }
		virtual bool			CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0 )		{ return true; }
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, float alpha = -1.0 )		{ return true; }
		virtual bool			CopyPixels( int xDest, int yDest, int dstWidth, int dstHeight, VGDevice* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, float alpha = -1.0)		{ return true; }

		// - Coordinate services ------------------------------------------------
		virtual	void			SetScale( float x, float y )					{}
		virtual	void			SetOrigin( float x, float y )					{}
		virtual	void			OffsetOrigin( float x, float y )				{}
		virtual	void			LogicalToDevice( float * x, float * y ) const	{}
		virtual	void			DeviceToLogical( float * x, float * y ) const	{}
		virtual float			GetXScale() const	{ return 1.f; }
		virtual	float			GetYScale() const	{ return 1.f; }
		virtual	float			GetXOrigin() const	{ return 0.f; }
		virtual	float			GetYOrigin() const	{ return 0.f; }

		virtual	void			NotifySize( int inWidth, int inHeight )			{}
		virtual	int				GetWidth() const			{ return 500; }
		virtual	int				GetHeight() const			{ return 1000; }

		// - Text and music symbols services -------------------------------------
		virtual void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID )			{}
		virtual	void			DrawString( float x, float y, const char * s, int inCharCount )		{}
		virtual	void			SetFontColor( const VGColor & inColor )								{}
		virtual	VGColor			GetFontColor() const												{ return VGColor(50); }
		virtual	void			SetFontBackgroundColor( const VGColor & inColor ) 					{}
		virtual	VGColor			GetFontBackgroundColor() const										{ return VGColor(50); }
		virtual	void			SetFontAlign( unsigned int inAlign )								{}
		virtual	unsigned int	GetFontAlign() const												{ return kAlignBase; }

		// - Printer informations services ----------------------------------------
		virtual	void			SetDPITag( float inDPI = 72.0f )	{}
		virtual	float			GetDPITag() const					{ return 72.f; }
		virtual void*			GetBitMapPixels()					{ return 0; }
		virtual void			ReleaseBitMapPixels()				{}

		/// Gives the current device data and returns the data associated mime type.
		virtual const char*		GetImageData(const char* & outDataPtr, int& outLength) { outLength=0; return 0;}
		/// Release the pointer returned by GetImageData
		virtual void			ReleaseImageData(const char *) const {}

		/// temporary hack - must be removed asap
		virtual	VGSystem *		getVGSystem() const					{ return mSys; }

	protected:
		/// Returns the platform-specific device context object.
		virtual void *			GetNativeContext() const			{ return 0; }

	private:
		VGSystem*   mSys;
		NullGFont	mCurrTextFont;
		NullGFont	mCurrMusicFont;

	public:
		// VGDevice extension
		virtual	void			SelectPenColor(const VGColor & inColor)		{}
		virtual	void			SelectPenWidth(float witdh)					{}

		virtual	void			PushPenColor( const VGColor & inColor)		{}
		virtual	void			PopPenColor()								{}
		
		virtual	void			PushPenWidth( float width)					{}
		virtual	void			PopPenWidth()								{}
};


#endif



