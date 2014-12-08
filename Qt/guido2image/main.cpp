
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

#include "GUIDOPianoRoll.h"

#ifdef USE_UNISTD
	#include <unistd.h>
#else
	#include "XGetopt.h"
#endif

#define ERROR_BUFFER_SIZE 500
//------------------------------------------------------------------------------------------
#define kVersion	"1.20"
//------------------------------------------------------------------------------------------

#ifdef WIN32
#include <io.h>
#define basename(a)	a
#define read	_read
#else
#include <libgen.h>
#endif

using namespace  std;

static const char * gAppName;
//------------------------------------------------------------------------------------------
typedef struct Guido2ImageOptions {
	bool stdInMode;
	bool hasLayout;
	int	 page;
	const char * inputFile;
	const char * inputString;
	const char * outputFile; 
	const char * imageFormat;
    const char * scoreFormat;
	float	zoom;
	int		height;
	int		width;
	
	float			systemsDistance;
	const char *	systemsDistribution; 
	const char *	optimalPageFill;
    float           proportionalRendering;
    const char *    resize2Page;

	Guido2ImageOptions () 
		: stdInMode(false), hasLayout(false), page(1),
		  inputFile(0), inputString(0), outputFile(0), imageFormat(0), scoreFormat("classic"),
		  zoom(-1.f), height(-1), width(-1), 
          systemsDistance(-1.f), systemsDistribution(0), optimalPageFill(0), proportionalRendering(0), resize2Page("on")  {}
} Guido2ImageOptions;

