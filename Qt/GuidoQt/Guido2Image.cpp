/*
 * Guido2Image.cpp
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
#include "QGuidoPainter.h"
#include "QGuidoGraphicsItem.h"

#include "GUIDOEngine.h"

#ifdef GUIDO_2_IMAGE_SVG_SUPPORT
	#include <QSvgGenerator>
#endif

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
		if ( isFile )
			WRITE_ERROR(errorMsgBuffer , "gmnFileName is null" , bufferSize );
		else
			WRITE_ERROR(errorMsgBuffer , "gmnString is null" , bufferSize );
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
Guido2ImageErrorCodes Guido2Image::gmnString2Image( const Params& p )
{
	Guido2ImageErrorCodes errorCode = checkParams( false, p.input, p.output, p.format, p.pageIndex, p.sizeConstraints, p.zoom, p.errorMsgBuffer, p.bufferSize );
	if ( errorCode != GUIDO_2_IMAGE_SUCCESS )
		return errorCode;
		
	QGuidoPainter * painter = buildPainterFromGMNString (p.input, p.layout);
	if ( !painter )
		return GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED;
	return guidoPainterToImage(painter, p.output, p.format, p.pageIndex, p.sizeConstraints, p.zoom, p.errorMsgBuffer, p.bufferSize);
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::gmnStringToImage( const char * gmnString, const char * imageFileName, Guido2ImageImageFormat imageFormat, 
	int pageIndex, const QSize& outputSizeConstraint , float zoom , char * errorMsgBuffer, int bufferSize)
{
	Guido2ImageErrorCodes errorCode = checkParams( false , gmnString , imageFileName , imageFormat , pageIndex , outputSizeConstraint , zoom , errorMsgBuffer , bufferSize );
	if ( errorCode != GUIDO_2_IMAGE_SUCCESS )
		return errorCode;
		
	QGuidoPainter * painter = buildPainterFromGMNString(gmnString);
	if ( !painter )
		return GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED;
	return guidoPainterToImage(painter,imageFileName,imageFormat,pageIndex,outputSizeConstraint,zoom,errorMsgBuffer, bufferSize);
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::gmnFile2Image	( const  Params& p )
{
	Guido2ImageErrorCodes errorCode = checkParams( true , p.input , p.output , p.format , p.pageIndex , p.sizeConstraints, p.zoom, p.errorMsgBuffer, p.bufferSize );
	if ( errorCode != GUIDO_2_IMAGE_SUCCESS )
		return errorCode;
		
	QGuidoPainter * painter = buildPainterFromGMNFile(p.input, p.layout);
	if ( !painter )
		return GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED;
	return guidoPainterToImage(painter, p.output , p.format , p.pageIndex , p.sizeConstraints, p.zoom, p.errorMsgBuffer, p.bufferSize);
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::gmnFileToImage	( const char * gmnFileName , const char * imageFileName	, Guido2ImageImageFormat imageFormat , 
	int pageIndex, const QSize& outputSizeConstraint , float zoom , char * errorMsgBuffer , int bufferSize )
{
	Guido2ImageErrorCodes errorCode = checkParams( true , gmnFileName , imageFileName , imageFormat , pageIndex , outputSizeConstraint , zoom , errorMsgBuffer , bufferSize );
	if ( errorCode != GUIDO_2_IMAGE_SUCCESS )
		return errorCode;
		
	QGuidoPainter * painter = buildPainterFromGMNFile(gmnFileName);
	if ( !painter )
		return GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED;
	return guidoPainterToImage(painter,imageFileName,imageFormat,pageIndex,outputSizeConstraint,zoom,errorMsgBuffer, bufferSize);
}

//----------------------------------------------------------------------------
Guido2ImageErrorCodes Guido2Image::guidoPainterToImage( QGuidoPainter * guidoPainter , const char * imageFileName , Guido2ImageImageFormat imageFormat ,
	int pageIndex, const QSize& outputSizeConstraint , float zoom , char * errorMsgBuffer , int bufferSize)
{
	Guido2ImageErrorCodes result = GUIDO_2_IMAGE_SUCCESS;
	if ( guidoPainter->isGMNValid() )
	{
		if ( ( pageIndex < 0 ) || ( pageIndex > guidoPainter->pageCount() ) )
		{
			WRITE_ERROR( errorMsgBuffer , "Invalid page index" , bufferSize );
			result = GUIDO_2_IMAGE_INVALID_PAGE_INDEX;
		}
		else 
		{
			if ( imageFormat == GUIDO_2_IMAGE_PDF )
				writePDF( guidoPainter , pageIndex , imageFileName );
			else
				writeImage(guidoPainter,imageFormat,pageIndex , outputSizeConstraint, zoom, imageFileName);
		}
	}
	else
	{
		WRITE_ERROR( errorMsgBuffer , guidoPainter->getLastErrorMessage().toAscii().data() , bufferSize );
		result = GUIDO_2_IMAGE_INVALID_GMN_CODE;
	}
	QGuidoPainter::destroyGuidoPainter( guidoPainter );
	return result;
}

//----------------------------------------------------------------------------
QGuidoPainter * Guido2Image::buildPainterFromGMNString	( const char * gmnString, const GuidoLayoutSettings* layout )
{
	QGuidoPainter * guidoPainter = QGuidoPainter::createGuidoPainter();
	if (guidoPainter) {
		if (layout) guidoPainter->setGuidoLayoutSettings (*layout);
		guidoPainter->setGMNCode(QString(gmnString));
	}
	return guidoPainter;
}

//----------------------------------------------------------------------------
QGuidoPainter * Guido2Image::buildPainterFromGMNFile ( const char * gmnFileName, const GuidoLayoutSettings* layout )
{
	QGuidoPainter * guidoPainter = QGuidoPainter::createGuidoPainter();
	if (guidoPainter) {
		if (layout) guidoPainter->setGuidoLayoutSettings (*layout);
		guidoPainter->setGMNFile(QString(gmnFileName));
	}
	return guidoPainter;
}

#define PDF_FORMAT				QString(".pdf")
//----------------------------------------------------------------------------
void Guido2Image::writePDF( QGuidoPainter * guidoPainter , int pageIndex, const char * fname )
{
	QString fileName (fname);
	if ( !fileName.toUpper().endsWith( PDF_FORMAT.toUpper())) fileName += PDF_FORMAT;

	QPrinter printer;
	printer.setFullPage(true);
	printer.setOutputFileName( fileName );
	printer.setOutputFormat( QPrinter::PdfFormat );

	QSizeF firstPageSize = guidoPainter->pageSizeMM(1);
	printer.setPaperSize( firstPageSize , QPrinter::Millimeter );

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
//		printer.setPaperSize( paperSize , QPrinter::Millimeter );
//		painter.setWindow( QRect( 0 , 0 , paperSize.width() , paperSize.height() ) );
//		paperSize = printer.paperSize( QPrinter::DevicePixel );

		guidoPainter->draw(&painter,page,QRect(0,0,paperSize.width() , paperSize.height() ) );

		if (page != lastPage)
			printer.newPage();
	}

	painter.end();
}

//----------------------------------------------------------------------------
void Guido2Image::writeImage( QGuidoPainter * guidoPainter, Guido2ImageImageFormat imageFormat, int pageIndex, const QSize& outputSizeConstraint, float zoom, const char * imageFileName )
{
	if (imageFormat == GUIDO_2_IMAGE_SVG) {
		std::string outname = imageFileName;
		outname += '.';
		outname += imageFormatToStr(imageFormat);
		std::fstream out(outname.c_str(), std::fstream::out | std::fstream::trunc);
		GuidoErrCode err = GuidoSVGExport (guidoPainter->getGRHandler(), pageIndex+1, out, 0);
		if (err != guidoNoErr)
			std::cerr << "Error writing SVG file: " << GuidoGetErrorString (err) << std::endl;
		return;
	}

	QRectF pictureRect = Guido2Image::getPictureRect( guidoPainter , pageIndex, outputSizeConstraint , zoom );
	
	QPaintDevice * paintDevice = Guido2Image::getPaintDevice( pictureRect , imageFormat , imageFileName);
	
	QPainter painter(paintDevice);
	painter.setRenderHint( QPainter::Antialiasing );
	
	if ( pageIndex )
		guidoPainter->draw(&painter,pageIndex,pictureRect.toRect());
	else
	{
		int pageCoordinateIncrement = 0;
		for (int page = 1 ; page <= guidoPainter->pageCount() ; page++ )
		{
			int pageIncrementedDimension;
			int x, y, width, height;
			QSizeF pageSize = guidoPainter->pageSizeMM(page) * zoom;
//			if ( PAGES_ALIGNMENT == QGuidoMPageItem::Horizontal )
			if ( true )
			{
				pageIncrementedDimension = pageSize.width();
				width = pageIncrementedDimension;
				height = pictureRect.height();
				x = pageCoordinateIncrement;
				y = 0;
			}
			else
			{
				pageIncrementedDimension = pageSize.height();
				height = pageIncrementedDimension;
				width = pictureRect.width();
				y = pageCoordinateIncrement;
				x = 0;
			}
			QRect pageRect( x , y , width , height );
			guidoPainter->draw( &painter , page , pageRect , QRect() );
			pageCoordinateIncrement += pageIncrementedDimension;
		}
	}

	painter.end();
	
	Guido2Image::finalizePaintDevice(paintDevice , imageFileName , imageFormat);
	
	delete paintDevice;
}

//----------------------------------------------------------------------------
#ifdef GUIDO_2_IMAGE_SVG_SUPPORT
QPaintDevice * Guido2Image::getPaintDevice( const QRectF& pictureRect , Guido2ImageImageFormat imageFormat , const QString& imageFileName )
#else
QPaintDevice * Guido2Image::getPaintDevice( const QRectF& pictureRect , Guido2ImageImageFormat imageFormat , const QString& )
#endif
{
	QPaintDevice * paintDevice = 0;
	if ( imageFormat != GUIDO_2_IMAGE_SVG )
	{
		QImage * pic = new QImage( QSize( pictureRect.width() , pictureRect.height() ) , QImage::Format_ARGB32_Premultiplied );
		pic->fill( QColor(Qt::white).rgb() );
		paintDevice = pic; 
	}
	else
	{
#ifdef GUIDO_2_IMAGE_SVG_SUPPORT
		QSvgGenerator * svgGenerator = new QSvgGenerator();
		svgGenerator->setFileName( imageFileName + "." + imageFormatToStr(imageFormat) );
		svgGenerator->setSize( pictureRect.size().toSize() );
		paintDevice = svgGenerator;
#else
		qFatal( "Guido2Image error: To use the SVG format, you must #define GUIDO_2_IMAGE_SVG_SUPPORT when compiling Guido2Image.cpp");
		assert(0);
#endif
	}
	return paintDevice;
}

//----------------------------------------------------------------------------
void Guido2Image::finalizePaintDevice(QPaintDevice * paintDevice  , const QString& imageFileName , Guido2ImageImageFormat imageFormat)
{
	if ( imageFormat != GUIDO_2_IMAGE_SVG )
	{
		QImage * pic = dynamic_cast<QImage*>( paintDevice );
		assert(pic);
		pic->save( imageFileName + "." + imageFormatToStr(imageFormat) );
	}
}
	
//----------------------------------------------------------------------------
QRectF Guido2Image::getPictureRect(QGuidoPainter * guidoPainter , int pageIndex, const QSize& outputSizeConstraint , float zoom )
{
	QRectF pictureRect;
	
	if ( pageIndex )
	{
		QSizeF s = guidoPainter->pageSizeMM( pageIndex );
		pictureRect = QRectF( 0, 0, s.width(), s.height() );
	}
	else
	{
		pictureRect = QRectF(0,0,0,0);
		for (int page = 1 ; page <= guidoPainter->pageCount() ; page++ ) 
		{
			QSizeF pageSize = guidoPainter->pageSizeMM(page);
//			if ( PAGES_ALIGNMENT == QGuidoMPageItem::Horizontal )
			if (true)
			{
				pictureRect.setWidth( pictureRect.width() + pageSize.width() );
				pictureRect.setHeight( ( pictureRect.height() > pageSize.height() ) ? pictureRect.height() : pageSize.height() );
			}
			else
			{
				pictureRect.setHeight( pictureRect.height() + pageSize.height() );
				pictureRect.setWidth( ( pictureRect.width() > pageSize.width() ) ? pictureRect.width() : pageSize.width() );
			}
		}
	}
	
	bool widthIsConstraining = false, heightIsConstraining = false;
	if ( outputSizeConstraint.width() || outputSizeConstraint.height() ) 
	{
		if ( outputSizeConstraint.width() && outputSizeConstraint.height() ) 
		{
			float heightZoom = outputSizeConstraint.height() / pictureRect.height();
			float widthZoom = outputSizeConstraint.width() / pictureRect.width();
			if ( heightZoom > widthZoom )
			{
				widthIsConstraining = true;
				zoom = widthZoom;
			}
			else
			{
				heightIsConstraining = true;
				zoom = heightZoom;
			}
		}
		else if ( outputSizeConstraint.width() )
		{
			widthIsConstraining = true;
			zoom = outputSizeConstraint.width() / pictureRect.width();
		}
		else if ( outputSizeConstraint.height() )
		{
			heightIsConstraining = true;
			zoom = outputSizeConstraint.height() / pictureRect.height();
		}
	}
	assert(zoom);

	pictureRect.setWidth( (int)(pictureRect.width() * zoom + 0.5f) );
	pictureRect.setHeight( (int)(pictureRect.height() * zoom + 0.5f) );

	if ( widthIsConstraining )
		pictureRect.setWidth( outputSizeConstraint.width() );
	if ( heightIsConstraining )
		pictureRect.setHeight( outputSizeConstraint.height() );
	
	return pictureRect;
}
