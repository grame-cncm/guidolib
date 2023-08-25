/*
 * QGuidoItemContainer.cpp
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
#include "QLanguageItem.h"

#include "QPaletteItem.h"

#include <QApplication>
#include <QGraphicsView>
#include <QVariant>
#include <QAction>
#include <QActionGroup>
#include <QList>
#include <QPoint>
#include <QGraphicsSceneContextMenuEvent>
#include <QPainter>
#include <QTextStream>
#include <QMimeData>
#include <QDrag>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QtDebug>
#include <QMessageBox>

#include <assert.h>

#define DEFAULT_INVALID_RECT		QRectF( 0 , 0 , 100 , 100 )
#define INVALID_RECT_BOTTOM_MARGIN	mInvalidTextMargin
#define INVALID_RECT_LEFT_MARGIN	mInvalidTextMargin
#define INVALID_RECT_RIGHT_MARGIN	mInvalidTextMargin
#define INVALID_RECT_TOP_MARGIN		mInvalidTextMargin

#define ITEM_SCALE_NULL_POINT QPointF(-1,-1)
#define ITEM_SCALE_NULL -1

#define KEY_ITEM_MOVE 5

#define TITLE_BAR_TEXT_MARGIN	10

#define DOM_LANGUAGE_ITEM_X			"ItemX"
#define DOM_LANGUAGE_ITEM_Y			"ItemY"
#define DOM_LANGUAGE_ITEM_Z			"ItemZ"
#define DOM_LANGUAGE_ITEM_SCALE		"Scale"
#define DOM_LANGUAGE_ITEM_CODE		"code"
#define DOM_LANGUAGE_ITEM_FILE		"file"

#define MIME_CODE			"Code"
#define MIME_FILE_NAME		"FileName"
#define MIME_SCALE			"Scale"
#define MIME_HOTSPOT_X		"HotSpotX"
#define MIME_HOTSPOT_Y		"HotSpotY"

#define FILE_MENU	"FileMenu"

//-------------------------------------------------------------------------
//								QTextPathItem class						///
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
QTextPathItem::QTextPathItem( const QString& text , bool centerText , const QColor& textColor , const QFont& textFont , float textPenWidth , QGraphicsItem* parent )
	: QGraphicsPathItem(parent)
{
	mText = new QGraphicsSimpleTextItem( text , this);
	mText->setPen( QPen( textColor , textPenWidth ) );
	mText->setBrush( QBrush( textColor ) );
	mText->setFont( textFont );
	mCenterText = centerText;
}

//-------------------------------------------------------------------------
void QTextPathItem::setText(const QString& text)
{
	if ( mText->text() != text )
		mText->setText(text);
	
	QString truncString = "...";
	int skippedChar = (int)truncString.length() + 1;
	while ( ( mText->boundingRect().width() > boundingRect().width() - TITLE_BAR_TEXT_MARGIN ) && ( skippedChar < text.length() ) )
	{
		mText->setText( text.left( text.length() - skippedChar ) + truncString );
		skippedChar++;
	}

	updateTextPos();
}

//-------------------------------------------------------------------------
void QTextPathItem::setPath( const QPainterPath& p )
{
	QGraphicsPathItem::setPath(p);
	
	setText( mText->text() );
}

//-------------------------------------------------------------------------		
void QTextPathItem::updateTextPos()
{
	if ( mCenterText )
	{				
		QPointF c = boundingRect().center();
		mText->setPos( c.x() - mText->boundingRect().width()/2.0f , c.y() - mText->boundingRect().height()/2.0f );
	}
	else
	{
		mText->setPos( 5 , 5 );
	}
}

//-------------------------------------------------------------------------
//								Misc functions							//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
static QString getFileContent(const QString& fileName)
{
	if ( !QFile::exists(fileName) )
		return "";
		
	QString content = "";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		file.close();
		return "";
	}

    while (!file.atEnd()) 
	{
        QByteArray line = file.readLine();
        content += line;
    }
	file.close();
		
	return content;
}

//-------------------------------------------------------------------------
//								Static Public functions					//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QLanguageItem::addFile(const QString& fileName , QLanguageItem* usingItem)
{
	if ( fileName.length() > 0 )
	{
		mUsedFiles.insert( fileName , usingItem );

		if ( !mFileContents.contains(fileName) )
			mFileContents.insert( fileName , getFileContent(fileName) );

	}	
}

//-------------------------------------------------------------------------
void QLanguageItem::removeFile(const QString& fileName , QLanguageItem* usingItem)
{
	if ( fileName.length() > 0 )
	{
		mUsedFiles.remove( fileName , usingItem );

		if ( !mUsedFiles.contains( fileName ) )
		{
			mFileContents.remove( fileName );
		}

	}
}

//-------------------------------------------------------------------------
//								Public functions						//
//-------------------------------------------------------------------------

//#include <QRegExp>
//-------------------------------------------------------------------------
/*
QString getCopyName( QString originalName )
{
	QRegExp rx(".*\\(\\d+\\)");
    int pos = rx.indexIn( originalName );
	
	int copyNumber = 1;
	
	if ( pos != -1 )
	{
		int numberPos = originalName.lastIndexOf("(");
		QString number = originalName.right( originalName.length() - numberPos - 1 );
		number = number.left( number.length() - 1 );
		copyNumber =  QVariant(number).toInt() + 1; 
		originalName = originalName.left( numberPos );
	}
	return originalName + "(" + QVariant(copyNumber).toString() + ")";
}
*/

