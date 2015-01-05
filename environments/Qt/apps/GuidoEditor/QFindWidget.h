/*
 * MainWindow.h
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
#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include "ui_findwidget.h"
#include <QWidget>
#include <QTextDocument>
#include <QPlainTextEdit>
#include <QSettings>

//-------------------------------------------------------------------------
class QFindWidget : public QWidget, private Ui::FindWidget
{
	Q_OBJECT

	QPlainTextEdit * fTextEdit;
	QTextDocument* fDocument;
	bool fBackward, fCaseSensitive, fWholeWords;

	public:
		QFindWidget(QPlainTextEdit * edit , QWidget *parent = 0);
		void readSettings(QSettings& settings);
		void writeSettings(QSettings& settings);
		void resetSettings();
		
	public slots:
		void find_slot();
		void findNext_slot();
		
	protected:
		bool find();
		bool findNext();
		bool findFirst();
		bool findPrevious();
		bool findLast();
		bool replaceNext();
		void doReplace();
		bool findFromHere();
		
		QTextCursor documentStart() const { QTextCursor c=fTextEdit->textCursor(); c.clearSelection(); c.movePosition(QTextCursor::Start); return c; }
		QTextCursor documentEnd() const { QTextCursor c=fTextEdit->textCursor(); c.clearSelection(); c.movePosition(QTextCursor::End); return c; }
		QTextDocument::FindFlags flags() const;
	
	protected slots:
		void findNext_slot(const QString&);
		void findFirst_slot();
		void find_slot(const QString&);
		void findFirst_slot(const QString&);
		void findPrevious_slot();
		void replaceNext_slot();
		void replacePrevious_slot();
		void replaceAll_slot();
		void caseChanged(int state);
		void wholeWordsChanged(int state);
		
};

#endif
