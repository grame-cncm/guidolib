/*
 * QFontDisplayer.h
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