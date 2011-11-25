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

class QGuidoItemContainerFactory;

#include "GraphicsSceneMainWindow.h"

class QPaletteItem;
enum CombinationType
{
	COMBINATION_UNDER_ALIGN_LEFT	= 0	, 
	COMBINATION_UNDER_ALIGN_RIGHT	, 
	COMBINATION_OVER_ALIGN_LEFT		, 
	COMBINATION_OVER_ALIGN_RIGHT	, 
	COMBINATION_AFTER				, 
	COMBINATION_BEFORE				,
	COMBINATION_UNDEFINED			,
	COMBINATION_TAIL				,
	COMBINATION_HEAD				,
	COMBINATION_VOICE_HEAD			,
	COMBINATION_VOICE_TAIL			,
	COMBINATION_INTERLEAVE			,
	COMBINATION_TRANSPOSE			,
	COMBINATION_DURATION			,
	COMBINATION_RYTHM				,
	COMBINATION_PITCH
};

class QGuidoItemContainer;
class GuidoHighlighter;

class MainWindow : public GraphicsSceneMainWindow
{
	Q_OBJECT

	friend class GuidoGraphicsView;
	
public:
	MainWindow();
	void adaptActions();
	~MainWindow();
	
protected Q_SLOTS:
	void exportToImage();
	void about();
	void itemRemoved( QLanguageItem * item );
	void addTextItem();
	void addItemFromAnyFile();
	void setPageA4Portrait();
	void setPageA3Portrait();
	void setPageA4Landscape();
	void setPageA3Landscape();

	
#ifdef USES_GUIDO_AR
	void playItem();
	void pauseItem();
	void stopItem();
	void exportItemToMidi();
	void combineItems( QGraphicsItem * droppedItem , QGraphicsItem* targetItem , int,int , int interactionId );
	void changeActivePalette(int index);
#endif

	void exportItem();
	
protected:
	
	// Widgets initialization methods.
    void	createActions();
    void	createMenus();
    void	createToolBars();
	
	virtual void setupNAddItem(QLanguageItem* decorator);	//A compléter dans la MainWindow derivée.

	void updateWindowState();

	// UI events handlers.
	void keyPressEvent ( QKeyEvent * event );
	void keyReleaseEvent ( QKeyEvent * event );

	// Ends the "export To Image" process, resetting the state of the window back to normal. If (cancel==true), the export isn't done.
	void	finalizeExport(bool cancel);
	// Render the exportRect (in scene coordinates) to an image (opens a "Save as..." dialog).

	//Render the exportRect (in scene coordinates) to the fileName.
	void	exportRectDialog(const QRectF &exportRectangle);
	void	exportRect(const QRectF &exportRectangle , const QString& fileName , const QString& filter);
	void	paintSceneRect( const QRectF& exportRectangle , QPainter * painter );	

	void exportFirstItem();
	void exportItem(QGuidoItemContainer * item);

	//Specified QItemResizer setup (Pens,Brushes,shape)
	QItemResizer* plugResizer(QLanguageItem * itemContainer);

	//ExportState defines the 4 states of the process "export a portion of the mGraphicsView to an image".
	typedef enum ExportState { 
		None ,				// Idle state : waiting for the user to press key "Alt".
		WaitRectStart ,		// First state : now waiting for the user to define the 
							//				top-left point of the export rectangle
		WaitRectEnd ,		// Second state: now waiting for the user to define the
							//				bottom-right point of the export rectangle
		WaitValidation		// Last state : now waiting for the user to confirm/cancel
							//				the export.
	} ExportState;

	QGraphicsRectItem *				mExportWindow;
	QPointF							mExportWindowStartPoint;
	ExportState						mExportState;
		
	QAction *mExportAct;
	QAction *mAddTextAct;
	QAction *mAddFromAnyFileAct;
	QAction *mSetPageA4PortraitAct;
	QAction *mSetPageA3PortraitAct;
	QAction *mSetPageA4LandscapeAct;
	QAction *mSetPageA3LandscapeAct;

	
	QGuidoItemContainerFactory * mGuidoItemContainerFactory;

	GuidoHighlighter * mGuidoHighlighter;
	
#ifdef USES_GUIDO_AR
	void controlPlayingContainer( QGraphicsItem * container );
	
	QAction *mPlayAct;
    QAction *mPauseAct;
    QAction *mStopAct;	

	QToolBar *mPlayToolBar;
	QLanguageItem * mPlayingContainer;

	QMenu		*mCombinationMenu;
	QToolBar	*mCombinationToolBar;
	QList<QPaletteItem*> mPalettes;
#endif

};

#include "MainWindowObjects.h"
//------------------------------------------------------------------------------------------------------------------------
class GuidoGraphicsView : public LanguageGraphicsView 
{
	protected:
		void mousePressEvent(QMouseEvent * event);
		void mouseMoveEvent(QMouseEvent * event);
};

#endif
