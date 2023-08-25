/*
 * MainWindow.cpp
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
 
#include <fstream>
#include <sstream>
#include <iostream> 

#include <QtGui>
#include <QPrintDialog>
#include <QPrinter>
#include <QDockWidget>
#include <QVariant>
#include <QImage>
#include <QProgressBar>
#include <QDir>
#include <QTextDocument>
#include <QMessageBox>
#include <QToolBar>
#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
# define Qt6 true
#endif

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#include "MainWindow.h"
#if dynamic_midi
typedef GuidoErrCode (* GuidoAR2MIDIFilePtr)(const struct NodeAR* ar, const char* filename, const Guido2MidiParams* params);
#else
#define MIDIEXPORT
#endif
#include "GUIDO2Midi.h"

#include "QGuidoWidget.h"
#include "QGuidoPainter.h"
#include "SetupDialog.h"
#include "QGuidoImporter.h"
#include "GuidoHighlighter.h"
#include "CodeEditor.h"


using namespace std;

#ifdef Q_OS_MAC
#include "CoreFoundation/CFBundle.h"
#endif


#define APP_NAME QString("GuidoEditor")
#define COMPANY_NAME QString("GRAME")

#define GMN_EXTENSION	QString("gmn")
#define GMN_FILTER		QString( QString("Guido Music Notation (*.")+ GMN_EXTENSION +")" )
#define ALL_FILTER		QString( "All (*.*)" )

#define POS_SETTING						"pos"
#define SIZE_SETTING					"size"
#define WINDOW_STATE_SETTING			"windowState"
#define RECENT_FILES_SETTING			"recentFiles"
#define RECENT_FILE_NAME_SETTING		"recentFileName"
#define LINE_WRAPPING_SETTING			"lineWrapping"
#define ADJUST_MODE_SETTING				"adjustMode"
#define FONT_SIZE_SETTING				"fontSize"
#define BBMAP_SETTING					"BBMap"
#define CHECK_LYRICS_SETTING			"checkLyrics"
#define SHOW_BOXES_SETTING				"showBoxes"
#define SHOW_MAPPING_SETTING			"showMapping"
#define RAW_MAPPING_SETTING				"rawMapping"
#define VOICE_NUM_SETTING				"voiceNum"
#define STAFF_NUM_SETTING				"staffNum"
#define ENGINE_SETTING					"engine"
#define NEIGHBORHOOD_SPACING_SETTING	"neighborhoodSpacing"
#define SPRING_SETTING					"spring"
#define FORCE_SETTING					"force"
#define SYSTEMS_DISTRIB_LIMIT_SETTING	"systemsDistribLimit"
#define SYSTEMS_DISTRIBUTION_SETTING	"systemsDistribution"
#define SYSTEMS_DISTANCE_SETTING		"systemsDistance"
#define OPTIMAL_PAGE_FILL_SETTING		"optimalPageFill"
#define PROPORTIONAL_RENDERING_SETTING  "proportionalRendering"
#define RESIZE_PAGE_2_MUSIC_SETTING     "resizePage2Music"
#define SCORE_COLOR_SETTING				"scoreColor"
#define SYNTAX_HIGHLIGHTER_SETTING		"syntaxHighlighter"
#define FONT_COLOR_SETTING				"fontColor"
#define FONT_WEIGHT_SETTING				"fontWeight"

#define ZOOM_FACTOR 1.5f
#define DEFAULT_WINDOW_WIDTH  800
#define DEFAULT_WINDOW_HEIGHT 600
#define MAXIMUM_ZOOM 1000
#define MINIMUM_ZOOM 1
#define HORIZONTAL_BORDER_MARGIN	20
#define VERTICAL_BORDER_MARGIN		20
#define TOOL_BAR_ICON_SIZE			20
#define TEXT_EDIT_TEMPO 500
#define MIN_FONT_SIZE 2
#define MAX_FONT_SIZE 72
#define DEFAULT_TOOL_BAR_VISIBILITY true


//#define VIOLET				QColor(72,66,139)
//#define LIGHT_YELLOW		QColor(250,250,210)
//#define LIGHT_GREEN			QColor(240,255,240)
//#define FLORAL_GREEN		QColor(255,250,240)
//#define DARK_OLIVE_GREEN	QColor(85,107,47)
//#define DODGER_BLUE_4		QColor(16,78,139)
//#define INDIAN_RED_1		QColor(255,106,106)
//#define INDIAN_RED_2		QColor(190,84,83)
//#define DARK_SEA_GREEN_1	QColor(193,255,193)
//#define DARK_SEA_BLUE_1		QColor(193,193,255)
//#define LAVENDER_BLUSH		QColor(255,240,245)
#define GREEN_4				QColor(0,139,0)
//#define ORANGE_1			QColor(240,110,36)
//#define ORANGE_2			QColor(209,110,36)
#define DEEP_BLUE			QColor(31,68,110)
//#define DEEP_TURQUOISE		QColor(31,109,110)
//#define DEEP_PURPLE			QColor( 70 , 0 , 83 )
#define RED					QColor( 128 , 0 , 0 )

#define VOICE_SEPARATOR_FONT	Qt::black
#define SQUARE_BRACKETS_WEIGHT	QFont::Bold

#define SCORE_SEPARATOR_FONT	Qt::black
#define CURLY_BRACKETS_WEIGHT	QFont::Bold

#define DURATIONS_FONT		RED
#define DURATIONS_WEIGHT	QFont::Bold

#define NOTE_FONT			DEEP_BLUE
#define NOTE_NAMES_WEIGHT	QFont::Bold

#define TAGS_FONT			Qt::black
#define TAGS_WEIGHT			QFont::Normal

#define FULL_TAGS_FONT		Qt::blue
#define FULL_TAGS_WEIGHT	QFont::Normal

#define COMMENTS_FONT		GREEN_4
#define COMMENTS_WEIGHT		QFont::Normal

QColor MainWindow::mDefaultFontColors[ GuidoHighlighter::SIZE ] =
{
	VOICE_SEPARATOR_FONT,
	SCORE_SEPARATOR_FONT,
	DURATIONS_FONT,
	NOTE_FONT,
	FULL_TAGS_FONT,
	TAGS_FONT,
	COMMENTS_FONT
};

int MainWindow::mDefaultFontWeights[ GuidoHighlighter::SIZE ] =
{
	SQUARE_BRACKETS_WEIGHT,
	CURLY_BRACKETS_WEIGHT,
	DURATIONS_WEIGHT,
	NOTE_NAMES_WEIGHT,
	FULL_TAGS_WEIGHT,
	TAGS_WEIGHT,
	COMMENTS_WEIGHT
};

class DragScrollArea : public QScrollArea 
{

	public:
		DragScrollArea(QWidget * parent) : QScrollArea( parent ) {}

	protected:
#define LINE_WRAPPING_SETTING			"lineWrapping"
	
		QPoint mPreviousPoint;
	
		void moveScrollBar(int pixelDelta , QScrollBar * scrollBar)	{ scrollBar->setValue( scrollBar->value() - pixelDelta ); }
	
		void mouseMoveEvent ( QMouseEvent * event ) {
			if ( widget() ) {
				event->accept();
				if ( !mPreviousPoint.isNull() ) {
					int dx = event->pos().x() - mPreviousPoint.x();
					int dy = event->pos().y() - mPreviousPoint.y();				
					moveScrollBar( dx , horizontalScrollBar() );
					moveScrollBar( dy , verticalScrollBar() );
				}
				mPreviousPoint = event->pos();
			}
		}
		
		void mouseReleaseEvent ( QMouseEvent * event ) {
			if ( widget() ) {
				event->accept();
//				widget()->setCursor( QCursor( Qt::OpenHandCursor ) );
				mPreviousPoint.setX(0);
				mPreviousPoint.setY(0);
			}
		}

		void mousePressEvent ( QMouseEvent * event ) {
			if ( widget() ) {
				event->accept();
//				widget()->setCursor( QCursor( Qt::ClosedHandCursor ) );
				mPreviousPoint = event->pos();
			}
		}
};

class QScoreDockWidget : public QDockWidget 
{
	MainWindow * mainWindow;

	public:
		QScoreDockWidget( const QString& name , MainWindow* parent ) : QDockWidget(name,parent), mainWindow(parent) {}
	
	protected:
		void resizeEvent ( QResizeEvent * event ) {
			event->accept();
			if ( mainWindow->mAdjustMode != MainWindow::AdjustNone )
				mainWindow->updateWidgetSize();
		}
};

//-------------------------------------------------------------------------
bool MapGuidoWidget::showStaff(int staffNum) const
{
	return (staffNum==fStaffNum) || (fStaffNum == ALL_STAFF );
}

//-------------------------------------------------------------------------
bool MapGuidoWidget::showVoice(int voiceNum) const
{
	return (voiceNum==fVoiceNum) || (fVoiceNum == ALL_VOICE );
}

//-------------------------------------------------------------------------
void MapGuidoWidget::mousePressEvent ( QMouseEvent* event)
{
	for (int p = firstVisiblePage(); p <= lastVisiblePage(); p++) {
		Time2GraphicMap	map;
		GuidoErrCode err = GuidoGetSystemMap( getGRHandler(), p, size().width(), size().height(), map);
		if (err) break;
		TimeSegment t; FloatRect r;
#if Qt6
		if (GuidoGetPoint( event->position().x(), event->position().y(), map, t, r)) {
#else
		if (GuidoGetPoint( event->x(), event->y(), map, t, r)) {
#endif
//			cout << "clicked page " << p << " in time segment " << t << endl;
			break;
		}
	}
}

//-------------------------------------------------------------------------
void MapGuidoWidget::Graph2TimeMap (const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
	if ( false )
		cout	<< "map collection: " << box << " dates: " << dates << " type: " << infos.type 
				<< ", voice=" << infos.voiceNum << ", staff=" << infos.staffNum  << endl;

	if ( showStaff(infos.staffNum) && showVoice(infos.voiceNum) )
	{
		QPainter paint(this);
		paint.setBrush( QBrush( QColor( 0 , 0, 255 , 100 ) ) );
		paint.drawRect( QRectF( box.left , box.top , box.right - box.left , box.bottom - box.top ) );
	}
}

//-------------------------------------------------------------------------
void MapGuidoWidget::paintMap(const Time2GraphicMap& map)
{
	QColor cols[2] = { QColor( 0, 0, 200, 100 ), QColor( 200, 0, 0, 100 ) };
	int colindex = 0;
	QPainter paint(this);
	for (Time2GraphicMap::const_iterator i = map.begin(); i != map.end(); i++) {
		paint.setBrush( QBrush( cols[colindex++ % 2] ) );
		paint.drawRect( QRectF( i->second.left, i->second.top, i->second.right - i->second.left, i->second.bottom - i->second.top));
	}
}

//-------------------------------------------------------------------------
GuidoErrCode MapGuidoWidget::paintStaff(int page, int num)
{
	Time2GraphicMap map;
	GuidoErrCode err = GuidoGetStaffMap(getGRHandler(), page, width(), height(), num, map);
	if (map.size()) paintMap (map);
	return err;
}

//-------------------------------------------------------------------------
GuidoErrCode MapGuidoWidget::paintVoice(int page, int num)
{
	Time2GraphicMap map;
	GuidoErrCode err = GuidoGetVoiceMap(getGRHandler(), page, width(), height(), num, map);
	if (map.size()) paintMap (map);
	else err = guidoErrActionFailed;
	return err;
}

//-------------------------------------------------------------------------
void MapGuidoWidget::paintEvent(QPaintEvent * event)
{
	QGuidoWidget::paintEvent (event);
	int page = firstVisiblePage();
	if (fMap) {
		if (fRaw) {
			GuidoErrCode err = guidoNoErr;
			if (fMap & kPageBB)
				err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoPage, *this);
			if (fMap & kSystemsBB)
				err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoSystem, *this);
			if (fMap & kSystemsSliceBB)
				err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoSystemSlice, *this);
			if (fMap & kStavesBB)
				err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoStaff, *this);
			if (fMap & kMeasureBB)
				err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoBar, *this);
			if (fMap & kEventsBB)
				err = GuidoGetMap(getGRHandler(), page, width(), height(), kGuidoEvent, *this);
			if (err != guidoNoErr) cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		}
		else {
			GuidoErrCode err = guidoNoErr;
			if (fMap & kPageBB) {
				Time2GraphicMap map;
				err = GuidoGetPageMap(getGRHandler(), page, width(), height(), map);
				if (err == guidoNoErr) paintMap (map);
			}
			if (fMap & kSystemsBB) {
				Time2GraphicMap map;
				err = GuidoGetSystemMap(getGRHandler(), page, width(), height(), map);
				if (err == guidoNoErr) paintMap (map);
			}
			if (fMap & kStavesBB) {
				if (fStaffNum > 0)	err = paintStaff(page, fStaffNum);
				else if (fStaffNum < 0) {
					int n = GuidoCountVoices (getARHandler());
					for (int staff=1; staff <= n ; staff++)
						err = paintStaff(page, staff);
				}
			}
			if (fMap & kEventsBB) {
				if (fVoiceNum > 0)	err = paintVoice(page, fVoiceNum);
				else if (fVoiceNum < 0) {
					int n = GuidoCountVoices (getARHandler());
					for (int voice=1; voice <= n; voice++)
						err = paintVoice(page, voice);
				}
			}
			if ((err != guidoNoErr) && (err != guidoErrActionFailed)) 
				cerr << "GuidoGetMap error: " << GuidoGetErrorString(err) << endl;
		}
	}
}


//-------------------------------------------------------------------------
MainWindow::MainWindow() :
	mGuidoWidget(0), mSetup(0)
{
	QGuidoPainter::startGuidoEngine();

	setAcceptDrops(true);

	mScrollArea = new DragScrollArea(this);
	mScrollArea->setAlignment( Qt::AlignCenter );
	mScrollArea->setBackgroundRole(QPalette::Dark);

	mTextEdit = new CodeEditor(this);
    connect( mTextEdit->document() , SIGNAL(contentsChanged()) , this , SLOT(documentWasModified()) );
	mTextEditTimer = new QTimer( this );
	connect( mTextEditTimer , SIGNAL( timeout() ) , this , SLOT( updateCode() ) );

	mFindWidget = new QFindWidget(mTextEdit , this);
	mFindWidget->hide();
	
	QVBoxLayout *codeLayout = new QVBoxLayout;
	codeLayout->addWidget(mTextEdit);
	codeLayout->addWidget(mFindWidget);
	codeLayout->setSpacing(0);
	
	QWidget * codeDockWidget = new QWidget(this);
	codeDockWidget->setLayout( codeLayout );

	mScoreDock = new QScoreDockWidget("Score" , this);
	mScoreDock->setWidget( mScrollArea );
	mScoreDock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
	mScoreDock->setObjectName( "ScoreDock" );
	mScoreDock->setTitleBarWidget( new QWidget(this) );

	mCodeDock = new QDockWidget("Code" , this);
	mCodeDock->setWidget( codeDockWidget );
	mCodeDock->setFeatures( QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable );
	mCodeDock->hide();
	mCodeDock->setObjectName( "CodeDock" );
	mCodeDock->setTitleBarWidget( new QWidget(this) );

	addDockWidget( Qt::TopDockWidgetArea , mScoreDock );
	addDockWidget( Qt::TopDockWidgetArea , mCodeDock );

	mPageLabel = new QLabel("" , this);
	mGuidoHighlighter = new GuidoHighlighter( mTextEdit->document() );

    createActions();
    createToolBars();
    createMenus();
    createStatusBar();

	mScaleFactor = 100;

	mFileSystemWatcher = new QFileSystemWatcher(this);
	connect( mFileSystemWatcher , SIGNAL( fileChanged(QString) ) , this , SLOT(fileChanged(QString)) );

    newFile();
    readSettings();
}

//-------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	QGuidoPainter::stopGuidoEngine();
}


//-------------------------------------------------------------------------
//								Slots									///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
const QString MainWindow::filePath() const
{
	if (mCurFile.size()) {
		QDir dir(mCurFile);
		dir.cdUp();
		return dir.absolutePath();
	}
	return "";
}

//-------------------------------------------------------------------------
void MainWindow::newFile()
{
    if (maybeSave()) {
		reinitGuidoWidget();
		mTextEdit->clear();
		mTextEdit->setPlainText("{\n\n}");
        setCurrentFile("");
        reinitARHandlerPath();
		mHiddenState.reset();
		if (mSetup) mSetup->setDisplayState (mHiddenState);
    }
}

//-------------------------------------------------------------------------
bool MainWindow::save()
{
    return mCurFile.isEmpty() ? saveAs() : saveFile(mCurFile);
}

//-------------------------------------------------------------------------
bool MainWindow::saveAs()
{
	QString filters = GMN_FILTER + ";;" + ALL_FILTER;
	QString savePath = mRecentFiles.size() ? QFileInfo(mRecentFiles.last()).path() : QDir::home().path();
	QString fileName = QFileDialog::getSaveFileName(this, "Save the Guido Score", savePath, tr(filters.toUtf8().data()));
    if (fileName.isEmpty())
        return false;
	if (!fileName.toUpper().endsWith("."+QString(GMN_EXTENSION).toUpper()))
		fileName += "." + GMN_EXTENSION;

    // - To be able to remove ex filePath from ARHandler path list
    std::string exFilePath(mCurFile.toUtf8().data());
    bool saveResult = saveFile(fileName);

    // - Add path to the AR
    if (saveResult) {
        std::string filePath(fileName.toUtf8().data());
        addFileDirectoryPathToARHandler(filePath, exFilePath);
    }
    return saveResult;
}
 
//-------------------------------------------------------------------------
void MainWindow::open()
{
	if (maybeSave())
	{
		const char *str = QGuidoImporter::musicxmlSupported() ? "Guido Music Notation or MusicXML (*.gmn *.xml)" : "Guido Music Notation (*.gmn)";
		QString openPath = mRecentFiles.size() ? mRecentFiles.last() : QDir::home().path();
		QString fileName = QFileDialog::getOpenFileName(this , tr("Open file") , openPath , tr(str) );
		if (!fileName.isEmpty())
        {
			loadFile(fileName);
            mTextEdit->document()->setModified(false);
            setWindowModified(false);
        }
	}
}

//-------------------------------------------------------------------------
void MainWindow::setDisplayState (const THideState& state)
{
	mHiddenState = state;
	updateDisplayState();
}

//-------------------------------------------------------------------------
void MainWindow::updateDisplayState ()
{
	GRHandler gr = GRHandler(mGuidoWidget->getGRHandler());
	if (gr) {
		GuidoShowElement(gr, kGRSlur, !mHiddenState.slurs);
		GuidoShowElement(gr, kGRDynamics, !mHiddenState.dynamics);
		GuidoShowElement(gr, kGRArticulations, !mHiddenState.articulations);
		GuidoShowElement(gr, kGRText, !mHiddenState.text);
		GuidoShowElement(gr, kGRLyrics, !mHiddenState.lyrics);
		mGuidoWidget->clearCache();
	}
}

//-------------------------------------------------------------------------
void MainWindow::openRecentFile()
{
	QAction * action = dynamic_cast<QAction *>( sender() );
	if ( action )
	{
		QString fileName = action->data().toString();
		if (maybeSave())
			loadFile( fileName );
	}
}

//-------------------------------------------------------------------------
void MainWindow::reload()
{
	if (maybeSave())
		if (!mCurFile.isEmpty())  
			loadFile(mCurFile);
}

//-------------------------------------------------------------------------
void MainWindow::documentWasModified()
{
    setWindowModified( mTextEdit->document()->isModified() );

	mTextEditTimer->start( TEXT_EDIT_TEMPO);
//	mTextEdit->highlightErrorLine( 0 );
}

//-------------------------------------------------------------------------
void MainWindow::showTime (QGuidoWidget* w)
{
	long ptime = GuidoGetParsingTime((const ARHandler)w->getARHandler());
	long gtime = GuidoGetAR2GRTime  ((const GRHandler)w->getGRHandler());
	long dtime = GuidoGetOnDrawTime ((const GRHandler)w->getGRHandler());
	if (mSetup) mSetup->setTimings (ptime, gtime, dtime);

//	cerr << "parsing time: " << ptime << " ms" << endl;
//	cerr << "ar2gr time  : " << gtime << " ms" << endl;
//	cerr << "drawing time: " << dtime << " ms" << endl;
//	cerr << "total time  : " << (ptime + gtime + dtime) << " ms" << endl;
}

//-------------------------------------------------------------------------
void MainWindow::updateCode (bool force)
{
	mTextEditTimer->stop();

	QString newGMNCode = mTextEdit->toPlainText();
	if ( !newGMNCode.length() )
		return;


	if ( !force && newGMNCode == mGuidoWidget->gmnCode() )
		return;

	if ( mGuidoWidget->setGMNCode( newGMNCode, filePath() ) )
	{
		statusBar()->showMessage( "Code ok" );
		setCurrentPage( mGuidoWidget->firstVisiblePage() );
		updateWidgetSize();
		mTextEdit->highlightErrorLine( 0 );
		showTime (mGuidoWidget);
	}
	else
	{
        int line;
        int col;
        mGuidoWidget->getLastParseErrorLine(line, col);
		mTextEdit->highlightErrorLine(line);
		statusBar()->showMessage( mGuidoWidget->getLastErrorMessage() );
	}

}

//-------------------------------------------------------------------------
void MainWindow::lineWrapSwitch()
{
	setLineWrap( mLineWrapAct->isChecked() );
}

#define PDF_FORMAT				QString(".pdf")
#define SVG_FORMAT				QString(".svg")
#define MID_FORMAT				QString(".mid")
#define MIDI_FORMAT				QString(".midi")
//-------------------------------------------------------------------------
void MainWindow::doexport()
{
	QGuidoPainter * guidoPainter = QGuidoPainter::createGuidoPainter();
	guidoPainter->setGuidoLayoutSettings(mGuidoEngineParams);

    // - Since QGuidoPainter is created again (then lose its parameters),
    //   we have to make a backup of current ARHandler's pathsVector before...
//    std::vector<std::string> pathsVector;
//    GuidoGetSymbolPath((ARHandler)mGuidoWidget->getARHandler(), pathsVector);

	if ( guidoPainter->setGMNCode(mTextEdit->toPlainText(), filePath().toUtf8().data()) )
    {
		QString savePath = mRecentFiles.size() ? QFileInfo(mRecentFiles.last()).path() : QDir::home().path();
		QString fileName = QFileDialog::getSaveFileName(this, "Export to:", savePath);
		if (!fileName.isEmpty()) {

			if ( QFileInfo(fileName).suffix().isEmpty() )
				fileName += ".png";

			if ( fileName.toUpper().endsWith( PDF_FORMAT.toUpper() ) )
				exportToPdf( guidoPainter, fileName );
			else if ( fileName.toUpper().endsWith( SVG_FORMAT.toUpper() ) )
				exportToSVG( guidoPainter, fileName );
			else if ( fileName.toUpper().endsWith( MID_FORMAT.toUpper() ) || (fileName.toUpper().endsWith( MIDI_FORMAT.toUpper()) ))
				exportToMidi( guidoPainter->getARHandler(), fileName );
			else
				exportToImage( guidoPainter, fileName );
		}
	}

	QGuidoPainter::destroyGuidoPainter( guidoPainter );
}

#if __APPLE__
# define guidolib	"GUIDOEngine.framework/GUIDOEngine"
#elif defined(WIN32)
# define guidolib	"GUIDOEngine"
#else
# define guidolib	"libGUIDOEngine"
#endif


//-------------------------------------------------------------------------
void MainWindow::exportToMidi (CARHandler ar, const QString& filename)
{
#if dynamic_midi
	QLibrary lib(guidolib);
    if (lib.load()) {
		GuidoAR2MIDIFilePtr f = (GuidoAR2MIDIFilePtr)lib.resolve("GuidoAR2MIDIFile");
		if (f) {
			GuidoErrCode err = f (ar, filename.toStdString().c_str(), 0);
			if (err != guidoNoErr)
				statusBar()->showMessage("Export failed: "+ QString(GuidoGetErrorString (err)));
		}
//		lib.unload();
		return;
	}
	statusBar()->showMessage("MIDI export not supported");
#else
    GuidoErrCode err = GuidoAR2MIDIFile ((const ARHandler)ar, filename.toStdString().c_str(), 0);
    if (err != guidoNoErr)
        statusBar()->showMessage("Export failed: "+ QString(GuidoGetErrorString (err)));

#endif
}

//-------------------------------------------------------------------------
void MainWindow::exportToSVG(QGuidoPainter * guidoPainter, const QString& filename)
{
	QDir dir(QApplication::applicationDirPath());
#if linux
#elif !defined(WIN32)
	dir.cdUp();
	dir.cd("Fonts");
#endif
	QString guidofont = dir.absoluteFilePath("guido2.svg");

	fstream out(filename.toStdString().c_str(), fstream::out | fstream::trunc);
	int page = mGuidoWidget->firstVisiblePage();	

//	GuidoErrCode err = GuidoSVGExport (guidoPainter->getGRHandler(), page, out, guidofont.toStdString().c_str());
	GuidoErrCode err = GuidoGR2SVG( guidoPainter->getGRHandler(), page, out, true, guidofont.toStdString().c_str() );
	if (err != guidoNoErr)
		statusBar()->showMessage("Export failed: "+ QString(GuidoGetErrorString (err)));
}

//-------------------------------------------------------------------------
void MainWindow::exportToPdf(QGuidoPainter * guidoPainter, const QString& filename)
{
#ifndef IOS
	QPrinter printer(QPrinter::HighResolution);
//	printer.setFullPage(true);
	printer.setOutputFormat( QPrinter::PdfFormat );
	printer.setOutputFileName( QString(filename) );
#ifdef QTSETPAGESIZE
	printer.setPageSize( QPageSize(QPageSize::A4) );
#else
	printer.setPaperSize( QPrinter::A4 );
#endif
	print (guidoPainter, printer);
#endif
}

//-------------------------------------------------------------------------
void MainWindow::exportToImage(QGuidoPainter * guidoPainter, const QString& filename)
{
	int page = mGuidoWidget->firstVisiblePage();

	//Create a new QImage, with a size corresponding to the Guido Score page
	QSizeF size = guidoPainter->pageSizeMM(page);
	size.setWidth( 5 * size.width() );
	size.setHeight( 5 * size.height() );
	QImage image( size.width() , size.height() , QImage::Format_ARGB32);

	//Create a QPainter to paint on the QImage
	QPainter painter;
	painter.begin( &image );
	painter.setRenderHints ( QPainter::Antialiasing | QPainter::TextAntialiasing, true);

	//With the QPainter, fill background with white.
	painter.setBrush(QBrush(QColor(255,255,255,255)));
	painter.setPen(QPen(QColor(255,255,255,255)));
	painter.drawRect(0,0,size.width(), size.height());

	//With the QPainter and the QGuidoPainter, draw the score.
	guidoPainter->draw( &painter , page , QRect(0,0,size.width(),size.height()) );
	painter.end();
	//Save the QImage to a file.
	if (!image.save( filename))
		statusBar()->showMessage("Export failed: unknown format "+QFileInfo(filename).suffix());
}

//-------------------------------------------------------------------------
void MainWindow::about()
{
	QString guidoeditor_version("2.9");
    int major, minor, sub;
	GuidoGetVersionNums(&major, &minor, &sub);
	QString mastr, mistr, substr;
	mastr.setNum(major);
	mistr.setNum(minor);
	substr.setNum(sub);
	QString version(mastr + '.' + mistr + '.' + substr);
	QMessageBox::about(this, tr(QString("About " + APP_NAME).toUtf8().data()),
             tr(QString("<h2>" + APP_NAME + "</h2>" + 
                "<p>Copyright &copy; 2008-2014 Grame. " 
                "<p>A Guido score viewer and GMN editor, using Qt. "
				"<p>Using the Guido Engine version " + version +
				"<p>" + APP_NAME + " version "
						+ guidoeditor_version).toUtf8().data()));
}

//-------------------------------------------------------------------------
void MainWindow::docTags()
{
    bool docFound = false;
    
#ifdef Q_OS_MAC
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
    const char *bundlePath = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
    CFRelease(appUrlRef);
    CFRelease(macPath);
    docFound = QDesktopServices::openUrl(QUrl::fromLocalFile(QString(bundlePath) + "/Contents/Doc/RefCardsTags.pdf"));
#else
    docFound = QDesktopServices::openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/doc/RefCardsTags.pdf"));
#endif
    
    if (!docFound)
        QMessageBox::critical(this, "Doc not found", "RefCardsTags.pdf has not been found");
}

//-------------------------------------------------------------------------
void MainWindow::docParams()
{
    bool docFound = false;
    
#ifdef Q_OS_MAC
    CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef, kCFURLPOSIXPathStyle);
    const char *bundlePath = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
    CFRelease(appUrlRef);
    CFRelease(macPath);
    docFound = QDesktopServices::openUrl(QUrl::fromLocalFile(QString(bundlePath) + "/Contents/Doc/RefCardsParams.pdf"));
#else
    docFound = QDesktopServices::openUrl(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/doc/RefCardsParams.pdf"));
#endif
    
    if (!docFound)
        QMessageBox::critical(this, "Doc not found", "RefCardsParams.pdf has not been found");
}

//-------------------------------------------------------------------------
void MainWindow::preferences()
{
    if (!mSetup) mSetup = new SetupDialog(this);
	mSetup->show();
	mSetup->raise();
	mSetup->activateWindow();}

//------------------------------------------------------------------------- 
#ifndef IOS
void MainWindow::print(QGuidoPainter * guidoPainter, QPrinter& printer)
{
	QPainter painter (&printer);
	painter.setRenderHint( QPainter::Antialiasing );

	int firstPage = 1;
	int lastPage = guidoPainter->pageCount();
	if ( printer.fromPage() )
		firstPage = printer.fromPage();
	if ( printer.toPage() )
		lastPage = printer.toPage();

#if Qt6
	QRect drawRect = printer.paperRect(QPrinter::DevicePixel).toRect();
#else
	QRect drawRect = printer.paperRect();
#endif

	for (int page = firstPage; page <= lastPage ; ++page) {
		guidoPainter->draw(&painter , page , drawRect );
		if (page != lastPage)
			printer.newPage();
	}
}
#endif

#ifndef IOS
//-------------------------------------------------------------------------
void MainWindow::print() 
{
	QGuidoPainter * guidoPainter = QGuidoPainter::createGuidoPainter();
	QPrinter printer;
	QPainter painter;

	guidoPainter->setGuidoLayoutSettings(mGuidoEngineParams);
	if ( guidoPainter->setGMNCode(mTextEdit->toPlainText(), filePath().toUtf8().data()) )
	{
#ifdef QTSETPAGESIZE
		printer.setPageSize( QPageSize(QPageSize::A4) );
#else
		printer.setPaperSize( QPrinter::A4 );
#endif
		QPrintDialog * dialog = new QPrintDialog( &printer , this);
		dialog->setMinMax( 1 , guidoPainter->pageCount() );
		dialog->setWindowTitle(tr("Print Score"));
		if (dialog->exec() == QDialog::Accepted)
			print (guidoPainter, printer);
	}


//	else statusBar()->showMessage(tr("Error reading file."));
	QGuidoPainter::destroyGuidoPainter( guidoPainter );
}
#else
void MainWindow::print()	{}
#endif

//-------------------------------------------------------------------------
void MainWindow::nextPage()		{ setCurrentPage( mGuidoWidget->firstVisiblePage() + 1 ); }
void MainWindow::prevPage()		{ setCurrentPage( mGuidoWidget->firstVisiblePage() - 1 ); }
void MainWindow::firstPage()	{ setCurrentPage( 1 ); }
void MainWindow::lastPage()		{ setCurrentPage( mGuidoWidget->pageCount() ); }

//-------------------------------------------------------------------------
void MainWindow::zoomIn()
{
	int factor = ZOOM_FACTOR * mScaleFactor;
	if ( factor == mScaleFactor )
		factor += 1;
	setZoom( factor );
	changeAdjustMode( AdjustNone );
}

//-------------------------------------------------------------------------
void MainWindow::zoomOut()
{
	setZoom( mScaleFactor / ZOOM_FACTOR );
	changeAdjustMode( AdjustNone );
}

//-------------------------------------------------------------------------
void MainWindow::spinBoxZoom(int percent)
{
	changeAdjustMode(AdjustNone);
	setZoom(percent);
}

//-------------------------------------------------------------------------
void MainWindow::setZoom(int percent)
{
	mScaleFactor = percent;
	mZoomSpinBox->setValue( percent );
	
	float xScrollFraction = getRatio(mScrollArea->horizontalScrollBar());
	float yScrollFraction = getRatio(mScrollArea->verticalScrollBar());

	mGuidoWidget->resize( QSizeF(mBaseSize * mScaleFactor/100.0f).toSize() );

	setRatio(mScrollArea->horizontalScrollBar(), xScrollFraction);
    setRatio(mScrollArea->verticalScrollBar(), yScrollFraction);

    mZoomInAct->setEnabled( mScaleFactor < MAXIMUM_ZOOM );
    mZoomOutAct->setEnabled( mScaleFactor > MINIMUM_ZOOM );
}

//-------------------------------------------------------------------------
void MainWindow::setZoom()
{
	QAction * action = dynamic_cast<QAction*>(sender());
	if ( action )
	{
		int zoom = action->data().toInt();
		if ( ( zoom > MINIMUM_ZOOM ) && ( zoom < MAXIMUM_ZOOM ) )
			setZoom( zoom );
	}
}

//-------------------------------------------------------------------------
void MainWindow::adjustToWidthSwitch()
{
	if ( mAdjustMode == AdjustToWidth )
		changeAdjustMode( AdjustNone );
	else
		scaleToWidth();
}


//-------------------------------------------------------------------------
void MainWindow::adjustToHeightSwitch()
{
	if ( mAdjustMode == AdjustToHeight )
		changeAdjustMode( AdjustNone );
	else
		scaleToHeight();
}

//-------------------------------------------------------------------------
void MainWindow::adjustToFitSwitch()
{
	if ( mAdjustMode == AdjustToFit )
		changeAdjustMode( AdjustNone );
	else
		scaleToFit();
}

//-------------------------------------------------------------------------
void MainWindow::fileChanged(const QString& file)
{
	if ( QFile::exists(file) )
	{
		if ( QFileInfo(file).lastModified() <= mLastModified )
			return;

		QMessageBox::StandardButton result = QMessageBox::question ( 0, "File changed", "The GMN file has been modified from outside the application. Would you like to reload it ?",
		QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes );
		if ( result == QMessageBox::Yes )
			reload();
	}
	else
	{
		QMessageBox::warning(0 , "File removed" , "The GMN file has been removed.");
	}
}

//-------------------------------------------------------------------------
void MainWindow::toggleDocks()
{	
	if ( mScoreDock->isVisible() && mCodeDock->isVisible() )
	{
		mCodeDock->toggleViewAction()->trigger();
	}
	else if ( !mScoreDock->isVisible() && !mCodeDock->isVisible() )
	{
		mScoreDock->toggleViewAction()->trigger();
	}
	else if ( mScoreDock->isVisible() )
	{
		mScoreDock->toggleViewAction()->trigger();
		mCodeDock->toggleViewAction()->trigger();
		mTextEdit->setFocus();
	}
	else
	{
		updateCode();
		mCodeDock->toggleViewAction()->trigger();
		mScoreDock->toggleViewAction()->trigger();
	}
	updateWidgetSize();
}

//-------------------------------------------------------------------------
void MainWindow::fontBigger()
{	
	changeFontSize( qMin( mTextEdit->document()->defaultFont().pointSize() + 1 , MAX_FONT_SIZE ) );
}

//-------------------------------------------------------------------------
void MainWindow::fontSmaller()
{
	changeFontSize( qMax( mTextEdit->document()->defaultFont().pointSize() - 1 , MIN_FONT_SIZE ) );
}

//-------------------------------------------------------------------------
//								Event handlers							///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
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
		if ( maybeSave() )
		{
			QString fileName = event->mimeData()->urls().first().toLocalFile();
			loadFile( fileName );
			event->accept();
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
//								Other functions							///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void MainWindow::setEngineSettings(const GuidoLayoutSettings& gls, int bbmap , bool showMapping , bool rawMapping , bool showBoxes, int voiceNum, int staffNum)
{	
	if ( (gls.systemsDistance      != mGuidoEngineParams.systemsDistance)		|
		(gls.systemsDistribution   != mGuidoEngineParams.systemsDistribution)	|
		(gls.systemsDistribLimit   != mGuidoEngineParams.systemsDistribLimit)	|
		(gls.force                 != mGuidoEngineParams.force)					|
		(gls.spring                != mGuidoEngineParams.spring)				|
		(gls.neighborhoodSpacing   != mGuidoEngineParams.neighborhoodSpacing)	|
		(gls.optimalPageFill       != mGuidoEngineParams.optimalPageFill)	    |
        (gls.resizePage2Music      != mGuidoEngineParams.resizePage2Music)	    |
        (gls.checkLyricsCollisions != mGuidoEngineParams.checkLyricsCollisions)	    |
        (gls.proportionalRenderingForceMultiplicator != mGuidoEngineParams.proportionalRenderingForceMultiplicator) )
	{
		mGuidoEngineParams = gls;
		mGuidoWidget->setGuidoLayoutSettings( mGuidoEngineParams );
		updateWidgetSize();
	}
	if ( (bbmap != mBBMap) || (showBoxes != mShowBoxes) )
	{
		mShowBoxes = showBoxes;
		int bbmap_boxes = mShowBoxes ? bbmap : 0;
		if ( bbmap_boxes != mBBMap )
		{
			mBBMap = bbmap_boxes;
			GuidoDrawBoundingBoxes(bbmap_boxes);
			mGuidoWidget->clearCache();
		}
	}
	if ( (bbmap != mGuidoWidget->fMap) || (showMapping != mShowMapping) || (rawMapping != mRawMapping) ) 
	{
		mShowMapping = showMapping;
		mRawMapping = rawMapping;
		int bbmap_mapping = mShowMapping ? bbmap : 0;
		if ((bbmap_mapping != mGuidoWidget->fMap) || (mRawMapping != mGuidoWidget->fRaw))
		{
			mBBMap = bbmap_mapping;
			mGuidoWidget->fMap = bbmap_mapping;
			mGuidoWidget->fRaw = mRawMapping;
			mGuidoWidget->update();
		}
	}
	if ( voiceNum != mVoiceNum )
	{
		mVoiceNum = voiceNum;
		mGuidoWidget->fVoiceNum = mVoiceNum;
		mGuidoWidget->update();
	}
	if ( staffNum != mStaffNum )
	{
		mStaffNum = staffNum;
		mGuidoWidget->fStaffNum = mStaffNum;
		mGuidoWidget->update();
	}
}

//-------------------------------------------------------------------------
void MainWindow::setScoreColor( const QColor& scoreColor )
{
	if ( mScoreColor != scoreColor )
	{
		mScoreColor = scoreColor;
		mGuidoWidget->setScoreColor( scoreColor );
	}
}

//-------------------------------------------------------------------------
void MainWindow::setCurrentPage(int pageIndex)
{
	int pageCount = mGuidoWidget->pageCount();
	mFirstPageAct->setEnabled( pageIndex > 1 );	
	mPrevPageAct->setEnabled( pageIndex > 1 );
	mNextPageAct->setEnabled( pageIndex < mGuidoWidget->pageCount() );
	mLastPageAct->setEnabled( pageIndex < mGuidoWidget->pageCount() );
	updatePageLabel(pageIndex , pageCount );

	if ( pageIndex < 1 )
		return;
	if ( pageIndex > pageCount )
		return;
	if ( mGuidoWidget->firstVisiblePage() == pageIndex )
		return;
	
	mGuidoWidget->setPage(pageIndex);
	updateWidgetSize();
}

//-------------------------------------------------------------------------
void MainWindow::updatePageLabel(int currentPage, int pageCount)
{
	char buffer[30];
#ifdef Qt6
	QString cp; cp.setNum(currentPage);
	QString pc; pc.setNum(pageCount);
	QString str = "Page " + cp + "/" + pc;
	mPageLabel->setText( str );
#else
	sprintf(buffer,"Page %d/%d" , currentPage , pageCount);
	mPageLabel->setText( tr(buffer) );
#endif
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
void MainWindow::changeAdjustMode( AdjustMode mode )
{
	mAdjustMode = mode;
	
	switch ( mAdjustMode ) {
		case AdjustToHeight:
			mAdjustToWidthAct->setChecked(false);
			mAdjustToHeightAct->setChecked(true);
			mAdjustToFitAct->setChecked(false);
			break;
		case AdjustToWidth:
			mAdjustToWidthAct->setChecked(true);
			mAdjustToHeightAct->setChecked(false);
			mAdjustToFitAct->setChecked(false);
			break;
		case AdjustToFit:
			mAdjustToWidthAct->setChecked(false);
			mAdjustToHeightAct->setChecked(false);
			mAdjustToFitAct->setChecked(true);
			break;
		case AdjustNone:
			mAdjustToWidthAct->setChecked(false);
			mAdjustToHeightAct->setChecked(false);
			mAdjustToFitAct->setChecked(false);
			break;
	}
}

//-------------------------------------------------------------------------
void MainWindow::scaleToWidth()
{
	int viewWidth = mScrollArea->viewport()->size().width()  - horizontalBorderMargin();
	float zoom = viewWidth / float(mBaseSize.width());
	setZoom( zoom * 100 );

	changeAdjustMode( AdjustToWidth );
}

//-------------------------------------------------------------------------
void MainWindow::scaleToHeight()
{
	int viewHeight = mScrollArea->viewport()->size().height()  - verticalBorderMargin();
	float zoom = viewHeight / float(mBaseSize.height());
	setZoom( zoom * 100 );

	changeAdjustMode( AdjustToHeight );
}

//-------------------------------------------------------------------------
void MainWindow::scaleToFit()
{
	int viewW = mScrollArea->viewport()->size().width() - horizontalBorderMargin();
	int viewH = mScrollArea->viewport()->size().height() - verticalBorderMargin();

	float viewHeightWidthRatio = viewH / float(viewW);

	int testWidth = 100;
	float scoreHeightWidthRatio = ( mGuidoWidget->heightForWidth( testWidth ) / float(testWidth) );
	if ( viewHeightWidthRatio < scoreHeightWidthRatio )
		scaleToHeight(); 
	else
		scaleToWidth();

	changeAdjustMode( AdjustToFit );
}

//-------------------------------------------------------------------------
void MainWindow::createActions()
{
    mNewAct = new QAction(QIcon(":/new.png"), tr("&New"), this);
    mNewAct->setShortcut(QKeySequence::New);
    mNewAct->setStatusTip(tr("Create a new Guido file"));
    connect(mNewAct, SIGNAL(triggered()), this, SLOT(newFile()));

    mOpenAct = new QAction(QIcon(":/open.png"), tr("&Open..."), this);
    mOpenAct->setShortcut(QKeySequence::Open);
    mOpenAct->setStatusTip(tr(QGuidoImporter::musicxmlSupported() ? "Open a Guido or MusicXML file" : "Open a Guido score file"));
    connect(mOpenAct, SIGNAL(triggered()), this, SLOT(open()));

    mSaveAct = new QAction(QIcon(":/save.png"), tr("&Save"), this);
    mSaveAct->setShortcut(QKeySequence::Save);
    mSaveAct->setStatusTip(tr("Save the Guido file"));
    connect(mSaveAct, SIGNAL(triggered()), this, SLOT(save()));

    mSaveAsAct = new QAction(QIcon(":/save.png"), tr("&Save as..."), this);	
    mSaveAsAct->setShortcut(QKeySequence::SaveAs);
    mSaveAsAct->setStatusTip(tr("Save the Guido file to..."));
    connect(mSaveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	mReloadAct = new QAction(QIcon(":/reload.png"), tr("&Reload"), this);
    mReloadAct->setShortcut(QKeySequence::Refresh);
    mReloadAct->setStatusTip(tr("Reload the current Guido score file"));
    connect(mReloadAct, SIGNAL(triggered()), this, SLOT(reload()));
	
	mExportToImageAct = new QAction(QIcon(":/export.png"), tr("&Export..."), this);
    mExportToImageAct->setShortcut(tr("Ctrl+E"));
    mExportToImageAct->setStatusTip(tr("Export the Guido score to a set of image files in a directory"));
    connect(mExportToImageAct, SIGNAL(triggered()), this, SLOT(doexport()));

    mExitAct = new QAction(tr("E&xit"), this);
    mExitAct->setShortcut(tr("Ctrl+Q"));
    mExitAct->setStatusTip(tr("Exit the application"));
    connect(mExitAct, SIGNAL(triggered()), this, SLOT(close()));

    mDocTagsAct = new QAction(tr("&Guido tags"), this);
    mDocTagsAct->setStatusTip(tr("Show the guido tags documentation"));
    connect(mDocTagsAct, SIGNAL(triggered()), this, SLOT(docTags()));

    mDocParamsAct = new QAction(tr("&Guido params"), this);
    mDocParamsAct->setStatusTip(tr("Show the guido params documentation"));
    connect(mDocParamsAct, SIGNAL(triggered()), this, SLOT(docParams()));

    mAboutAct = new QAction(tr("&About"), this);
    mAboutAct->setStatusTip(tr("Show the application's About box"));
    connect(mAboutAct, SIGNAL(triggered()), this, SLOT(about()));

    mAboutQtAct = new QAction(tr("About &Qt"), this);
    mAboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(mAboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    mSetupAct = new QAction(tr("Preferences"), this);
    mSetupAct->setStatusTip(tr("Setup the GUIDO engine parameters"));
    connect(mSetupAct, SIGNAL(triggered()), this, SLOT(preferences()));

	mPrintAct = new QAction(QIcon(":/print.png"), tr("&Print") , this);
	mPrintAct->setShortcut(QKeySequence::Print);
	mPrintAct->setStatusTip(tr("Print the score"));
	connect(mPrintAct , SIGNAL(triggered()) , this , SLOT(print()));
	
	mNextPageAct = new QAction(QIcon(":/next.png"), tr("&Next") , this);
	mNextPageAct->setShortcut(tr("PgDown"));
	mNextPageAct->setStatusTip(tr("Show next page"));
	connect(mNextPageAct , SIGNAL(triggered()) , this , SLOT(nextPage()) );
	mNextPageAct->setEnabled( false ) ;
		
	mPrevPageAct = new QAction(QIcon(":/prev.png"), tr("P&rev") , this);
	mPrevPageAct->setShortcut(tr("PgUp"));
	mPrevPageAct->setStatusTip(tr("Show previous page"));
	connect(mPrevPageAct , SIGNAL(triggered()) , this , SLOT(prevPage()) );
	mPrevPageAct->setEnabled( false ) ;
		
	mFirstPageAct = new QAction(QIcon(":/first.png"), tr("&First") , this);
	mFirstPageAct->setShortcut(tr("Home"));
	mFirstPageAct->setStatusTip(tr("Show first page"));
	connect(mFirstPageAct , SIGNAL(triggered()) , this , SLOT(firstPage()) );
	mFirstPageAct->setEnabled( false ) ;
		
	mLastPageAct = new QAction(QIcon(":/last.png"), tr("L&ast") , this);
	mLastPageAct->setShortcut(tr("End"));
	mLastPageAct->setStatusTip(tr("Show last page"));
	connect(mLastPageAct , SIGNAL(triggered()) , this , SLOT(lastPage()) );
	mLastPageAct->setEnabled( false ) ;

	mZoomInAct = new QAction(QIcon(":/zoomIn.png"), tr("Zoom &In") , this);
	mZoomInAct->setShortcut( QKeySequence::ZoomIn);
	mZoomInAct->setStatusTip(tr("Zoom in"));
	connect(mZoomInAct , SIGNAL(triggered()) , this , SLOT(zoomIn()) );
	mZoomInAct->setEnabled( false ) ;
	
	mZoomOutAct = new QAction(QIcon(":/zoomOut.png"), tr("Zoom O&ut") , this);
	mZoomOutAct->setShortcut( QKeySequence::ZoomOut);
	mZoomOutAct->setStatusTip(tr("Zoom out"));
	connect(mZoomOutAct , SIGNAL(triggered()) , this , SLOT(zoomOut()) );
	mZoomOutAct->setEnabled( false ) ;
	
	mLineWrapAct = new QAction(tr("Line wrapping") , this);
	mLineWrapAct->setStatusTip(tr("Active/Desactive line wrapping"));
	mLineWrapAct->setCheckable( true );
	connect(mLineWrapAct , SIGNAL(triggered()) , this , SLOT(lineWrapSwitch()) );

	mAdjustToWidthAct = new QAction(QIcon(":/adjustToWidth.png"), tr("Adjust to &width") , this);
	mAdjustToWidthAct->setShortcut(tr("Ctrl+W"));
	mAdjustToWidthAct->setStatusTip(tr("Adjust zoom to page width"));
	mAdjustToWidthAct->setCheckable( true );
	connect(mAdjustToWidthAct , SIGNAL(triggered()) , this , SLOT(adjustToWidthSwitch()) );
//	mAdjustToWidthAct->setEnabled( false );
	
	mAdjustToHeightAct = new QAction(QIcon(":/adjustToHeight.png"), tr("Adjust to &height") , this);
	mAdjustToHeightAct->setShortcut(tr("Ctrl+H"));
	mAdjustToHeightAct->setStatusTip(tr("Adjust zoom to page height"));
	mAdjustToHeightAct->setCheckable( true );
	connect(mAdjustToHeightAct , SIGNAL(triggered()) , this , SLOT(adjustToHeightSwitch()) );
//	mAdjustToHeightAct->setEnabled( false );
	
	mAdjustToFitAct = new QAction(QIcon(":/adjustToFit.png"), tr("Auto-adjust &mode") , this);
	mAdjustToFitAct->setShortcut(tr("Ctrl+M"));
	mAdjustToFitAct->setStatusTip(tr("Auto-adjust zoom to page"));
	mAdjustToFitAct->setCheckable( true );
	connect(mAdjustToFitAct , SIGNAL(triggered()) , this , SLOT(adjustToFitSwitch()) );
//	mAdjustToFitAct->setEnabled( false );
	
	mToggleDocksAct = new QAction(QIcon(":/toggleView.png"), tr("Toggle code/score") , this);
	mToggleDocksAct->setShortcut(tr("Ctrl+T"));
	connect( mToggleDocksAct , SIGNAL(triggered()) , this , SLOT(toggleDocks()) );
	
	mFontBiggerAct = new QAction(QIcon(":/fontBigger.png"), tr("Font bigger") , this);
//	mFontBiggerAct->setShortcut( tr("Ctrl+Alt+Shift+Plus") );
	mFontBiggerAct->setStatusTip(tr("Font bigger"));
	connect(mFontBiggerAct , SIGNAL(triggered()) , this , SLOT(fontBigger()) );

	mFontSmallerAct = new QAction(QIcon(":/fontSmaller.png"), tr("Font smaller") , this);
//	mFontSmallerAct->setShortcut( tr("Ctrl+Alt+Shift+Minus") );
	mFontSmallerAct->setStatusTip(tr("Font smaller"));
	connect(mFontSmallerAct , SIGNAL(triggered()) , this , SLOT(fontSmaller()) );
	
	mFindAct = new QAction(tr("Find") , this);
	mFindAct->setStatusTip( tr("Find in text") );
	mFindAct->setShortcut( QKeySequence::Find );
	connect(mFindAct , SIGNAL(triggered()) , mFindWidget , SLOT(find_slot()) );

	mFindNextAct = new QAction(tr("Find next") , this);
	mFindNextAct->setStatusTip( tr("Find next") );
	mFindNextAct->setShortcut( QKeySequence::FindNext );
	connect(mFindNextAct , SIGNAL(triggered()) , mFindWidget , SLOT(findNext_slot()) );
}

//-------------------------------------------------------------------------
void MainWindow::createMenus()
{
    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mSetupAct);
	mFileMenu->addAction(mNewAct);
    mFileMenu->addAction(mOpenAct);
    mFileMenu->addAction(mSaveAct);
    mFileMenu->addAction(mSaveAsAct);
	mFileMenu->addAction(mPrintAct);
	mFileMenu->addAction(mExportToImageAct);
    mFileMenu->addSeparator();
	mFileMenu->addAction(mReloadAct);
	mFileMenu->addSeparator();
	mRecentFileSeparator = mFileMenu->addSeparator();
	mFileMenu->addAction(mExitAct);

    menuBar()->addSeparator();

	mViewMenu = menuBar()->addMenu(tr("&View"));
	mViewMenu->addAction(mFirstPageAct);
	mViewMenu->addAction(mPrevPageAct);
	mViewMenu->addAction(mNextPageAct);
	mViewMenu->addAction(mLastPageAct);
	mViewMenu->addSeparator();
	mViewMenu->addAction(mZoomInAct);
	mViewMenu->addAction(mZoomOutAct);
	QMenu * zoomMenu = mViewMenu->addMenu( tr("&Zoom") );
	zoomMenu->addAction( buildZoomAction(500) );
	zoomMenu->addAction( buildZoomAction(200) );
	zoomMenu->addAction( buildZoomAction(100) );
	zoomMenu->addAction( buildZoomAction(50) );
	zoomMenu->addAction( buildZoomAction(25) );
	zoomMenu->addAction( buildZoomAction(10) );

	mViewMenu->addSeparator();
	mViewMenu->addAction(mLineWrapAct);
	mViewMenu->addAction(mFontSmallerAct);
	mViewMenu->addAction(mFontBiggerAct);
				
	mViewMenu->addSeparator();
	mViewMenu->addAction(mAdjustToWidthAct);
	mViewMenu->addAction(mAdjustToHeightAct);
	mViewMenu->addAction(mAdjustToFitAct);
	
	mEditMenu = menuBar()->addMenu(tr("&Edit"));
	mEditMenu->addAction(mFindAct);
	mEditMenu->addAction(mFindNextAct);
		
	mWindowMenu = createPopupMenu();
	mWindowMenu->setTitle( tr("&Window") );
	mWindowMenu->addSeparator();
	mWindowMenu->addAction( mToggleDocksAct );
	menuBar()->addMenu( mWindowMenu );
	
    mHelpMenu = menuBar()->addMenu(tr("&Help"));
	QMenu * docMenu = mHelpMenu->addMenu(tr("&Guido documentation") );
	docMenu->addAction(mDocTagsAct);
	docMenu->addAction(mDocParamsAct);
    mHelpMenu->addAction(mAboutAct);
    mHelpMenu->addAction(mAboutQtAct);
}

//-------------------------------------------------------------------------
void MainWindow::createToolBars()
{
    mFileToolBar = addToolBar(tr("File"));
	mFileToolBar->addAction(mNewAct);
	mFileToolBar->addAction(mOpenAct);
	mFileToolBar->addAction(mSaveAct);
	mFileToolBar->addAction(mPrintAct);
	mFileToolBar->addAction(mExportToImageAct);
	mFileToolBar->addSeparator();
	mFileToolBar->addAction(mReloadAct);
	mFileToolBar->setIconSize( QSize(TOOL_BAR_ICON_SIZE,TOOL_BAR_ICON_SIZE) );
	mFileToolBar->setObjectName("File");
	mFileToolBar->setVisible( DEFAULT_TOOL_BAR_VISIBILITY );
	
	mPageToolBar = addToolBar(tr("Page"));
	mPageToolBar->addAction(mFirstPageAct);
	mPageToolBar->addAction(mPrevPageAct);
	mPageToolBar->addAction(mNextPageAct);
	mPageToolBar->addAction(mLastPageAct);
	mPageToolBar->setIconSize( QSize(TOOL_BAR_ICON_SIZE,TOOL_BAR_ICON_SIZE) );
	mPageToolBar->setObjectName("Page");
	mPageToolBar->setVisible( DEFAULT_TOOL_BAR_VISIBILITY );

	mZoomToolBar = addToolBar(tr("Zoom"));
	mZoomToolBar->addAction(mZoomInAct);
	mZoomToolBar->setIconSize( QSize(TOOL_BAR_ICON_SIZE,TOOL_BAR_ICON_SIZE) );
		
	mZoomSpinBox = new QSpinBox(mZoomToolBar);
	mZoomSpinBox->setMaximum( MAXIMUM_ZOOM );
	mZoomSpinBox->setMinimum( MINIMUM_ZOOM );
	mZoomSpinBox->setValue(100);
	mZoomSpinBox->setAccelerated(true);
	mZoomSpinBox->setKeyboardTracking(false);
	connect( mZoomSpinBox , SIGNAL(valueChanged(int)) , this , SLOT(spinBoxZoom(int)) );
	mZoomToolBar->addWidget(mZoomSpinBox);

	mZoomToolBar->addAction(mZoomOutAct);
	mZoomToolBar->addSeparator();
	mZoomToolBar->addAction(mAdjustToWidthAct);
	mZoomToolBar->addAction(mAdjustToHeightAct);
	mZoomToolBar->addAction(mAdjustToFitAct);
	mZoomToolBar->addAction(mFontSmallerAct);
	mZoomToolBar->addAction(mFontBiggerAct);
	mZoomToolBar->addAction(mToggleDocksAct);
	mZoomToolBar->setObjectName("Zoom");
	mZoomToolBar->setVisible( DEFAULT_TOOL_BAR_VISIBILITY );
}

//-------------------------------------------------------------------------
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
	statusBar()->addPermanentWidget( mPageLabel );
}

//-------------------------------------------------------------------------
QAction * MainWindow::buildZoomAction(int zoom)
{
	QVariant variant( zoom );
	QAction * action = new QAction( variant.toString() +"%" , this );
	action->setStatusTip( "Set zoom to " + QVariant(zoom).toString() + "%");
	action->setData( zoom );
	connect(action , SIGNAL(triggered()) , this , SLOT(setZoom()) );
	return action;
}

//-------------------------------------------------------------------------
void MainWindow::readSettings()
{
	QCoreApplication::setOrganizationName(COMPANY_NAME);
	QCoreApplication::setApplicationName(APP_NAME);

    QSettings settings;
    QPoint pos = settings.value(POS_SETTING, QPoint(200, 200)).toPoint();
    QSize winSize = settings.value(SIZE_SETTING, QSize(DEFAULT_WINDOW_WIDTH , DEFAULT_WINDOW_HEIGHT)).toSize();
	QByteArray state = settings.value( WINDOW_STATE_SETTING ).toByteArray();
	bool lineWrapping = settings.value( LINE_WRAPPING_SETTING ).toBool();
	int size = settings.beginReadArray(RECENT_FILES_SETTING);
	for (int i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		mRecentFiles <<  settings.value(RECENT_FILE_NAME_SETTING).toString();
	}
	settings.endArray();
	AdjustMode adjustMode = (AdjustMode)settings.value(ADJUST_MODE_SETTING , AdjustToFit ).toInt();
	int fontSize = settings.value( FONT_SIZE_SETTING , 15 ).toInt();
	
	// Guido Mapping
	int bbmap = settings.value( BBMAP_SETTING , 0 ).toInt();
	bool showBoxes = settings.value( SHOW_BOXES_SETTING , false ).toBool();
	bool showMapping = settings.value( SHOW_MAPPING_SETTING , true ).toBool();
	bool rawMapping = settings.value( RAW_MAPPING_SETTING , true ).toBool();
	
	int voiceNum = settings.value( VOICE_NUM_SETTING, -1 ).toInt();
	int staffNum = settings.value( STAFF_NUM_SETTING, -1 ).toInt();
		
	GuidoLayoutSettings guidoLayoutSettings;
	if (settings.childGroups().contains(ENGINE_SETTING)) {
		settings.beginGroup( ENGINE_SETTING );
		guidoLayoutSettings.neighborhoodSpacing   = settings.value( NEIGHBORHOOD_SPACING_SETTING,   0 ).toInt();
		guidoLayoutSettings.spring                = settings.value( SPRING_SETTING,                 0 ).toDouble();
		guidoLayoutSettings.force                 = settings.value( FORCE_SETTING,                  0 ).toDouble();
		guidoLayoutSettings.systemsDistribLimit   = settings.value( SYSTEMS_DISTRIB_LIMIT_SETTING,  0 ).toDouble();
		guidoLayoutSettings.systemsDistribution   = settings.value( SYSTEMS_DISTRIBUTION_SETTING,   0 ).toInt();
		guidoLayoutSettings.systemsDistance       = settings.value( SYSTEMS_DISTANCE_SETTING,       0 ).toDouble();
		guidoLayoutSettings.optimalPageFill       = settings.value( OPTIMAL_PAGE_FILL_SETTING,      0 ).toInt();
		guidoLayoutSettings.resizePage2Music      = settings.value( RESIZE_PAGE_2_MUSIC_SETTING,    0 ).toInt();
		guidoLayoutSettings.checkLyricsCollisions = settings.value( CHECK_LYRICS_SETTING,			0 ).toBool();
        guidoLayoutSettings.proportionalRenderingForceMultiplicator = settings.value( PROPORTIONAL_RENDERING_SETTING, 0 ).toFloat();
		settings.endGroup();
	}
	else
		GuidoGetDefaultLayoutSettings (&guidoLayoutSettings);
	
	settings.beginGroup( SYNTAX_HIGHLIGHTER_SETTING );
	for ( int i = 0 ; i < GuidoHighlighter::SIZE ; i++ )
	{
		settings.beginGroup( QVariant(i).toString() );
		QColor fontColor = settings.value( FONT_COLOR_SETTING , mDefaultFontColors[i] ).value<QColor>();
		QFont::Weight weight = (QFont::Weight)settings.value( FONT_WEIGHT_SETTING , mDefaultFontWeights[i] ).toInt();
		mGuidoHighlighter->addRule( i , fontColor , weight );
		settings.endGroup();
	}
	settings.endGroup();

	mFindWidget->readSettings(settings);
	
	QColor black = Qt::black;
	QVariant color = black;
	QColor scoreColor = settings.value( SCORE_COLOR_SETTING, color).value<QColor>();
	
	setLineWrap(lineWrapping);
    resize(winSize);
    move(pos);
	restoreState( state );
	updateRecentFilesMenu();
	changeAdjustMode( adjustMode );
	changeFontSize( fontSize );
	setScoreColor( scoreColor );
//	mGuidoEngineParams = guidoLayoutSettings;
	mBBMap=-1; mShowBoxes=!showBoxes; mShowMapping=!showMapping; mVoiceNum=voiceNum-1; mStaffNum=staffNum-1; //Unset values.
	setEngineSettings( guidoLayoutSettings, bbmap, showMapping, rawMapping, showBoxes, voiceNum, staffNum );
}

//-------------------------------------------------------------------------
void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue(POS_SETTING, pos());
    settings.setValue(SIZE_SETTING, size());
	settings.setValue( WINDOW_STATE_SETTING, saveState() );	
	settings.setValue( LINE_WRAPPING_SETTING , mLineWrapAct->isChecked() );
	
	settings.beginWriteArray(RECENT_FILES_SETTING);
	for (int i = 0; i < mRecentFiles.size(); ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue(RECENT_FILE_NAME_SETTING, mRecentFiles[i]);
	}
	settings.endArray();
	settings.setValue( ADJUST_MODE_SETTING , mAdjustMode );
	settings.setValue( FONT_SIZE_SETTING , mTextEdit->document()->defaultFont().pointSize() );
	// Guido Mapping
	settings.setValue( BBMAP_SETTING , mBBMap );
	settings.setValue( SHOW_BOXES_SETTING , mShowBoxes );
	settings.setValue( SHOW_MAPPING_SETTING , mShowMapping );
	settings.setValue( RAW_MAPPING_SETTING , mRawMapping );
	settings.setValue( VOICE_NUM_SETTING, mVoiceNum );
	settings.setValue( STAFF_NUM_SETTING, mStaffNum );
	
	// Guido Engine
	settings.beginGroup( ENGINE_SETTING );
	settings.setValue( NEIGHBORHOOD_SPACING_SETTING,   mGuidoEngineParams.neighborhoodSpacing );
	settings.setValue( SPRING_SETTING,                 mGuidoEngineParams.spring );
	settings.setValue( FORCE_SETTING,                  mGuidoEngineParams.force );
	settings.setValue( SYSTEMS_DISTRIB_LIMIT_SETTING,  mGuidoEngineParams.systemsDistribLimit );
	settings.setValue( SYSTEMS_DISTRIBUTION_SETTING,   mGuidoEngineParams.systemsDistribution );
	settings.setValue( SYSTEMS_DISTANCE_SETTING,       mGuidoEngineParams.systemsDistance );
	settings.setValue( OPTIMAL_PAGE_FILL_SETTING,      mGuidoEngineParams.optimalPageFill );
    settings.setValue( RESIZE_PAGE_2_MUSIC_SETTING,    mGuidoEngineParams.resizePage2Music );
    settings.setValue( PROPORTIONAL_RENDERING_SETTING, mGuidoEngineParams.proportionalRenderingForceMultiplicator );
	settings.setValue( CHECK_LYRICS_SETTING ,		   mGuidoEngineParams.checkLyricsCollisions);

	settings.endGroup();
	
	settings.setValue( SCORE_COLOR_SETTING , mScoreColor );

	settings.beginGroup( SYNTAX_HIGHLIGHTER_SETTING );
	for ( int i = 0 ; i < GuidoHighlighter::SIZE ; i++ )
	{
		settings.beginGroup( QVariant(i).toString() );
		settings.setValue( FONT_COLOR_SETTING , mGuidoHighlighter->color(i) );
		settings.setValue( FONT_WEIGHT_SETTING , mGuidoHighlighter->weight(i) );
		settings.endGroup();
	}
	settings.endGroup();

	mFindWidget->writeSettings(settings);
}

//-------------------------------------------------------------------------
void MainWindow::updateWidgetSize()
{
	mBaseSize = mGuidoWidget->pageSizeMM( mGuidoWidget->firstVisiblePage() ) * (this->width() / float(this->widthMM()));
	switch ( mAdjustMode )
	{
		case AdjustToHeight :
			scaleToHeight();
			break;
		case AdjustToWidth :
			scaleToWidth();
			break;
		case AdjustToFit :
			scaleToFit();
			break;
		case AdjustNone :	
			setZoom(mScaleFactor);
			break;
	}
	updatePageLabel(mGuidoWidget->firstVisiblePage() , mGuidoWidget->pageCount() );
}

//-------------------------------------------------------------------------
void MainWindow::updateRecentFilesMenu()
{
	while ( mRecentFileActions.size() )
	{
		delete mRecentFileActions[0];
		mRecentFileActions.removeAt(0);
	}
	
	for ( int i = int(mRecentFiles.size()-1) ; i >=0 ; i-- )
	{
		QAction * action = new QAction( QVariant(mRecentFiles.size() - i).toString() + ". " + QFileInfo(mRecentFiles[i]).fileName() , mFileMenu );
		connect( action, SIGNAL(triggered()) , this , SLOT(openRecentFile()) );
		action->setData( mRecentFiles[i] );
		mFileMenu->insertAction( mRecentFileSeparator , action );
		mRecentFileActions << action;
	}
}

//-------------------------------------------------------------------------
bool MainWindow::loadFile(const QString &fileName)
{
	if ( !QFile::exists(fileName) )
	{
		statusBar()->showMessage("File "+fileName+" doesn't exist.", 2000);
		return false;
	}

	statusBar()->showMessage(tr(QString("Loading " + fileName + "...").toUtf8().data()));
    QApplication::setOverrideCursor(Qt::WaitCursor);

	reinitGuidoWidget();
    
    setCurrentFile(fileName.toUtf8().data());

	bool loadOk = mGuidoWidget->setGMNFile( fileName );
	if (!loadOk && QGuidoImporter::musicxmlSupported()) {	// try to import file as MusicXML file
		stringstream out;
		if ( QGuidoImporter::musicxmlFile2Guido(fileName.toUtf8().constData(), true, out) )
		{
			loadOk = mGuidoWidget->setGMNCode( out.str().c_str(),  filePath());
		}
	}
	mLastModified = QFileInfo(fileName).lastModified();
	
	mTextEdit->setPlainText(mGuidoWidget->gmnCode());

	if ( loadOk )
	{
		mBaseSize = mGuidoWidget->pageSizeMM( mGuidoWidget->firstVisiblePage() ) * (this->width() / float(this->widthMM()));
		firstPage();
		updateWidgetSize();
		mTextEdit->highlightErrorLine( 0 );
		statusBar()->showMessage(tr("File loaded"), 2000);
		showTime (mGuidoWidget);
	}
	else
	{
		QString errorMessage = "Invalid GMN file : " + mGuidoWidget->getLastErrorMessage();
//		mGuidoWidget->resize( QSize( 0,0 ) );
        int line;
        int col;
        mGuidoWidget->getLastParseErrorLine(line, col);
		mTextEdit->highlightErrorLine(line);
		statusBar()->showMessage(tr(errorMessage.toUtf8().data()), 2000);
	}

	QApplication::restoreOverrideCursor();
// call moved to the beginning of the function to benefit of the filePath() method
//	setCurrentFile(fileName.toUtf8().data());

    // In order not to make appear the little star next to filename (at the top of the window)
    mTextEdit->document()->setModified( false );
    setWindowModified(false);
	mHiddenState.reset();
	if (mSetup) mSetup->setDisplayState (mHiddenState);
	recentFileListUpdate(fileName);
	return loadOk;
}

//-------------------------------------------------------------------------
void MainWindow::setCurrentFile(const QString &fileName)
{
	unplugFileWatcher();
    mCurFile = fileName;
	if ( mCurFile.length() )
		mFileSystemWatcher->addPath( mCurFile );

	setWindowModified(false);

	mReloadAct->setEnabled( !mCurFile.isEmpty() );

    QString shownName;
	shownName = strippedName(mCurFile);

	setWindowTitle( shownName + "[*] - " + APP_NAME );
}

//-------------------------------------------------------------------------
void MainWindow::setLineWrap( bool on )
{
	if ( on )
		mTextEdit->setLineWrapMode( QPlainTextEdit::WidgetWidth );
	else
		mTextEdit->setLineWrapMode( QPlainTextEdit::NoWrap );
		
	if ( on != mLineWrapAct->isChecked() )
		mLineWrapAct->setChecked( on );
}

//-------------------------------------------------------------------------
QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

//-------------------------------------------------------------------------
bool MainWindow::maybeSave()
{
    if (mTextEdit->document()->isModified())
	{
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, APP_NAME,
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

//-------------------------------------------------------------------------
bool MainWindow::saveFile(const QString &fileName)
{
	unplugFileWatcher();

	{
		QFile file(fileName);
		if (!file.open(QFile::WriteOnly | QFile::Text)) 
		{
			QMessageBox::warning(this, APP_NAME,
                            tr("Cannot write file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
			return false;
		}
		{
			QTextStream out(&file);
			QApplication::setOverrideCursor(Qt::WaitCursor);
			out << mTextEdit->toPlainText();
			QApplication::restoreOverrideCursor();
		}
	}
	mLastModified = QFileInfo(fileName).lastModified();

	mTextEdit->document()->setModified( false );
    setCurrentFile(fileName);
	
	recentFileListUpdate(fileName);
	
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
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

//-------------------------------------------------------------------------
void MainWindow::unplugFileWatcher()
{
	if ( mCurFile.length() )
		mFileSystemWatcher->removePath( mCurFile );
}

//-------------------------------------------------------------------------
void MainWindow::recentFileListUpdate(const QString& fileName)
{
	// Recent files menu update
	if ( !mRecentFiles.size() )
		mRecentFiles << fileName;
	else if ( !mRecentFiles.contains( fileName ) )
		mRecentFiles << fileName;
	else
	{
		mRecentFiles.removeAll( fileName );
		mRecentFiles << fileName;
	}
	if ( mRecentFiles.size() > 5 )
		mRecentFiles.removeAt(0);
	updateRecentFilesMenu();
}

//-------------------------------------------------------------------------
void MainWindow::changeFontSize( float newFontPointSize )
{
	bool modified = mTextEdit->document()->isModified();

//	QFont newFont = mTextEdit->document()->defaultFont();
//	QFont newFont ( "Courier new", newFontPointSize);  // Courier new seems to be buggy - incorrect rendering
//	QFont newFont ( "Courier", newFontPointSize);
	QFont newFont ( "Monaco", newFontPointSize);
	newFont.setPointSize( newFontPointSize );
	mTextEdit->document()->setDefaultFont( newFont  );

	int vValue = mTextEdit->verticalScrollBar()->value();
	int hValue = mTextEdit->horizontalScrollBar()->value();
	mTextEdit->setPlainText( mTextEdit->toPlainText() );
	mTextEdit->verticalScrollBar()->setValue( vValue );
	mTextEdit->horizontalScrollBar()->setValue( hValue );
	
	mTextEdit->document()->setModified( modified );
	setWindowModified( mTextEdit->document()->isModified() );
}

//-------------------------------------------------------------------------
void MainWindow::reinitGuidoWidget()
{
//	if ( mGuidoWidget )
//			delete mGuidoWidget;
	if ( !mGuidoWidget )
		mGuidoWidget = new MapGuidoWidget();
//	mGuidoWidget->setCursor( QCursor( Qt::OpenHandCursor ) );
	mGuidoWidget->setGuidoLayoutSettings(mGuidoEngineParams);
	mGuidoWidget->fMap = mBBMap;
	mGuidoWidget->fVoiceNum = mVoiceNum;
	mGuidoWidget->fStaffNum = mStaffNum;
	mGuidoWidget->setScoreColor( mScoreColor );
	mScrollArea->setWidget( mGuidoWidget );
}

//-------------------------------------------------------------------------
void MainWindow::setHighlighter( int syntaxElementId, const QColor& color , int weight )
{
	mGuidoHighlighter->setDocument(0);
	
	GuidoHighlighter * highlighter = new GuidoHighlighter();
	
	for ( int i = 0 ; i < GuidoHighlighter::SIZE ; i++ )
	{
		if ( i == syntaxElementId )
		{
			highlighter->addRule( i , color , QFont::Weight(weight) );			
		}
		else
		{
			highlighter->addRule( i , mGuidoHighlighter->color(i) , QFont::Weight(mGuidoHighlighter->weight(i)) );
		}
	}
	
	setHighlighter(highlighter);
}

//-------------------------------------------------------------------------
void MainWindow::setHighlighter( GuidoHighlighter * highlighter )
{
	delete mGuidoHighlighter;
	mGuidoHighlighter = highlighter;
	highlighter->setDocument( mTextEdit->document() );
}

//-------------------------------------------------------------------------
void MainWindow::addFileDirectoryPathToARHandler(const std::string filePath, const std::string exFilePath)
{
    CARHandler currentARHandler = mGuidoWidget->getARHandler();

    size_t lastSlashPosition = filePath.find_last_of("/");

    std::string baseFilePath("");
    baseFilePath.append(filePath, 0, lastSlashPosition);

    std::vector<std::string> paths;
    GuidoGetSymbolPath((const ARHandler)currentARHandler, paths);

    if (std::find(paths.begin(), paths.end(), baseFilePath) == paths.end())
    {
        // - Remove ex current directory
        lastSlashPosition = exFilePath.find_last_of("/");

        std::string baseExFilePath("");
        baseExFilePath.append(exFilePath, 0, lastSlashPosition);

        std::vector<std::string>::iterator pos = std::find(paths.begin(), paths.end(), baseExFilePath);
        if (pos != paths.end())
            paths.erase(pos);
        // -----------------------------

        paths.push_back(baseFilePath);
        GuidoSetSymbolPath((ARHandler)currentARHandler, paths);
    }
}

//-------------------------------------------------------------------------
void MainWindow::reinitARHandlerPath()
{
    CARHandler currentARHandler = mGuidoWidget->getARHandler();

    std::vector<std::string> pathsVector;

    // - Home directory
    std::string homePath("");

#ifdef WIN32
    // For windows
    homePath.append(getenv("HOMEDRIVE"));
    homePath.append(getenv("HOMEPATH"));
#else
    // For unix
    homePath.append(getenv("HOME"));
#endif

    homePath.append("\\");

    pathsVector.push_back(homePath);

    GuidoSetSymbolPath((ARHandler)currentARHandler, pathsVector);
}
