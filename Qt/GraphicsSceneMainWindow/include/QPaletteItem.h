/*
 * QPaletteItem.h
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
#ifndef PALETTE_ITEM_H
#define PALETTE_ITEM_H

class QPaletteItemDropper;
class QPaletteManager;

//#include "QPluggableItem.h"

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QGraphicsScene>

/*!	\brief	The QPaletteItem is part of the QPaletteManager system (see QPaletteManager).
*
*			To use a QPaletteItem in the QPaletteItem system, add it to the same scene as the QGraphicsItems
*			that will use the QPaletteItem system.
*
*			How to use a QPaletteItem:
*			- You must specify in the QPaletteItem's constructor the id of the QPaletteItemDroppers that
*			it can handle, both as droppedItem and as targetItem
*			- The QPaletteItem is a circle ; you can specify its radius.
*			- The circle is split in different equal pies ; each pie is for one interaction. 
*			- You can add a new interaction with addInteraction(QGraphicsItem*,int). The QGraphicsItem* will be
*			the icon of the interaction (note : the QPaletteItem takes ownership of the QGraphicsItem).
*			The int will be the id of the interaction, used by the signal interactionActivated.
*			The interaction's icon is displayed at the center of the interaction's pie. The pies
*			are ordered in positive trigonometric order. By default, the first pie is at angle zero 
*			(right of the palette), but you can set this with setOriginAngle(int).
*
*			According to the place where you drop the item, it activate one type of interaction. Then the QPaletteManager::mInstance
*			will emit the signal interactionActivated with the id of the activated interaction, and the dropped and 
*			destination items.
*
*			To use several equivalent QPaletteItem (they manage the same QPaletteItemDropper's ids) but switching between
*			them, you can enable/disable the QPaletteItem  with QPaletteItem::setPaletteEnabled.
*
*			TODO: WRITE DOC ABOUT MAKING QPaletteItem SUBCLASSES.
*/
class QPaletteItem : public QObject , public QGraphicsEllipseItem
{
	Q_OBJECT
	
	friend class QPaletteManager;
	
	public:

		/*!
		*	\brief Constructor.
		*
		*	\param handledDroppedItemIds List of ids that the QPaletteItem can manage as "droppedItems". Note: Ids >= 0.
		*	\param handledTargetItemIds  List of ids that the QPaletteItem can manage as "targetItems".  Note: Ids >= 0.
		*	\param radius Radius of the QPaletteItem circle.
		*	\param parent The parentItem of the QPaletteItem.
		*/
		QPaletteItem( const QList<int>& handledDroppedItemIds , const QList<int>& handledTargetItemIds , float radius , QGraphicsItem * parent = 0 );
		
		/*!
		*	\brief	Convenience constructor. Equivalent to calling QPaletteItem( QList<int>() << paletteId , QList<int>() << paletteId , radius , parent ).
		*			Note: Ids >= 0.
		*/
		QPaletteItem( int paletteId , float radius , QGraphicsItem * parent = 0 );
		
		virtual ~QPaletteItem();
		
		/*!	\brief	Add a new interaction to the palette, with the specified QGraphicsItem as an icon, and 
		*			the specified Id.
		*/
		void addInteraction( QGraphicsItem * itemIcon , int interactionId );
		
		/*!	\brief Sets the origin angle of the pies.
		*/
		void setOriginAngle( int degreeOriginAngle );
		
		/*!
		*	\brief Sets the pen/brush used to draw an highlighted interaction's pie.
		*/
		void setHighlightStyle( const QPen& pen , const QBrush& brush );

		void paint(QPainter* painter , const QStyleOptionGraphicsItem* style , QWidget* widget);
		
		/*! \brief	Changes the QPaletteItem radius with the specified newRadius.
		*
		*	\param	newRadius New radius of the palette. If zero, the radius
		*			will be maximized to fit in the item's parent's boundingRect.
		*/
		void setRadius( float newRadius );
		
		/*!
		*	\brief See showOnItem.
		*
		*	CENTER_AND_RESIZE_POLICY : moves the palette at the center of the target item,
		*							maximizing its radius to fit inside it.
		*	CENTER_POLICY :			moves the palette at the center of the target item,
		*							without changing its radius. (default)
		*/
		typedef enum {
			CENTER_AND_RESIZE_POLICY , CENTER_POLICY
		} PositioningPolicy ;

		/*!
		*	\brief Changes the QPaletteItem's PositioningPolicy.
		*/
		void setPositioningPolicy( const PositioningPolicy& policy );

