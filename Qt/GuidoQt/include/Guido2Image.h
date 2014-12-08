/*
 * Guido2Image.h
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
#ifndef GUIDO_2_IMAGE_H
#define GUIDO_2_IMAGE_H

#include <QSize>
#include <QRectF>
#include <QString>
#include <QIODevice>

#include "GUIDOEngine.h"
#include "GUIDOPianoRoll.h"

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
	GUIDO_2_IMAGE_INVALID_IMAGE_FORMAT = 8,
	//! The output image is too big to be created
	GUIDO_2_IMAGE_OUTPUT_FILE_OVERSIZED = 9,
	//! The output image is too small to be created
	GUIDO_2_IMAGE_OUTPUT_FILE_UNDERSIZED = 10,
	//! All unspecified errors
	GUIDO_2_IMAGE_UNSPECIFIED_ERROR = 11
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

#define GUIDO_2_IMAGE_PIANO_ROLL_STR    "pianoroll"
#define GUIDO_2_IMAGE_CLASSIC_SCORE_STR "classic"

/**
*	\brief Offers functions to export GMN code (from a string or a file) to various formats of images, or to PDF.
*/
class Guido2Image
{
	public :
		typedef struct Params {
			const char *			input;				///< the input file name or gmn code
			const char *			output;				///< the output file name
			QIODevice*				device;				///< or the output device
			Guido2ImageImageFormat	format;				///< the output format
			const GuidoLayoutSettings*	layout;			///< layout setting (see guido doc)
			const GuidoPageFormat*	pageFormat;			///< layout setting (see guido doc)
			int						pageIndex;
			QSize					sizeConstraints;
			float					zoom;
			bool resizePageToMusic; // do we resize the page to the music?
			Params () : input(0), output(0), device(0), format(GUIDO_2_IMAGE_PNG), layout(0), pageIndex(1), zoom(1.0), resizePageToMusic(false) {}
		} Params;

		/*!
		*	\brief	gives an error code textual description
		*/
		static const char* getErrorString( Guido2ImageErrorCodes err );

		/*!
		*	\brief	converts a gmn string to an image
		*/
		static Guido2ImageErrorCodes gmnString2Image( const Params& p);

		/*!
		*	\brief	converts a gmn file to an image
		*/
		static Guido2ImageErrorCodes gmnFile2Image	( const Params& p);
        
        /*!
		*	\brief	converts a gmn file to an image
		*/
        static Guido2ImageErrorCodes guidoPianoRoll2Image(const Params& p, PianoRoll *pianoRoll);

	private :

		static Guido2ImageErrorCodes	check( const Params& p );
		
		static Guido2ImageErrorCodes	guidoPainterToImage ( QGuidoPainter * guidoPainter, const Params& p );
		static Guido2ImageErrorCodes	writeImage			( QGuidoPainter * guidoPainter, const Params& p );
        static Guido2ImageErrorCodes	writePianoRollImage	( QGuidoPainter * guidoPainter, const Params& p, PianoRoll *pianoRoll);
		static Guido2ImageErrorCodes	writePDF			( QGuidoPainter * guidoPainter, int pageIndex, const char * fileName );
		static Guido2ImageErrorCodes	writePianoRollPDF	( QGuidoPainter * guidoPainter, const char * fileName, PianoRoll *pianoRoll, int width, int height );
		
		static QRectF			getPictureRect(QGuidoPainter * guidoPainter, int pageIndex, const QSize& outputSizeConstraint, float zoom);
		static void				save(QPaintDevice * paintDevice, const Params& p);
		static QSizeF			size2constrainedsize(const QSizeF& size, const QSize& constraint);
};

#endif
