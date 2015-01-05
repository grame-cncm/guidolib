/*
 * QLanguageCommandPalette.cpp
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
#include "QLanguageCommandPalette.h"

#include "GraphicsSceneMainWindow.h"

#include <QPushButton>
#include <QTabWidget>
#include <QHBoxLayout>

#define INDENT					QString("      ")

//------------------------------------------------------------------------------------
QLanguageCommandPalette::QLanguageCommandPalette(const QString& commandsFile , QWidget * parent) :
	QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);

	mTabStack << new QTabWidget(this);
	layout->addWidget( mTabStack.top() );

	setLayout(layout);
	
	QLanguageCommandReader reader;
	reader.read( this , commandsFile );
}

#include "QLanguageCommandPaletteLayout.h"
//------------------------------------------------------------------------------------
void QLanguageCommandPalette::categoryBegin( const QString& name )
{
	//Add new tab to mCurrentTab. The new tab has the name of the category.
	//The new tab is another QTabWidget.
	//
	QTabWidget * newTab = new QTabWidget( this );
	newTab->setUsesScrollButtons ( true );
	
	QLanguageCommandPaletteLayout * layout = new QLanguageCommandPaletteLayout( newTab );
	newTab->setLayout( layout );
	
	mTabStack.top()->addTab( newTab , name );
	mTabStack.top()->setTabToolTip( mTabStack.top()->count()-1 , name );
	mTabStack << newTab;

	mCategories.push( name );
	mIndent += INDENT;
	
	setMinimumSize( QSize(200,150) );
}

//------------------------------------------------------------------------------------
void QLanguageCommandPalette::categoryEnd()
{
	mIndent = mIndent.left( mIndent.length() - INDENT.length() );

	mCategories.pop();
	
	mTabStack.pop();
}

#include <QtDebug>
//------------------------------------------------------------------------------------
void QLanguageCommandPalette::newElement( const QString& name , const QString& value , int insertIndex )
{
	QValuePushButton * button = new QValuePushButton( name , value , insertIndex , this );

	button->setMaximumSize( QSize(10000,10000) );
	connect( button , SIGNAL(clicked()) , this , SLOT(elementClick()) );
	mTabStack.top()->layout()->addWidget( button );
}

//------------------------------------------------------------------------------------
void QLanguageCommandPalette::elementClick()
{
	QValuePushButton * button = dynamic_cast<QValuePushButton *> (sender());
	Q_EMIT insertCode(button->value(),button->insertIndex());
}

//------------------------------------------------------------------------------------
//						QValuePushButton											//
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
QValuePushButton::QValuePushButton(const QString& name , const QString& value , int insertIndex , QWidget * parent)
	:	QToolButton( parent ),
		mValue(value) ,
		mInsertIndex( insertIndex )
{
	setText( name );
	setToolTip( mValue );
}

//------------------------------------------------------------------------------------
int	QValuePushButton::insertIndex() const
{
	return mInsertIndex;
}

//------------------------------------------------------------------------------------
QString QValuePushButton::value() const
{
	return mValue;
}
