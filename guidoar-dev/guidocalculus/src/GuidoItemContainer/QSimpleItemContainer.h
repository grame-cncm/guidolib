/*
 * QSimpleItemContainer.h
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
#ifndef SIMPLE_ITEM_CONTAINER_H
#define SIMPLE_ITEM_CONTAINER_H

#include "QLanguageItem.h"
#include "QSwitcher.h"

#include <QPen>
#include <QBrush>
#include <QtDebug>
#include <QMimeData>

#define SIMPLE_ITEM_BASE_RGB	197,214,255
#define SIMPLE_ITEM_ALPHA_1		200
#define SIMPLE_ITEM_ALPHA_2		100
#define SIMPLE_ITEM_ALPHA_3		50

#define DOM_SIMPLE_TEXT_ITEM		"DomSimpleTextItem"
#define MIME_SIMPLE_TEXT_ITEM		"MimeSimpleTextItem"
#define DOM_SIMPLE_PIXMAP_ITEM		"DomSimplePixmapItem"
#define MIME_SIMPLE_PIXMAP_ITEM		"MimeSimplePixmapItem"

class QItemAdapter
{
	public :
		virtual ~QItemAdapter() {}

		virtual QString code() const = 0;
		virtual bool setCode(const QString& code) = 0;
		virtual bool isValid() const = 0;
		virtual void setFile(const QString& fileName) = 0;
		virtual QString domId() const = 0;
		virtual QString mimeId() const = 0;
		virtual QMenu* buildContextMenu( QMenu* standardMenu ) const { return standardMenu; }
		virtual bool isEqualTo( const QItemAdapter * other ) const = 0;
};

class QSimpleItemContainer : public QLanguageItem
{
	Q_OBJECT

	protected:
		QItemAdapter * mItemAdapter;
		QGraphicsItem * mContainedItem;

	public:
		
		/**
		*	\brief Constructor.
		*/
		QSimpleItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,										QGraphicsItem * parent = 0);
		QSimpleItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter , const QMimeData * mimeData ,			QGraphicsItem * parent = 0);
		QSimpleItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter , const QDomElement * domElement ,		QGraphicsItem * parent = 0);
		QSimpleItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter , const QSimpleItemContainer * other ,	QGraphicsItem * parent = 0);

		virtual ~QSimpleItemContainer();
		
		/*!
		*	\brief Create a new QDomElement storing the properties of the QSimpleItemContainer.
		*
		*	\note The QDomElement may be further used to setup another QSimpleItemContainer, using QSimpleItemContainer::loadFromDomElement.
		*/
		virtual QDomElement saveToDomElement( QDomDocument * doc);

		float	currentScale() const						{ return mContainedItem->transform().m11(); }

		bool	isValid() const								{ return mItemAdapter->isValid(); }
		bool	setCode( const QString& code );
		QString	code() const								{ return mItemAdapter->code(); }

		QGraphicsItem * containedItem()						{ return mContainedItem; }

		/*!
		*	\brief QSimpleItemContainer comparator.
		*/
		bool isEqualTo( QLanguageItem * item ) const
		{ 
			QSimpleItemContainer* other = dynamic_cast<QSimpleItemContainer*>(item);
			if ( !other )
				return false;
			else
				return mItemAdapter->isEqualTo( other->mItemAdapter ); 
		}

		QItemAdapter * adapter() const { return mItemAdapter; }

	public Q_SLOTS :
	
		void resized(const QRectF& newRect);

 	protected:
		
		void setFile( const QString& fileName )				{ QLanguageItem::setFile(fileName); mItemAdapter->setFile(fileName); }
		
		void init(QGraphicsItem * containedItem , QItemAdapter * adapter);

		void simpleItemUpdateGeometry(const QRectF& newGeometry);

		/*!
		*	\brief QLanguageItem implementation.
		*/
		QImage* buildDragImage();
		
		/*!
		*	\brief QLanguageItem specialization.
		*/
		QMimeData * buildMimeData();
		
		/*!
		*	\brief QLanguageItem specialization.
		*/
		QMenu * buildContextMenu();

		/*!
		*	\brief Updates the "mSelectionItem" pen and brush according to mPenBrushSwitcher's state.
		*/
		void updateSelectionItem();
		
		QVariant itemChange( GraphicsItemChange change, const QVariant& value );
		void hoverEnterEvent ( QGraphicsSceneHoverEvent * e );
		void hoverLeaveEvent ( QGraphicsSceneHoverEvent * e );

		struct PenBrush {
			QPen mPen;
			QBrush mBrush;
			PenBrush(const QPen& p = QPen() , const QBrush& b = QBrush())
			{	
				mPen = p; mBrush = b;
			}
		};
		QSwitcher<PenBrush> mPenBrushSwitcher;
		QGraphicsRectItem * mSelectionItem;

	public:
		
		static bool isSimpleItemContainer( const QMimeData* mimeData);
		static bool isSimpleItemContainer( const QDomElement* e);

};

