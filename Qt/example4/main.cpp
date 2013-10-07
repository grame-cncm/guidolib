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
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

#include "QGuidoGraphicsItem.h"

int main(int argc , char* argv[])
{
	QApplication app( argc , argv );

	// Guido Engine initialization
	QGuidoPainter::startGuidoEngine();

	// Create a QGraphicsScene...
	QGraphicsScene scene;

	// Create a QGuidoGraphicsItem...
	QGuidoGraphicsItem graphic;
	//...and give it some Guido Music Notation
	graphic.setGMNCode( "[c/8 d e g e d c]" );
	scene.addItem (&graphic);

	QGraphicsView view(&scene);
	view.show();
	app.exec();

	//Destroy the Guido Engine resources
	QGuidoPainter::stopGuidoEngine();
	return 0;
}
