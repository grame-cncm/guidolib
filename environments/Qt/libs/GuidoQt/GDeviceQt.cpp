/*
 * GDeviceQt.cpp
 *
 * Created by Christophe Daudin on 12/05/09.
 * Copyright 2009 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */

#include "GSystemQt.h"
#include "GDeviceQt.h"
#include "GFontQt.h"
#include <QPolygonF>
#include <QRectF>
#include <QLineF>
#include <math.h>
#include <QPainter>
#include <QPaintDevice>
#include <QWidget>
#ifndef __MOBILE__
#include <QPrinter>
#endif
#include <QImage>
#include <QBuffer>

#include <QVariant>
#include <QtDebug>
#include <QtGlobal>

#include <assert.h>
#include <iostream>

using namespace std;

#define PI 3.1415926536f

//#define USE_CACHED_MUSIC_FONT
#define CACHED_FONT_SIZE 200


#ifdef USE_CACHED_MUSIC_FONT
class TCachedFontDestructor
{
	public:
				 TCachedFontDestructor() {}
		virtual ~TCachedFontDestructor() { delete GDeviceQt::mCachedMusicFont; }
};

TCachedFontDestructor gCleanCacheAtExit;
#endif

QMap< QString , QMap< unsigned int, QPainterPath > > * GDeviceQt::mCachedMusicFont = 0;

//------------------------------------------- GRAPHIC FUNCTIONS ------------------------------------------------------//
//
//
GDeviceQt::GDeviceQt(QPainter * qPainter , GSystemQt * sys)
		: 
		mCurrTextFont(NULL), mCurrMusicFont(NULL), mScaleX(1), mScaleY(1), mCurrPenPos(0,0)

{
#ifdef USE_CACHED_MUSIC_FONT
	// Static member initialization
	if ( !mCachedMusicFont )
	{
		mCachedMusicFont = new QMap< QString , QMap< unsigned int, QPainterPath > >();
	}
#endif
	
	mSys=sys;
	mQPainter = qPainter;
	
	if ( qPainter )
	{
		SelectPen( VGColor( 0, 0, 0), 1 );
		SelectFillColor( VGColor( 255, 255, 255) );
		SetFontColor( VGColor(0,0,0) );
		SetFontBackgroundColor( VGColor(255,255,255,ALPHA_TRANSPARENT) );	
		SetDPITag(72.0f);
		SetFontAlign( kAlignLeft | kAlignBase );

		MoveTo(0,0);
	}
}
	
//-----------------------------------------------------------------------
GDeviceQt::~GDeviceQt()
{
	//
}

//-----------------------------------------------------------------------
bool GDeviceQt::IsValid()const
{	
	return ( mQPainter != NULL ); 
}

//-----------------------------------------------------------------------
bool GDeviceQt::BeginDraw()
{
	// general device's state save - was VGDevice::SaveDC()
/*	
	GState			s;
	s.textColor		= mTextColor;
	s.textAlign		= mTextAlign;
	s.currTextFont	= mCurrTextFont;
	s.currMusicFont = mCurrMusicFont;
	s.penPos		= mCurrPenPos;
	
	mStateStack.push( s );
*/
	mQPainter->save();
	return true;
};

//-----------------------------------------------------------------------
void GDeviceQt::EndDraw()
{
	// general device's state restore - was RestoreDC()
/*
	GState & s = mStateStack.top();

	mTextColor		= s.textColor;	
	mTextAlign		= s.textAlign;
	mCurrTextFont	= s.currTextFont;
	mCurrMusicFont	= s.currMusicFont;
	mCurrPenPos		= s.penPos;

	mStateStack.pop();
*/		
	mQPainter->restore();
};

//-----------------------------------------------------------------------
void GDeviceQt::InvalidateRect( float left, float top, 
		float right, float bottom )
{	
	QWidget * device = dynamic_cast<QWidget *>( mQPainter->device() );
	if ( device != 0 )
	{
		device->update( left , top , (right - left) , (bottom - top) );
	}
}

