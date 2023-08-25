/*
 * GraphicsSceneMainWindow.cpp
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

#include <assert.h>
#include <math.h>

#include <QAbstractButton>
#include <QApplication>
#include <QDockWidget>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QFileDialog>
#include <QGraphicsRectItem>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollBar>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QtGlobal>
#include <QtGui>

#include "GraphicsSceneMainWindow.h"
#include "MainWindowObjects.h"
#include "QItemResizer.h"
#include "QLanguageCommandPalette.h"
#include "QLanguageItem.h"
#include "QLanguageItemFactory.h"
#include "QResolutionDialog.h"


#define LANGUAGE_NAME_SHORT		GraphicsSceneMainWindow::applicationSettings().mLanguageNameShort
#define LANGUAGE_NAME_LONG		GraphicsSceneMainWindow::applicationSettings().mLanguageNameLong
#define LANGUAGE_FILE_EXTENSION	GraphicsSceneMainWindow::applicationSettings().mLanguageFileExtension
#define SCENE_FILE_EXTENSION	GraphicsSceneMainWindow::applicationSettings().mSceneFileExtension
#define DEFAULT_LANGUAGE_CODE	GraphicsSceneMainWindow::applicationSettings().mDefaultLanguageCode
#define	LANGUAGE_COMMANDS_FILE	GraphicsSceneMainWindow::applicationSettings().mLanguageCommandsFile

#define	COMMAND_PALETTE_DOCK_NAME	LANGUAGE_NAME_SHORT + " Helper"
#define	GMN_EDIT_DOCK_NAME			LANGUAGE_NAME_SHORT + " Code"

#define GSC_FILE_EXTENSION	SCENE_FILE_EXTENSION
#define GMN_FILE_EXTENSION	LANGUAGE_FILE_EXTENSION

#define GSC_FILE_FILTER		QString( LANGUAGE_NAME_SHORT + QString(" Scene (*."+ GSC_FILE_EXTENSION + ")") )
#define GMN_FILE_FILTER		QString( LANGUAGE_NAME_LONG + QString(" (*."	+ GMN_FILE_EXTENSION + ")") )

#define DEFAULT_WINDOW_WIDTH  1000
#define DEFAULT_WINDOW_HEIGHT 700
#define DEFAULT_WINDOW_X	200
#define DEFAULT_WINDOW_Y	200
#define DEFAULT_WINDOW_POS QPoint( DEFAULT_WINDOW_X , DEFAULT_WINDOW_Y )
#define SLIDER_PRECISION_FACTOR 100.0f
#define SLIDER_TOOL_BAR_MARGIN 50

#define CACHE_MEMORY_SIZE	100000	
#define	MAX_COORDINATE_EDIT	99999
#define PASTE_ITEM_DISTANCE 5
#define START_SCENE_RECT_SIZE_X 400
#define START_SCENE_RECT_SIZE_Y 400

#define START_SCENE_RECT QRectF(														\
							-START_SCENE_RECT_SIZE_X/2,-START_SCENE_RECT_SIZE_Y/2,		\
							START_SCENE_RECT_SIZE_X,START_SCENE_RECT_SIZE_Y				\
								)

#define MIN_PAGE_SIZE 20

//#define ITEM_SIZE_BOUNDS	QSize( 400,300 )

#define TEXT_EDIT_TEMPO 500

#define COMBINATOR_PIXMAP_MARGIN 10
#define COMBINATOR_HIGHLIGHT_PEN_COLOR Qt::red
#define COMBINATOR_HIGHLIGHT_BRUSH_COLOR QColor(255,0,0,50)
#define COMBINATOR_HIGHLIGHT_PEN QPen( COMBINATOR_HIGHLIGHT_PEN_COLOR )
#define COMBINATOR_HIGHLIGHT_BRUSH QBrush( COMBINATOR_HIGHLIGHT_BRUSH_COLOR )

//DOM Document defines
#define DOM_DOC				"GraphicsScene"
#define DOM_ROOT			"SceneRoot"
#define DOM_SCENE			"Scene"
#define DOM_VIEW_X			"viewX"
#define DOM_VIEW_Y			"viewY"
#define DOM_VIEW_ZOOM		"viewZoom"
#define DOM_SCENE_X			"sceneX"
#define DOM_SCENE_Y			"sceneY"
#define DOM_SCENE_WIDTH		"sceneWidth"
#define DOM_SCENE_HEIGHT	"sceneHeight"
#define DOM_SPLITTER_SIZE_0	"Splitter0"
#define DOM_SPLITTER_SIZE_1	"Splitter1"

#define INIT_DONE_SETTING			"InitDone"
#define WINDOW_SIZE_SETTING			"WindowSize"
#define WINDOW_POS_SETTING			"WindowPos"
#define WINDOW_STATE_SETTING		"WindowState"
#define WINDOW_ZOOM_SETTING			"WindowZoom"
#define FILE_DIALOG_PATH_SETTING	"FileDialogPathSetting"
#define STORAGE_VISIBLE_SETTING		"StorageVisibleSetting"
#define HISTORY_VISIBLE_SETTING		"HistoryVisibleSetting"
#define CURRENT_SCENE_SETTING		"CurrentSceneSetting"

enum FileFilters
{
	PNG_FILTER = 0,
	PDF_FILTER = 1,
	PS_FILTER = 2,
	GSC_FILTER = 3,
	GMN_FILTER = 4
};

GraphicsSceneMainWindowSettings GraphicsSceneMainWindow::mSettings = GraphicsSceneMainWindowSettings();

//-------------------------------------------------------------------------
GraphicsSceneMainWindow::GraphicsSceneMainWindow( LanguageGraphicsView * graphicsView , QLanguageItemFactory * factory )
	:	mGraphicsView( graphicsView ) ,
		mFactory( factory )
{
	connect( qApp, SIGNAL( focusChanged ( QWidget *, QWidget *  ) ) , this, SLOT( focusChanged(QWidget*,QWidget*) ) );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::init()
{	
//	Create History & Storage GraphicsView/GraphicsScene
	createHistoryAndStorage();

//	Main GraphicsScene/GraphicsView init
	((LanguageGraphicsView *)mGraphicsView)->setMainWindow( this );
	mGraphicsScene = 0;
	mBackgroundItem = 0;
	reinitScene( START_SCENE_RECT );
	mGraphicsView->setBackgroundRole(QPalette::Dark);
//	mGraphicsView->setResizeAnchor(QGraphicsView::NoAnchor);
//	mGraphicsView->setAlignment( 0 );

//	Guido code edit widget
	setupTextEdit();

//	Layout the History widget, the main GraphicsView widget and the Storage widget
	QWidget *window = new QWidget();
	
    QHBoxLayout *layout = new QHBoxLayout(window);

	if ( mSettings.mHasHistory )
		layout->addWidget(mHistoryGraphicsView);

	mSplitter = new QSplitter(this);
	mSplitter->addWidget(mGraphicsView);
	layout->addWidget( mSplitter );

	if ( mSettings.mHasStorage )
		mSplitter->addWidget( mStorageGraphicsView );
		
//	Main Scene is the central Widget
	setCentralWidget( window );

//	Create actions, menus, toolbars
    createActions();
	createToolBars();
    createMenus();
    createStatusBar();
	
//	App init
	setDockOptions ( AnimatedDocks );
	QPixmapCache::setCacheLimit ( CACHE_MEMORY_SIZE );
	setAcceptDrops(true);
	mSceneHasChanged = false;
//	mCurrentZOrderItem = 0;

	updateWindowState();

	mStdMainWindowState = saveState();
	checkSettings();
}

//-------------------------------------------------------------------------
GraphicsSceneMainWindow::~GraphicsSceneMainWindow()
{
	delete mFactory;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::reinitSettings()
{	 
	QSettings settings;

	settings.setValue( INIT_DONE_SETTING , true );

	settings.setValue( FONT_SIZE_SETTING , 12 );

	settings.setValue( WINDOW_POS_SETTING , DEFAULT_WINDOW_POS );
	settings.setValue( WINDOW_SIZE_SETTING, QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) );

	settings.setValue( FILE_DIALOG_PATH_SETTING , QDir::homePath() );

//	settings.setValue( WORKSPACE_MODE_SETTING , false );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::writeSettings()
{
	QSettings settings;
	settings.setValue( WINDOW_POS_SETTING , pos() );
	settings.setValue( WINDOW_SIZE_SETTING, size() );
	settings.setValue( WINDOW_STATE_SETTING, saveState() );
	settings.setValue( WINDOW_ZOOM_SETTING , mGraphicsView->transform().m11() );
	settings.setValue( FILE_DIALOG_PATH_SETTING , mLastOpenFilePath );
//	settings.setValue( WORKSPACE_MODE_SETTING , mIsWorkspaceModeOn );
	if ( mSettings.mHasStorage )
	{
		settings.setValue( STORAGE_VISIBLE_SETTING , mStorageGraphicsView->isVisible() );
	}
	if ( mSettings.mHasHistory )
	{
		settings.setValue( HISTORY_VISIBLE_SETTING , mHistoryGraphicsView->isVisible() );
	}
}

//-------------------------------------------------------------------------
const GraphicsSceneMainWindowSettings& GraphicsSceneMainWindow::applicationSettings()
{
	return GraphicsSceneMainWindow::mSettings;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::initApplicationSettings( const GraphicsSceneMainWindowSettings& settings )
{
	GraphicsSceneMainWindow::mSettings = settings;
}
	
//-----------------------------------------------------------------------//	
//						Private slots									 //
//-----------------------------------------------------------------------//

//-------------------------------------------------------------------------
QPointF GraphicsSceneMainWindow::viewCenterInSceneCoordinate()
{
	QPoint viewPortCenter( mGraphicsView->width() / 2 , mGraphicsView->height() / 2 );
	QPointF sceneCenterPoint = mGraphicsView->mapToScene( viewPortCenter );
	return sceneCenterPoint;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::addItem()
{
	QLanguageItem* createdItem = 0;
	addCode( DEFAULT_LANGUAGE_CODE , &createdItem );
	setupItemFromMenu(createdItem);
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::addItemFromFile()
{
	//Select a file
	QString fileName = QFileDialog::getOpenFileName(
		this, QString("Add a ") + LANGUAGE_NAME_LONG + QString(" file"),
        getFileDialogPath(),
        tr( QString( GMN_FILE_FILTER  + QString("\nAll (*.*)") ).toUtf8().data() ));

	// Create the item with the file
	QLanguageItem* createdItem = 0;
	addFile( fileName , &createdItem );
	// Add & setup the created item
	if ( createdItem )
		setupItemFromMenu( createdItem );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::removeSelectedItems()
{
	//StorageGraphicsView gets the removeAction if:
	//	- it has focus
	//	- AND it has selected items
	if ( ( mSettings.mHasStorage ) && ( mStorageGraphicsView->hasFocus() ) && ( mStorageGraphicsScene->selectedItems().size() > 0 ) )
	{
		while ( mStorageGraphicsScene->selectedItems().size() > 0 )
		{
			QGraphicsItem * item = mStorageGraphicsScene->selectedItems()[0];
			mStorageGraphicsView->removeItem( item );
			mSceneHasChanged = true;
		}
		updateRemoveActState();
	}
	else
	{
		// Delete all the items that are in the mSelectedItems list.
		QList<QLanguageItem*> selectedItems = selectedLanguageItems();
		while ( selectedItems.size() > 0 )
		{
			if ( !removeItem( selectedItems[0] ) )
			{
				break;
			}
			selectedItems.removeAt( 0 );
			mSceneHasChanged = true;
		}
		updateWindowState();
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::copyItem()
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	
	mCopiedItems.clear();
	for ( int i = 0 ; i < selectedItems.size() ; i++ )
	{
		mCopiedItems.append( selectedItems[i] );
	}
	//Paste action is now available
	mPasteAct->setEnabled( true );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::pasteItem()
{
	// Deselect all the currently selected item,
	// in order to select only the newly pasted items.
	unselectAll();
	// Create and add to selection all the item in mCopiedItems.
	for ( int i = 0 ; i < mCopiedItems.size() ; i++ )
	{
		QLanguageItem * item = addItemCopy( mCopiedItems[i] );
		item->moveBy( PASTE_ITEM_DISTANCE , PASTE_ITEM_DISTANCE );
		item->setSelected( true );
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::selectAll()
{
	// Ignore the QGraphicsScene::selectionChanged() signal during the operation,
	// so that it can be done just once, at the end of the operation.
	disconnect(mGraphicsScene,SIGNAL(selectionChanged ()),this,SLOT(sceneSelectionChanged()));

	QList<QLanguageItem*> items = languageItems();
	for ( int i = 0 ; i < items.size() ; i++ )
	{
		items[i]->setSelected(true);
	}	
	
	connect(mGraphicsScene,SIGNAL(selectionChanged ()),this,SLOT(sceneSelectionChanged()));
	sceneSelectionChanged();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::cleanHistory()
{
	if ( mSettings.mHasHistory )
		mHistoryGraphicsView->clear();
}

//-------------------------------------------------------------------------
QString getZoomLabel(float zoom)
{
	QString label;
	label.asprintf("Zoom: x%0.2f" , zoom );
	return label;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::zoomIn()
{
	zoom( mSettings.mZoomFactor );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::zoomOut()
{
	zoom( 1.0f / mSettings.mZoomFactor );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::zoomStandard()
{
	zoom( 1.0f / mGraphicsView->transform().m11() );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::zoom( float zoom )
{
	if ( mGraphicsView->transform().m11() * zoom < mSettings.mMinZoom )
	{
		zoom = mSettings.mMinZoom / mGraphicsView->transform().m11();
	}
	else if ( mGraphicsView->transform().m11() * zoom > mSettings.mMaxZoom )
	{
		zoom = mSettings.mMaxZoom / mGraphicsView->transform().m11();
	}
	
	mGraphicsView->scale( zoom , zoom );
	mZoomOutAct->setEnabled( mGraphicsView->transform().m11() > ( mSettings.mMinZoom + 0.00001f ) );
	mZoomInAct->setEnabled( mGraphicsView->transform().m11() < mSettings.mMaxZoom - 0.00001f );

	updateZoomLabel();
}

//-------------------------------------------------------------------------
QString GraphicsSceneMainWindow::getFileDialogPath()
{
	if ( mLastOpenFilePath == "" )
		mLastOpenFilePath = windowFilePath();
	if ( mLastOpenFilePath == "" )
		mLastOpenFilePath = QDir::homePath();
	
	return mLastOpenFilePath;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::setLastManagedFile(const QString& lastFile)
{
	mLastOpenFilePath = QFileInfo( lastFile ).absolutePath();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::fontBigger()
{	
	changeFontSize( MIN( mLanguageTextEdit->fontPointSize() + 1 , MAX_FONT_SIZE ) );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::fontSmaller()
{
	changeFontSize( MAX( mLanguageTextEdit->fontPointSize() - 1 , MIN_FONT_SIZE ) );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::reloadTextEdits()
{
	int vValue = mLanguageTextEdit->verticalScrollBar()->value();
	int hValue = mLanguageTextEdit->horizontalScrollBar()->value();
	mLanguageTextEdit->setText( mLanguageTextEdit->toPlainText() );
	mLanguageTextEdit->verticalScrollBar()->setValue( vValue );
	mLanguageTextEdit->horizontalScrollBar()->setValue( hValue );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::changeFontSize( float newFontPointSize )
{
	mLanguageTextEdit->setFontPointSize( newFontPointSize );
	QSettings().setValue( FONT_SIZE_SETTING , mLanguageTextEdit->fontPointSize() );
	reloadTextEdits();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::sceneSelectionChanged()
{
	QString statusMsg;
	QTextStream(&statusMsg) << selectedLanguageItems().size() << " item(s) selected";
	statusBar()->showMessage( statusMsg );
	updateWindowState();
}
//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::sceneChanged( const QList<QRectF> &)
{
	updateItemPositionControls();
//	autoScrollOnSelectedItems(mGraphicsView);

	int totalMargin = PAGE_ITEM_IN_MARGIN + PAGE_ITEM_OUT_MARGIN;

	QRectF backgroundRect = mBackgroundItem->boundingRect();

	QList<QGraphicsItem*> items = mGraphicsScene->items();
	for ( int i = 0; i < items.size() ; i++ )
	{
		QLanguageItem * languageItem = dynamic_cast<QLanguageItem *>(items[i]);
		if ( languageItem) 
		{
			QRectF decoratorBoundingRect = languageItem->mapToScene( languageItem->boundingRect() ).boundingRect();
			if (		( !mBackgroundItem->collidesWithItem( languageItem , Qt::ContainsItemBoundingRect ) ) 
					||	( backgroundRect.width() < decoratorBoundingRect.width() + totalMargin ) 
					||	( backgroundRect.height() < decoratorBoundingRect.height() + totalMargin ) )
			{
				QRectF rect1 = decoratorBoundingRect;
				QRectF rect2 = decoratorBoundingRect;
				rect1.translate( totalMargin , totalMargin );
				rect2.translate(-totalMargin ,-totalMargin );
				QRectF totalRect = mBackgroundItem->sceneBoundingRect() | rect1 | rect2;
				
				mBackgroundItem->setRect( totalRect );
			}
		}
	}

	mGraphicsScene->setSceneRect( mBackgroundItem->sceneBoundingRect() );
}


//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::textEditActivity()
{	
	mLanguageTextEditTimer->start( TEXT_EDIT_TEMPO );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateCode()
{	
	mLanguageTextEditTimer->stop();

	if ( mLanguageTextEdit->isEnabled() )
	{
		QString newGMNCode = mLanguageTextEdit->toPlainText();
		if ( newGMNCode == mFirstSelectedItem->code() )
		{
			return;
		}
			
		QApplication::setOverrideCursor(Qt::WaitCursor);

		mSceneHasChanged = true;

		if ( mFirstSelectedItem->setCode( newGMNCode ) )
		{
			statusBar()->showMessage( LANGUAGE_NAME_SHORT + " Code ok" );
			addToHistory( mFirstSelectedItem );
		}
		else
		{
			statusBar()->showMessage( mFirstSelectedItem->lastErrorMessage() );
		}

		mLanguageTextEditDock->setWindowTitle( (mFirstSelectedItem->file().length() > 0 ) ? mFirstSelectedItem->getDescriptiveFileName() : GMN_EDIT_DOCK_NAME );
			
		QApplication::restoreOverrideCursor();
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::resizeSliderChanged(int value)
{
	float scale = pow(10,value / SLIDER_PRECISION_FACTOR );
	scaleItems( scale );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::scaleItems(double scale)
{
	scaleItems( 0,0, scale );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::scaleItems( double , double , double sizeScale )
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	for ( int i = 0 ; i < selectedItems.size() ; i++ )
	{
		QRectF r = selectedItems[i]->rect();
		r.setWidth( r.width() * sizeScale/selectedItems[i]->currentScale() );
		r.setHeight( r.height() * sizeScale/selectedItems[i]->currentScale() );

		selectedItems[i]->resized( r );
	}
	updateScaleControls(sizeScale);
	
	mSceneHasChanged = true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::scaleChanged(float newScale)
{
	updateScaleControls(newScale);
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::moveItemX(double x)
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	if ( selectedItems.size() == 1 )
	{
		mFirstSelectedItem->setPos( x , mFirstSelectedItem->pos().y() );
	}
	
	mSceneHasChanged = true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::moveItemY(double y)
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	if ( selectedItems.size() == 1 )
	{
		mFirstSelectedItem->setPos( mFirstSelectedItem->pos().x() , y );
	}
	
	mSceneHasChanged = true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::resizeItems()
{
	//Sets the scale of all selected items to the size of the first selected
	scaleItems( mFirstSelectedItem->currentScale() );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::hAlignItems()
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	
	//Sets the scale of all selected items to the size of the first selected
	float x = mFirstSelectedItem->pos().x();
	for ( int i = 0 ; i < selectedItems.size() ; i++ )
	{
		float y = selectedItems[i]->pos().y();
		selectedItems[i]->setPos(x,y);
	}
	
	mSceneHasChanged = true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::vAlignItems()
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	
	//Sets the scale of all selected items to the size of the first selected
	float y = mFirstSelectedItem->pos().y();
	for ( int i = 0 ; i < selectedItems.size() ; i++ )
	{
		float x = selectedItems[i]->pos().x();
		selectedItems[i]->setPos(x,y);
	}
	
	mSceneHasChanged = true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::newScene()
{
	//Save if necessary
	//saveOrCancelDialog == false  <=> cancel
	if ( !saveOrCancelDialog() )
		return;
	
	//Clear the scene
	reinitScene( START_SCENE_RECT );
	
	mGraphicsView->centerOn( 0 , 0 );
	mGraphicsView->setTransform( QTransform() );
	
	//Update states
	setWindowFilePath("");
	updateWindowState();
	updateZoomLabel();
	mSceneHasChanged = false;
}


// - If one item is selected (generally : because the user is editing its code)
//		saves this item.
// - Else, saves the scene.
//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::save()
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	if ( selectedItems.size() == 1 )
	{
		if ( selectedItems[0]->file().length() )
			selectedItems[0]->save( selectedItems[0]->file() );
		else
			saveItemAs( selectedItems[0] );
	}
	else
		saveScene();
}

//-------------------------------------------------------------------------
QString getFileFilterExtension( const QString& filter )
{
	if ( filter == GSC_FILE_FILTER )
		return GSC_FILE_EXTENSION;
	else if ( filter == PNG_FILE_FILTER )
		return PNG_FILE_EXTENSION;
	else if ( filter == PDF_FILE_FILTER )
		return PDF_FILE_EXTENSION;
	else if ( filter == PS_FILE_FILTER )
		return PS_FILE_EXTENSION;
	else if ( filter == GMN_FILE_FILTER )
		return GMN_FILE_EXTENSION;
		
	return "";
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::formatFileName(QString& fileName , QString& selectedFilter , const QString& filters)
{
	QStringList filterList = filters.split("\n");

	bool hasAFilter = false;
	QString fileNameFilter , fileNameExtension;
	for ( int i = 0 ; i < filterList.size() ; i++ )
	{
		QString extension = getFileFilterExtension( filterList[i] );
		if ( fileName.toUpper().endsWith( "." + extension.toUpper() ) )
		{
			hasAFilter = true;
			fileNameFilter = filterList[i];
			fileNameExtension = extension;
			break;
		}
	}

//	If the fileName already has an extension
	if ( hasAFilter )
	{
//		If the extension is the same as the selectedFilter, ok.
		if ( fileNameFilter.toUpper() == selectedFilter.toUpper() )
			return;
		else
//		If not, change the selectedFilter to match the fileName extension
			selectedFilter = fileNameFilter;
	}
//	If the fileName doesn't have an extension
//		- put the extension of the selectedFilter
	else
	{
		fileName += "." + getFileFilterExtension(selectedFilter);
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::saveSceneAs()
{
	QString savePath = windowFilePath();
	if ( savePath == "" )
		savePath = QDir::homePath() + "/untitled_scene";

	QString filters =						GSC_FILE_FILTER;

	QString selectedFilter("");
	QString fileName = QFileDialog::getSaveFileName(this, "Save the " + LANGUAGE_NAME_SHORT + " scene",
                            savePath,
                            tr(filters.toUtf8().data()) ,
							&selectedFilter);

	if ( fileName.isEmpty() )
		return;

	formatFileName(fileName,selectedFilter,filters);

	saveSceneFile(fileName);
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::saveAll()
{
	QList<QLanguageItem*> items = languageItems();
	for ( int i = 0 ; i < items.size() ; i++ )
	{
		if ( !items[i]->file().isEmpty() )
		{
			items[i]->saveItem();
		}
	}

	saveScene();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::loadScene()
{
	//Save if necessary
	//saveOrCancelDialog == false  <=> cancel loadScene
	if ( !saveOrCancelDialog() )
		return;

	//Open a file
	QString openPath = windowFilePath();
	if ( openPath == "" )
		openPath = QDir::homePath();
	QString fileName = QFileDialog::getOpenFileName(this, "Open a " + LANGUAGE_NAME_SHORT + " scene",
                                                     openPath,
                                                     tr( GSC_FILE_FILTER.toUtf8().data() ));
	
	if ( !fileName.isEmpty() )
	if ( !loadSceneFile(fileName) )
	{
		QMessageBox::critical( 0 , "Load scene error" , fileName + " : Invalid GuidoScene file" );
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::rescaleItem(float xScale , float yScale , float sizeScale)
{
	scaleItems(xScale , yScale , sizeScale);
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::resizePage( const QRectF& newRect )
{
	QRectF pageRect( newRect );

	if ( pageRect.width() < MIN_PAGE_SIZE )
		pageRect.setWidth( MIN_PAGE_SIZE );
	if ( pageRect.height() < MIN_PAGE_SIZE )
		pageRect.setHeight( MIN_PAGE_SIZE );


	mBackgroundItem->setPageSize( pageRect.size() );
	mBackgroundItem->moveBy( pageRect.topLeft().x() , pageRect.topLeft().y() );

	mGraphicsScene->setSceneRect( mBackgroundItem->sceneBoundingRect() );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::storeItem(QLanguageItem * container)
{
	if ( mSettings.mHasStorage )
	{	
		QLanguageItem * other = mStorageGraphicsView->find( container );
		if ( !other )
		{
			mStorageGraphicsView->addItem( mFactory->buildLanguageItem( container ) );
		}
		else
		{
			other->setSelected(true);
		}
		container->deleteLater();
		itemRemoved( container );
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::itemRemoved()
{
	QLanguageItem * container = dynamic_cast<QLanguageItem *>( sender() );
	assert(container);
	itemRemoved( container );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::itemRemoved( QLanguageItem * languageItem )
{
	mGraphicsScene->removeItem( languageItem );
	
	//Remove the item from all other references
	if ( mFirstSelectedItem == languageItem )
		mFirstSelectedItem = 0;

	mCopiedItems.removeAll( languageItem );
	mZOrderedItems.removeAll( languageItem );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::insertText( const QString& gmnCode , int insertIndex )
{
	if ( !mLanguageTextEdit->isEnabled() )
		return;

	QTextCursor cursor = mLanguageTextEdit->textCursor();

	QString selectedText = cursor.selectedText();
	QString leftPart , rightPart;
	if ( insertIndex >= 0 ) 
	{	
		leftPart = gmnCode.left( insertIndex );
		rightPart = gmnCode.right( gmnCode.length() - insertIndex );
	}
	else
	{
		leftPart = gmnCode.left(	gmnCode.length() + insertIndex );
		rightPart = gmnCode.right(	-insertIndex );
	}
	cursor.insertText( leftPart + selectedText + rightPart );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::switchCommandPaletteVisible()
{
	mCommandPaletteDock->setVisible( mSwitchCommandPaletteVisible->isChecked() );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::switchTextEditVisible()
{
	mLanguageTextEditDock->setVisible( mSwitchTextEditVisible->isChecked() );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateWindowMenu()
{
	if ( mSettings.mHasHistory )
		mSwitchHistoryVisible->setChecked( mHistoryGraphicsView->isVisible() );
	if ( mSettings.mHasStorage )
		mSwitchStorageVisible->setChecked( mStorageGraphicsView->isVisible() );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::switchStorageVisible()
{
	if ( mSettings.mHasStorage )
		mStorageGraphicsView->setVisible( !mStorageGraphicsView->isVisible() );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::switchHistoryVisible()
{
	if ( mSettings.mHasHistory )
		mHistoryGraphicsView->setVisible( !mHistoryGraphicsView->isVisible() );
}

//-------------------------------------------------------------------------
QString GraphicsSceneMainWindow::findFile(const QString& file)
{
	// demander de specifier l'endroit du fichier.
	QString filter = QString( file  + QString("\nAll (*.*)") );
	QString fileName = QFileDialog::getOpenFileName ( this, "Find file", file, filter );
	return fileName;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::saveItemAs()
{
	QLanguageItem * languageItem = (QLanguageItem *)(sender());
	saveItemAs( languageItem );
}

//-------------------------------------------------------------------------
QLanguageItem* GraphicsSceneMainWindow::addItemCopy( QLanguageItem* item )
{
	QLanguageItem * languageItem = mFactory->buildLanguageItem( item );
	setupNAddItem(languageItem);
	return languageItem;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::fileChanged(bool isFileRemoved)
{
	QLanguageItem * languageItem = (QLanguageItem*)sender();

	if ( isFileRemoved )
	{
		unselectAll();
		languageItem->setSelected( true );

		QMessageBox msgBox( QMessageBox::Warning, "File not found", "Can't find the " + mSettings.mLanguageNameShort + " item's file " + languageItem->file() );
        QPushButton *findButton = msgBox.addButton(tr("Find file..."), QMessageBox::ActionRole);
		QPushButton *removeButton = msgBox.addButton(tr("Remove item"), QMessageBox::ActionRole);
        msgBox.addButton(QMessageBox::Ignore);
        msgBox.exec();

        if (msgBox.clickedButton() == findButton) 
		{
			QString file = findFile( languageItem->file() );
			if ( QFile::exists( file ) )
				languageItem->loadFile( file );
		}
		else if ( msgBox.clickedButton() == removeButton )
		{
			itemRemoved( languageItem );
			languageItem->deleteLater();
		}
		
	}
	else
	{
		unselectAll();
		languageItem->setSelected( true );
		
		QMessageBox msgBox( QMessageBox::Warning, "File change", "The " + mSettings.mLanguageNameShort + " item's file " + languageItem->file() + " has changed." );
        QPushButton *reloadButton = msgBox.addButton(tr("Reload"), QMessageBox::ActionRole);
        msgBox.addButton(QMessageBox::Ignore);
        msgBox.exec();

        if (msgBox.clickedButton() == reloadButton) 
		{
			languageItem->reloadItem();
			updateWindowState();
		}
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::descriptiveNameChanged()
{
	if ( mFirstSelectedItem )
		mLanguageTextEditDock->setWindowTitle( (mFirstSelectedItem->file().length() > 0 ) ? mFirstSelectedItem->getDescriptiveFileName() : GMN_EDIT_DOCK_NAME );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::itemHasBeenReloaded()
{
	if ( dynamic_cast<QLanguageItem*>(sender()) )
	{
		addToHistory( (QLanguageItem*)sender() );
	}
	updateWindowState();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::focusChanged( QWidget * old, QWidget * )
{
	if ( !old )
	{
		QLanguageItem::checkAllFiles();
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::bringForward()
{
	QLanguageItem* languageItem = dynamic_cast<QLanguageItem*>(sender());
	if ( !languageItem )
		languageItem = mFirstSelectedItem;
	if ( !languageItem )
		return;

	qsizetype index = mZOrderedItems.indexOf( languageItem );
	mZOrderedItems.move( index , MIN(mZOrderedItems.size() - 1 , index + 1) );
	updateZOrders();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::sendBackward()
{
	QLanguageItem* languageItem = dynamic_cast<QLanguageItem*>(sender());
	if ( !languageItem )
		languageItem = mFirstSelectedItem;
	if ( !languageItem )
		return;
	
	qsizetype index = mZOrderedItems.indexOf( languageItem );
	mZOrderedItems.move( index , MAX(0,index - 1) );
	updateZOrders();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::bringToFront()
{
	QLanguageItem* languageItem = dynamic_cast<QLanguageItem*>(sender());
	if ( !languageItem )
		languageItem = mFirstSelectedItem;
	if ( !languageItem )
		return;

	qsizetype index = mZOrderedItems.indexOf( languageItem );
	mZOrderedItems.move( index , mZOrderedItems.size() - 1 );
	updateZOrders();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::sendToBack()
{
	QLanguageItem* languageItem = dynamic_cast<QLanguageItem*>(sender());
	if ( !languageItem )
		languageItem = mFirstSelectedItem;
	if ( !languageItem )
		return;

	qsizetype index = mZOrderedItems.indexOf( languageItem );
	mZOrderedItems.move( index , 0 );
	updateZOrders();
}

//-----------------------------------------------------------------------//	
//						Private functions								 //
//-----------------------------------------------------------------------//

//-----------------------------------------------------------------------
void GraphicsSceneMainWindow::checkSettings()
{	
	QSettings settings;
	if ( !settings.value( INIT_DONE_SETTING ).toBool() )
	{
		reinitSettings();
	}

	//Update dependencies.
	readSettings();
}

/*!
*	\brief Reads last application's settings, using Qt's QSetting system.
*/
//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::readSettings()
{
	readPreferencesSettings();

	QSettings settings;

//	Font size for QTextEdits
	changeFontSize( settings.value( FONT_SIZE_SETTING ).toDouble() );
	
//	Window position & size
	QPoint pos = settings.value(WINDOW_POS_SETTING	, DEFAULT_WINDOW_POS ).toPoint();
    QSize size = settings.value(WINDOW_SIZE_SETTING	, QSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) ).toSize();
    resize(size);
    move(pos);

