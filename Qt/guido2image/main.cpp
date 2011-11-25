

#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QSize>
#include <QVariant>

#include "Guido2Image.h"
#include "QGuidoPainter.h"

#ifdef USE_UNISTD
	#include <unistd.h>
#else
	#include "XGetopt.h"
#endif

#define ERROR_BUFFER_SIZE 500

using namespace  std;

//------------------------------------------------------------------------------------------
static void usage(const char * name)
{
	cerr << "usages: " << name << " [options]    inputGMNFile   [outputImagefile]"  << endl;
	cerr << "        " << name << " [options] -s inputGMNString  outputImagefile"  << endl;
	cerr << endl;
	cerr << "options:   -s                     String mode" << endl;
	cerr << "           -h imageHeight (int)   Output image height in pixels" << endl;
	cerr << "           -w imageWidth  (int)   Output image width in pixels" << endl;
	cerr << "           -z imageZoom   (float) Output image zoom (bigger/smaller image)" << endl;
	cerr << "           -p                     Page mode : each page of the score will be in a separate image file," << endl;
	cerr << "                                  with suffix _i (where i is the page number)" << endl;
	cerr << "           -f format              Image format (see supported formats below). Default : PNG." << endl;
	cerr << endl;
	cerr << "description:  Save the Guido Score to an image file." << endl;
	cerr << "              Supported formats : PDF, BMP, GIF, JPG, JPEG, PNG, PGM, PPM, TIFF, XBM, XPM" << endl;
	cerr << endl;
	cerr << "note:         * If you use both -h and -w options, the score will be reduced/enlarged to fit" << endl;
	cerr << "                inside the height*width rect ; the score's aspect ratio will be kept." << endl;
	cerr << "              * If you use -h or -w options with -z option, -z option has no effect." << endl;
	cerr << "              * -h, -w and -z options have no effect on PDF's." << endl;
	cerr << "              * -p option has no effect on PDF's ; in PDF's, the score pages are always separated." << endl;
	cerr << "              * To export GIF images, you need the Qt framework to support GIF. (see Qt doc about GIF)" << endl;
	exit(1);
}

//------------------------------------------------------------------------------------------
static int error (const char* msg, const char* name)
{
	cerr << msg << endl;
	usage(name);
	return 1;
}

//------------------------------------------------------------------------------------------
static void parseOutFile(char * outputFile , char **outFormat )
{
	if (!*outFormat) {
		string name (outputFile);
		size_t n = name.find_last_of('.');
		if (n != string::npos) *outFormat = &outputFile[n+1];
	}
}

//------------------------------------------------------------------------------------------
static void parseOptions(int argc, char *argv[] , int* outSFlag, char** outImageHeight, char** outImageWidth, 
			char** outImageZoom , char ** outInput , char ** outOutputFile , int * outPFlag , char **outFormat )
{
	*outSFlag = 0;
	*outImageZoom = 0;
	*outImageHeight = 0;
	*outImageWidth = 0;
	*outInput = 0;
	*outOutputFile = 0;
	*outPFlag = 0;
	*outFormat = 0;

	int index, c;
	opterr = 0;
	while ((c = getopt (argc, argv, "f:psw:h:z:")) != -1)
		switch (c)
		{
			case 's':	*outSFlag = 1;				break;
			case 'h':	*outImageHeight = optarg;	break;
			case 'w':	*outImageWidth = optarg;	break;
			case 'z':	*outImageZoom = optarg;		break;
			case 'f':	*outFormat = optarg;		break;
			case 'p':	*outPFlag = 1;				break;
			case '?':	usage( argv[0] );			return;
			default:
				usage( argv[0] );
				abort();
		}
     
	index = optind;

	//Too many arguments
	if ( argc - index > 2 )	error ("Error : Too many arguments.", argv[0] );
	//In "String Mode", outputFile is mandatory.
	else if ( ( *outSFlag ) && ( argc - index < 2 ) ) {
		if ( argc - index == 1 )
			cerr << "Error : outputImagefile is mandatory with -s option." << endl;
		else
			cerr << "Error : inputGMNString is null." << endl;
		usage( argv[0] );
	}
	//Missing arguments : display help
	else if ( ( *outSFlag == 0 ) && ( argc - index == 0 ) )
		usage( argv[0] );
	else
	{
		*outInput = argv[ index ];
		//String mode or outputFileSpecfied: use the outputImageFile
		if ( ( *outSFlag ) || ( argc - index == 2 ) ) {
//			*outOutputFile = argv[ index+1 ];
			size_t len = strlen(argv[ index+1 ]);
			*outOutputFile = new char[len + 1];
			(*outOutputFile)[len] = 0;
			strncpy( *outOutputFile , argv[ index+1 ] , len );
		}
		//No string mode, and no outputFile specified : use a default output name
		else
		{
			QString autoOutputFile( *outInput );
			int lastIndexOfDot = autoOutputFile.lastIndexOf(".");
			if ( lastIndexOfDot > 0 )
				autoOutputFile = autoOutputFile.left(lastIndexOfDot);
			*outOutputFile = new char[autoOutputFile.length() + 1];
			(*outOutputFile)[autoOutputFile.length()] = 0;
			strncpy( *outOutputFile , autoOutputFile.toAscii().data() , autoOutputFile.length() );
		}
	}
	parseOutFile (*outOutputFile, outFormat);
}