//-------------------------------------------------------------------------
QLanguageItem::QLanguageItem(QGraphicsItem * parent) : QGraphicsRectItem(parent)
{
	mFile = "";
	mIsModified = false;
	
	mIsStorageModeOn		= false;

	mDragImage = 0;
	mHasToUpdateDragImage = true;

	mIgnoreChangeNotification = false;
	mIsProcessingFileChange = false;
}

//-------------------------------------------------------------------------
QLanguageItem::~QLanguageItem()
{
	delete mDragImage;
	
	QLanguageItem::removeFile( file() , this );
}

//-------------------------------------------------------------------------
QDomElement QLanguageItem::saveToDomElement( QDomDocument * doc)
{
	QDomElement element = doc->createElement( DOM_LANGUAGE_ITEM );

	element.setAttribute( DOM_LANGUAGE_ITEM_X, pos().x() );
	element.setAttribute( DOM_LANGUAGE_ITEM_Y, pos().y() );
	element.setAttribute( DOM_LANGUAGE_ITEM_Z, zValue() );

	if ( file().length() > 0 )
		element.setAttribute( DOM_LANGUAGE_ITEM_FILE , file() );
	else
	{
		QDomCDATASection codeSection = doc->createCDATASection( code() );
		element.appendChild(codeSection);
	}

	return element;
}

//-------------------------------------------------------------------------
bool QLanguageItem::isEqualTo( QLanguageItem * item ) const
{
	if ( this->code() != item->code() )
		return false;

	return true;
}

//-------------------------------------------------------------------------
void QLanguageItem::keyPressEvent ( QKeyEvent * event )
{
	if (mIsStorageModeOn)
		return;

	int key = event->key();
	if  ( ( key == Qt::Key_Left ) || ( key == Qt::Key_Right ) || ( key == Qt::Key_Down ) || ( key == Qt::Key_Up ) )
	{
		int dx = 0 , dy = 0;
		switch (key)
		{
			case Qt::Key_Left:  dx = -KEY_ITEM_MOVE;	dy = 0; break;
			case Qt::Key_Right:	dx = KEY_ITEM_MOVE;		dy = 0; break;
			case Qt::Key_Up:	dy = -KEY_ITEM_MOVE;	dx = 0; break;
			case Qt::Key_Down:	dy = KEY_ITEM_MOVE;		dx = 0; break;
		}
		moveBy( dx , dy );
		
		event->accept();
		return;
	}
	else
	{
		event->ignore();
	}
}

//-------------------------------------------------------------------------
QString QLanguageItem::getDescriptiveFileName() const
{
	QString result = name();
	if ( isModified() )
		result += "*";
	return result;
}

//-------------------------------------------------------------------------
QString QLanguageItem::name() const
{
	if ( file().length() )
	{
		QFileInfo f( file() );
		QString result = f.fileName();
		return result;
	}
	else
	{
		return "untitled";
	}
}

