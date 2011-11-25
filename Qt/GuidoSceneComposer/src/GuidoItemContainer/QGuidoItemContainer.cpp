/*
 * QGuidoItemContainer.cpp
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
 
#include <sstream>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>

#include "QGuidoItemContainer.h"
#include "QGuidoGraphicsItem.h"
#include "QGuidoImporter.h"

#include "QItemResizer.h"

#ifdef USES_GUIDO_AR
	#include "QGuidoAR.h"
	#include "QPaletteItem.h"
#endif

#include <assert.h>
#include <QtDebug>

#define PROPORTIONAL_SPRING		1.0f
#define PROPORTIONAL_FORCE		1500.0f
#define NO_PROPORTIONAL_SPRING	1.1f
#define NO_PROPORTIONAL_FORCE	750.0f

#define DEFAULT_PROPORTIONAL		false
#define DEFAULT_OPTIMAL_PAGE_FILL	true
#define DEFAULT_RESIZE_PAGE			true

#define SELECTION_FLAG_SELECTED		2
#define SELECTION_FLAG_HIGHLIGHTED	1
#define SELECTION_FLAG_IDLE			0
#define SELECTION_SWITCH_INVALID_CODE	1

#define TITLE_BAR_FONT_COLOR		Qt::black
#define TITLE_BAR_FONT				QFont("Lucida Grande")
#define TITLE_BAR_FONT_PEN_WIDTH	1.0f


#define ARROW_PEN_WIDTH 2

#define PAGE_TURNER_ARROW_SIZE 10

#define PAGE_LABEL_RECT_OUT_WIDTH 5
#define PAGE_LABEL_RECT_OUT_HEIGHT 5
#define PAGE_LABEL_TEXT_MARGIN 0
#define PAGE_LABEL_RECT_RADIUS_FACTOR 0.5f

#define INVALID_RECT QRect(0,0,100,100)

#define DOM_GUIDO_ITEM_CURRENT_PAGE			"currentPage"
#define DOM_GUIDO_ITEM_GRID_WIDTH			"gridWidth"
#define DOM_GUIDO_ITEM_GRID_HEIGHT			"gridHeight"

#define GMN_CONTAINER_MIME_FLAG					"GMNContainerMimeFlag"
#define GMN_CONTAINER_MIME_GRID_WIDTH			"GMNContainerMimeGridWidth"
#define GMN_CONTAINER_MIME_GRID_HEIGHT			"GMNContainerMimeGridHeight"
#define GMN_CONTAINER_MIME_PAGE_INDEX			"gmnContainerMimePageIndex"

class QPageTurnerItem : public QGraphicsPolygonItem
{
	public:
		QPageTurnerItem( bool isNext , const QPolygonF& polygon , QGuidoItemContainer * parent ) 
			: QGraphicsPolygonItem( polygon , parent  )
		{
			mParent = parent;
			mIsNext = isNext;

			setAcceptHoverEvents(true);
			
			setCursor( Qt::PointingHandCursor );
		}

	protected:
		
		void mousePressEvent( QGraphicsSceneMouseEvent * event )
		{
			if ( mIsNext )
				mParent->nextPage();
			else
				mParent->previousPage();

			event->accept();
		}

		QGuidoItemContainer * mParent;
		bool mIsNext;

};

QPolygonF getRightArrowPolygon(const QRectF& boundingRect);
QPolygonF getLeftArrowPolygon(const QRectF& boundingRect);


//-------------------------------------------------------------------------
//								Public functions						//
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
QGuidoItemContainer::QGuidoItemContainer(QGraphicsItem * parent)
	: QLanguageItem(parent)
{
	init();
}

//-------------------------------------------------------------------------
QGuidoItemContainer::QGuidoItemContainer(const QMimeData * mimeData			, QGraphicsItem * parent)
	: QLanguageItem(parent)
{
	QGuidoItemContainer::init(mimeData);
}

//-------------------------------------------------------------------------
QGuidoItemContainer::QGuidoItemContainer(const QDomElement * domElement		, QGraphicsItem * parent)
	: QLanguageItem(parent)
{
	QGuidoItemContainer::init(domElement);
}

//-------------------------------------------------------------------------
QGuidoItemContainer::QGuidoItemContainer(const QGuidoItemContainer * other	, QGraphicsItem * parent)
	: QLanguageItem(parent)
{
	QGuidoItemContainer::init(other);
}

//-------------------------------------------------------------------------
QGuidoItemContainer::~QGuidoItemContainer()
{
#ifdef USES_GUIDO_AR
	if ( mMidiRef != -1 )
	{
		stop();
		QGuidoAR::midiClose(mMidiRef);
		mPlayerState = STOPPED;
		mMidiRef = -1;
	}
#endif
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::exportToPdf(const QString& fileName)
{
	QPrinter printer;
	printer.setFullPage(true);
	printer.setOutputFileName( fileName );
	printer.setOutputFormat( QPrinter::PdfFormat );
	exportToPdf( &printer );
}

//-------------------------------------------------------------------------
QDomElement QGuidoItemContainer::saveToDomElement( QDomDocument * doc)
{
	QDomElement element = QLanguageItem::saveToDomElement(doc);
	
	element.setAttribute( DOM_GUIDO_ITEM_PROPORTIONAL_LAYOUT , mIsProportionalOn );
	element.setAttribute( DOM_GUIDO_ITEM_OPTIMAL_PAGE_FILL , mIsOptimalPageFillOn );
	element.setAttribute( DOM_GUIDO_ITEM_RESIZE_PAGE_ON , mGuidoItem->isResizePageToMusic() );
	element.setAttribute( DOM_GUIDO_ITEM_SCALE , mGuidoItem->transform().m11() );

	element.setAttribute( DOM_GUIDO_ITEM_CURRENT_PAGE, mGuidoItem->firstVisiblePage() );
	element.setAttribute( DOM_GUIDO_ITEM_GRID_WIDTH, mGuidoItem->gridWidth() );
	element.setAttribute( DOM_GUIDO_ITEM_GRID_HEIGHT, mGuidoItem->gridHeight() );

  	return element;
}

//-------------------------------------------------------------------------
bool QGuidoItemContainer::recognizes( const QMimeData * mimeData )
{
	return mimeData->hasFormat( GMN_CONTAINER_MIME_FLAG );
}

//-------------------------------------------------------------------------
bool QGuidoItemContainer::recognizes( const QDomElement * e )
{
	return ( QVariant( e->attribute( DOM_GUIDO_ITEM_PROPORTIONAL_LAYOUT, "-1" ) ).toInt() != -1 );
}

//-------------------------------------------------------------------------
bool QGuidoItemContainer::setCode( const QString& code )
{
//	bool oldIsValid = isValid();
	
	mGuidoItem->setGMNCode(code); 
	if (!isValid() && QGuidoImporter::musicxmlSupported()) {	// try to import file as MusicXML file
		std::stringstream out;
		if ( QGuidoImporter::musicxmlString2Guido(code.toAscii().constData(), true, out) )
		{
			mGuidoItem->setGMNCode( out.str().c_str() );
			unlinkFile();
		}
	}

	QLanguageItem::codeChanged();

	mPenBrushSwitcher.setActiveSwitch( isValid() ? 0 : SELECTION_SWITCH_INVALID_CODE );
	updateSelectionItem();
	mHasToUpdateDragImage = true;
		
	if ( mResizer )
		updateResizerColor();

#ifdef USES_GUIDO_AR
	stop();
	mMidiRef = QGuidoAR::getMidiRef( this->code() );
#endif
	
	return isValid(); 
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::descriptiveNameHasChanged()
{
	updateTitleBar();
}

//-------------------------------------------------------------------------
bool QGuidoItemContainer::isEqualTo( QLanguageItem * item ) const
{	
	if ( !QLanguageItem::isEqualTo(item) )
		return false;

	QGuidoItemContainer* otherContainer = dynamic_cast<QGuidoItemContainer*>(item);
	if ( !otherContainer )
		return false;

//	if ( mIsOptimalPageFillOn != otherContainer->mIsOptimalPageFillOn )
//		return false;
//	if ( mIsProportionalOn != otherContainer->mIsProportionalOn )
//		return false;
		
//	if ( mGuidoItem->firstVisiblePage()!= other->mGuidoItem->firstVisiblePage())
//		return false;

	return true;
}

//-------------------------------------------------------------------------
QGuidoGraphicsItem * QGuidoItemContainer::guidoItem() const
{
	return mGuidoItem;
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::setMinMaxScale(float min , float max)
{
	mMinScale = min;
	mMaxScale = max;
}

//-------------------------------------------------------------------------
//								Public slots							//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGuidoItemContainer::resized(const QRectF& newRect)
{
	if ( newRect.toRect() == rect().toRect() )
		return;

	
	if ( mResizeMode == RESIZE_GRID )
	{
		QRectF oldRect = rect();
		mGuidoItem->setGridWidth( mGuidoItem->gridWidth() * newRect.width() / rect().width() + 0.5f );
		mGuidoItem->setGridHeight( mGuidoItem->gridHeight() * newRect.height() / rect().height() + 0.5f );
		guidoGeometryChanged();
		
		QSizeF dSize( rect().size() - oldRect.size() );
		moveBy( dSize.width() * ( newRect.x() ? -1 : 0 ) , dSize.height() * ( newRect.y() ? -1 : 0 ) );
	}
	else if ( mResizeMode == RESIZE_FORMAT )
	{
		QRectF oldRect = rect();
	
		GuidoPageFormat f = mGuidoItem->guidoPageFormat();
		f.width = f.width * newRect.width() / rect().width();
		f.height = f.height * newRect.height() / rect().height();
		mGuidoItem->setGuidoPageFormat(f);

		//Control if how the page format was actually supported,
		//and set it to its actual value.
		if ( rect().toRect() != newRect.toRect() )	//has use int comparison, float isn't reliable
		{
			f = mGuidoItem->guidoPageFormat();
			f.width = f.width * rect().width() / newRect.width();
			f.height = f.height * rect().height() / newRect.height();
			mGuidoItem->setGuidoPageFormat(f);
		}
		
		QSizeF dSize( rect().size() - oldRect.size() );
		
//		moveBy( newRect.x() , newRect.y() );
		moveBy( dSize.width() * ( newRect.x() ? -1 : 0 ) , dSize.height() * ( newRect.y() ? -1 : 0 ) );
	}
	else
	{
		float xScale = mGuidoItem->transform().m11();
		float yScale = mGuidoItem->transform().m22();

		float dx = newRect.x();
		float dy = newRect.y();

		float sx = ( newRect.width() / rect().width() );
		float sy = ( newRect.height() / rect().height() );

		float boundedSx = sx;
		float boundedSy = sy;

		if ( mMaxScale )
			boundedSx = qMin( mMaxScale/xScale , boundedSx );
		boundedSx = qMax( mMinScale/xScale , boundedSx );
		if ( mMaxScale )
			boundedSy = qMin( mMaxScale/yScale , boundedSy );
		boundedSy = qMax( mMinScale/yScale , boundedSy );

		dx = dx * (boundedSx - 1 ) / ( sx - 1 );
		dy = dy * (boundedSy - 1 ) / ( sy - 1 );
		
		moveBy( dx , dy );
		
		mGuidoItem->scale( boundedSx , boundedSy );
		guidoGeometryChanged();

		Q_EMIT scaleChanged( mGuidoItem->transform().m11() );
	}
	mResizeMode = RESIZE_NORMAL;
	if ( mResizer )
		mResizer->setKeepAspectRatio(true);
}

//-------------------------------------------------------------------------
//								Protected slots							//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGuidoItemContainer::switchOptimalPageFill()
{
	setOptimalPageFill( !mIsOptimalPageFillOn );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::switchProportional()
{
	setProportional( !mIsProportionalOn );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::switchResizePage()
{
	setResizePageToMusic( !mGuidoItem->isResizePageToMusic() );
}

//-------------------------------------------------------------------------
//							Protected functions							 //
//-------------------------------------------------------------------------

/*
*	This method is called in subclasses constructors.
*
*	Why isn't it called direclty in QGuidoItemContainer constructor ?
*		->	because subclasses build a specific QGuidoItem, accessed
*			by the pure virtual QGuidoItemContainer::mGuidoItem,
*			and used in QGuidoItemContainer::init().
*		->	this design has pros (one initialisation code for the mGuidoItem) 
*			and cons (subclasses MUST call the baseclass's init function)
*/
//-------------------------------------------------------------------------
void QGuidoItemContainer::init()
{
	mGuidoItem = new QGuidoGraphicsItem( this );

#if !linux		// bug in Qt 4.4 with the linux cache mode 
	mGuidoItem->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
#endif

#ifdef USES_GUIDO_AR
	mMidiRef = -1;
	mPlayerState = STOPPED;

	QLanguageItem::plug( new QPaletteItemDropper( PALETTE_GUIDO_ELEMENT	, QList<int>() << PALETTE_GUIDO_ELEMENT , this , this ) );
#endif

	connect( mGuidoItem , SIGNAL(geometryHasChanged()), this, SLOT(guidoGeometryChanged()) );

	mIsProportionalOn		= DEFAULT_PROPORTIONAL;
	mIsOptimalPageFillOn	= DEFAULT_OPTIMAL_PAGE_FILL;
	mGuidoItem->setResizePageToMusic( DEFAULT_RESIZE_PAGE );

	setMinMaxScale(0,0);

	mHeadBar = new QTextPathItem( "" , true , TITLE_BAR_FONT_COLOR , TITLE_BAR_FONT , TITLE_BAR_FONT_PEN_WIDTH , this );
	mHeadBar->setZValue( 1 );

	mSelectionItem = new QGraphicsPathItem(this);	
	mSelectionItem->setZValue(-1);

	setAcceptHoverEvents(true);

	connect( this , SIGNAL(descriptiveNameChanged()) , this, SLOT(descriptiveNameHasChanged()) );

	// Set pens & brushes
	mPenBrushSwitcher.addFlag( SELECTION_FLAG_SELECTED , 2 , PenBrush(SELECTED_PEN , SELECTED_BRUSH ) );
	mPenBrushSwitcher.addFlag( SELECTION_FLAG_HIGHLIGHTED , 1 , PenBrush(HIGHLIGHTED_PEN , HIGHLIGHTED_BRUSH ) );
	mPenBrushSwitcher.addFlag( SELECTION_FLAG_IDLE , 0, PenBrush(STANDARD_PEN , STANDARD_BRUSH ) );
	mPenBrushSwitcher.setFlag( SELECTION_FLAG_IDLE , true );
	mPenBrushSwitcher.setSwitchedFlag(SELECTION_FLAG_IDLE, SELECTION_SWITCH_INVALID_CODE, PenBrush(INVALID_PEN, INVALID_STANDARD_BRUSH ) );
	mPenBrushSwitcher.setSwitchedFlag(SELECTION_FLAG_HIGHLIGHTED, SELECTION_SWITCH_INVALID_CODE, PenBrush(INVALID_PEN, INVALID_HIGHLIGHTED_BRUSH ) );
	mPenBrushSwitcher.setSwitchedFlag(SELECTION_FLAG_SELECTED, SELECTION_SWITCH_INVALID_CODE, PenBrush(INVALID_PEN, INVALID_BRUSH_SELECTED ) );

//Imported from QGuidoSPageItemContainer
	mNextPageTurner		= new QPageTurnerItem( true,	QPolygonF() ,	this );
	mPreviousPageTurner = new QPageTurnerItem( false,	QPolygonF() ,	this );

	mNextPageTurner->setZValue(4);
	mPreviousPageTurner->setZValue(4);

	mPageLabelItem = new QTextPathItem( "" , true , Qt::black , QFont(FONT_FAMILY , FONT_SIZE) , 1 , this );
	mPageLabelItem->setZValue(2);
	
	//Visibility controlled in updatePageItemsVisibility, only for non-storage items.
	mPageLabelItem->setVisible( false );
	mNextPageTurner->setVisible(false);
	mPreviousPageTurner->setVisible(false);

	//Set pen
	QPen pen = QPen( Qt::black );
	pen.setJoinStyle( Qt::RoundJoin );
	pen.setWidth( ARROW_PEN_WIDTH );
	mNextPageTurner->setBrush( SELECTED_BRUSH );
	mPreviousPageTurner->setBrush( SELECTED_BRUSH );
	mNextPageTurner->setPen( pen );
	mPreviousPageTurner->setPen( pen );
	
	mPageLabelItem->setPen( SELECTED_PEN );
	mPageLabelItem->setBrush( SELECTED_BRUSH );
	updatePageIndexLabel();

	guidoUpdateGeometry( INVALID_RECT );
	
	mResizeMode = RESIZE_NORMAL;
	mResizer = 0;
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::init(const QMimeData * mimeData)
{
	init();
	loadFromMimeData(mimeData);
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::init(const QDomElement * domElement)
{
	init();
	loadFromDomElement(domElement);
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::init(const QGuidoItemContainer * other)
{
	init();
	loadFromOtherContainer(other);
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::loadFromMimeData( const QMimeData * mimeData )
{
	load( mimeData );

	if ( mimeData->hasFormat(GMN_CONTAINER_MIME_PROPORTIONAL_ON) )
		mIsProportionalOn	= mimeData->data( GMN_CONTAINER_MIME_PROPORTIONAL_ON ).toInt();
	else
		mIsProportionalOn = DEFAULT_PROPORTIONAL;
		
	if ( mimeData->hasFormat(GMN_CONTAINER_MIME_OPTIMAL_PAGE_FILL_ON) )
		mIsOptimalPageFillOn	= mimeData->data( GMN_CONTAINER_MIME_OPTIMAL_PAGE_FILL_ON ).toInt();
	else
		mIsOptimalPageFillOn = DEFAULT_OPTIMAL_PAGE_FILL;

	if ( mimeData->hasFormat(GMN_CONTAINER_MIME_RESIZE_PAGE_ON) )
		setResizePageToMusic( mimeData->data( GMN_CONTAINER_MIME_RESIZE_PAGE_ON ).toInt() );
	else
		setResizePageToMusic( DEFAULT_RESIZE_PAGE );


	if ( mimeData->hasFormat( GMN_CONTAINER_MIME_SCALE ) )
	{
		float scale = mimeData->data( GMN_CONTAINER_MIME_SCALE ).toDouble();
		resize( scale , scale );
	}
	
	updateLayoutSettings();
	
	mGuidoItem->setGridWidth( mimeData->data( GMN_CONTAINER_MIME_GRID_WIDTH ).toInt() );
	mGuidoItem->setGridHeight( mimeData->data( GMN_CONTAINER_MIME_GRID_HEIGHT ).toInt() );
	setCurrentPage( mimeData->data( GMN_CONTAINER_MIME_PAGE_INDEX ).toInt() );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::loadFromOtherContainer( const QGuidoItemContainer * otherContainer )
{
	setResizePageToMusic( otherContainer->mGuidoItem->isResizePageToMusic() );
	load( otherContainer );
	mIsOptimalPageFillOn	= otherContainer->mIsOptimalPageFillOn;
	mIsProportionalOn		= otherContainer->mIsProportionalOn;

	mGuidoItem->scale( otherContainer->mGuidoItem->transform().m11() , otherContainer->mGuidoItem->transform().m22() );
	updateLayoutSettings();
	mGuidoItem->setGridWidth(otherContainer->mGuidoItem->gridWidth());
	mGuidoItem->setGridHeight(otherContainer->mGuidoItem->gridHeight());
	setCurrentPage( otherContainer->mGuidoItem->firstVisiblePage());
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::loadFromDomElement( const QDomElement * e)
{
	if ( e->tagName() != DOM_LANGUAGE_ITEM )
		return;
 
	mIsProportionalOn		= QVariant( e->attribute( DOM_GUIDO_ITEM_PROPORTIONAL_LAYOUT, "#DEFAULT_PROPORTIONAL" ) ).toBool();
	mIsOptimalPageFillOn	= QVariant( e->attribute( DOM_GUIDO_ITEM_OPTIMAL_PAGE_FILL, "#DEFAULT_OPTIMAL_PAGE_FILL" ) ).toBool();
	setResizePageToMusic( QVariant( e->attribute( DOM_GUIDO_ITEM_RESIZE_PAGE_ON, "#DEFAULT_RESIZE_PAGE" ) ).toBool() );

	load( e );

	float scale = QVariant( e->attribute( DOM_GUIDO_ITEM_SCALE, "1" ) ).toDouble();
	resize( scale , scale );
	
	updateLayoutSettings();

	int currentPage = QVariant( e->attribute( DOM_GUIDO_ITEM_CURRENT_PAGE, "1" ) ).toInt();
	int gridWidth = QVariant( e->attribute( DOM_GUIDO_ITEM_GRID_WIDTH, "1" ) ).toInt();
	int gridHeight = QVariant( e->attribute( DOM_GUIDO_ITEM_GRID_HEIGHT, "1" ) ).toInt();
	mGuidoItem->setGridWidth(gridWidth);
	mGuidoItem->setGridHeight(gridHeight);
	setCurrentPage( currentPage );
}

//-------------------------------------------------------------------------
QMenu *  QGuidoItemContainer::buildContextMenu()
{
	QMenu * menu = QLanguageItem::buildContextMenu();

	QAction * exportAct = new QAction( "Export" , this );
	connect( exportAct , SIGNAL( triggered() ) , this , SIGNAL( exportItem() ) );
	menu->addAction( exportAct );

	QMenu * layoutMenu = menu->addMenu( CONTEXT_MENU_LAYOUT );
	layoutMenu->setObjectName( CONTEXT_MENU_LAYOUT );
	
	//Layout actions :Proportional layout
	QString layoutProportionalMsg = QString("Switch ") + ( mIsProportionalOn ? "OFF" : "ON" ) + " Proportionnal layout";
	QAction * layoutProportionalSwitch = new QAction( layoutProportionalMsg , layoutMenu );
	connect( layoutProportionalSwitch , SIGNAL(triggered()) , this , SLOT(switchProportional()) );
	layoutMenu->addAction(layoutProportionalSwitch);

	//Layout actions : Optimal page fill
	QString layoutOptimalPageFillMsg = QString("Switch ") + ( mIsOptimalPageFillOn ? "OFF" : "ON" ) + " Optimal page fill";		
	QAction * layoutOptimalPageFillSwitch = new QAction( layoutOptimalPageFillMsg , layoutMenu );
	connect( layoutOptimalPageFillSwitch , SIGNAL(triggered()) , this , SLOT(switchOptimalPageFill()) );
	layoutMenu->addAction(layoutOptimalPageFillSwitch);	

	//Layout actions : Auto-resizePageToMusic
	QString layoutResizePageMsg = QString("Switch ") + ( mGuidoItem->isResizePageToMusic() ? "OFF" : "ON" ) + " Auto Resize-Page";		
	QAction * layoutResizePageSwitch = new QAction( layoutResizePageMsg , layoutMenu );
	connect( layoutResizePageSwitch , SIGNAL(triggered()) , this , SLOT(switchResizePage()) );
	layoutMenu->addAction(layoutResizePageSwitch);	

//"Change the current page" actions.
	QAction * firstPage = new QAction( "First page" , this );
	QAction * previousPage = new QAction( "Previous page" , this );
	QAction * nextPage = new QAction( "Next page" , this );
	QAction * lastPage = new QAction( "Last page" , this );
	
	connect(firstPage , SIGNAL(triggered()) , this , SLOT(firstPage()) );
	connect(previousPage , SIGNAL(triggered()) , this , SLOT(previousPage()) );
	connect(nextPage , SIGNAL(triggered()) , this , SLOT(nextPage()) );
	connect(lastPage , SIGNAL(triggered()) , this , SLOT(lastPage()) );

	layoutMenu->addAction( newSeparator(this) );
	layoutMenu->addAction(firstPage);
	layoutMenu->addAction(previousPage);
	layoutMenu->addAction(nextPage);
	layoutMenu->addAction(lastPage);

/*
	bool onceOnly = false;
	for ( int i = 0 ; i < menu->actions().size() ; i++ )
	{
		if ( menu->actions()[i]->data().toString() == CONTEXT_MENU_LAYOUT )
		{
			if ( !onceOnly )
			{
				layoutMenu->addSeparator();
				onceOnly = true;
			}
//			qDebug("QGuidoItemContainer::buildContextMenu() : action removed from menu and added to ");
			layoutMenu->addAction( menu->actions()[i] );
			menu->actions()[i]->setParent( layoutMenu );
			menu->removeAction( menu->actions()[i] );
			i--;
		}
	}
*/

	return menu;
}

//-------------------------------------------------------------------------
QImage* QGuidoItemContainer::buildDragImage()
{
	return new QImage( itemToImage( mSelectionItem , 1.0f , 0 , true ) );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::exportToPdf( QPrinter * printer )
{
	int originalPage = mGuidoItem->firstVisiblePage();
			
	mGuidoItem->setPage(1);
	QRectF firstPageRect = guidoItem()->boundingRect();
	printer->setPaperSize( QSizeF( firstPageRect.width() , firstPageRect.height() ) , QPrinter::Millimeter );

	QPainter painter;
	painter.begin( printer );
	
	painter.setWindow( firstPageRect.toRect() );
	
	int pCount = mGuidoItem->pageCount();
	for ( int page = 1 ; page <= pCount ; page++ )
	{
		QRectF rect = guidoItem()->boundingRect();
		
		QStyleOptionGraphicsItem option;
		option.exposedRect = rect;
		mGuidoItem->paint( &painter , &option , 0 );

		if (page != pCount)
		{
			mGuidoItem->setPage(page+1);
			rect = guidoItem()->boundingRect();
			
			printer->newPage();
		}
	}
	painter.end();

	mGuidoItem->setPage( originalPage );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updateLayoutSettings()
{
	GuidoLayoutSettings settings = mGuidoItem->guidoLayoutSettings();
		
	settings.optimalPageFill = ( mIsOptimalPageFillOn ) ? 1 : 0;
	settings.force  = ( mIsProportionalOn ) ? PROPORTIONAL_FORCE : NO_PROPORTIONAL_FORCE;
	settings.spring = ( mIsProportionalOn ) ? PROPORTIONAL_SPRING : NO_PROPORTIONAL_SPRING;
		
	mGuidoItem->setGuidoLayoutSettings( settings );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::setOptimalPageFill(bool isOptimalPageFillOn)
{
	mIsOptimalPageFillOn = isOptimalPageFillOn;
	updateLayoutSettings();
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::setResizePageToMusic(bool isResizePageOn)
{
	mGuidoItem->setResizePageToMusic(isResizePageOn);
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::setProportional(bool isProportionalOn)
{
	mIsProportionalOn = isProportionalOn;
	updateLayoutSettings();
}

//-------------------------------------------------------------------------
QMimeData * QGuidoItemContainer::buildMimeData()
{
	QMimeData * data = QLanguageItem::buildMimeData();
	data->setData( GMN_CONTAINER_MIME_FLAG , 0 );
	data->setData( GMN_CONTAINER_MIME_PROPORTIONAL_ON , QByteArray::number(mIsProportionalOn) );
	data->setData( GMN_CONTAINER_MIME_OPTIMAL_PAGE_FILL_ON , QByteArray::number(mIsOptimalPageFillOn) );
	data->setData( GMN_CONTAINER_MIME_RESIZE_PAGE_ON , QByteArray::number(mGuidoItem->isResizePageToMusic()) );
	data->setData( GMN_CONTAINER_MIME_SCALE , QByteArray::number(mGuidoItem->transform().m11() ) );
	
	data->setData( GMN_CONTAINER_MIME_PAGE_INDEX , QByteArray::number( mGuidoItem->firstVisiblePage()) );
	data->setData( GMN_CONTAINER_MIME_GRID_WIDTH , QByteArray::number( mGuidoItem->gridWidth()) );
	data->setData( GMN_CONTAINER_MIME_GRID_HEIGHT , QByteArray::number( mGuidoItem->gridHeight()) );
	return data;
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updateTitleBarVisibility()
{
//	mHeadBar->setVisible( ( mPenBrushSwitcher.isFlagOn( SELECTION_FLAG_HIGHLIGHTED ) || isSelected() ) && file().length() && ( mHeadBar->textItem()->boundingRect().height() < rect().height() ) );
	mHeadBar->setVisible( false );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updateTitleBar()
{
	int barHeight = QFontMetrics( mHeadBar->textItem()->font() ).height();

	QPainterPath path;
	path.moveTo( 0 , barHeight );
	path.arcTo( QRect(0,0,ROUNDED_RECT_RADIUS * 2 , ROUNDED_RECT_RADIUS * 2) , 180 , -90 );
	path.lineTo( rect().width() - ROUNDED_RECT_RADIUS * 2 , 0 );
	path.arcTo( QRect( path.currentPosition().x() , path.currentPosition().y() ,ROUNDED_RECT_RADIUS * 2 , ROUNDED_RECT_RADIUS * 2) , 90 , -90 );
	path.lineTo( rect().width() , barHeight );
	path.closeSubpath();

	mHeadBar->setPath( path );

	updateTitleText();
	updateTitleBarVisibility();
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updateTitleText()
{
	if ( !file().length() )
		return;

	if ( !rect().isValid() )
		return;

	mHeadBar->setText( getDescriptiveFileName() );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updateSyntaxErrorItem()
{
/*
	QPainterPath path;
	path.addRoundedRect( rect() ,  mRoundedRectRadius  , mRoundedRectRadius  );
	mSyntaxErrorItem->setPath( path );
*/
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updateSelectionItem()
{
	mSelectionItem->setPen( mPenBrushSwitcher.activeObject().mPen );
	mSelectionItem->setBrush( mPenBrushSwitcher.activeObject().mBrush );
	mHeadBar->setPen( mPenBrushSwitcher.activeObject().mPen );
	mHeadBar->setBrush( mPenBrushSwitcher.activeObject().mBrush );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::setFile(const QString& fileName)
{
	QLanguageItem::setFile(fileName);
	updateTitleText();
	updateTitleBarVisibility();
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::keyPressEvent ( QKeyEvent * event )
{
	if ( mIsStorageModeOn )
		return;
		
	QLanguageItem::keyPressEvent( event );
	int key = event->key();
	if (		( key == Qt::Key_PageUp ) 
			||	( key == Qt::Key_PageDown ) 
			)
	{
		int pageIncrement;
		switch (key)
		{
			case Qt::Key_PageUp:	pageIncrement = -1;	break;
			case Qt::Key_PageDown:	pageIncrement = 1;	break;
			default:				pageIncrement = 0;	break;
		}
		if ( pageIncrement == 1 )
			nextPage();
		else
			previousPage();
			
		event->accept();
	}
	else if ( key == Qt::Key_G )
	{
		mResizeMode = RESIZE_GRID;
		mResizer->setKeepAspectRatio(false);
		event->accept();
	}
	else if ( key == Qt::Key_F )
	{
		mResizeMode = RESIZE_FORMAT;
		mResizer->setKeepAspectRatio(false);
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::setResizer( QItemResizer* resizer )
{
	mResizer = resizer;
	updateResizerColor();
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updatePageItemsVisibility()
{
	if ( mIsStorageModeOn )
		return;

	int firstVisiblePage = mGuidoItem->firstVisiblePage();
	int lastVisiblePage = mGuidoItem->lastVisiblePage();
	int pageCount = mGuidoItem->pageCount();
	mNextPageTurner->setVisible		( mGuidoItem->isGMNValid() && isSelected() && ( pageCount > lastVisiblePage ) );
	mPreviousPageTurner->setVisible	( mGuidoItem->isGMNValid() && isSelected() && ( firstVisiblePage > 1 ) );
	mPageLabelItem->setVisible		( mGuidoItem->isGMNValid() && isSelected() && !((mGuidoItem->firstVisiblePage()==1) && (mGuidoItem->lastVisiblePage()==mGuidoItem->pageCount())) );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updatePageIndexLabel()
{
//	qDebug() << "updatePageIndexLabel() : " << mGuidoItem->firstVisiblePage() << "-" << mGuidoItem->lastVisiblePage() << "/" << mGuidoItem->pageCount() ;
	
	//Updates the page index item.
	QString pageLabel;
	if ( mGuidoItem->firstVisiblePage() != mGuidoItem->lastVisiblePage() )
		QTextStream(&pageLabel) << mGuidoItem->firstVisiblePage() << "-" << mGuidoItem->lastVisiblePage() << "/" << mGuidoItem->pageCount() ;
	else
		QTextStream(&pageLabel) << mGuidoItem->firstVisiblePage() << "/" << mGuidoItem->pageCount() ;
	
	QFontMetrics fontMetrics( mPageLabelItem->textItem()->font() );
	QRect pageLabelRect = fontMetrics.boundingRect( pageLabel );
	pageLabelRect.moveTo( rect().width() , rect().height() );
	pageLabelRect.translate( -pageLabelRect.width() + PAGE_LABEL_RECT_OUT_WIDTH , -pageLabelRect.height() + PAGE_LABEL_RECT_OUT_HEIGHT );
	pageLabelRect.translate(-5,-2);
	pageLabelRect.setWidth( pageLabelRect.width() + 10 );
	pageLabelRect.setHeight( pageLabelRect.height() + 4 );
	QPainterPath pageLabelPath;
	pageLabelPath.addRoundedRect( pageLabelRect , ROUNDED_RECT_RADIUS , ROUNDED_RECT_RADIUS );
	if ( mPageLabelItem->path() != pageLabelPath )
		mPageLabelItem->setPath( pageLabelPath );

	mPageLabelItem->setText(pageLabel);
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::updateResizerColor()
{
	mResizer->setPen( isValid() ? SELECTED_PEN : INVALID_PEN );
	mResizer->setBrush( isValid() ? HIGHLIGHTED_BRUSH : INVALID_STANDARD_BRUSH );	
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::setCurrentPage(int page)
{
	mGuidoItem->setPage(page);

	guidoGeometryChanged();
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::firstPage()
{
	if ( mGuidoItem->firstVisiblePage()!= 1 )
		setCurrentPage(1);
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::previousPage()
{
	int currentPage = mGuidoItem->firstVisiblePage();
	if ( currentPage - 1 >= 1 )
		setCurrentPage( currentPage - 1 );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::nextPage()
{
	int currentPage = mGuidoItem->firstVisiblePage();
	if ( currentPage + 1 <= mGuidoItem->pageCount() )
		setCurrentPage( currentPage + 1 );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::lastPage()
{
	int currentPage = mGuidoItem->firstVisiblePage();
	int pageCount = mGuidoItem->pageCount();
	if ( currentPage != pageCount )
		setCurrentPage( pageCount );
}

//-------------------------------------------------------------------------
QPolygonF getRightArrowPolygon(const QRectF& boundingRect)
{
	QVector<QPointF> rightPoints;
	rightPoints << QPointF(0,0) << QPointF(-PAGE_TURNER_ARROW_SIZE,-PAGE_TURNER_ARROW_SIZE) << QPointF(-PAGE_TURNER_ARROW_SIZE,PAGE_TURNER_ARROW_SIZE);
	QPolygonF rightPolygon( rightPoints );
	rightPolygon.translate( boundingRect.width() + PAGE_TURNER_ARROW_SIZE/2 , boundingRect.height() / 2 );
	return rightPolygon;
}

//-------------------------------------------------------------------------
QPolygonF getLeftArrowPolygon(const QRectF& boundingRect)
{
	QVector<QPointF> leftPoints;
	leftPoints << QPointF(0,0) << QPointF(PAGE_TURNER_ARROW_SIZE,-PAGE_TURNER_ARROW_SIZE) << QPointF(PAGE_TURNER_ARROW_SIZE,PAGE_TURNER_ARROW_SIZE);
	QPolygonF leftPolygon( leftPoints );
	leftPolygon.translate( 0 - PAGE_TURNER_ARROW_SIZE/2, boundingRect.height() / 2 );
	return leftPolygon;
}

//-------------------------------------------------------------------------
QVariant QGuidoItemContainer::itemChange( GraphicsItemChange change, const QVariant& value )
{
	QVariant standardResult = QLanguageItem::itemChange( change , value );

	if ( change == QGraphicsItem::ItemSelectedHasChanged )
	{
		updateTitleBarVisibility();
		mPenBrushSwitcher.setFlag( SELECTION_FLAG_SELECTED , value.toBool() );
		updateSelectionItem();
		updatePageItemsVisibility();
	}
	return standardResult;
}

//-------------------------------------------------------------------------	
void QGuidoItemContainer::hoverEnterEvent ( QGraphicsSceneHoverEvent * e )
{
	QLanguageItem::hoverEnterEvent(e);
	
	mPenBrushSwitcher.setFlag( SELECTION_FLAG_HIGHLIGHTED , true );	
	updateSelectionItem();

	updateTitleBarVisibility();
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::hoverLeaveEvent ( QGraphicsSceneHoverEvent * e )
{
	QLanguageItem::hoverLeaveEvent(e);

	mPenBrushSwitcher.setFlag( SELECTION_FLAG_HIGHLIGHTED , false );
	updateSelectionItem();
	updateTitleBarVisibility();
}

/*
* The mGuidoItem geometry has changed, for various reasons (codeChanged, layout, pages alignment, ...)
* so the QGuidoItemContainer compute its new size (for now, uses the new size of mGuidoItem)
* and call updateGeometry(QRect) with it.
*
* This method doesn't exists in QLanguageItem.
*/
//-------------------------------------------------------------------------
void QGuidoItemContainer::guidoGeometryChanged()
{	
//	if ( isValid() )
//	{
		QRectF itemRect = mGuidoItem->boundingRect();
		if ( ( itemRect.width() <= 0 ) || ( itemRect.height() <= 0 ) )
			itemRect = INVALID_RECT;

		QRectF newGeometry = mapFromItem( mGuidoItem , itemRect ).boundingRect();
		if ( newGeometry != rect() )
		{
			guidoUpdateGeometry(newGeometry);
		}
		//Updates page-turn arrows visibility
		updatePageItemsVisibility();
	
		//Updates page label
		updatePageIndexLabel();
//	}
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::guidoUpdateGeometry(const QRectF& newGeometry)
{
	QLanguageItem::updateGeometry( newGeometry );

	QPainterPath p;
	p.addRoundedRect( newGeometry , ROUNDED_RECT_RADIUS , ROUNDED_RECT_RADIUS );
	mSelectionItem->setPath(p);
	
	updateTitleBar();
	updateSyntaxErrorItem();
	
	//Updates page-turn arrows positions
	mNextPageTurner->setPolygon( getRightArrowPolygon(newGeometry) );
	mPreviousPageTurner->setPolygon( getLeftArrowPolygon(newGeometry) );
}

//-------------------------------------------------------------------------
void QGuidoItemContainer::resize( float xScale , float yScale )
{
	resized( QRectF( 0, 0, xScale * rect().width() , yScale * rect().height()) );
}

//------------------------------------------------------------------------
//						Guido AR section (palette stuff, guido2midi)	//
//------------------------------------------------------------------------

#ifdef USES_GUIDO_AR

//-------------------------------------------------------------------------
void QGuidoItemContainer::play()
{

	if ( mMidiRef == -1 )
	{
		stop();
		mMidiRef = QGuidoAR::getMidiRef(mGuidoItem->gmnCode());
	}
	
	if ( mPlayerState == PAUSED )
		QGuidoAR::midiResume(mMidiRef);
	else
		QGuidoAR::midiPlay(mMidiRef);
		
	mPlayerState = PLAYING;

}

//-------------------------------------------------------------------------
void QGuidoItemContainer::pause()
{

	if ( mMidiRef != -1 )
	{
		QGuidoAR::midiPause(mMidiRef);
		mPlayerState = PAUSED;
	}

}

//-------------------------------------------------------------------------
void QGuidoItemContainer::stop()
{

	if ( mMidiRef != -1 )
	{
		QGuidoAR::midiStop(mMidiRef);
		mPlayerState = STOPPED;
		mMidiRef = -1;
	}

}

#endif
