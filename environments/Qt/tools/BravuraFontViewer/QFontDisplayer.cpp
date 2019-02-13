/*
 * QFontDisplayer.cpp
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
#include <QFont>
#include <QPen>
#include <QBrush>
#include <QVariant>

//#define PRINTEXTEND
#ifdef PRINTEXTEND
#include <fstream>
std::ofstream bravuraout("bravura.txt");
std::ofstream timesout("times.txt");
# ifdef WIN32
#  define OS "Win32"
# else
#  define OS "MacOS X"
# endif
using namespace std;
#endif

#ifdef WITH_GRAPHICS_SCENE
	#include <QGraphicsItem>
	#include <QGraphicsTextItem>
	#include <QGraphicsScene>
	#include <QGraphicsView>
#else
	#include <QPainter>
#endif

#include <QKeyEvent>

#include "QFontDisplayer.h"

#define STARTGLYPH 57344


QFontDisplayer::QFontDisplayer(const QString& policeFamily , int nbOfColumns , int nbOfChars , QWidget* )
    : mPoliceFamily(policeFamily), mNbOfColumns(nbOfColumns) , mNbOfChars(nbOfChars)
{
}

//--------------------------------------------------
void QFontDisplayer::printMetrics(QPainter& painter )
{
#ifdef PRINTEXTEND
	static bool done = false;
	if (!done) {
		painter.save();
		painter.scale( 1/painter.worldTransform().m11(), 1/painter.worldTransform().m22());
		const float LSPACE = 50;
		QFont musicFont	= QFont("Bravura" , 4*LSPACE);
		QFontMetrics *metrics = new QFontMetrics(musicFont);
		bravuraout << OS << " - Qt - Bravura font - size: " << 4*LSPACE << endl;
		for (unsigned char i=STARTGLYPH; i<STARTGLYPH+mNbOfChars; i++) {
			bravuraout << "0x" << QString::number( i, 16 ).toUpper() << ": " << metrics->width( QChar(i) ) << endl;
		}
		delete metrics;

		QFont timesFont	= QFont("Times" , 1.5*LSPACE);
		metrics = new QFontMetrics(timesFont);
		timesout << OS << " - Qt - Time font - size: " << 1.5*LSPACE << endl;
		for (unsigned char i=32; i<255; i++) {
			timesout << int(i) << ": " << metrics->width( QChar(i) ) << endl;
		}
		delete metrics;
		painter.restore();
		done = true;
	}
#endif
}

//--------------------------------------------------	
void QFontDisplayer::paintEvent( QPaintEvent * event )
{
	event->accept();

	QPainter painter(this);
	painter.fillRect( 0 , 0 , width() , height() , QBrush(Qt::white) );

#ifdef PRINTEXTEND
	printMetrics( painter );
#endif

	int cellSize = width() / (mNbOfColumns + 3);

	int policeSize = (cellSize / 6) * 3;
	QFont font( mPoliceFamily );
	font.setPixelSize( policeSize );
	QFontMetrics metric (font);
	
	QString textPoliceFamily = "Arial";
	int textPoliceSize = (cellSize / 6) * 3;
	int miniTextSize = (cellSize / 6);
	QFont textFont		= QFont(textPoliceFamily , textPoliceSize);
	textFont.setPixelSize( textPoliceSize );
	QFont miniTextFont	= QFont(textPoliceFamily , miniTextSize );
	miniTextFont.setPixelSize( miniTextSize );
	
	int CELL_WIDTH = cellSize;
	int CELL_HEIGHT = cellSize;
	
	int nbOfRows = mNbOfChars / mNbOfColumns + 1;
	resize( width() , (nbOfRows + 1) * CELL_HEIGHT );

	int xOffset = cellSize / 4;
	int yOffset = cellSize / 4;
	QPen lightPen( Qt::red );
	lightPen.setWidth( 0.5f );
	lightPen.setStyle( Qt::DotLine );

	QPen pen;
	pen.setWidth(1.5);
	for (int i = 0 ; i < mNbOfColumns ; i++ )
	{
		int x = (i+2) * CELL_WIDTH;
		int y = 0;
		painter.setPen( pen );
		painter.setFont( textFont );
		painter.drawText( x+10 , y -10 + CELL_HEIGHT , QVariant(i).toString() );
		painter.drawRect( x , y , CELL_WIDTH , CELL_HEIGHT );
	}
	
	for (int j = 0 ; j < nbOfRows ; j++ )
	{
		int x = 0;
		int y = (j+1) * CELL_HEIGHT;
		painter.drawText( x+10 , y -10 + CELL_HEIGHT , QVariant(j * mNbOfColumns).toString() );
		painter.drawRect( x , y , 2 * CELL_WIDTH , CELL_HEIGHT );
	}
	
	QPen bbpen (Qt::blue);
	bbpen.setWidth(1.5);
	unsigned int glyph = STARTGLYPH;
	for (int j = 0 ; j < nbOfRows ; j++ )
	{
		for (int i = 0 ; i < mNbOfColumns ; i++ )
		{	
			int x = (i+2) * CELL_WIDTH;
			int y = (j+1) * CELL_HEIGHT;
		
			QString symbol = QString( QChar( glyph ) );

			QPen redPen(Qt::red );
			painter.setPen( redPen );
			painter.setFont( miniTextFont );
			painter.drawText( x , y + QFontMetrics(miniTextFont).height() , QString::number( glyph, 16 ).toUpper() );
			painter.setPen( lightPen );
			painter.drawLine( x + xOffset , y , x + xOffset , y + CELL_HEIGHT );
			painter.drawLine( x , y + CELL_HEIGHT - yOffset , x + CELL_WIDTH , y + CELL_HEIGHT - yOffset );
			painter.setPen( pen );
			painter.setFont( font );
			painter.drawText( x + xOffset , y + CELL_HEIGHT - yOffset , symbol );
			painter.drawRect( x , y , CELL_WIDTH , CELL_HEIGHT );
//			painter.setPen( bbpen );
//			painter.drawRect( x + xOffset , y +10, metric.width(symbol) , CELL_HEIGHT-20 );
			glyph++;
		}
	}

}
