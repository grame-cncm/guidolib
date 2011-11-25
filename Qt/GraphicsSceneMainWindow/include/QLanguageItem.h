/*
 * QLanguageItem.h
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
#ifndef LANGUAGE_ITEM_H
#define LANGUAGE_ITEM_H

class QLanguageItemAction;
class QPaletteItemDropper;

#include <QObject>

#include <QDrag>
#include <QDomElement>
#include <QAction>
#include <QMultiHash>
#include <QFileSystemWatcher>
#include <QMenu>
#include <QGraphicsSimpleTextItem>

#define DOM_LANGUAGE_ITEM	"Item"

#define MIME_DO_COPY		"MimeDoCopy"
#define MIME_PALETTE_ID		"MimePaletteId"
#define MIME_PALETTE_ITEM	"MimePaletteItem"

#define FONT_FAMILY "Arial"
#define FONT_SIZE	15

#define OBJECT_NAME_DELETE_ITEM_AND_FILE_ACT	"DeleteItemAndFileAct"
#define OBJECT_NAME_UNLINK_ACT					"UnlinkAct"
#define OBJECT_NAME_SAVE_AS_ACT					"SaveAsAct"
#define OBJECT_NAME_SAVE_ACT					"SaveAct"
#define OBJECT_NAME_RELOAD_ACT					"ReloadAct"
#define OBJECT_NAME_FILE_MENU					"FileMenu"
#define OBJECT_NAME_Z_MENU						"ZMenu"

/*!
*	\brief QTextPathItem is QGraphicsPathItem with a centered text at its center.
*/
class QTextPathItem : public QGraphicsPathItem
{
	public :
	
		QTextPathItem( const QString& text , bool centerText , const QColor& textColor , const QFont& textFont , float textPenWidth , QGraphicsItem* parent = 0 );		
		
		QGraphicsSimpleTextItem * textItem() { return mText; }
		void setText(const QString& text);
		void setPath( const QPainterPath& p );
	protected :
	
		void updateTextPos();
		QGraphicsSimpleTextItem *	mText;
		bool mCenterText;
};

/**
*	\brief	A QLanguageItem is a QGraphicsItem with a textual attribute, its code.
*			Typically, the code will be expressed in a certain languauge, 
*			and will be used to generate the item's appearance & shape.
*
*	A QLanguageItem may be "file-linked" or not. An item is "file-linked" when
*	its code has been loaded from a file (loadFile( const QString& fileName )), 
*	or saved to a file (save(const QString& fileName)). A "file-linked" item can
*	be unlinked with unlinkFile().
*/
//class QLanguageItem : public QObject , public QSelectionItem
class QLanguageItem : public QObject , public QGraphicsRectItem
{
	Q_OBJECT

	public:
		
		/**
		*	\brief Constructor.
		*/
		QLanguageItem(QGraphicsItem * parent = 0);

		virtual ~QLanguageItem();

		/**
		*	\brief The painting of the QLanguageItem is undefined.
		*/
		void paint( QPainter * , const QStyleOptionGraphicsItem *, QWidget * ) {}

		/**
		*	\brief Sets the item's code.
		*/
		virtual bool	setCode( const QString& code ) = 0;
		
		/**
		*	\brief Returns the item's code.
		*/
		virtual QString	code() const = 0;
		
		/**
		*	\brief Returns true if the item's code is valid.
		*/
		virtual bool isValid() const				{ return true; }
		
		/**
		*	\brief Returns an error message relative to the item's code.
		*/
		virtual QString lastErrorMessage() const	{ return ""; }
		
		virtual float currentScale() const = 0;
		
		/**
		*	\brief Loads the specified text file and sets the item's code with the file's contents.
		*/
		void loadFile( const QString& fileName );
		
		/**
		*	\brief Returns the item's file. If no file has been loaded (with loadFile), returns "".
		*/
		QString file() const;

		/**
		*	\brief Saves the code() to 'fileName'.
		*/		
		virtual void save(const QString& fileName);
		
		/*!
		*	\brief Create a new QDomElement storing the properties of the QLanguageItem.
		*
		*	\note The QDomElement may be further used to setup another QLanguageItem, using QLanguageItem::loadFromDomElement.
		*/
		virtual QDomElement saveToDomElement( QDomDocument * doc);
				
		/*!
		*	\brief QLanguageItem comparator. The default implementation compares the items' codes.
		*/
		virtual bool isEqualTo( QLanguageItem * item ) const;

		/**
		*	\brief	The standard virtual QGraphicsItem::keyPressEvent made public, because GraphicsSceneMainWindow
		*			intercepts this event.
		*/
		virtual void keyPressEvent ( QKeyEvent * e );

