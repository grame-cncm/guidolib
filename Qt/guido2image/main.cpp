
/*
	GUIDO Tools
	Copyright (C) 2012	Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2), 
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.
*/

/* change log:
	version 1.1
		- help message includes version number
	version 1.0 (still not displayed)
		- support SVG output
	initial version (not displayed)
*/


#include <iostream>
#include <sstream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QSize>
#include <QVariant>
#include <QDebug>

#include "Guido2Image.h"
#include "QGuidoPainter.h"

#ifdef USE_UNISTD
	#include <unistd.h>
#else
	#include "XGetopt.h"
#endif

#define ERROR_BUFFER_SIZE 500
//------------------------------------------------------------------------------------------
#define kVersion	"1.10"
//------------------------------------------------------------------------------------------

#ifdef WIN32
#define basename (a)	a
#else
#include <libgen.h>
#endif

using namespace  std;

static const char * gAppName;
//------------------------------------------------------------------------------------------
typedef struct Guido2ImageOptions {
	bool stdInMode;
	bool pageMode;
	bool hasLayout;
	const char * inputFile;
	const char * inputString;
	const char * outputFile; 
	const char * imageFormat;
	float	zoom;
	int		height;
	int		width;
	
	float			systemsDistance;
	const char *	systemsDistribution; 
	const char *	optimalPageFill;

	Guido2ImageOptions () 
		: stdInMode(false), pageMode(false), hasLayout(false),
		  inputFile(0), inputString(0), outputFile(0), imageFormat(0),
		  zoom(-1.f), height(-1), width(-1), 
		  systemsDistance(-1.f), systemsDistribution(0), optimalPageFill(0)  {}
} Guido2ImageOptions;

//------------------------------------------------------------------------------------------
static void usage(const char * name)
{
	cerr << name << " version " << kVersion << endl;
	cerr << "usages: (1) " << name << " [options] -f GMNFile   [-o outfile]"  << endl;
	cerr << "        (2) " << name << " [options] -s GMNString  -o outfile"  << endl;
	cerr << "        (3) " << name << " [options] -o outfile"  << endl;
	cerr << endl;
	cerr << "description:  Save a Guido Score to an image file." << endl;
	cerr << "              (1) takes a guido file name as argument and an optional output file name" << endl;
	cerr << "                  when not specified, the output file name is the input file + the corresponding format extension." << endl;
	cerr << "              (2) takes a GMN string as argument and an output file name" << endl;
	cerr << "              (3) reads the standard input" << endl;
	cerr << "              Supported formats : PDF, BMP, GIF, JPG, JPEG, PNG, PGM, PPM, SVG, TIFF, XBM, XPM" << endl;
	cerr << endl;
	cerr << "options:   -h imageHeight (int)   Output image height in pixels" << endl;
	cerr << "           -w imageWidth  (int)   Output image width in pixels" << endl;
	cerr << "           -z imageZoom   (float) Output image zoom (bigger/smaller image)" << endl;
	cerr << "           -p                     Page mode : put each page of the score in a separate image file," << endl;
	cerr << "                                  suffixed with _i (where i is the page number)" << endl;
	cerr << "           -t format              Image format (see supported formats below)." << endl; 
	cerr << "           -?                     print this help message." << endl;
	cerr << "options controlling the Guido Engine layout settings:" << endl;
	cerr << "           -d distance (int)      control the systems distance (default value is 75)." << endl;
	cerr << "           -a [auto|always|never] control systems distribution (default value is 'auto')." << endl;
	cerr << "           -b [on|off]			   control the optimal page fill (default value is 'on')." << endl;
	cerr << endl;
	cerr << "notes:        * If you use both -h and -w options, the score will be reduced/enlarged to fit" << endl;
	cerr << "                inside the height*width rect ; the score's aspect ratio will be preserved." << endl;
	cerr << "              * -z option has no effect with -h or -w options." << endl;
	cerr << "              * -p, -h, -w and -z options are ignored when PDF format is selected." << endl;
	cerr << "              * To export GIF images, you need the Qt framework to support GIF. (see Qt doc about GIF)" << endl;
	exit(1);
}

//------------------------------------------------------------------------------------------
static int error (const char* msg)
{
	cerr << "Error : " << msg << endl;
	usage(gAppName);
	return 1;
}

//------------------------------------------------------------------------------------------
static const char* getExtension(const char * file)
{
	int i=0, lastdot = -1;
	const char * ptr = file;
	while (*ptr) {
		i++;
		if (*ptr++ == '.') lastdot = i;
	}
	return (lastdot > 0) ? &file[lastdot] : 0;
}

