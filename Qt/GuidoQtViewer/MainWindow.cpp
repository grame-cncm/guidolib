/*
 * MainWindow.cpp
 *
 * Created by Christophe Daudin on 17/04/08.
 * Copyright 2008 Grame. All rights reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.

 * Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 * research@grame.fr
 */
 
#include <sstream>

#include "MainWindow.h"

#include "QGuidoWidget.h"
#include "QGuidoPainter.h"
#include "SetupDialog.h"
#include "QGuidoImporter.h"

#include <QtGui>
#include <QPrintDialog>
#include <QPrinter>
#include <QDockWidget>
#include <QVariant>
#include <QImage>
#include <QProgressBar>
#include <QDir>
#include <QScrollBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>
using namespace std;

#define APP_NAME QString("GuidoQtViewer")
#define COMPANY_NAME QString("GRAME") 

#define ZOOM_FACTOR 1.5f
#define DEFAULT_WINDOW_WIDTH  800
#define DEFAULT_WINDOW_HEIGHT 600
#define MAXIMUM_ZOOM 5.0f
#define HORIZONTAL_BORDER_MARGIN	50
#define VERTICAL_BORDER_MARGIN		50


//-------------------------------------------------------------------------
class DragScrollArea : public QScrollArea 
{

	public:
		DragScrollArea(QWidget * parent) : QScrollArea( parent ) {}

	protected:
	
		QPoint mPreviousPoint;
	
		void moveScrollBar(int pixelDelta , QScrollBar * scrollBar)
		{
			scrollBar->setValue( scrollBar->value() - pixelDelta );
		}
	
		void mouseMoveEvent ( QMouseEvent * event )
		{
			if ( widget() )
			{
				event->accept();
				if ( !mPreviousPoint.isNull() )
				{
					int dx = event->pos().x() - mPreviousPoint.x();
					int dy = event->pos().y() - mPreviousPoint.y();				
					moveScrollBar( dx , horizontalScrollBar() );
					moveScrollBar( dy , verticalScrollBar() );
				}
				mPreviousPoint = event->pos();
			}
		}
		
		void mouseReleaseEvent ( QMouseEvent * event )
		{
			if ( widget() )
			{
				event->accept();
				widget()->setCursor( QCursor( Qt::OpenHandCursor ) );
				mPreviousPoint.setX(0);
				mPreviousPoint.setY(0);
			}
		}

		void mousePressEvent ( QMouseEvent * event )
		{
			if ( widget() )
			{
				event->accept();
				widget()->setCursor( QCursor( Qt::ClosedHandCursor ) );
				mPreviousPoint = event->pos();
			}
		}
		
};


//-------------------------------------------------------------------------
void MapGuidoWidget::Graph2TimeMap (const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos)
{
	cout << "map collection: " << box << " dates: " << dates << " type: " << infos.type << endl;
	QPainter paint(this);
	paint.setBrush( QBrush( QColor( 0 , 0, 255 , 100 ) ) );
//if (duration)
	paint.drawRect( QRectF( box.left , box.top , box.right - box.left , box.bottom - box.top ) );
}

//-------------------------------------------------------------------------
void MapGuidoWidget::paintEvent(QPaintEvent * event)
{
	QGuidoWidget::paintEvent (event);
	int page = firstVisiblePage();
//	if (fMap & kNewMap) {
		GuidoErrCode err = guidoNoErr;
		if (fMap & kPageBB) {
			err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoPage, *this);
			if (err != guidoNoErr) cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		}
		if (fMap & kSystemsBB) {
			err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoSystem, *this);
			if (err != guidoNoErr) cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		}
		if (fMap & kSystemsSliceBB) {
			err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoSystemSlice, *this);
			if (err != guidoNoErr) cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		}
		if (fMap & kStavesBB) {
			err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoStaff, *this);
			if (err != guidoNoErr) cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		} 
		if (fMap & kMeasureBB) {
			err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoBar, *this);
			if (err != guidoNoErr) cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		}
		if (fMap & kEventsBB) {
			err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoEvent, *this);
			if (err != guidoNoErr) cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		}
