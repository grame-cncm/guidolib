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
std::ofstream guidoout("guido2.txt");
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


#ifdef WITH_GRAPHICS_SCENE
QFontDisplayer::QFontDisplayer(const QString& policeFamily , int nbOfColumns , int nbOfChars , QWidget *parent)
    : QGraphicsView(parent) ,   mPoliceFamily(policeFamily) ,
	mNbOfColumns(nbOfColumns) , mNbOfChars(nbOfChars)
{
	
	mScene = new QGraphicsScene();
	setScene(mScene);
	showPolice();
}
#else
QFontDisplayer::QFontDisplayer(const QString& policeFamily , int nbOfColumns , int nbOfChars , QWidget* )
    : mPoliceFamily(policeFamily) ,
	mNbOfColumns(nbOfColumns) , mNbOfChars(nbOfChars)
{
}
#endif

//--------------------------------------------------
QFontDisplayer::~QFontDisplayer()
{
#ifdef WITH_GRAPHICS_SCENE
	delete mScene;
#endif
}

//--------------------------------------------------	
#ifdef WITH_GRAPHICS_SCENE
void QFontDisplayer::showPolice()
{
#else
void QFontDisplayer::paintEvent( QPaintEvent * event )
{
	event->accept();

	QPainter painter(this);
	painter.fillRect( 0 , 0 , width() , height() , QBrush(Qt::white) );

#endif

#ifdef PRINTEXTEND
	static bool done = false;
	if (!done) {
		painter.save();
		painter.scale( 1/painter.worldTransform().m11(), 1/painter.worldTransform().m22());
		const float LSPACE = 50;
		QFont guidoFont	= QFont("Guido2" , 4*LSPACE);
		QFontMetrics *metrics = new QFontMetrics(guidoFont);	
		guidoout << OS << " - Qt - Guido2 font - size: " << 4*LSPACE << endl; 
		for (unsigned char i=32; i<255; i++) {
			guidoout << int(i) << ": " << metrics->width( QChar(i) ) << endl; 
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

	int cellSize = width() / (mNbOfColumns + 3);

	int policeSize = (cellSize / 6) * 3;
	QFont font( mPoliceFamily );
	font.setPixelSize( policeSize );
	
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
#ifndef WITH_GRAPHICS_SCENE
	resize( width() , (nbOfRows + 1) * CELL_HEIGHT );

	int xOffset = cellSize / 4;
	int yOffset = cellSize / 4;
	QPen lightPen( Qt::red );
	lightPen.setWidth( 0.5f );
	lightPen.setStyle( Qt::DotLine );
#endif	

	QPen pen;
	pen.setWidth(2);
	for (int i = 0 ; i < mNbOfColumns ; i++ )
	{
		int x = (i+2) * CELL_WIDTH;
		int y = 0;
#ifdef WITH_GRAPHICS_SCENE
		QGraphicsItem * t = mScene->addText(QVariant(i).toString() , textFont );
		t->translate( x , y );		
		mScene->addRect( x , y , CELL_WIDTH , CELL_HEIGHT , pen );
#else
		painter.setPen( pen );
		painter.setFont( textFont );
		painter.drawText( x , y + CELL_HEIGHT , QVariant(i).toString() );
		painter.drawRect( x , y , CELL_WIDTH , CELL_HEIGHT );
#endif
	}
	
	for (int j = 0 ; j < nbOfRows ; j++ )
	{
		int x = 0;
		int y = (j+1) * CELL_HEIGHT;
#ifdef WITH_GRAPHICS_SCENE
		QGraphicsItem * t = mScene->addText(QVariant(j * mNbOfColumns).toString() , textFont);
		t->translate( x , y );
		mScene->addRect( x , y , 2 * CELL_WIDTH , CELL_HEIGHT , pen );
#else
		painter.drawText( x , y + CELL_HEIGHT , QVariant(j * mNbOfColumns).toString() );
		painter.drawRect( x , y , 2 * CELL_WIDTH , CELL_HEIGHT );
#endif
	}
	
	bool quit = false;
	unsigned int n = STARTGLYPH;
	for (int j = 0 ; j < nbOfRows ; j++ )
	{
		if ( quit ) 
		{
			break;
		}
		for (int i = 0 ; i < mNbOfColumns ; i++ )
		{	
			int x = (i+2) * CELL_WIDTH;
			int y = (j+1) * CELL_HEIGHT;
		
//			QChar c = QChar::fromAscii( n );
			QChar c = QChar( n );
			QString symbol = QString( c );

#ifdef WITH_GRAPHICS_SCENE
			QGraphicsItem * text;			
			text = mScene->addText(QVariant(n).toString(),miniTextFont);
			text->translate(x,y);
			((QGraphicsTextItem*)text)->setDefaultTextColor(Qt::red);
			text = mScene->addText(symbol,font);
			text->translate(x,y);
			mScene->addRect( x , y , CELL_WIDTH , CELL_HEIGHT );
#else
			QPen redPen(Qt::red );
			painter.setPen( redPen );
			painter.setFont( miniTextFont );
			painter.drawText( x , y + QFontMetrics(miniTextFont).height() , QVariant(n).toString() );
			painter.setPen( lightPen );
			painter.drawLine( x + xOffset , y , x + xOffset , y + CELL_HEIGHT );
			painter.drawLine( x , y + CELL_HEIGHT - yOffset , x + CELL_WIDTH , y + CELL_HEIGHT - yOffset );
			painter.setPen( pen );
			painter.setFont( font );
			painter.drawText( x + xOffset , y + CELL_HEIGHT - yOffset , symbol );
			painter.drawRect( x , y , CELL_WIDTH , CELL_HEIGHT );
#endif
			n++;
			if ( int(n) == mNbOfChars )
			{
				quit = true;
				break;
			}
		}
	}

}
