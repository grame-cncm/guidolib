/*
 * QSimpleItemContainer.cpp
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
#include "QSimpleItemContainer.h"

#include <QMimeData>

#include <QtDebug>

#define MIME_SIMPLE_ITEM_SCALE_X	"MimeImageItemScaleX"
#define MIME_SIMPLE_ITEM_SCALE_Y	"MimeImageItemScaleY"

#define DOM_SIMPLE_ITEM_SCALE_X	"DomSimpleItemScaleX"
#define DOM_SIMPLE_ITEM_SCALE_Y	"DomSimpleItemScaleY"

#define PEN_WIDTH 2.0f
#define SELECTED_PEN		QPen( QColor(SIMPLE_ITEM_BASE_RGB,SIMPLE_ITEM_ALPHA_1) , PEN_WIDTH )
//#define SELECTED_BRUSH		QBrush( QColor(SIMPLE_ITEM_BASE_RGB,SIMPLE_ITEM_ALPHA_2) )
#define HIGHLIGHTED_PEN		QPen( QColor(SIMPLE_ITEM_BASE_RGB,SIMPLE_ITEM_ALPHA_2) , PEN_WIDTH )
//#define HIGHLIGHTED_BRUSH	QBrush( QColor(SIMPLE_ITEM_BASE_RGB,SIMPLE_ITEM_ALPHA_3) )
#define SELECTED_BRUSH		Qt::NoBrush
#define HIGHLIGHTED_BRUSH	Qt::NoBrush
#define STANDARD_PEN		Qt::NoPen
#define STANDARD_BRUSH		Qt::NoBrush

#define FLAG_HIGHLIGHTED	0
#define FLAG_SELECTED		1
#define FLAG_IDLE			2

//-------------------------------------------------------------------------
//								Public functions						//
//-------------------------------------------------------------------------
QSimpleItemContainer::QSimpleItemContainer( QGraphicsItem * containedItem , QItemAdapter * adapter , QGraphicsItem * parent )
 : QLanguageItem(parent)
{
	init( containedItem , adapter );
}

//-------------------------------------------------------------------------
QSimpleItemContainer::QSimpleItemContainer( QGraphicsItem * containedItem , QItemAdapter * adapter , const QMimeData * mimeData , QGraphicsItem * parent)
 : QLanguageItem(parent)
{
	init( containedItem , adapter );
	QLanguageItem::load( mimeData );
	unlinkFile();
	
	if ( mimeData->hasFormat( MIME_SIMPLE_ITEM_SCALE_X ) )
	{
		float scaleX = mimeData->data( MIME_SIMPLE_ITEM_SCALE_X ).toDouble();
		float scaleY = mimeData->data( MIME_SIMPLE_ITEM_SCALE_Y ).toDouble();
		QRectF newRect(0,0,mContainedItem->boundingRect().width() * scaleX , mContainedItem->boundingRect().height() * scaleY );
		resized( newRect );
	}
}

//-------------------------------------------------------------------------
QSimpleItemContainer::QSimpleItemContainer( QGraphicsItem * containedItem , QItemAdapter * adapter , const QDomElement * domElement  , QGraphicsItem * parent)
 : QLanguageItem(parent)
{
	init( containedItem , adapter );
	QLanguageItem::load( domElement );
	unlinkFile();
				
	float scaleX = QVariant( domElement->attribute( DOM_SIMPLE_ITEM_SCALE_X, "1" ) ).toDouble();
	float scaleY = QVariant( domElement->attribute( DOM_SIMPLE_ITEM_SCALE_Y, "1" ) ).toDouble();
	QRectF newRect(0,0,mContainedItem->boundingRect().width() * scaleX , mContainedItem->boundingRect().height() * scaleY );
	resized( newRect );
}

//-------------------------------------------------------------------------
QSimpleItemContainer::QSimpleItemContainer( QGraphicsItem * containedItem , QItemAdapter * adapter , const QSimpleItemContainer * other  , QGraphicsItem * parent)
 : QLanguageItem(parent)
{
	init( containedItem , adapter );
	QLanguageItem::load( other );
	unlinkFile();
	
	float scaleX = other->mContainedItem->transform().m11();
	float scaleY = other->mContainedItem->transform().m22();
	QRectF newRect(0,0,mContainedItem->boundingRect().width() * scaleX , mContainedItem->boundingRect().height() * scaleY );
	resized( newRect );
}

//-------------------------------------------------------------------------
QSimpleItemContainer::~QSimpleItemContainer()
{
	delete mItemAdapter;
}

//-------------------------------------------------------------------------
QDomElement QSimpleItemContainer::saveToDomElement( QDomDocument * doc)
{
	QDomElement element = QLanguageItem::saveToDomElement(doc);

	element.setAttribute( DOM_SIMPLE_ITEM_SCALE_X , mContainedItem->transform().m11() );
	element.setAttribute( DOM_SIMPLE_ITEM_SCALE_Y , mContainedItem->transform().m22() );

	element.setAttribute( mItemAdapter->domId() , 1 );

  	return element;
}

//-------------------------------------------------------------------------
bool QSimpleItemContainer::setCode( const QString& code )
{
	bool result = mItemAdapter->setCode(code);
	simpleItemUpdateGeometry( mContainedItem->mapToParent( mContainedItem->boundingRect() ).boundingRect() );
	return result;
}

//-------------------------------------------------------------------------
//								Public slots							//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QSimpleItemContainer::resized(const QRectF& newRect)
{
	if ( newRect.toRect() == rect().toRect() )
		return;

	float xRatio = ( newRect.width() / rect().width() );
	float yRatio = ( newRect.height() / rect().height() );
	
	moveBy( newRect.x() , newRect.y() );

	mContainedItem->scale( xRatio , yRatio );
	simpleItemUpdateGeometry( mContainedItem->mapToParent( mContainedItem->boundingRect() ).boundingRect() );

	Q_EMIT scaleChanged( mContainedItem->transform().m11() );
}

//-------------------------------------------------------------------------
//							Protected functions							 //
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QSimpleItemContainer::init( QGraphicsItem * containedItem , QItemAdapter * adapter )
{
	mItemAdapter = adapter;

	mContainedItem = containedItem;
	mContainedItem->setParentItem(this);

#if !linux		// bug in Qt 4.4 with the linux cache mode 
	mContainedItem->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
#endif

	setAcceptsHoverEvents(true);
	
	// Set pens & brushes
	mPenBrushSwitcher.addFlag( FLAG_SELECTED , 2 ,		PenBrush(SELECTED_PEN , SELECTED_BRUSH ) );
	mPenBrushSwitcher.addFlag( FLAG_HIGHLIGHTED , 1 ,	PenBrush(HIGHLIGHTED_PEN , HIGHLIGHTED_BRUSH ) );
	mPenBrushSwitcher.addFlag( FLAG_IDLE , 0,			PenBrush(STANDARD_PEN , STANDARD_BRUSH ) );
	mPenBrushSwitcher.setFlag( FLAG_IDLE , true );
/*
	mPenBrushSwitcher.setSwitchedFlag(FLAG_IDLE, SWITCH_INVALID_CODE, PenBrush(Qt::NoPen, INVALID_STANDARD_BRUSH ) );
	mPenBrushSwitcher.setSwitchedFlag(FLAG_HIGHLIGHTED, SWITCH_INVALID_CODE, PenBrush(Qt::NoPen, INVALID_HIGHLIGHTED_BRUSH ) );
	mPenBrushSwitcher.setSwitchedFlag(FLAG_SELECTED, SWITCH_INVALID_CODE, PenBrush(Qt::NoPen, INVALID_BRUSH_SELECTED ) );
*/
	mSelectionItem = new QGraphicsRectItem(this);
	mSelectionItem->setZValue( 2 );
	updateSelectionItem();
}

