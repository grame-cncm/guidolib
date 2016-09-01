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
#include <QPainter>
#include <QImage>

#include "QGuidoPainter.h"

int main(int argc , char* argv[])
{
	QApplication app( argc , argv );

	//Guido Engine initialization
	QGuidoPainter::startGuidoEngine();

	//Create a QGuidoPainter...
	QGuidoPainter * p = QGuidoPainter::createGuidoPainter();
	//...and give it some Guido Music Notation
	p->setGMNCode( "[c/8 d e g e d c]" );

	//Get the size of the score's first and only page
	int pageIndex = 1;
	QSizeF s = p->pageSizeMM( pageIndex );

	//Create a blank image, using the size of the score
	QImage image( s.toSize() * 10 , 
				  QImage::Format_ARGB32 );
	image.fill( QColor(Qt::white).rgb() );
	
	//Draw on the QImage with the QGuidoPainter, via a QPainter
	QPainter painter( &image );
	p->draw( &painter , pageIndex , image.rect() );
	
	QGuidoPainter::destroyGuidoPainter( p );

	//Destroy the Guido Engine resources 
	QGuidoPainter::stopGuidoEngine();

	//Save the score
	image.save( "myScore.png" );

	return 0;
}