//	}
}

//-------------------------------------------------------------------------
MainWindow::MainWindow()
{
	QGuidoPainter::startGuidoEngine();

	setAcceptDrops(true);

	mGuidoWidget = new MapGuidoWidget();
	mScrollArea = new DragScrollArea(this);
	mScrollArea->setWidget( mGuidoWidget );
	mScrollArea->setAlignment( Qt::AlignCenter );
	mScrollArea->setBackgroundRole(QPalette::Dark);

	setCentralWidget(mScrollArea);
//	setCentralWidget( mGuidoWidget );

	mGuidoWidget->setCursor( QCursor( Qt::OpenHandCursor ) );
	mPageLabel = new QLabel("" , this);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

	mScaleFactor = 1.0f;
	mAdjustMode = AdjustNone;
	
    readSettings();
    setCurrentFile("");
	
	GuidoGetDefaultLayoutSettings (&mGuidoEngineParams);
	mGuidoWidget->setGuidoLayoutSettings(mGuidoEngineParams);
	resize( DEFAULT_WINDOW_WIDTH , DEFAULT_WINDOW_HEIGHT );
}

//-------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	QGuidoPainter::stopGuidoEngine();
}

//-------------------------------------------------------------------------
void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
	if (event->mimeData()->hasUrls())
         event->acceptProposedAction();
}

//-------------------------------------------------------------------------
void MainWindow::dropEvent(QDropEvent* event)
{
	if (event->mimeData()->hasUrls())
	{
        QString fileName = event->mimeData()->urls().first().toLocalFile();
        loadFile( fileName );
		event->accept();
		return;
    }
}

//-------------------------------------------------------------------------
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::FileOpen) 
	{
		loadFile( ((QFileOpenEvent*)event)->file() );
		return true;
	} 
	else 
	{
		// standard event processing
		return QObject::eventFilter(obj, event);
	}
}
	 
//-------------------------------------------------------------------------
void MainWindow::open()
{
    const char *str = QGuidoImporter::musicxmlSupported() ? "Guido Music Notation or MusicXML (*.gmn *.xml)" : "Guido Music Notation (*.gmn)";
	QString fileName = QFileDialog::getOpenFileName(this , QString() , QString() , tr(str) );
    if (!fileName.isEmpty())  
	{
        loadFile(fileName);
	}
}

//-------------------------------------------------------------------------
void MainWindow::reload()
{
    if (!mCurFile.isEmpty())  
	{
        loadFile(mCurFile);
	}
}
	

//-------------------------------------------------------------------------
void MainWindow::about()
{
    QMessageBox::about(this, tr(QString("About " + APP_NAME).toUtf8().data()),
             tr(QString("<h2>" + APP_NAME + "</h2>" + 
                "<p>Copyright &copy; 2008 Grame. " 
                "<p>A GMN files viewer based on Qt. ").toUtf8().data()));
}

//-------------------------------------------------------------------------
void MainWindow::setEngineSettings(const GuidoLayoutSettings& gls, int bbmap)
{
	if ( (gls.systemsDistance != mGuidoEngineParams.systemsDistance)		|
		(gls.systemsDistribution != mGuidoEngineParams.systemsDistribution)	|
		(gls.systemsDistribLimit != mGuidoEngineParams.systemsDistribLimit)	|
		(gls.force != mGuidoEngineParams.force)								|
		(gls.spring != mGuidoEngineParams.spring)							|
		(gls.neighborhoodSpacing != mGuidoEngineParams.neighborhoodSpacing)	|
		(gls.optimalPageFill != mGuidoEngineParams.optimalPageFill)	)
	{
		mGuidoEngineParams = gls;
		mGuidoWidget->setGuidoLayoutSettings( mGuidoEngineParams );
		updateWidgetSize();
	}
	if (bbmap != GuidoGetDrawBoundingBoxes()) {
		GuidoDrawBoundingBoxes(bbmap);
		updateWidgetSize();
	}
	if (bbmap != mGuidoWidget->fMap) {
		mGuidoWidget->fMap = bbmap;
	}
}

