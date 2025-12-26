/*
 * QGuidoItemContainerFactory.cpp
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
#include "QGuidoItemContainerFactory.h"

#include "QGuidoItemContainer.h"
#include "QSimpleItemContainer.h"
#include <assert.h>

#include <QImageReader>

#define DOM_GMN_CONTAINER_ITEM_DISPLAY_TYPE	"displayType"

//------------------------------------------------------------------------------------------
QGuidoItemContainer * QGuidoItemContainerFactory::buildGuidoItemContainer( QGraphicsItem * parent )
{
	return new QGuidoItemContainer(parent);
}

//------------------------------------------------------------------------------------------
QGuidoItemContainer * QGuidoItemContainerFactory::buildGuidoItemContainer( const QGuidoItemContainer * other , QGraphicsItem * parent )
{
	return new QGuidoItemContainer(other,parent);
}

//------------------------------------------------------------------------------------------
QLanguageItem * QGuidoItemContainerFactory::buildSimpleTextItemContainer( QGraphicsItem * parent )
{
	QGraphicsSimpleTextItem * item = new QGraphicsSimpleTextItem();
	QGraphicsSimpleTextItemAdapter * adapter = new QGraphicsSimpleTextItemAdapter(item);
	return new QSimpleItemContainer( item , adapter , parent );
}

//------------------------------------------------------------------------------------------
QLanguageItem * QGuidoItemContainerFactory::buildLanguageItem( QGraphicsItem * parent )
{
	return buildGuidoItemContainer( parent );
}

//------------------------------------------------------------------------------------------
QLanguageItem * QGuidoItemContainerFactory::buildLanguageItem( const QLanguageItem * other , QGraphicsItem * parent )
{
	if( dynamic_cast<const QGuidoItemContainer*> (other) )
		return buildGuidoItemContainer( (QGuidoItemContainer*)other , parent );
	else if ( dynamic_cast<const QSimpleItemContainer*> (other) )
	{
		const QSimpleItemContainer* simpleItemContainer = (QSimpleItemContainer*)other;
		QGraphicsItem * item = 0;
		QItemAdapter * adapter = 0;
		if ( dynamic_cast<const QGraphicsPixmapItemAdapter*> (simpleItemContainer->adapter()) )
		{
			return QPixmapItemContainer::buildPixmapItemContainer( simpleItemContainer , parent );
		}
		else if ( dynamic_cast<const QGraphicsSimpleTextItemAdapter*> (simpleItemContainer->adapter()) )
		{
			item = new QGraphicsSimpleTextItem();
			adapter = new QGraphicsSimpleTextItemAdapter((QGraphicsSimpleTextItem*)item);
		}
		if ( item && adapter )
			return new QSimpleItemContainer( item , adapter , simpleItemContainer , parent );
	}
	
	assert(0);
	return 0;
}
		
//------------------------------------------------------------------------------------------
QLanguageItem * QGuidoItemContainerFactory::buildLanguageItem( const QMimeData * mimeData , QGraphicsItem * parent )
{
	if ( QGuidoItemContainer::recognizes(mimeData) )
	{
		return new QGuidoItemContainer(mimeData, parent );
	}
	else
	{
		QGraphicsItem * item = 0;
		QItemAdapter * adapter = 0;
		if ( QGraphicsPixmapItemAdapter::recognizes(mimeData) )
		{
			return QPixmapItemContainer::buildPixmapItemContainer( mimeData , parent );
		}
		else if ( QGraphicsSimpleTextItemAdapter::recognizes(mimeData) )
		{
			item = new QGraphicsSimpleTextItem();
			adapter = new QGraphicsSimpleTextItemAdapter((QGraphicsSimpleTextItem*)item);
		}
		if ( item && adapter )
			return new QSimpleItemContainer( item , adapter , mimeData , parent );
		assert(0);
		return 0;
	}
}

//------------------------------------------------------------------------------------------
QLanguageItem * QGuidoItemContainerFactory::buildLanguageItem( const QDomElement * domElement , QGraphicsItem * parent )
{
	if ( QGuidoItemContainer::recognizes( domElement ) )
	{
		return new QGuidoItemContainer(domElement, parent );
	}
	else 
	{
		QGraphicsItem * item = 0;
		QItemAdapter * adapter = 0;
		if ( QGraphicsPixmapItemAdapter::recognizes(domElement) )
		{
			return QPixmapItemContainer::buildPixmapItemContainer( domElement , parent );
		}
		else if ( QGraphicsSimpleTextItemAdapter::recognizes(domElement) )
		{
			item = new QGraphicsSimpleTextItem();
			adapter = new QGraphicsSimpleTextItemAdapter((QGraphicsSimpleTextItem*)item);
		}
		if ( item && adapter )
			return new QSimpleItemContainer( item , adapter , domElement , parent );
			
		assert(0);
		return 0;
	}
}

//------------------------------------------------------------------------------------------
QLanguageItem * QGuidoItemContainerFactory::buildLanguageItemFromFile( const QString& fileName , QGraphicsItem * parent )
{
	QLanguageItem * languageItem;
	// 1. Try to build a QGraphicsPixmapItem container
	if ( QImageReader( fileName ).canRead() )
	{
		languageItem = QPixmapItemContainer::buildPixmapItemContainer( parent );
		languageItem->loadFile( fileName );
		languageItem->unlinkFile();	// QPixmapItemContainer are never file-linked.
		return languageItem;
	}
	else
	{
		// 2.	The file is not a valid image file 
		//		=> Try to build a QGuidoItemContainer
		languageItem = buildGuidoItemContainer( parent );
		languageItem->loadFile( fileName );
		if ( languageItem->isValid() )
			return languageItem;
		else
		{
			delete languageItem;
			// 3.	The file is not a valid Guido file
			//		=> add  the contents of the file as a QGraphicsSimpleTextItem
			QGraphicsSimpleTextItem * i = new QGraphicsSimpleTextItem();
			languageItem = new QSimpleItemContainer( i , new QGraphicsSimpleTextItemAdapter(i) , parent );
			languageItem->loadFile( fileName );
			return languageItem;
		}
	}
}
