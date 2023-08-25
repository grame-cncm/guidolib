/*
 * QPaletteItem.cpp
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
#include "QPaletteItem.h"

#include <math.h>

#include <QPainter>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QTimer>
#include <QtDebug>

#define PALETTE_BRUSH_ALPHA 200
#define PALETTE_HIGHLIGHT_BRUSH QBrush( QColor(255,230,230,PALETTE_BRUSH_ALPHA) )
#define PALETTE_HIGHLIGHT_PEN	QPen(	QColor(255,0,0,220) )

#define PALETTE_HOLE_RATIO 0.25f
#define MIN(a,b) ( ((a)<(b)) ? (a) : (b) )
#define MAX(a,b) ( ((a)>(b)) ? (a) : (b) )

#define PALETTE_DEFAULT_RADIUS 100

#define SHOW_ANIMATION_TIMER_INTERVAL 80
#define SHOW_ANIMATION_TIMER_ITERATION 4


//-------------------------------------------------------------------------
//								QPaletteItem class						///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QPaletteItem::QPaletteItem( const QList<int>& handledDroppedItemIds , const QList<int>& handledTargetItemIds ,
							float radius , QGraphicsItem * parent ) :
	QGraphicsEllipseItem( QRect() , parent ),
	mHandledDroppedItemIds(handledDroppedItemIds),
	mHandledTargetItemIds(handledTargetItemIds)
{
	init(radius);
}

//-------------------------------------------------------------------------
QPaletteItem::QPaletteItem( int paletteId , float radius , QGraphicsItem * parent ) :
	QGraphicsEllipseItem( QRect() , parent )
{
	mHandledTargetItemIds << paletteId;
	mHandledDroppedItemIds << paletteId;
	init(radius);
}

//-------------------------------------------------------------------------
void QPaletteItem::init(float radius)
{ 
	setRadius( radius );
	mHighlightedInteractionIndex = -1;
	mOriginAngle = 0;
	mIsPaletteEnabled = true;
	mPositioningPolicy = CENTER_POLICY;
	setVisible(false);

	mHighlightPen = PALETTE_HIGHLIGHT_PEN;
	mHighlightBrush = PALETTE_HIGHLIGHT_BRUSH;

/*	
	mShowAnimationTimer = new QTimer(this);
	connect( mShowAnimationTimer , SIGNAL(timeout()) , this , SLOT( showAnimation() ) );
*/	
	QPaletteManager::instance()->registerPalette( this );
}

//-------------------------------------------------------------------------
QPaletteItem::~QPaletteItem()
{
	for ( int i = 0 ; i < mInteractions.size() ; i++ )
	{
		delete mInteractions[i].mPath;
	}
	
	QPaletteManager::instance()->unregisterPalette( this );
}

//-------------------------------------------------------------------------
void QPaletteItem::setRadius( float newRadius )
{
	QPointF oldCenter = pos() + rect().center();
	
	mRadius = newRadius;
	setRect( QRectF(0,0,mRadius*2,mRadius*2) );
	mNeedReorder = true;
	update();
	
	setCenter( oldCenter );
}

//-------------------------------------------------------------------------
void QPaletteItem::addInteraction( QGraphicsItem * itemIcon , int interactionId )
{
	itemIcon->setParentItem( this );
	
	Interaction interaction;
	interaction.mId = interactionId;
	interaction.mItem = itemIcon;
	interaction.mPath = 0;
		
	mInteractions.append( interaction );
	
	mNeedReorder = true;
	update();
}

//-------------------------------------------------------------------------
void QPaletteItem::setOriginAngle( int degreeOriginAngle )
{
	mOriginAngle = degreeOriginAngle;
	mNeedReorder = true;
	update();
}

//-------------------------------------------------------------------------
void QPaletteItem::setHighlightStyle( const QPen& pen , const QBrush& brush )
{
	mHighlightPen = pen;
	mHighlightBrush = brush;
}