//------------------------------------------------------------------------------------------
static void usage(const char * name)
{
	cerr << name << " version " << kVersion << " using Guido Engine v." << GuidoGetVersionStr() << endl;
	cerr << "usages: (1) " << name << " [options] -f GMNFile   [-o outfile]"  << endl;
	cerr << "        (2) " << name << " [options] -s GMNString  -o outfile"  << endl;
	cerr << "        (3) " << name << " [options] -o outfile"  << endl;
	cerr << endl;
	cerr << "description:  Save a Guido Score to an image file." << endl;
	cerr << "              (1) takes a guido file name as argument and an optional output file name" << endl;
	cerr << "                  when not specified, the output file name is the input file + the corresponding format extension." << endl;
	cerr << "              (2) takes a GMN string as argument and an output file name" << endl;
	cerr << "              (3) reads the standard input" << endl;
	cerr << endl;
	cerr << "options:   -h imageHeight (int)    Output image height in pixels" << endl;
	cerr << "           -w imageWidth  (int)    Output image width in pixels" << endl;
	cerr << "           -z imageZoom   (float)  Output image zoom (bigger/smaller image)" << endl;
	cerr << "           -p num (int)            print page number 'num' (default is 1)" << endl;
	cerr << "           -t format               Image format" << endl; 
	cerr << "                                   Supported formats: PDF, BMP, GIF, JPG, JPEG, PNG, PGM, PPM, SVG, TIFF, XBM, XPM" << endl;
	cerr << "           -q format               Score format" << endl; 
	cerr << "                                   Supported formats: pianoroll, classic (default is classic)" << endl;
	cerr << "           -v                      print the version number and exit." << endl;
	cerr << "           -?                      print this help message." << endl;
	cerr << "options controlling the Guido Engine layout settings:" << endl;
	cerr << "           -d distance (int)       control the systems distance (default value is 75)." << endl;
	cerr << "           -a [auto|always|never]  control systems distribution (default value is 'auto')." << endl;
	cerr << "           -b [on|off]			    control the optimal page fill (default value is 'on')." << endl;
	cerr << "           -r [on|off]             automatically resize page to music (default value is 'on')." << endl;
	cerr << "           -c (float)			    sets the proportional force multiplicator (0 is default value and disables proportional rendering)." << endl;
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
	while ((c = getopt (argc, argv, "f:s:o:pw:h:z:t:q:d:a:b:r:c:?:v")) != -1)
		switch (c)
		{
			case 'f':	opts.inputFile = optarg;		break;
			case 's':	opts.inputString = optarg;		break;
			case 'o':	opts.outputFile = optarg;		break;

			case 'p':	opts.page = (int)strtol(optarg, 0, 10);		break;
			case 'w':	opts.width = (int)strtol(optarg, 0, 10);	break;
			case 'h':	opts.height = (int)strtol(optarg, 0, 10);	break;
			case 'z':	opts.zoom = strtod(optarg, 0);				break;
			case 't':	opts.imageFormat = optarg;		break;
            case 'q':	opts.scoreFormat = optarg;		break;

			case 'd':	opts.systemsDistance = strtod(optarg, 0);
						opts.hasLayout = true;
						break;
			case 'a':	opts.systemsDistribution = optarg;
						opts.hasLayout = true;
						break;
			case 'b':	opts.optimalPageFill = optarg;
						opts.hasLayout = true;
						break;
			case 'r':	opts.resize2Page = optarg;
						opts.hasLayout = true;
						break;
			case 'c':	opts.proportionalRendering = strtod(optarg, 0);
						opts.hasLayout = true;
						break;
			case 'v':	cout << basename(argv[0]) << " version " << kVersion << " using Guido Engine v." << GuidoGetVersionStr() << endl;
						exit(0);

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
	static GuidoLayoutSettings layout = { 75.f, kAutoDistrib, 0.25f, 750, 1.1f, 0, 1, 1 };
	if (opts.hasLayout) {
		if (opts.systemsDistance > 0)
            layout.systemsDistance = opts.systemsDistance;

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

        if (opts.resize2Page) {
			string str (toLower (opts.resize2Page));
			if (str == "on")		layout.resizePage2Music = 1;
			else if (str == "off")	layout.resizePage2Music = 0;
			else error ("invalid resize page to music mode");
		}

        layout.proportionalRenderingForceMultiplicator = opts.proportionalRendering;

		return &layout;
	}
	return 0;
}

//------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	gAppName = basename (argv[0]);		// stores the application name for global access
	QApplication app( argc , argv );	// required by Qt
	Guido2ImageOptions options;			// the tool options
	parseOptions( argc, argv, options );// parse the command line (may exit in case of incorrect args)

	Guido2Image::Params p;		// a struct used as arg of the conversion function
	string stdinstr;			// a string to read the standard input
	Guido2ImageErrorCodes (*convertFunction)(const Guido2Image::Params &);	// a pointer to the conversion function

    if (options.inputString) {							    // string parsing mode
        convertFunction = &Guido2Image::gmnString2Image;
		p.input = options.inputString;
	}
	else if (options.inputFile) {							// file parsing mode
		convertFunction = &Guido2Image::gmnFile2Image;
		p.input = options.inputFile;
	}
	else {													// std in mode
		convertFunction = &Guido2Image::gmnString2Image;	// handled like the string parsing mode
		int c;
		while (read(0, &c, 1) == 1)
			stdinstr += char(c);							// read the standard input into a string
		p.input = stdinstr.c_str();
	}
	
	//----------------------------------------------------
	// the stuff below is for historical reasons and should be avoided
	string output;											// a string for the ouput file name i.e. without extension
	// removes the file extension (when there is one)
	if (options.outputFile) stripext ( options.outputFile, output);	
	else stripext ( options.inputFile, output);
	p.output = output.c_str();
	//----------------------------------------------------
	p.pageFormat = 0;
	p.format = strToFormat (options.imageFormat);			// the image output format
	p.layout = options2layout (options);					// the layout options (if any)
	p.pageIndex = 0;										// page index starts at 0 (I guess it means all pages - to be checked)
	p.sizeConstraints = QSize(options.width , options.height); // size constraints
	p.zoom = options.zoom;									// zoom value

	Guido2ImageErrorCodes error = GUIDO_2_IMAGE_SUCCESS;
	QGuidoPainter::startGuidoEngine();						// starts the guido engine
	// the page mode should be removed in a future version
	// a simple shell script can do the equivalent without constraint on the naming scheme
//	if (options.pageMode && (p.format != GUIDO_2_IMAGE_PDF))// we're in page mode 
//	{
//		string outputBase (p.output);
//		while ( error == GUIDO_2_IMAGE_SUCCESS )	// Export the pages until the function fails (at least it will fail returning INVALID_PAGE_INDEX error)
//		{
//			p.pageIndex++;
//			//----------------------------------------------------
//			// this was originaly made using QStrings + QVariant
//			// another version was using a stringstream
//			// both were producing an unexpected strange behavior (inconsistent output name after calling GuidoAR2GR)
//			char buff[1024]; 
//			sprintf (buff, "%s_%d", outputBase.c_str(), p.pageIndex);
//			//----------------------------------------------------
//			p.output = buff;
//			error = convertFunction( p );					// convert to an image
//			output.clear();
//		}
//		if ( error == GUIDO_2_IMAGE_INVALID_PAGE_INDEX )
//			error = GUIDO_2_IMAGE_SUCCESS;
//	}

    if (!strcmp(options.scoreFormat, GUIDO_2_IMAGE_PIANO_ROLL_STR)) {
        GuidoParser *parser = GuidoOpenParser();

        ARHandler arh;

        if (options.inputFile)
            arh = GuidoFile2AR(parser, options.inputFile);
        else
            arh = GuidoString2AR(parser, options.inputString);
        
        PianoRoll *pianoRoll = GuidoAR2PianoRoll(kSimplePianoRoll, arh);
        GuidoPianoRollEnableKeyboard(pianoRoll, true);

        if (p.sizeConstraints.width() <= 0)
            p.sizeConstraints.setWidth(1024);

        if (p.sizeConstraints.height() <= 0)
            p.sizeConstraints.setHeight(512);

        error = Guido2Image::guidoPianoRoll2Image(p, pianoRoll);

        GuidoDestroyPianoRoll(pianoRoll);
    }
    else
	    error = convertFunction(p);			                // convert to an image

	QGuidoPainter::stopGuidoEngine();						// stop the guido engine
	
	if (error)
        cerr << Guido2Image::getErrorString(error) << endl;
	
    return error;
}