//-----------------------------------------------------------------------
void GDeviceQt::MoveTo( float x, float y )
{
	mCurrPenPos.setX(int(x));
    mCurrPenPos.setY(int(y));
}

//-----------------------------------------------------------------------
void GDeviceQt::LineTo( float x, float y )
{
	mQPainter->drawLine( QLineF(mCurrPenPos.x(),mCurrPenPos.y(),x,y) );
	MoveTo(x , y);
}

//-----------------------------------------------------------------------
void GDeviceQt::Line( float x1, float y1, 
		float x2, float y2 )
{
	MoveTo( x1, y1 );
	LineTo( x2, y2 );
}

//-----------------------------------------------------------------------
void GDeviceQt::Frame( float left, float top, 
		float right, float bottom )
{
	MoveTo (left, top);
	LineTo (right, top);
	LineTo (right, bottom);
	LineTo (left, bottom);
	LineTo (left, top);	
}

// --------------------------------------------------------------
// Return an angle from 0 to 360
float GDeviceQt::CoordToDegree( float x, float y )
{
	const float kRadToDeg = 180 / PI;
	float outAngle = (float)atan( y / x ) * kRadToDeg;
	if ( x < 0 )
		outAngle += 180;
	if( outAngle < 0 )
		outAngle += 360;
	else if ( outAngle > 360 )
		outAngle -= 360;

	return outAngle;
}

//-----------------------------------------------------------------------
void GDeviceQt::Arc( float left,   float top, 
		float right,  float bottom, 
		float startX, float startY, 
		float endX,   float endY )
{	
	int width = (int) (right - left);
	int height = (int) (bottom - top);

	float midX = (left + right) * 0.5f;
	float midY = (top + bottom) * 0.5f;

	float startAngle	= CoordToDegree( (startX - midX)/float(width)	, (midY   - startY )	/ float(height)  ) * 16;
	float endAngle		= CoordToDegree( (endX - midX)	/float(width)	, (midY   - endY )		/ float(height)  ) * 16;
	
	int spanAngle = abs(int(endAngle - startAngle));
	startAngle = abs(int(startAngle));

	mQPainter->drawArc(left, top , width , height , startAngle , spanAngle );
}

//-----------------------------------------------------------------------
void GDeviceQt::FrameEllipse( float x, float y, float width, float height)
{
#ifdef WIN32
	QPointF pos(x, y+height/2);
#else
	QPointF pos(x, y);
#endif
	const QBrush &	b = mQPainter->brush();
	mQPainter->setBrush (Qt::NoBrush);
	mQPainter->drawEllipse (pos, width, height);
	mQPainter->setBrush (b);
}

//-----------------------------------------------------------------------
void GDeviceQt::Ellipse( float x, float y, float width, float height, const VGColor& color)
{
#ifdef WIN32
	QPointF pos(x, y+height/2);
#else
	QPointF pos(x, y);
#endif
	PushFillColor (color);
	mQPainter->drawEllipse (pos, width, height);
	PopFillColor ();
}

//-----------------------------------------------------------------------
void GDeviceQt::Triangle( float x1, float y1, 
		float x2, float y2, 
		float x3, float y3 )
{
	QVector<QPointF> points;
	points.append(QPointF(x1,y1));
	points.append(QPointF(x2,y2));
	points.append(QPointF(x3,y3));
	QPolygonF triangle(points);
	
	QPen pen = mQPainter->pen();
	mQPainter->setPen( Qt::NoPen );
	mQPainter->drawPolygon(triangle);
	mQPainter->setPen(pen);
}

//-----------------------------------------------------------------------
void GDeviceQt::Polygon( const float * xCoords, const float * yCoords, int count)
{
	QVector<QPointF> points;
	for ( int i = 0 ; i < count ; i++ )
		points.append(QPointF(xCoords[i] , yCoords[i]));

	QPolygonF polygon(points);

	QPen pen = mQPainter->pen();
	mQPainter->setPen( Qt::NoPen );
	mQPainter->drawPolygon(polygon);
	mQPainter->setPen(pen); 
}

