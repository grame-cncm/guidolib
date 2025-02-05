/*
 * QGuidoItemContainer.h
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
#ifndef GUIDO_ITEM_CONTAINER_H
#define GUIDO_ITEM_CONTAINER_H

#include "QLanguageItem.h"

#include "QGuidoGraphicsItem.h"
#include "QSwitcher.h"

#include <QObject>
#include <QGraphicsRectItem>
#include <QPrinter>
#include <QDomElement>
#include <QDrag>
#include <QMimeData>
#include <QAction>
#include <QMenu>
#include <QDataStream>
#include <QKeyEvent>

#define GMN_CONTAINER_MIME_PROPORTIONAL_ON		"GMNContainerMimeTypeProportionalOn"
#define GMN_CONTAINER_MIME_OPTIMAL_PAGE_FILL_ON	"GMNContainerMimeTypeOptimalPageFillOn"
#define GMN_CONTAINER_MIME_RESIZE_PAGE_ON		"GMNContainerMimeTypeResizePageOn"
#define GMN_CONTAINER_MIME_DISPLAY_TYPE			"GMNContainerMimeTypeDisplayType"
#define GMN_CONTAINER_MIME_SCALE				"GMNContainerMimeTypeScale"
	
#define DOM_GUIDO_ITEM_PAGE_MODE			"mode"
#define DOM_GUIDO_ITEM_ORIENTATION			"orientation"
#define DOM_GUIDO_ITEM_OPTIMAL_PAGE_FILL	"optimalPageFill"
#define DOM_GUIDO_ITEM_PROPORTIONAL_LAYOUT	"proportionalLayout"
#define DOM_GUIDO_ITEM_RESIZE_PAGE_ON		"resizePage"
#define DOM_GUIDO_ITEM_DISPLAY_TYPE			"displayType"
#define DOM_GUIDO_ITEM_SCALE				"scale"

//#define BASE_RGB					125,200,245
#define BASE_RGB					197,214,255
#define INVALID_RGB					250,20,20
#define BASE_COLOR					QColor(BASE_RGB)
#define ALPHA_0						150
#define ALPHA_1						80
#define ALPHA_2						50
#define ALPHA_3						20
#define SELECTED_PEN				QPen( QColor( BASE_RGB ) )
#define SELECTED_BRUSH				QBrush( QColor( BASE_RGB , ALPHA_1 ) )
#define HIGHLIGHTED_PEN				QPen( QColor( BASE_RGB ) )
#define HIGHLIGHTED_BRUSH			QBrush( QColor( BASE_RGB , ALPHA_2 ) )
//#define STANDARD_PEN				QPen( QColor( BASE_RGB , ALPHA_0 ) )
#define STANDARD_PEN				Qt::NoPen
#define STANDARD_BRUSH				Qt::NoBrush
#define INVALID_PEN					Qt::NoPen
#define INVALID_STANDARD_BRUSH		QBrush( QColor( INVALID_RGB , ALPHA_3 ) )
#define INVALID_HIGHLIGHTED_BRUSH	QBrush( QColor( INVALID_RGB , ALPHA_2 ) )
#define INVALID_BRUSH_SELECTED		QBrush( QColor( INVALID_RGB , ALPHA_1 ) )
#define ROUNDED_RECT_RADIUS			5

#define CONTEXT_MENU_LAYOUT	"Layout"

#ifdef USES_GUIDO_AR
	class QPaletteItem;
	class QPaletteItemDropper;

#define PALETTE_GUIDO_ELEMENT	1

#endif

class QPageTurnerItem;
class QTextPathItem;
class QItemResizer;

/**
*	\brief A specialization of QSelectionItem that only works with QGuidoItem.
*	It ads functionnalities to its child QGuidoItem :
*
*		- adds a contextual menu to navigate through the pages of the score when
*		the decorated item is a QGuidoPageGraphicsItem
*		- adds a contextual menu to switch the pagesAlignment of the decorated item
*		- adds a contextual menu to emit a switchMode signal, requesting for a change
*		of the decorated class. (QGuidoItem <-> QGuidoPageGraphicsItem)
*
*	You can dynamically add a QResizerItem or a QPaletteItem, in which case the QGuidoItemContainer
*	will emit the rescaleItem & combineItem signals when necessary.
*
*	\note In order that the QGuidoItemContainer catches UI events (mouse & keyboard) for the
*	the decorated QGuidoItem, the decorated QGuidoItem flags
*	QGraphicsItem::ItemIsSelectable and QGraphicsItem::ItemIsFocusable must remain disabled.
*/
class QGuidoItemContainer : public QLanguageItem
{
	Q_OBJECT

	public:
		
		/**
		*	\brief Constructor.
		*/
		QGuidoItemContainer(QGraphicsItem * parent);
		QGuidoItemContainer(const QMimeData * mimeData			, QGraphicsItem * parent);
		QGuidoItemContainer(const QDomElement * domElement		, QGraphicsItem * parent);
		QGuidoItemContainer(const QGuidoItemContainer * other	, QGraphicsItem * parent);

		virtual ~QGuidoItemContainer();

		/**
		*	\brief Exports the Guido Score to pdf-file 'fileName'.
		*/
		void exportToPdf(const QString& fileName);
		
		/*!
		*	\brief Create a new QDomElement storing the properties of the QGuidoItemContainer.
		*
		*	\note The QDomElement may be further used to setup another QGuidoItemContainer, using QGuidoItemContainer::loadFromDomElement.
		*/
		virtual QDomElement saveToDomElement( QDomDocument * doc);
		static bool recognizes(const QMimeData * data);
		static bool recognizes( const QDomElement * e );

		virtual bool	setCode( const QString& code );
		virtual QString	code() const						{ return guidoItem()->gmnCode(); }