//-------------------------------------------------------------------------
void MainWindow::preferences()
{
    SetupDialog * dialog = new SetupDialog(this);
	dialog->show();
}

//------------------------------------------------------------------------- 
void MainWindow::print() 
{
	QGuidoPainter * guidoPainter = QGuidoPainter::createGuidoPainter();
	QPrinter printer;
	QPainter painter;

	guidoPainter->setGuidoLayoutSettings(mGuidoEngineParams);
	if ( guidoPainter->setGMNFile(mCurFile) )
	{
		QPrintDialog * dialog = new QPrintDialog( &printer , this);
		dialog->setMinMax( 1 , guidoPainter->pageCount() );
		dialog->setWindowTitle(tr("Print Score"));
		if (dialog->exec() == QDialog::Accepted)
		{
			painter.begin(&printer);
			painter.setRenderHint( QPainter::Antialiasing );
			QRect drawRect = QRect( 0 , 0 , printer.pageRect().width() , printer.pageRect().height() );
			int pCount = guidoPainter->pageCount();
			for (int page = 1; page <= pCount ; ++page) 
			{
				guidoPainter->draw(&painter , page , drawRect );
				if (page != pCount)
					printer.newPage();
			}
			painter.end();
		}
	}
	else
	{
		statusBar()->showMessage(tr("Error reading file."));
	}
	
	QGuidoPainter::destroyGuidoPainter( guidoPainter );
}

