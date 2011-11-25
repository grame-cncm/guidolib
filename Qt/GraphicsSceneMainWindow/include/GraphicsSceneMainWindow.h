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
#ifndef GRAPHICS_SCENE_MAINWINDOW_H
#define GRAPHICS_SCENE_MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QList>
#include <QMap>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QGraphicsRectItem>
#include <QRectF>
#include <QPrinter>
#include <QSplitter>

#define RSC_DIR QString(":/")

#define MAX(a,b) ((a)>(b)) ? (a) : (b)
#define MIN(a,b) ((a)<(b)) ? (a) : (b)

#define FROM_STORAGE_DRAG_MIME			"FromStorageMimeType"
#define FROM_SCENE_DRAG_MIME			"FromSceneMimeType"
#define FROM_HISTORY_DRAG_MIME			"FromHistoryMimeType"

#define PAGE_ITEM_OUT_MARGIN 20
#define PAGE_ITEM_IN_MARGIN 5

#define EXPORT_MIN_DETAIL 5
#define EXPORT_MAX_DETAIL 50
#define EXPORT_DEFAULT_DETAIL 30
#define EXPORT_DETAIL_TO_SCALE_FACTOR 0.1f

#define MIN_FONT_SIZE 2
#define MAX_FONT_SIZE 72

#define PALETTE_UNDEFINED -1

#define PNG_FILE_EXTENSION	QString("png")
#define PDF_FILE_EXTENSION	QString("pdf")
#define PS_FILE_EXTENSION	QString("ps")
#define MIDI_FILE_EXTENSION	QString("midi")

#define PNG_FILE_FILTER		QString("Image (*."					+ PNG_FILE_EXTENSION + ")")
#define PDF_FILE_FILTER		QString("PDF (*."					+ PDF_FILE_EXTENSION + ")")
#define PS_FILE_FILTER		QString("PostScript (*."			+ PS_FILE_EXTENSION	 + ")")
#define MIDI_FILE_FILTER	QString("Midi (*."					+ MIDI_FILE_EXTENSION + ")")

#define FONT_SIZE_SETTING			"FontSizeSetting"
#define WORKSPACE_MODE_SETTING		"WorkspaceModeSetting"

class QLanguageItem;

class QAction;
class QMenu;
class QGraphicsView;
class QGraphicsScene;
class LanguageGraphicsView;
class LanguageTextEdit;
class QBackgroundItem;
class QSideBarGraphicsView;
class QStorageGraphicsView;
class QPaintDevice;
class QLanguageItemFactory;
class QItemResizer;


#define DEFAULT_MAX_SCALE 10.0f
#define DEFAULT_MIN_SCALE 0.1f

#define DEFAULT_MIN_ZOOM	0.1f
#define DEFAULT_MAX_ZOOM	10.0f
#define DEFAULT_ZOOM_FACTOR	1.5f

enum UIElementId {
			EXIT_ACT = 0,
			ABOUT_QT_ACT,
			ZOOM_IN_ACT,
			ZOOM_OUT_ACT,
			ZOOM_STANDARD_ACT,
			FONT_BIGGER_ACT,
			FONT_SMALLER_ACT,
			ADD_ACT,
			ADD_FROM_FILE_ACT,
			REMOVE_ACT,
			COPY_ACT,
			PASTE_ACT,
			CLEAN_HISTORY_ACT,
			SEND_TO_BACK_ACT,
			SEND_BACKWARD_ACT,
			BRING_TO_FRONT_ACT,
			BRING_FORWARD_ACT,
			SELECT_ALL_ACT,
			NEW_ACT,
			SAVE_ACT,
			SAVE_AS_ACT,
			SAVE_ALL_ACT,
			OPEN_ACT,
			RESCALE_ACT,
			H_ALIGN_ACT,
			V_ALIGN_ACT,
			FILE_MENU,
			HELP_MENU,
			VIEW_MENU,
			EDIT_MENU,
			WINDOW_MENU,
			FILE_TOOL_BAR,
			VIEW_TOOL_BAR ,
			EDIT_TOOL_BAR ,
			SIZE_TOOL_BAR , 
			RESIZE_SLIDER ,
			RESIZE_EDIT ,
			X_ITEM_EDIT ,
			Y_ITEM_EDIT ,
			STATUS_BAR_MOUSE_POS ,
			STATUS_BAR_ZOOM
};
		
