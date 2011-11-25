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
