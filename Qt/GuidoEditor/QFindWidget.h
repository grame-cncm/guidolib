/*
 * MainWindow.h
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