//-------------------------------------------------------------------------
void QLanguageItem::plug( QPaletteItemDropper * dropper )
{
	dropper->setParentItem( this );
	dropper->updateGeometry( this->boundingRect() );
	connect( this ,	SIGNAL( updateChildrenItemsGeometry(const QRectF&) ) , dropper , SLOT(updateGeometry(const QRectF&)) );			
	connect( this , SIGNAL(dragStarted(int&,QGraphicsItem*&)) , dropper , SLOT( dragStarted(int&,QGraphicsItem*&) ) );		
}

//-------------------------------------------------------------------------
QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath())
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

//-------------------------------------------------------------------------
void QLanguageItem::save(const QString& fileName)
{
	QFile file( fileName );
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);
	out << code();
	
	setFile( fileName );
	setModified( false );
	
	mIgnoreChangeNotification = true;
	QLanguageItem::fileChanged( fileName , code() );
	mIgnoreChangeNotification = false;	
}

//-------------------------------------------------------------------------
void QLanguageItem::setFile( const QString& fileName )
{
	QLanguageItem::removeFile( file() , this );

	mFile = fileName;

	Q_EMIT descriptiveNameChanged();
	QLanguageItem::addFile( file() , this );
}

//-------------------------------------------------------------------------
QString QLanguageItem::file() const
{
	return mFile;
}

//-------------------------------------------------------------------------
void QLanguageItem::setMimeId( const QString& mimeId )
{
	mMimeId = mimeId;
}

//-------------------------------------------------------------------------
void QLanguageItem::setStorageMode( bool isStorageModeOn )
{
	if ( isStorageModeOn == mIsStorageModeOn )
		return;

	mIsStorageModeOn = isStorageModeOn;
	
	// Break the file link for items in storage mode.
	if ( mIsStorageModeOn )
	{
		setFile("");
	}
}

//-------------------------------------------------------------------------
void QLanguageItem::addContainerAction( QLanguageItemAction * containerAction )
{
	mContainerActionList << containerAction;
}

//-------------------------------------------------------------------------
bool QLanguageItem::isModified() const
{
	return mIsModified;
}

//-------------------------------------------------------------------------
bool QLanguageItem::fileNotFound() const
{
	return ( ( mFile.length() > 0 ) && ( !QFile::exists( mFile ) ) );
}

//-------------------------------------------------------------------------
QPointF QLanguageItem::moveItem( const QPointF& destPoint )
{
	QPointF oldPos = pos();
	setPos( pos() + destPoint - mMousePressPoint );
	return pos() - oldPos;
}

//-------------------------------------------------------------------------
void QLanguageItem::load(const QDomElement * e)
{
	if ( e->tagName() != DOM_LANGUAGE_ITEM )
		return;

	QString fileName = QVariant( e->attribute( DOM_LANGUAGE_ITEM_FILE, "" ) ).toString();
	if ( fileName.length() > 0 )
		loadFile( fileName );
	else
		setCode( e->firstChild().toCDATASection().data() );
	
	float x = QVariant( e->attribute( DOM_LANGUAGE_ITEM_X, "0" ) ).toDouble();
	float y = QVariant( e->attribute( DOM_LANGUAGE_ITEM_Y, "0" )).toDouble();
	setPos(x,y);
	setZValue( QVariant( e->attribute( DOM_LANGUAGE_ITEM_Z, "0" )).toDouble() );
}

//-------------------------------------------------------------------------
void QLanguageItem::load(const QLanguageItem * other)
{
	setCode( other->code() );
	setModified( other->isModified() );
	setPos( other->pos() );
	setZValue( other->zValue() );
}

//------------------------------------------------------------
void QLanguageItem::loadFile( const QString& fileName )
{
	QString absoluteFileName = QFileInfo(fileName).absoluteFilePath();
	
	setFile( absoluteFileName );
	reloadItem();
}

