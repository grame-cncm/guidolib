/*
 * QGuidoItemContainerFactory.h
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
#ifndef GUIDO_ITEM_CONTAINER_FACTORY_H
#define GUIDO_ITEM_CONTAINER_FACTORY_H

#include "QLanguageItemFactory.h"

class QGuidoItemContainer;

class QGuidoItemContainerFactory : public QLanguageItemFactory
{

	public:

		QGuidoItemContainer * buildGuidoItemContainer( QGraphicsItem * parent = 0 );
		
		/**
		*	\brief Builds a copy of the specified QGuidoItemContainer. 
		*
		*	The DisplayType can be forced and then be different of the DisplayType of
		*	the specified QGuidoItemContainer ; else, you can leave displayType to TYPE_UNDEFINED.
		*/
		QGuidoItemContainer * buildGuidoItemContainer( const QGuidoItemContainer * other , QGraphicsItem * parent = 0);
		
		/**
		*	\brief Builds an empty QLanguageItem thats only displays the text.
		*/
		QLanguageItem * buildSimpleTextItemContainer( QGraphicsItem * parent = 0 );
		
		QLanguageItem * buildLanguageItem( QGraphicsItem * parent = 0 );
		QLanguageItem * buildLanguageItem( const QLanguageItem * other		, QGraphicsItem * parent = 0);
		QLanguageItem * buildLanguageItem( const QMimeData * mimeData		, QGraphicsItem * parent = 0);
		QLanguageItem * buildLanguageItem( const QDomElement * domElement	, QGraphicsItem * parent = 0);
		
		QLanguageItem * buildLanguageItemFromFile( const QString& fileName , QGraphicsItem * parent = 0 );
};

#endif //GUIDO_ITEM_CONTAINER_FACTORY_H
