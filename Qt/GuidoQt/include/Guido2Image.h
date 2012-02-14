/*
 * Guido2Image.h
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
#ifndef GUIDO_2_IMAGE_H
#define GUIDO_2_IMAGE_H

#include <QSize>
#include <QRectF>
#include <QString>
#include <QIODevice>

#ifdef WIN32
#define warndeprecated __declspec(deprecated("** method is deprecated **"))
#else
#define warndeprecated  __attribute__((__deprecated__))
#endif


class QPaintDevice;
class QGuidoPainter;
struct GuidoLayoutSettings;

/** \brief Error codes description (values returned by gmnStringToImage and gmnFileToImage).
*
*/
enum Guido2ImageErrorCodes
{
	//! Success
	GUIDO_2_IMAGE_SUCCESS = 0,
	//! The input GMN file/GMN string is null
	GUIDO_2_IMAGE_SOURCE_IS_NULL = 1,
	//! The output file is null
	GUIDO_2_IMAGE_OUTPUT_IS_NULL = 2,
	//! The input GMN file can't be opened
	GUIDO_2_IMAGE_INPUT_FILE_ERROR = 3,
	//! The zoom and size-constraint specified for the output image are invalid (typically, both are null).
	GUIDO_2_IMAGE_INVALID_SIZE_AND_ZOOM = 4,
	//! The page index is either negative, or bigger than the page number of the score.
	GUIDO_2_IMAGE_INVALID_PAGE_INDEX = 5,
	//! The GMN code inside the input GMN file/GMN string is not valid. You can have a more detailed error message in errorMsgBuffer.
	GUIDO_2_IMAGE_INVALID_GMN_CODE = 6,
	//! You forgot to call QGuidoPainter::startGuidoEngine.
	GUIDO_2_IMAGE_GUIDO_ENGINE_NOT_STARTED = 7,
	//! The image format is not valid (see Guido2ImageImageFormat).
	GUIDO_2_IMAGE_INVALID_IMAGE_FORMAT = 8
};

/*!
*	\brief List of the supported image formats.
*/
enum Guido2ImageImageFormat
{
	GUIDO_2_IMAGE_PDF = 0, 
	GUIDO_2_IMAGE_BMP, 
	GUIDO_2_IMAGE_GIF, 
	GUIDO_2_IMAGE_JPEG, 
	GUIDO_2_IMAGE_PNG, 
	GUIDO_2_IMAGE_PGM, 
	GUIDO_2_IMAGE_PPM, 
	GUIDO_2_IMAGE_TIFF, 
	GUIDO_2_IMAGE_XBM, 
	GUIDO_2_IMAGE_XPM,
	GUIDO_2_IMAGE_SVG,
	GUIDO_2_IMAGE_NB_OF_FORMAT
};

#define GUIDO_2_IMAGE_PDF_STR "pdf"
#define GUIDO_2_IMAGE_BMP_STR "bmp"
#define GUIDO_2_IMAGE_GIF_STR "gif"
#define GUIDO_2_IMAGE_JPEG_STR "jpeg"
#define GUIDO_2_IMAGE_PNG_STR "png"
#define GUIDO_2_IMAGE_PGM_STR "pgm"
#define GUIDO_2_IMAGE_PPM_STR "ppm"
#define GUIDO_2_IMAGE_TIFF_STR "tiff"
#define GUIDO_2_IMAGE_XBM_STR "xbm"
#define GUIDO_2_IMAGE_XPM_STR "xpm"
#define GUIDO_2_IMAGE_SVG_STR "svg"

/**
*	\brief Offers functions to export GMN code (from a string or a file) to various formats of images, or to PDF.
*/
class Guido2Image
{
	public :
		typedef struct Params {
			const char *			input;
			const char *			output;
			QIODevice*				device;
			Guido2ImageImageFormat	format;
			const GuidoLayoutSettings*	layout;
			int						pageIndex;
			QSize					sizeConstraints;
			float					zoom;
			Params () : input(0), output(0), device(0), format(GUIDO_2_IMAGE_PNG), layout(0), pageIndex(1), zoom(1.0) {}
		} Params;