//-------------------------------------------------------------------------
void MainWindow::exportToImage()
{
	QGuidoPainter * guidoPainter = QGuidoPainter::createGuidoPainter();
	guidoPainter->setGuidoLayoutSettings(mGuidoEngineParams);
	if ( guidoPainter->setGMNFile(mCurFile) )
	{
		 QString dir = QFileDialog::getExistingDirectory(this, tr("Export to file - Create & select a directory"),
                                                 "",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
		if (!dir.isEmpty())  
		{
			QString fileName;
			int pos = dir.lastIndexOf( QDir::separator() );
			fileName = dir.right( dir.length() - pos );

			int pageCount = guidoPainter->pageCount();

			QLabel label("Exporting...");
			statusBar()->addWidget( &label );
			QProgressBar progressBar;
			progressBar.setMaximumHeight( statusBar()->height() - 5 );
			progressBar.setMaximum( pageCount );
			statusBar()->addWidget( &progressBar );

			progressBar.setValue(0);
			for ( int page = 1 ; page <= pageCount ; page++ )
			{	
				//Create a new QImage, with a size corresponding to the Guido Score page
				QSizeF size = guidoPainter->pageSizeMM(page);
				size.setWidth( 5 * size.width() );
				size.setHeight( 5 * size.height() );
				QImage image( size.width() , size.height() , QImage::Format_ARGB32);
				//Create a QPainter to paint on the QImage
				QPainter painter;
				painter.begin( &image );
				//With the QPainter, fill background with white.
				painter.setBrush(QBrush(QColor(255,255,255)));
				painter.setPen(QPen(QColor(255,255,255)));
				painter.drawRect(0,0,size.width(), size.height());
				//With the QPainter and the QGuidoPainter, draw the score.
				guidoPainter->draw( &painter , page , QRect(0,0,size.width(),size.height()) );
				painter.end();
				//Save the QImage to a file.
				image.save( dir + QDir::separator() + fileName + "_page" + QVariant(page).toString() + ".png" , "PNG" );
				progressBar.setValue(page);
			}
		}
	}
}

//-------------------------------------------------------------------------
void MainWindow::setCurrentPage(int pageIndex)
{
	int pageCount = mGuidoWidget->pageCount();
	if ( pageIndex < 1 )
		return;
	if ( pageIndex > pageCount )
		return;
	if ( mGuidoWidget->firstVisiblePage() == pageIndex )
		return;
	
	mGuidoWidget->setPage(pageIndex);
	updatePageLabel(pageIndex , pageCount );

	updateWidgetSize();
}

//-------------------------------------------------------------------------
void MainWindow::nextPage()
{
	setCurrentPage( mGuidoWidget->firstVisiblePage() + 1 );
}

//-------------------------------------------------------------------------
void MainWindow::prevPage()
{
	setCurrentPage( mGuidoWidget->firstVisiblePage() - 1 );
}

//-------------------------------------------------------------------------
void MainWindow::firstPage()
{
	setCurrentPage( 1 );
}

//-------------------------------------------------------------------------
void MainWindow::lastPage()
{
	setCurrentPage( mGuidoWidget->pageCount() );
}

//-------------------------------------------------------------------------
void MainWindow::updatePageLabel(int currentPage, int pageCount)
{
	char buffer[30];
	sprintf(buffer,"Page %d/%d" , currentPage , pageCount);
	mPageLabel->setText( tr(buffer) );
}

//-------------------------------------------------------------------------
void MainWindow::zoomIn()
{
	scaleImage( ZOOM_FACTOR );
	changeAdjustMode( AdjustNone );
}

//-------------------------------------------------------------------------
void MainWindow::zoomOut()
{
	scaleImage( 1.0f / ZOOM_FACTOR );
	changeAdjustMode( AdjustNone );
}

//-------------------------------------------------------------------------
float MainWindow::getRatio(QScrollBar *scrollBar)
{
	if ( scrollBar->maximum() == 0 )
	{
		//The slider is hidden. (the widget is smaller than the scroll area)
		return 0.5f;
	}
	else
	{
		float result = ( scrollBar->value() + scrollBar->pageStep()/2.0f ) / float(scrollBar->maximum() + scrollBar->pageStep());
		return result;
	}
}

//-------------------------------------------------------------------------
void MainWindow::setRatio(QScrollBar *scrollBar, float ratio)
{
	float value = ratio * ( scrollBar->maximum() + scrollBar->pageStep() )
					- scrollBar->pageStep()/2.0f;
	scrollBar->setValue( value );
}

//-------------------------------------------------------------------------
void MainWindow::scaleImage(double factor)
{
	float xScrollFraction = getRatio(mScrollArea->horizontalScrollBar());
	float yScrollFraction = getRatio(mScrollArea->verticalScrollBar());

	mScaleFactor *= factor;
	mGuidoWidget->resize( mGuidoWidget->size() * factor  );

	setRatio(mScrollArea->horizontalScrollBar(), xScrollFraction);
    setRatio(mScrollArea->verticalScrollBar(), yScrollFraction);
	
    zoomInAct->setEnabled(mScaleFactor < MAXIMUM_ZOOM );
    zoomOutAct->setEnabled(mScaleFactor > 1 / MAXIMUM_ZOOM);
}

//-------------------------------------------------------------------------
void MainWindow::changeAdjustMode( AdjustMode mode )
{
	mAdjustMode = mode;
	
	switch ( mAdjustMode ) {
	case AdjustToHeight:
		adjustToWidthAct->setChecked(false);
		adjustToHeightAct->setChecked(true);
		break;
	case AdjustToWidth:
		adjustToWidthAct->setChecked(true);
		adjustToHeightAct->setChecked(false);
		break;
	case AdjustNone:
		adjustToWidthAct->setChecked(false);
		adjustToHeightAct->setChecked(false);
		break;
	}
}

//-------------------------------------------------------------------------
void MainWindow::adjustToWidthSwitch()
{
	if ( mAdjustMode == AdjustToWidth )
	{
		changeAdjustMode( AdjustNone );
		return;
	}
	scaleToWidth();
} 

//-------------------------------------------------------------------------
void MainWindow::adjustToHeightSwitch()
{
	if ( mAdjustMode == AdjustToHeight )
	{
		changeAdjustMode( AdjustNone );
		return;
	}
	scaleToHeight();
}

//-------------------------------------------------------------------------
void MainWindow::scaleToWidth()
{
	int viewWidth = mScrollArea->viewport()->size().width()  - horizontalBorderMargin();
	int currentWidth = mGuidoWidget->width();
	
	float zoom = viewWidth / float(currentWidth);
	
	scaleImage(zoom);
	
	changeAdjustMode( AdjustToWidth );
}

//-------------------------------------------------------------------------
void MainWindow::scaleToHeight()
{
	int viewHeight = mScrollArea->viewport()->size().height()  - verticalBorderMargin();
	int currentHeight = mGuidoWidget->height();

	float zoom = viewHeight / float(currentHeight);
	
	scaleImage(zoom);
	
	changeAdjustMode( AdjustToHeight );
}

//-------------------------------------------------------------------------
void MainWindow::resizeEvent ( QResizeEvent * event )
{
	event->accept();
	switch (mAdjustMode)
	{
		case AdjustToWidth :
			scaleToWidth();
			break;
		case AdjustToHeight :
			scaleToHeight();
			break;
		case AdjustNone:	break;
		default:			break;
	}
}

//-------------------------------------------------------------------------
void MainWindow::createActions()
{
    openAct = new QAction(QIcon(":/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr(QGuidoImporter::musicxmlSupported() ? "Open a Guido or MusicXML file" : "Open a Guido score file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	reloadAct = new QAction(QIcon(":/reload.png"), tr("&Reload"), this);
    reloadAct->setShortcut(tr("F5"));
    reloadAct->setStatusTip(tr("Reload the current Guido score file"));
    connect(reloadAct, SIGNAL(triggered()), this, SLOT(reload()));
	
	exportToImageAct = new QAction(QIcon(":/export.png"), tr("&Export to image..."), this);
    exportToImageAct->setShortcut(tr("Ctrl+E"));
    exportToImageAct->setStatusTip(tr("Export the Guido score to a set of image files in a directory"));
    connect(exportToImageAct, SIGNAL(triggered()), this, SLOT(exportToImage()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    setupAct = new QAction(tr("Preferences"), this);
    setupAct->setStatusTip(tr("Setup the GUIDO engine parameters"));
    connect(setupAct, SIGNAL(triggered()), this, SLOT(preferences()));

	printAct = new QAction(QIcon(":/print.png"), tr("&Print") , this);
	printAct->setShortcut(tr("Ctrl+P"));
	printAct->setStatusTip(tr("Print the score"));
	connect(printAct , SIGNAL(triggered()) , this , SLOT(print()));
	
	nextPageAct = new QAction(QIcon(":/next.png"), tr("&Next") , this);
	nextPageAct->setShortcut(tr("PgDown"));
	nextPageAct->setStatusTip(tr("Show next page"));
	connect(nextPageAct , SIGNAL(triggered()) , this , SLOT(nextPage()) );
	
	prevPageAct = new QAction(QIcon(":/prev.png"), tr("P&rev") , this);
	prevPageAct->setShortcut(tr("PgUp"));
	prevPageAct->setStatusTip(tr("Show previous page"));
	connect(prevPageAct , SIGNAL(triggered()) , this , SLOT(prevPage()) );
	
	firstPageAct = new QAction(QIcon(":/first.png"), tr("&First") , this);
	firstPageAct->setShortcut(tr("Home"));
	firstPageAct->setStatusTip(tr("Show first page"));
	connect(firstPageAct , SIGNAL(triggered()) , this , SLOT(firstPage()) );
	
	lastPageAct = new QAction(QIcon(":/last.png"), tr("L&ast") , this);
	lastPageAct->setShortcut(tr("End"));
	lastPageAct->setStatusTip(tr("Show last page"));
	connect(lastPageAct , SIGNAL(triggered()) , this , SLOT(lastPage()) );

	zoomInAct = new QAction(QIcon(":/zoomIn.png"), tr("Zoom &In") , this);
	zoomInAct->setShortcut( QKeySequence::ZoomIn);
	zoomInAct->setStatusTip(tr("Zoom in"));
	connect(zoomInAct , SIGNAL(triggered()) , this , SLOT(zoomIn()) );
	zoomInAct->setEnabled( false ) ;
	
	zoomOutAct = new QAction(QIcon(":/zoomOut.png"), tr("Zoom O&ut") , this);
	zoomOutAct->setShortcut( QKeySequence::ZoomOut);
	zoomOutAct->setStatusTip(tr("Zoom out"));
	connect(zoomOutAct , SIGNAL(triggered()) , this , SLOT(zoomOut()) );
	zoomOutAct->setEnabled( false ) ;

	adjustToWidthAct = new QAction(QIcon(":/adjustToWidth.png"), tr("Adjust to &width") , this);
	adjustToWidthAct->setShortcut(tr("Ctrl+W"));
	adjustToWidthAct->setStatusTip(tr("Adjust zoom to page width"));
	adjustToWidthAct->setCheckable( true );
	connect(adjustToWidthAct , SIGNAL(triggered()) , this , SLOT(adjustToWidthSwitch()) );
	
	adjustToHeightAct = new QAction(QIcon(":/adjustToHeight.png"), tr("Adjust to &height") , this);
	adjustToHeightAct->setShortcut(tr("Ctrl+H"));
	adjustToHeightAct->setStatusTip(tr("Adjust zoom to page height"));
	adjustToHeightAct->setCheckable( true );
	connect(adjustToHeightAct , SIGNAL(triggered()) , this , SLOT(adjustToHeightSwitch()) );
}

//-------------------------------------------------------------------------
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(setupAct);
    fileMenu->addAction(openAct);
	fileMenu->addAction(printAct);
	fileMenu->addAction(exportToImageAct);
    fileMenu->addSeparator();
	fileMenu->addAction(reloadAct);
	fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

	viewMenu = menuBar()->addMenu(tr("&View"));
	viewMenu->addAction(firstPageAct);
	viewMenu->addAction(prevPageAct);
	viewMenu->addAction(nextPageAct);
	viewMenu->addAction(lastPageAct);
	viewMenu->addSeparator();
	viewMenu->addAction(zoomInAct);
	viewMenu->addAction(zoomOutAct);
	viewMenu->addSeparator();
	viewMenu->addAction(adjustToWidthAct);
	viewMenu->addAction(adjustToHeightAct);
	
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

//-------------------------------------------------------------------------
void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
	fileToolBar->addAction(printAct);
	fileToolBar->addAction(exportToImageAct);
	fileToolBar->addSeparator();
	fileToolBar->addAction(reloadAct);
	fileToolBar->addSeparator();
	fileToolBar->setMovable( false );
	
	viewToolBar = addToolBar(tr("View"));
	viewToolBar->addAction(firstPageAct);
	viewToolBar->addAction(prevPageAct);
	viewToolBar->addAction(nextPageAct);
	viewToolBar->addAction(lastPageAct);
	viewToolBar->addSeparator();
	viewToolBar->addAction(zoomInAct);
	viewToolBar->addAction(zoomOutAct);
	viewToolBar->addSeparator();
	viewToolBar->addAction(adjustToWidthAct);
	viewToolBar->addAction(adjustToHeightAct);
	viewToolBar->addSeparator();
	viewToolBar->setMovable( false );
}

//-------------------------------------------------------------------------
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
	statusBar()->addPermanentWidget( mPageLabel );
}

//-------------------------------------------------------------------------
void MainWindow::readSettings()
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

//-------------------------------------------------------------------------
void MainWindow::writeSettings()
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}


//-------------------------------------------------------------------------
void MainWindow::updateWidgetSize()
{	
//	mGuidoWidget->resize( QSize( scoreWidth , mGuidoWidget->heightForWidth( scoreWidth ) ) );
	mGuidoWidget->resize( mScrollArea->viewport()->size() - QSize( horizontalBorderMargin() , verticalBorderMargin() ) );
	
	switch ( mAdjustMode )
	{
		case AdjustToHeight :
			scaleToHeight(); break;
		case AdjustToWidth :
			scaleToWidth(); break;
		case AdjustNone :
		{
			int viewW = mScrollArea->viewport()->size().width();
			int viewH = mScrollArea->viewport()->size().height();
			float viewHeightWidthRatio = viewH / float(viewW);

			int testWidth = 100;
			float scoreHeightWidthRatio = ( mGuidoWidget->heightForWidth( testWidth ) / float(testWidth) );
			if ( viewHeightWidthRatio < scoreHeightWidthRatio )
			{
				scaleToHeight(); 
			}
			else
			{
				scaleToWidth();
			}
			changeAdjustMode( AdjustNone );
			break;
		}
	}
}

//-------------------------------------------------------------------------
bool MainWindow::loadFile(const QString &fileName)
{
	statusBar()->showMessage(tr(QString("Loading " + fileName + "...").toUtf8().data()));
    QApplication::setOverrideCursor(Qt::WaitCursor);
 	mScaleFactor = 1;

	bool loadOk = mGuidoWidget->setGMNFile( fileName );
	if (!loadOk && QGuidoImporter::musicxmlSupported()) {	// try to import file as MusicXML file
		std::stringstream out;
		if ( QGuidoImporter::musicxmlFile2Guido(fileName.toUtf8().constData(), true, out) )
			loadOk = mGuidoWidget->setGMNCode( out.str().c_str() );
	}

	if ( loadOk )
	{
//		mGuidoWidget->resize( QSize( scoreWidth , mGuidoWidget->heightForWidth( scoreWidth ) ) );

		mGuidoWidget->resize( mScrollArea->viewport()->size() - QSize( horizontalBorderMargin() , verticalBorderMargin() ) );
		switch ( mAdjustMode )
		{
			case AdjustToHeight :
				adjustToHeightSwitch(); break;
			case AdjustToWidth :
				adjustToWidthSwitch(); break;
			case AdjustNone :
			{
				int viewW = mScrollArea->viewport()->size().width();
				int viewH = mScrollArea->viewport()->size().height();
				float viewHeightWidthRatio = viewH / float(viewW);

				int testWidth = 100;
				float scoreHeightWidthRatio = ( mGuidoWidget->heightForWidth( testWidth ) / float(testWidth) );
				if ( viewHeightWidthRatio < scoreHeightWidthRatio )
				{
					scaleToHeight(); 
				}
				else
				{
					scaleToWidth();
				}
				changeAdjustMode( AdjustNone );
				break;
			}
		}

		firstPage();
		statusBar()->showMessage(tr("File loaded"), 2000);
	}
	else
	{
		QString errorMessage = "Invalid GMN file : " + mGuidoWidget->getLastErrorMessage();
		mGuidoWidget->resize( QSize( 0,0 ) );
		statusBar()->showMessage(tr(errorMessage.toUtf8().data()), 2000);
	}

	QApplication::restoreOverrideCursor();
	setCurrentFile(fileName.toUtf8().data());

	return loadOk;
}

//-------------------------------------------------------------------------
void MainWindow::setCurrentFile(const QString &fileName)
{
    mCurFile = fileName;
    setWindowModified(false);

	reloadAct->setEnabled( !mCurFile.isEmpty() );

    QString shownName;
	shownName = strippedName(mCurFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr(QString(APP_NAME).toUtf8().data())));
}

//-------------------------------------------------------------------------
QString MainWindow::strippedName(const QString &fullFileName)
{
    //return QFileInfo(fullFileName).fileName();
	return fullFileName;
}

//-------------------------------------------------------------------------
int MainWindow::horizontalBorderMargin()
{
	if ( mScrollArea->verticalScrollBar()->isVisible() )
		return HORIZONTAL_BORDER_MARGIN - mScrollArea->verticalScrollBar()->width();
	else
		return HORIZONTAL_BORDER_MARGIN;
}

//-------------------------------------------------------------------------
int MainWindow::verticalBorderMargin()
{
	if ( mScrollArea->horizontalScrollBar()->isVisible() )
		return VERTICAL_BORDER_MARGIN - mScrollArea->horizontalScrollBar()->height();
	else
		return VERTICAL_BORDER_MARGIN;
}