//-----------------------------------------------------------------------
void GDeviceQt::Rectangle( float left,  float top, 
		float right, float bottom )
{
	float fLeft = left;
	float fTop = top;
	float fRight = right;
	float fBottom = bottom;
	
	int w = fRight - fLeft;
	int h = fBottom - fTop;

	QPen pen = mQPainter->pen();
	mQPainter->setPen( Qt::NoPen );
	mQPainter->drawRect( QRectF( int(fLeft) , int(fTop) , w , h ) );
	
	mQPainter->setPen(pen);
}
//------------------------------------------- FONTS FUNCTIONS --------------------------------------------------------//
//
//

//-------------------------------------------------------------
void GDeviceQt::SetMusicFont( const VGFont * font )
{	
	if ( font == mCurrMusicFont )
		return;

	mCurrMusicFont = (GFontQt*)font;
	
#ifdef USE_CACHED_MUSIC_FONT
	mCurrentFontRatio = mCurrMusicFont->GetSize()/float(CACHED_FONT_SIZE);

	if ( mCurrentFontName != QString(font->GetName())  )
	{
		mCurrentFontName = QString( font->GetName() );
		if ( mCachedMusicFont->contains( mCurrentFontName ) )
		{
			mCurrCachedMusicFont = &(*mCachedMusicFont)[mCurrentFontName];
			return;
		}
	}
	else
		return;
	
	GFontQt * cacheFont = (GFontQt*)(mSys->CreateVGFont( font->GetName() , CACHED_FONT_SIZE , font->GetProperties() ));

	QFont qtFont(*(cacheFont->GetNativeFont()));
	for ( int i = 0 ; i <= kMaxMusicalSymbolID ; i++ )
	{
		QString symbol = mCurrMusicFont->Symbol(i);
		QPainterPath path;
		path.addText( 0 , 0 , qtFont , symbol );
		(*mCachedMusicFont)[mCurrentFontName][i] = path;
	}

	mCurrCachedMusicFont = &(*mCachedMusicFont)[mCurrentFontName];
#endif
}

//-------------------------------------------------------------
const VGFont *	GDeviceQt::GetMusicFont()const
{
	return mCurrMusicFont;
}

//-------------------------------------------------------------
void GDeviceQt::SetTextFont( const VGFont * font )
{
	mCurrTextFont = (GFontQt*)font; 
}

//-------------------------------------------------------------
const VGFont *	GDeviceQt::GetTextFont()const
{
	return mCurrTextFont;
}

//------------------------------------------- DRAWING FUNCTIONS -------------------------------------------------------//
void GDeviceQt::SelectPen( const VGColor & inColor, float width )
{
	QPen qPen;
	qPen.setColor(QColor(inColor.mRed,inColor.mGreen,inColor.mBlue,inColor.mAlpha));
	qPen.setWidthF(width);	
	mQPainter->setPen( qPen );
}

//-------------------------------------------------------------
void GDeviceQt::SelectPenColor ( const VGColor & inColor)
{
	mQPainter->setPen( QColor(inColor.mRed,inColor.mGreen,inColor.mBlue,inColor.mAlpha) );
}

//-------------------------------------------------------------
void GDeviceQt::SelectPenWidth ( float width )
{
	QPen qPen = mQPainter->pen();
	qPen.setWidthF(width);	
	mQPainter->setPen( qPen );
}

//-------------------------------------------------------------
void GDeviceQt::SelectFillColor( const VGColor & c )
{
	mQPainter->setBrush( QBrush( QColor( c.mRed , c.mGreen, c.mBlue , c.mAlpha ) ) );
}

//-------------------------------------------------------------
void GDeviceQt::PushPen( const VGColor & inColor, float inWidth )
{
	QPen qPen = mQPainter->pen();
	PenState state = {qPen.color(), static_cast<float>(qPen.widthF())};
	mPenStack.push( state );
	SelectPen( inColor, inWidth );
}

//-------------------------------------------------------------
void GDeviceQt::PopPen()
{
	PenState state = mPenStack.top();
	mPenStack.pop();
	QPen qPen;
	qPen.setColor(state.color);
	qPen.setWidthF(state.width);	
	mQPainter->setPen( qPen );
}

