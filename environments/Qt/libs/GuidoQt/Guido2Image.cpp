/*
 * Guido2Image.cpp
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

#include "Guido2Image.h"

#include <iostream>
#include <fstream>
#include <string>

#include <QPainter>
#include <QPrinter>
#include <QImage>
#include <QRectF>
#include <QRect>
#include <QSize>
#include <QString>
#include <QFile>
#include <QDebug>

#include "QGuidoPainter.h"
#include "QGuidoGraphicsItem.h"
#include "GUIDOEngine.h"

#include <assert.h>

#define WRITE_ERROR(a,b,c) if ( ( a ) && ( c ) ) strncpy( a , b , c )
#define PAGES_ALIGNMENT QGuidoMPageItem::Horizontal

//----------------------------------------------------------------------------
const char* imageFormatToStr(Guido2ImageImageFormat imageFormat)
{
	switch ( imageFormat ) {
	case GUIDO_2_IMAGE_PDF:
		return GUIDO_2_IMAGE_PDF_STR;
	case GUIDO_2_IMAGE_BMP:
		return GUIDO_2_IMAGE_BMP_STR;
	case GUIDO_2_IMAGE_GIF:
		return GUIDO_2_IMAGE_GIF_STR;
	case GUIDO_2_IMAGE_JPEG:
		return GUIDO_2_IMAGE_JPEG_STR;
	case GUIDO_2_IMAGE_PNG:
		return GUIDO_2_IMAGE_PNG_STR;
	case GUIDO_2_IMAGE_PGM:
		return GUIDO_2_IMAGE_PGM_STR;
	case GUIDO_2_IMAGE_PPM:
		return GUIDO_2_IMAGE_PPM_STR;
	case GUIDO_2_IMAGE_TIFF:
		return GUIDO_2_IMAGE_TIFF_STR;
	case GUIDO_2_IMAGE_XBM:
		return GUIDO_2_IMAGE_XBM_STR;
	case GUIDO_2_IMAGE_XPM:
		return GUIDO_2_IMAGE_XPM_STR;
	case GUIDO_2_IMAGE_SVG:
		return GUIDO_2_IMAGE_SVG_STR;
	default:
		return "";
	}
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes checkParams( bool isFile , const char * gmnSource , const char * imageFileName , 
		Guido2ImageImageFormat imageFormat , int, const QSize& outputSizeConstraint , float zoom , 
		char * errorMsgBuffer , int bufferSize)
{
	if ( !gmnSource )
	{
		if ( isFile ) {
			WRITE_ERROR(errorMsgBuffer , "gmnFileName is null" , bufferSize );
		} else {
			WRITE_ERROR(errorMsgBuffer , "gmnString is null" , bufferSize );
		}
		return GUIDO_2_IMAGE_SOURCE_IS_NULL;
	}
	
	if ( !imageFileName )
	{
		WRITE_ERROR(errorMsgBuffer , "imageFileName is null" , bufferSize );
		return GUIDO_2_IMAGE_OUTPUT_IS_NULL;
	}
	if ( ( isFile ) && ( !QFile::exists( gmnSource ) ) )
	{
		if ( ( errorMsgBuffer ) && ( bufferSize ) )
			sprintf(errorMsgBuffer , "Can't open file %s" , gmnSource );
		return GUIDO_2_IMAGE_INPUT_FILE_ERROR;
	}
	if ( ( outputSizeConstraint.isNull() ) && ( !zoom ) )
	{
		WRITE_ERROR(errorMsgBuffer , "outputSizeConstraint and zoom are both null" , bufferSize );
		return GUIDO_2_IMAGE_INVALID_SIZE_AND_ZOOM;
	}
	if ( ( imageFormat < 0 ) || ( imageFormat >= GUIDO_2_IMAGE_NB_OF_FORMAT ) )
	{
		WRITE_ERROR(errorMsgBuffer , "Invalid image format" , bufferSize );
		return GUIDO_2_IMAGE_INVALID_IMAGE_FORMAT;
	}
	return GUIDO_2_IMAGE_SUCCESS;
}


//----------------------------------------------------------------------------
const char* Guido2Image::getErrorString( Guido2ImageErrorCodes err )
{
	switch (err) {
		case GUIDO_2_IMAGE_SUCCESS:					 return "success";
		case GUIDO_2_IMAGE_SOURCE_IS_NULL:			 return "invalid input";
		case GUIDO_2_IMAGE_OUTPUT_IS_NULL:			 return "invalid output";
		case GUIDO_2_IMAGE_INPUT_FILE_ERROR:		 return "no such input file";
		case GUIDO_2_IMAGE_INVALID_SIZE_AND_ZOOM:	 return "invalid size and zoom";
		case GUIDO_2_IMAGE_INVALID_PAGE_INDEX:		 return "invalid page index";
		case GUIDO_2_IMAGE_INVALID_GMN_CODE:		 return "invalid gmn code";
		case GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED: return "Guido Engine not started";
		case GUIDO_2_IMAGE_INVALID_IMAGE_FORMAT:	 return "invalid image format";
        case GUIDO_2_IMAGE_OUTPUT_FILE_OVERSIZED:	 return "output size is too big";
        case GUIDO_2_IMAGE_OUTPUT_FILE_UNDERSIZED:	 return "output size is too small";
        case GUIDO_2_IMAGE_UNSPECIFIED_ERROR:        return "unspecified error";
	}
	return "unknown error";
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::check( const Params& p )
{
	if ( !p.input )
		return GUIDO_2_IMAGE_SOURCE_IS_NULL;
	
	if ( !p.output && !p.device )
		return GUIDO_2_IMAGE_OUTPUT_IS_NULL;

	if ( ( p.sizeConstraints.isNull() ) && ( !p.zoom ) ) 
		return GUIDO_2_IMAGE_INVALID_SIZE_AND_ZOOM;

	if ( ( p.format < 0 ) || ( p.format >= GUIDO_2_IMAGE_NB_OF_FORMAT ) )
		return GUIDO_2_IMAGE_INVALID_IMAGE_FORMAT;
	return GUIDO_2_IMAGE_SUCCESS;
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::gmnString2Image(const Params& p)
{
	Guido2ImageErrorCodes errorCode = check(p);

	if (errorCode != GUIDO_2_IMAGE_SUCCESS)
		return errorCode;
		
	QGuidoPainter * painter = QGuidoPainter::createGuidoPainter();

	if (painter) {
		if (p.layout)
            painter->setGuidoLayoutSettings (*p.layout);

        painter->setGMNCode(p.input);
	}
	else
        return GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED;			// likely

	return guidoPainterToImage(painter, p);
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::gmnFile2Image( const  Params& p )
{
	Guido2ImageErrorCodes errorCode = check( p );
	if ( errorCode != GUIDO_2_IMAGE_SUCCESS )
		return errorCode;
	if (!QFile::exists( p.input))
		return GUIDO_2_IMAGE_INPUT_FILE_ERROR;

	QGuidoPainter * painter = QGuidoPainter::createGuidoPainter();
	if (painter) {
		if (p.layout) painter->setGuidoLayoutSettings (*p.layout);
		painter->setGMNFile(p.input);
	}
	else return GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED;			// likely

    return guidoPainterToImage(painter, p);
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::guidoPianoRoll2Image(const Params& p, PianoRoll *pianoRoll)
{
	Guido2ImageErrorCodes result = GUIDO_2_IMAGE_SUCCESS;

	QGuidoPainter *painter = QGuidoPainter::createGuidoPainter();

	if (!painter)
        return GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED;

    if (p.format == GUIDO_2_IMAGE_PDF)
        result = writePianoRollPDF(painter, p.output, pianoRoll, p.sizeConstraints.width(), p.sizeConstraints.height());
    else
        result = writePianoRollImage(painter, p, pianoRoll);

	QGuidoPainter::destroyGuidoPainter(painter);

	return result;
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::guidoPainterToImage(QGuidoPainter * guidoPainter, const Params& p)
{
	Guido2ImageErrorCodes result = GUIDO_2_IMAGE_SUCCESS;

	if (guidoPainter->isGMNValid()) {
		int page = p.pageIndex;
		if (page < 0)
            page = 0;
		else if (page > guidoPainter->pageCount())
            page = guidoPainter->pageCount();

		if (p.format == GUIDO_2_IMAGE_PDF)
			result = writePDF(guidoPainter, page, p.output);
		else
			result = writeImage(guidoPainter, p);
	}
	else
        result = GUIDO_2_IMAGE_INVALID_GMN_CODE;

	QGuidoPainter::destroyGuidoPainter(guidoPainter);

	return result;
}


#define PDF_FORMAT				QString(".pdf")
//----------------------------------------------------------------------------
#ifdef IOS
Guido2ImageErrorCodes Guido2Image::writePDF( QGuidoPainter * guidoPainter, int pageIndex, const char * fname )
{
	return GUIDO_2_IMAGE_OUTPUT_IS_NULL;
}
#else
Guido2ImageErrorCodes Guido2Image::writePDF( QGuidoPainter * guidoPainter, int pageIndex, const char * fname )
{
	QString fileName (fname);
	if ( !fileName.toUpper().endsWith( PDF_FORMAT.toUpper())) fileName += PDF_FORMAT;

	QPrinter printer(QPrinter::HighResolution);
//	printer.setFullPage(true);
	printer.setOutputFormat( QPrinter::PdfFormat );
	printer.setOutputFileName( QString(fileName) );
	printer.setPaperSize( QPrinter::A4 );

	QSizeF firstPageSize = guidoPainter->pageSizeMM(1);
	printer.setPaperSize( firstPageSize, QPrinter::Millimeter );

	QPainter painter(&printer);
	painter.setRenderHint( QPainter::Antialiasing );
	painter.setWindow( QRect( 0 , 0 , firstPageSize.width() , firstPageSize.height() ) );
	
	int pageCount = guidoPainter->pageCount();
	int startPage = 1;
	int lastPage = pageCount;
	if ( pageIndex != 0 ) //Draw only the requested page
	{
		startPage = pageIndex;
		lastPage = pageIndex;
	}
	
	for ( int page = startPage ; page <= lastPage ; page++ )
	{
		QSizeF paperSize = guidoPainter->pageSizeMM(page);
		guidoPainter->draw(&painter,page,QRect(0,0,paperSize.width() , paperSize.height() ) );
		if (page != lastPage)
			printer.newPage();
	}
	painter.end();

    return GUIDO_2_IMAGE_SUCCESS;
}

Guido2ImageErrorCodes Guido2Image::writePianoRollPDF(QGuidoPainter *guidoPainter, const char *fname, PianoRoll *pianoRoll, int width, int height)
{
	QString fileName(fname);

	if (!fileName.toUpper().endsWith(PDF_FORMAT.toUpper()))
        fileName += PDF_FORMAT;

    QPrinter printer(QPrinter::ScreenResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(QString(fileName));

    printer.setFullPage(true);
    printer.setPaperSize(QSizeF(width, height), QPrinter::DevicePixel);

	QPainter painter(&printer);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setWindow(QRect(0, 0, width, height));
	
    GuidoErrCode result = guidoPainter->drawPianoRoll(&painter, QRect(0, 0, width, height), pianoRoll);

	painter.end();
    
    if (result != guidoNoErr)
        return GUIDO_2_IMAGE_UNSPECIFIED_ERROR;

    return GUIDO_2_IMAGE_SUCCESS;
}
#endif

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::writeImage( QGuidoPainter * guidoPainter, const Params& p)
{
	int page = p.pageIndex;
	if (page <= 0) page = 1;
	else if ( page > guidoPainter->pageCount() ) page = guidoPainter->pageCount();

	QSizeF size = guidoPainter->pageSizeMM( page );
	size.setWidth(5 * size.width() * p.zoom);
    size.setHeight(5 * size.height() * p.zoom);
	size = size2constrainedsize (size, p.sizeConstraints);
//qDebug() << "Guido2Image::writeImage size:" << size;

    if (size.width() * size.height() >= 300000000) // It seems that QPainter can't handle images with more than 300 millions of pixels
        return GUIDO_2_IMAGE_OUTPUT_FILE_OVERSIZED;
    else if (size.width() < 1 || size.height() < 1)
        return GUIDO_2_IMAGE_OUTPUT_FILE_UNDERSIZED;

	QImage image( size.width() , size.height() , QImage::Format_ARGB32);
	image.fill( QColor(255,255,255,0).rgba() );

	QPainter painter;
	painter.begin( &image );
	painter.setRenderHints( QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
	
//	if ( page )
		guidoPainter->draw(&painter, page, QRect(0,0,size.width(),size.height()) );
//	else
//	{
//		int pageCoordinateIncrement = 0;
//		for (int page = 1 ; page <= guidoPainter->pageCount() ; page++ )
//		{
//			int pageIncrementedDimension;
//			int x, y, width, height;
//			QSizeF pageSize = guidoPainter->pageSizeMM(page) * p.zoom;
//			pageIncrementedDimension = pageSize.width();
//			width = pageIncrementedDimension;
//			height = size.height();
//			x = pageCoordinateIncrement;
//			y = 0;
//			QRect pageRect( x , y , width , height );
//			guidoPainter->draw( &painter , page , pageRect , QRect() );
//			pageCoordinateIncrement += pageIncrementedDimension;
//		}
//	}
	painter.end();
	if( !Guido2Image::save (&image, p)) return GUIDO_2_IMAGE_INVALID_IMAGE_FORMAT;

    return GUIDO_2_IMAGE_SUCCESS;
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::writePianoRollImage(QGuidoPainter * guidoPainter, const Params& p, PianoRoll *pianoRoll)
{
    QSizeF size = p.sizeConstraints;
	size.setWidth(size.width() * p.zoom);
    size.setHeight(size.height() * p.zoom);

    if (size.width() * size.height() >= 300000000) // It seems that QPainter can't handle images with more than 300 millions of pixels
        return GUIDO_2_IMAGE_OUTPUT_FILE_OVERSIZED;
    else if (size.width() < 1 || size.height() < 1)
        return GUIDO_2_IMAGE_OUTPUT_FILE_UNDERSIZED;

	QImage image(size.width(), size.height(), QImage::Format_ARGB32);
	image.fill(QColor(255, 255, 255, 0).rgba());

	QPainter painter;
	painter.begin(&image);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
	
	GuidoErrCode result = guidoPainter->drawPianoRoll(&painter, QRect(0, 0, size.width(), size.height()), pianoRoll);

	painter.end();
	if (!Guido2Image::save(&image, p)) return GUIDO_2_IMAGE_INVALID_IMAGE_FORMAT;

    if (result != guidoNoErr)
        return GUIDO_2_IMAGE_UNSPECIFIED_ERROR;

    return GUIDO_2_IMAGE_SUCCESS;
}

//----------------------------------------------------------------------------
QSizeF Guido2Image::size2constrainedsize(const QSizeF& size, const QSize& constraint)
{
	float ratio = size.width() / size.height();
	if (!constraint.width() && constraint.height())
		return QSizeF(constraint.height()*ratio, constraint.height());
	if (!constraint.height() && constraint.width())
		return QSizeF(constraint.width(), constraint.width()/ratio);
	if (constraint.height() && constraint.width())
		return QSizeF(constraint.width(), constraint.height());
	return size;
}

//----------------------------------------------------------------------------
bool Guido2Image::save(QPaintDevice * paintDevice, const Params& p)
{
	QImage * pic = dynamic_cast<QImage*>( paintDevice );
	assert(pic);
	if (p.output) {
		QString imageFileName (p.output);
		return pic->save( imageFileName + "." + imageFormatToStr(p.format) );
	}
	else if (p.device)
		return pic->save( p.device, imageFormatToStr(p.format) );
}