#define SELECTION_ITEM_BORDER_MARGIN 3
//-------------------------------------------------------------------------
void QSimpleItemContainer::simpleItemUpdateGeometry(const QRectF& newGeometry)
{
	QLanguageItem::updateGeometry(newGeometry);
	
	QRectF selectionRect = rect();
	QPointF delta(PEN_WIDTH/2.0f , PEN_WIDTH/2.0f);
	selectionRect.setTopLeft( selectionRect.topLeft() - delta );
	selectionRect.setBottomRight( selectionRect.bottomRight() + delta );
	mSelectionItem->setRect( selectionRect );
}

#include <QPainter>

//-------------------------------------------------------------------------
QImage* QSimpleItemContainer::buildDragImage()
{
	QImage* result = new QImage( rect().toRect().size() , QImage::Format_ARGB32 );
	QPainter painter( result );
	painter.setPen( SELECTED_PEN );
	painter.setBrush( SELECTED_BRUSH );
	painter.drawRect( rect() );
	painter.end();
	return result;
}

//-------------------------------------------------------------------------
QMimeData * QSimpleItemContainer::buildMimeData()
{
	QMimeData * data = QLanguageItem::buildMimeData();
	data->setData( MIME_SIMPLE_ITEM_SCALE_X ,	QByteArray::number(mContainedItem->transform().m11() ) );
	data->setData( MIME_SIMPLE_ITEM_SCALE_Y ,	QByteArray::number(mContainedItem->transform().m22() ) );
	data->setData( mItemAdapter->mimeId() ,		QByteArray::number(1) );
	return data;
}