class QPixmapItemContainer : public QSimpleItemContainer
{

	public:	
		static QPixmapItemContainer* buildPixmapItemContainer(QGraphicsItem * parent = 0);
		static QPixmapItemContainer* buildPixmapItemContainer(const QMimeData * mimeData ,			QGraphicsItem * parent = 0);
		static QPixmapItemContainer* buildPixmapItemContainer(const QDomElement * domElement ,		QGraphicsItem * parent = 0);
		static QPixmapItemContainer* buildPixmapItemContainer(const QSimpleItemContainer * other ,	QGraphicsItem * parent = 0);

	protected:
		QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,										QGraphicsItem * parent = 0);
		QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,const QMimeData * mimeData ,			QGraphicsItem * parent = 0);
		QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,const QDomElement * domElement ,		QGraphicsItem * parent = 0);
		QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,const QSimpleItemContainer * other ,	QGraphicsItem * parent = 0);
		
		void init();

};

class QGraphicsSimpleTextItemAdapter : public QItemAdapter
{
	public :
		QGraphicsSimpleTextItemAdapter( QGraphicsSimpleTextItem* item)	{ mItem = item; }
	
		QString code() const											{ return mItem->text(); }
		bool setCode(const QString& code)								{ mItem->setText(code); return isValid(); }
		bool isValid() const											{ return true; }
		void setFile(const QString&)									{}

		static bool recognizes( const QMimeData* mimeData)				{ return mimeData->hasFormat( MIME_SIMPLE_TEXT_ITEM ); }		
		static bool recognizes( const QDomElement* e)					{ return ( QVariant( e->attribute( DOM_SIMPLE_TEXT_ITEM, "0" ) ).toInt() == 1 ); }		
		QString domId() const											{ return DOM_SIMPLE_TEXT_ITEM; }
		QString mimeId() const											{ return MIME_SIMPLE_TEXT_ITEM; }
		bool isEqualTo( const QItemAdapter * adapter ) const
		{
			const QGraphicsSimpleTextItemAdapter* other = dynamic_cast<const QGraphicsSimpleTextItemAdapter*>(adapter);
			if ( !other )
				return false;
			else
				return ( other->code() == code() ); 
		}

	protected:
		QGraphicsSimpleTextItem * mItem;
};

#include <QImageReader>
#include <QFile>
class QGraphicsPixmapItemAdapter : public QItemAdapter
{
	public :

		QGraphicsPixmapItemAdapter( QGraphicsPixmapItem* item );

		QString code() const										{ return mFile; }
		bool setCode(const QString& file);
		bool isValid() const										{ return mIsValid; }

		void setFile(const QString& fileName)						{ setCode( fileName ); }

		static bool recognizes( const QMimeData* mimeData)			{ return mimeData->hasFormat( MIME_SIMPLE_PIXMAP_ITEM ); }		
		static bool recognizes( const QDomElement* e)				{ return ( QVariant( e->attribute( DOM_SIMPLE_PIXMAP_ITEM, "0" ) ).toInt() == 1 ); }		
		QString domId() const										{ return DOM_SIMPLE_PIXMAP_ITEM; }
		QString mimeId() const										{ return MIME_SIMPLE_PIXMAP_ITEM; }
		QMenu* buildContextMenu( QMenu* standardMenu ) const;
		bool isEqualTo( const QItemAdapter * adapter ) const;

	protected:
		QGraphicsPixmapItem * mItem;
		QString mFile;
		bool mIsValid;
};

#endif //SIMPLE_ITEM_CONTAINER_H