struct GraphicsSceneMainWindowSettings
{		
		GraphicsSceneMainWindowSettings()
		{		
			mMaxScale				= DEFAULT_MAX_SCALE;
			mMinScale				= DEFAULT_MIN_SCALE;

			mMinZoom				= DEFAULT_MIN_ZOOM;
			mMaxZoom				= DEFAULT_MAX_ZOOM;
			mZoomFactor				= DEFAULT_ZOOM_FACTOR;			
			
			mLanguageNameShort				= "";
			mLanguageNameLong				= "";
			mLanguageFileExtension			= "";			
			mSceneFileExtension				= "";
			mDefaultLanguageCode			= "";
			mLanguageCommandsFile			= "";
			
			mHasStorage						= true;
			mHasHistory						= true;
		}
		
		QString mLanguageName;
		
		int mResizerBorderWidth;

		float	mMinZoom;
		float	mMaxZoom;
		float	mZoomFactor;
		float	mMinScale;
		float	mMaxScale;
		QSizeF	mMinItemSize , mMaxItemSize;
		
		QString mLanguageNameShort;
		QString mLanguageNameLong;
		QString mLanguageFileExtension;			
		QString mSceneFileExtension;
		QString mDefaultLanguageCode;
		QString mLanguageCommandsFile;
		
		bool mHasStorage;
		bool mHasHistory;

		QList<int> mDesactiveUIElements;

};

class GraphicsSceneMainWindow : public QMainWindow
{
	friend class LanguageGraphicsView;
	friend class LanguageTextEdit;
	friend class QStorageGraphicsView;

    Q_OBJECT

public:
	GraphicsSceneMainWindow( LanguageGraphicsView * graphicsView , QLanguageItemFactory * factory );
	~GraphicsSceneMainWindow();

	virtual void reinitSettings();

	static void initApplicationSettings( const GraphicsSceneMainWindowSettings& settings );
	static const GraphicsSceneMainWindowSettings& applicationSettings();

protected Q_SLOTS:
    void addItem();
    void addItemFromFile();
	void removeSelectedItems();
	void copyItem();
	void pasteItem();
	void selectAll();
	void cleanHistory();
	void zoomIn();
	void zoomOut();
	void zoomStandard();
	virtual void fontBigger();
	virtual void fontSmaller();
	void sceneSelectionChanged();
	void sceneChanged( const QList<QRectF> & region );
	void textEditActivity();
	virtual void updateCode();
	void resizeSliderChanged(int value);
	void scaleItems(double scale);
	void scaleItems(double xScale , double yScale , double sizeScale );
	void scaleChanged(float newScale);
	void moveItemX(double x);
	void moveItemY(double y);
	void resizeItems();
	void hAlignItems();
	void vAlignItems();
	void newScene();
	void save();
	void saveSceneAs();
	void saveAll();
	void loadScene();
	void rescaleItem(float xScale , float yScale , float sizeScale );
	void resizePage( const QRectF& newRect );
	void storeItem(QLanguageItem * container);
	void itemRemoved();
	virtual void itemRemoved( QLanguageItem * );
	void insertText( const QString& gmnCode , int insertIndex );
	void switchCommandPaletteVisible();
	void switchTextEditVisible();
	virtual void updateWindowMenu();
	void switchStorageVisible();
	void switchHistoryVisible();
	void saveItemAs();
	void fileChanged(bool isFileRemoved);
	void descriptiveNameChanged();
	void itemHasBeenReloaded();
	void focusChanged( QWidget * old, QWidget * now );
	void bringForward();
	void sendBackward();
	void bringToFront();
	void sendToBack();

protected:
	
	void init();
	
	/*!
	*	\brief	Checks if the settings have been initialized for this application ; 
	*			and does the initialization if necessary. 
	*			Loads the settings (readSettings()).
	*/
	void checkSettings();
	
	/*!
	*	\brief	Loads the application settings.
	*/
	virtual void readSettings();
	
	/*!
	*	\brief	Loads the settings that can be modified in the preference menu.
	*
	*			This function should be called each time the Preference dialog is asked.
	*/
	void readPreferencesSettings();
	
	/*!
	*	\brief	Writes the application state into the settings. (usually called when the application closes)
	*/
	virtual void writeSettings();

