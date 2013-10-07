/*
 * main.cpp
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