//------------------------------------------------------------------------------------------
Guido2ImageImageFormat strToFormat(char * imageFormat)
{
	QString imageFormatStr(imageFormat);
	imageFormatStr = imageFormatStr.toUpper();
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_PDF_STR).toUpper() )
		return GUIDO_2_IMAGE_PDF;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_BMP_STR).toUpper() )
		return GUIDO_2_IMAGE_BMP;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_JPEG_STR).toUpper() )
		return GUIDO_2_IMAGE_JPEG;
	if ( imageFormatStr == QString("jpg").toUpper() )
		return GUIDO_2_IMAGE_JPEG;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_XBM_STR).toUpper() )
		return GUIDO_2_IMAGE_XBM;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_XPM_STR).toUpper() )
		return GUIDO_2_IMAGE_XPM;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_TIFF_STR).toUpper() )
		return GUIDO_2_IMAGE_TIFF;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_PNG_STR).toUpper() )
		return GUIDO_2_IMAGE_PNG;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_PGM_STR).toUpper() )
		return GUIDO_2_IMAGE_PGM;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_PPM_STR).toUpper() )
		return GUIDO_2_IMAGE_PPM;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_GIF_STR).toUpper() )
		return GUIDO_2_IMAGE_GIF;
	if ( imageFormatStr == QString(GUIDO_2_IMAGE_SVG_STR).toUpper() )
		return GUIDO_2_IMAGE_SVG;

	return GUIDO_2_IMAGE_PNG;
}

#include <QImageReader>

//------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	QApplication app( argc , argv );

	int stringMode;
	int pageMode;
	char * imageZoomStr = 0;
	char * imageHeightStr = 0;
	char * imageWidthStr = 0;
	char * input = 0;
	char * outputFile = 0; 
	char * imageFormatStr = 0; 
	parseOptions( argc, argv, &stringMode, &imageHeightStr, &imageWidthStr , &imageZoomStr, &input, &outputFile , &pageMode , &imageFormatStr );

	float imageZoom = 1;
	int imageHeight = 0;
	int imageWidth = 0;
	if ( (imageHeightStr) || (imageWidthStr) ) {
		if ( imageHeightStr ) {
			imageHeight = atoi( imageHeightStr );
			if ( !imageHeight ) return error("Invalid image height value.", argv[0]);
		}
		if ( imageWidthStr ) {
			imageWidth = atoi( imageWidthStr );
			if ( !imageWidth ) return error("Invalid image width value.", argv[0]);
		}
	}
	else if ( imageZoomStr ) {
		imageZoom = atof( imageZoomStr );
		if ( !imageZoom )	return error("Invalid image zoom value.", argv[0]);
	}

	Guido2ImageErrorCodes (*convertFunction)(const char *, const char *, Guido2ImageImageFormat , int , const QSize& , float, char * , int );

	if ( stringMode )
		convertFunction = &Guido2Image::gmnStringToImage;
	else
		convertFunction = &Guido2Image::gmnFileToImage;

	char errorMsgBuffer[ ERROR_BUFFER_SIZE ];
	const int allPages = 0;

	Guido2ImageImageFormat imageFormat = strToFormat(imageFormatStr);
	int error = 0;
	QGuidoPainter::startGuidoEngine();
	if ( ( pageMode ) && ( imageFormat != GUIDO_2_IMAGE_PDF ) )
	{
		int pageIndex = 1;
		QString outputFileStr( outputFile );
		outputFileStr += "_";
		while ( !error )	//Export the pages until the function fails (at least it will fail returning INVALID_PAGE_INDEX error)
		{
			QString indexedOutputFile = outputFileStr + QVariant(pageIndex).toString();
			error = convertFunction( input , indexedOutputFile.toAscii().data() , imageFormat , pageIndex++ , QSize(imageWidth , imageHeight) , imageZoom , errorMsgBuffer , ERROR_BUFFER_SIZE );
		}
		if ( error == GUIDO_2_IMAGE_INVALID_PAGE_INDEX )
			error = 0;
	}
	else
		error = convertFunction( input , outputFile , imageFormat , allPages , QSize(imageWidth , imageHeight) , imageZoom , errorMsgBuffer , ERROR_BUFFER_SIZE );
	
	if ( error ) cerr << errorMsgBuffer << endl;
	QGuidoPainter::stopGuidoEngine();
	delete outputFile;
	return 0;
}

