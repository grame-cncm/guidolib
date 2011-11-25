/*
 * MainWindow.cpp
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
#include "MainWindow.h"

#include "QGuidoPainter.h"
#include "QSimpleItemContainer.h"
#include "QGuidoItemContainerFactory.h"
#include "QGuidoItemContainer.h"

#include "MainWindowObjects.h"
#include "QResolutionDialog.h"
#include "GuidoHighlighter.h"
#include "QItemResizer.h"
#include "QPaletteItem.h"

#include <QApplication>
#include <QInputDialog>
#include <QFileDialog>
#include <QMenu>
#include <QToolBar>
#include <QStatusBar>
#include <QKeyEvent>
#include <QTextStream>
#include <QMessageBox>
#include <QStyle>
#include <QComboBox>

#ifdef USES_GUIDO_AR
	#include "QGuidoAR.h"
#endif

#ifdef GUIDO_CALCULUS_APP
#define APP_NAME "GuidoCalculus"
#else
#define APP_NAME "GuidoSceneComposer"
#endif

#include <assert.h>

#define DEFAULT_DISPLAY_TYPE QGuidoItemContainerFactory::TYPE_M_PAGE

#define RESIZER_BORDER_WIDTH 5

#include "QPageManager.h"

//-----------------------------------------------------------------------
MainWindow::MainWindow()
	:	GraphicsSceneMainWindow( new GuidoGraphicsView() , new QGuidoItemContainerFactory() )
{
	//Dynamic plugin load.
	QDir dir(QApplication::applicationDirPath());
	dir.cdUp();
	dir.cd("Plugins");
	QCoreApplication::addLibraryPath( dir.absolutePath());

	mGraphicsView->setRenderHint( QPainter::Antialiasing , true );

	//Guido Engine initialization.
	QGuidoPainter::startGuidoEngine();
	
	mIsWorkspaceModeOn = false;
	GraphicsSceneMainWindow::init();

	mGuidoItemContainerFactory = (QGuidoItemContainerFactory*)mFactory;
	mExportState = None;
	mExportWindow = 0;
	
	mLanguageTextEdit->setLineWrapMode(QTextEdit::NoWrap);
	
#ifdef USES_GUIDO_AR
	mPlayingContainer = 0;

	//Add the QPaletteItem-s
	QPaletteItem * paletteItem;
	//Sum palette
	paletteItem = new QPaletteItem( PALETTE_GUIDO_ELEMENT , 100 , 0);
	paletteItem->addInteraction( buildPixmapItem(RSC_DIR + "combiAfter.png" ) ,			COMBINATION_AFTER );
	paletteItem->addInteraction( buildPixmapItem(RSC_DIR + "combiOverEnd.png" ) ,		COMBINATION_OVER_ALIGN_RIGHT );
	paletteItem->addInteraction( buildPixmapItem(RSC_DIR + "combiOverStart.png" ) ,		COMBINATION_OVER_ALIGN_LEFT );
	paletteItem->addInteraction( buildPixmapItem(RSC_DIR + "combiBefore.png" ) ,		COMBINATION_BEFORE );
	paletteItem->addInteraction( buildPixmapItem(RSC_DIR + "combiUnderStart.png" ) ,	COMBINATION_UNDER_ALIGN_LEFT );
	paletteItem->addInteraction( buildPixmapItem(RSC_DIR + "combiUnderEnd.png" ) ,		COMBINATION_UNDER_ALIGN_RIGHT );
	mPalettes << paletteItem;
	//Selection palette
	paletteItem = new QPaletteItem( PALETTE_GUIDO_ELEMENT , 100 , 0);
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Tail") ,			COMBINATION_TAIL );
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Voice Head") ,	COMBINATION_VOICE_HEAD );
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Head") ,			COMBINATION_HEAD );
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Voice tail") ,	COMBINATION_VOICE_TAIL );
	mPalettes << paletteItem;
	//Selection palette
	paletteItem = new QPaletteItem( PALETTE_GUIDO_ELEMENT , 100 , 0);
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Transpose") ,		COMBINATION_TRANSPOSE );
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Stretch") ,		COMBINATION_DURATION );
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Rythm") ,			COMBINATION_RYTHM );
	paletteItem->addInteraction( new QGraphicsSimpleTextItem("Note") ,			COMBINATION_PITCH );
	mPalettes << paletteItem;

	for ( int i = 0 ; i < mPalettes.size() ; i++ )
	{
		//Set each palette pen & brush
		mPalettes[i]->setBrush( HIGHLIGHTED_BRUSH );
		QPen palettePen( HIGHLIGHTED_PEN );
		palettePen.setWidth( palettePen.width() * 2 );
		mPalettes[i]->setPen( palettePen );
		mGraphicsScene->addItem( mPalettes[i] );
		
		//Desactivate each palette (its activation will be done according to the palette combobox selected item)
		mPalettes[i]->setPaletteEnabled( false );
		
		//Palette's positioning policy
		mPalettes[i]->setPositioningPolicy(QPaletteItem::CENTER_AND_RESIZE_POLICY);
	}
	
	QPaletteManager::instance()->setActivationDelay(500);
	connect( QPaletteManager::instance() , SIGNAL(interactionActivated(QGraphicsItem*,QGraphicsItem*,int,int,int)) , this , SLOT( combineItems(QGraphicsItem*,QGraphicsItem*,int,int,int) ) );
	changeActivePalette(0);
#endif

	adaptActions();
	
	mGuidoHighlighter = 0;
}

//-----------------------------------------------------------------------
MainWindow::~MainWindow()
{
	QGuidoPainter::stopGuidoEngine();
}

//-----------------------------------------------------------------------
void MainWindow::adaptActions()
{
	mAddAct->setIcon(QIcon(RSC_DIR + QString("addGuido.png")));
	mAddAct->setText( tr("Add Guido item") );
	mAddAct->setStatusTip(tr("Add a Guido item to the scene"));
	mAddFromFileAct->setIcon(QIcon(RSC_DIR + QString("addFromFileGuido.png")));
//	mAddFromFileAct->setStatusTip(tr("Add a new GMN graphics item from a GMN file"));
	mRemoveAct->setIcon(QIcon(RSC_DIR + QString("removeItem.png")));
//	mRemoveAct->setStatusTip(tr("Remove the selected GMN graphics items"));
//	mCopyAct->setStatusTip(tr("Copy the selected GMN graphics items"));
}
		
//-----------------------------------------------------------------------//	
//						Private slots									 //
//-----------------------------------------------------------------------//

#define EXPORT_SELECTED_ITEMS "Export selected item(s)"
#define EXPORT_THE_SCENE "Export scene"
//-------------------------------------------------------------------------
void MainWindow::exportToImage()
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();

	if ( mExportWindow )
		finalizeExport(false);
	else  if ( selectedItems.size() > 0 )
	{
		QStringList comboboxItems;
		comboboxItems << tr(EXPORT_SELECTED_ITEMS) << tr(EXPORT_THE_SCENE);

		bool ok;
		QString item = QInputDialog::getItem(this, tr("Export to image ..."),
                                          tr("What would you like to export ?"), comboboxItems, 0, false, &ok);
		if (ok && !item.isEmpty())
		{
			if ( item == EXPORT_SELECTED_ITEMS )
			{
				if ( selectedItems.size() == 1 )
					exportFirstItem();
				else
				{
					QRectF exportRect;
					int i;
					QList<QLanguageItem*> containers = languageItems();
					for ( i = 0 ; i < containers.size() ; i++ )
					{
						if ( !containers[i]->isSelected() )
							containers[i]->setVisible(false);
						else
							exportRect |= containers[i]->sceneBoundingRect();
					}

					exportRectDialog( exportRect );

					for ( i = 0 ; i < containers.size() ; i++ )
					{
						if ( !containers[i]->isSelected() )
							containers[i]->setVisible(true);
					}
				}
				
			}
			else if ( item == EXPORT_THE_SCENE )
			{
				exportRectDialog( mBackgroundItem->pageRectInSceneCoords() );
			}
		}
	}
	else
		exportRectDialog( mBackgroundItem->pageRectInSceneCoords() );
}

//-------------------------------------------------------------------------
void MainWindow::about()
{
    QMessageBox::about(this, tr(QString("About " + QString(APP_NAME)).toAscii().data()),
             tr(QString("The <b>"+ QString(APP_NAME)+"</b> allows you to compose a graphics scene " + 
                "made of GUIDO Scores.").toAscii().data()));
}

//-------------------------------------------------------------------------
void MainWindow::itemRemoved( QLanguageItem * item )
{
	GraphicsSceneMainWindow::itemRemoved( item );
#ifdef USES_GUIDO_AR
	controlPlayingContainer( item );
#endif
}

//-------------------------------------------------------------------------
void MainWindow::addTextItem()
{
	QLanguageItem* languageItem = mGuidoItemContainerFactory->buildSimpleTextItemContainer();
	languageItem->setCode( "A text item" );
	setupNAddNewItem( languageItem );
	setupItemFromMenu( languageItem );
	
}

//-------------------------------------------------------------------------
void MainWindow::addItemFromAnyFile()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, QString("Add an item from a file"),
        getFileDialogPath(),
        tr("All (*.*)"));

	// Create the item with the file
	QLanguageItem* createdItem = 0;
	// Add & setup the created item
	if ( addFile( fileName , &createdItem ) )
		setupItemFromMenu( createdItem );
}

#define A4_LITTLE	210
#define A4_BIG		297
//-------------------------------------------------------------------------
void MainWindow::setPageA4Portrait()
{
	mBackgroundItem->setPageSize(QSizeF( A4_LITTLE , A4_BIG ));
	mBackgroundItem->setResizeEnabled( false );
}

//-------------------------------------------------------------------------
void MainWindow::setPageA3Portrait()
{
	mBackgroundItem->setPageSize(QSizeF( 2*A4_LITTLE , 2*A4_BIG ));
	mBackgroundItem->setResizeEnabled( false );
}

//-------------------------------------------------------------------------
void MainWindow::setPageA4Landscape()
{
	mBackgroundItem->setPageSize(QSizeF( A4_BIG , A4_LITTLE ));
	mBackgroundItem->setResizeEnabled( false );
}

//-------------------------------------------------------------------------
void MainWindow::setPageA3Landscape()
{
	mBackgroundItem->setPageSize(QSizeF( 2*A4_BIG , 2*A4_LITTLE ));
	mBackgroundItem->setResizeEnabled( false );
}

#ifdef USES_GUIDO_AR
//-------------------------------------------------------------------------
void MainWindow::playItem()
{
	if ( !dynamic_cast<QGuidoItemContainer*>(mFirstSelectedItem) )
		return;
	
	if ( mPlayingContainer != mFirstSelectedItem )
	{
		// An item has already been played, without being explicitely stopped.
		stopItem();
	}
	
	((QGuidoItemContainer*)mFirstSelectedItem)->play();

	mPlayingContainer = mFirstSelectedItem;
	
	mPauseAct->setEnabled( true );
	mStopAct->setEnabled( true );
}

//-------------------------------------------------------------------------
void MainWindow::pauseItem()
{
	if ( mPlayingContainer )
	{
		((QGuidoItemContainer*)mPlayingContainer)->pause();
		mPauseAct->setEnabled( false );
	}
}

//-------------------------------------------------------------------------
void MainWindow::stopItem()
{
	if ( mPlayingContainer )
	{
		((QGuidoItemContainer*)mPlayingContainer)->stop();
		mPlayingContainer = 0;
		mPauseAct->setEnabled( false );
		mStopAct->setEnabled( false );
	}
}

//-------------------------------------------------------------------------
void MainWindow::exportItemToMidi()
{
	QLanguageItem * container = dynamic_cast<QLanguageItem *>( sender() );
	assert(container);

	QString savePath;
	if ( container->file().length() )
		savePath = QFileInfo( container->file() ).absolutePath();
	if ( savePath == "" )	
		savePath = windowFilePath();
	if ( savePath == "" )
		savePath = QDir::homePath();

	savePath += "/" + container->name();

	QString filters = MIDI_FILE_FILTER;

	QString selectedFilter("");
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export item"),
                            savePath,
                            tr(filters.toAscii().data()) ,
							&selectedFilter);

	if ( fileName.isEmpty() )
		return;

	formatFileName(fileName,selectedFilter,filters);

	QGuidoAR::midiExport(container->code() , fileName );
}

//-------------------------------------------------------------------------
void MainWindow::combineItems( QGraphicsItem * droppedItem , QGraphicsItem* targetItem , int,int, int interactionId )
{
	QGuidoItemContainer * itemA = dynamic_cast<QGuidoItemContainer *>(targetItem);
	QGuidoItemContainer * itemB = dynamic_cast<QGuidoItemContainer *>(droppedItem);
	if ( !itemA || !itemB )
		return;

	//Asking for building a new QGuidoItem or QGuidoSPageItem (depending on "isPageMode" parameter)
	QString gmnCode;
	switch ( interactionId )
	{
		case COMBINATION_UNDER_ALIGN_LEFT :
			gmnCode = QGuidoAR::parallel( itemA->code() , itemB->code() , QGuidoAR::Left );
			break;
		case COMBINATION_UNDER_ALIGN_RIGHT :
			gmnCode = QGuidoAR::parallel( itemA->code() , itemB->code() , QGuidoAR::Right );
			break;
		case COMBINATION_OVER_ALIGN_LEFT :
			gmnCode = QGuidoAR::parallel( itemB->code() , itemA->code() , QGuidoAR::Left );
			break;
		case COMBINATION_OVER_ALIGN_RIGHT :
			gmnCode = QGuidoAR::parallel( itemB->code() , itemA->code() , QGuidoAR::Right );
			break;
		case COMBINATION_BEFORE :
			gmnCode = QGuidoAR::sequence( itemB->code() , itemA->code() );
			break;
		case COMBINATION_AFTER :
			gmnCode = QGuidoAR::sequence( itemA->code() , itemB->code() );
			break;
		case COMBINATION_HEAD :
			gmnCode = QGuidoAR::head( itemA->code() , itemB->code() );
			break;
		case COMBINATION_TAIL :
			gmnCode = QGuidoAR::tail( itemA->code() , itemB->code() );
			break;
		case COMBINATION_VOICE_HEAD :
			gmnCode = QGuidoAR::top( itemA->code() , itemB->code() );
			break;
		case COMBINATION_VOICE_TAIL :
			gmnCode = QGuidoAR::bottom( itemA->code() , itemB->code() );
			break;
		case COMBINATION_INTERLEAVE :
			gmnCode = QGuidoAR::interleave( itemA->code() , itemB->code() );
			break;
		case COMBINATION_TRANSPOSE :
			gmnCode = QGuidoAR::transpose( itemA->code() , itemB->code() );
			break;
		case COMBINATION_DURATION :
			gmnCode = QGuidoAR::duration( itemA->code() , itemB->code() );
			break;
		case COMBINATION_RYTHM :
			gmnCode = QGuidoAR::rythm( itemA->code() , itemB->code() );
			break;
		case COMBINATION_PITCH :
			gmnCode = QGuidoAR::pitch( itemA->code() , itemB->code() );
			break;
		default:
			return;
	}

	//Set the code of the target item with the code computed by GuidoAR
	itemA->setCode( gmnCode );

	//It's a fusion of 2 items, so we remove the dropped item (only if it was in the main scene: history&storage items are not affected).
	if ( itemB->scene() == mGraphicsScene )
	{
		itemB->deleteLater();
		itemRemoved( itemB );
	}
	unselectAll();
	itemA->setSelected( true );
	
	//Don't forget to call QGraphicsSceneMainWindow::addToHistory, because we changed the code
	GraphicsSceneMainWindow::addToHistory(itemA);
}

//-------------------------------------------------------------------------
void MainWindow::changeActivePalette(int index)
{
	for ( int i = 0 ; i < mPalettes.size() ; i++ )
	{
		mPalettes[i]->setPaletteEnabled( i == index );
	}
}

#endif

//-------------------------------------------------------------------------
void MainWindow::exportItem()
{
	QGuidoItemContainer * item = (QGuidoItemContainer *)sender();
	exportItem( item );
}

//-----------------------------------------------------------------------//	
//						Private functions								 //
//-----------------------------------------------------------------------//

//-------------------------------------------------------------------------
void MainWindow::createActions()
{
	mSettings.mDesactiveUIElements << ADD_FROM_FILE_ACT;

	GraphicsSceneMainWindow::createActions();

	mExportAct = new QAction(QIcon(RSC_DIR + QString("export.png")), tr("&Export...") , this);
	mExportAct->setShortcut( tr("Ctrl+E") );
	mExportAct->setStatusTip(tr("Export the scene, an item, or use Shift+Drag&Drop to specify an export-window"));
	connect(mExportAct , SIGNAL(triggered()) , this , SLOT(exportToImage()) );
	
	mAddTextAct = new QAction(QIcon(RSC_DIR + QString("addText.png")), tr("Add text item") , this);
	mAddTextAct->setStatusTip(tr("Add a text item to the scene"));
	connect(mAddTextAct , SIGNAL(triggered()) , this , SLOT(addTextItem()) );
	
	mAddFromAnyFileAct = new QAction(QIcon(RSC_DIR + QString("addFromAnyFile.png")), tr("Add item from file...") , this);
	mAddFromAnyFileAct->setStatusTip("Add a new item from a file");
	connect(mAddFromAnyFileAct , SIGNAL(triggered()) , this , SLOT(addItemFromAnyFile()) );

	mSetPageA4PortraitAct = new QAction(tr("A4 Portrait") , this);
	connect(mSetPageA4PortraitAct , SIGNAL(triggered()) , this , SLOT(setPageA4Portrait()) );
	mSetPageA3PortraitAct = new QAction(tr("A3 Portrait") , this);
	connect(mSetPageA3PortraitAct , SIGNAL(triggered()) , this , SLOT(setPageA3Portrait()) );
	mSetPageA4LandscapeAct = new QAction(tr("A4 Landscape") , this);
	connect(mSetPageA4LandscapeAct , SIGNAL(triggered()) , this , SLOT(setPageA4Landscape()) );
	mSetPageA3LandscapeAct = new QAction(tr("A3 Landscape") , this);
	connect(mSetPageA3LandscapeAct , SIGNAL(triggered()) , this , SLOT(setPageA3Landscape()) );

#ifdef USES_GUIDO_AR
	mPlayAct = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
	mPlayAct->setStatusTip(tr("Play selected item"));
	connect( mPlayAct , SIGNAL(triggered()) , this , SLOT(playItem()) );
	mPauseAct = new QAction(style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
	mPauseAct->setStatusTip(tr("Pause selected item"));
	connect( mPauseAct , SIGNAL(triggered()) , this , SLOT(pauseItem()) );
	mStopAct = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
	mStopAct->setStatusTip(tr("Stop selected item"));
	connect( mStopAct , SIGNAL(triggered()) , this , SLOT(stopItem()) );
#endif
}

//-------------------------------------------------------------------------
void MainWindow::createMenus()
{
	GraphicsSceneMainWindow::createMenus();
	
	mFileMenu->addAction(mExportAct);
	
	mEditMenu->addSeparator();
	QMenu * menu = mEditMenu->addMenu("Page format");
	menu->addAction( mSetPageA4PortraitAct );
	menu->addAction( mSetPageA3PortraitAct );
	menu->addSeparator();
	menu->addAction( mSetPageA4LandscapeAct );
	menu->addAction( mSetPageA3LandscapeAct );	

	mEditMenu->insertAction(mRemoveAct , mAddFromAnyFileAct);
	mEditMenu->insertAction(mAddAct , mAddTextAct);
}

//-------------------------------------------------------------------------
void MainWindow::createToolBars()
{
	GraphicsSceneMainWindow::createToolBars();

	mFileToolBar->addAction(mExportAct);
	
	mEditToolBar->insertAction(mRemoveAct , mAddFromAnyFileAct);
	mEditToolBar->insertAction(mAddAct , mAddTextAct);
	
#ifdef USES_GUIDO_AR
	mPlayToolBar = addToolBar(tr("MIDI play"));
	mPlayToolBar->addAction( mPlayAct );
	mPlayToolBar->addAction( mPauseAct );
	mPlayToolBar->addAction( mStopAct );
	mPlayToolBar->setObjectName( "PlayToolBar" );

	mCombinationToolBar = addToolBar(tr("&Combinations"));
	mCombinationToolBar->setObjectName("CombinationsToolBar");
	QComboBox * paletteBox = new QComboBox();
	mCombinationToolBar->addWidget(paletteBox);
	paletteBox->addItem( "Sum palette" );
	paletteBox->addItem( "Selection palette" );
	paletteBox->addItem( "Complex palette" );
	connect( paletteBox , SIGNAL( currentIndexChanged(int) ) , this , SLOT( changeActivePalette(int) ) );
#endif
}

//-------------------------------------------------------------------------
void MainWindow::setupNAddItem(QLanguageItem* guidoItemContainer)
{
	GraphicsSceneMainWindow::setupNAddItem( guidoItemContainer );

	if ( dynamic_cast<QGuidoItemContainer*> (guidoItemContainer) )
	{
		connect( guidoItemContainer , SIGNAL(exportItem()) , this, SLOT(exportItem()) );
		((QGuidoItemContainer*)guidoItemContainer)->setMinMaxScale( mSettings.mMinScale , mSettings.mMaxScale );
	}
}

//-------------------------------------------------------------------------
void MainWindow::updateWindowState()
{
	GraphicsSceneMainWindow::updateWindowState();
#ifdef USES_GUIDO_AR
	mPlayAct->setEnabled( ( selectedLanguageItems().size() == 1 ) && (dynamic_cast<QGuidoItemContainer*>(mFirstSelectedItem)) );
	mPauseAct->setEnabled( mPlayingContainer );
	mStopAct->setEnabled( mPlayingContainer );
#endif

	// Adds a Guido syntax highlighter on the textEdit if the only selected item is a QGuidoItemContainer.
	if ( ( selectedLanguageItems().size() == 1 ) && (dynamic_cast<QGuidoItemContainer*>(mFirstSelectedItem)) )
	{
		// Adds the Guido syntax highlighter if there wasn't already one.
		if ( !mGuidoHighlighter )
			mGuidoHighlighter = new GuidoHighlighter( mLanguageTextEdit->document() );
	}
	else
	{
		// Destroys the Guido syntax highlighter if the only selected item is not a QGuidoItemContainer.
		if ( ( selectedLanguageItems().size() == 1 ) && !(dynamic_cast<QGuidoItemContainer*>(mFirstSelectedItem)) )
		{
			if ( mGuidoHighlighter )
			{ 
				delete mGuidoHighlighter;
				mGuidoHighlighter = 0;
			}
		}
	}

}

#ifdef USES_GUIDO_AR
//-------------------------------------------------------------------------
void MainWindow::controlPlayingContainer( QGraphicsItem * container )
{
	if ( mPlayingContainer == container )
		mPlayingContainer = 0;
}
#endif

//-------------------------------------------------------------------------
void MainWindow::keyPressEvent( QKeyEvent * event )
{
	if ( mLanguageTextEdit->hasFocus() )
		return;

	int key = event->key();
	if ( key == Qt::Key_Shift )
	{
		if ( selectedLanguageItems().size() == 0 )
		{
			//Cancel any previous export process.
			finalizeExport(true);
			//Start the export process. 
			//Go in first state of export process.
			mExportState = WaitRectStart;
	
			unselectAll();
			QApplication::setOverrideCursor( Qt::CrossCursor );
			event->accept();
		}
	}
	else if ( key == Qt::Key_Escape )
	{
		finalizeExport(true);
		event->accept();
	}
}

//-------------------------------------------------------------------------
void MainWindow::keyReleaseEvent ( QKeyEvent * event )
{
	if ( mLanguageTextEdit->hasFocus() )
		return;
		
	if ( event->key() == Qt::Key_Shift )
	{
		if ( mExportState == WaitRectEnd )
		{
			QApplication::restoreOverrideCursor();
			mExportState = WaitValidation;
			event->accept();
		}
		else if ( mExportState == WaitRectStart )
		{
			finalizeExport(true);
			event->accept();
		}
	}
}


//-------------------------------------------------------------------------
void MainWindow::finalizeExport(bool cancel)
{
	mExportState = None;

	QApplication::restoreOverrideCursor();
	if ( mExportWindow )
	{
		QRectF exportRectangle = mExportWindow->rect();
		mGraphicsScene->removeItem( mExportWindow );
		delete mExportWindow;
		mExportWindow = 0;
		//Export the view only in case of a validation
		if ( !cancel )
		{
			exportRectDialog( exportRectangle );
			statusBar()->showMessage("Export done");
		}
		else
			statusBar()->showMessage("Export cancelled");
	}
}

//-------------------------------------------------------------------------
void MainWindow::exportRectDialog(const QRectF& exportRectangle )
{
	//Select filename & export type
	QString savePath = getFileDialogPath();
	if ( savePath == "" )
		savePath = windowFilePath();
	savePath += "/" + QFileInfo(windowFilePath()).fileName();

	QString filters =	PNG_FILE_FILTER 
					+	QString("\n") +	PDF_FILE_FILTER
//					+	QString("\n") +	PS_FILE_FILTER
					;
	QString selectedFilter("");
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export the scene"),
							savePath,
							tr(filters.toAscii().data()) ,
							&selectedFilter);

	if ( fileName.isEmpty() )
		return;
	
	setLastManagedFile(fileName);
	formatFileName(fileName,selectedFilter,filters);

	exportRect( exportRectangle , fileName , selectedFilter );
}

//-------------------------------------------------------------------------
void MainWindow::exportRect(const QRectF& exportRectangle , const QString& fileName , const QString& fileType )
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	unselectAll();

	QPainter painter;
	if ( fileType == PNG_FILE_FILTER )
	{
		int result = QResolutionDialog( exportRectangle , EXPORT_MIN_DETAIL , EXPORT_MAX_DETAIL , EXPORT_DEFAULT_DETAIL , EXPORT_DETAIL_TO_SCALE_FACTOR ).exec();
		if ( result )
		{
			//Create a new QImage, with a size corresponding to the Guido Score page
			QSizeF size( exportRectangle.width() , exportRectangle.height() );
			size.setWidth( result * EXPORT_DETAIL_TO_SCALE_FACTOR * size.width() );
			size.setHeight( result * EXPORT_DETAIL_TO_SCALE_FACTOR * size.height() );
			QImage image( size.width() , size.height() , QImage::Format_ARGB32);
			image.fill( QColor(Qt::white).rgb() );
			
			painter.begin( &image );

			//Paint in the QImage
			paintSceneRect( exportRectangle , &painter );

			painter.end();

			image.save( fileName , "PNG" );
		}
	}
	else if ( ( fileType == PDF_FILE_FILTER ) || ( fileType == PS_FILE_FILTER ) )
	{
		QPrinter printer;
		printer.setFullPage(true);
		printer.setOutputFileName( fileName );
		if ( fileType == PS_FILE_FILTER )
		{
			printer.setOutputFormat( QPrinter::PostScriptFormat );
		}
		else if ( fileType == PDF_FILE_FILTER )
		{
			printer.setOutputFormat( QPrinter::PdfFormat );
		}

		printer.setPaperSize( QSizeF( exportRectangle.width() , exportRectangle.height() ) , QPrinter::Millimeter );
		painter.setWindow( exportRectangle.toRect() );
		
		painter.begin( &printer );

		paintSceneRect( exportRectangle , &painter );
		
		painter.end();
	}

	for ( int i = 0 ; i < selectedItems.size() ; i++ )
		selectedItems[i]->setSelected(true);
}

//-------------------------------------------------------------------------
void MainWindow::paintSceneRect( const QRectF& exportRectangle , QPainter * painter )
{	
	// print, fitting the viewport contents into a full page
	QRectF target(0,0,painter->device()->width() , painter->device()->height() );

#if !linux
	QList<QLanguageItem*> containers = languageItems();
	for ( int i = 0 ; i < containers.size() ; i++ )
	{
		if ( dynamic_cast<QGuidoItemContainer*>(containers[i]) )
			((QGuidoItemContainer*)containers[i])->guidoItem()->setCacheMode( QGraphicsItem::NoCache );
		else if ( dynamic_cast<QSimpleItemContainer*>(containers[i]) )
			((QSimpleItemContainer*)containers[i])->containedItem()->setCacheMode( QGraphicsItem::NoCache );
	}
#endif
	mGraphicsScene->render( painter , target , exportRectangle );
#if !linux
	for ( int i = 0 ; i < containers.size() ; i++ )
	{
		if ( dynamic_cast<QGuidoItemContainer*>(containers[i]) )
			((QGuidoItemContainer*)containers[i])->guidoItem()->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
		else if ( dynamic_cast<QSimpleItemContainer*>(containers[i]) )
			((QSimpleItemContainer*)containers[i])->containedItem()->setCacheMode( QGraphicsItem::DeviceCoordinateCache );
	}
#endif
}

//-------------------------------------------------------------------------
void MainWindow::exportFirstItem()
{
	QList<QLanguageItem*> selectedItems = selectedLanguageItems();
	if ( selectedItems.size() > 0 )
	{
		//Export the first (and only) selected item, if it is a QGuidoItemContainer.
		//(do nothing for other types of items)
		if ( dynamic_cast<QGuidoItemContainer*>(mFirstSelectedItem) )
			exportItem( (QGuidoItemContainer*)mFirstSelectedItem );
		else
			QMessageBox::warning( this , "Export item" , "Item not exported : only Guido items can be exported."  );
	}
}

//-------------------------------------------------------------------------
void MainWindow::exportItem(QGuidoItemContainer * item)
{
	//Select filename & export type
	QString savePath;
	if ( item->file().length() )
		savePath = QFileInfo( item->file() ).absolutePath();
	if ( savePath == "" )	
		savePath = getFileDialogPath();

	savePath += "/" + item->name();

	QString filters =	PNG_FILE_FILTER 
					+	QString("\n") +	PDF_FILE_FILTER
//					+	QString("\n") +	GMN_FILE_FILTER
//						+	QString("\n") +	PS_FILE_FILTER
					;
	QString selectedFilter("");
	QString fileName = QFileDialog::getSaveFileName(this, "Export the selected " + GraphicsSceneMainWindow::applicationSettings().mLanguageNameShort + " item",
							savePath,
							tr(filters.toAscii().data()) ,
							&selectedFilter);

	if ( fileName.isEmpty() )
		return;
		
	formatFileName(fileName,selectedFilter,filters);
	setLastManagedFile(fileName );


	if ( selectedFilter == PNG_FILE_FILTER )
	{
		int result = QResolutionDialog( item->guidoItem()->boundingRect() , EXPORT_MIN_DETAIL , EXPORT_MAX_DETAIL , EXPORT_DEFAULT_DETAIL , EXPORT_DETAIL_TO_SCALE_FACTOR ).exec();
		if ( result )
			itemToImage( item->guidoItem() , result * EXPORT_DETAIL_TO_SCALE_FACTOR , QColor(Qt::white).rgb() ).save( fileName );

		return;
	}
	
	if ( selectedFilter == PDF_FILE_FILTER )
	{
		item->exportToPdf( fileName );
		return;
	}

	assert(0);
}

//-------------------------------------------------------------------------
QItemResizer* MainWindow::plugResizer(QLanguageItem * itemContainer)
{
	QItemResizer * resizer = GraphicsSceneMainWindow::plugResizer(itemContainer);

	if ( dynamic_cast<QSimpleItemContainer*>(itemContainer) )
	{
		resizer->setPen( QPen( QColor( SIMPLE_ITEM_BASE_RGB,SIMPLE_ITEM_ALPHA_1) ) );
		resizer->setBrush( QBrush( QColor(SIMPLE_ITEM_BASE_RGB,SIMPLE_ITEM_ALPHA_2) ) );
		resizer->setResizeBorder( RESIZER_BORDER_WIDTH , Qt::NoBrush );
		resizer->setKeepAspectRatio(false);
	}
	else
	{
//		resizer->setPen( SELECTED_PEN );
//		resizer->setBrush( HIGHLIGHTED_BRUSH );
		resizer->setRoundedRectRadius( ROUNDED_RECT_RADIUS );
		resizer->setResizeBorder( RESIZER_BORDER_WIDTH , Qt::NoBrush );
		
		assert( dynamic_cast<QGuidoItemContainer*>(itemContainer) );
		QGuidoItemContainer* guidoContainer = (QGuidoItemContainer*)itemContainer;
		guidoContainer->setResizer( resizer );
	}
	
	return resizer;
}

//-------------------------------------------------------------------------
void GuidoGraphicsView::mousePressEvent ( QMouseEvent * event )
{
	MainWindow* mainWindow = (MainWindow*)mMainWindow;
	//If in first state of export process (WaitRectStart),
	//anchor the top-left point of the export rectangle
	//Go in second state of export process (WaitRectEnd).
	if ( mainWindow->mExportState == MainWindow::WaitRectStart )
	{
		event->accept();
		QPointF graphicsViewPos = mainWindow->mGraphicsView->mapToScene( event->pos() );

		QRectF rect( graphicsViewPos.x() , graphicsViewPos.y() , 0 , 0 );
		mainWindow->mExportWindowStartPoint = graphicsViewPos;
		mainWindow->mExportWindow = mainWindow->mGraphicsScene->addRect( rect , QPen( Qt::DashLine ) , QBrush( QColor(230,230,230,50) ) );		
		
		mainWindow->mExportState = MainWindow::WaitRectEnd;
	}
	else
	{
		// Cancel any export-selection
		if ( mainWindow->mExportState == MainWindow::WaitValidation )
		{
			mainWindow->finalizeExport(true);
		}
		LanguageGraphicsView::mousePressEvent(event);
	}
}

//-------------------------------------------------------------------------
void GuidoGraphicsView::mouseMoveEvent( QMouseEvent * event )
{
	MainWindow* mainWindow = (MainWindow*)mMainWindow;
	//If in second state of the export process (WaitRectEnd),
	//update the bottom-right point of the export rectangle
	//with the current mouse position.
	if ( mainWindow->mExportState == MainWindow::WaitRectEnd )
	{
		event->accept();
		
		QPointF graphicsViewPos = mainWindow->mGraphicsView->mapToScene( event->pos() );

		QRectF exportRect( mainWindow->mExportWindowStartPoint.x() , mainWindow->mExportWindowStartPoint.y() , graphicsViewPos.x() - mainWindow->mExportWindowStartPoint.x() , graphicsViewPos.y() - mainWindow->mExportWindowStartPoint.y());
		
		mainWindow->mExportWindow->setRect( exportRect & mainWindow->mBackgroundItem->pageRectInSceneCoords() );
		
		QString statusMsg;
		QTextStream(&statusMsg) << "Exported rectangle size = " << mainWindow->mExportWindow->rect().width() << "x" << mainWindow->mExportWindow->rect().height();
		mainWindow->statusBar()->showMessage( statusMsg );
	}
	else
	{
		QGraphicsView::mouseMoveEvent(event);
	}
	QPointF posSceneCoords = mainWindow->mGraphicsView->mapToScene( event->pos() );
	mainWindow->updateCursorPos( posSceneCoords );
}