		/**
		*	\brief	Changes the item's position to destPoint. Only used when dropping the item inside the scene to move it :
		*			uses the mouse position when the drag started.
		*			
		*	\return	Returns the resulting movement.
		*/
		QPointF moveItem( const QPointF& destPoint );
		
		/*!
		*	\brief Sets the mimeId of the QLanguageItem. The mimeId is included in the QMimeData
		*	when drag'N'dropping the QLanguageItem, and then is used by drop targets to accept/refuse the drop.
		*/
		void setMimeId(const QString& mimeId);

		/*!
		*	\brief Sets if the QLanguageItem is in the main QGraphicsItem or if it's in a storage/history zone.
		*/
		virtual void setStorageMode( bool isStorageModeOn );
		
		/*!
		*	\brief Adds a QLanguageItemAction ; the QLanguageItemAction's action will be used in
		*	the QLanguageItem's contextual menu.
		*/
		void addContainerAction( QLanguageItemAction * containerAction );
		
		/**
		*	\brief	Returns true if the item is modified.
		*
		*	An item becomes modified when calling codeChanged().
		*	It is not modified when calling saveItem, loadItem, reloadItem.
		*/
		bool isModified() const;
		
		/**
		*	\brief Returns true if file() is defined, whereas the file doesn't exists.
		*/
		bool fileNotFound() const;

		/**
		*	\brief Returns the descriptive file name. 
		*	
		*	The descriptive file name is name() + "*" if the item isModified.
		*/
		QString getDescriptiveFileName() const;

		/**
		*	\brief Returns the item's name.
		*
		*	The name is :
		*		- for a file-linked item, the item's file (not its full path)
		*		- for a not file-linked item, "untitled"
		*/
		QString name() const;
		
		void plug( QPaletteItemDropper* dropper );

	Q_SIGNALS:

		/*!
		*	\brief Used internally. Emit when the contained QGuidoItem's format changes.
		*/
		void updateChildrenItemsGeometry(const QRectF& newGeometry);
		
		/*!
		*	\brief Emitted when the item asks to be stored.
		*/
		void storeItem();

		/**
		*	\brief Emitted when the item asks to be be saved in a new file.
		*/
		void saveItemAs();
		
		/*!
		*	\brief Emitted when the item removes itself from the scene it was in.
		*/
		void removed();

		/*!
		*	\brief Emitted when the item has changed its code from inside.
		*/
		void itemHasBeenReloaded();

		//QItemResizer connection
		void showResizer(bool isShown);

		/**
		*	\brief Emitted when the item's file has been changed from outside :
		*		- if isFileRemoved, the file has been removed or renamed
		*		- if not, its content has changed
		*/
		void fileChanged(bool isFileRemoved);

		/**
		*	\brief Emitted when descriptive file name has changed. (see getDescriptiveFileName)
		*/
		void descriptiveNameChanged();

		void dragStarted(int & inId , QGraphicsItem*& inDraggedItem);
		
		/*!
		*	\brief Emitted when the item scale has been changed from inside.
		*/
		void scaleChanged(float newScale);
		
		/*!
		*	\brief Emitted when the item asks to be brought to front/forward/backward/back.
		*/
		void bringToFront();
		void bringForward();
		void sendBackward();
		void sendToBack();

	public Q_SLOTS:

		/**
		*	\brief Resize the item with the specified QRectF.
		*/
		virtual void resized( const QRectF& newRect ) = 0;

		/**
		*	\brief For a file-linked item, saves the item's code to file(). Else, does nothing.
		*/
		void saveItem();
		
		/**
		*	\brief For a file-linked item, sets the item's code with the contents of file(). Else, does nothing.
		*
		*	If the item isModified(), ask for a confirmation before reloading.
		*/
		void reloadItem();
		
		/**
		*	\brief	If the item was "file-linked", it won't be anymore. The previously linked file
		*			is not affected in any way by this operation.
		*/
		void unlinkFile();
		
	protected Q_SLOTS:

		/**
		*	\brief Must be called after that the item's code() has changed (typically, in a QLanguageItem::setCode implementation).
		*/
		void codeChanged();

		/**
		*	\brief	UI asks if the user wants to remove the item and the file() (for a file-linked item),
		*			before actually doing it.
		*
		*			If the item is not file-linked, does nothing.
		*/
		void removeItemAndFile();

 	protected:

		/**
		*	\brief Loads the contents of the QDomElement. The QDomElement must have initially been created by QLanguageItem::saveToDomElement.
		*/
		void load(const QDomElement * domElement);
		
		/**
		*	\brief Setup the item with the data of the other item.
		*/
		void load(const QLanguageItem * other);
		
