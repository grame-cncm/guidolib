/*
 * GDeviceQt.h
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * GNU Lesser General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU Lesser
 * General Public License version 2.1 as published by the Free Software
 * Foundation and appearing in the file LICENSE.LGPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU Lesser General Public License version 2.1 requirements
 * will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef GDEVICEQT_H
#define GDEVICEQT_H

#include "VGDevice.h"

#include <QPaintDevice>
#include <QPoint>
#include <QFontDatabase>
#include <QPen>
#include <QStack>

class GSystemQt;
class QPainter;
class GFontQt;
class TCachedFontDestructor;
/**
*	\brief Qt implementation of the VGDevice interface, more precisely : a wrapper between
*	the VGDevice and the QPainter objects.
*
*	\warning Only the methods needed by the Guido Engine are implemented.
*/
class GDeviceQt: public VGDevice
{
	
	public:
	
		GDeviceQt(QPainter * qPainter , GSystemQt * sys);
		~GDeviceQt();
		
		bool IsValid() const;
		
		bool BeginDraw();
		void EndDraw();
		void InvalidateRect( float left, float top, float right, float bottom );
		
		void MoveTo( float x, float y );
		void LineTo( float x, float y );
		void Line( float x1, float y1, float x2, float y2 );
		void Frame( float left, float top, float right, float bottom );
		void Arc( float left,   float top, 	float right,  float bottom, 
					float startX, float startY,	float endX,   float endY );
		
		void Triangle( float x1, float y1, 
				float x2, float y2, 
				float x3, float y3 );
		void Polygon( const float * xCoords, const float * yCoords, int count);
		void Rectangle( float left,  float top, 
				float right, float bottom );
		
		void			SetMusicFont( const VGFont * font );
		const VGFont *	GetMusicFont() const;
		void			SetTextFont( const VGFont * font );
		const VGFont *	GetTextFont() const;
		
		void SelectPen( const VGColor & inColor, float width );
		void SelectFillColor( const VGColor & c );
		void PushPen( const VGColor & inColor, float inWidth );
		void PopPen();
		void PushFillColor( const VGColor & inColor );
		void PopFillColor();
		void SetRasterOpMode( VRasterOpMode ROpMode);
		VGDevice::VRasterOpMode	GetRasterOpMode() const;
		
		bool CopyPixels( VGDevice* pSrcDC, float alpha = -1.0);
		bool CopyPixels( int xDest, int yDest, VGDevice* pSrcDC, 
				int xSrc, int ySrc, 
				int nSrcWidth, int nSrcHeight, float alpha = -1.0);
		bool CopyPixels( int xDest, int yDest, 
				int dstWidth, int dstHeight, 
				VGDevice* pSrcDC, float alpha = -1.0);
		bool CopyPixels( int xDest, int yDest, 
				int dstWidth, int dstHeight, 
				VGDevice* pSrcDC, 
				int xSrc, int ySrc, 
				int nSrcWidth, int nSrcHeight, float alpha = -1.0);
		
		void  SetScale		 ( float x, float y );
		void  SetOrigin		 ( float x, float y );
		void  OffsetOrigin	 ( float x, float y );
		void  LogicalToDevice( float * x, float * y ) const;
		void  DeviceToLogical( float * x, float * y ) const;
		float GetXScale()  const;
		float GetYScale()  const;
		float GetXOrigin() const;
		float GetYOrigin() const;
		
		void NotifySize( int inWidth, int inHeight );
		int  GetWidth()  const;
		int  GetHeight() const;
		
		void			DrawMusicSymbol(float x, float y, unsigned int inSymbolID );
		void			DrawString( float x, float y, const char * s, int inCharCount );

		void			SetFontColor( const VGColor & inColor );
		VGColor			GetFontColor() const;
		void			SetFontBackgroundColor( const VGColor & inColor );
		VGColor			GetFontBackgroundColor() const;
		void			SetFontAlign( unsigned int inAlign );
		unsigned int	GetFontAlign() const;
		
		void			SetDPITag( float inDPI );
		float			GetDPITag() const;
		
		void*			GetBitMapPixels();
		void			ReleaseBitMapPixels();
		
		VGSystem * getVGSystem() const;
		
		void ExportToFile( const char * inFilePath );
		
		QPainter * painter();

	protected:
	
		void * GetNativeContext() const;
		
		// device's state
/*		struct GState 
		{
			VGColor			textColor;
			float			originX;
			float			originY;
			unsigned int 	textAlign;
			const GFontQt *	currTextFont;
			const GFontQt *	currMusicFont;
			QPoint			penPos;
		};
*/
		struct PenState 
		{
			QColor			color;
			float			width;
		};
			
	private:
		
		QPainter*			mQPainter;

		void				SetupCharEncodingTable();
		float				CoordToDegree( float x, float y );

//		QStack<GState>		mStateStack;
		QStack<QColor>		mFillColorStack;
		QStack<PenState>	mPenStack;
		QStack<QColor>		mPenColorStack;
		QStack<float>		mPenWidthStack;

		const GFontQt *		mCurrTextFont; 
		const GFontQt *		mCurrMusicFont; 

		// text and symbol 
		VGColor				mTextColor; 
		unsigned int 		mTextAlign;
		
		/// temporary hack - must be removed asap
		GSystemQt *			mSys;		

		QPoint			mCurrPenPos; 

		/// Music font cache system.
		static QMap< QString , QMap< unsigned int, QPainterPath > > * mCachedMusicFont;
		/// Optimization stored values
		QString mCurrentFontName;
		float mCurrentFontRatio , mCurrentFontIRatio;
		QBrush mCurrMusicFontBrush;
		QMap< unsigned int, QPainterPath > * mCurrCachedMusicFont;
		
	public:
		// - VGDevice extension
		virtual	void			SelectPenColor(const VGColor & inColor);
		virtual	void			SelectPenWidth(float witdh);
		virtual	void			PushPenColor( const VGColor & inColor);
		virtual	void			PopPenColor();
		virtual	void			PushPenWidth( float width);
		virtual	void			PopPenWidth();
	
	friend class TCachedFontDestructor;
};

#endif /*GDEVICEQT_H*/

