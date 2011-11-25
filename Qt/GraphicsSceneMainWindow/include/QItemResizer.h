/*
 * QItemResizer.h
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
#ifndef ITEM_RESIZER_H
#define ITEM_RESIZER_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>

class QItemResizerSlot;

/*! \brief	QItemResizer adds "resize borders" to its parent item.
*
*			The user can drag&drop the borders to change the width & height
*			of the parent item. On drop, the QItemResizer 
*			emits resized(QRectF), where ths QRectF param is the new requested size.
*			
*			\important	The QItemResizer doesn't actually change the scale or the size of 
*						its parent item ; it only emits the resized(QRectF) signal. 
*						It's on the parent's responsibility of the parent item to 
*						process the signal accordingly.
*
*			The QItemResizer is rectangular and thus works well with rectangular items (optionnaly with 
*			rounded borders - see setRoundedRectRadius).
*
*			You can set the border's thickness and brush with setResizeBorder. 
*
*			You can set if the QItemResizer keeps item's width/height ratio
*			with setKeepAspectRatio.
*
*/
class QItemResizer : public QObject , public QGraphicsRectItem
{

	Q_OBJECT
	
	public:

		/*!	\brief Builds a QItemResizer for the QGraphicsItem * parent.
		*/
		QItemResizer( const QRectF& rect , QGraphicsItem * parent );

		void paint(QPainter* painter , const QStyleOptionGraphicsItem* style , QWidget* widget);

		/*!	\brief	Sets the rounded rect radius. Zero is for a simple (not rounded) rectangle.
		*/
		void setRoundedRectRadius( float radius );

		/*!	\brief	Indicates if the QItemResizer must keep the width/height ratio, or
		*			is allowed to change (per example) the width without changing the height.
		*/
		void setKeepAspectRatio( bool isAspectRatioKept );

		/*!	\brief	Sets the thickness and the brush of the border.
		*/
		void setResizeBorder( float width , const QBrush& brush );

		void setSizeBounds( const QSizeF& min , const QSizeF& max );

	public Q_SLOTS:
		void updateGeometry(const QRectF& newGeometry);
		void show(bool isShown);

	Q_SIGNALS:
		void resized( const QRectF& newRect );

	protected :

		typedef enum ScaleFocus
		{
			FOCUS_TOP , FOCUS_LEFT , FOCUS_BOTTOM , FOCUS_RIGHT , 
			FOCUS_TOP_LEFT , FOCUS_TOP_RIGHT , FOCUS_BOTTOM_LEFT , FOCUS_BOTTOM_RIGHT ,
			FOCUS_CENTER , FOCUS_NONE
		}
		ScaleFocus;
		
		virtual void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
		virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * );
		virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * event );
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
		virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );

		QRectF boundRect( QSizeF referenceRect , bool doReduce );

		void checkCursorPosition( const QPointF& pos );
		void setCursorAndUpdate( const QCursor& cursor );
		
		static QRectF getRescaledRectWithAspectRatio( const QRectF& baseRect , const QPointF& delta , QItemResizer::ScaleFocus scaleFocus );
		static QRectF getRescaledRectWithoutAspectRatio( const QRectF& baseRect , const QPointF& delta , QItemResizer::ScaleFocus scaleFocus );
		
		ScaleFocus mScaleFocus;
		float mRoundedRectRadius;
		bool mIsResizing;
		QRectF mStartResizeRect;
		bool mIsAspectRatioKept;
		QBrush mBorderBrush;
		float mBorderWidth;
		QSizeF mMaxSize , mMinSize;
};

#endif //ITEM_RESIZER_H

