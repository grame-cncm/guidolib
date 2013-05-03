/*
 * QLanguageCommandReader.h
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
#ifndef LANGUAGE_COMMAND_READER_H
#define LANGUAGE_COMMAND_READER_H

#include <QString>

class QDomElement;

class QLanguageCommandVisitor
{
	public :
	
		virtual ~QLanguageCommandVisitor() {}
	
		virtual void categoryBegin( const QString& name ) = 0;
		virtual void categoryEnd() = 0;
		virtual void newElement( const QString& name , const QString& value , int insertIndex ) = 0;
};

class QLanguageCommandReader
{
	public :
		QLanguageCommandReader();
		bool read( QLanguageCommandVisitor * visitor , const QString& fileName );
		
	protected : 
	
		void readNode( QDomElement * element);
		QLanguageCommandVisitor * mVisitor;
};

#endif //GUIDO_COMMAND_READER_H