//-------------------------------------------------------------
void GDeviceQt::PushPenColor( const VGColor & inColor )
{
	QPen qPen = mQPainter->pen();
	mPenColorStack.push( qPen.color() );
	SelectPenColor(inColor);
}

//-------------------------------------------------------------
void GDeviceQt::PopPenColor()
{
	mQPainter->setPen( mPenColorStack.top() );
	mPenColorStack.pop();
}

//-------------------------------------------------------------
void GDeviceQt::PushPenWidth( float width)
{
	mPenWidthStack.push( mQPainter->pen().widthF() );
	SelectPenWidth(width);
}

//-------------------------------------------------------------
void GDeviceQt::PopPenWidth()
{
	SelectPenWidth( mPenWidthStack.top() );
	mPenWidthStack.pop();
}

//-------------------------------------------------------------
void GDeviceQt::PushFillColor( const VGColor & inColor )
{
	mFillColorStack.push( mQPainter->brush().color() );
	SelectFillColor(inColor);
}

//-------------------------------------------------------------
void GDeviceQt::PopFillColor()
{
	mQPainter->setBrush( QBrush( mFillColorStack.top() ) );
	mFillColorStack.pop();
}

//-------------------------------------------------------------
QPainter::CompositionMode rasterMode2CompositionMode(VGDevice::VRasterOpMode ROpMode)
{
	switch ( ROpMode ) 
	{
	case VGDevice::kOpCopy:
		return QPainter::CompositionMode_SourceOver;
	case VGDevice::kOpAnd:
		return QPainter::CompositionMode_Darken;
	case VGDevice::kOpXOr:
		return QPainter::CompositionMode_Difference;
	case VGDevice::kOpInvert:
		qWarning("GDeviceQt::SetRasterOpMode(kOpInvert) : not supported. Paint with white with VRasterOpMode=kOpXOr to invert the dest color.\n");
		return QPainter::CompositionMode_Clear;
	case VGDevice::kOpOr:
		return QPainter::CompositionMode_Lighten;
	case VGDevice::kUnknown:
	default:
		return QPainter::CompositionMode_SourceOver;
	}
}
//kOpCopy:	-> Source , SourceOver , HardLight
//kOpAnd:	-> Darken, Multiply , ColorBurn
//kOpOr:	-> Lighten
// ?		-> Plus , Source/DestinationIn/Out , Source/DestinationAtop,
//				Xor , Screen , Overlay , ColorDodge, SoftLight , Difference , Exclusion


//-------------------------------------------------------------
VGDevice::VRasterOpMode compositionMode2RasterMode(QPainter::CompositionMode compositionMode)
{
	switch ( compositionMode )
	{
		case QPainter::CompositionMode_SourceOver :
			return VGDevice::kOpCopy;
		case QPainter::CompositionMode_Darken :
			return VGDevice::kOpAnd;
		case QPainter::CompositionMode_Difference :
			return VGDevice::kOpXOr;
		case QPainter::CompositionMode_Lighten :
			return VGDevice::kOpOr;
		case  QPainter::CompositionMode_Clear:
			return VGDevice::kOpInvert;
		default :
			return VGDevice::kUnknown;
	}
}

//-------------------------------------------------------------
void GDeviceQt::SetRasterOpMode( VRasterOpMode ROpMode)
{
	mQPainter->setCompositionMode( rasterMode2CompositionMode(ROpMode) );
}

//-------------------------------------------------------------
VGDevice::VRasterOpMode	GDeviceQt::GetRasterOpMode()const
{
	return compositionMode2RasterMode( mQPainter->compositionMode() );
}
//------------------------------------------- BITMAP FUNCTIONS --------------------------------------------------------//
//
//