#define PI 3.1416f
float degToRad( int degree )
{
	return 2*PI*degree / 360.0f;
}

QPointF polarToOrtho( float radius , float teta  )
{
	return radius * QPointF( cos(teta) , -sin(teta) );
}

QPointF polarToOrtho( float radius , int angle  )
{
	float teta = degToRad( angle );
	return polarToOrtho( radius , teta );
}

//-------------------------------------------------------------------------
void QPaletteItem::paint(QPainter* painter , const QStyleOptionGraphicsItem*  , QWidget* )
{
	if ( mNeedReorder )
		reorderItems();

	painter->setRenderHint( QPainter::Antialiasing );

	//Render non-highlighted parts
	QColor brushColor( brush().color() );
	brushColor.setAlpha( PALETTE_BRUSH_ALPHA );
	painter->setBrush( brushColor.lighter(150) );
	painter->setPen( pen() );
	for ( int i = 0 ; i < mInteractions.size() ; i++ )
	{
		if ( i == mHighlightedInteractionIndex )
			continue;

		painter->drawPath( *(mInteractions[i].mPath) );
	}
	
	//Render highlighted part
	if ( mHighlightedInteractionIndex != -1 )
	{
		painter->setBrush(	mHighlightBrush );
		painter->setPen(	mHighlightPen );
		painter->drawPath( *(mInteractions[mHighlightedInteractionIndex].mPath) );
	}
}

//-------------------------------------------------------------------------
void QPaletteItem::moveOver( const QPointF& pos )
{
	if (!mIsPaletteEnabled)
		return;

	if ( mNeedReorder )
		reorderItems();
	
	for ( int i = 0 ; i < mInteractions.size() ; i++ )
	{
		if ( mInteractions[i].mPath->contains(pos) )
		{
			if ( mHighlightedInteractionIndex != i )
			{
				mHighlightedInteractionIndex = i;
				update();
			}
			return;
		}
	}

	if ( mHighlightedInteractionIndex != -1 )
	{
		mHighlightedInteractionIndex = -1;
		update();
	}
}

//-------------------------------------------------------------------------
bool QPaletteItem::hasActiveInteraction() const
{
	return ( mHighlightedInteractionIndex != -1 );
}

//-------------------------------------------------------------------------
int QPaletteItem::activeInteraction() const
{
	if ( !hasActiveInteraction() )
		return -1;
	else
		return mInteractions[ mHighlightedInteractionIndex ].mId;
}

/*
//-------------------------------------------------------------------------
void QPaletteItem::specialShow()
{
	mShowAnimationCounter = 0;
	mShowAnimationTimer->start( SHOW_ANIMATION_TIMER_INTERVAL );
}

//-------------------------------------------------------------------------
void QPaletteItem::specialHide()
{
	setVisible( false );
	mShowAnimationTimer->stop();
}

//-------------------------------------------------------------------------
void QPaletteItem::showAnimation()
{
	if ( mShowAnimationCounter < SHOW_ANIMATION_TIMER_ITERATION )
	{
		setVisible( !isVisible() );
		mShowAnimationCounter++;
	}
	else
	{
		mShowAnimationTimer->stop();
		setVisible( true );
	}
}
*/