		void setPaletteEnabled(bool isEnabled);
		bool isPaletteEnabled() const;
		
		/*!
		*	\brief Can be used instead of calling setVisible( true ).
		*
		*	\note When using the specialShow(), you must call specialHide() instead of the classic setVisible( false )
		*			to hide the palette.
		*/
/*		void specialShow();
		void specialHide();
	
	protected Q_SLOTS:
		void showAnimation();
*/

	protected :

		void init(float radius);

		virtual void reorderItems();

		/*!
		*	\brief	The implementation of showOnItem defines the behaviour (or policy) of the palette 
		*			when it has to be shown for an interaction of an item (the target) with 
		*			another (the droppedItem).
		*
		*			You can override this function to define a specific behaviour. The
		*			standard implementation uses the QPaletteItem's PositioningPolicy.
		*/
		virtual void showOnItem( const QGraphicsItem * target , const QGraphicsItem * droppedItem );

		/*!
		*	\brief	Notify the QPaletteItem that something is being dragged over it.
		*			The palette highlights the undermouse interaction if there is one.
		*/
		virtual void moveOver( const QPointF& pos );

		bool canHandle(int droppedItemId , int targetItemId) const;

		/*!
		*	\brief Returns true if there currently is an highlighted interaction.
		*/
		bool hasActiveInteraction() const;
		
		/*!
		*	\brief Returns the id of the currently highlighted interaction (as specified in addInteraction).
		*
		*	\note If !hasActiveInteraction(), returns -1.
		*/
		int activeInteraction() const;

		void setCenter( const QPointF& center );
		
		typedef struct Interaction {
			int mId;
			QGraphicsItem * mItem;
			QPainterPath * mPath;
		} Interaction;
		
		QList<int> mHandledDroppedItemIds;
		QList<int> mHandledTargetItemIds;
		QList<Interaction> mInteractions;
		int mOriginAngle;
		int mHighlightedInteractionIndex;
		bool mNeedReorder;
		bool mIsPaletteEnabled;
		float mRadius;
//		QTimer *	mShowAnimationTimer;
//		int			mShowAnimationCounter;
		PositioningPolicy	mPositioningPolicy;
		QPen mHighlightPen;
		QBrush mHighlightBrush;
};

/*!	\brief	QPaletteItemDropper is part of the QPaletteItem system. (see QPaletteManager)
*
*			Once you add a (usually as a child) QPaletteItemDropper to a QGraphicsItem, you make it possible to
*			use the QPaletteItem system with this QGraphicsItem.
*			The QPaletteItemDropper keeps all the identification informations about an item used in the QPaletteItem system:
*				- paletteId: id of the type of the related QGraphicsItem
*				- acceptedDroppersId: id of the type of item that can be dropped on the QPaletteItemDropper's related item. 
*				Per example, "I am a QPaletteItemDropper of id 2, and I accept that someone drops QPaletteItemDropper of 
*				id 2 (like me), 3 & 5".
*				Note: beware of having the QPaletteItems to handle all those possible combinations.
*
*			Moreover :
*				- the QPaleteItemDropper's related item (usually also its parent) should call the 
*				slot QPaletteItemDropper::updateGeometry when its own geometry changes, to maintain
*				a valid detection rectangle for the QPaletteItemDropper.
*				- the  QPaleteItemDropper's related item (usually also its parent) should call the
*				slot QPaletteItemDropper::dragStarted when a drag (or move) is being started.
*/
class QPaletteItemDropper : public QObject , public QGraphicsRectItem 
{

	Q_OBJECT

	public :
	
		/*!	\brief	Creates a QPaletteItemDropper with the specified paletteId, rect, and parent.
		*
		*	\param	paletteId Allows QPaletteItems to know if they should accept or not
		*			a drop from this QPaletteItemDropper.
		*	\param	rect Rect of the QPaletteItemDropper. Usually, its parentItem boundingRect.
		*	\param	parent The parentItem.
		*/
		QPaletteItemDropper(int paletteId , const QList<int>& acceptedDroppersId, QGraphicsItem* relatedItem, QGraphicsItem * parent);
		~QPaletteItemDropper();
		
		/*!
		*	\brief Returns the QPaletteItemDropper's own paletteId.
		*/
		int paletteId() const;
		
		/*!
		*	\brief Returns true if this QPaletteItemDropper accepts
		*/
		bool isAccepted(int paletteId) const;
	
		QGraphicsItem* item() { return mItem; }

