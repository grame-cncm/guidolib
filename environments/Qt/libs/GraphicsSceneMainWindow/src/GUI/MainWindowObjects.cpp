/*
 * MainWindowObjects.cpp
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
#include "MainWindowObjects.h"

#include "GraphicsSceneMainWindow.h"

#include "QLanguageItem.h"
#include "QLanguageItemFactory.h"

#include "QItemResizer.h"
#include "QPaletteItem.h"

#include <assert.h>

#include <QFileInfo>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QTextStream>
#include <QStatusBar>
#include <QtDebug>
#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
# define Qt6 true
#endif


#define DOM_GMN_CONTAINER_ITEM "containerItem"
#define DOM_GMN_CONTAINER_ITEM_X "containerItemX"
#define DOM_GMN_CONTAINER_ITEM_Y "containerItemY"
#define DOM_HISTORY "history"
#define DOM_STORAGE "storage"

#define PAGE_ITEM_SHADOW_DISTANCE 5

#define HISTORY_ITEM_MARGIN 25
#define HISTORY_WIDTH 120
#define STORAGE_WIDTH 150
#define STORAGE_ITEM_WIDTH 66
#define STORAGE_ITEM_HEIGHT 100

#define TEXT_MARGIN_X 10
#define TEXT_MARGIN_Y 15
#define MAX_FONT 15

//------------------------------------------------------------------------------------------------------------------------
QSideBarGraphicsView::QSideBarGraphicsView(QGraphicsScene * s , QLanguageItemFactory* factory ) 
	:	QGraphicsView(s) ,
		mFactory(factory)
{}

//------------------------------------------------------------------------------------------------------------------------
QDomElement QSideBarGraphicsView::save(QDomDocument &d)
{
	QDomElement root = d.createElement( domId() );
	
	for ( int i = 0 ; i < mAddedItems.size() ; i++ )
	{
		root.appendChild( mAddedItems[i]->saveToDomElement(&d) );
	}
	
	return root;
}

//------------------------------------------------------------------------------------------------------------------------
bool QSideBarGraphicsView::load( const QDomElement& root )
{
	if( root.tagName() != domId() )
		return false;

	clear();
	
	QDomNode n = root.firstChild();	
	while ( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if ( ( !e.isNull() ) && ( e.tagName() == DOM_LANGUAGE_ITEM ) )
		{
			QLanguageItem * container = mFactory->buildLanguageItem(&e);
			if ( !find(container) )
				addItem( container );
			else
				delete container;
		}
			
		n = n.nextSibling();
	}
	
	return true;
}

//------------------------------------------------------------------------------------------------------------------------
void QSideBarGraphicsView::clear()
{
	while ( mAddedItems.size() > 0 )
	{
		scene()->removeItem( mAddedItems[0] );
		delete mAddedItems[0];
		mAddedItems.removeAt(0);
	}
}

//------------------------------------------------------------------------------------------------------------------------
void QSideBarGraphicsView::removeItem( QGraphicsItem * item )
{
	QLanguageItem * selectionItem = dynamic_cast<QLanguageItem *>(item);
	assert( selectionItem );
	scene()->removeItem( selectionItem );
	mAddedItems.removeAll(selectionItem);
	delete item;
}

//------------------------------------------------------------------------------------------------------------------------
QLanguageItem* QSideBarGraphicsView::find(const QLanguageItem * other )
{
	for ( int i = 0 ; i < mAddedItems.size() ; i++ )
	{
		QLanguageItem * languageItem = mAddedItems[i];
		if ( other->isEqualTo( languageItem ) )
			return languageItem;
	}
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------
void QSideBarGraphicsView::setupContainer( QLanguageItem * container )
{	
	container->setMimeId( mimeId() );
	container->setStorageMode(true);
	container->unlinkFile();
}

#define SCROLL_BAR_MARGIN	5
#define MAX_HISTORY_SIZE	30
#define EMPTY_SCENE_HEIGHT 1
//------------------------------------------------------------------------------------------------------------------------
QHistoryGraphicsView::QHistoryGraphicsView(QGraphicsScene * s , QLanguageItemFactory* factory ) 
	: QSideBarGraphicsView(s , factory )
{
	setFixedWidth( HISTORY_WIDTH );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	setStatusTip ( "Keep trace of all items added to the scene" );
}

//------------------------------------------------------------------------------------------------------------------------
void QHistoryGraphicsView::clear()
{
	QSideBarGraphicsView::clear();
	scene()->setSceneRect( QRectF(0,0,0,EMPTY_SCENE_HEIGHT ) );
}

//------------------------------------------------------------------------------------------------------------------------
void QHistoryGraphicsView::resizeEvent( QResizeEvent * )
{
	scene()->setSceneRect( QRectF(0,0,width() - HISTORY_ITEM_MARGIN , MAX( height() - SCROLL_BAR_MARGIN , scene()->sceneRect().height() ) ) );
}

//------------------------------------------------------------------------------------------------------------------------
void QHistoryGraphicsView::addItem( QLanguageItem * container )
{	
	setupContainer( container );
	
	//Scale the item so that it fits inside the side bar, maximizing its size but keeping margins.
	float widthScaleFactor = ( this->width() - HISTORY_ITEM_MARGIN ) / container->sceneBoundingRect().width();
	float heightScaleFactor = ( 1.5f * this->width() - HISTORY_ITEM_MARGIN ) / container->sceneBoundingRect().height();
	float scaleFactor = MIN(widthScaleFactor,heightScaleFactor);
	scaleFactor = MIN( MIN(scaleFactor,1.0f/container->transform().m11()) , MIN(scaleFactor,1.0f/container->transform().m22()) );
	container->setScale( scaleFactor );

	//Add the item to the scene, and to the list of all items.
	scene()->addItem( container );
	mAddedItems.append( container );
	
	reorderItems();
}

//------------------------------------------------------------------------------------------------------------------------
QString QHistoryGraphicsView::domId() 
{ return DOM_HISTORY; }
//------------------------------------------------------------------------------------------------------------------------
QString QHistoryGraphicsView::mimeId() 
{ return FROM_HISTORY_DRAG_MIME; }

#include <QtDebug>
//------------------------------------------------------------------------------------------------------------------------
void QHistoryGraphicsView::reorderItems()
{
	if ( mAddedItems.size() > MAX_HISTORY_SIZE )
	{
		removeItem( mAddedItems.first() );
	}
	
	int currentY = 0;
	int maxWidth = 0;
	for ( int i = (int)mAddedItems.size()-1 ; i >= 0  ; i-- )
	{
		QRectF r(mAddedItems[i]->sceneBoundingRect());
		mAddedItems[i]->setPos( 0 , currentY );
		currentY += r.height();
		maxWidth = MAX( r.width() , maxWidth );
	}
	for ( int i = (int)mAddedItems.size()-1 ; i >= 0  ; i-- )
	{
		QRectF r(mAddedItems[i]->sceneBoundingRect());
		mAddedItems[i]->setPos( (maxWidth - r.width())/2.0f , mAddedItems[i]->pos().y() );
	}
	
	int sceneHeight = scene()->sceneRect().height();
	if ( ( sceneHeight == EMPTY_SCENE_HEIGHT ) || ( currentY > sceneHeight ) )
	{
		sceneHeight += height() - SCROLL_BAR_MARGIN;
	}
	scene()->setSceneRect( QRectF(0,0,width() - HISTORY_ITEM_MARGIN , sceneHeight ) );
}

//------------------------------------------------------------------------------------------------------------------------
QStorageGraphicsView::QStorageGraphicsView( QGraphicsScene * s , GraphicsSceneMainWindow * mainWindow , QLanguageItemFactory* factory ) 
	: QSideBarGraphicsView(s , factory)
{
	setAcceptDrops(true);
	setDragMode(QGraphicsView::RubberBandDrag);
	mMainWindow = mainWindow;
	setStatusTip ( "Drag&drop an item to store it in the Storage Zone" );
	
	connect( s , SIGNAL( changed(const QList<QRectF> &) ) , this , SLOT( sceneChanged(const QList<QRectF> &) ) );
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::addItem( QLanguageItem * container )
{
	setupContainer(container);

	container->setPos(0,0);

	//Scale the item so that it fits within a constraint size (STORAGE_ITEM_WIDTH,STORAGE_ITEM_HEIGHT)
	QRectF rect = container->sceneBoundingRect();
	float scaleFactor = MIN( float(STORAGE_ITEM_WIDTH) / rect.width() , float(STORAGE_ITEM_HEIGHT) / rect.height() );
	scaleFactor = MIN( MIN(scaleFactor,1.0f/container->transform().m11()) , MIN(scaleFactor,1.0f/container->transform().m22()) );
	container->setScale( scaleFactor );

	//Item is only selectable, not movable. (moved by drag&drop, other mechanism)
	container->setFlags( QGraphicsItem::ItemIsSelectable );

	//Add the item to the scene, and to the list of all items.
	scene()->addItem( container );
	mAddedItems.append( container );

	reorderItems();
}

//------------------------------------------------------------------------------------------------------------------------
QString QStorageGraphicsView::domId() 
{ return DOM_STORAGE; }

//------------------------------------------------------------------------------------------------------------------------
QString QStorageGraphicsView::mimeId() 
{ return FROM_STORAGE_DRAG_MIME; }

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::sceneChanged( const QList<QRectF> &)
{
	mMainWindow->updateRemoveActState();
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::resizeEvent( QResizeEvent * )
{
	scene()->setSceneRect( 0 , 0 , this->width()-SCROLL_BAR_MARGIN , this->height()-SCROLL_BAR_MARGIN );
	reorderItems();
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::dragEnterEvent(QDragEnterEvent* event)
{
//	qDebug() << "QStorageGraphicsView::dragEnterEvent";
	if (	( event->mimeData()->hasFormat(FROM_SCENE_DRAG_MIME) ) 
		||	( event->mimeData()->hasFormat(FROM_STORAGE_DRAG_MIME) ) 
		||	( event->mimeData()->hasFormat(FROM_HISTORY_DRAG_MIME) ) 
		)
	{
		event->acceptProposedAction();
	}
	else
		event->ignore();
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::dragMoveEvent(QDragMoveEvent* event)
{
	event->acceptProposedAction();
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::dragLeaveEvent(QDragLeaveEvent* )
{
//	qDebug() << "QStorageGraphicsView::dragLeaveEvent";
//	QGraphicsView::dragLeaveEvent( event );
}

/*!
*	\brief Drops a QLanguageItem in the QStorageGraphicsView. The
*	QLanguageItem is built from the information in the QMimeData.
*/
//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::dropEvent(QDropEvent* event)
{
	// Only accepts drops that come from within the application : main scene, or sidebars.
	if (	( event->mimeData()->hasFormat(FROM_SCENE_DRAG_MIME) )
		||	( event->mimeData()->hasFormat(FROM_STORAGE_DRAG_MIME) ) 
		||	( event->mimeData()->hasFormat(FROM_HISTORY_DRAG_MIME) ) 
		)
	{
		// Build the QLanguageItem from the informations in the QMimeData.
		QLanguageItem * container = mFactory->buildLanguageItem( event->mimeData() );
		// Do not accept invalid items.
		if ( !(container->isValid()) )
		{
			delete container;
			return;
		}
		
		// If the storage scene already contains an equivalent item,
		// do not add the new item ; only move the existing one.
		QLanguageItem * other = find(container);
		if ( other )
		{
			QRectF itemRect = other->sceneBoundingRect();
#if Qt6
			other->setPos( event->position().toPoint() - QPointF(itemRect.width(),itemRect.height())/2.0f );
#else
			other->setPos( event->pos() - QPointF(itemRect.width(),itemRect.height())/2.0f );
#endif
			reorderItems();
			delete container;
		}
		else
		{
			// There is no equivalent item in the scene : add the created item.
			addItem( container );
#if Qt6
			QPointF scenePos = mapToScene( event->position().toPoint() );
#else
			QPointF scenePos = mapToScene( event->pos() );
#endif
			QRectF itemRect = container->sceneBoundingRect();
			container->setPos( scenePos - QPointF( itemRect.width() , itemRect.height() )/2.0f );
		}
		event->accept();
		
		if ( event->mimeData()->hasFormat(FROM_SCENE_DRAG_MIME) )
		{
			QLanguageItem * item = (QLanguageItem*)((const void *) event->mimeData()->data( FROM_SCENE_DRAG_MIME ));

			if ( mMainWindow->saveItemModificationsDialog(item) )
			{
				mMainWindow->itemRemoved( item );
				item->deleteLater();
			}

		}
	}
	else
	{
		event->ignore();
	}
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::focusOutEvent( QFocusEvent * event )
{
	QGraphicsView::focusOutEvent( event );
	while ( scene()->selectedItems().size() > 0 )
		scene()->selectedItems()[0]->setSelected( false );
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::focusInEvent( QFocusEvent * event )
{
	QGraphicsView::focusOutEvent( event );
	mMainWindow->unselectAll();
}

//------------------------------------------------------------------------------------------------------------------------
void QStorageGraphicsView::reorderItems()
{	
/*
	QList<QLanguageItem*> decorators = filterItems( scene()->items() );	
	QRectF visibleRect = mapFromScene( sceneRect() ).boundingRect();
	for ( int i = 0 ; i < decorators.size()  ; i++ )
	{
		QRectF decoratorRect = decorators[i]->sceneBoundingRect();
		
		if ( decoratorRect.x() < visibleRect.x() )
			decorators[i]->setPos( visibleRect.x() , decorators[i]->y() );
		if ( decorators[i]->y() < visibleRect.y() )
			decorators[i]->setPos( decorators[i]->x() , visibleRect.y() );
		if ( decoratorRect.bottomRight().x() > visibleRect.bottomRight().x() )
			decorators[i]->setPos( visibleRect.bottomRight().x() - decoratorRect.width() , decorators[i]->y() );
		if ( decoratorRect.bottomRight().y() > visibleRect.bottomRight().y() )
			decorators[i]->setPos( decorators[i]->x() , visibleRect.bottomRight().y() - decoratorRect.height() );
	}
*/
}

//------------------------------------------------------------------------------------------------------------------------
QList<QLanguageItem*> QStorageGraphicsView::filterItems(const QList<QGraphicsItem*>& list)
{
	QList<QLanguageItem*> result;
	for ( int i = 0 ; i < list.size() ; i++ )
	{
		QLanguageItem * languageItem = dynamic_cast<QLanguageItem*> (list[i]);
		if ( languageItem )
			result.append( languageItem );
	}
	return result;
}

#define SHADOW_COLOR QColor(150,150,150,50)
//------------------------------------------------------------------------------------------------------------------------
QBackgroundItem::QBackgroundItem( GraphicsSceneMainWindow * mainWindow ,  const QSizeF& size )
{
	mRect = QRectF( 0, 0, size.width() , size.height() );
		
	mPage = new QGraphicsRectItem( QRectF() , this );
	mPage->setBrush( Qt::white );
	mPage->setPen( Qt::NoPen );
	mPage->setZValue(0);
		
	mBackgroundItemResizer = new QItemResizer( QRectF() , mPage );
	mBackgroundItemResizer->setZValue(1);
	mBackgroundItemResizer->setKeepAspectRatio(false);
			
	QPen resizerPen(Qt::black);
	resizerPen.setStyle( Qt::DotLine );
	mBackgroundItemResizer->setPen( resizerPen );
	mBackgroundItemResizer->setBrush( SHADOW_COLOR );

	mIsResizeEnabled = true;

	QObject::connect( mBackgroundItemResizer , SIGNAL(resized(const QRectF&) ) , mainWindow , SLOT(resizePage(const QRectF&)) );
			
	updatePageRect();
}

//------------------------------------------------------------------------------------------------------------------------
void QBackgroundItem::setRect( const QRectF& r ) 
{
//	qDebug() << "QBackgroundItem::setRect : " << mRect << " -> " << r;

	if ( ( r.width() != mRect.width() ) || ( r.height() != mRect.height() ) )
	{
		prepareGeometryChange();
		mRect.setWidth(r.width());
		mRect.setHeight(r.height());
		update();
	
		updatePageRect();
	}
	setPos( r.x() , r.y() );
}

//------------------------------------------------------------------------------------------------------------------------		
QRectF QBackgroundItem::boundingRect() const
{
	return mRect;
}
		
//------------------------------------------------------------------------------------------------------------------------
QRectF QBackgroundItem::pageRectInSceneCoords() const
{
	return mPage->mapToScene(mPage->boundingRect()).boundingRect();
}
		
//------------------------------------------------------------------------------------------------------------------------
void QBackgroundItem::setPageSize( const QSizeF& size ) 
{
	setResizeEnabled( true );
	QSize totalSize(size.width() + 2*PAGE_ITEM_OUT_MARGIN , size.height() + 2*PAGE_ITEM_OUT_MARGIN );
//	setSize( totalSize );
	setRect( QRectF( pos().x() , pos().y() , totalSize.width() , totalSize.height() ) );
}

//------------------------------------------------------------------------------------------------------------------------
QSizeF QBackgroundItem::pageSize() const 
{
	return QSize( mPage->boundingRect().width() , mPage->boundingRect().height() );
}

//------------------------------------------------------------------------------------------------------------------------
void QBackgroundItem::setResizeEnabled( bool isEnabled )
{
	mIsResizeEnabled = isEnabled;
}

//------------------------------------------------------------------------------------------------------------------------
bool QBackgroundItem::isResizeEnabled() const
{
	return mIsResizeEnabled;
}

//------------------------------------------------------------------------------------------------------------------------
void QBackgroundItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * , QWidget * )
{
	QRectF pageRect = mPage->boundingRect();
	pageRect.translate( mPage->pos() );
			
	QRectF shadowRect( pageRect );
	shadowRect.translate( PAGE_ITEM_SHADOW_DISTANCE , PAGE_ITEM_SHADOW_DISTANCE );			

	painter->setPen( Qt::NoPen );
	painter->setBrush( QBrush(Qt::darkGray) );
	painter->drawRect( shadowRect );
}

//	The pageRect is the "white paper" part of the QBackgroundItem.
//	
//	----------------------------------------------------------------------------------------------------------------------
void QBackgroundItem::updatePageRect()
{
	if ( mIsResizeEnabled )
	{
		QRectF pageRect(0,0,0,0);
		pageRect.setWidth( mRect.width() - 2*PAGE_ITEM_OUT_MARGIN );
		pageRect.setHeight( mRect.height() - 2*PAGE_ITEM_OUT_MARGIN );

		mPage->setRect( pageRect );
		mPage->setPos( PAGE_ITEM_OUT_MARGIN , PAGE_ITEM_OUT_MARGIN );

		mBackgroundItemResizer->updateGeometry( pageRect );
	}
}

//------------------------------------------------------------------------------------------------------------------------
LanguageTextEdit::LanguageTextEdit(const QString& initString , GraphicsSceneMainWindow * mainWindow) 
			: QTextEdit(initString) , mMainWindow(mainWindow) 
{
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageTextEdit::wheelEvent( QWheelEvent * e )
{
	mMainWindow->textEditActivity();
	QTextEdit::wheelEvent( e );
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageTextEdit::focusOutEvent( QFocusEvent * event )
{
	mMainWindow->updateCode();
	QTextEdit::focusOutEvent( event );
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageTextEdit::changeEvent ( QEvent * event )
{
	if ( event->type() == QEvent::EnabledChange )
		Q_EMIT enabled( isEnabled() );
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageTextEdit::dragEnterEvent(QDragEnterEvent* event)
{
	if (		
		(
				( event->mimeData()->hasText() )
			||  ( event->mimeData()->hasUrls() )
		)
	)
	{
		event->acceptProposedAction();
	}
	else
	{
		qDebug("LanguageTextEdit::dragEnterEvent : refused");
	}
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageTextEdit::dragMoveEvent(QDragEnterEvent* event)
{
	if (		
		(
				( event->mimeData()->hasText() )
			||  ( event->mimeData()->hasUrls() )
		)
	)
	{
		event->acceptProposedAction();
	}
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageTextEdit::dropEvent(QDropEvent* event)
{
	QLanguageItem* createdItem = 0;
	mMainWindow->dropItem( event->mimeData() , mMainWindow->viewCenterInSceneCoordinate() , &createdItem );
	event->accept();
}

//------------------------------------------------------------------------------------------------------------------------
LanguageGraphicsView::LanguageGraphicsView() : QGraphicsView() 
{ 
	setDragMode(QGraphicsView::RubberBandDrag);
	setAcceptDrops(true); 
	setMouseTracking (true);
}

//------------------------------------------------------------------------------------------------------------------------
QSize LanguageGraphicsView::sizeHint () const
{
	return QSize(500,500);
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageGraphicsView::setMainWindow( GraphicsSceneMainWindow * mainWindow )
{
	setParent( mainWindow );
	mMainWindow = mainWindow;
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageGraphicsView::dragEnterEvent(QDragEnterEvent* event)
{
//	qDebug() << "LanguageGraphicsView::dragEnterEvent";
	if (		
			( event->mimeData()->hasText() ||  event->mimeData()->hasUrls() )
		||	( event->mimeData()->hasFormat( FROM_SCENE_DRAG_MIME ) )
	)
	{
		event->acceptProposedAction();
	}
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageGraphicsView::dragLeaveEvent(QDragLeaveEvent* )
{
//	qDebug() << "LanguageGraphicsView::dragLeaveEvent";
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageGraphicsView::dropEvent(QDropEvent* event)
{
	// Take the focus.
	setFocus();

	QLanguageItem* droppedItem = 0;

#ifdef Qt6
	QPointF pos = mapToScene( event->position().toPoint() );
#else
	QPointF pos = mapToScene( event->pos() );
#endif
	// Check if the drop QMimeData contains the "copy drag" flag.
	bool doCopy = false;
	if ( event->mimeData()->hasFormat( MIME_DO_COPY ) )
	{
		doCopy = QVariant( event->mimeData()->data( MIME_DO_COPY ).constData() ).toBool();
	}

	// Check if the drop triggers a palette interaction.
	bool isPaletteDrop = event->mimeData()->hasFormat( MIME_PALETTE_ID );
	QGraphicsItem * draggedItem = 0;
	int paletteId = -1;
	if ( isPaletteDrop )
	{
		paletteId = event->mimeData()->data( MIME_PALETTE_ID ).toInt();
		draggedItem = (QGraphicsItem*)((const void *) event->mimeData()->data( MIME_PALETTE_ITEM ));
	}
	if ( (!isPaletteDrop) || (!QPaletteManager::instance()->drop( scene() , paletteId , draggedItem ) ) )
	{
		// The drop didn't triggered a palette interaction.
		
		// If the QMimeData tells that the drop comes from the main scene, then
		// the QMimeData also contains the dragged item pointer.
		if ( event->mimeData()->hasFormat( FROM_SCENE_DRAG_MIME ) )
		{
			droppedItem = (QLanguageItem*)((const void *) event->mimeData()->data( FROM_SCENE_DRAG_MIME ));
			// Move the item to the drop position, and get the resulting movement coordinate.
#if Qt6
			QPointF movementValue = droppedItem->moveItem( mapToScene( event->position().toPoint() ) );
#else
			QPointF movementValue = droppedItem->moveItem( mapToScene( event->pos() ) );
#endif
			// Move all other selected item of the scene by the same (dx,dy).
			QList<QLanguageItem*> selectedItems = mMainWindow->selectedLanguageItems();
			QList<QLanguageItem*> otherSelectedItems(selectedItems);
			otherSelectedItems.removeAll( droppedItem );
			for ( int i = 0 ; i < otherSelectedItems.size() ; i++ )
				otherSelectedItems[i]->moveBy( movementValue.x() , movementValue.y() );
			
			if ( doCopy )
			{
				// The "copy flag" is on: 
				QList<QLanguageItem*> copiedItems;
				for ( int i = 0 ; i < selectedItems.size() ; i++ )
				{
					// 1. copy all the moved items (storing their respective copies in a list)
					copiedItems << mMainWindow->addItemCopy(selectedItems[i]);
					// 2. then put them back to their original position,
					selectedItems[i]->moveBy( -movementValue.x() , -movementValue.y() );
				}
				// 3. then select all the copied items.
				scene()->clearSelection();
				for ( int i = 0 ; i < copiedItems.size() ; i++ )
					copiedItems[i]->setSelected( true );
			}
		}
		else
		{
			// The drop comes whether:
			//	- from the OS file explorer, 
			//	- from the side-bar scenes
			// create a new item according to the content of the QMimeData.
			// (the mainWindow handles this operation)
			mMainWindow->dropItem( event->mimeData() , pos , &droppedItem );
		}
	}
	event->accept();
}

//------------------------------------------------------------------------------------------------------------------------
void LanguageGraphicsView::dragMoveEvent(QDragMoveEvent* event)
{
	event->acceptProposedAction();

	// If we are dragging a "Palette Enabled" item
	if ( event->mimeData()->hasFormat( MIME_PALETTE_ID ) )
	{
		// Get the paletteId and the dragged item...
		int paletteId = event->mimeData()->data( MIME_PALETTE_ID ).toInt();
		QGraphicsItem * draggedItem = (QGraphicsItem*)((const void *) event->mimeData()->data( MIME_PALETTE_ITEM ));
		// ... and notify the QPaletteManager that we are moving it, so it can, if necessary, activate the palette.
#if Qt6
		QPaletteManager::instance()->move( mapToScene( event->position().toPoint() ) , scene() , paletteId , draggedItem );
#else
		QPaletteManager::instance()->move( mapToScene( event->pos() ) , scene() , paletteId , draggedItem );
#endif
	}
}

//-------------------------------------------------------------------------
void LanguageGraphicsView::mouseMoveEvent( QMouseEvent * event )
{
	QGraphicsView::mouseMoveEvent(event);
	QPointF posSceneCoords = mMainWindow->mGraphicsView->mapToScene( event->pos() );
	mMainWindow->updateCursorPos( posSceneCoords );
}