#define RADIUS_FACTOR_INCREMENT 0.1f
#define RADIUS_FACTOR_MARGIN 0.1f
#define HYPOTHENUSE(a)  (sqrt( (a).width() * (a).width() + (a).height() * (a).height() ))
//-------------------------------------------------------------------------
void QPaletteItem::reorderItems()
{
	QPointF center = QPointF( rect().width()/2 , rect().height()/2 );

	if ( mInteractions.size() == 0 )
		return;

	if ( mInteractions.size() == 1 )
	{
		if ( mInteractions[0].mPath )
			delete mInteractions[0].mPath;
			
		mInteractions[0].mPath = new QPainterPath();
		mInteractions[0].mPath->addEllipse(rect());
		
		QRectF itemBoundingRect = mInteractions[0].mItem->mapToParent( mInteractions[0].mItem->boundingRect() ).boundingRect();
		mInteractions[0].mItem->setPos( center - QPointF(itemBoundingRect.width()/2 , itemBoundingRect.height()/2) );
		while ( !mInteractions[0].mPath->contains( itemBoundingRect ) )
		{
			mInteractions[0].mItem->setScale(0.9f);
			itemBoundingRect = mInteractions[0].mItem->mapToParent( mInteractions[0].mItem->boundingRect() ).boundingRect();
			mInteractions[0].mItem->setPos( center - QPointF(itemBoundingRect.width()/2 , itemBoundingRect.height()/2) );
		}
		while ( mInteractions[0].mPath->contains( itemBoundingRect ) )
		{
			mInteractions[0].mItem->setScale(1/0.9f);
			itemBoundingRect = mInteractions[0].mItem->mapToParent( mInteractions[0].mItem->boundingRect() ).boundingRect();
			mInteractions[0].mItem->setPos( center - QPointF(itemBoundingRect.width()/2 , itemBoundingRect.height()/2) );
		}
		mInteractions[0].mItem->setScale(0.9f);
		return;
	}

	QRectF littleRect(rect().x() , rect().y() , rect().width() * PALETTE_HOLE_RATIO , rect().height() * PALETTE_HOLE_RATIO );
	littleRect.moveTo( rect().x() + rect().width()/2 - littleRect.width()/2 , rect().y() + rect().height()/2 - littleRect.height()/2 );
		
	int baseItemAngle = 360 / mInteractions.size();
	int completingAngle = 360 - baseItemAngle * (int)mInteractions.size();
	
	int currentAngle = mOriginAngle - baseItemAngle/2;
	for ( int i = 0 ; i < mInteractions.size() ; i++ )
	{
		int itemAngle = baseItemAngle;
		if ( completingAngle > 0 )
		{
			itemAngle++;
			completingAngle--;
		}

		// A. Build the element's pie path.
		if ( mInteractions[i].mPath )
			delete mInteractions[i].mPath;
		
		mInteractions[i].mPath = new QPainterPath();

		QPointF startPointA = center + polarToOrtho( littleRect.width()/2 , currentAngle );
		QPointF endPointA = center + polarToOrtho( rect().width()/2 , currentAngle );
		mInteractions[i].mPath->moveTo(startPointA);
		mInteractions[i].mPath->lineTo(endPointA);
		mInteractions[i].mPath->arcTo(rect(),		currentAngle,			itemAngle);		
		mInteractions[i].mPath->arcTo(littleRect,	currentAngle+itemAngle, -itemAngle);
			
		// B. Resize & set the position of the element's item.
		float radiusFactor = 0.5f;
		float resultingScale = 0;
		float maxScale = 0;
		float posRadius;
		
		while ( ( resultingScale >= maxScale ) && ( radiusFactor < (1-RADIUS_FACTOR_MARGIN) ) )
		{
			mInteractions[i].mItem->resetTransform();
			
			// B.1 Initialize: Put the element's item (<=> icon) at the center of its pie.
			posRadius = littleRect.width()/2 + (mRadius - littleRect.width()/2) * radiusFactor;
		
			QRectF itemBoundingRect = mInteractions[i].mItem->mapToParent( mInteractions[i].mItem->boundingRect() ).boundingRect();
			QPainterPath itemBoundingCircle;
			float boundingCircleRadius = HYPOTHENUSE( itemBoundingRect ) / 2.0f;
			float startRadius = boundingCircleRadius;
			itemBoundingCircle.addEllipse( itemBoundingRect.center() , boundingCircleRadius , boundingCircleRadius );
	
			// B.2 Make it fit in : Scale down the element's item until it fits inside the element's pie.
			while ( !mInteractions[i].mPath->contains( itemBoundingCircle ) )
			{				
				boundingCircleRadius = 0.9f * boundingCircleRadius;
				itemBoundingCircle = QPainterPath();
				itemBoundingCircle.addEllipse( center + polarToOrtho( posRadius, currentAngle + itemAngle/2 ) , boundingCircleRadius , boundingCircleRadius );
			}
			// B.3 Make it the biggest possible : And then, scale up the element's item while it fits inside the element's pie.
			while ( mInteractions[i].mPath->contains( itemBoundingCircle ) )
			{				
				boundingCircleRadius = boundingCircleRadius / 0.9f;
				itemBoundingCircle = QPainterPath();
				itemBoundingCircle.addEllipse( center + polarToOrtho( posRadius, currentAngle + itemAngle/2 ) , boundingCircleRadius , boundingCircleRadius );
			}
			// B.4 Finalize : Finally, scale down 1 time the element's item, so that 
			//	- the scale of the item is the biggest possible, 
			//	- and the item still fits in the pie.
		
			maxScale = resultingScale;	
			resultingScale = boundingCircleRadius / startRadius;
			
			radiusFactor += RADIUS_FACTOR_INCREMENT;
		}
		
		posRadius = littleRect.width()/2 + (mRadius - littleRect.width()/2) * (radiusFactor-2*RADIUS_FACTOR_INCREMENT);
		mInteractions[i].mItem->setScale( maxScale );
		QRectF itemBoundingRect = mInteractions[i].mItem->mapToParent( mInteractions[i].mItem->boundingRect() ).boundingRect();
		mInteractions[i].mItem->setPos( center + polarToOrtho( posRadius, currentAngle + itemAngle/2 ) - QPointF(itemBoundingRect.width()/2 , itemBoundingRect.height()/2) );
		
		currentAngle += itemAngle;		
	}
	
	mNeedReorder = false;
}