	public Q_SLOTS:

		/*!
		*	\brief Updates the paletteItemDropper rect with the specified QRectF ( it's a QGraphicsRectItem::setRect slot ).
		*/
		void updateGeometry(const QRectF& newGeometry);

		void dragStarted(int & outId , QGraphicsItem*& outItem);

	protected :

		QGraphicsItem* mItem;
		int mPaletteId;
		QList<int> mAcceptedPaletteIds;


};

/*!
*	\brief QPaletteManager class manages the QPaletteItem system. 
*	
*	The QPaletteItem system is made of :
*		- 1 instance of QPaletteManager, QPaletteManager::mInstance, a singleton.
*		- 1 QPaletteItemDropper for each QGraphicsItem you want to use in the QPaletteItem system
*		- several QPaletteItem ; each QPaletteItem regroups a category of interactions between 2 QGraphicsItem
*
*	See QPaletteItemDropper & QPaletteItem for more informations about how to use them.
*
*	How to use the QPaletteManager :
*	1. QPaletteManager::move : The QPaletteManager must be noticed when you are moving a "palette enabled" QGraphicsItem, 
*	so that it can detect the collisions between the QGraphicsItem's QPaletteItemDropper, and other QPaletteItemDropper.
*	If an interaction is possible (see QPaletteItemDropper), the corresponding QPaletteItem will appear, allowing to choose
*	between different interactions.
*	2. QPaletteManager::drop : The QPaletteManager must also be noticed when you drop a "palette enabled" QGraphicsItem, or when you leave the
*	drop zone ( on a QGraphicsView::dragLeaveEvent, per example ).
*	3. QPaletteManager::interactionActivated : Output of the system, emitted when an interaction is activated by dropping an item on a valid
*	part of a QPaletteItem.
*
*	Other QPaletteManager settings:
*		- setActivationDelay : sets the time the mouse must be kept above an item without moving to show a QPaletteItem.
*/
class QPaletteManager : public QObject
{
	Q_OBJECT

	friend class QPaletteItem;
	friend class QPaletteItemDropper;

	public:

		static QPaletteManager * instance();

		//detectionRect & pos are in scene coordinates.
		void move(const QPointF& mouseScenePos , const QGraphicsScene * scene , int draggedItemPaletteId , QGraphicsItem * draggedItem );
		bool drop(const QGraphicsScene * scene , int draggedItemPaletteId , QGraphicsItem * draggedItem );

/*
		void setDragIn();
		void setDragOut();
*/

		/*!
		*	\brief Changes the palette activation delay (time spent above an item to activate the palette).
		*
		*	Default is 1000ms.
		*/
		void setActivationDelay(int msTime);

	Q_SIGNALS:
	
		/*!	\brief	Emitted when an item is dropped on an interaction of a QPaletteItem:
		*		- droppedItem is the item that was currently dragged, and has been dropped on the QPaletteItem.
		*		- targetItem is the item on which the droppedItem is dropped.
		*		- droppedItemPaletteId is the paletteId of the QPaletteItemDropper of the droppedItem
		*		- targetItemPaletteId is the paletteId of the QPaletteItemDropper of the targetItem
		*		- interactionId is the id of the interaction on which the item has been dropped.
		*/
		void interactionActivated(QGraphicsItem* droppedItem, QGraphicsItem* targetItem, int droppedItemPaletteId, int targetItemPaletteId, int interactionId);

	protected Q_SLOTS:
		void showPalette();

	protected:
		
		QPaletteManager();
		static QPaletteManager * mInstance;

		void registerPalette(QPaletteItem* palette);
		void unregisterPalette(QPaletteItem* palette);
		void registerDropper(QPaletteItemDropper* dropper);
		void unregisterDropper(QPaletteItemDropper* dropper);
//		void setDraggedItem( QGraphicsItem* item , int paletteId );

		QPaletteItemDropper* mDropDestination;
		QList<QPaletteItem*> mPalettes;
		QList<QPaletteItemDropper*> mDroppers;
		QGraphicsItem* mDraggedItem;
		int mDraggedItemPaletteId;		//< Stores the draggedItem palette id while the timer goes. (between start and timeout)
		QTimer * mShowPaletteTimer;		//< Stores the draggedItem while the timer goes.			(between start and timeout)
		QPaletteItem* mShownPalette;
		QPointF mLastMousePos;
		int mActivationDelay;
//		bool mIsDragging;
};
		
#endif //PALETTE_ITEM_H
