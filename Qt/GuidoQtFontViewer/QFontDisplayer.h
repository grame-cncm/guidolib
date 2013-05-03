/*
 * QFontDisplayer.h
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
 #include <QWidget>
 #include <QString>

#ifdef WITH_GRAPHICS_SCENE
 #include <QGraphicsView>
 #include <QGraphicsScene>
class QFontDisplayer : public QGraphicsView
#else
class QFontDisplayer : public QWidget
#endif
{

public:
    
	QFontDisplayer(const QString& policeFamily , int nbOfColumns , int nbOfChars , QWidget *parent = 0);
	~QFontDisplayer();
	
protected:

#ifdef WITH_GRAPHICS_SCENE
	void showPolice();
	QGraphicsScene * mScene;
#else
	void paintEvent( QPaintEvent * event );
#endif

	QString mPoliceFamily;
	int mNbOfColumns;
	int mNbOfChars;	
};