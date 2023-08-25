/*
 * QLanguageCommandPaletteLayout.cpp
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
#include <QtGui>
#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
# define Qt6 true
#endif

#include "QLanguageCommandPaletteLayout.h"

//----------------------------------------------------------------------
QLanguageCommandPaletteLayout::QLanguageCommandPaletteLayout(QWidget *parent, int margin, int spacing)
    : QLayout(parent)
{
#if Qt6
    setContentsMargins(margin, margin, margin, margin);
#else
    setMargin(margin);
#endif
    setSpacing(spacing);
}

//----------------------------------------------------------------------
QLanguageCommandPaletteLayout::QLanguageCommandPaletteLayout(int spacing)
{
    setSpacing(spacing);
}

//----------------------------------------------------------------------
QLanguageCommandPaletteLayout::~QLanguageCommandPaletteLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

//----------------------------------------------------------------------
void QLanguageCommandPaletteLayout::addItem(QLayoutItem *item)
{
    mItemList.append(item);
}

//----------------------------------------------------------------------
int QLanguageCommandPaletteLayout::count() const
{
    return (int)mItemList.size();
}

//----------------------------------------------------------------------
QLayoutItem *QLanguageCommandPaletteLayout::itemAt(int index) const
{
    return mItemList.value(index);
}

//----------------------------------------------------------------------
QLayoutItem *QLanguageCommandPaletteLayout::takeAt(int index)
{
    if (index >= 0 && index < mItemList.size())
        return mItemList.takeAt(index);
    else
        return 0;
}

//----------------------------------------------------------------------
//Qt::Orientations QLanguageCommandPaletteLayout::expandingDirections() const
//{
//    return 0;
//}

//----------------------------------------------------------------------
void QLanguageCommandPaletteLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

//----------------------------------------------------------------------
QSize QLanguageCommandPaletteLayout::sizeHint() const
{
    return minimumSize();
}

//----------------------------------------------------------------------
QSize QLanguageCommandPaletteLayout::minimumSize() const
{
    QSize size;
    QLayoutItem *item;
    foreach (item, mItemList)
        size = size.expandedTo(item->minimumSize());

#if Qt6
    size += QSize(5,5);
#else
    size += QSize(2*margin(), 2*margin());
#endif
    return size;
}

#include <QtDebug>

//----------------------------------------------------------------------
void fillWidth(int startX , int endX , QList<QRect> & items)
{
	if ( items.size() == 0 )
		return;
	
	int additionalSpace = endX - items.last().right();
	int spaceAdded = additionalSpace / items.size();
	int currentX = startX;
	for ( int i = 0 ; i < items.size() ; i++ )
	{
		int newWidth = items[i].width() + spaceAdded;
		items[i].setWidth( newWidth );
		items[i].moveLeft( currentX );
		currentX += newWidth;
	}
	items.last().setRight( endX );
}

//----------------------------------------------------------------------
void fitInHeight( int startY , int endY , QList<QRect> & items )
{
	if ( items.size() == 0 )
		return;

	if ( items.last().bottom() > endY )
	{
		//We need to compress the height of the items.
		float compressFactor = (endY - startY) / float( items.last().bottom() - items.first().top() );
		//We'll apply on each item : height = compressFactor * height.
		int previousY = startY;
		int currentY = startY;
		for ( int i = 0 ; i < items.size() ; i++ )
		{
			items[i].setHeight( items[i].height() * compressFactor );
			if ( items[i].top() != previousY )
			{
				//Linebreak
				previousY = items[i].top();
				currentY = items[i-1].bottom();
			}
			items[i].moveTop( currentY );
		}
	}
}
			
//----------------------------------------------------------------------
int QLanguageCommandPaletteLayout::doLayout(const QRect &rect, bool testOnly) const
{
	if ( mItemList.size() == 0 )
		return 0;

	QList<QSize> sizeHints;
	QList<QRect> geometries;
	
	int i;
	int surface = 0;
	int largestItem = 0;
	for ( i = 0 ; i < mItemList.size() ; i++ )
	{
		sizeHints << mItemList[i]->sizeHint();
		surface += ( sizeHints[i].width() * sizeHints[i].height() );
		largestItem = qMax( largestItem , sizeHints[i].width() );
	}
	
	float surfaceRatio = surface / float( rect.width() * rect.height() );
	surfaceRatio = qMin( surfaceRatio , 1.0f );
	float sideRatio = qSqrt( surfaceRatio );
	int newRectWidth = rect.width() * sideRatio;
	int newRectHeight = rect.height() * sideRatio;

	newRectWidth = qMin( rect.width() , qMax( newRectWidth , largestItem ) );

	QRect newRect( (rect.width()-newRectWidth)/2 , qMax( (rect.height()-newRectHeight)/2 - sizeHints[0].height() , 0 ) , newRectWidth , newRectHeight );

    int x = newRect.x();
    int y = newRect.y();	

	int lineHeight = 0;
	QList<QRect> lineItems;
	for ( i = 0 ; i < mItemList.size() ; i++ )
	{
        int nextX = x + sizeHints[i].width() + spacing();
        if ( (nextX - spacing() > newRect.right()) && (lineHeight > 0) )
		{
			//Linebreak
			fillWidth(newRect.left() , newRect.right() , lineItems);
			geometries << lineItems;
			lineItems.clear();
			
            x = newRect.x();
            y = y + lineHeight + spacing();
            nextX = x + sizeHints[i].width() + spacing();
            lineHeight = 0;
        }
		
		lineItems << QRect(QPoint(x, y), sizeHints[i] );

        x = nextX;
        lineHeight = qMax(lineHeight, sizeHints[i].height());
    }
	//Linebreak
	fillWidth(newRect.left() , newRect.right() , lineItems);
	geometries << lineItems;
	lineItems.clear();
	
	fitInHeight( newRect.top() , rect.bottom() , geometries );

	if (!testOnly)
	{
		for ( i = 0 ; i < mItemList.size() ; i++ )
		{
			mItemList[i]->setGeometry( geometries[i] );
		}		
	}
	
    return y + lineHeight - newRect.y();
}

