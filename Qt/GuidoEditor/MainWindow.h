/*
 * MainWindow.h
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include <QDialog>
#include <QSpinBox>
#include <QFileSystemWatcher>
#include <QPlainTextEdit>
#include <QTimer>
#include <QDateTime>
#include <QSet>

#include "GuidoHighlighter.h"
#include "QGuidoWidget.h"
#include "GUIDOEngine.h"
#include "TRect.h"
#include "GUIDOScoreMap.h"
#include "QFindWidget.h"

class QAction;
class QMenu;
class QScoreDockWidget;
class CodeEditor;

#define ALL_STAFF -1000
#define ALL_VOICE -1000
//-------------------------------------------------------------------------
class MapGuidoWidget : public QGuidoWidget, public MapCollector
{
	public:

		int fMap;
		int fVoiceNum, fStaffNum;

				 MapGuidoWidget() :fMap(kNoBB) {}
		virtual ~MapGuidoWidget() {};		

		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos );

	protected:
		void paintEvent(QPaintEvent * event);
		
		bool showStaff(int staffNum) const;
		bool showVoice(int voiceNum) const;
};

//-------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

	friend class QScoreDockWidget;

public:
	MainWindow();
	~MainWindow();
	
	const GuidoLayoutSettings&	getEngineSettings() const		{ return mGuidoEngineParams; }
	void				setEngineSettings(const GuidoLayoutSettings& gls, int bbmap , bool showMapping , bool showBoxes, int voiceNum, int staffNum);
	bool				getShowMapping() const { return mShowMapping; }
	bool				getShowBoxes() const { return mShowBoxes; }
	int					getBBMap() const { return mBBMap; }
	int					getVoiceNum() const { return mVoiceNum; }
	int					getStaffNum() const { return mStaffNum; }
	void				setScoreColor(const QColor& c);
	const QColor&		getScoreColor() { return mScoreColor; }
	GuidoHighlighter*	getHighlighter() { return mGuidoHighlighter; }
	void				setHighlighter( int syntaxElementId, const QColor& color , int weight );
	void				setHighlighter( GuidoHighlighter * highlighter );

	static QColor	mDefaultFontColors[ GuidoHighlighter::SIZE ];
	static int		mDefaultFontWeights[ GuidoHighlighter::SIZE ];
	
private slots:
	void newFile();
	bool save();
	bool saveAs();
	void open();
	void openRecentFile();
	void reload();
    void documentWasModified();
	void updateCode();
	void lineWrapSwitch();
	void doexport();
	void about();
	void preferences();
	void print();
	void nextPage();
	void prevPage();
	void firstPage();
	void lastPage();
	void zoomIn();
	void zoomOut();
	void spinBoxZoom(int percent);
	void setZoom();
	void adjustToWidthSwitch();
	void adjustToHeightSwitch();
	void adjustToFitSwitch();
	void fileChanged(const QString& file);
	void toggleDocks();
	void fontSmaller();
	void fontBigger();
	
private:

	// Event handlers
	void closeEvent(QCloseEvent *event);
//	void resizeEvent ( QResizeEvent * event );
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	bool eventFilter(QObject *obj, QEvent *event);
	void exportToImage( QGuidoPainter * painter, const QString& filename);
	void exportToPdf( QGuidoPainter * painter, const QString& filename);
	void exportToSVG( QGuidoPainter * painter, const QString& filename);
	void exportToMidi( CARHandler ar, const QString& filename);
	void print(QGuidoPainter * guidoPainter, QPrinter& printer);

	// AdjustMode defines zoom auto-adjustement modes :
	typedef enum  AdjustMode {
		AdjustNone=0 ,	// no auto-adjustement
		AdjustToWidth,	// zoom is adjusted so that the page's width fits the viewport's width.
		AdjustToHeight,	// zoom is adjusted so that the page's height fits the viewport's height.
		AdjustToFit		// zoom is adjusted so that the page fits the viewport.
	} AdjustMode;

	// Widgets initialization methods.
    void		createActions();
    void		createMenus();
    void		createToolBars();
    void		createStatusBar();
	QAction *	buildZoomAction(int zoom);
	// Settings methods.
    void	readSettings();
    void	writeSettings();
	// File-related methods.
    bool	loadFile(const QString &fileName);
    void	setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
	bool	maybeSave();
	bool	saveFile(const QString &fileName);
	// Update page index label in the status bar.
	void	updatePageLabel(int currentPage, int pageCount);
	void	setCurrentPage(int pageIndex);
	// Score zoom & adjust-mode methods.
	void setZoom(int percent);
	void	changeAdjustMode( AdjustMode mode );
	static float	getRatio(QScrollBar *scrollBar);
	static void		setRatio(QScrollBar *scrollBar, float ratio);
	void	scaleToWidth();
	void	scaleToHeight();
	void	scaleToFit();
	void	updateWidgetSize();
	
	void	updateRecentFilesMenu();
	void	setLineWrap( bool on );
	void	unplugFileWatcher();
	void	recentFileListUpdate(const QString& fileName);
	void	changeFontSize( float newFontPointSize );
	void	reinitGuidoWidget();
	
	int horizontalBorderMargin();
	int verticalBorderMargin();
		
    QString			mCurFile;
	MapGuidoWidget*	mGuidoWidget;
	QScrollArea *	mScrollArea;
	QLabel *		mPageLabel;
	AdjustMode		mAdjustMode;
	int				mScaleFactor;
	GuidoLayoutSettings mGuidoEngineParams;
	bool			mShowBoxes;
	bool			mShowMapping;
	int				mBBMap;
	int				mVoiceNum, mStaffNum;
	QColor			mScoreColor;
	QSizeF			mBaseSize;
	QFileSystemWatcher	*mFileSystemWatcher;
	QList<QString>	mRecentFiles;
	QList<QAction*> mRecentFileActions;
	CodeEditor*		mTextEdit;
	QTimer*			mTextEditTimer;
	QDockWidget*	mCodeDock;
	QDockWidget*	mScoreDock;
	GuidoHighlighter *mGuidoHighlighter;
	QFindWidget*	mFindWidget;
	QDateTime		mLastModified;

    QMenu *mFileMenu;
    QMenu *mHelpMenu;
    QMenu *mViewMenu;
	QMenu *mEditMenu;
	QMenu *mWindowMenu;
    QMenu *mRecentFileMenu;
    QToolBar *mFileToolBar;
    QToolBar *mPageToolBar;
	QToolBar *mZoomToolBar;
    QAction *mNewAct;
    QAction *mOpenAct;
	QAction *mSaveAct;
	QAction *mSaveAsAct;
    QAction *mReloadAct;
	QAction *mExportToImageAct;
	QAction *mPrintAct;
    QAction *mExitAct;
    QAction *mAboutAct;
    QAction *mAboutQtAct;
    QAction *mSetupAct;
    QAction *mNextPageAct;
    QAction *mPrevPageAct;
    QAction *mFirstPageAct;
    QAction *mLastPageAct;
	QAction *mZoomInAct;
	QAction *mZoomOutAct;
	QAction *mAdjustToWidthAct;
	QAction *mAdjustToHeightAct;
	QAction *mAdjustToFitAct;
	QAction *mLineWrapAct;
	QAction *mToggleDocksAct;
	QAction *mFontBiggerAct;
	QAction *mFontSmallerAct;
	QAction *mRecentFileSeparator;
	QAction *mFindAct;
	QAction *mFindNextAct;
	
	QSpinBox *mZoomSpinBox;
};

#endif