		virtual bool isValid() const						{ return guidoItem()->isGMNValid(); }
		virtual QString lastErrorMessage() const			{ return guidoItem()->getLastErrorMessage(); }

		float	currentScale() const						{ return guidoItem()->transform().m11(); }

		/*!
		*	\brief QGuidoItemContainer comparator.
		*/
		virtual bool isEqualTo( QLanguageItem * item ) const;

		/**
		*	\brief Getter to the contained QGuidoItem.
		*/
		//Note : hack. Will be removed when the Qt bug with linux & QGraphicsItem::setCacheMode will be solved.
		virtual QGuidoGraphicsItem * guidoItem() const;

		/*!
		*	\brief	Sets the min/max scale used to set bounds to the QItemResizer.
		*/
		void setMinMaxScale(float min , float max);

		void keyPressEvent ( QKeyEvent * event );

		void setResizer( QItemResizer* resizer );

	public Q_SLOTS :
	
		void resized(const QRectF& newRect);

		void switchOptimalPageFill();
		void switchProportional();
		void switchResizePage();

		void firstPage();
		void previousPage();
		void nextPage();
		void lastPage();

	protected Q_SLOTS :
		
		virtual void guidoGeometryChanged();
		void descriptiveNameHasChanged();

	Q_SIGNALS:
	
		/*!
		*	\brief Emitted when the item asks to be exported.
		*/
		void exportItem();

 	protected:
		
		virtual void init();
		void init(const QMimeData * mimeData);
		void init(const QDomElement * domElement);
		void init(const QGuidoItemContainer * other);

		/*!
		*	\brief Sets the properties of the QGuidoItemContainer according to the content of the QDomElement.
		*
		*	\note QGuidoItemContainer::saveToDomElement is the symmetrical function.
		*	This method never fails, thanks to a default-value policy.
		*/
		virtual void loadFromDomElement( const QDomElement * );
		
		/*!
		*	\brief Sets the properties of the QGuidoItemContainer according to the content of the QMimeData.
		*
		*	\note QGuidoItemContainer::buildMimeData is the symmetrical function.
		*	This method never fails, thanks to a default-value policy.
		*/
		virtual void loadFromMimeData( const QMimeData * mimeData );
		
		/*!
		*	\brief Sets the properties of the QGuidoItemContainer according to the content of the otherContainer.
		*
		*	It doesn't matter if the other QGuidoItemContainer is a different derived class : in this case,
		*	only the common attributes are imported.
		*/
		virtual void loadFromOtherContainer( const QGuidoItemContainer * otherContainer );
	
		/*!
		*	\brief Returns the action list for contextual menu. Subclasses may extend this function.
		*/
		virtual QMenu* buildContextMenu();

		/*!
		*	\brief QLanguageItem implementation.
		*/
		QImage* buildDragImage();

		/*!
		*	\brief Print the contained QGuidoItem on the QPrinter. Pure virtual.
		*/
		void exportToPdf( QPrinter * printer );
		
		/*!
		*	\brief Updates the layoutSettings of the contained QGuidoItem according to mIsProportionalOn and mIsOptimalPageFillOn.
		*/
		void updateLayoutSettings();
		
		void setOptimalPageFill(bool isOptimalPageFillOn);
		void setProportional(bool isProportionalOn);
		void setResizePageToMusic(bool isResizePageOn);
		
		/*!
		*	\brief Builds the QMimeData containing the properties of the QGuidoItemContainer. 
		*
		*	Subclasses may extend this function.
		*
		*	\note QGuidoItemContainer::loadFromMimeData is the symmetrical function.
		*/
		virtual QMimeData *	buildMimeData();
		
		void updateTitleBarVisibility();
		void updateTitleBar();
		void updateTitleText();
		void updateSyntaxErrorItem();
		void updateSelectionItem();
		void updatePageItemsVisibility();
		void updatePageIndexLabel();
		void updateResizerColor();
		void setCurrentPage(int page);
		void setFile(const QString& fileName);
		QVariant itemChange( GraphicsItemChange change, const QVariant& value );
		void hoverEnterEvent ( QGraphicsSceneHoverEvent * e );
		void hoverLeaveEvent ( QGraphicsSceneHoverEvent * e );
		void guidoUpdateGeometry(const QRectF& r);
		
		void resize( float xScale , float yScale );

		QGuidoGraphicsItem * mGuidoItem;

		bool mIsProportionalOn;
		bool mIsOptimalPageFillOn;

		float mMinScale, mMaxScale;

		QPageTurnerItem * mNextPageTurner;
		QPageTurnerItem * mPreviousPageTurner;
		QTextPathItem * mPageLabelItem;

		QTextPathItem * mHeadBar;
//		QTextPathItem * mSyntaxErrorItem;
		struct PenBrush {
			QPen mPen;
			QBrush mBrush;
			PenBrush(const QPen& p = QPen() , const QBrush& b = QBrush())
			{	
				mPen = p; mBrush = b;
			}
		};
		QSwitcher<PenBrush> mPenBrushSwitcher;
		QGraphicsPathItem * mSelectionItem;

		QItemResizer* mResizer;
		enum ResizeMode {
			RESIZE_NORMAL = 0,
			RESIZE_GRID = 1,
			RESIZE_FORMAT = 2
		};
		int mResizeMode;

#ifdef USES_GUIDO_AR

	public :
	
		virtual void play();
		virtual void pause();
		virtual void stop();

		int mMidiRef;
		typedef enum PlayerState {
			PLAYING , PAUSED , STOPPED
		} PlayerState;
		PlayerState mPlayerState;
#endif

};

#endif //GUIDO_ITEM_DECOARTOR_H
