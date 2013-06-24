/*
 * MainWindow.h
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include <QDialog>
#include <QMenu>

#include "QGuidoWidget.h"
#include "GUIDOEngine.h"
#include "TRect.h"
#include "GUIDOScoreMap.h"

class QAction;
class QMenu;


//-------------------------------------------------------------------------
class MapGuidoWidget : public QGuidoWidget, public MapCollector
{
	public:
		int		fMap;

				 MapGuidoWidget() :fMap(kNoBB) {}
		virtual ~MapGuidoWidget() {};		

		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos );

	protected:
		void paintEvent(QPaintEvent * event);
};

//-------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	
	const GuidoLayoutSettings&	getEngineSettings() const		{ return mGuidoEngineParams; }
	void						setEngineSettings(const GuidoLayoutSettings&, int bbmap);

private slots:
	void open();
	void reload();
	void exportToImage();
	void about();
	void preferences();
	void print();
	void nextPage();
	void prevPage();
	void firstPage();
	void lastPage();
	void zoomIn();
	void zoomOut();
	void adjustToWidthSwitch();
	void adjustToHeightSwitch();
	
private:

	// AdjustMode defines zoom auto-adjustement modes :
	typedef enum  AdjustMode {
		AdjustNone ,	// no auto-adjustement
		AdjustToWidth,	// zoom is adjusted so that the page's width fits the viewport's width.
		AdjustToHeight	// zoom is adjusted so that the page's height fits the viewport's height.
	} AdjustMode;

	// Widgets initialization methods.
    void	createActions();
    void	createMenus();
    void	createToolBars();
    void	createStatusBar();
	// Settings methods.
    void	readSettings();
    void	writeSettings();
	// File-loading methods.
    bool	loadFile(const QString &fileName);
    void	setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
	// Update page index label in the status bar.
	void	updatePageLabel(int currentPage, int pageCount);
	void	setCurrentPage(int pageIndex);
	// Score zoom & adjust-mode methods.
	void	scaleImage(double factor);
	void	changeAdjustMode( AdjustMode mode );
	static float	getRatio(QScrollBar *scrollBar);
	static void		setRatio(QScrollBar *scrollBar, float ratio);
	void	scaleToWidth();
	void	scaleToHeight();
	void	updateWidgetSize();
	
	int horizontalBorderMargin();
	int verticalBorderMargin();
	
	void resizeEvent ( QResizeEvent * event );
	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);
	bool eventFilter(QObject *obj, QEvent *event);
		
    QString			mCurFile;
	MapGuidoWidget *mGuidoWidget;
	QScrollArea *	mScrollArea;
	QLabel *		mPageLabel;
	AdjustMode		mAdjustMode;
	float			mScaleFactor;
	GuidoLayoutSettings mGuidoEngineParams;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *viewMenu;
    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    QAction *openAct;
    QAction *reloadAct;
	QAction *exportToImageAct;
	QAction *printAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *setupAct;
    QAction *nextPageAct;
    QAction *prevPageAct;
    QAction *firstPageAct;
    QAction *lastPageAct;
	QAction *zoomInAct;
	QAction *zoomOutAct;
	QAction *adjustToWidthAct;
	QAction *adjustToHeightAct;	
};

#endif