//------------------------------------------------------------
QMimeData *	QLanguageItem::buildMimeData()
{
	QMimeData *data = new QMimeData;
	
	data->setData( mMimeId , QByteArray::fromRawData( reinterpret_cast<char*>(this) , 4 ) );

	data->setText( code() );
	data->setData( MIME_FILE_NAME ,	file().toUtf8().data()  );
	data->setData( MIME_CODE ,		code().toUtf8().data() );

	if ( !mIsStorageModeOn )
	{
//		QByteArray a = QByteArray::number( containedItem()->transform().m11() );
//		data->setData( MIME_SCALE , a );
	
		QPointF p = mapFromScene(mMousePressPoint);
		data->setData( MIME_HOTSPOT_X , QByteArray::number( p.x() * sceneTransform().m11() ) );
		data->setData( MIME_HOTSPOT_Y , QByteArray::number( p.y() * sceneTransform().m22() ) );
	}
	
	QByteArray b;
	b.append(mCopyDrag);
	data->setData( MIME_DO_COPY , b );
		
	return data;
}

//-------------------------------------------------------------------------
void QLanguageItem::load(const QMimeData * mimeData)
{
	if ( mimeData->hasFormat(MIME_CODE) )
		setCode( QString( mimeData->data( MIME_CODE ) ) );

	if ( mimeData->hasFormat(MIME_FILE_NAME) )
		setFile( QString( mimeData->data( MIME_FILE_NAME ) ) );
		
	setModified( code() != getFileContent( file() ) );

	if ( mimeData->hasFormat( MIME_SCALE ) )
	{
		float hotSpotX = mimeData->data( MIME_HOTSPOT_X ).toFloat();
		float hotSpotY = mimeData->data( MIME_HOTSPOT_Y ).toFloat();
		moveBy( -hotSpotX , -hotSpotY );
	}

}

//-------------------------------------------------------------------------
//								Public slots							//
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
void QLanguageItem::saveItem()
{
	save( mFile );
}

//-------------------------------------------------------------------------
void QLanguageItem::reloadItem()
{	
	if ( file().length() == 0 )
	{
		qWarning("QLanguageItem::reloadItem() : file() is not defined");
		return;
	}
	
	QMessageBox::StandardButton result = QMessageBox::Yes;
	if ( mIsModified && !mIsProcessingFileChange )
	{
		result = QMessageBox::question ( 0, "Reload", "Unsaved modifications will be lost. Reload ?",
			QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel );
	}
	if ( result == QMessageBox::Yes )
	{
		setCode( QLanguageItem::mFileContents[ file() ] );
		setModified( false );
		itemHasBeenReloaded();
	}
}


//-------------------------------------------------------------------------
void QLanguageItem::unlinkFile()
{
	setFile("");
}


//-------------------------------------------------------------------------
//								Protected slots							//
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QLanguageItem::codeChanged()
{
	if ( !isModified() )
	{
		setModified( true );
	}
}


//-------------------------------------------------------------------------
void QLanguageItem::removeItemAndFile()
{
	if ( file().length() == 0 )
	{
		qWarning("QLanguageItem::removeItemAndFile() : file() is not defined.");
		return;
	}
	
	QMessageBox::StandardButton result = QMessageBox::question ( 0, "Delete file", "Remove item and file ?",
			QMessageBox::Yes | QMessageBox::Cancel, QMessageBox::Cancel );

	if ( result == QMessageBox::Yes )
	{
		QFile::remove( file() );
		this->deleteLater();
		Q_EMIT removed();
	
		mIgnoreChangeNotification = true;
		QLanguageItem::fileRemoved( file() );
		mIgnoreChangeNotification = false;
	}
}

//-------------------------------------------------------------------------
//							Protected functions							 //
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QLanguageItem::contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) 
{
	if (mIsStorageModeOn)
		return;
	
	if ( scene()->selectedItems().size() > 1 )
	{
		event->ignore();
		return;
	}

	QMenu * m = buildContextMenu();
	if ( m )
	{
		event->accept();
		m->exec( event->screenPos() );
		delete m;
	}
}

//-------------------------------------------------------------------------
QAction * newSeparator(QObject * parent)
{
	QAction * separator = new QAction(parent);
	separator->setSeparator(true);
	return separator;
}

