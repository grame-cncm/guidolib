
#include <vector>
#include <string>

#ifndef WIN32
#include <libgen.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#else
#pragma warning(disable:4996)
#endif

#include <string.h>

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "GUIDOReducedProportional.h"
#include "Colors.h"
#include "SVGDevice.h"
#include "SVGSystem.h"

using namespace std;

const int  kDefaultWidth           = -1;
const int  kDefaultHeight          = -1;
const int  kDefaultMinPitch        = -1;
const int  kDefaultMaxPitch        = -1;

const char* kOptions[] = { "-help", "-width", "-height", "-start", "-end", "-minpitch", "-maxpitch", "-voicesautocolor", "-measurebars", "-lines", "-voicecolor" };
enum { kHelp, kWidth, kHeight, kStart, kEnd, kMinPitch, kMaxPitch, kVoicesAutoColor, kMeasureBars, kLines, kVoiceColor, kMaxOpt };

//---------------------------------------------------------------------------------------------
static void usage(char* name)
{
#ifndef WIN32
	const char* tool = basename (name);
#else
	const char* tool = name;
#endif
	cerr << "usage: " << tool << " [options] gmnfile" << endl;
	cerr << "options:" << endl;
	cerr << "         -width           value  : set the output width (default is " << kDefaultWidth << " -> width is adjusted to 1024)" << endl;
	cerr << "         -height          value  : set the output height (default is " << kDefaultHeight << " -> height is adjusted to 512)" << endl;
	cerr << "         -start           date   : set time zone start (default is 0/0 -> start time is automatically adjusted)" << endl;
	cerr << "         -end             date   : set time zone end (default is 0/0 -> end time is automatically adjusted)" << endl;
    cerr << "         -minpitch        value  : set minimum midi pitch (default is " << kDefaultMinPitch << " -> min pitch is automatically adjusted)" << endl;
	cerr << "         -maxpitch        value  : set maximum midi pitch (default is " << kDefaultMaxPitch << " -> max pitch is automatically adjusted)" << endl;
	cerr << "         -voicecolor voice color : set color to voice. Color should be specified as an html color name or using the '#rrggbb[aa]' format" << endl;
	cerr << "                                   Voices are indexed from 1." << endl;
	cerr << "         -voicesautocolor        : enable voices auto color" << endl;
	cerr << "         -measurebars            : enable measure bars" << endl;
	cerr << "         -lines                  : enable duration lines" << endl;
    exit(1);
}

static void error(GuidoErrCode err)
{
    if (err != guidoNoErr) {
        cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
        exit(err);
    }
}

//---------------------------------------------------------------------------------------------
static void checkusage(int argc, char **argv)
{
    if (argc == 1)
        usage(argv[0]);

    for (int i = 1; i < argc - 1; i++) {
        if (!strcmp(argv[i], kOptions[kHelp]))
            usage(argv[0]);
        else if (*argv[i] == '-') {

            bool unknownOpt = true;
            for (int n = 1; (n < kMaxOpt) && unknownOpt; n++) {
                if (!strcmp (argv[i], kOptions[n]))
                    unknownOpt = false;
            }

            if (unknownOpt) usage(argv[0]);
		}
    }
}

//---------------------------------------------------------------------------------------------
static const char* getInputFile(int argc, char *argv[])
{
	const char * file = argv[argc-1];		// input file is the last arg
	if (*file == '-') usage(argv[0]);
	return file;
}

//---------------------------------------------------------------------------------------------
static GuidoDate ldateopt(int argc, char **argv, const char* opt, GuidoDate defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp (argv[i], opt)) {
			i++;

			if (i >= argc)
                usage(argv[0]);
			else {
				int n,d;

				if (sscanf(argv[i], "%d/%d", &n, &d) == 2) {
					GuidoDate ret = {n, d};
					return ret;
				}
                else if (sscanf(argv[i], "%d", &n) == 1) {
					GuidoDate ret = {n, 1};
					return ret;
				}
                else
                    usage(argv[0]);
			}
		}
	}

	return defaultvalue;
}

//---------------------------------------------------------------------------------------------
static int lintopt(int argc, char **argv, const char* opt, int defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt)) {
			i++;
			if (i >= argc)
                usage(argv[0]);
			else
				return atoi(argv[i]);
		}
	}

	return defaultvalue;
}

//---------------------------------------------------------------------------------------------
static void lvoicecolor(int argc, char **argv, const char* opt, vector<pair<int,string> >& out)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt)) {
			if (i >= argc-3) usage (argv[0]);	// not enough args to color option
			int voice = atoi(argv[i+1]);
			if (!voice) usage (argv[0]);		// invalid voice number (should start at 1)
			string color = argv[i+2];
			out.push_back (make_pair(voice, color));
		}
	}
}

//---------------------------------------------------------------------------------------------
static bool lnoargopt(int argc, char **argv, const char* opt)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt))
			return true;
	}
	return false;
}

