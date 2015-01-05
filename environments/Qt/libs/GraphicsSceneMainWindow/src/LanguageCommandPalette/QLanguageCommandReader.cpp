/*
 * QLanguageCommandReader.cpp
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
#include "QLanguageCommandReader.h"

#define DOM_LANGUAGE_COMMAND_LIBRARY		"LanguageCommandsLibrary"
#define DOM_LANGUAGE_COMMAND_LIBRARY_ROOT	"LanguageCommandsLibraryRoot"
#define DOM_LANGUAGE_COMMAND_CATEGORY		"LanguageCommandsCategory"
#define DOM_LANGUAGE_COMMAND_CATEGORY_NAME	"name"
#define DOM_LANGUAGE_COMMAND				"LanguageCommand"
#define DOM_LANGUAGE_COMMAND_NAME			"name"
#define DOM_LANGUAGE_COMMAND_VALUE			"value"
#define DOM_LANGUAGE_COMMAND_INSERT_INDEX	"insertIndex"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QVariant>

//------------------------------------------------------------------------------------
QLanguageCommandReader::QLanguageCommandReader()
{
	
}

//------------------------------------------------------------------------------------
bool QLanguageCommandReader::read( QLanguageCommandVisitor * visitor , const QString& fileName )
{
	mVisitor = visitor;

	QFile file( fileName );
	if( !file.open( QFile::ReadOnly ) )
	{
		//Can't read file
		qDebug("QLanguageCommandReader::read : Can't read file %s" , fileName.toUtf8().data() );
		return false;
	}

	//Create a new DomDocument with file contents
	QDomDocument doc( DOM_LANGUAGE_COMMAND_LIBRARY );
	if( !doc.setContent( &file ) )
	{
		file.close();
		//File doesn't contain valid XML data
		qDebug("QLanguageCommandReader::read : File doesn't contain valid XML data");
		return false;
	}
	file.close();
	
	//Check the root
	QDomElement root = doc.documentElement();
	if( root.tagName() != DOM_LANGUAGE_COMMAND_LIBRARY_ROOT )
	{
		//No valid root found.
		qDebug("QLanguageCommandReader::read : No valid root found");
		return false;
	}
	
	readNode( &root );
	
	return true;
}

//------------------------------------------------------------------------------------
void QLanguageCommandReader::readNode( QDomElement * element)
{
	QDomNode n = element->firstChild();
	while ( !n.isNull() )
	{
		QDomElement e = n.toElement();
		if( !e.isNull() )
		{
			if( e.tagName() == DOM_LANGUAGE_COMMAND_CATEGORY )
			{
				mVisitor->categoryBegin( QVariant( e.attribute( DOM_LANGUAGE_COMMAND_CATEGORY_NAME, "" ) ).toString() );
				readNode( &e );
				mVisitor->categoryEnd();
			}
			else if( e.tagName() == DOM_LANGUAGE_COMMAND )
			{
				QString gmnCommandName =	QVariant( e.attribute( DOM_LANGUAGE_COMMAND_NAME, "" ) ).toString();
				QString gmnCommand =		e.firstChild().toCDATASection().data();
				int gmnInsertIndex =		QVariant( e.attribute( DOM_LANGUAGE_COMMAND_INSERT_INDEX, "0" ) ).toInt();
				mVisitor->newElement( gmnCommandName , gmnCommand , gmnInsertIndex );
			}
			else
			{
				qWarning( "QLanguageCommandReader::QLanguageCommandReader : Unknown tag : %s" , e.tagName().toUtf8().data() );
			}
		}
		n = n.nextSibling();
	}
}
