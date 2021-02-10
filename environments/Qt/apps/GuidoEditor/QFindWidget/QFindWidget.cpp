/*
 * MainWindow.cpp
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
 
 #include "QFindWidget.h"
 
#include <QtDebug>
#include <QStyle>

#define SETTINGS_GROUP "FindWidget"
#define FIND_EDIT_SETTING		"FindEdit"
#define REPLACE_EDIT_SETTING	"ReplaceEdit"
#define CASE_SENSITIVE_SETTING	"CaseSensitive"
#define WHOLE_WORDS_SETTING	"WholeWords"
#define FIND_EDIT_DEFAULT		""
#define REPLACE_EDIT_DEFAULT	""
#define CASE_SENSITIVE_DEFAULT	false
#define WHOLE_WORDS_DEFAULT	false
#define TOOL_BUTTON_ICON_SIZE QSize(12,12)
   
//-------------------------------------------------------------------------
QFindWidget::QFindWidget(QPlainTextEdit * edit , QWidget * parent)
	:	QWidget(parent), fTextEdit(edit), fDocument(edit->document()),
	fBackward(false), fCaseSensitive(CASE_SENSITIVE_DEFAULT), fWholeWords(WHOLE_WORDS_DEFAULT)
{
	setupUi(this);
	
	findEdit->setText( FIND_EDIT_DEFAULT );
	replaceEdit->setText( REPLACE_EDIT_DEFAULT );
	
	closeButton->setIcon( style()->standardIcon( QStyle::SP_TitleBarCloseButton ) );
	
	connect( findPreviousButton, SIGNAL(clicked()) , this, SLOT(findPrevious_slot()) );
	connect( findNextButton, SIGNAL(clicked()) , this, SLOT(findNext_slot()) );
	connect( replacePreviousButton, SIGNAL(clicked()) , this, SLOT(replacePrevious_slot()) );
	connect( replaceNextButton, SIGNAL(clicked()) , this, SLOT(replaceNext_slot()) );
	connect( replaceAllButton, SIGNAL(clicked()) , this, SLOT(replaceAll_slot()) );
	connect( closeButton, SIGNAL(clicked()) , this, SLOT(hide()) );
	
	connect( findEdit , SIGNAL(textChanged(const QString&)) , this, SLOT(find_slot(const QString&)) );
	connect( findEdit , SIGNAL(returnPressed()) , this, SLOT(findNext_slot()) );
	connect( replaceEdit , SIGNAL(returnPressed()) , this, SLOT(replaceNext_slot()) );

	connect( caseSensitiveBox , SIGNAL(stateChanged(int)) , this , SLOT(caseChanged(int)) );
	connect( wholeWordsBox , SIGNAL(stateChanged(int)) , this , SLOT(wholeWordsChanged(int)) );
	
	replacePreviousButton->setIconSize( TOOL_BUTTON_ICON_SIZE );
	replaceNextButton->setIconSize( TOOL_BUTTON_ICON_SIZE );
	findPreviousButton->setIconSize( TOOL_BUTTON_ICON_SIZE );
	findNextButton->setIconSize( TOOL_BUTTON_ICON_SIZE );
	closeButton->setIconSize( TOOL_BUTTON_ICON_SIZE );
}

//-------------------------------------------------------------------------
void QFindWidget::readSettings(QSettings& settings)
{
	settings.beginGroup( SETTINGS_GROUP );
	findEdit->setText( settings.value( FIND_EDIT_SETTING , FIND_EDIT_DEFAULT ).toString() );
	replaceEdit->setText( settings.value( REPLACE_EDIT_SETTING , REPLACE_EDIT_DEFAULT ).toString() );
	caseSensitiveBox->setChecked( settings.value( CASE_SENSITIVE_SETTING , CASE_SENSITIVE_DEFAULT ).toBool() );
	wholeWordsBox->setChecked( settings.value( WHOLE_WORDS_SETTING , WHOLE_WORDS_DEFAULT ).toBool() );
	settings.endGroup();
}

//-------------------------------------------------------------------------
void QFindWidget::writeSettings(QSettings& settings)
{
	settings.beginGroup( SETTINGS_GROUP );
	settings.setValue( FIND_EDIT_SETTING , findEdit->text() );
	settings.setValue( REPLACE_EDIT_SETTING , replaceEdit->text() );
	settings.setValue( CASE_SENSITIVE_SETTING , caseSensitiveBox->isChecked() );
	settings.setValue( WHOLE_WORDS_SETTING , wholeWordsBox->isChecked() );
	settings.endGroup();
}

//-------------------------------------------------------------------------
void QFindWidget::resetSettings()
{
	findEdit->setText( FIND_EDIT_DEFAULT );
	replaceEdit->setText( REPLACE_EDIT_DEFAULT );
	caseSensitiveBox->setChecked( CASE_SENSITIVE_DEFAULT );
	wholeWordsBox->setChecked( WHOLE_WORDS_DEFAULT );
}
		
//-------------------------------------------------------------------------
void QFindWidget::find_slot()
{
	show();
	findEdit->setFocus();
	findEdit->selectAll();
}

//-------------------------------------------------------------------------
bool QFindWidget::find()
{	
	QTextCursor oldCursor = fTextEdit->textCursor();
	QTextCursor cursor = fDocument->find( findEdit->text() , fTextEdit->textCursor().selectionStart() , flags() );
		
	if ( cursor.isNull() )
	{
		cursor = oldCursor;
		cursor.clearSelection();
	}
/*	
	QTextCharFormat format;
	format.setBackground( QBrush( Qt::green ) );
	cursor.setCharFormat(format);
*/
	
	fTextEdit->setTextCursor(cursor);
	if ( cursor.hasSelection() )
		fTextEdit->ensureCursorVisible();

	return cursor.hasSelection();
}