		/**
		*	\brief Loads the contents of the QMimeData. The QMimeData must have initially been created by QLanguageItem::buildMimeData.
		*/
		void load(const QMimeData * mimeData);
		
		/**
		*	\brief Changes the file name.
		*/
		virtual void setFile( const QString& fileName );
		
		void updateGeometry( const QRectF& newGeometry );
		
		/*!
		*	\brief Returns the QMenu to be used in contextMenuEvent. Caller takes ownership of the menu.
		*/
		virtual QMenu * buildContextMenu();

		/**
		*	\brief QGraphicsItem virtual function implementation.
		*/
		virtual QVariant itemChange	 ( GraphicsItemChange change, const QVariant& value );

		/*!
		*	\brief Builds an image that will be used when draging the item. (see QDrag::pixmap() )
		*/
		virtual QImage*	buildDragImage() = 0;
	
		/*!
		*	\brief Builds the QMimeData containing the properties of the QLanguageItem. 
		*
		*	Subclasses may extend this function to add specified properties.
		*
		*	\note QLanguageItem::load(QMimeData*) is the symmetrical function.
		*/
		virtual QMimeData *	buildMimeData();

		/**
		*	\brief Sets isModified() to the specified value. (see QLanguageItem::isModified() )
		*/
		virtual void setModified( bool isModified );

		/**
		*	\brief QGraphicsItem virtual function implementation.
		*/
		void contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
				
		/**
		*	\brief QGraphicsItem virtual function implementation.
		*/
		void mousePressEvent(QGraphicsSceneMouseEvent *event);
		void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
		void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

		/*!
		*	\brief Updates the cached image used when drag'n'dropping the QLanguageItem. Used only when mIsStorageModeOn == true.
		*/
		void	updateDragImage();

		void fileChangeEvent( bool isFileRemoved );

		QString mMimeId;
		bool mIsStorageModeOn;
		QImage * mDragImage;
		bool mHasToUpdateDragImage;
		QList<QLanguageItemAction*> mContainerActionList;
		QSizeF mMinSize, mMaxSize;

		QString mFile;
		bool mIsModified;

		bool mIgnoreChangeNotification;
		bool mIsProcessingFileChange;

		QPointF mMousePressPoint;
		bool mCopyDrag;

	private:
			
		/**
		*	\brief Build the QDrag of this item, with source QWidget.
		*/
		QDrag *	buildDrag(QWidget * widget);
		
// -------------------- Static members --------------------
		
	public :
		
		static void checkAllFiles();
		static void fileChanged( const QString& fileName , const QString& newContent );
		static void fileRemoved( const QString& fileName );

	protected :

		/**
		*	\brief Registers the fileName as "used by" the usingItem. 
		*/
		static void addFile(const QString& fileName , QLanguageItem* usingItem);
		static void removeFile(const QString& fileName , QLanguageItem* usingItem);

		static QMultiHash<QString,QLanguageItem*> mUsedFiles;
		static QHash<QString,QString> mFileContents;
};

/**
*	\brief QLanguageItemAction allows to add an item in a QLanguageItem's contextual menu ;
*	QLanguageItemAction::triggered signal will be emitted when the action is triggered.
*
*	-	When you build a QLanguageItemAction on a QLanguageItem,
*		it calls QGuidoItemContainaer::addContainerAction(this).
*	-	QLanguageItemAction::buildAction builds an action ( owned by the QLanguageItem ) ; when this action
*		will be triggered, the QuidoItemContainerAction will emit the signal triggered(QLanguageItem*).
*
*	\note The QLanguageItem takes ownership of the QLanguageItemAction.
*/
class QLanguageItemAction : public QObject
{
	Q_OBJECT

	public:
	
		QLanguageItemAction( QLanguageItem * container , const QString& actionLabel , const QVariant& actionData = QString() );
		
		QAction * buildAction() const;
		
	Q_SIGNALS:
		void triggered(QLanguageItem * container);
		
	private Q_SLOTS:
		void triggered();
		
	protected : 
		QLanguageItem * mContainer;
		QString mActionLabel;
		QVariant mActionData;
};

/*!
*	\brief 
*	(From QGraphicsItem.cpp in Qt sources)
!*/
QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);

void paintOnDevice( QPaintDevice * device , QGraphicsItem * item , float scaleFactor , const QRect& painterWindow = QRect() );
QImage itemToImage( QGraphicsItem * item , float scaleFactor = 1.0f , unsigned int backgroundColor = 0 , bool setPainterWindow = false );
QAction * newSeparator(QObject * parent);

#endif //LANGUAGE_ITEM_H
