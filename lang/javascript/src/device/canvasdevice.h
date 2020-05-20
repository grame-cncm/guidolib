#ifndef CANVASDEVICE_H
#define CANVASDEVICE_H

/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "VGDevice.h"
#include "VGFont.h"
#include "VGColor.h"

#include <emscripten.h>
#include <ostream>

/*!
\addtogroup VGSysJavascript Virtual Graphic System
@{
*/

class CanvasSystem;

/** \brief A device use in web context to draw on a html canvas.
 * The canvas must be initialised with the right variable (see jsCanvasDevice.js)
 **
*/
class CanvasDevice : public VGDevice
{
	public:
		/*!
		 * \brief CanvasDevice
		 * \param system a CanvasSystem
		 */
		CanvasDevice(CanvasSystem *system);
		virtual	~CanvasDevice();

		/// Returns the ability of the current VGdevice to be drawn into.
		bool	IsValid() const		{ return true; }


		// - Drawing services ------------------------------------------------
		// 0
		bool	BeginDraw();
		// 1
		void	EndDraw();
		// 2
		void	InvalidateRect(float left, float top, float right, float bottom);

		// - Standard graphic primitives -------------------------
		// 3
		void	MoveTo(float x, float y);
		// 4
		void	LineTo(float x, float y);
		// 5
		void	Line(float x1, float y1, float x2, float y2);
		// 6
		void	Frame(float left, float top, float right, float bottom);
		// 7
		void	Arc(float left,   float top, float right,  float bottom,
									 float startX, float startY, float endX, float endY);

		// - Filled surfaces --------------------------------------
		// 8
		void	Triangle(float x1, float y1, float x2, float y2, float x3, float y3);
		// 9
		void	Polygon(const float * xCoords, const float * yCoords, int count);
		// 10
		void	Rectangle(float left,  float top, float right, float bottom);

		// - Font services ---------------------------------------------------
		// 11
		void			SetMusicFont(const VGFont * font);
		// 12
		const VGFont *	GetMusicFont() const;
		// 13
		void			SetTextFont(const VGFont * font);
		// 14
		const VGFont *	GetTextFont() const;

		// - Pen & brush services --------------------------------------------
		// 15
		void			SelectPen(const VGColor & inColor, float witdh);
		// 16
		void			SelectFillColor(const VGColor & c);
		// 17
		void			PushPen(const VGColor & inColor, float inWidth);
		// 18
		void			PopPen();
		// 19
		void			PushFillColor(const VGColor & inColor);
		// 20
		void			PopFillColor();
		// 21
		void			SetRasterOpMode(VRasterOpMode ROpMode);
		// 22
		VRasterOpMode	GetRasterOpMode() const;


		// - Bitmap services (bit-block copy methods) --------------------------
		// 23
		bool			CopyPixels(VGDevice* pSrcDC, float alpha = -1.0);
		// 24
		bool			CopyPixels(int xDest, int yDest, VGDevice* pSrcDC, int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);
		// 25
		bool			CopyPixels(int xDest, int yDest, int dstWidth, int dstHeight,
											VGDevice* pSrcDC, float alpha = -1.0);
		// 26
		bool			CopyPixels(int xDest, int yDest, int dstWidth, int dstHeight,
											VGDevice* pSrcDC, int xSrc, int ySrc,
											int nSrcWidth, int nSrcHeight, float alpha = -1.0);

		// - Coordinate services ------------------------------------------------
		// 27
		void			SetScale(float x, float y);
		// 28
		void			SetOrigin(float x, float y);
		//29
		void			OffsetOrigin(float x, float y);
		// 30
		void			LogicalToDevice(float * x, float * y) const;
		// 31
		void			DeviceToLogical(float * x, float * y) const;

		// 32
		float			GetXScale() const;
		// 33
		float			GetYScale() const;
		// 34
		float			GetXOrigin() const;
		// 35
		float			GetYOrigin() const;

		// 36
		void			NotifySize(int inWidth, int inHeight);
		// 37
		int				GetWidth() const;
		// 38
		int				GetHeight() const;


		// - Text and music symbols services -------------------------------------
		// 39
		void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID);
		// 40
		void			DrawString(float x, float y, const char * s, int inCharCount);

		// 41
		void			SetFontColor(const VGColor & inColor);
		// 42
		VGColor			GetFontColor() const;
		// 43
		void			SetFontBackgroundColor(const VGColor & inColor);
		// 44
		VGColor			GetFontBackgroundColor() const;
		// 45
		void			SetFontAlign(unsigned int inAlign);
		// 46
		unsigned int	GetFontAlign() const;

		// - Printer informations services ----------------------------------------
		// 47
		void			SetDPITag(float inDPI);
		// 48
		float			GetDPITag() const;

		// ----------------------- never needed in abstract device ------------------
		void*			GetBitMapPixels()		{ return 0; }
		void			ReleaseBitMapPixels()	{}
		const char*		GetImageData(const char* & outDataPtr, int& outLength)	{ return 0; }
		void			ReleaseImageData(const char *) const					{}

		VGSystem *		getVGSystem() const		{ return (VGSystem *)fSystem; }

		// - Data export services ----------------------------------------
		void *			GetNativeContext() const	{ return 0; }
		// ----------------------- end "never needed in abstract device" ------------------


		// - VGDevice extension --------------------------------------------
		// 49
		void			SelectPenColor(const VGColor & inColor);
		// 50
		void			SelectPenWidth(float witdh);
		// 51
		void			PushPenColor(const VGColor & inColor);
		// 52
		void			PopPenColor();
		// 53
		void			PushPenWidth(float width);
		// 54
		void			PopPenWidth();


		// - VGDevice extension --------------------------------------------
		// 55
		void			FrameEllipse( float x, float y, float width, float height);
		// 56
		void			Ellipse( float x, float y, float width, float height, const VGColor& color);

	private:
		CanvasSystem * fSystem;

		VRasterOpMode fOpMode;

		unsigned int inAlign;
};

/*! @} */

#endif // CANVASDEVICE_H
