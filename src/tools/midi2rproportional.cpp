
#ifndef WIN32
#include <libgen.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#include <string.h>

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "GUIDOPianoRoll.h"
#include "SVGDevice.h"
#include "SVGSystem.h"

using namespace std;

const int kDefaultWidth         = 1024;
const int kDefaultHeight        = 400;
const int kDefaultMinPitch      = -1;
const int kDefaultMaxPitch      = -1;
const bool kDefaultDrawDurLines = false;
const bool kDefaultMeasureBars  = false;

const char* kOptions[] = { "-help", "-width", "-height", "-start", "-end", "-minpitch", "-maxpitch", "-measurebars", "-drawdurlines" };
enum { kHelp, kWidth, kHeight, kStart, kEnd, kMinPitch, kMaxPitch, kMeasureBars, kDrawDurLines, kMaxOpt };

static void usage (char* name)
{
#ifndef WIN32
	const char* tool = basename (name);
#else
	const char* tool = name;
#endif
	cerr << "usage: " << tool << " midifile [options] " << endl;
	cerr << "options: -width        value : set the output width (default is " << kDefaultWidth << ")" << endl;
	cerr << "         -height       value : set the output height (default is " << kDefaultHeight << ")" << endl;
	cerr << "         -start        date  : set time zone start (default is 0/0 -> start time is automatically adjusted)" << endl;
	cerr << "         -end          date  : set time zone end (default is 0/0 -> end time is automatically adjusted)" << endl;
    cerr << "         -minpitch     value : set minimum midi pitch (default is " << kDefaultMinPitch << " -> min pitch is automatically adjusted)" << endl;
	cerr << "         -maxpitch     value : set maximum midi pitch (default is " << kDefaultMaxPitch << " -> max pitch is automatically adjusted)" << endl;
	cerr << "         -measurebars  bool  : set if measure bars will be enabled (default is " << kDefaultMeasureBars << ")" << endl;
    cerr << "         -drawdurlines bool  : set if duration lines will be drawn (default is " << kDefaultDrawDurLines << ")" << endl;

	exit (1);
}

static void error (GuidoErrCode err)
{
	if (err != guidoNoErr) {
        cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;

        exit(err);
    }
}

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

            if (unknownOpt || i + 1 >= argc || *(argv[i + 1]) == '-')
                usage(argv[0]);
            else {
                i++;

                if (i >= argc - 1)
                    usage(argv[0]);
            }
        }
        else {
            if (i != argc - 2)
                usage(argv[0]);
        }
    }
}

static const char* getInputFile(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc - 1; i++) {
		if (*argv[i] == '-')
            i++;	// skip option value
	}

	return (i < argc) ? argv[i] : 0;
}

static GuidoDate ldateopt(int argc, char **argv, const char* opt, GuidoDate defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt)) {
			i++;

			if (i >= argc)
                usage(argv[0]);
			else {
				int n, d;

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

static bool lopt(int argc, char **argv, const char* opt)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt))
			return true;
	}

	return false;
}

static int lintopt(int argc, char **argv, const char* opt, int defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp (argv[i], opt)) {
			i++;

			if (i >= argc)
                usage(argv[0]);
			else
				return atoi(argv[i]);
		}
	}

	return defaultvalue;
}

static bool lboolopt(int argc, char **argv, const char* opt, bool defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt)) {
			i++;

			if (i >= argc)
                usage(argv[0]);
			else {
                if (!strcmp(argv[i], "true"))
                    return true;
                else
                    return false;
            }
		}
	}

	return defaultvalue;
}

int main(int argc, char **argv)
{
 	SVGSystem sys;
    SVGDevice dev(cout, &sys, 0, 0);
	
	checkusage (argc, argv);

    GuidoInitDesc gd = { &dev, 0, 0, 0 };
    GuidoInit(&gd);
	
    const char* fileName = getInputFile(argc, argv);

	int  w            = lintopt (argc, argv, kOptions[kWidth],        kDefaultWidth);
	int  h            = lintopt (argc, argv, kOptions[kHeight],       kDefaultHeight);
    int  minPitch     = lintopt (argc, argv, kOptions[kMinPitch],     kDefaultMinPitch);
    int  maxPitch     = lintopt (argc, argv, kOptions[kMaxPitch],     kDefaultMaxPitch);
    bool measureBars  = lboolopt(argc, argv, kOptions[kMeasureBars],  kDefaultMeasureBars);
	bool drawDurLines = lboolopt(argc, argv, kOptions[kDrawDurLines], kDefaultDrawDurLines);

	GuidoDate defDate = {0, 0};
	GuidoDate start   = ldateopt(argc, argv, kOptions[kStart], defDate);
	GuidoDate end     = ldateopt(argc, argv, kOptions[kEnd],   defDate);

    //PianoRoll *pianoRoll = GuidoMidi2PianoRoll(reducedProportional, fileName);
    
    //if (pianoRoll) {
        //GuidoErrCode err;

        ///**** LIMITS ****/
        //err = GuidoPianoRollSetLimits(pianoRoll, limitParams);
        //error(err);
        ///*********************/

        ///**** MEASURE BARS ****/
        //err = GuidoPianoRollEnableMeasureBars(pianoRoll, measureBars);
        //error(err);
        ///**********************/

        ///**** DURATION LINES ****/
        //err = GuidoPianoRollEnableDurationLines(pianoRoll, drawDurLines);
        //error(err);
        ///************************/
    
        ///**** DRAW ****/
        //err = GuidoPianoRollOnDraw(pianoRoll, w, h, &dev);
        //error(err);
        ///**************/

        //err = GuidoPianoRollOnDraw(pianoRoll, &dev);
        //error(err);

        //GuidoDestroyPianoRoll(pianoRoll);
    //}
    //else
        //cout << "Piano roll hasn't been correctly created" << endl;

    return 0;
}
