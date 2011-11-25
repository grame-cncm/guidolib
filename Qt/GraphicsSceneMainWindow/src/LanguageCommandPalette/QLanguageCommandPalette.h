/*
 * QLanguageCommandPalette.h
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
#ifndef LANGUAGE_COMMAND_PALETTE_H
#define LANGUAGE_COMMAND_PALETTE_H

#include <QStack>
#include <QWidget>

#include "QLanguageCommandReader.h"
class QTabWidget;

class QLanguageCommandPalette : public QWidget , public QLanguageCommandVisitor
{
	Q_OBJECT

	public :

		QLanguageCommandPalette(const QString& commandsFile , QWidget * parent = 0);

		void categoryBegin( const QString& name );
		void categoryEnd();
		void newElement( const QString& name , const QString& value , int insertIndex );

	Q_SIGNALS:

		void insertCode(const QString& gmnCode , int insertIndex );

	protected Q_SLOTS:

		void elementClick();

	protected : 

		QStack<QString> mCategories;
		QString mIndent;

		QStack<QTabWidget*> mTabStack;
		QList<QWidget *> mCurrentWidgetList;
};

#include <QToolButton>
class QValuePushButton : public QToolButton
{
	Q_OBJECT

	public:
	
		QValuePushButton(const QString& name , const QString& value , int insertIndex , QWidget * parent = 0);
		
		int		insertIndex() const;
		QString	value() const;

	protected:
	
		QString mValue;
		int mInsertIndex;

};

#endif //GUIDO_COMMAND_PALETTE_H