//-------------------------------------------------------------
bool GDeviceQt::CopyPixels( VGDevice* pSrcDC, float alpha )
{
	GDeviceQt * srcDCQt = dynamic_cast<GDeviceQt *>( pSrcDC );
	assert( srcDCQt );
	
	//Default : copy from (0,0) to (0,0), dest rect = Src rect
	int xDest = 0;
	int yDest = 0;
	int dstWidth = srcDCQt->mQPainter->device()->width();
	int dstHeight = srcDCQt->mQPainter->device()->height();
	int xSrc = 0;
	int ySrc = 0;
	int nSrcWidth = srcDCQt->mQPainter->device()->width();
	int nSrcHeight = srcDCQt->mQPainter->device()->height();
	return CopyPixels( xDest , yDest , dstWidth , dstHeight , pSrcDC , xSrc , ySrc , nSrcWidth , nSrcHeight , alpha );
}


//-------------------------------------------------------------
bool GDeviceQt::CopyPixels( int xDest, int yDest,
		VGDevice* pSrcDC, 
		int xSrc, int ySrc, 
		int nSrcWidth, int nSrcHeight, float alpha )
{
	GDeviceQt * srcDCQt = dynamic_cast<GDeviceQt *>( pSrcDC );
	assert( srcDCQt );
	//Src rect is truncated by src Device
	int srcW = srcDCQt->mQPainter->device()->width();
	int srcH = srcDCQt->mQPainter->device()->height();
	nSrcWidth = ( srcW < nSrcWidth ) ? srcW : nSrcWidth;
	nSrcHeight = ( srcH < nSrcHeight ) ? srcH : nSrcHeight;
	//Non-specified Dest rect: default=no-stretching => Dest rect=Src rect 
	int dstWidth = nSrcWidth;
	int dstHeight = nSrcHeight;
	return CopyPixels( xDest , yDest , dstWidth , dstHeight , pSrcDC , xSrc , ySrc , nSrcWidth , nSrcHeight , alpha );
}

//-------------------------------------------------------------
bool GDeviceQt::CopyPixels( int xDest, int yDest, 
		int dstWidth, int dstHeight, 
		VGDevice* pSrcDC, float alpha )
{
	GDeviceQt * srcDCQt = dynamic_cast<GDeviceQt *>( pSrcDC );
	assert( srcDCQt );
	int xSrc = 0;
	int ySrc = 0;
	//Stretch the full src-device into dest device : Src rect=Device bounds
	int nSrcWidth = srcDCQt->mQPainter->device()->width();
	int nSrcHeight = srcDCQt->mQPainter->device()->height();
	return CopyPixels( xDest , yDest , dstWidth , dstHeight , pSrcDC , xSrc , ySrc , nSrcWidth , nSrcHeight , alpha );
}

//-------------------------------------------------------------
bool GDeviceQt::CopyPixels( int xDest, int yDest, 
		int dstWidth, int dstHeight, 
		VGDevice* pSrcDC, 
		int xSrc, int ySrc, 
		int nSrcWidth, int nSrcHeight, float alpha )
{
	if ( GetRasterOpMode() == VGDevice::kOpInvert )
		return false;

	if ( alpha == -1.0f )
		alpha = 1.0f;
	
	mQPainter->save();
	mQPainter->setOpacity( alpha );

	GDeviceQt * srcDCQt = dynamic_cast<GDeviceQt *>( pSrcDC );
	assert( srcDCQt );

	QRect target( xDest , yDest , dstWidth , dstHeight );
	QRect source( xSrc , ySrc , nSrcWidth , nSrcHeight );

	QPaintDevice * srcDeviceQt = srcDCQt->mQPainter->device();
	
	bool copyOk = false;
	QImage * qImage = dynamic_cast<QImage *>( srcDeviceQt );
	if ( qImage )
	{
		mQPainter->drawImage( target , *qImage , source );
		copyOk = true;
	}
//	else 
//	{
//		QPicture * qPicture = dynamic_cast<QPicture *>( srcDeviceQt );
//		if ( qPicture )
//		{
//			if (		( xSrc != 0 ) || ( ySrc != 0 )
//					||	( source.width() != target.width() ) || ( source.height() != target.height() )
//				)
//			{
//				//	- The source-point is not specifiable with QPicture
//				//	- Can't stretch from a Rect to another with QPicture
//				qWarning("GDeviceQt::CopyPixels: Can't do stretching on a QPicture : x=%d , y=%d , sW=%d , sH=%d , dW=%d , dH=%d\n" , 
//						xSrc , ySrc , source.width() , source.height() , target.width() , target.height() );
//				copyOk = false;
//			}
//			else
//			{
//				mQPainter->drawPicture( target.topLeft() , *qPicture );
//				copyOk = true;
//			}
//		}
//		else
//		{
//			QPixmap * qPixmap = dynamic_cast<QPixmap *>( srcDeviceQt );
//			if ( qPixmap )
//			{
//				mQPainter->drawPixmap( target , *qPixmap , source );
//				copyOk = true;
//			}
//		}
//	}
	
	mQPainter->restore();
	return copyOk;
}
//------------------------------------------- COORDINATE FUNCTIONS ----------------------------------------------------//