//-------------------------------------------------------------------------
void QFindWidget::find_slot(const QString&)
{
	findFromHere();
}

//-------------------------------------------------------------------------
bool QFindWidget::findFromHere()
{
	fBackward = false;
	return find();
}
	
//-------------------------------------------------------------------------
bool QFindWidget::findNext()
{
	QTextCursor c = fTextEdit->textCursor();
	c.clearSelection();
	fTextEdit->setTextCursor( c );

	fBackward = false;

	bool result = find();
	if ( !result && (fTextEdit->textCursor()!=documentStart()) )
		findFirst();

	return result;
}

//-------------------------------------------------------------------------
void QFindWidget::findNext_slot(const QString&)
{
	findNext();
}

//-------------------------------------------------------------------------
void QFindWidget::findNext_slot()
{
	findNext();
}

//-------------------------------------------------------------------------
void QFindWidget::findFirst_slot()
{
	findFirst();
}

//-------------------------------------------------------------------------
bool QFindWidget::findFirst()
{
	QTextCursor c = fTextEdit->textCursor();
	c.movePosition( QTextCursor::Start );
	fTextEdit->setTextCursor( c );
	return findNext();
}

//-------------------------------------------------------------------------
bool QFindWidget::findLast()
{
	QTextCursor c = fTextEdit->textCursor();
	c.movePosition( QTextCursor::End );
	fTextEdit->setTextCursor( c );

	return findPrevious();
}

//-------------------------------------------------------------------------
void QFindWidget::findFirst_slot(const QString&)
{
	findFirst();
}

//-------------------------------------------------------------------------
void QFindWidget::findPrevious_slot()
{
	findPrevious();
}

//-------------------------------------------------------------------------
bool QFindWidget::findPrevious()
{
	QTextCursor c = fTextEdit->textCursor();
	c.clearSelection();

	c.setPosition( fTextEdit->textCursor().selectionStart() );
	fTextEdit->setTextCursor( c );

	fBackward = true;

	bool result = find();
	if ( !result && (fTextEdit->textCursor()!=documentEnd()) )
		findLast();
		
	return result;
}

//-------------------------------------------------------------------------
void QFindWidget::replaceNext_slot()
{
	replaceNext();
}

//-------------------------------------------------------------------------
bool QFindWidget::replaceNext()
{
	QTextCursor c = fTextEdit->textCursor();
	c.beginEditBlock();
	doReplace();
	bool result = findNext();
	c.endEditBlock();
	return result;
}

//-------------------------------------------------------------------------
void QFindWidget::replacePrevious_slot()
{
	QTextCursor c = fTextEdit->textCursor();
	c.beginEditBlock();
	doReplace();
	findPrevious_slot();
	c.endEditBlock();
}

//-------------------------------------------------------------------------
void QFindWidget::doReplace()
{
	QTextCursor c = fTextEdit->textCursor();
	c.removeSelectedText();
	c.insertText( replaceEdit->text() );
}

//-------------------------------------------------------------------------
void QFindWidget::replaceAll_slot()
{
	QTextCursor c = fTextEdit->textCursor();
	c.beginEditBlock();
	
	if (findFirst())
	{
		while ( replaceNext() ) {}
	}
	
	c.endEditBlock();
}

//-------------------------------------------------------------------------
void QFindWidget::caseChanged(int state)
{
	fCaseSensitive = (state==Qt::Checked);
	findFromHere();
}

//-------------------------------------------------------------------------
void QFindWidget::wholeWordsChanged(int state)
{
	fWholeWords = (state==Qt::Checked);
	findFromHere();
}

//-------------------------------------------------------------------------
QTextDocument::FindFlags QFindWidget::flags() const
{
	QTextDocument::FindFlags result;
	if (  fBackward )
		result |= QTextDocument::FindBackward;
	if ( fCaseSensitive )
		result |= QTextDocument::FindCaseSensitively;
	if ( fWholeWords )
		result |= QTextDocument::FindWholeWords;
	return result;
}