//-------------------------------------------------------------------------
void QPaletteItem::setCenter( const QPointF& pos )
{
	setPos( pos - rect().center() );
}

//-------------------------------------------------------------------------
void QPaletteItem::setPaletteEnabled(bool isEnabled)
{
	mIsPaletteEnabled = isEnabled;
}

//-------------------------------------------------------------------------
bool QPaletteItem::isPaletteEnabled() const
{
	return mIsPaletteEnabled;
}

//-------------------------------------------------------------------------
bool QPaletteItem::canHandle(int droppedItemId , int targetItemId) const
{
	return ( mHandledDroppedItemIds.contains( droppedItemId ) && mHandledTargetItemIds.contains( targetItemId ) );
}

//-------------------------------------------------------------------------
void QPaletteItem::setPositioningPolicy( const PositioningPolicy& policy )
{
	mPositioningPolicy = policy;
}

//-------------------------------------------------------------------------
void QPaletteItem::showOnItem( const QGraphicsItem * target , const QGraphicsItem * )
{
	switch ( mPositioningPolicy )
	{
		case CENTER_POLICY:
			setCenter( target->sceneBoundingRect().center() );
			break;
		case CENTER_AND_RESIZE_POLICY:
			setCenter( target->sceneBoundingRect().center() );		
			setRadius( target->sceneBoundingRect().height()/2.0f );
			break;
	}

	QList<QGraphicsItem*> colliding = collidingItems();
	float z = zValue();
	for ( int i = 0 ; i < colliding.size() ; i++ )
		z = MAX( z , colliding[i]->zValue() );

	setZValue( z + 0.1f );
}