//-------------------------------------------------------------
void GDeviceQt::SetScale( float x, float y )
{
    mScaleX = x;
    mScaleY = y;

#if absoluteTransform1
	QTransform m = mQPainter->worldTransform();
	m.setMatrix (x, m.m12(), m.m13(), m.m21(), y, m.m23(), m.m31(), m.m32(), m.m33()),
	mQPainter->setWorldTransform (m);
#elif absoluteTransform2
	float curxs = GetXScale();
	float curys = GetYScale();
	mQPainter->scale( x/curxs, y/curys);
#else
	mQPainter->scale(x, y);
#endif
}

//-------------------------------------------------------------
void GDeviceQt::UnsetScale()
{
#if absoluteTransform1
	QTransform m = mQPainter->worldTransform();
	m.setMatrix (1 / mScaleX, m.m12(), m.m13(), m.m21(), 1 / mScaleY, m.m23(), m.m31(), m.m32(), m.m33()),
	mQPainter->setWorldTransform (m);
#elif absoluteTransform2
	float curxs = GetXScale();
	float curys = GetYScale();
	mQPainter->scale( mPrevScaleX / curxs, mPrevScaleY / curys);
#else
	mQPainter->scale(1 / mScaleX, 1 / mScaleY);
#endif
}

//-------------------------------------------------------------
void GDeviceQt::SetOrigin( float , float )
{
//	mQPainter->translate(x-GetXOrigin() , y-GetYOrigin());	// Absolute transformations prohibited.
}

//-------------------------------------------------------------
void GDeviceQt::OffsetOrigin( float x, float y )
{
	mQPainter->translate(x , y);	
}

//-------------------------------------------------------------
void GDeviceQt::LogicalToDevice( float * x, float * y )const
{
	*x = (*x * GetXScale() - GetXOrigin());
	*y = (*y * GetYScale() - GetYOrigin());
}

//-------------------------------------------------------------
void GDeviceQt::DeviceToLogical( float * x, float * y )const
{
	*x = ( *x + GetXOrigin() ) / GetXScale();
	*y = ( *y + GetYOrigin() ) / GetYScale();
}

//-------------------------------------------------------------
float GDeviceQt::GetXScale()const
{  
	return  mQPainter->worldTransform().m11();
}

//-------------------------------------------------------------
float GDeviceQt::GetYScale()const
{  
	return mQPainter->worldTransform().m22();
}

//-------------------------------------------------------------
float GDeviceQt::GetXOrigin()const
{  
	return mQPainter->worldTransform().dx() / GetXScale();
}

//-------------------------------------------------------------
float GDeviceQt::GetYOrigin()const
{  
	return  mQPainter->worldTransform().dy() / GetYScale();
}

//-------------------------------------------------------------
void GDeviceQt::NotifySize( int inWidth, int inHeight )
{
	mQPainter->setViewport(GetXOrigin(), GetYOrigin(), inWidth, inHeight);
}

//-------------------------------------------------------------
int GDeviceQt::GetWidth()const
{  
	QRect r = mQPainter->viewport();
	return r.width();
}