//-------------------------------------------------------------------------
QMenu * QLanguageItem::buildContextMenu()
{
	QMenu * menu = new QMenu();
	
	QList<QAction *> actions;

	QAction * removeFileAct = new QAction( "Delete item && file" , menu );
	removeFileAct->setObjectName( OBJECT_NAME_DELETE_ITEM_AND_FILE_ACT );
	connect( removeFileAct , SIGNAL(triggered()) , this , SLOT(removeItemAndFile()) );
	actions.append(removeFileAct);		
	removeFileAct->setEnabled( !mFile.isEmpty() );
	removeFileAct->setProperty( FILE_MENU , 1 );

	QAction * unlinkAct = new QAction( "Unlink" , menu );
	unlinkAct->setObjectName( OBJECT_NAME_UNLINK_ACT );
	connect( unlinkAct , SIGNAL(triggered()) , this , SLOT(unlinkFile()) );
	actions.append(unlinkAct);
	unlinkAct->setEnabled( !mFile.isEmpty() );
	unlinkAct->setProperty( FILE_MENU , 1 );

	QAction * reloadAct = new QAction( "Reload" , menu );
	reloadAct->setObjectName( OBJECT_NAME_RELOAD_ACT );
	connect( reloadAct , SIGNAL(triggered()) , this , SLOT(reloadItem()) );
	actions.append(reloadAct);
	reloadAct->setEnabled( !mFile.isEmpty() );
	reloadAct->setProperty( FILE_MENU , 1 );

	QAction * saveItemAsAct = new QAction( "Save as..." , menu );
	saveItemAsAct->setObjectName( OBJECT_NAME_SAVE_AS_ACT );
	connect( saveItemAsAct , SIGNAL(triggered()) , this , SIGNAL(saveItemAs()) );
	actions.append(saveItemAsAct);
	saveItemAsAct->setProperty( FILE_MENU , 1 );

	QAction * saveItemAct = new QAction( "Save" , menu );
	saveItemAct->setObjectName( OBJECT_NAME_SAVE_ACT );
	connect( saveItemAct , SIGNAL(triggered()) , this , SLOT(saveItem()) );
	actions.append(saveItemAct);
	saveItemAct->setEnabled( !file().isEmpty() && mIsModified );
	saveItemAct->setProperty( FILE_MENU , 1 );

	for ( int i = 0 ; i < mContainerActionList.size() ; i++ )
		actions.append( mContainerActionList[i]->buildAction() );
	
	QMenu * fileMenu = menu->addMenu( "File" );
	fileMenu->setObjectName( OBJECT_NAME_FILE_MENU );

	for ( int i = (int)actions.size()-1 ; i >= 0 ; i-- )
	{
		if ( actions[i]->property( FILE_MENU ).toBool() )
		{
			fileMenu->addAction( actions[i] );
			actions.removeAt(i);
		}
	}
	
	QMenu * zMenu = menu->addMenu("Z order");
	zMenu->setObjectName( OBJECT_NAME_Z_MENU );
	
	QAction * bringToFrontAct = zMenu->addAction( "Bring to front" );
	connect( bringToFrontAct , SIGNAL(triggered()) , this , SIGNAL(bringToFront()) );
	QAction * bringForwardAct = zMenu->addAction( "Bring forward" );
	connect( bringForwardAct , SIGNAL(triggered()) , this , SIGNAL(bringForward()) );
	QAction * sendBackwardAct = zMenu->addAction( "Send backward" );
	connect( sendBackwardAct , SIGNAL(triggered()) , this , SIGNAL(sendBackward()) );
	QAction * sendToBackAct = zMenu->addAction( "Send to back" );
	connect( sendToBackAct , SIGNAL(triggered()) , this , SIGNAL(sendToBack()) );

	foreach (QAction *a, actions)
	{
		menu->addAction(a);
	}
	
	return menu;
}

//-------------------------------------------------------------------------
QVariant QLanguageItem::itemChange( GraphicsItemChange change, const QVariant& value )
{

	QVariant standardResult = QGraphicsRectItem::itemChange( change , value );

	if (mIsStorageModeOn)
	{
		return standardResult;
	}

	if ( change == QGraphicsItem::ItemSelectedHasChanged )
	{
		Q_EMIT showResizer( value.toBool() );
	}
	return standardResult;
}

