/*
 * main.cpp
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

#include <QApplication>
#include "QFontDisplayer.h"
#include <QVariant>
#include <QString>
#include <QScrollArea>

#define POLICE_NAME		"Guido2"
#define NB_OF_COLUMNS		10
#define NB_OF_CHARS_SHOWN	1000

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef WITH_GRAPHICS_SCENE
	QFontDisplayer fontDisplayer( POLICE_NAME , NB_OF_COLUMNS , NB_OF_CHARS_SHOWN );
	fontDisplayer.show();
#else	
	QFontDisplayer * fontDisplayer = new QFontDisplayer( POLICE_NAME , NB_OF_COLUMNS , NB_OF_CHARS_SHOWN );
	QScrollArea scrollArea;
	scrollArea.setWidget( fontDisplayer );
	scrollArea.setAlignment( Qt::AlignHCenter );
	scrollArea.resize( 800 , 600 );
	fontDisplayer->resize( 800 , 1 );
	scrollArea.show();
#endif
	
	return app.exec();
}