//-------------------------------------------------------------
int GDeviceQt::GetHeight()const
{  
	QRect r = mQPainter->viewport();
	return r.height();
}

//------------------------------------------- DISPLAY FUNCTIONS -----------------------------------------------------//
//

void setXY( float * x , float * y , int alignMode , const QFontMetrics& metrics , const QString& str )
{
	if ( alignMode == VGDevice::kAlignBaseLeft )
		return;
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
	float w = metrics.horizontalAdvance( str );
#else
	float w = metrics.width( str );
#endif
	float descent = metrics.descent();
	float ascent = metrics.ascent();
	
	//Vertical align
	if ( alignMode & VGDevice::kAlignTop )
	{
		(*y) += ascent;
	}
	else if ( alignMode & VGDevice::kAlignBottom )
	{
		(*y) -= descent;
	}
	else
	{
		// kAlignBase : do nothing, standard QPainter's behaviour.
	}

	// Horizontal align
	if( alignMode & VGDevice::kAlignRight )
	    (*x) -= w;
	else if( alignMode & VGDevice::kAlignCenter )
	    (*x) -= (w * 0.5);
	else
	{
		// kAlignLeft : do nothing, standard QPainter's behaviour.
	}
}

#ifdef USE_CACHED_MUSIC_FONT
//--------------------------------------------------------------------
void GDeviceQt::DrawMusicSymbol(float x, float y, 
		unsigned int inSymbolID )
{
	if ( !mCurrMusicFont )
		return;

	if ( mTextAlign != VGDevice::kAlignBaseLeft )
	{
		QString symbol = mCurrMusicFont->Symbol(inSymbolID);
		//Use cached path
		setXY(&x,&y,mTextAlign,QFontMetrics(*(mCurrMusicFont->GetNativeFont())) , symbol );
	}

	QPen oldPen = mQPainter->pen();
	QBrush oldBrush = mQPainter->brush();
	QTransform oldTransform = mQPainter->transform();
	
	mQPainter->setPen( Qt::NoPen );
	mQPainter->setBrush( mCurrMusicFontBrush );
	mQPainter->translate( x,y );
	mQPainter->scale( mCurrentFontRatio , mCurrentFontRatio );
	
//	mQPainter->drawPath( (*mCachedMusicFont)[mCurrentFontName][inSymbolID] );
	mQPainter->drawPath( (*mCurrCachedMusicFont)[inSymbolID] );

	mQPainter->setPen( oldPen );
	mQPainter->setBrush( oldBrush );
	mQPainter->setTransform( oldTransform );
}
#else
//--------------------------------------------------------------------
void GDeviceQt::DrawMusicSymbol(float x, float y, 
		unsigned int inSymbolID )
{	
	if ( !mCurrMusicFont )
		return;
	
	QString symbol = mCurrMusicFont->Symbol(inSymbolID);
	QFont qtFont(*(mCurrMusicFont->GetNativeFont()));
	
	// - Draw text
	QPen currentQPen = mQPainter->pen();

	mQPainter->setPen( QPen( QColor(mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue , mTextColor.mAlpha) ) );
	mQPainter->setFont( qtFont );

	setXY(&x,&y,mTextAlign,QFontMetrics(qtFont) , symbol );
	mQPainter->drawText( x, y, symbol );
	
	mQPainter->setPen( currentQPen );
}
#endif

//--------------------------------------------------------------------
void GDeviceQt::DrawString( float x, float y, 
		const char * s, 
		int)
{
	if(!mCurrTextFont)
	{
	  return;
	}

	QFont qtFont( *(mCurrTextFont->GetNativeFont()) );

	// - Draw text
	QPen currentQPen = mQPainter->pen();

	mQPainter->setPen( QPen( QColor(mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue , mTextColor.mAlpha) ) );
	mQPainter->setFont( qtFont );

	setXY(&x,&y,mTextAlign,QFontMetrics(qtFont) , s );
	mQPainter->drawText( x, y, s );

	mQPainter->setPen( currentQPen );	
}