//-------------------------------------------------------------------------
//							QPaletteItemDropper class					///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QPaletteItemDropper::QPaletteItemDropper(int dropperId , const QList<int>& acceptedPaletteIds, QGraphicsItem * item , QGraphicsItem * parent) :
	QGraphicsRectItem( QRect() , parent) ,
	mItem(item) ,
	mPaletteId(dropperId),
	mAcceptedPaletteIds(acceptedPaletteIds)
{
	setPen(Qt::NoPen);
	setBrush(Qt::NoBrush);
	
	QPaletteManager::instance()->registerDropper( this );

	if ( mItem )
		updateGeometry( mItem->boundingRect() );
}

//-------------------------------------------------------------------------
QPaletteItemDropper::~QPaletteItemDropper()
{
	QPaletteManager::instance()->unregisterDropper( this );
}

//-------------------------------------------------------------------------
void QPaletteItemDropper::updateGeometry(const QRectF& newGeometry)
{
	setRect( newGeometry );
}

//-------------------------------------------------------------------------
void QPaletteItemDropper::dragStarted(int & outId , QGraphicsItem*& outItem)
{
	outId = mPaletteId;
	outItem = mItem;
//	QPaletteManager::instance()->setDraggedItem( mItem , mPaletteId );
	QPaletteManager::instance()->mShowPaletteTimer->stop();
}

//-------------------------------------------------------------------------
int QPaletteItemDropper::paletteId() const
{
	return mPaletteId;
}

//-------------------------------------------------------------------------
bool QPaletteItemDropper::isAccepted(int paletteId) const
{
	return mAcceptedPaletteIds.contains( paletteId );
}

//-------------------------------------------------------------------------
//						QPaletteManager class							///
//-------------------------------------------------------------------------

QPaletteManager * QPaletteManager::mInstance = 0;

//-------------------------------------------------------------------------
QPaletteManager * QPaletteManager::instance()
{
	if ( !mInstance )
		mInstance = new QPaletteManager();
	
	return mInstance;
}

//-------------------------------------------------------------------------
QPaletteManager::QPaletteManager()
{
	mShowPaletteTimer = new QTimer(this);
	connect( mShowPaletteTimer , SIGNAL(timeout()) , this , SLOT( showPalette() ) );
	
	mShownPalette = 0;
	mActivationDelay = 1000;
}

//-------------------------------------------------------------------------
void QPaletteManager::move(const QPointF& mouseScenePos , const QGraphicsScene * scene , int draggedItemPaletteId , QGraphicsItem * draggedItem )
{	
	//If a palette is already active and the mouse is still over the same drop destination.
	if ( mShownPalette )
	{
		if ( mDropDestination->isUnderMouse() || mShownPalette->isUnderMouse() )
		{
			//Call the palette::moveOver to highlight the undermouse interaction.
			mShownPalette->moveOver( mShownPalette->mapFromScene( mouseScenePos ) );
			return;
		}
	}

	QPaletteItemDropper * oldDropDestination = mDropDestination;

	QPaletteItemDropper * dropDestination  = 0;
	//Watch if there is a QPaletteItemDropper undermouse that accepts the currently dragged item's paletteId.
	//Store the QPaletteItemDropper found in mDropDestination
	for ( int i = 0 ; i < mDroppers.size() ; i++ )
	{
		if (		( mDroppers[i]->scene() == scene )
				&&	( mDroppers[i]->isAccepted( draggedItemPaletteId ) )
				&&	( mDroppers[i]->item() != draggedItem )
				&&	( mDroppers[i]->isUnderMouse() )
			)
		{
			dropDestination = mDroppers[i];
			break;
		}
	}
	
	//If mDropDestination has changed :
	//	- if there was one, stop highlighting the oldDropDestination
	//	- if there is one, start highlighting the new palette
	if ( oldDropDestination != dropDestination )
	{
		if ( mShownPalette )
		{
			mShownPalette->setVisible( false );
//			mShownPalette->specialHide();
			mShownPalette = 0;
		}
		mShowPaletteTimer->stop();
	}
	
	if ( dropDestination )
	{
		if ( !mShownPalette )
		{
			mDraggedItemPaletteId = draggedItemPaletteId;
			mDraggedItem = draggedItem;
			mShowPaletteTimer->start( mActivationDelay );
			mLastMousePos = mouseScenePos;
		}
	}

	mDropDestination = dropDestination;
}