	// Widgets initialization methods.
    virtual void	createActions();
    virtual void	createMenus();
    virtual void	createToolBars();
    void	createStatusBar();
	void	setupTextEdit();
	void	createHistoryAndStorage();
	//Destroys the current mGraphicsScene and build a new one.
	void	reinitScene(QRectF sceneRect); 
	// If the scene has changed, opens a QDialog asking whether the scene should be saved or not,
	// and saves (or not), according to the user's choice.
	// Returns false if the user chooses to cancel.
	bool	saveOrCancelDialog();
	// Checks if items modifications should be saved, asking the user when necessary.
	bool checkAllItems();
	
	// Calls save or saveAs, depending on whether the scene has already been saved or not.
	void saveScene();
	// Save the scene to the specified file. (creating a new file / erasing any file with the same name)
	void	saveSceneFile(const QString& fileName);
	/*
	*	\brief Load a Guido Scene file.
	*
	*	\return True if the load was successful.
	*/
	bool loadSceneFile(const QString& fileName);
	
	bool	dropItem( const QMimeData * mimeData , const QPointF& pos , QLanguageItem** createdItem );
	
	bool	addFile(const QString &fileName , QLanguageItem** createdItem = 0);
    bool	addCode(const QString &gmnCode, QLanguageItem** createdItem = 0);
	// Only method used to add a new QGuidoItem to the scene (on clicking on the "AddItem" button, on dragging a file inside the window, on pasting an item, ...).
	//	\param gmnSource may contain GMN Code or the full path to the GMN file to load
	//	\param isFileName Indicates whether gmnSource contains GMN code or a path to a GMN file
	//	\param isPageItem Indicates whether a QGuidoItem or a QGuidoPageGraphicsItem must be instanciated.
	//	\param pagesAlignment The pagesAlignment of the created QGuidoItem
	//	\param createdItem a reference to the pointer to the created QGuidoItem.
	//	\return true if the gmnSource was valid (correct GMN code/GMN file).
	virtual bool	addLanguageItem(const QString &gmnSource, bool sourceIsAFile , QLanguageItem** createdItem);
	bool	fileControl(QLanguageItem* languageItem);
	// Standard treatment applied to each QLanguageItem added to the main scene (new, dropped, from saved file, ... each one).
	virtual void	setupNAddItem(QLanguageItem* languageItem);
	// Extra-treatment applied to each QLanguageItem added to the main scene from a "Add item..." menu action.
	virtual void	setupItemFromMenu(QLanguageItem* languageItem);
	// Standard treatment applied to each brand new QLanguageItem added to the main scene by the user 
	// ( <=> menu-created or dropped) : setupNAddItem, controls the size, centers the position.
	// note: Copied items, scene-loaded items, don't use this function.
	virtual void	setupNAddNewItem(QLanguageItem* languageItem);
	// Removes and destroys the specified item. Returns false if the removal has been canceled.
	bool	removeItem( QLanguageItem * item );
	// Removes and destroys all the QGraphicsItem present in mGraphicsScene.
	void	removeAllItems();
	bool	saveItemModificationsDialog(QLanguageItem * item);
	void	saveItemAs(QLanguageItem * item);
	// Adds a copy of the item, at the same position.
	QLanguageItem*	addItemCopy(QLanguageItem* item);
	
	QString findFile(const QString& file);
	QPointF viewCenterInSceneCoordinate();

	//Scene items listing/filtering functions.
	QList<QLanguageItem*> selectedLanguageItems();
	QList<QLanguageItem*> languageItems();

	void	unselectAll();
	// Activate/Desactivate the actions/control widgets, depending on the number of selected items. 
	// Updates the content of the gmn-code text edit.
	// Updates the item scale/position control widgets.
	virtual void	updateWindowState();
	// Updates the item scale control widgets.
	void	updateScaleControls(double scale);
	// Updates the item position control widgets. 
	// If (connectedToItem==false), the connection "control widget value"-> "actual item position" will be desactivated during the update of the value.
	void	updateItemPositionControls(bool connectedToItem = true);
	// Updates the cursor position labels in the status bar.
	void	updateCursorPos(QPointF pos);
	// Updates the current view zoom label in the status bar.
	void	updateZoomLabel();
	void	updateRemoveActState();
	// Sets the zValue of each QLanguageItem with its position in mZOrderedItems list.
	void 	updateZOrders();
	//	Adds the specified item to history if necessary. Should be called each time an item has been modified, added, etc... so that
	//	each step is kept in the history.
	void	addToHistory(QLanguageItem* item);