inline bool checkrange(char c, char low, char high) { return (c >= low) && (c <= high); }
inline bool checkhexa(char c) { return checkrange(c, '0', '9') || checkrange(c, 'a', 'f') || checkrange(c, 'A', 'F'); }
static int  hex2dec(char c)   {
	if (checkrange(c, '0', '9')) return c - '0';
	if (checkrange(c, 'a', 'f')) return 10 + c - 'a';
	return 10 + c - 'A';
}
//---------------------------------------------------------------------------------------------
static bool decodehexa (const char* ptr, int& v)
{
	char c1 = *ptr++;
	char c2 = *ptr++;
	if (!checkhexa (c1) || !checkhexa (c2))
		return false;
	v = (hex2dec(c1) << 4) + hex2dec(c2);
	return true;
}
//---------------------------------------------------------------------------------------------
static bool decodecolor (const char* ptr, int& r, int& g, int& b, int& a)
{
	size_t n = strlen(ptr);
	if ((n != 6) && (n != 8)) return false;
	if (n >= 6) {
		if (!decodehexa (ptr++, r)) return false;
		ptr++;
		if (!decodehexa (ptr++, g)) return false;
		ptr++;
		if (!decodehexa (ptr++, b)) return false;
		ptr++;
		if (n == 8) {
			if (!decodehexa (ptr++, a)) return false;
		}
		else a = 255;
	}
	return true;
}

//---------------------------------------------------------------------------------------------
static void processvoicecolors (RProportional* rp, const vector<pair<int,string> >& c)
{
	vector<pair<int,string> >::const_iterator i = c.begin();
	while (i != c.end()) {
		int v = i->first;
		const string& color = i->second;
		if (color[0] == '#') {
			int r, g, b, a;
			if (decodecolor (&color.c_str()[1], r, g, b, a)) {
				GuidoErrCode err = GuidoRProportionalSetRGBColorToVoice (rp, v, r, g, b, a);
				if (err != guidoNoErr)
					cerr << "can't set color " << color << " to voice " << v << endl;
			}
			else cerr << "invalid color " << color << endl;
		}
		else {
			GuidoErrCode err = GuidoRProportionalSetHtmlColorToVoice (rp, v, color.c_str());
			if (err != guidoNoErr)
				cerr << "can't set color " << color << " to voice " << v << endl;
		}
		i++;
	}
}

//---------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
 	SVGSystem sys;
	VGDevice *dev = sys.CreateDisplayDevice();
//    SVGDevice dev(cout, &sys, 0);
	
	checkusage(argc, argv);
	
	GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit(&gd);

    const char* fileName = getInputFile(argc, argv);

	int  w               = lintopt (argc, argv, kOptions[kWidth],           kDefaultWidth);
	int  h               = lintopt (argc, argv, kOptions[kHeight],          kDefaultHeight);
    int  minPitch        = lintopt (argc, argv, kOptions[kMinPitch],        kDefaultMinPitch);
    int  maxPitch        = lintopt (argc, argv, kOptions[kMaxPitch],        kDefaultMaxPitch);
    bool voicesAutoColor = lnoargopt (argc, argv, kOptions[kVoicesAutoColor]);
    bool measureBars     = lnoargopt (argc, argv, kOptions[kMeasureBars]);
    bool drawline        = lnoargopt (argc, argv, kOptions[kLines]);
	vector<pair<int,string> > colors;
	lvoicecolor (argc, argv, kOptions[kVoiceColor], colors);

	GuidoDate defDate = {0, 0};
	GuidoDate start   = ldateopt(argc, argv, kOptions[kStart], defDate);
	GuidoDate end     = ldateopt(argc, argv, kOptions[kEnd],   defDate);

	GuidoParser *parser = GuidoOpenParser();
	ARHandler    arh    = GuidoFile2AR(parser, fileName);

    GuidoErrCode err;
	if (arh) {
        RProportional *rprop = GuidoAR2RProportional (arh);
		
        /**** LIMITS ****/
        err = GuidoRProportionalSetLimits(rprop, start, end, minPitch, maxPitch);
        error(err);

        /**** VOICES COLOR ****/
        err = GuidoRProportionalEnableAutoVoicesColoration(rprop, voicesAutoColor);
        error(err);
		processvoicecolors (rprop, colors);

        /**** MEASURE BARS ****/
        err = GuidoRProportionalEnableMeasureBars(rprop, measureBars);
        error(err);

        /**** Duration lines ****/
        err = GuidoRProportionalDrawDurationLines(rprop, drawline);
        error(err);

        /**** DRAW ****/
		dev->NotifySize(w, h);
		dev->BeginDraw();
		dev->SelectPenColor(VGColor(0, 0, 0));
		dev->SelectFillColor(VGColor(0, 0, 0));
        err = GuidoRProportionalOnDraw(rprop, w, h, dev);
		dev->EndDraw();
        error(err);

        GuidoDestroyRProportional(rprop);
		GuidoFreeAR(arh);
	}
	else {
		int line, col;
		const char *msg;
        err = GuidoParserGetErrorCode(parser, line, col, &msg);
		cerr << "parse error line " << line << " col " << col << " : " << msg << endl;
	}

	GuidoCloseParser(parser);
    return 0;
}