//-------------------------------------------------------------------------
void QLanguageItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	mMousePressPoint = event->scenePos();
	
	mCopyDrag = ( event->modifiers() & Qt::AltModifier );
	
	QGraphicsItem::mousePressEvent( event );
	
	event->accept();
}


//-------------------------------------------------------------------------
void QLanguageItem::updateDragImage()
{
	delete mDragImage;
	mDragImage = buildDragImage();

	mHasToUpdateDragImage = false;
}

//-------------------------------------------------------------------------
QDrag *	QLanguageItem::buildDrag(QWidget * widget)
{
	QImage image;

	if ( mHasToUpdateDragImage )
		updateDragImage();
		
	image = mDragImage->scaled( mDragImage->size() * sceneTransform().m11() * scene()->views()[0]->transform().m11() , Qt::KeepAspectRatio , Qt::SmoothTransformation );

	QMimeData * data = buildMimeData();

	QDrag *drag = new QDrag( widget );
	drag->setMimeData(data);
	drag->setPixmap( QPixmap::fromImage( image ) );
	
	QPointF p = mapFromScene(mMousePressPoint);
	p.setX( p.x() * sceneTransform().m11() * scene()->views()[0]->transform().m11() );
	p.setY( p.y() * sceneTransform().m22() * scene()->views()[0]->transform().m22() );

	QPointF hotSpot = mIsStorageModeOn ? QPointF( image.width()/2 , image.height()/2 ) : (p);
	drag->setHotSpot( hotSpot.toPoint() );

	return drag;
}

//-------------------------------------------------------------------------
void QLanguageItem::updateGeometry( const QRectF& newGeometry )
{
	if ( rect().toRect() != newGeometry.toRect() )
	{
		mHasToUpdateDragImage = true;
		
		setRect(newGeometry);
		Q_EMIT updateChildrenItemsGeometry(newGeometry);
	}
}

//-------------------------------------------------------------------------
void QLanguageItem::fileChangeEvent( bool isFileRemoved )
{
	if ( mIsStorageModeOn )
	{
		return;
	}

	if ( mIgnoreChangeNotification )
		return;

	if ( !isFileRemoved )
	{
		setModified( true );
		mIsProcessingFileChange = true;
	}

	Q_EMIT fileChanged(isFileRemoved);
	
	mIsProcessingFileChange = false;
}

//-------------------------------------------------------------------------
void QLanguageItem::setModified( bool isModified )
{
	mIsModified = isModified;
	Q_EMIT descriptiveNameChanged();
}

/*
//-------------------------------------------------------------------------
void QLanguageItem::paletteActivated( int elementId , QGraphicsItem *  , QGraphicsItem * droppedItem )
{
	QLanguageItem * droppedDecorator = dynamic_cast<QLanguageItem *>(droppedItem);
	assert( droppedDecorator );
	Q_EMIT combineItems(elementId, this , droppedDecorator );
}
*/

#define DRAG_THRESHOLD_DISTANCE 10
//-------------------------------------------------------------------------
void QLanguageItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mouseMoveEvent( event );

	if ( ( mMousePressPoint - event->scenePos() ).toPoint().manhattanLength() > DRAG_THRESHOLD_DISTANCE )
	{
		//Select this item
		setSelected( true );
/*
		//Unselect all other items
		int index = 0;
		while ( scene()->selectedItems().size() > 1 )
		{
			QGraphicsItem * item = scene()->selectedItems()[index];
			if ( item == this )
				index = 1;
			else
				item->setSelected( false );
		}
*/
		//Emit dragStarted for QPaletteItem
		int paletteId = -1;
		QGraphicsItem * item = 0;
		Q_EMIT dragStarted( paletteId , item );
		//Build QDrag & start the drag.
		QDrag * d = buildDrag( event->widget() );
		d->mimeData()->setData( MIME_PALETTE_ID ,	QByteArray::number(paletteId) );
		d->mimeData()->setData( MIME_PALETTE_ITEM , QByteArray::fromRawData( reinterpret_cast<char*>(item) , 4 ) );
		d->exec();
	}
}

//-------------------------------------------------------------------------
void QLanguageItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	QGraphicsItem::mouseReleaseEvent( event );
}

