/*
 * MainWindowObjects.h
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
#ifndef MAINWINDOW_OBJECTS_H
#define MAINWINDOW_OBJECTS_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QTextEdit>
#include <QDialog>
//#include "ui_ResolutionDialog.h"
#include <QDomElement>
#include <QList>

class QItemResizer;
class GraphicsSceneMainWindow;
class QLanguageItem;
class QLanguageItemFactory;

//------------------------------------------------------------------------------------------------------------------------
class QSideBarGraphicsView : public QGraphicsView 
{
	public :
	
		QSideBarGraphicsView(QGraphicsScene * s , QLanguageItemFactory* factory );
		virtual ~QSideBarGraphicsView() {}
		
		virtual void addItem( QLanguageItem * container ) = 0;
		QLanguageItem * find(const QLanguageItem * other );

		virtual QString domId() = 0;
		virtual QString mimeId() = 0;

		virtual QDomElement save(QDomDocument &d);
		virtual bool load( const QDomElement& root );
		virtual void clear();
		void removeItem( QGraphicsItem * item );

	protected :

		void setupContainer( QLanguageItem * container );

		QList<QLanguageItem*> mAddedItems;
		QLanguageItemFactory* mFactory;
};

//------------------------------------------------------------------------------------------------------------------------
class QHistoryGraphicsView : public QSideBarGraphicsView 
{
	public :
		QHistoryGraphicsView(QGraphicsScene * s , QLanguageItemFactory* factory );
		virtual ~QHistoryGraphicsView() {}

		void addItem( QLanguageItem * container );

		virtual void clear();

		QString domId();
		QString mimeId();

	protected :
		void reorderItems();

		void resizeEvent( QResizeEvent * e );
};


//------------------------------------------------------------------------------------------------------------------------
class QStorageGraphicsView : public QSideBarGraphicsView 
{
	Q_OBJECT

	public :
		QStorageGraphicsView( QGraphicsScene * s , GraphicsSceneMainWindow * mainWindow , QLanguageItemFactory * factory  );
		virtual ~QStorageGraphicsView() {}
		
		void addItem( QLanguageItem * container );
		QString domId();
		QString mimeId();
			
	protected Q_SLOTS:
	
		void sceneChanged(const QList<QRectF> &);
						
	protected :
//		void mousePressEvent( QMouseEvent * event );
		void resizeEvent( QResizeEvent * );
		void dragEnterEvent(QDragEnterEvent* event);
		void dragMoveEvent(QDragMoveEvent* event);
		void dragLeaveEvent(QDragLeaveEvent* event);
		void dropEvent(QDropEvent* event);
		void focusOutEvent( QFocusEvent * event );
		void focusInEvent( QFocusEvent * event );
		void reorderItems();
		QList<QLanguageItem*> filterItems(const QList<QGraphicsItem*>& list);

		GraphicsSceneMainWindow * mMainWindow;
		int mPreferredWidth;
};

//------------------------------------------------------------------------------------------------------------------------
class QBackgroundItem : public QGraphicsItem
{
	public:
		
		QBackgroundItem( GraphicsSceneMainWindow * mainWindow ,  const QSizeF& size );
		virtual ~QBackgroundItem() {}

		void setRect( const QRectF& size );
		QRectF boundingRect() const;
		QRectF pageRectInSceneCoords() const;
		void setPageSize( const QSizeF& size );
		QSizeF pageSize() const;
		
		void setResizeEnabled( bool isEnabled );
		bool isResizeEnabled() const;

	protected:
		void paint ( QPainter * painter, const QStyleOptionGraphicsItem * , QWidget * );
		void updatePageRect();
		
		QRectF mRect;
		QGraphicsRectItem * mPage;
		QItemResizer * mBackgroundItemResizer;
		bool mIsResizeEnabled;
};

//------------------------------------------------------------------------------------------------------------------------
class LanguageTextEdit : public QTextEdit
{
	Q_OBJECT

	public :
		LanguageTextEdit(const QString& initString , GraphicsSceneMainWindow * mainWindow);
		virtual ~LanguageTextEdit() {}

	Q_SIGNALS:
		void enabled( bool isEnabled );
		
	protected :
		void changeEvent ( QEvent * event );
		void wheelEvent( QWheelEvent * e );
		void focusOutEvent( QFocusEvent * event );
		void dragEnterEvent(QDragEnterEvent* event);
		void dragMoveEvent(QDragEnterEvent* event);
		void dropEvent(QDropEvent* event);
		
		GraphicsSceneMainWindow * mMainWindow;
};

//------------------------------------------------------------------------------------------------------------------------
class LanguageGraphicsView : public QGraphicsView 
{
	public:
		LanguageGraphicsView();
		virtual ~LanguageGraphicsView() {}
		void setMainWindow( GraphicsSceneMainWindow * mainWindow );

		QSize sizeHint () const;

	protected:
		void dragEnterEvent(QDragEnterEvent* event);
		void dragLeaveEvent(QDragLeaveEvent* event);
		void dragMoveEvent(QDragMoveEvent* event);
		void dropEvent(QDropEvent* event);
		void mouseMoveEvent(QMouseEvent * event);

		GraphicsSceneMainWindow * mMainWindow;
};

#endif