//------------------------------------------------------------------------------------------
static void parseOptions(int argc, char *argv[] , Guido2ImageOptions& opts )
{
	int c;
	opterr = 0;
	while ((c = getopt (argc, argv, "f:s:o:pw:h:z:t:d:a:b:?")) != -1)
		switch (c)
		{
			case 'f':	opts.inputFile = optarg;		break;
			case 's':	opts.inputString = optarg;		break;
			case 'o':	opts.outputFile = optarg;		break;

			case 'p':	opts.pageMode = true;			break;
			case 'w':	opts.width = (int)strtol(optarg, 0, 10);	break;
			case 'h':	opts.height = (int)strtol(optarg, 0, 10);	break;
			case 'z':	opts.zoom = strtod(optarg, 0);				break;
			case 't':	opts.imageFormat = optarg;		break;

			case 'd':	opts.systemsDistance = strtod(optarg, 0);
						opts.hasLayout = true;
						break;
			case 'a':	opts.systemsDistribution = optarg;
						opts.hasLayout = true;
						break;
			case 'b':	opts.optimalPageFill = optarg;
						opts.hasLayout = true;
						break;

			default:
				usage( basename(argv[0]) );
				abort();
		}
     
	int remain = argc - optind;
	if (remain) error ("too many arguments.");
	if (opts.inputFile && opts.inputString) error ("-s and -f options are exclusive.");
	if (opts.inputString && !opts.outputFile) error ("missing output file (-o option).");
	if (!opts.inputFile && !opts.inputString && !opts.outputFile) error ("missing output file (-o option).");

	if (opts.width == 0) error ("invalid width value.");
	else if (opts.width < 0) opts.width = 0;
	if (opts.height == 0) error ("invalid height value.");
	else if (opts.height < 0) opts.height = 0;
	if (opts.zoom == 0.f) error ("invalid zoom value.");
	else if (opts.zoom < 0) opts.zoom = 1.f;
	if (opts.systemsDistance == 0.f) error ("invalid systems distance value.");
	
	// try to infer the output format from the output file extension
	if (!opts.imageFormat && opts.outputFile) opts.imageFormat = getExtension(opts.outputFile);
	if (!opts.imageFormat) opts.imageFormat = "PNG";		// default format when none can be inferred	
}

//------------------------------------------------------------------------------------------
Guido2ImageImageFormat strToFormat(const char * imageFormat)
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

//------------------------------------------------------------------------------------------
static void stripext (const char* name, string& outname)
{
	string tmp (name);
	size_t n = tmp.find_last_of ( '.' );
	if (n == string::npos ) outname = name;
	else outname = tmp.substr(0, n);
}

//------------------------------------------------------------------------------------------
static string toLower (const char* str)
{
	string out;
	while (*str) out += tolower(*str++);
	return out;
}

//------------------------------------------------------------------------------------------
static GuidoLayoutSettings* options2layout (const Guido2ImageOptions& opts)
{
	static GuidoLayoutSettings layout = { 75.f, kAutoDistrib, 0.25f, 750, 1.1f, 0, 1 };
	if (opts.hasLayout) {
		if (opts.systemsDistance > 0) layout.systemsDistance = opts.systemsDistance;
		if (opts.systemsDistribution) {
			string str (toLower (opts.systemsDistribution));
			if (str == "auto")			layout.systemsDistribution = kAutoDistrib;
			else if (str == "always")	layout.systemsDistribution = kAlwaysDistrib;
			else if (str == "never")	layout.systemsDistribution = kNeverDistrib;
			else error ("invalid system distribution mode");
		}
		if (opts.optimalPageFill) {
			string str (toLower (opts.optimalPageFill));
			if (str == "on")		layout.optimalPageFill = 1;
			else if (str == "off")	layout.optimalPageFill = 0;
			else error ("invalid optimal page fill mode");
		}
		return &layout;
	}
	return 0;
}

//------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	gAppName = basename (argv[0]);

	QApplication app( argc , argv );
	Guido2ImageOptions options;
	parseOptions( argc, argv, options );

	Guido2ImageErrorCodes (*convertFunction)(const Guido2Image::Params &);

	Guido2Image::Params p;
	string stdinstr;
	if ( options.inputString ) {
		convertFunction = &Guido2Image::gmnString2Image;
		p.input = options.inputString;
	}
	else if ( options.inputFile ) {
		convertFunction = &Guido2Image::gmnFile2Image;
		p.input = options.inputFile;
	}
	else {	// read std in
		convertFunction = &Guido2Image::gmnString2Image;
		int c;
		while (read(0, &c, 1) == 1)
			stdinstr += char(c);
		p.input = stdinstr.c_str();
	}
	
	char errorMsgBuffer[ ERROR_BUFFER_SIZE ];
	string output;
	if (options.outputFile) stripext ( options.outputFile, output);
	else stripext ( options.inputFile, output);
	p.output = output.c_str();
	p.format = strToFormat (options.imageFormat);
	p.layout = options2layout (options);
	p.pageIndex = 0;
	p.sizeConstraints = QSize(options.width , options.height);
	p.zoom = options.zoom;
	p.errorMsgBuffer = errorMsgBuffer;
	p.bufferSize = ERROR_BUFFER_SIZE;

	int error = 0;
	QGuidoPainter::startGuidoEngine();
	if ( options.pageMode && ( p.format != GUIDO_2_IMAGE_PDF ) )
	{
		string outputBase (p.output);
		while ( !error )	// Export the pages until the function fails (at least it will fail returning INVALID_PAGE_INDEX error)
		{
			p.pageIndex++;
			char buff[1024];
			sprintf (buff, "%s_%d", outputBase.c_str(), p.pageIndex);
			p.output = buff;
			error = convertFunction( p );
			output.clear();
		}
		if ( error == GUIDO_2_IMAGE_INVALID_PAGE_INDEX )
			error = 0;
	}
	else error = convertFunction( p );
	
	if ( error ) cerr << errorMsgBuffer << endl;
	QGuidoPainter::stopGuidoEngine();
	return 0;
}

