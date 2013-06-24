/*
 * QItemResizer.cpp
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
#include "QItemResizer.h"

#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>

#include <assert.h>

#define DEFAULT_BORDER_WIDTH 5
#define DEFAULT_BORDER_BRUSH QColor(150,150,150,50)

//-------------------------------------------------------------------------
QItemResizer::QItemResizer( const QRectF& rect , QGraphicsItem * parent ) 
	: QGraphicsRectItem( rect , parent )
{
	setAcceptHoverEvents( true );
	mIsResizing = false;
	mRoundedRectRadius = 0;

	mIsAspectRatioKept = false;
	mScaleFocus = FOCUS_NONE;
	
	mBorderBrush = DEFAULT_BORDER_BRUSH;
	mBorderWidth = DEFAULT_BORDER_WIDTH;

	mMinSize = QSize(0,0);
	mMaxSize = QSize(0,0);
}

#include <QtDebug>
//-------------------------------------------------------------------------
void QItemResizer::updateGeometry(const QRectF& newGeometry)
{
	setRect( newGeometry );
}

//-------------------------------------------------------------------------
void QItemResizer::setKeepAspectRatio( bool isAspectRatioKept )
{
	mIsAspectRatioKept = isAspectRatioKept;
}

//-------------------------------------------------------------------------
void QItemResizer::paint(QPainter* painter , const QStyleOptionGraphicsItem* , QWidget* )
{	
	QRectF r = boundingRect();
	if ( mIsResizing )
	{
		painter->setPen( pen() );
		painter->setBrush( brush() );
		painter->drawRoundedRect( r , mRoundedRectRadius , mRoundedRectRadius );
	}
	else if ( hasCursor() )
	{
		painter->setBrush( mBorderBrush );
		painter->setPen( Qt::NoPen );
		painter->drawRect( 0 , 0 , r.width() , mBorderWidth );
		painter->drawRect( 0 , mBorderWidth , mBorderWidth , r.height() - mBorderWidth );
		painter->drawRect( mBorderWidth , r.height() , r.width() - 2 * mBorderWidth , -mBorderWidth );
		painter->drawRect( r.width() , mBorderWidth , -mBorderWidth , r.height() - mBorderWidth );
	}
}

//-------------------------------------------------------------------------
void QItemResizer::setRoundedRectRadius( float radius )
{
	mRoundedRectRadius = radius;
}

//-------------------------------------------------------------------------
void QItemResizer::setResizeBorder( float width , const QBrush& brush )
{
	mBorderBrush = brush;
	mBorderWidth = width;
}

//-------------------------------------------------------------------------
void QItemResizer::setSizeBounds( const QSizeF& min , const QSizeF& max )
{
	mMinSize = min;
	mMaxSize = max;
}

//-------------------------------------------------------------------------
void QItemResizer::checkCursorPosition( const QPointF& position )
{
	QRectF r = boundingRect();
	
	if ( position.x() < mBorderWidth ) 
	{
		if ( position.y() < mBorderWidth )
		{
			mScaleFocus = FOCUS_BOTTOM_RIGHT;
			setCursorAndUpdate( Qt::SizeFDiagCursor );
		}
		else if ( position.y() > r.height() - mBorderWidth ) 
		{
			mScaleFocus = FOCUS_TOP_RIGHT;
			setCursorAndUpdate( Qt::SizeBDiagCursor );
		}
		else
		{
			mScaleFocus = FOCUS_RIGHT;
			setCursorAndUpdate( Qt::SizeHorCursor );
		}
	}
	else if ( position.x() > r.width() - mBorderWidth ) 
	{
		if ( position.y() < mBorderWidth ) 
		{
			mScaleFocus = FOCUS_BOTTOM_LEFT;
			setCursorAndUpdate( Qt::SizeBDiagCursor );
		}
		else if ( position.y() > r.height() - mBorderWidth ) 
		{
			mScaleFocus = FOCUS_TOP_LEFT;
			setCursorAndUpdate( Qt::SizeFDiagCursor );
		}
		else
		{
			mScaleFocus = FOCUS_LEFT;
			setCursorAndUpdate( Qt::SizeHorCursor );
		}
	}
	else if ( position.y() < mBorderWidth )
	{
		mScaleFocus = FOCUS_BOTTOM;
		setCursorAndUpdate( Qt::SizeVerCursor );
	}
	else if ( position.y() > r.height() - mBorderWidth )
	{
		mScaleFocus = FOCUS_TOP;
		setCursorAndUpdate( Qt::SizeVerCursor );
	}
	else
	{
		if ( mScaleFocus != FOCUS_NONE  )
		{
			mScaleFocus = FOCUS_NONE;
			unsetCursor();
			update();
		}
	}
}

//-------------------------------------------------------------------------
void QItemResizer::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )
{
	QPointF position = event->pos();
	checkCursorPosition(position);
}

//-------------------------------------------------------------------------
void QItemResizer::setCursorAndUpdate( const QCursor& newCursor )
{
	if ( cursor().shape() != newCursor.shape() )
	{
		setCursor( newCursor );
		update();
	}
}

//-------------------------------------------------------------------------
QRectF QItemResizer::getRescaledRectWithAspectRatio( const QRectF& baseRect , const QPointF& delta , QItemResizer::ScaleFocus scaleFocus )
{
	QRectF newRect(baseRect);
	float heightForWidth = baseRect.height() / baseRect.width();

	switch ( scaleFocus )
	{
	case QItemResizer::FOCUS_BOTTOM_LEFT:
		{
			QPointF p = newRect.topRight();
			if ( heightForWidth > 1 )
				p += QPointF( - delta.y() / heightForWidth , delta.y() );
			else
				p += QPointF( delta.x() , -delta.x() * heightForWidth  );
			newRect.setTopRight( p );
			break;
		}
	case QItemResizer::FOCUS_BOTTOM_RIGHT:
		{
			QPointF p = newRect.topLeft();
			if ( heightForWidth > 1 )
				p += QPointF( delta.y() / heightForWidth , delta.y() );
			else
				p += QPointF( delta.x() , delta.x() * heightForWidth  );
				
			newRect.setTopLeft( p );
			break;
		}
	case QItemResizer::FOCUS_TOP_RIGHT:
		{
			QPointF p = newRect.bottomLeft();
			if ( heightForWidth > 1 )
				p += QPointF( - delta.y() / heightForWidth , delta.y() );
			else
				p += QPointF( delta.x() , -delta.x() * heightForWidth  );
			newRect.setBottomLeft( p );
			break;
		}
	case QItemResizer::FOCUS_TOP_LEFT:
		{
			QPointF p = newRect.bottomRight();
			if ( heightForWidth > 1 )
				p += QPointF( delta.y() / heightForWidth , delta.y() );
			else
				p += QPointF( delta.x() , delta.x() * heightForWidth  );
				
			newRect.setBottomRight( p );
			break;
		}
	case QItemResizer::FOCUS_BOTTOM:
		{
			QPointF p = newRect.topRight();
			p += QPointF( - delta.y() / (2 * heightForWidth ), delta.y() );
			newRect.setTopRight( p );
			p = newRect.topLeft();
			p += QPointF( + delta.y() / (2 *heightForWidth ), 0 );
			newRect.setTopLeft( p );
			break;
		}
	case QItemResizer::FOCUS_RIGHT:
		{
			QPointF p = newRect.topLeft();
			p += QPointF( delta.x() , delta.x() * (heightForWidth/2)  );
			newRect.setTopLeft( p );
			p = newRect.bottomLeft();
			p += QPointF( 0 , -delta.x() * (heightForWidth/2)  );
			newRect.setBottomLeft( p );
			break;
		}
	case QItemResizer::FOCUS_TOP:
		{
			QPointF p = newRect.bottomLeft();
			p += QPointF( - delta.y() / (2*heightForWidth) , delta.y() );
			newRect.setBottomLeft( p );
			p = newRect.bottomRight();
			p += QPointF( delta.y() / (2*heightForWidth) , 0 );
			newRect.setBottomRight( p );
			break;
		}
	case QItemResizer::FOCUS_LEFT:
		{
			QPointF p = newRect.bottomRight();
			p += QPointF( delta.x() , delta.x() * (heightForWidth/2) );
			newRect.setBottomRight( p );
			p = newRect.topRight();
			p += QPointF( 0 , -delta.x() * (heightForWidth/2)  );
			newRect.setTopRight( p );
			break;
		}
	default:
		break;
	}
	
	return newRect;
}

//-------------------------------------------------------------------------
QRectF QItemResizer::getRescaledRectWithoutAspectRatio( const QRectF& baseRect , const QPointF& delta , QItemResizer::ScaleFocus scaleFocus )
{
	QRectF newRect(baseRect);
	
	switch ( scaleFocus )
	{
	case QItemResizer::FOCUS_BOTTOM_LEFT:
		{
			QPointF p = newRect.topRight();
			p += QPointF( delta.x() , delta.y() );
			newRect.setTopRight( p );
			break;
		}
	case QItemResizer::FOCUS_BOTTOM_RIGHT:
		{
			QPointF p = newRect.topLeft();
			p += QPointF( delta.x()  , delta.y() );
			newRect.setTopLeft( p );
			break;
		}
	case QItemResizer::FOCUS_TOP_RIGHT:
		{
			QPointF p = newRect.bottomLeft();
			p += QPointF( delta.x()  , delta.y() );
			newRect.setBottomLeft( p );
			break;
		}
	case QItemResizer::FOCUS_TOP_LEFT:
		{
			QPointF p = newRect.bottomRight();
			p += QPointF( delta.x()  , delta.y() );	
			newRect.setBottomRight( p );
			break;
		}
	case QItemResizer::FOCUS_BOTTOM:
		{
			QPointF p = newRect.topRight();
			p += QPointF( 0  , delta.y() );
			newRect.setTopRight( p );
			break;
		}
	case QItemResizer::FOCUS_RIGHT:
		{
			QPointF p = newRect.topLeft();
			p += QPointF( delta.x(), 0 );
			newRect.setTopLeft( p );
			break;
		}
	case QItemResizer::FOCUS_TOP:
		{
			QPointF p = newRect.bottomLeft();
			p += QPointF( 0  , delta.y() );
			newRect.setBottomLeft( p );
			break;
		}
	case QItemResizer::FOCUS_LEFT:
		{
			QPointF p = newRect.bottomRight();
			p += QPointF( delta.x()  , 0 );
			newRect.setBottomRight( p );
			break;
		}
	default:
		break;
	}
	
	return newRect;
}

//-------------------------------------------------------------------------
void QItemResizer::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	if ( (mIsResizing) && ( mScaleFocus != FOCUS_NONE ) )
	{
		QRectF oldRect = boundingRect();
		QPointF delta = event->pos() - event->lastPos();

		QRectF newRect;
		if ( mIsAspectRatioKept )
			newRect = getRescaledRectWithAspectRatio(oldRect , delta , mScaleFocus);
		else
			newRect = getRescaledRectWithoutAspectRatio(oldRect , delta , mScaleFocus);

		if ( ( newRect.height() < 0 ) || ( newRect.width() < 0 ) )
			return;
		
		setRect( newRect );
	}
}

//-------------------------------------------------------------------------
void QItemResizer::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	QPointF position = event->pos();
	checkCursorPosition(position);
	
	if ( mScaleFocus != FOCUS_NONE )
	{
		event->accept();
		mIsResizing = true;
		mStartResizeRect = rect();
	}
	else
		event->ignore();
}

//-------------------------------------------------------------------------
void QItemResizer::hoverLeaveEvent ( QGraphicsSceneHoverEvent * )
{
	unsetCursor();
	update();
}


#include <QtDebug>
//-------------------------------------------------------------------------
void QItemResizer::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	QGraphicsRectItem::mouseReleaseEvent( event );

	QRectF resizedRect = rect();

	if ( mMinSize.width() * mMinSize.height() > 0 )
	{
		QRectF min = boundRect( mMinSize , false );
		if ( !resizedRect.contains( min ) )
		{
			resizedRect = min | resizedRect;
		}
	}
	
	if ( mMaxSize.width() * mMaxSize.height() > 0 )
	{
		QRectF max = boundRect( mMaxSize , true );
		if ( !max.contains( resizedRect ) )
		{
			resizedRect = max & resizedRect;
		}
	}
	
	setRect( mStartResizeRect );

	mIsResizing = false;
	unsetCursor();
	update();

	if ( resizedRect != rect() )
	{
		Q_EMIT resized( resizedRect );
	}

}


//-------------------------------------------------------------------------
static QPointF segmentCenter( const QPointF& a , const QPointF& b )
{
	QPointF result = a + b;
	result.setX( result.x() / 2 );
	result.setY( result.y() / 2 );
	return result;
}

//-------------------------------------------------------------------------
QRectF QItemResizer::boundRect( QSizeF referenceRect , bool doReduce )
{
	if ( mIsAspectRatioKept )
	{
		float widthForHeight = rect().width() / rect().height();

		bool b = ( widthForHeight > 1.0f );
		if ( doReduce )
			b = !b;
		if ( b )
		{
			referenceRect.setWidth( widthForHeight * referenceRect.height() );
		}
		else
		{
			referenceRect.setHeight( referenceRect.width() / float(widthForHeight) );
		}
	}
	
	QRectF result = rect();
	
	switch ( mScaleFocus )
	{
	case QItemResizer::FOCUS_BOTTOM_LEFT:
		{
			result.setTopRight( result.bottomLeft() + QPointF( referenceRect.width() , -referenceRect.height() ) );
			break;
		}
	case QItemResizer::FOCUS_BOTTOM_RIGHT:
		{
			result.setTopLeft( result.bottomRight() - QPointF( referenceRect.width() , referenceRect.height() ) );
			break;
		}
	case QItemResizer::FOCUS_TOP_RIGHT:
		{
			result.setBottomLeft( result.bottomRight() + QPointF( -referenceRect.width() , referenceRect.height() ) );
			break;
		}
	case QItemResizer::FOCUS_TOP_LEFT:
		{
			result.setBottomRight( result.topLeft() + QPointF( referenceRect.width() , referenceRect.height() ) );
			break;
		}
	case QItemResizer::FOCUS_BOTTOM:
		{
			QPointF c = segmentCenter( result.bottomLeft() , result.bottomRight() );
			result.setBottomLeft( c - QPointF( referenceRect.width()  / 2.0f, 0 ) );
			result.setTopRight( result.bottomLeft() + QPointF( referenceRect.width() , -referenceRect.height() ) );
			break;
		}
	case QItemResizer::FOCUS_RIGHT:
		{
			QPointF c = segmentCenter( result.topRight() , result.bottomRight() );
			result.setBottomRight( c + QPointF( 0, referenceRect.height()/2.0f ) );
			result.setTopLeft( result.bottomRight() - QPointF( referenceRect.width() , referenceRect.height() ) );
			break;
		}
	case QItemResizer::FOCUS_TOP:
		{
			QPointF c = segmentCenter( result.topLeft() , result.topRight() );
			result.setTopLeft( c - QPointF( referenceRect.width()  / 2.0f, 0 ) );
			result.setBottomRight( result.topLeft() + QPointF( referenceRect.width() , referenceRect.height() ) );
			break;
		}
	case QItemResizer::FOCUS_LEFT:
		{
			QPointF c = segmentCenter( result.topLeft() , result.bottomLeft() );
			result.setBottomLeft( c + QPointF( 0, referenceRect.height()/2.0f ) );
			result.setTopRight( result.bottomLeft() + QPointF( referenceRect.width() , -referenceRect.height() ) );
			break;
		}
	default:
		break;
	}
	return result;
}

//-------------------------------------------------------------------------
void QItemResizer::show(bool isShown)
{
	setVisible(isShown);
}