//	Dockwidgets & toolbars status (see QMainWindow::restoreState)
	QByteArray state = settings.value( WINDOW_STATE_SETTING ).toByteArray();
	restoreState( state );

//	Side history/storage bars visibility (those are not DockWidgets and thus must be managed manually). 
	if ( mSettings.mHasStorage )
	{	
		mStorageGraphicsView->setVisible( settings.value( STORAGE_VISIBLE_SETTING , true ).toBool() );
	}
	if ( mSettings.mHasHistory )
	{
		mHistoryGraphicsView->setVisible( settings.value( HISTORY_VISIBLE_SETTING , true ).toBool() );
	}
	
//	Current graphicView zoom.
	zoom( (float)settings.value( WINDOW_ZOOM_SETTING , 1.0f ).toDouble() );

//	Current scene ( <=> workspace )
	QString currentScene = settings.value( CURRENT_SCENE_SETTING ).toString();
	if ( mIsWorkspaceModeOn )
	{
		if ( !loadSceneFile( currentScene ) )
		{
			QString msg;
			if ( currentScene.size() != 0 )
				msg = "Can't load the scene " + currentScene + ". ";
			msg += "Choose the path of your new 'workspace' scene.";
			QMessageBox::warning(this , "Workspace scene", msg);
			saveSceneAs();
		}
	}