	// Misc.
	void			formatFileName(QString& fileName , QString& selectedFilter , const QString& filters);
	void			controlPlayingContainer( QGraphicsItem * container );
	virtual void	changeFontSize( float newFontPointSize );
	virtual void	reloadTextEdits();	//Workaround : QTextEdit won't update when calling setFontPointSize.
	void			zoom( float zoom );
	QString			getFileDialogPath();
	void			setLastManagedFile(const QString& lastFile);
	
	// UI events handlers.
	void closeEvent(QCloseEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	QMenu * createPopupMenu();

	void dragEnterEvent(QDragEnterEvent* event);
	void dropEvent(QDropEvent* event);

	/*
	*	\brief Creates and plug a QItemResizer on the specified QLanguageItem.
	*
	*	\return A reference to the created QItemResizer.
	*/
	virtual QItemResizer* plugResizer( QLanguageItem * itemContainer );
	
    QGraphicsView *					mGraphicsView;
    QGraphicsScene *				mGraphicsScene;
	QDockWidget *					mLanguageTextEditDock;
	LanguageTextEdit *				mLanguageTextEdit;
	QTimer *						mLanguageTextEditTimer;
	QWidget *						mCommandPaletteDock;
	QLanguageItem *					mFirstSelectedItem;
	QList<QLanguageItem *>			mCopiedItems;
	QSlider *						mResizeSlider;
	QDoubleSpinBox *				mResizeEdit;
	QDoubleSpinBox *				mItemXEdit;
	QDoubleSpinBox *				mItemYEdit;

	QBackgroundItem *				mBackgroundItem;
	QSplitter *						mSplitter;
	
	QGraphicsScene *				mStorageGraphicsScene;
	QSideBarGraphicsView *			mStorageGraphicsView;
	QGraphicsScene *				mHistoryGraphicsScene;
	QSideBarGraphicsView *			mHistoryGraphicsView;

	QLabel * mViewXLabel;
	QLabel * mViewYLabel;
	QLabel * mViewScaleLabel;
		
    QMenu *mFileMenu;
    QMenu *mHelpMenu;
    QMenu *mViewMenu;
    QMenu *mEditMenu;
	QMenu *mWindowMenu;
    QToolBar *mFileToolBar;
    QToolBar *mViewToolBar;
    QToolBar *mEditToolBar;
	QToolBar *mSizeToolBar;
    QAction *mExitAct;
    QAction *mAboutQtAct;
    QAction *mZoomInAct;
	QAction *mZoomOutAct;
	QAction *mZoomStandardAct;
	QAction *mFontBiggerAct;
	QAction *mFontSmallerAct;
	QAction *mAddAct;
	QAction *mAddFromFileAct;
	QAction *mRemoveAct;
	QAction *mCopyAct;
	QAction *mPasteAct;
	QAction *mCleanHistoryAct;
	QAction *mSelectAllAct;
	QAction *mNewAct;
	QAction *mSaveAct;
	QAction *mSaveAsAct;
	QAction *mSaveAllAct;
	QAction *mOpenAct;
	QAction *mRescaleAct;
	QAction *mHAlignAct;
	QAction *mVAlignAct;
	QAction *mSwitchCommandPaletteVisible;
	QAction *mSwitchTextEditVisible;
	QAction *mSendBackwardAct;
	QAction *mSendToBackAct;
	QAction *mBringToFrontAct;
	QAction *mBringForwardAct;

	QByteArray mStdMainWindowState;

	QAction *mSwitchHistoryVisible;
	QAction *mSwitchStorageVisible;
	
	int mCurrentZOrderItem;
	bool mSceneHasChanged;
	QString mLastOpenFilePath;
	bool mIsWorkspaceModeOn;
	
	QList<QLanguageItem*> mZOrderedItems;
	
	QLanguageItemFactory * mFactory;
	static GraphicsSceneMainWindowSettings mSettings;
	
};

QGraphicsPixmapItem * buildPixmapItem( const QString& fileName );
QGraphicsItem * buildTextItem(const QString& text);

#endif //GRAPHICS_SCENE_MAINWINDOW_H