//-------------------------------------------------------------------------
//								Static members							///
//-------------------------------------------------------------------------
QMultiHash<QString,QLanguageItem*> QLanguageItem::mUsedFiles = QMultiHash<QString,QLanguageItem*>();

QHash<QString,QString> QLanguageItem::mFileContents = QHash<QString,QString>();

//-------------------------------------------------------------------------
void QLanguageItem::checkAllFiles()
{
	QList<QString> usedFiles = mUsedFiles.uniqueKeys();
	
	for ( int i = 0 ; i < usedFiles.size() ; i++ )
	{
		bool fileHasBeenRemoved = ( !QFile::exists( usedFiles[i] ) );
		QString newContent = getFileContent( usedFiles[i] );
		if ( (fileHasBeenRemoved) || ( mFileContents[ usedFiles[i] ] != newContent ) )
		{
			QList<QLanguageItem*> values = mUsedFiles.values( usedFiles[i] );
			
			if ( fileHasBeenRemoved )
			{
				mUsedFiles.remove( usedFiles[i] );
				mFileContents.remove( usedFiles[i] );
			}
			else
			{
				mFileContents[ usedFiles[i] ] = newContent;
			}
			
			for ( int j = 0 ; j < values.size() ; j++ )
			{				
				values[j]->fileChangeEvent(fileHasBeenRemoved);
			}
		}
	}
}

//-------------------------------------------------------------------------
void QLanguageItem::fileChanged( const QString& fileName , const QString& newContent )
{
	mFileContents[ fileName ] = newContent;
	
	QList<QLanguageItem*> values = mUsedFiles.values( fileName );
	for ( int j = 0 ; j < values.size() ; j++ )
	{
		values[j]->fileChangeEvent(false);
	}
}

//-------------------------------------------------------------------------
void QLanguageItem::fileRemoved( const QString& fileName )
{
	QList<QLanguageItem*> values = mUsedFiles.values( fileName );
	for ( int j = 0 ; j < values.size() ; j++ )
	{
		values[j]->fileChangeEvent(true);
	}
}

//------------------------------------------------------------------------
//							QLanguageItemAction	class					//
//------------------------------------------------------------------------

//------------------------------------------------------------------------
QLanguageItemAction::QLanguageItemAction( QLanguageItem * container , const QString& actionLabel , const QVariant& actionData )
	:	QObject(container),
		mContainer(container),
		mActionLabel(actionLabel),
		mActionData( actionData )
{
	mContainer->addContainerAction(this);
}

//-------------------------------------------------------------------------
QAction * QLanguageItemAction::buildAction() const
{
	QAction * result = new QAction( mActionLabel , mContainer );
	result->setData( mActionData );
	connect(result , SIGNAL(triggered()) , this , SLOT(triggered()) );
	return result;
}

//-------------------------------------------------------------------------
void QLanguageItemAction::triggered()
{
	Q_EMIT triggered( mContainer );
}

//-------------------------------------------------------------------------
//								Other functions							///
//-------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
void paintOnDevice( QPaintDevice * device , QGraphicsItem * item , float scaleFactor , const QRect& painterWindow )
{
	QRectF rect(0,0,device->width() , device->height() );
	QPainter painter;
	painter.begin( device );
	if ( painterWindow.isValid() )
		painter.setWindow( painterWindow );
	QStyleOptionGraphicsItem option;
	option.exposedRect = rect;
	painter.scale( scaleFactor , scaleFactor );
	item->paint( &painter , &option , 0 );
	painter.end();
};


//------------------------------------------------------------------------------------------------------------------------
QImage itemToImage( QGraphicsItem * item , float scaleFactor , unsigned int backgroundColor , bool setPainterWindow )
{
	QRectF picRect = item->boundingRect();
	QImage pic( (int)(picRect.width() * scaleFactor) , (int)(picRect.height() * scaleFactor) , QImage::Format_ARGB32_Premultiplied );
	pic.fill( backgroundColor );
	QRect painterWindow;
	if ( setPainterWindow )
		painterWindow = picRect.toRect();
	paintOnDevice( &pic , item , scaleFactor , painterWindow );
	return pic;
};