//-------------------------------------------------------------------------
QMenu * QSimpleItemContainer::buildContextMenu()
{
	return mItemAdapter->buildContextMenu( QLanguageItem::buildContextMenu() );
}

//-------------------------------------------------------------------------
void QSimpleItemContainer::updateSelectionItem()
{
	mSelectionItem->setPen( mPenBrushSwitcher.activeObject().mPen );
	mSelectionItem->setBrush( mPenBrushSwitcher.activeObject().mBrush );
}

//-------------------------------------------------------------------------
QVariant QSimpleItemContainer::itemChange( GraphicsItemChange change, const QVariant& value )
{
	QVariant standardResult = QLanguageItem::itemChange( change , value );

	if ( change == QGraphicsItem::ItemSelectedHasChanged )
	{
		mPenBrushSwitcher.setFlag( FLAG_SELECTED , value.toBool() );
		updateSelectionItem();
	}
	return standardResult;
}

//-------------------------------------------------------------------------	
void QSimpleItemContainer::hoverEnterEvent ( QGraphicsSceneHoverEvent * e )
{
	QLanguageItem::hoverEnterEvent(e);
	
	mPenBrushSwitcher.setFlag( FLAG_HIGHLIGHTED , true );	
	updateSelectionItem();
}

//-------------------------------------------------------------------------
void QSimpleItemContainer::hoverLeaveEvent ( QGraphicsSceneHoverEvent * e )
{
	QLanguageItem::hoverLeaveEvent(e);

	mPenBrushSwitcher.setFlag( FLAG_HIGHLIGHTED , false );
	updateSelectionItem();
}

//-------------------------------------------------------------------------
//					QGraphicsPixmapItemAdapter class					///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QPixmapItemContainer* QPixmapItemContainer::buildPixmapItemContainer(QGraphicsItem * parent )
{
	QGraphicsPixmapItem * item = new QGraphicsPixmapItem();
	QGraphicsPixmapItemAdapter * adapter = new QGraphicsPixmapItemAdapter(item);
	return new QPixmapItemContainer(item, adapter, parent);
}

//-------------------------------------------------------------------------
QPixmapItemContainer* QPixmapItemContainer::buildPixmapItemContainer(const QMimeData * mimeData ,	QGraphicsItem * parent )
{
	QGraphicsPixmapItem * item = new QGraphicsPixmapItem();
	QGraphicsPixmapItemAdapter * adapter = new QGraphicsPixmapItemAdapter(item);
	return new QPixmapItemContainer(item, adapter, mimeData , parent);
}

