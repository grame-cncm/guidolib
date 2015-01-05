/*
 * GSystemQt.cpp
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
#include "GDeviceQt.h"
#include "GFontQt.h"
#include "GSystemQt.h"

#include <assert.h>

#define GSYSTEMQT_GUIDO_FONT	"Guido2"
#define GSYSTEMQT_TEXT_FONT		"Arial"

//------------------------------------------------------------------------
GSystemQt::GSystemQt( QPainter* qPainter ) : mQPainter(qPainter) {}
GSystemQt::~GSystemQt()	{}

//------------------------------------------------------------------------
VGDevice * GSystemQt::CreateDisplayDevice()
{
	GDeviceQt * dispDevice = new GDeviceQt( mQPainter , this );
	
	//Set Device text font.
//	const GFontQt * dispFont = (const GFontQt*)(CreateVGFont( GSYSTEMQT_TEXT_FONT , 10, VGFont::kFontNone ));
//	dispDevice->SetTextFont(dispFont);
//
//	//Set Device music font.
//	const GFontQt * musicFont = (const GFontQt*)(CreateVGFont( GSYSTEMQT_GUIDO_FONT , 10, VGFont::kFontNone ));
//	dispDevice->SetMusicFont(musicFont);
//	
//	QFontDatabase database;
//	if ( !database.families().contains(GSYSTEMQT_GUIDO_FONT , Qt::CaseInsensitive) )
//		qDebug( "Warning : GSystemQt::CreateDisplayDevice() : %s font is not available" , GSYSTEMQT_GUIDO_FONT );

   	return dispDevice;	
}

//------------------------------------------------------------------------
VGDevice*		GSystemQt::CreateMemoryDevice( int, int )
{
	assert(0);	//Not implemented.
	return 0;
}

//------------------------------------------------------------------------
VGDevice*		GSystemQt::CreateMemoryDevice( const char * inPath)
{
    GDeviceQt *memDevice = 0;

#if 1
	// new implementation, more simple, more direct, only QImage expected
    QImage qImage(inPath);
	if (qImage.isNull()) {
		return 0;
	}

    QPainter * mQPainter = new QPainter(new QImage (qImage.convertToFormat (QImage::Format_ARGB32)));
	mQPainter->setRenderHints (QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	memDevice = new GDeviceQt(mQPainter, this);
	
#else
    QImage qImage(inPath);

    if (!qImage.isNull())
    {
        QPoint qPoint(0, 0);

        QSize qSize(qImage.width(),qImage.height());
        QPixmap *qPixmap = new QPixmap(qSize);
        QPainter *mQPainter = new QPainter(qPixmap);
        mQPainter->drawImage(qPoint, qImage);

        memDevice = new GDeviceQt(mQPainter, this);
    }
#endif
	return memDevice;
}

//------------------------------------------------------------------------
VGDevice*		GSystemQt::CreatePrinterDevice( )
{
	assert(0);	//Not implemented.
	return 0;
}

//------------------------------------------------------------------------
VGDevice*		GSystemQt::CreateAntiAliasedMemoryDevice( int , int )
{
	assert(0);	//Not implemented.
	return 0;
}

//------------------------------------------------------------------------
const VGFont*	GSystemQt::CreateVGFont( const char * faceName, int size, int properties )const 
{
  	int myWeight = QFont::Normal;
  	int myStyle	 = QFont::StyleNormal;
  	if(!faceName)
	{
  		return 0;
  	}
  	if (properties)
	{
		if (properties & VGFont::kFontBold)			myWeight = QFont::Bold; 		
		if (properties & VGFont::kFontItalic)		myStyle	 = QFont::StyleItalic;
	}
	// The QFont constructor asks for a size in points, but we'll set a size in pixels anyway, so, we give the constructor
	//	a "foo" value.
	int fooSize = 10;
	QFont * qtFont = new QFont(faceName, fooSize ,myWeight,(myStyle == QFont::StyleItalic)?1:0);
	// Set the real size in pixels.
	qtFont->setPixelSize( size );
	GFontQt * gFont = new GFontQt(qtFont,faceName,size,properties);
	return gFont;
}