		/*!
		*	\brief	Build a Guido Score from the specified string, and exports the Guido Score to the specified image.
		*	
		*			\param gmnString		The GMN code used to build the Guido Score.
		*			\param imageFileName	The output image file, without suffix. A suffix will be added to the actual output file, according to the imageFormat (param below).
		*			\param imageFormat		The output image format (see Guido2ImageImageFormat above).
		*			\param outputSizeConstraint	In pixels. The output image will be maximized to fit inside this size constraint. 
		*										The height/width ratio of the score remains unchanged. 
		*										If null (that is : width==height==0), ignored.
		*										If one dimension is null, only the other one is used.
		*			\param zoom				Conversion factor between the Guido Score page format in mm and the image pixel resolution. 
		*									If outputSizeConstraint is not null, this parameter is ignored.
		*			\param pageIndex		Index of the score page to draw. Starts with 1. 0 is all-pages. Invalid page indexes will make the
		*									function fail (see Guido2ImageErrorCodes above).
		*			\param errorMsgBuffer	If the function fails and errorMsgBuffer nor bufferSize are null, the error message will be written in this buffer.
		*			\param bufferSize		Size of the errorMsgBuffer.
		*
		*			\note	If both outputSizeConstraint & zoom are null, error GUIDO_2_IMAGE_INVALID_SIZE_AND_ZOOM is returned.
		*			\note	Params outputSizeConstraint & zoom are ignored when using PDF format.
		*			\note	You must call QGuidoPainter::startGuidoEngine before calling this function, and call QGuidoPainter::stopGuidoEngine
		*					after (or at least once at the end of your application).
		*			\warning	To export GIF images, you need the Qt framework to support this format. (see Qt doc about GIF).
		*
		*			\return 0: Success. Else, error (see Guido2ImageErrorCodes above).
		*/
		warndeprecated static Guido2ImageErrorCodes gmnStringToImage( const char * gmnString, const char * imageFileName, Guido2ImageImageFormat imageFormat, 
										int pageIndex, const QSize& outputSizeConstraint, float zoom, 
										char * errorMsgBuffer = 0, int bufferSize = 0);

		/*!
		*	\brief	Same as gmnStringToImage above, but using a data structure instead.
		*/
		static const char* getErrorString( Guido2ImageErrorCodes err );

		/*!
		*	\brief	Same as gmnStringToImage above, but using a data structure instead.
		*/
		static Guido2ImageErrorCodes gmnString2Image( const Params& p);

		/*!
		*	\brief	Same as gmnStringToImage, except that it uses the gmnFileName GMN file.
		*/
		warndeprecated static Guido2ImageErrorCodes gmnFileToImage	( const char * gmnFileName, const char * imageFileName, Guido2ImageImageFormat imageFormat,
										int pageIndex, const QSize& outputSizeConstraint, float zoom,  
										char * errorMsgBuffer = 0, int bufferSize = 0);
		/*!
		*	\brief	Same as gmnFileToImage above, but output is send to dev instead a file.
		*/
		static Guido2ImageErrorCodes gmnFile2Image	( const Params& p);

	private :

		static Guido2ImageErrorCodes check( const Params& p );
		
		static QGuidoPainter * buildPainterFromGMNString	( const char * gmnString, const GuidoLayoutSettings* layout = 0 );
		static QGuidoPainter * buildPainterFromGMNFile		( const char * gmnFileName, const GuidoLayoutSettings* layout = 0 );

		static Guido2ImageErrorCodes guidoPainterToImage( QGuidoPainter * guidoPainter, const char * imageFileName, Guido2ImageImageFormat imageFormat, int pageIndex, const QSize& outputSizeConstraint, float zoom, char * errorMsgBuffer, int bufferSize );
		static Guido2ImageErrorCodes guidoPainterToImage( QGuidoPainter * guidoPainter, const Params& p );

		static void writeImage( QGuidoPainter * guidoPainter, Guido2ImageImageFormat imageFormat, int pageIndex, const QSize& outputSizeConstraint, float zoom, const char * imageFileName );
		static void writeImage( QGuidoPainter * guidoPainter, const Params& p);
		static void writePDF( QGuidoPainter * guidoPainter, int pageIndex, const char * fileName );
		
		static QRectF			getPictureRect(QGuidoPainter * guidoPainter, int pageIndex, const QSize& outputSizeConstraint, float zoom );
		static QPaintDevice *	getPaintDevice( const QRectF& pictureRect);
		static void				finalizePaintDevice(QPaintDevice * paintDevice, const QString& imageFileName, Guido2ImageImageFormat imageFormat);
		static void				save(QPaintDevice * paintDevice, const Params& p);
};

#endif