//	Last openned file
	mLastOpenFilePath = settings.value( FILE_DIALOG_PATH_SETTING ).toString();
}


//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::readPreferencesSettings()
{
	QSettings settings;

//	Workspace mode on/off
//	mIsWorkspaceModeOn = settings.value( WORKSPACE_MODE_SETTING ).toBool();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::createActions()
{

	mNewAct = new QAction(QIcon( RSC_DIR + QString("new.png") ), tr("&New scene") , this);
	mNewAct->setShortcut( QKeySequence::New );
	mNewAct->setStatusTip(tr("Create a new scene"));
	if ( !mSettings.mDesactiveUIElements.contains( NEW_ACT ) )
		connect(mNewAct , SIGNAL(triggered()) , this , SLOT(newScene()) );

	mSaveAct = new QAction(QIcon(RSC_DIR + QString("save.png")), tr("&Save item or scene") , this);
	mSaveAct->setShortcut( QKeySequence::Save );
	mSaveAct->setStatusTip(tr("Save the current item or the current scene"));
	if ( !mSettings.mDesactiveUIElements.contains( SAVE_ACT ) )
		connect(mSaveAct , SIGNAL(triggered()) , this , SLOT(save()) );

	mSaveAsAct = new QAction(QIcon(RSC_DIR + QString("save.png")), tr("&Save scene as...") , this);
	mSaveAsAct->setStatusTip(tr("Save the current scene to another file"));
	if ( !mSettings.mDesactiveUIElements.contains( SAVE_AS_ACT ) )
		connect(mSaveAsAct , SIGNAL(triggered()) , this , SLOT(saveSceneAs()) );

	mSaveAllAct = new QAction(QIcon(RSC_DIR + QString("saveAll.png")), tr("Save All") , this);
	mSaveAllAct->setStatusTip( "Save all items & scene" );
	if ( !mSettings.mDesactiveUIElements.contains( SAVE_ALL_ACT ) )
		connect(mSaveAllAct , SIGNAL(triggered()) , this , SLOT(saveAll()) );
		
	mOpenAct = new QAction(QIcon(RSC_DIR + QString("open.png")), tr("&Open scene...") , this);
	mOpenAct->setShortcut( QKeySequence::Open );
	mOpenAct->setStatusTip(tr("Open a scene"));
	if ( !mSettings.mDesactiveUIElements.contains( OPEN_ACT ) )
		connect(mOpenAct , SIGNAL(triggered()) , this , SLOT(loadScene()) );

    mExitAct = new QAction(tr("E&xit"), this);
    mExitAct->setShortcut(tr("Ctrl+Q"));
    mExitAct->setStatusTip(tr("Exit the application"));
	if ( !mSettings.mDesactiveUIElements.contains( EXIT_ACT ) )
		connect(mExitAct, SIGNAL(triggered()), this, SLOT(close()));

    mAboutQtAct = new QAction(tr("About &Qt"), this);
    mAboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	if ( !mSettings.mDesactiveUIElements.contains( ABOUT_QT_ACT ) )
		connect(mAboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	mZoomInAct = new QAction(QIcon(RSC_DIR + QString("zoomIn.png")), tr("Zoom &In") , this);
	mZoomInAct->setShortcut( QKeySequence::ZoomIn );
	mZoomInAct->setStatusTip(tr("Zoom in"));
	if ( !mSettings.mDesactiveUIElements.contains( ZOOM_IN_ACT ) )
		connect(mZoomInAct , SIGNAL(triggered()) , this , SLOT(zoomIn()) );
	
	mZoomOutAct = new QAction(QIcon(RSC_DIR + QString("zoomOut.png")), tr("Zoom O&ut") , this);
	mZoomOutAct->setShortcut( QKeySequence::ZoomOut);
	mZoomOutAct->setStatusTip(tr("Zoom out"));
	if ( !mSettings.mDesactiveUIElements.contains( ZOOM_OUT_ACT ) )
		connect(mZoomOutAct , SIGNAL(triggered()) , this , SLOT(zoomOut()) );
		
	mZoomStandardAct = new QAction( tr("Zoom 100%") , this);
	mZoomStandardAct->setStatusTip(tr("Zoom 100%"));
	if ( !mSettings.mDesactiveUIElements.contains( ZOOM_STANDARD_ACT ) )
		connect(mZoomStandardAct , SIGNAL(triggered()) , this , SLOT(zoomStandard()) );

	mFontBiggerAct = new QAction(QIcon(RSC_DIR + QString("fontBigger.png")), tr("Font bigger") , this);
	mFontBiggerAct->setShortcut( tr("Ctrl+Shift+Plus") );
	mFontBiggerAct->setStatusTip(tr("Font bigger"));
	if ( !mSettings.mDesactiveUIElements.contains( FONT_BIGGER_ACT ) )
		connect(mFontBiggerAct , SIGNAL(triggered()) , this , SLOT(fontBigger()) );

	mFontSmallerAct = new QAction(QIcon(RSC_DIR + QString("fontSmaller.png")), tr("Font smaller") , this);
	mFontSmallerAct->setShortcut( tr("Ctrl+Shift+Minus") );
	mFontSmallerAct->setStatusTip(tr("Font smaller"));
	if ( !mSettings.mDesactiveUIElements.contains( FONT_SMALLER_ACT ) )
		connect(mFontSmallerAct , SIGNAL(triggered()) , this , SLOT(fontSmaller()) );	

	mAddAct = new QAction(QIcon(RSC_DIR + QString("add.png")), tr("Add &item") , this);
	mAddAct->setShortcut( tr("Ctrl+I") );
	mAddAct->setStatusTip("Add a new " + LANGUAGE_NAME_SHORT + " item");
	if ( !mSettings.mDesactiveUIElements.contains( ADD_ACT ) )
		connect(mAddAct , SIGNAL(triggered()) , this , SLOT(addItem()) );

	mAddFromFileAct = new QAction(QIcon(RSC_DIR + QString("addFromFile.png")), tr("Add item from file...") , this);
	mAddFromFileAct->setShortcut( tr("Ctrl+Shift+O") );
	mAddFromFileAct->setStatusTip("Add a new " + LANGUAGE_NAME_SHORT + " item from a file");
	if ( !mSettings.mDesactiveUIElements.contains( ADD_FROM_FILE_ACT ) )
		connect(mAddFromFileAct , SIGNAL(triggered()) , this , SLOT(addItemFromFile()) );

	mRemoveAct = new QAction(QIcon(RSC_DIR + QString("remove.png")), tr("Remove items") , this);
	mRemoveAct->setShortcut( QKeySequence::Delete);
	mRemoveAct->setStatusTip("Remove the selected " + LANGUAGE_NAME_SHORT + " items");
	if ( !mSettings.mDesactiveUIElements.contains( REMOVE_ACT ) )
		connect(mRemoveAct , SIGNAL(triggered()) , this , SLOT(removeSelectedItems()) );
	
	mCopyAct = new QAction(QIcon(RSC_DIR + QString("copy.png")), tr("&Copy") , this);
	mCopyAct->setShortcut( QKeySequence::Copy);
	mCopyAct->setStatusTip("Copy the selected " + LANGUAGE_NAME_SHORT + " items");
	if ( !mSettings.mDesactiveUIElements.contains( COPY_ACT ) )
		connect(mCopyAct , SIGNAL(triggered()) , this , SLOT(copyItem()) );
	
	mPasteAct = new QAction(QIcon(RSC_DIR + QString("paste.png")), tr("&Paste") , this);
	mPasteAct->setShortcut( QKeySequence::Paste);
	mPasteAct->setStatusTip(tr("Paste previously copied items"));
	if ( !mSettings.mDesactiveUIElements.contains( PASTE_ACT ) )
		connect(mPasteAct , SIGNAL(triggered()) , this , SLOT(pasteItem()) );
	
	mCleanHistoryAct = new QAction(QIcon(), tr("Clean history") , this);
	mCleanHistoryAct->setStatusTip(tr("Clean your history side bar"));
	if ( !mSettings.mDesactiveUIElements.contains( CLEAN_HISTORY_ACT ) )
		connect(mCleanHistoryAct , SIGNAL(triggered()) , this , SLOT(cleanHistory()) );
	
	mSelectAllAct = new QAction(QIcon(), tr("Select &All") , this);
	mSelectAllAct->setShortcut( QKeySequence::SelectAll);
	mSelectAllAct->setStatusTip(tr("Select all the items"));
	if ( !mSettings.mDesactiveUIElements.contains( SELECT_ALL_ACT ) )
		connect(mSelectAllAct , SIGNAL(triggered()) , this , SLOT(selectAll()) );
	
	mRescaleAct = new QAction(QIcon(RSC_DIR + QString("resize.png")), tr("&Rescale") , this);
	mRescaleAct->setShortcut( tr("Ctrl+R") );
	mRescaleAct->setStatusTip(tr("Set the scale of all selected items to the scale of the first selected one"));
	if ( !mSettings.mDesactiveUIElements.contains( RESCALE_ACT ) )
		connect(mRescaleAct , SIGNAL(triggered()) , this , SLOT(resizeItems()) );

	mHAlignAct = new QAction(QIcon(RSC_DIR + QString("hAlign.png")), tr("&Horizontal alignment") , this);
	mHAlignAct->setShortcut( tr("Ctrl+H") );
	mHAlignAct->setStatusTip(tr("Horizontally align all selected items on the first selected one"));
	if ( !mSettings.mDesactiveUIElements.contains( H_ALIGN_ACT ) )
		connect(mHAlignAct , SIGNAL(triggered()) , this , SLOT(hAlignItems()) );

	mVAlignAct = new QAction(QIcon(RSC_DIR + QString("vAlign.png")), tr("Ver&tical alignment") , this);
	mVAlignAct->setShortcut( tr("Ctrl+T") );
	mVAlignAct->setStatusTip(tr("Vertically align all selected items on the first selected one"));
	if ( !mSettings.mDesactiveUIElements.contains( V_ALIGN_ACT ) )
		connect(mVAlignAct , SIGNAL(triggered()) , this , SLOT(vAlignItems()) );

	mBringToFrontAct = new QAction(QIcon(RSC_DIR + QString("bringToFront.png")), tr("Bring to front") , this);
	mBringToFrontAct->setStatusTip(tr("Brings the first selected item to front"));
	if ( !mSettings.mDesactiveUIElements.contains( BRING_TO_FRONT_ACT ) )
		connect(mBringToFrontAct , SIGNAL(triggered()) , this , SLOT(bringToFront()) );

	mBringForwardAct = new QAction(QIcon(RSC_DIR + QString("bringForward.png")), tr("Bring forward") , this);
	mBringForwardAct->setStatusTip(tr("Brings the first selected item forward"));
	if ( !mSettings.mDesactiveUIElements.contains( BRING_FORWARD_ACT ) )
		connect(mBringForwardAct , SIGNAL(triggered()) , this , SLOT(bringForward()) );
		
	mSendToBackAct = new QAction(QIcon(RSC_DIR + QString("sendToBack.png")), tr("Send to back") , this);
	mSendToBackAct->setStatusTip(tr("Sends the first selected item to back"));
	if ( !mSettings.mDesactiveUIElements.contains( SEND_TO_BACK_ACT ) )
		connect(mSendToBackAct , SIGNAL(triggered()) , this , SLOT(sendToBack()) );
	
	mSendBackwardAct = new QAction(QIcon(RSC_DIR + QString("sendBackward.png")), tr("Send backward") , this);
	mSendBackwardAct->setStatusTip(tr("Sends the first selected item backward"));
	if ( !mSettings.mDesactiveUIElements.contains( SEND_BACKWARD_ACT ) )
		connect(mSendBackwardAct , SIGNAL(triggered()) , this , SLOT(sendBackward()) );

	mSwitchHistoryVisible = 0;
	mSwitchStorageVisible = 0;

}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::createMenus()
{
	if ( !mSettings.mDesactiveUIElements.contains( FILE_MENU ) )
	{
		mFileMenu = menuBar()->addMenu(tr("&File"));
		if ( !mSettings.mDesactiveUIElements.contains( NEW_ACT ) ) mFileMenu->addAction(mNewAct);
		if ( !mSettings.mDesactiveUIElements.contains( OPEN_ACT ) ) mFileMenu->addAction(mOpenAct);
		if ( !mSettings.mDesactiveUIElements.contains( SAVE_ACT ) ) mFileMenu->addAction(mSaveAct);
		if ( !mSettings.mDesactiveUIElements.contains( SAVE_AS_ACT ) ) mFileMenu->addAction(mSaveAsAct);
		if ( !mSettings.mDesactiveUIElements.contains( SAVE_ALL_ACT ) ) mFileMenu->addAction(mSaveAllAct);
		if ( !mSettings.mDesactiveUIElements.contains( EXIT_ACT ) ) mFileMenu->addAction(mExitAct);
	}
	
	if ( !mSettings.mDesactiveUIElements.contains( EDIT_MENU ) )	
    {
		mEditMenu = menuBar()->addMenu(tr("&Edit"));
		if ( !mSettings.mDesactiveUIElements.contains( ADD_ACT ) ) mEditMenu->addAction(mAddAct);
		if ( !mSettings.mDesactiveUIElements.contains( ADD_FROM_FILE_ACT ) ) mEditMenu->addAction(mAddFromFileAct);
		if ( !mSettings.mDesactiveUIElements.contains( REMOVE_ACT ) ) mEditMenu->addAction(mRemoveAct);
		mEditMenu->addSeparator();
		if ( !mSettings.mDesactiveUIElements.contains( CLEAN_HISTORY_ACT ) ) mEditMenu->addAction( mCleanHistoryAct );
		mEditMenu->addSeparator();
		if ( !mSettings.mDesactiveUIElements.contains( SELECT_ALL_ACT ) ) mEditMenu->addAction( mSelectAllAct );
		if ( !mSettings.mDesactiveUIElements.contains( COPY_ACT ) ) mEditMenu->addAction(mCopyAct);
		if ( !mSettings.mDesactiveUIElements.contains( PASTE_ACT ) ) mEditMenu->addAction(mPasteAct);
		mEditMenu->addSeparator();
		if ( !mSettings.mDesactiveUIElements.contains( RESCALE_ACT ) ) mEditMenu->addAction( mRescaleAct );
		if ( !mSettings.mDesactiveUIElements.contains( H_ALIGN_ACT ) ) mEditMenu->addAction(mHAlignAct);
		if ( !mSettings.mDesactiveUIElements.contains( V_ALIGN_ACT ) ) mEditMenu->addAction(mVAlignAct);
		mEditMenu->addSeparator();
		if ( !mSettings.mDesactiveUIElements.contains( BRING_TO_FRONT_ACT ) )	mEditMenu->addAction( mBringToFrontAct );
		if ( !mSettings.mDesactiveUIElements.contains( BRING_FORWARD_ACT ) )	mEditMenu->addAction( mBringForwardAct );
		if ( !mSettings.mDesactiveUIElements.contains( SEND_BACKWARD_ACT ) )	mEditMenu->addAction( mSendBackwardAct );
		if ( !mSettings.mDesactiveUIElements.contains( SEND_TO_BACK_ACT ) )		mEditMenu->addAction( mSendToBackAct );
	}
	
	if ( !mSettings.mDesactiveUIElements.contains( VIEW_MENU ) )
	{
		mViewMenu = menuBar()->addMenu(tr("&View"));
		if ( !mSettings.mDesactiveUIElements.contains( ZOOM_IN_ACT ) )			mViewMenu->addAction(mZoomInAct);
		if ( !mSettings.mDesactiveUIElements.contains( ZOOM_OUT_ACT ) )			mViewMenu->addAction(mZoomOutAct);
		if ( !mSettings.mDesactiveUIElements.contains( ZOOM_STANDARD_ACT ) )	mViewMenu->addAction(mZoomStandardAct);

		if ( !mSettings.mDesactiveUIElements.contains( FONT_BIGGER_ACT ) ) mViewMenu->addAction(mFontBiggerAct);
		if ( !mSettings.mDesactiveUIElements.contains( FONT_SMALLER_ACT ) ) mViewMenu->addAction(mFontSmallerAct);
	}
	
	if ( !mSettings.mDesactiveUIElements.contains( HELP_MENU ) )
	{
		mHelpMenu = menuBar()->addMenu(tr("&About"));
		if ( !mSettings.mDesactiveUIElements.contains( ABOUT_QT_ACT ) )		mHelpMenu->addAction(mAboutQtAct);
	}
	
	if ( !mSettings.mDesactiveUIElements.contains( WINDOW_MENU ) )
	{
		mWindowMenu = createPopupMenu();
		mWindowMenu->setTitle( tr("&Window") );
		menuBar()->addMenu( mWindowMenu );
		connect( mWindowMenu , SIGNAL( aboutToShow() ) , this , SLOT( updateWindowMenu() ) );
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::createToolBars()
{
	if ( !mSettings.mDesactiveUIElements.contains( FILE_TOOL_BAR ) )
	{
		mFileToolBar = addToolBar(tr("&File"));
		if ( !mSettings.mDesactiveUIElements.contains( NEW_ACT ) )		mFileToolBar->addAction(mNewAct);
		if ( !mSettings.mDesactiveUIElements.contains( OPEN_ACT ) )		mFileToolBar->addAction(mOpenAct);
		if ( !mSettings.mDesactiveUIElements.contains( SAVE_ACT ) )		mFileToolBar->addAction(mSaveAct);
		if ( !mSettings.mDesactiveUIElements.contains( SAVE_ALL_ACT ) ) mFileToolBar->addAction(mSaveAllAct);
		mFileToolBar->addSeparator();
		mFileToolBar->setObjectName("FileToolBar");
	}
	
	if ( !mSettings.mDesactiveUIElements.contains( EDIT_TOOL_BAR ) )
	{
		mEditToolBar = addToolBar(tr("Edit"));
		if ( !mSettings.mDesactiveUIElements.contains( ADD_ACT ) ) mEditToolBar->addAction(mAddAct);
		if ( !mSettings.mDesactiveUIElements.contains( ADD_FROM_FILE_ACT ) ) mEditToolBar->addAction(mAddFromFileAct);
		if ( !mSettings.mDesactiveUIElements.contains( REMOVE_ACT ) ) mEditToolBar->addAction(mRemoveAct);
		if ( !mSettings.mDesactiveUIElements.contains( COPY_ACT ) ) mEditToolBar->addAction(mCopyAct);
		if ( !mSettings.mDesactiveUIElements.contains( PASTE_ACT ) ) mEditToolBar->addAction(mPasteAct);
		mEditToolBar->addSeparator();
		mEditToolBar->setObjectName("EditToolBar");
	}
	
	if ( !mSettings.mDesactiveUIElements.contains( VIEW_TOOL_BAR ) )
	{
		mViewToolBar = addToolBar(tr("View"));
		if ( !mSettings.mDesactiveUIElements.contains( ZOOM_IN_ACT ) ) mViewToolBar->addAction(mZoomInAct);
		if ( !mSettings.mDesactiveUIElements.contains( ZOOM_OUT_ACT ) ) mViewToolBar->addAction(mZoomOutAct);

		if ( !mSettings.mDesactiveUIElements.contains( FONT_BIGGER_ACT ) ) mViewToolBar->addAction(mFontBiggerAct);
		if ( !mSettings.mDesactiveUIElements.contains( FONT_SMALLER_ACT ) ) mViewToolBar->addAction(mFontSmallerAct);

		mViewToolBar->addSeparator();
		mViewToolBar->setObjectName("ViewToolBar");
	}
		
	mResizeSlider = new QSlider(Qt::Horizontal);
	mResizeSlider->setRange( -1 * SLIDER_PRECISION_FACTOR , 1  * SLIDER_PRECISION_FACTOR );
	mResizeSlider->setValue( 0 );
	mResizeSlider->setMaximumWidth( width() / 10 + SLIDER_TOOL_BAR_MARGIN );
	mResizeSlider->setMinimumWidth( width() / 10 + SLIDER_TOOL_BAR_MARGIN );
	mResizeSlider->setStatusTip(tr("Resize the selected items"));
	connect(mResizeSlider , SIGNAL(valueChanged(int)) , this , SLOT(resizeSliderChanged(int)) );
	
	mResizeEdit = new QDoubleSpinBox();
	mResizeEdit->setRange( mSettings.mMinScale , mSettings.mMaxScale );
	mResizeEdit->setDecimals(2);
	mResizeEdit->setValue(1);
	mResizeEdit->setSingleStep( 0.1f );
	mResizeEdit->setAccelerated( true);
	mResizeEdit->setStatusTip(tr("Resize the selected items"));
	connect( mResizeEdit , SIGNAL(valueChanged(double)) , this , SLOT(scaleItems(double)) );
	
	mItemXEdit = new QDoubleSpinBox();
	mItemXEdit->setRange( -MAX_COORDINATE_EDIT , MAX_COORDINATE_EDIT );
	mItemXEdit->setStatusTip(tr("Change the abscissa of the selected item"));
	mItemXEdit->setAccelerated( true);
	connect( mItemXEdit , SIGNAL(valueChanged(double)) , this , SLOT(moveItemX(double)) );

	mItemYEdit = new QDoubleSpinBox();
	mItemYEdit->setRange( -MAX_COORDINATE_EDIT , MAX_COORDINATE_EDIT );
	mItemYEdit->setStatusTip(tr("Change the ordinate of the selected item"));
	mItemYEdit->setAccelerated( true);
	connect( mItemYEdit , SIGNAL(valueChanged(double)) , this , SLOT(moveItemY(double)) );
	
	if ( !mSettings.mDesactiveUIElements.contains( SIZE_TOOL_BAR ) )
	{
		mSizeToolBar = addToolBar(tr("Scale && position"));
		if ( !mSettings.mDesactiveUIElements.contains( RESCALE_ACT ) )		mSizeToolBar->addAction( mRescaleAct );
		if ( !mSettings.mDesactiveUIElements.contains( RESIZE_SLIDER ) )	{ mSizeToolBar->addWidget( mResizeSlider );		mResizeSlider->setParent(mSizeToolBar); }
		if ( !mSettings.mDesactiveUIElements.contains( RESIZE_EDIT ) )		{ mSizeToolBar->addWidget( mResizeEdit );		mResizeEdit->setParent(mSizeToolBar); }
		mSizeToolBar->addSeparator();
		if ( !mSettings.mDesactiveUIElements.contains( H_ALIGN_ACT ) )		mSizeToolBar->addAction( mHAlignAct );
		if ( !mSettings.mDesactiveUIElements.contains( V_ALIGN_ACT ) )	mSizeToolBar->addAction( mVAlignAct );
		if ( !mSettings.mDesactiveUIElements.contains( X_ITEM_EDIT ) )	{ mSizeToolBar->addWidget( new QLabel("X:" , mSizeToolBar) ); }
		if ( !mSettings.mDesactiveUIElements.contains( X_ITEM_EDIT ) )	{ mSizeToolBar->addWidget( mItemXEdit );	mItemXEdit->setParent(mSizeToolBar); }
		if ( !mSettings.mDesactiveUIElements.contains( Y_ITEM_EDIT ) )	{ mSizeToolBar->addWidget( new QLabel("Y:" , mSizeToolBar) ); }
		if ( !mSettings.mDesactiveUIElements.contains( Y_ITEM_EDIT ) )	{ mSizeToolBar->addWidget( mItemYEdit );	mItemYEdit->setParent(mSizeToolBar); }
		mSizeToolBar->addSeparator();
		mSizeToolBar->setObjectName("ScaleToolBar");
	}

}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
	
	mViewXLabel	= new QLabel( "X = 0" , this);
	mViewXLabel->setMinimumWidth(75);
	mViewXLabel->setMaximumWidth(75);
	mViewXLabel->setAlignment(Qt::AlignLeft);
	mViewYLabel	= new QLabel( "Y = 0" , this);
	mViewYLabel->setMinimumWidth(75);
	mViewYLabel->setMaximumWidth(75);
	mViewYLabel->setAlignment(Qt::AlignLeft);
	mViewScaleLabel = new QLabel( "Zoom = 1.0" , this);
	mViewScaleLabel->setMinimumWidth(100);
	mViewScaleLabel->setMaximumWidth(100);
	mViewScaleLabel->setAlignment(Qt::AlignLeft);
	
	if ( !mSettings.mDesactiveUIElements.contains( STATUS_BAR_MOUSE_POS ) )	statusBar()->addPermanentWidget( new QLabel("View: " , this) );
	if ( !mSettings.mDesactiveUIElements.contains( STATUS_BAR_MOUSE_POS ) )	statusBar()->addPermanentWidget( mViewXLabel );
	if ( !mSettings.mDesactiveUIElements.contains( STATUS_BAR_MOUSE_POS ) )	statusBar()->addPermanentWidget( mViewYLabel );
	if ( !mSettings.mDesactiveUIElements.contains( STATUS_BAR_ZOOM ) )		statusBar()->addPermanentWidget( mViewScaleLabel );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::setupTextEdit()
{
	mLanguageTextEdit = new LanguageTextEdit("" , this);

	mLanguageTextEdit->setAcceptRichText( false );
	mLanguageTextEdit->setAcceptDrops( false );

	mLanguageTextEditDock = new QDockWidget(GMN_EDIT_DOCK_NAME , this);
	mLanguageTextEditDock->setWidget( mLanguageTextEdit );
	mLanguageTextEditDock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable );
	mLanguageTextEditDock->setObjectName( "LanguageTextEditDock" );
	
	connect(mLanguageTextEdit,SIGNAL(cursorPositionChanged(void)),this,SLOT(textEditActivity(void)));
	connect(mLanguageTextEdit,SIGNAL(textChanged(void)),this,SLOT(textEditActivity(void)));
	connect(mLanguageTextEdit,SIGNAL(selectionChanged(void)),this,SLOT(textEditActivity(void)));

	addDockWidget( Qt::BottomDockWidgetArea , mLanguageTextEditDock );

//	Setup GuidoItem refresh timer (update guidoItem render when code changes)
	mLanguageTextEditTimer = new QTimer( this );
	connect( mLanguageTextEditTimer , SIGNAL( timeout() ) , this , SLOT( updateCode() ) );

	if ( !LANGUAGE_COMMANDS_FILE.isEmpty() )
	{
		QDockWidget * gmnPaletteDock = new QDockWidget(COMMAND_PALETTE_DOCK_NAME , this);
		QLanguageCommandPalette * gmnPalette = new QLanguageCommandPalette( LANGUAGE_COMMANDS_FILE , this);
		gmnPaletteDock->setWidget( gmnPalette );
		gmnPaletteDock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable );
		gmnPaletteDock->setVisible( true );
		gmnPaletteDock->setFloating( true );
		addDockWidget( Qt::BottomDockWidgetArea , gmnPaletteDock );
		gmnPaletteDock->setObjectName( "languagePaletteDock" );

		mCommandPaletteDock = gmnPaletteDock;

		connect( gmnPalette , SIGNAL( insertCode(const QString& , int) ) , this , SLOT( insertText( const QString& , int ) ) );
//		connect( mLanguageTextEdit , SIGNAL( enabled(bool) ) , gmnPalette , SLOT( setEnabled(bool) ) );
	}
	
	mLanguageTextEdit->setEnabled(false);
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::createHistoryAndStorage()
{
	if ( mSettings.mHasHistory )
	{
		mHistoryGraphicsScene = new QGraphicsScene(this);
		mHistoryGraphicsView = new QHistoryGraphicsView( mHistoryGraphicsScene , mFactory );
	}
	else
		mHistoryGraphicsView = 0;
	
	if ( mSettings.mHasStorage )
	{
		mStorageGraphicsScene = new QGraphicsScene( this );
		mStorageGraphicsView = new QStorageGraphicsView( mStorageGraphicsScene , this , mFactory );
	}
	else
	{
		mStorageGraphicsView = 0;
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::reinitScene(QRectF sceneRect)
{
	if ( mGraphicsScene == 0 )
	{
		mGraphicsScene = new QGraphicsScene(this);
		connect(mGraphicsScene,SIGNAL(selectionChanged ()),this,SLOT(sceneSelectionChanged()));
		connect(mGraphicsScene,SIGNAL(changed( const QList<QRectF>& )),this,SLOT(sceneChanged( const QList<QRectF> &)));
		mGraphicsView->setScene( mGraphicsScene );
	
		mBackgroundItem = new QBackgroundItem( this , QSizeF(sceneRect.width() , sceneRect.height()) );
//		mBackgroundItem->setPos( sceneRect.x() , sceneRect.y() );
		mBackgroundItem->setZValue( -1 );

		mGraphicsScene->addItem( mBackgroundItem );
	}
	else
		// Removes all languageItems.
		removeAllItems();
	
//	mBackgroundItem->setPageSize( QSizeF(sceneRect.width() , sceneRect.height()) );
//	mBackgroundItem->setPos( sceneRect.x() , sceneRect.y() );
	mBackgroundItem->setRect( sceneRect );

	if ( mSettings.mHasHistory )
		mHistoryGraphicsView->clear();
	if ( mSettings.mHasStorage )
		mStorageGraphicsView->clear();
}

/*
*	Shows the user a dialog with 3 choices :
*		- save :		saves the scene and returns true
*		- don't save :	doesn't save the scene and returns true
*		- cancel :		doesn't save the scene and returns false
*
*	If save isn't necessary (no changes detected), no dialog is shown, 
*	returns true.
*/
//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::saveOrCancelDialog()
{
	if ( ( !mSceneHasChanged ) || ( mGraphicsScene->items().size() == 0 ) )
		return true;

	if ( !checkAllItems() )
		return false;
	
	QMessageBox::StandardButton result = QMessageBox::question ( 0, "Save", "The scene has been modified. Would you like to save the current scene ?",
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel );
	if ( result != QMessageBox::Cancel )
	{
		if ( result == QMessageBox::Yes )
			saveScene();
		return true;
	}
	else
	{
		return false;
	}
}


//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::checkAllItems()
{
	QList<QLanguageItem*> items = languageItems();
	for ( int i = 0 ; i < items.size() ; i++ )
	{
		if ( !saveItemModificationsDialog( items[i] ) )
			return false;
	}
	return true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::saveScene()
{
	QString savePath = windowFilePath();
	if ( savePath == "" )
	{
		saveSceneAs();
	}
	else
	{
		saveSceneFile(savePath);
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::saveSceneFile(const QString& fileName)
{
	//Create & initialize a DomDocument
	QDomDocument doc( DOM_DOC + mSettings.mLanguageNameShort );
	QDomElement root = doc.createElement( DOM_ROOT );
	doc.appendChild( root );

	//Add first element : Scene
	QPointF viewCenter = viewCenterInSceneCoordinate();
	QDomElement element = doc.createElement( DOM_SCENE );
  	element.setAttribute( DOM_SCENE_X,		mGraphicsScene->sceneRect().x() );
	element.setAttribute( DOM_SCENE_Y,		mGraphicsScene->sceneRect().y() );
  	element.setAttribute( DOM_SCENE_WIDTH,	mGraphicsScene->sceneRect().width() );
	element.setAttribute( DOM_SCENE_HEIGHT,	mGraphicsScene->sceneRect().height() );
  	element.setAttribute( DOM_VIEW_X, viewCenter.x() );
	element.setAttribute( DOM_VIEW_Y, viewCenter.y() );
  	element.setAttribute( DOM_VIEW_ZOOM, mGraphicsView->transform().m11() );
	
	if ( mSettings.mHasStorage )
	{
		QList<int> splitterSizes = mSplitter->sizes();
		element.setAttribute( DOM_SPLITTER_SIZE_0 , splitterSizes[0] );
		element.setAttribute( DOM_SPLITTER_SIZE_1 , splitterSizes[1] );
	}

	root.appendChild( element );
	
	//Add 2nd element : storage
	if ( mSettings.mHasStorage )
		root.appendChild( mStorageGraphicsView->save( doc ) );
	
	//Add 3rd element : history
	if ( mSettings.mHasHistory )
		root.appendChild( mHistoryGraphicsView->save( doc ) );
	
	//Populate the DomDocument with the GuidoItems.
	QList<QLanguageItem*> items = languageItems();
	for ( int i = 0 ; i < items.size() ; i++ )
	{
		root.appendChild( items[i]->saveToDomElement(&doc) );
	}
							
	QFile file(fileName);
	if (file.open(QFile::WriteOnly | QFile::Truncate)) 
	{
		QTextStream ts( &file );
		ts << doc.toString();
		
		setWindowFilePath(fileName);
		mSceneHasChanged = false;
	}
	file.close();

	QSettings().setValue( CURRENT_SCENE_SETTING, fileName );
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::loadSceneFile(const QString& fileName)
{
	QFile file( fileName );
	if( !file.open( QFile::ReadOnly ) )
		//Can't read file
		return false;
	
	//Create a new DomDocument with file contents
	QDomDocument doc( DOM_DOC + mSettings.mLanguageNameShort );
	if( !doc.setContent( &file ) )
	{
		file.close();
		//File doesn't contain valid XML data
		return false;
	}
	file.close();
	
	//Check the root
	QDomElement root = doc.documentElement();
	if( root.tagName() != DOM_ROOT )
		//No valid Guido-Root found.
		return false;
	
	//Read the first DomElement: scene
	float centerX;
	float centerY;
	QRectF sceneRect;
	QDomNode n = root.firstChild();	
	if ( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if( e.tagName() == DOM_SCENE )
			{
				float sceneX		= QVariant( e.attribute( DOM_SCENE_X, "" ) ).toDouble();
				float sceneY		= QVariant( e.attribute( DOM_SCENE_Y, "" ) ).toDouble();
				float sceneWidth	= QVariant( e.attribute( DOM_SCENE_WIDTH, "" ) ).toDouble();
				float sceneHeight	= QVariant( e.attribute( DOM_SCENE_HEIGHT, "" ) ).toDouble();
				centerX				= QVariant( e.attribute( DOM_VIEW_X, "" ) ).toDouble();
				centerY				= QVariant( e.attribute( DOM_VIEW_Y, "" ) ).toDouble();
				float zoom			= QVariant( e.attribute( DOM_VIEW_ZOOM, "" ) ).toDouble();
				sceneRect.setX(sceneX);
				sceneRect.setY(sceneY);
				sceneRect.setWidth(sceneWidth);
				sceneRect.setHeight(sceneHeight);
				
				reinitScene(sceneRect);
				mGraphicsView->setTransform( QTransform() );
				mGraphicsView->scale( zoom , zoom );
				mGraphicsView->centerOn( centerX , centerY );
				
				if ( mSettings.mHasStorage )
				{
					int splitterSize0	= QVariant( e.attribute( DOM_SPLITTER_SIZE_0, "" ) ).toInt();
					int splitterSize1	= QVariant( e.attribute( DOM_SPLITTER_SIZE_1, "" ) ).toInt();
					mSplitter->setSizes( QList<int>() << splitterSize0 << splitterSize1 );
				}
			}
		}
	}
	n = n.nextSibling();
	
	//Load storage
	if ( mSettings.mHasStorage )
	{
		if( !n.isNull() )
		{
			QDomElement e = n.toElement();
			if( !e.isNull() )
			{
				if( e.tagName() == mStorageGraphicsView->domId() )
				{
					if ( !mStorageGraphicsView->load( e ) )
						return false;
				}
			}
		}
		n = n.nextSibling();
	}
	//Load history
	if ( mSettings.mHasHistory )
	{
		if( !n.isNull() )
		{
			QDomElement e = n.toElement();
			if( !e.isNull() )
			{
				if( e.tagName() == mHistoryGraphicsView->domId() )
				{
					if ( !mHistoryGraphicsView->load( e ) )
						return false;
				}
			}
		}
		n = n.nextSibling();
	}
	
	//Create the Guido Items
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			QLanguageItem * i = mFactory->buildLanguageItem(&e);			
			if ( fileControl(i) )
				setupNAddItem(i);
		}

		n = n.nextSibling();
	}
				
	setWindowFilePath(fileName);
	QSettings().setValue( CURRENT_SCENE_SETTING , fileName );
	updateWindowState();
	updateZoomLabel();
	mSceneHasChanged = false;
	
	return true;
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::dropItem( const QMimeData * mimeData , const QPointF& pos , QLanguageItem** createdItem )
{
//	mGraphicsView->setFocus();

	//Drop a/several file(s) from outside the application.
	if (		mimeData->hasUrls() 
			&&	!mimeData->hasFormat( FROM_SCENE_DRAG_MIME )
			&&	!mimeData->hasFormat( FROM_HISTORY_DRAG_MIME )
			&&	!mimeData->hasFormat( FROM_STORAGE_DRAG_MIME )
	)
	{
		int dx = 0;
		
		//Unselect all
		unselectAll();
		
		//Drop several files at a time
		for ( int i = 0 ; i < mimeData->urls().size() ; i++ )
		{
			//Create an item for each dropped file
			QString fileName = mimeData->urls()[i].toLocalFile();

			if ( !addFile( fileName , createdItem ) )
				return false;
			else
			{
				//Move each new item a the right border of the previously added item, to put them
				//in an horizontal line.
				QRectF itemRect = (*createdItem)->boundingRect();
				if ( i==0 ) dx -= itemRect.width() / 2;
				dx += itemRect.width() / 2 + 1;
				(*createdItem)->setPos( pos.x() - itemRect.width() /2 + dx , pos.y() - itemRect.height()/2 );
				dx += itemRect.width() / 2 + 1;
				
				//Each dropped item is selected
				(*createdItem)->setSelected(true);
			}
		}
	
		return true;
	}
	//Drop an item from within the application (storage/history bars, or copying an item inside the 
	//main scene).
	else if (		( mimeData->hasFormat(FROM_STORAGE_DRAG_MIME) )
				||	( mimeData->hasFormat(FROM_SCENE_DRAG_MIME) )
				||	( mimeData->hasFormat(FROM_HISTORY_DRAG_MIME) )
			)
	{
		(*createdItem) = mFactory->buildLanguageItem(mimeData);
/*
		setupNAddItem(*createdItem);
		QRectF itemRect = (*createdItem)->boundingRect();

		if (		( mimeData->hasFormat(FROM_STORAGE_DRAG_MIME) )
				||	( mimeData->hasFormat(FROM_SCENE_DRAG_MIME) )
				||	( mimeData->hasFormat(FROM_HISTORY_DRAG_MIME) )
			)
		{
			(*createdItem)->moveBy( -itemRect.width()/2.0f , -itemRect.height()/2.0f );
		}
*/
		setupNAddNewItem( *createdItem );
		(*createdItem)->moveBy( pos.x() , pos.y() );
		
		unselectAll();
		(*createdItem)->setSelected(true);
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::addFile( const QString &fileName  ,  QLanguageItem** createdItem)
{
	if ( QFile::exists( fileName ) )
	{
		setLastManagedFile( fileName );
		return addLanguageItem( fileName , true , createdItem );
	}
	else
		return false;
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::addCode(const QString &gmnCode , QLanguageItem** createdItem)
{
	return addLanguageItem( gmnCode , false , createdItem );
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::addLanguageItem(const QString &gmnSource, bool sourceIsAFile , QLanguageItem** createdItem)
{
	statusBar()->showMessage( "Loading " + mSettings.mLanguageNameShort + " code..." );
    QApplication::setOverrideCursor(Qt::WaitCursor);

	//Create a new QLanguageItem.
	QLanguageItem * languageItem;
	
	if ( sourceIsAFile )
	{
		languageItem = mFactory->buildLanguageItemFromFile( gmnSource );
		
		if ( !fileControl( languageItem ) )
			return false;
	}
	else
	{
		languageItem = mFactory->buildLanguageItemFromCode( gmnSource );
	}

	setupNAddNewItem( languageItem );

	if ( createdItem != 0 )
		(*createdItem) = languageItem;
	
	return languageItem->isValid();
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::fileControl(QLanguageItem* languageItem)
{
	if ( languageItem->fileNotFound() )
	{
		unselectAll();
		languageItem->setSelected( true );
		
		QMessageBox::StandardButton result = QMessageBox::warning ( this, 
						"File not found", 
						"Can't open file \"" + languageItem->file() + "\". Find it ?", 
						QMessageBox::Yes | QMessageBox::Ignore , 
						QMessageBox::Yes ); 
		if ( result == QMessageBox::Yes )
		{
			QString newFile = findFile( languageItem->file() );
			if ( QFile::exists(newFile) )
			{
				languageItem->loadFile( newFile );
				return true;
			}
		}
		delete languageItem;
		return false;
	}
	return true;
} 

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::setupNAddItem(QLanguageItem* languageItem)
{	
	//Adds the languageItem at its correct place in the mZOrderedItems list.
	//Fill with 0.
	while ( mZOrderedItems.size() <= languageItem->zValue() )
			mZOrderedItems << 0;
	//Replace if the corresponding place is free ( mZOrderedItems[zValue] = 0 ).
	if ( mZOrderedItems[int(languageItem->zValue())] == 0 )
		mZOrderedItems.replace( languageItem->zValue() , languageItem );
	else
	//There is already an item with this ZValue : 
	{
		//insert the item
		mZOrderedItems.insert( languageItem->zValue() , languageItem );
		//Updates all the items zValue acording to their index in mZOrderedItems list.
		updateZOrders();
	}

	languageItem->setFlags( QGraphicsItem::ItemIsSelectable );
	languageItem->setMimeId( FROM_SCENE_DRAG_MIME );

	if ( mSettings.mHasStorage )
	{
		QLanguageItemAction * storeAct = new QLanguageItemAction( languageItem , "Store" );
		connect( storeAct , SIGNAL(triggered(QLanguageItem*)) , this , SLOT(storeItem(QLanguageItem*)) );
	}
	
	connect( languageItem , SIGNAL( saveItemAs() ) , this , SLOT( saveItemAs() ) );
	connect( languageItem , SIGNAL( fileChanged(bool) ) , this , SLOT( fileChanged(bool) ) );
	connect( languageItem , SIGNAL( descriptiveNameChanged() ) , this , SLOT( descriptiveNameChanged() ) );
	connect( languageItem , SIGNAL( itemHasBeenReloaded() ) , this , SLOT( itemHasBeenReloaded() ) );

	connect( languageItem , SIGNAL( bringToFront() )	, this , SLOT( bringToFront() ) );	
	connect( languageItem , SIGNAL( bringForward() )	, this , SLOT( bringForward() ) );	
	connect( languageItem , SIGNAL( sendBackward() )	, this , SLOT( sendBackward() ) );	
	connect( languageItem , SIGNAL( sendToBack() )		, this , SLOT( sendToBack() ) );	
	
	plugResizer( languageItem );

	connect( languageItem , SIGNAL(scaleChanged(float) ) , this , SLOT(scaleChanged( float )) );
	connect( languageItem , SIGNAL(removed()) , this , SLOT( itemRemoved() ) );
	//Add the item and its QLanguageItem to the scene
	mGraphicsScene->addItem( languageItem );
	
	QApplication::restoreOverrideCursor();

	if ( languageItem->isValid() )
	{
		statusBar()->showMessage( GraphicsSceneMainWindow::mSettings.mLanguageName + " Code Ok" );
		addToHistory( languageItem );
	}
	else
	{
		statusBar()->showMessage( languageItem->lastErrorMessage() );
	}
	
	mSceneHasChanged = true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::setupItemFromMenu(QLanguageItem* decorator)
{
	QPointF viewCenter = viewCenterInSceneCoordinate();
	decorator->moveBy( viewCenter.x() , viewCenter.y() );
	unselectAll();
	decorator->setSelected(true);
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::setupNAddNewItem(QLanguageItem* languageItem)
{
	//Reduce the item if too large, if a valid mSettings.mMaxItemSize is defined.
	float widthFactor = 1.0f , heightFactor = 1.0f;
	if ( ( mSettings.mMaxItemSize.height() > 0 ) && ( mSettings.mMaxItemSize.width() > 0 ) )
	{
		if ( languageItem->boundingRect().width() > mSettings.mMaxItemSize.width() )
		{
			widthFactor = mSettings.mMaxItemSize.width() / languageItem->boundingRect().width();
		}
		if ( languageItem->boundingRect().height() > mSettings.mMaxItemSize.height() )
		{
			heightFactor = mSettings.mMaxItemSize.height() / languageItem->boundingRect().height();
		}
		float factor = MIN( widthFactor , heightFactor );
		if ( factor < 1.0f )
		{
			QRectF baseR = languageItem->boundingRect();
			QRectF r( 0,0, baseR.width() * factor , baseR.height() * factor );
			languageItem->resized( r );
		}
	}

	//Set Z on the top.
	languageItem->setZValue( mZOrderedItems.size() );
	setupNAddItem(languageItem);

	//Center the graphicsItem
	QRectF boundingRect = languageItem->sceneBoundingRect();
	languageItem->moveBy( -boundingRect.width() / 2 , -boundingRect.height() / 2 );
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::removeItem(QLanguageItem * item )
{
	if ( !saveItemModificationsDialog(item) )
	{	
		return false;
	}
	
	itemRemoved( item );
	delete item;
	return true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::removeAllItems()
{
	// Delete all the items
	QList<QLanguageItem*> items = languageItems();
	for ( int i = 0 ; i < items.size() ; i++ )
	{
		itemRemoved(items[i]);
		delete items[i];
	}
		
	updateWindowState();
}

//-------------------------------------------------------------------------
// Returns false if cancelled.
bool GraphicsSceneMainWindow::saveItemModificationsDialog(QLanguageItem * item)
{
	bool fileExists = QFile::exists( item->file() );
	if ( ( item->file().length() > 0 ) && ( item->isModified() || !fileExists ) )
	{
		unselectAll();
		item->setSelected( true );

		QString dialogText = ( fileExists ) ?
				item->file() + " has changed. Save modifications ?"
			:	item->file() + " doesn't exist. Re-save item ?"
			;
		QMessageBox::StandardButton result = QMessageBox::warning ( this, 
				"Save item", 
				dialogText, 
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , 
				QMessageBox::Yes ); 

		if ( result == QMessageBox::Yes )
		{
			item->saveItem();
		}
		else if ( result == QMessageBox::No )
		{
			//Do nothing.
		}
		else
		{
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::saveItemAs(QLanguageItem * languageItem)
{
	QString savePath = "";
	if ( languageItem->file().length() > 0 ) 
		savePath = QFileInfo( languageItem->file() ).absolutePath();
	if ( savePath == "" )
		savePath = getFileDialogPath();
		
	savePath += "/" + languageItem->name();

	QString filters =						GMN_FILE_FILTER;

	QString selectedFilter("");
	QString fileName = QFileDialog::getSaveFileName(this, "Save the " + LANGUAGE_NAME_SHORT + " item",
                            savePath,
                            tr(filters.toUtf8().data()) ,
							&selectedFilter);

	if ( fileName.isEmpty() )
		return;

	formatFileName(fileName,selectedFilter,filters);
	setLastManagedFile(fileName);
	
	languageItem->save( fileName );
}

//-------------------------------------------------------------------------
QList<QLanguageItem*> GraphicsSceneMainWindow::selectedLanguageItems()
{
	QList<QGraphicsItem*> selectedItems = mGraphicsScene->selectedItems();
	QList<QLanguageItem*> result;
	for ( int i = 0 ; i < selectedItems.size() ; i++ )
	{
		QLanguageItem* languageItem = dynamic_cast<QLanguageItem*>( selectedItems[i] );
		if ( languageItem )
			result.append( languageItem );
	}
	return result;
}

//-------------------------------------------------------------------------
QList<QLanguageItem*> GraphicsSceneMainWindow::languageItems()
{
	QList<QGraphicsItem*> items = mGraphicsScene->items();
	QList<QLanguageItem*> result;
	for ( int i = 0 ; i < items.size() ; i++ )
	{
		QLanguageItem* languageItem = dynamic_cast<QLanguageItem*>( items[i] );
		if ( languageItem )
			result.append( languageItem );
	}
	return result;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::unselectAll()
{
	while ( mGraphicsScene->selectedItems().size() > 0 )
	{
		mGraphicsScene->selectedItems()[0]->setSelected( false );
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateWindowState()
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();

	if ( selectedItems.size() > 0 )
	{
		// If only one items is selected
		if ( selectedItems.size() == 1 )
		{
			mFirstSelectedItem = selectedItems[0];
//			mFirstSelectedItem->setZValue( mCurrentZOrderItem++ );
			// Activate the text-edit when 1 item is selected only,
			mLanguageTextEdit->setEnabled(true);
			mLanguageTextEdit->setPlainText( mFirstSelectedItem->code() );
			
			mLanguageTextEditDock->setWindowTitle( (mFirstSelectedItem->file().length() > 0 ) ? mFirstSelectedItem->getDescriptiveFileName() : GMN_EDIT_DOCK_NAME );

			// Desactivate the resize action
			mRescaleAct->setEnabled(false);
			mHAlignAct->setEnabled(false);
			mVAlignAct->setEnabled(false);
			// Activate & update the x/y item position edit
			mItemXEdit->setEnabled( true );
			mItemYEdit->setEnabled( true );
			updateItemPositionControls( false );
		}
		else
		// Several items are selected
		{
			if ( ( !mFirstSelectedItem ) || ( !mFirstSelectedItem->isSelected() ) )
				mFirstSelectedItem = selectedItems[0];

			// Desactivate the text-edit
			mLanguageTextEdit->setEnabled(false);
			mLanguageTextEdit->setText("");
			mLanguageTextEditDock->setWindowTitle( GMN_EDIT_DOCK_NAME );
			// Activate the resize/align actions
			mRescaleAct->setEnabled(true);
			mHAlignAct->setEnabled(true);
			mVAlignAct->setEnabled(true);
			// Desactivate the x/y item position edit
			mItemXEdit->setEnabled( false );
			mItemYEdit->setEnabled( false );
		}

		//Activate the scale-slider
		mResizeSlider->setEnabled( true );
		mResizeEdit->setEnabled( true );
		//Value of the scale-slider/edit according to the first item
		float scale = mFirstSelectedItem->currentScale();
		updateScaleControls(scale);
		
		//Activate the copy, remove actions
		mCopyAct->setEnabled( true );
	}
	else 
	{
		mFirstSelectedItem = 0;
		// Desactivate the text-edit
		mLanguageTextEdit->setEnabled(false);
		mLanguageTextEdit->setText("");
		mLanguageTextEditDock->setWindowTitle( GMN_EDIT_DOCK_NAME );
		// Desactivate the scale-slider & resize action
		mResizeSlider->setEnabled( false );
		mResizeEdit->setEnabled( false );
		mResizeSlider->setValue( 0 );
		// Desactivate the x/y position edit
		mItemXEdit->setEnabled( false );
		mItemYEdit->setEnabled( false );
		mRescaleAct->setEnabled(false);
		mHAlignAct->setEnabled(false);
		mVAlignAct->setEnabled(false);
		//Desactivate the copy, remove actions
		mCopyAct->setEnabled( false );
	}
	
	if ( mCopiedItems.size() == 0 )
	{
		//Desactivate paste action
		mPasteAct->setEnabled( false );
	}
	else 
	{
		//Activate paste action
		mPasteAct->setEnabled( true );
	}

	updateRemoveActState();
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateScaleControls(double scale)
{
	int value = ( log10(scale) * SLIDER_PRECISION_FACTOR ) + 0.5f; //+0.5f to round int->float
	disconnect(mResizeSlider , SIGNAL(valueChanged(int)) , this , SLOT(resizeSliderChanged(int)) );
	disconnect(mResizeEdit , SIGNAL(valueChanged(double)) , this , SLOT(scaleItems(double)) );
	mResizeSlider->setValue( value );
	mResizeEdit->setValue( scale );
	connect(mResizeSlider , SIGNAL(valueChanged(int)) , this , SLOT(resizeSliderChanged(int)) );
	connect(mResizeEdit , SIGNAL(valueChanged(double)) , this , SLOT(scaleItems(double)) );
}


/*
*	- Update the values shown in the X/Y position edits in the tool bar.
*	- (connectedToItem == false) means that we update the values displayed by
*		the edits without "back-modifying" the concerned item's position.
*/
//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateItemPositionControls(bool connectedToItem)
{
	if ( !connectedToItem )
	{
		disconnect( mItemXEdit , SIGNAL(valueChanged(double)) , this , SLOT(moveItemX(double)) );
		disconnect( mItemYEdit , SIGNAL(valueChanged(double)) , this , SLOT(moveItemY(double)) );
	}

	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	if ( selectedItems.size() > 0 )
	{
		mItemXEdit->setValue( mFirstSelectedItem->pos().x() );
		mItemYEdit->setValue( mFirstSelectedItem->pos().y() );
	}
	
	if ( !connectedToItem )
	{
		connect( mItemXEdit , SIGNAL(valueChanged(double)) , this , SLOT(moveItemX(double)) );
		connect( mItemYEdit , SIGNAL(valueChanged(double)) , this , SLOT(moveItemY(double)) );
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateCursorPos(QPointF pos)
{
	QString xLabel , yLabel;
	QTextStream xStream(&xLabel);
	xStream.setRealNumberPrecision (5);
	xStream << "X=" << pos.x();
	QTextStream yStream(&yLabel);
	yStream.setRealNumberPrecision (5);
	yStream << "Y=" << pos.y();
				
	mViewXLabel->setText(xLabel);
	mViewYLabel->setText(yLabel);
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateZoomLabel()
{
	mViewScaleLabel->setText( getZoomLabel(mGraphicsView->transform().m11()) );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateRemoveActState()
{
	mRemoveAct->setEnabled( ( mGraphicsScene->selectedItems().size() > 0 ) || ( ( mSettings.mHasStorage ) && ( mStorageGraphicsView->scene()->selectedItems().size() > 0 ) ) );
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::updateZOrders()
{
	for ( int i = 0 ; i < mZOrderedItems.size() ; i++ )
	{
		if ( mZOrderedItems[i] )
			mZOrderedItems[i]->setZValue( i );
	}
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::addToHistory(QLanguageItem* item)
{
	if ( mSettings.mHasHistory )
	{
		if ( item->isValid() )
		{
			if ( !mHistoryGraphicsView->find(item) )
				mHistoryGraphicsView->addItem( mFactory->buildLanguageItem( item ) );
		}
	}
}

//-----------------------------------------------------------------------//	
//						Private UI events handlers						 //
//-----------------------------------------------------------------------//

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::closeEvent(QCloseEvent *event)
{
	if ( !checkAllItems() )
	{
		unselectAll();
		event->ignore();
	}
	else
	{
		if ( mIsWorkspaceModeOn )
		{
			QString currentScene = QSettings().value( CURRENT_SCENE_SETTING ).toString();
/*
		int version = 1;
		while ( QFile::exists( currentScene + "_V" + QVariant(version).toString() ) )
			version++;
*/
			QFileInfo fi(currentScene); 
			QFile::rename( currentScene , fi.absolutePath() + "/~" + fi.fileName() );
			saveSceneFile( currentScene );
		}
		else
		{
			if ( !saveOrCancelDialog() )
			{
				event->ignore();
				return;
			}
		}
		writeSettings();
		event->accept();
	}
}


//-------------------------------------------------------------------------
void putInFile( const QString& msg , const QString& fileName )
{						
	QFile data(fileName);
	if (data.open(QFile::WriteOnly | QFile::Append | QFile::Text )) 
	{
		QTextStream out(&data);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		out << msg << Qt::endl;
#else
		out << msg << endl;
#endif
	}
}

//-------------------------------------------------------------------------
bool GraphicsSceneMainWindow::eventFilter(QObject *obj, QEvent *e)
{
	if (e->type() == QEvent::FileOpen) 
	{
		e->accept();
		QString fileName = ((QFileOpenEvent*)e)->file();
		if ( !loadSceneFile( fileName ) )
			addFile( fileName , 0 );

		return true;
	}
	else if ( e->type() == QEvent::KeyPress )
	{
		if ( ( !mResizeEdit->hasFocus() ) && ( !mItemXEdit->hasFocus() ) && ( !mItemYEdit->hasFocus() ) && ( !mLanguageTextEdit->hasFocus() ) )
		{
			e->setAccepted(false);
			QList<QLanguageItem*> selectedItems = selectedLanguageItems();
			for ( int i = 0 ; i < selectedItems.size() ; i++ )
			{
				selectedItems[i]->keyPressEvent( (QKeyEvent*) e );
			}
			if ( e->isAccepted() )
			{
				return true;
			}
		}
		return QMainWindow::eventFilter(obj, e);
	}
	else 
	{
		if ( e->type() == QEvent::Shortcut )
		{
			QShortcutEvent *keyEvent = (QShortcutEvent*)(e);
			QKeySequence keySequence = QKeySequence::SelectAll;
			if ( (keyEvent->key()) == keySequence )
			{
				if ( mLanguageTextEdit->hasFocus() )
				{
					mLanguageTextEdit->selectAll();
					e->accept();
					return true;
				}
			}
		}
		// standard event processing
		return QMainWindow::eventFilter(obj, e);
	}
}

//-------------------------------------------------------------------------
QMenu * GraphicsSceneMainWindow::createPopupMenu()
{
	QMenu * menu = QMainWindow::createPopupMenu();
	
	if ( mSettings.mHasHistory || mSettings.mHasStorage )
		menu->addSeparator();

	if ( mSettings.mHasHistory )
	{
		QAction * switchHistoryVisible = new QAction( tr("History") , this );
		switchHistoryVisible->setCheckable(true);
		connect( switchHistoryVisible , SIGNAL(triggered()) , this , SLOT(switchHistoryVisible()) );
		switchHistoryVisible->setChecked( mHistoryGraphicsView->isVisible() );
		menu->addAction( switchHistoryVisible );

		if ( !mSwitchHistoryVisible )
		{
			mSwitchHistoryVisible = switchHistoryVisible;
		}
	}

	if ( mSettings.mHasStorage )
	{
		QAction * switchStorageVisible = new QAction( tr("Storage") , this );
		switchStorageVisible->setCheckable(true);
		connect( switchStorageVisible , SIGNAL(triggered()) , this , SLOT(switchStorageVisible()) );
		switchStorageVisible->setChecked( mStorageGraphicsView->isVisible() );
		menu->addAction( switchStorageVisible );
		
		if ( !mSwitchStorageVisible )
		{
			mSwitchStorageVisible = switchStorageVisible;
		}
	}

	return menu;
}

//-------------------------------------------------------------------------
void GraphicsSceneMainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	if (	
			(
				( event->mimeData()->hasText() )
			||  ( event->mimeData()->hasUrls() )
			)
			&& ( !event->mimeData()->hasFormat(FROM_STORAGE_DRAG_MIME) )
			&& ( !event->mimeData()->hasFormat(FROM_HISTORY_DRAG_MIME) )
	)
	{
		event->acceptProposedAction();
	}
}

// Drop in the GraphicsSceneMainWindow space.
// Note : the GuidoGraphicsView has its own dropEvent handler when
// an item is dropped specifically in the view.
//-------------------------------------------------------------------------		
void GraphicsSceneMainWindow::dropEvent(QDropEvent* event)
{
	if (	
			(
				( event->mimeData()->hasText() )
			||  ( event->mimeData()->hasUrls() )
			)
			&& ( !event->mimeData()->hasFormat(FROM_STORAGE_DRAG_MIME) )
			&& ( !event->mimeData()->hasFormat(FROM_HISTORY_DRAG_MIME) )
			&& ( !event->mimeData()->hasFormat(FROM_SCENE_DRAG_MIME) )
	)
	{
		QLanguageItem* createdItem = 0;
	
		//If a new item has been dropped, move to the mouse position
		dropItem( event->mimeData() , viewCenterInSceneCoordinate() , &createdItem );

		event->accept();
	}
	else
	{
		event->ignore();
	}
}

//-------------------------------------------------------------------------
QGraphicsPixmapItem * buildPixmapItem( const QString& fileName )
{
	QGraphicsPixmapItem * i = new QGraphicsPixmapItem( QPixmap( fileName ) );
	i->setTransformationMode( Qt::SmoothTransformation );
	return i;
}

//-------------------------------------------------------------------------
QItemResizer * GraphicsSceneMainWindow::plugResizer( QLanguageItem * itemContainer )
{
	QRectF guidoRect = itemContainer->boundingRect();

	QItemResizer * itemResizer = new QItemResizer( guidoRect , itemContainer );
	itemResizer->setVisible( false );
	itemResizer->setKeepAspectRatio(true);

	itemResizer->setZValue(2);

	itemResizer->setPen( QPen(Qt::black) );
	
	connect( itemResizer , SIGNAL( resized(const QRectF&) ) , itemContainer , SLOT(resized(const QRectF&)) );
	connect( itemContainer , SIGNAL( updateChildrenItemsGeometry(const QRectF&) ) , itemResizer , SLOT(updateGeometry(const QRectF&)) );
	connect( itemContainer , SIGNAL( showResizer(bool) ) , itemResizer , SLOT(show(bool)) );
	
	return itemResizer;
}
