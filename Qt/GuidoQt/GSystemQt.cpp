/*
 * GSystemQt.cpp
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
VGDevice*		GSystemQt::CreateMemoryDevice( const char * )
{
	assert(0);	//Not implemented.
	return 0;
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
