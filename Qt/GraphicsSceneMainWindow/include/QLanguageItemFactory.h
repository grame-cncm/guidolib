/*
 * QLanguageItemFactory.h
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
#ifndef LANGUAGE_ITEM_FACTORY_H
#define LANGUAGE_ITEM_FACTORY_H

#include <QMimeData>
#include <QDomElement>
#include <QGraphicsItem>

#include "QLanguageItem.h"

/*
*	\brief A factory interface, that can build QLanguageItem from :
*				- scratch,
*				- another QLanguageItem (copy)
*				- a QMimeData (d&d information)
*				- a QDomElement (element of a XML-like file)
*/
class QLanguageItemFactory
{

	public:

		virtual ~QLanguageItemFactory() {}

		virtual QLanguageItem * buildLanguageItem( QGraphicsItem * parent = 0 ) = 0;
		virtual QLanguageItem * buildLanguageItem( const QLanguageItem * other		, QGraphicsItem * parent = 0) = 0;
		virtual QLanguageItem * buildLanguageItem( const QMimeData * mimeData		, QGraphicsItem * parent = 0) = 0;
		virtual QLanguageItem * buildLanguageItem( const QDomElement * domElement	, QGraphicsItem * parent = 0) = 0;
		
		virtual QLanguageItem * buildLanguageItemFromFile( const QString& fileName , QGraphicsItem * parent = 0 ) 
			{ QLanguageItem * item = buildLanguageItem(parent); item->loadFile(fileName); return item; }
		virtual QLanguageItem * buildLanguageItemFromCode( const QString& code , QGraphicsItem * parent = 0 ) 
			{ QLanguageItem * item = buildLanguageItem(parent); item->setCode(code); return item; }
};

#endif //GUIDO_ITEM_CONTAINER_FACTORY_H