//-------------------------------------------------------------------------
QPixmapItemContainer* QPixmapItemContainer::buildPixmapItemContainer(const QDomElement * domElement ,	QGraphicsItem * parent )
{
	QGraphicsPixmapItem * item = new QGraphicsPixmapItem();
	QGraphicsPixmapItemAdapter * adapter = new QGraphicsPixmapItemAdapter(item);
	return new QPixmapItemContainer(item, adapter, domElement , parent);
}

//-------------------------------------------------------------------------
QPixmapItemContainer* QPixmapItemContainer::buildPixmapItemContainer(const QSimpleItemContainer * other ,	QGraphicsItem * parent )
{
	QGraphicsPixmapItem * item = new QGraphicsPixmapItem();
	QGraphicsPixmapItemAdapter * adapter = new QGraphicsPixmapItemAdapter(item);
	return new QPixmapItemContainer(item, adapter, other , parent);
}

//-------------------------------------------------------------------------
QPixmapItemContainer::QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,	QGraphicsItem * parent )
	:	QSimpleItemContainer(containedItem, adapter, parent)
{
	init();
}

//-------------------------------------------------------------------------
QPixmapItemContainer::QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,const QMimeData * mimeData ,			QGraphicsItem * parent )
	:	QSimpleItemContainer(containedItem, adapter, mimeData , parent)
{
	init();
}

//-------------------------------------------------------------------------
QPixmapItemContainer::QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,const QDomElement * domElement ,		QGraphicsItem * parent )
	:	QSimpleItemContainer(containedItem, adapter, domElement , parent)
{
	init();
}

//-------------------------------------------------------------------------
QPixmapItemContainer::QPixmapItemContainer(QGraphicsItem * containedItem , QItemAdapter * adapter ,const QSimpleItemContainer * other ,	QGraphicsItem * parent )
	:	QSimpleItemContainer(containedItem, adapter, other , parent)
{
	init();
}

//-------------------------------------------------------------------------
void QPixmapItemContainer::init()
{
	unlinkFile();
}

//-------------------------------------------------------------------------
//					QGraphicsPixmapItemAdapter class					///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGraphicsPixmapItemAdapter::QGraphicsPixmapItemAdapter( QGraphicsPixmapItem* item )
{
	mItem = item;
	mFile = "";
	mIsValid = false;
}

//-------------------------------------------------------------------------
bool QGraphicsPixmapItemAdapter::setCode(const QString& file)								
{
	//	Bad hack : if file length > 4000, assume it is not a file path.
	//	Avoids to call the very slow function QFile::exists() with
	//	a random big invalid string.
	if (  file.length() > 4000 )
		return false;

	//	Ignore setCode msg that contains the actual binary data of the image file
	//	or any invalid setCode.
	if ( !QFile::exists( file ) )
		return false;

	if ( file == mFile )
		return mIsValid;
	
	mFile = file;

	if ( mFile.length() )
	{
		mIsValid = QImageReader( mFile ).canRead();
		if ( mIsValid )
			mItem->setPixmap( QPixmap( mFile ) );

		return mIsValid;
	}
	return true;
}

//-------------------------------------------------------------------------
QMenu* QGraphicsPixmapItemAdapter::buildContextMenu( QMenu* standardMenu ) const			
{ 
	//TODO : remove the File menu from standardMenu.
	QMenu *fileMenu = standardMenu->findChild<QMenu*>( OBJECT_NAME_FILE_MENU );
	delete fileMenu;
	return standardMenu; 
}

//-------------------------------------------------------------------------
bool QGraphicsPixmapItemAdapter::isEqualTo( const QItemAdapter * adapter ) const
{
	const QGraphicsPixmapItemAdapter* other = dynamic_cast<const QGraphicsPixmapItemAdapter*>(adapter);
	if ( !other )
		return false;
	else
		return ( other->code() == code() ); 
}
