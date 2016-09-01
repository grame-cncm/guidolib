/*
 * QLanguageItemFactory.h
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