//-------------------------------------------------------------------------
bool QPaletteManager::drop(const QGraphicsScene * scene , int draggedItemPaletteId , QGraphicsItem * draggedItem )
{
	bool interactionHasBeenActivated = false;
	// If there is an active palette 
	if ( mShownPalette )
	{
		// If the drop has been done on a palette interaction, and in the right scene,
		// then emit the corresponding "interactionActivated" signal
		if ( mShownPalette->hasActiveInteraction() && ( mShownPalette->scene() == scene ) )
		{
			Q_EMIT interactionActivated(draggedItem, mDropDestination->item(), draggedItemPaletteId , mDropDestination->paletteId() , mShownPalette->activeInteraction());
			interactionHasBeenActivated = true;
		}
	}
//	setDragOut();

	if ( mShownPalette )
	{
		// The palette is no longer active.
		mShownPalette->setVisible( false );
		mShownPalette = 0;
	}
	mShowPaletteTimer->stop();
	mDropDestination = 0;
	
	return interactionHasBeenActivated;
}

/*
//-------------------------------------------------------------------------
void QPaletteManager::setDragIn()
{
	qDebug() << "QPaletteManager::setDragIn()";
	mIsDragging = true;
}

//-------------------------------------------------------------------------
void QPaletteManager::setDragOut()
{
	qDebug() << "QPaletteManager::setDragOut()";
	mIsDragging = false;
	
	if ( mShownPalette )
	{
		// The palette is no longer active.
		mShownPalette->setVisible( false );
		mShownPalette = 0;
	}
	mShowPaletteTimer->stop();
	mDropDestination = 0;
}
*/

//-------------------------------------------------------------------------
void QPaletteManager::setActivationDelay(int msTime)
{
	mActivationDelay = msTime;
}

//-------------------------------------------------------------------------
void QPaletteManager::registerPalette(QPaletteItem* palette)
{
	mPalettes << palette;
}

//-------------------------------------------------------------------------
void QPaletteManager::unregisterPalette(QPaletteItem* palette)
{
	mPalettes.removeAll( palette );
}

//-------------------------------------------------------------------------
void QPaletteManager::registerDropper(QPaletteItemDropper* dropper)
{
	mDroppers << dropper;
}

//-------------------------------------------------------------------------
void QPaletteManager::unregisterDropper(QPaletteItemDropper* dropper)
{
	mDroppers.removeAll( dropper );
}

/*
//-------------------------------------------------------------------------
void QPaletteManager::setDraggedItem( QGraphicsItem* item , int paletteId )
{
	qDebug() << "QPaletteManager::setDraggedItem : " << paletteId;
	mDraggedItem = item;
	mDraggedItemPaletteId = paletteId;
	mShowPaletteTimer->stop();
}
*/

//-------------------------------------------------------------------------
void QPaletteManager::showPalette()
{
	mShowPaletteTimer->stop();

	// Looking for an enabled palette with the ID corresponding to the dragged item, in the same scene as the mDropDestination
	for ( int i = 0 ; i < mPalettes.size() ; i++ )
	{
		if (		( mPalettes[i]->isPaletteEnabled()) 
				&&	( mPalettes[i]->canHandle( mDraggedItemPaletteId , mDropDestination->paletteId() ) ) 
				&&	( mPalettes[i]->scene() == mDropDestination->scene() ) 
			)
		{
			mShownPalette = mPalettes[i];
			mShownPalette->setVisible( true );
//			mShownPalette->specialShow();
			mShownPalette->showOnItem( mDropDestination->item() , mDraggedItem );
			mShownPalette->moveOver( mShownPalette->mapFromScene( mLastMousePos ) );
			break;
		}
	}

}