//--------------------------------------------------------------------
void GDeviceQt::SetFontColor( const VGColor & inColor )
{
	mTextColor = inColor;
	mCurrMusicFontBrush = QBrush( QColor(mTextColor.mRed, mTextColor.mGreen, mTextColor.mBlue , mTextColor.mAlpha) );
}

//--------------------------------------------------------------------
VGColor GDeviceQt::GetFontColor()const
{  
	return mTextColor;
}

//--------------------------------------------------------------------
void GDeviceQt::SetFontBackgroundColor( const VGColor & inColor )
{
	if( inColor.mAlpha == ALPHA_TRANSPARENT )
	{
		mQPainter->setBackgroundMode( Qt::TransparentMode );
	}
	else
	{
		mQPainter->setBackgroundMode( Qt::OpaqueMode );
	}
		
	mQPainter->setBackground( QBrush( QColor(inColor.mRed, inColor.mGreen, inColor.mBlue, inColor.mAlpha) ) );
}

//--------------------------------------------------------------------
VGColor	 GDeviceQt::GetFontBackgroundColor()const
{
	QBrush backgroundColor = mQPainter->background();
	return VGColor( backgroundColor.color().red() , backgroundColor.color().green() , backgroundColor.color().blue() , backgroundColor.color().alpha() );
}

//--------------------------------------------------------------------
void GDeviceQt::SetFontAlign( unsigned int inAlign )
{
	mTextAlign = inAlign;	
}

//--------------------------------------------------------------------
unsigned int GDeviceQt::GetFontAlign() const
{
	return mTextAlign;
}

//--------------------------------------------------------------------
void GDeviceQt::SetDPITag( float )
{
#if !defined(__MOBILE__)
	QPrinter * device = dynamic_cast<QPrinter *>( mQPainter->device() );
	if ( device != 0 )
	{
		//device->setResolution ( inDPI );
	}
#endif
}

//--------------------------------------------------------------------
float GDeviceQt::GetDPITag()const
{  
#if !defined(__MOBILE__)
	QPrinter * device = dynamic_cast<QPrinter *>( mQPainter->device() );
	if ( device != 0 )
	{
		return device->resolution ();
	}
#endif
	return -1;
}

//--------------------------------------------------------------------
void* GDeviceQt::GetBitMapPixels()
{
	QImage * qImage = dynamic_cast<QImage *>( mQPainter->device() );
	if ( qImage ) return (void*)qImage->bits();

	cerr << "Warning: GDeviceQt::GetBitMapPixels cannot get image data" << endl;
	return 0;
}

//--------------------------------------------------------------------
void GDeviceQt::ReleaseBitMapPixels()
{  
//	assert(0);//Not implemented.
//	cerr << "Warning: GDeviceQt::ReleaseBitMapPixels not implemented" << endl;
}

//--------------------------------------------------------------------
const char* GDeviceQt::GetImageData(const char* & outDataPtr, int& outLength)
{
	QImage * image = dynamic_cast<QImage *>( mQPainter->device() );
	if ( image ) {
		QByteArray * ba = new QByteArray;
		QBuffer buffer(ba);
        buffer.open(QIODevice::WriteOnly);
        image->save(&buffer, "PNG"); // writes image into ba in PNG format
		outDataPtr = ba->data();
		outLength = (int)ba->size();
		return "image/png";
	}
	return 0;
}

//--------------------------------------------------------------------
void GDeviceQt::ReleaseImageData(const char * ptr) const
{
	delete ptr;
}

//--------------------------------------------------------------------
VGSystem * GDeviceQt::getVGSystem()const
{
	return mSys;
}

//--------------------------------------------------------------------
void GDeviceQt::ExportToFile( const char * )
{
//	assert(0);//Not implemented.
	cerr << "Warning: GDeviceQt::ExportToFile not implemented" << endl;
}

//--------------------------------------------------------------------
void * GDeviceQt::GetNativeContext()const
{  
	return reinterpret_cast<void*>(mQPainter);
}


//------------------------------------------------------
QPainter * GDeviceQt::painter()
{
	return mQPainter;
}
