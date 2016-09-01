/*
 * QLanguageCommandPalette.h
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
