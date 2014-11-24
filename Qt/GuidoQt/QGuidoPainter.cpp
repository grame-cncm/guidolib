/*
 * QGuidoPainter.cpp
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

#ifdef WIN32
#pragma warning (disable: 4996)
#endif

#include <sstream>
#include <iostream>

#include "QGuidoPainter.h"
#include "GUIDOEngine.h"
#include "GUIDOParse.h"
#include "GUIDOPianoRoll.h"
#include "VGColor.h"

#include <QVariant>
#include <QDir>
#include <QFile>
#include <QTime>

#include <QtDebug>

#include "GSystemQt.h"
#include "GDeviceQt.h"


#define DEFAULT_DRAW_SIZE 100
#define MAX(a,b) ( a > b ) ? a : b
#define MIN(a,b) ( a < b ) ? a : b

GSystemQt * QGuidoPainter::mSys = 0;
VGDevice * QGuidoPainter::mDev  = 0;

//-------------------------------------------------------------------------
//								Static functions						///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
void QGuidoPainter::startGuidoEngine()
{
	if ( !QGuidoPainter::isGuidoEngineStarted() )
	{
		QGuidoPainter::mSys = new GSystemQt(0);
		QGuidoPainter::mDev = mSys->CreateDisplayDevice();
		
		GuidoInitDesc gd = { QGuidoPainter::mDev , 0, "Guido2", "Times New Roman" };
		GuidoInit (&gd);             // Initialise the Guido Engine first
	}
}

//-------------------------------------------------------------------------
QGuidoPainter *	QGuidoPainter::createGuidoPainter()
{
	if ( QGuidoPainter::isGuidoEngineStarted() )
	{
		return new QGuidoPainter;
	}
	else
		return 0;
}

//-------------------------------------------------------------------------
void QGuidoPainter::destroyGuidoPainter(QGuidoPainter * painter)
{
	if (painter) 
		delete painter;
}

//-------------------------------------------------------------------------
void QGuidoPainter::stopGuidoEngine()
{
	if ( QGuidoPainter::isGuidoEngineStarted() )
	{
		delete QGuidoPainter::mSys;
		delete QGuidoPainter::mDev;
		QGuidoPainter::mSys = 0;
		QGuidoPainter::mDev = 0;
	}
}

//-------------------------------------------------------------------------
bool QGuidoPainter::isGuidoEngineStarted()
{
	return ( QGuidoPainter::mSys != 0 );
}

//-------------------------------------------------------------------------
//							Non-Static functions						///
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
QGuidoPainter::QGuidoPainter()
{
	mDesc.hdc = 0;
	mDesc.page = 1;	
	mDesc.scrollx = 0;
	mDesc.scrolly = 0;
	mDesc.handle = 0;
	mARHandler = 0;
	mLastErr = guidoNoErr;
	mGMNCode = "";
	mFileName = "";
    fParser = GuidoOpenParser();

	GuidoGetDefaultLayoutSettings ( &mLayoutSettings );
//	mLayoutSettings.optimalPageFill = 0;
	GuidoGetDefaultPageFormat ( &mPageFormat );	
	fCurrentColor =	QColor(0,0,0);  // opaque black color by default
}
	
//-------------------------------------------------------------------------
QGuidoPainter::~QGuidoPainter()
{
	GuidoFreeGR( mDesc.handle );
	GuidoFreeAR( mARHandler );
    if (fParser)
        GuidoCloseParser(fParser);
}

//-------------------------------------------------------------------------
QString getFileContent(const QString& fileName)
{
	QString content = "";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		file.close();
		return "";
	}

    while (!file.atEnd()) 
	{
        QByteArray line = file.readLine();
        content += line;
    }
	file.close();
	return content;
}

//-------------------------------------------------------------------------
bool QGuidoPainter::setGMNFile(const QString& fileName)
{
	mFileName = fileName;
	QDir path(fileName);
	path.cdUp();
	return setGMNCode (getFileContent(fileName), path.absolutePath().toUtf8().data());
}

//-------------------------------------------------------------------------
bool QGuidoPainter::setGMNCode( const QString& gmnCode, const char* datapath )
{
	mGMNCode = gmnCode;
	return setGMNData( gmnCode , datapath );
}

//-------------------------------------------------------------------------
bool QGuidoPainter::setGMNStream(GuidoStream * gmnStream)
{
    mGMNStream = gmnStream;
    return setGMNDataStream(gmnStream);
}

//-------------------------------------------------------------------------
void QGuidoPainter::setARHandler( ARHandler ar )
{
	if (!ar) return;
	if (mDesc.handle)	GuidoFreeGR( mDesc.handle );
	if (mARHandler)		GuidoFreeAR( mARHandler );

	// Build a new score Graphic Representation according the score's Abstract Representation.
	mARHandler = ar;
	mGMNCode = "";
	GuidoPageFormat currentFormat;
	GuidoGetDefaultPageFormat ( &currentFormat );
	GuidoSetDefaultPageFormat( &mPageFormat );
	convertAR2GR (mARHandler, &mLayoutSettings , &mDesc.handle);
	GuidoSetDefaultPageFormat( &currentFormat );
    
    if (mLayoutSettings.resizePage2Music)
		mLastErr = GuidoResizePageToMusic( mDesc.handle );
}

//-------------------------------------------------------------------------
bool QGuidoPainter::setGMNDataStream (GuidoStream * guidoStream)
{
    // Read the gmnStream and build the score's Abstract Representation,
	// containing all the notes, rests, staffs, lyrics ...
	ARHandler arh;
	GRHandler grh;

    arh = GuidoStream2AR(fParser, guidoStream);

    if (!arh)
        return false;
    
	// Build a new score Graphic Representation according the score's Abstract Representation.
	GuidoPageFormat currentFormat;
	GuidoGetDefaultPageFormat ( &currentFormat );
	GuidoSetDefaultPageFormat( &mPageFormat );

	mLastErr = convertAR2GR(arh, &mLayoutSettings , &grh);

	GuidoSetDefaultPageFormat( &currentFormat );
	if (mLastErr == guidoNoErr)
	{
		GuidoFreeGR( mDesc.handle );
		mDesc.handle = grh;
		GuidoFreeAR( mARHandler );
		mARHandler = arh;

        if ( mLayoutSettings.resizePage2Music )
			mLastErr = GuidoResizePageToMusic( mDesc.handle );
	}
	return (mLastErr == guidoNoErr);

}

//-------------------------------------------------------------------------
bool QGuidoPainter::setGMNData( const QString& gmncode, const char* dataPath)
{
	// Read the gmnCode and build the score's Abstract Representation,
	// containing all the notes, rests, staffs, lyrics ...
	ARHandler arh;
	GRHandler grh;

    arh = GuidoString2AR(fParser, gmncode.toUtf8().data());

    if (!arh)
        return false;

	setPathsToARHandler(arh, dataPath);

	// Build a new score Graphic Representation according the score's Abstract Representation.
	GuidoPageFormat currentFormat;
	GuidoGetDefaultPageFormat ( &currentFormat );
	GuidoSetDefaultPageFormat( &mPageFormat );

    mLastErr = convertAR2GR(arh, &mLayoutSettings, &grh);

	GuidoSetDefaultPageFormat( &currentFormat );
	if (mLastErr == guidoNoErr)
	{
		GuidoFreeGR( mDesc.handle );
		mDesc.handle = grh;
		GuidoFreeAR( mARHandler );
		mARHandler = arh;
        if ( mLayoutSettings.resizePage2Music )
			mLastErr = GuidoResizePageToMusic( mDesc.handle );
	}
	return (mLastErr == guidoNoErr);
}

GuidoErrCode QGuidoPainter::convertAR2GR(ARHandler ar, const GuidoLayoutSettings* settings, GRHandler* gr) {
	GuidoErrCode errCode = GuidoAR2GR(ar, settings , gr);

    return errCode;
}

//-------------------------------------------------------------------------
bool QGuidoPainter::isGMNValid() const
{
	return (mLastErr == guidoNoErr);
}

//-------------------------------------------------------------------------
int QGuidoPainter::pageCount() const
{
	if ( hasValidGR() )
		return GuidoGetPageCount(mDesc.handle);
	else
		return 1;
}

//-------------------------------------------------------------------------
void QGuidoPainter::draw( QPainter * painter , int page , const QRect& drawRectangle , const QRect& redrawRectangle)
{
	if ( !hasValidGR() )
		return;
	
	painter->save();
	painter->setClipRect( drawRectangle );
	painter->translate( drawRectangle.x() , drawRectangle.y() );
	
	//Creation of temporaries Qt implementations of VGSystem & VGDevice.
	VGSystem * sys = new GSystemQt( painter );
	VGDevice * dev = sys->CreateDisplayDevice();
	
	//Update the mDesc with the specified page and draw dimensions.
	mDesc.hdc = dev;
	page = MAX(1 , page);
	page = MIN(pageCount() , page);
	mDesc.page = page;	

	mDesc.sizex = drawRectangle.width();
	mDesc.sizey = drawRectangle.height();

	//mDesc.scrollx = -drawRectangle.x();
	//mDesc.scrolly = -drawRectangle.y();
	
	if ( redrawRectangle.isNull() )
	{
		//Redraw everything
		mDesc.updateRegion.erase = true;
	}
	else
	{
		//1. Computes the actual drawing rectangle 
		//(because the Guido Score won't strech and will keep its height/width ratio,
		//the drawing rectangle is different from the QPainter's QPaintDevice rectangle.).
		float ratio = heightForWidth(1000,page) / 1000.0f;
		//This ratio means that:  height = ratio * width.
		bool drawRectTooHigh = ( mDesc.sizey >= (mDesc.sizex * ratio) );
		int actualWidth, actualHeight;
		if ( drawRectTooHigh )
		{
			actualWidth = mDesc.sizex;
			actualHeight = actualWidth * ratio;
		}
		else
		{
			actualHeight = mDesc.sizey;
			actualWidth = actualHeight / ratio;
		}
		//2. Conversion of the redrawRectangle from QPaintDevice coordinate space to GuidoVirtualUnit.
		GuidoPageFormat format;
		GuidoGetPageFormat( mDesc.handle , page , &format );
		float widthConversionFactor = actualWidth / format.width;
		float heightConversionFactor = actualHeight / format.height;
		// pixel / conversionFactor = GuidoVirtualUnit
		mDesc.updateRegion.left = (redrawRectangle.x() - drawRectangle.x()) / widthConversionFactor;
		mDesc.updateRegion.top  = (redrawRectangle.y() - drawRectangle.y()) / heightConversionFactor;
		mDesc.updateRegion.right =  ( (redrawRectangle.x() - drawRectangle.x()) + redrawRectangle.width()  )  / widthConversionFactor;
		mDesc.updateRegion.bottom = ( (redrawRectangle.y() - drawRectangle.y()) + redrawRectangle.height() )  / heightConversionFactor;
		mDesc.updateRegion.erase	= false;
	}

//	QTime time;
//	time.start();

	//Actual draw of the Guido Score.
	VGColor color(fCurrentColor.red(), fCurrentColor.green(), fCurrentColor.blue(), fCurrentColor.alpha());
	dev->SelectPenColor (color);
	dev->SelectFillColor(color);
	dev->SetFontColor	(color);

#if absoluteTransform1 || absoluteTransform2
	// DF Apr. 28 2011
	// rescaling introduced to take account of the QTDevice::SetScale change
 	// the QTDevice::SetScale change corresponds to the common VGDevice semantic and implementation
	// actually commented out due to unresolved problems with rotations
	qreal xs, ys;
	QPainter * p = (QPainter*)dev->GetNativeContext();
	p->worldTransform().map(qreal(mDesc.sizex), qreal(mDesc.sizey), &xs, &ys);
	mDesc.sizex = xs;
	mDesc.sizey = ys;
#endif
	GuidoOnDraw (&mDesc);
	
//	qDebug("Score : width = %d , height = %d" , mDesc.sizex , mDesc.sizey );
//	qDebug("QGuidoPainter: Draw time : %d ms" , time.elapsed() );
	
	delete dev;
	delete sys;
	
	painter->restore();
}

//-------------------------------------------------------------------------
void QGuidoPainter::drawPianoRoll(QPainter *painter, const QRect& drawRectangle, PianoRoll *pianoRoll)
{
	painter->save();
	painter->setClipRect(drawRectangle);
    painter->translate(drawRectangle.x(), drawRectangle.y());
	
	//Creation of temporaries Qt implementations of VGSystem & VGDevice.
	VGSystem * sys = new GSystemQt(painter);
	VGDevice * dev = sys->CreateDisplayDevice();

    GuidoPianoRollOnDraw(pianoRoll, drawRectangle.width(), drawRectangle.height(), dev);
	
	delete dev;
	delete sys;
	
	painter->restore();
}

//-------------------------------------------------------------------------
int QGuidoPainter::heightForWidth ( int w , int page ) const
{
	if ( !hasValidGR() )
		return w;
		
	GuidoPageFormat format;
	GuidoGetPageFormat( mDesc.handle , page , &format );
	float ratio = format.width / format.height;
	
	return int( w / ratio );
}

//-------------------------------------------------------------------------
QSizeF QGuidoPainter::pageSizeMM(int page) const
{
	if ( !hasValidGR() )
		return QSizeF(0,0);

	GuidoPageFormat format;
	GuidoGetPageFormat( mDesc.handle , page , &format );
	float widthMM = GuidoUnit2CM( format.width ) * 10.0f;	
	float heightMM = GuidoUnit2CM( format.height ) * 10.0f;	
	
	return QSizeF( widthMM , heightMM );
}

//-------------------------------------------------------------------------
QString QGuidoPainter::gmnCode() const
{
	return mGMNCode;
}

//-------------------------------------------------------------------------
QString QGuidoPainter::getLastErrorMessage() const
{
	QString result = QString( GuidoGetErrorString(mLastErr) );
	if ( mLastErr == guidoErrParse )
	{
		int line;
        int col;
        GuidoParserGetErrorCode(fParser, line, col, 0);
		result += " (line " + QVariant(line).toString() + ", col " + QVariant(col).toString() + ")";
	}
	return result;
}

//-------------------------------------------------------------------------
void QGuidoPainter::getLastParseErrorLine(int &line, int &col) const
{
    GuidoParserGetErrorCode(fParser, line, col, 0);
}
		
//-------------------------------------------------------------------------
const QString& QGuidoPainter::fileName() const
{
	return mFileName;
}

//-------------------------------------------------------------------------
void QGuidoPainter::setGuidoLayoutSettings(const GuidoLayoutSettings& layoutSettings)
{
	mLayoutSettings = layoutSettings;

	if (hasValidGR()) {
		GuidoUpdateGR(mDesc.handle , &mLayoutSettings);

        if (mLayoutSettings.resizePage2Music)
			GuidoResizePageToMusic(mDesc.handle);
	}
}

//-------------------------------------------------------------------------
GuidoLayoutSettings QGuidoPainter::guidoLayoutSettings() const
{
	return mLayoutSettings;
}

//-------------------------------------------------------------------------
void QGuidoPainter::setSystemsDistance(float distance)
{
	GuidoLayoutSettings settings = mLayoutSettings;
	if (distance != settings.systemsDistance) {
		settings.systemsDistance = (distance > kMinSystemDistance ? distance : float(kMinSystemDistance) );
		setGuidoLayoutSettings (settings);
	}
}

//-------------------------------------------------------------------------
float QGuidoPainter::getSystemsDistance() const
{
	return mLayoutSettings.systemsDistance;
}

//-------------------------------------------------------------------------
void QGuidoPainter::setResizePageToMusic(bool isOn)
{
    int booleanInt = (isOn ? 1 : 0);

	if (mLayoutSettings.resizePage2Music != booleanInt) {
		mLayoutSettings.resizePage2Music = booleanInt;
		setGMNCode(gmnCode());
	}
}

//-------------------------------------------------------------------------
void QGuidoPainter::setGuidoPageFormat(const GuidoPageFormat& pageFormat)
{
	mPageFormat = pageFormat;
	setGMNCode( gmnCode() );
}

//-------------------------------------------------------------------------
GuidoPageFormat QGuidoPainter::guidoPageFormat() const
{
	return mPageFormat;
}

//-------------------------------------------------------------------------
// associates a set of default paths to a handler
// note that the user home directory is always added as default path
void QGuidoPainter::setPathsToARHandler(ARHandler inARHandler, const char* path)
{
    std::vector<std::string> pathsVector;
	if (path) pathsVector.push_back (path);
	
	std::string homePath;
#ifdef WIN32
	// For windows
	homePath.append(getenv("HOMEDRIVE"));
	homePath.append(getenv("HOMEPATH"));
#else
	// For unix
	homePath.append(getenv("HOME"));
#endif
	pathsVector.push_back (homePath);
	GuidoSetSymbolPath(inARHandler, pathsVector);
}

//-------------------------------------------------------------------------
void QGuidoPainter::setScoreColor(const QColor& color)	{ fCurrentColor = color; }
const QColor& QGuidoPainter::getScoreColor() const		{ return fCurrentColor; }

