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
#include "GUIDOScoreMap.h"
#include "Colors.h"
#include "SVGDevice.h"
#include "SVGSystem.h"

using namespace std;

const int  kDefaultWidth           = -1;
const int  kDefaultHeight          = -1;
const int  kDefaultMinPitch        = -1;
const int  kDefaultMaxPitch        = -1;
const bool kDefaultKeyboard        = false;
const bool kDefaultVoicesAutoColor = false;
const bool kDefaultMeasureBars     = false;
const int  kDefaultPitchLines      = kAutoLines;

const PianoRollType kDefaultPianoRoll = kSimplePianoRoll;

const char* kOptions[] = { "-help", "-pianoroll", "-width", "-height", "-start", "-end", "-minpitch", "-maxpitch", "-keyboard", "-voicesautocolor", "-measurebars", "-pitchlines" };
enum { kHelp, kPianoRoll, kWidth, kHeight, kStart, kEnd, kMinPitch, kMaxPitch, kKeyboard, kVoicesAutoColor, kMeasureBars, kPitchLines, kMaxOpt };

static void usage(char* name)
{
#ifndef WIN32
	const char* tool = basename (name);
#else
	const char* tool = name;
#endif
	cerr << "usage: " << tool << "[options] gmnfile" << endl;
	cerr << "options: -pianoroll       string : set the pianoroll type (default is " << kDefaultPianoRoll << ")" << endl;
	cerr << "                                        simple" << endl;
    cerr << "                                        trajectory" << endl;
	cerr << "         -width           value  : set the output width (default is " << kDefaultWidth << ")" << endl;
	cerr << "         -height          value  : set the output height (default is " << kDefaultHeight << ")" << endl;
	cerr << "         -start           date   : set time zone start (default is 0/0 -> start time is automatically adjusted)" << endl;
	cerr << "         -end             date   : set time zone end (default is 0/0 -> end time is automatically adjusted)" << endl;
    cerr << "         -minpitch        value  : set minimum midi pitch (default is " << kDefaultMinPitch << " -> min pitch is automatically adjusted)" << endl;
	cerr << "         -maxpitch        value  : set maximum midi pitch (default is " << kDefaultMaxPitch << " -> max pitch is automatically adjusted)" << endl;
	cerr << "         -keyboard        bool   : set if keyboard will be displayed (default is " << kDefaultKeyboard << ")" << endl;
	cerr << "         -voicesautocolor bool   : set if voices will be auto colored (default is " << kDefaultVoicesAutoColor << ")" << endl;
	cerr << "         -measurebars     bool   : set if measure bars will be enabled (default is " << kDefaultMeasureBars << ")" << endl;
	cerr << "         -pitchlines      string : set pitch lines display mode (default is " << kDefaultPitchLines << ")" << endl;
	cerr << "                                        automatic" << endl;
    cerr << "                                        noline" << endl;

    exit(1);
}

static void error(GuidoErrCode err)
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

static PianoRollType lPianoRollTypeopt(int argc, char **argv, const char* opt, PianoRollType defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt)) {
			i++;

			if (i >= argc)
                usage(argv[0]);
			else {
                if (!strcmp(argv[i], "simple"))
                    return kSimplePianoRoll;
                else if (!strcmp(argv[i], "trajectory"))
                    return kTrajectoryPianoRoll;
                else
                    return kSimplePianoRoll;
            }
		}
	}

	return defaultvalue;
}

static int lPitchLinesopt(int argc, char **argv, const char* opt, int defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt)) {
			i++;

			if (i >= argc)
                usage(argv[0]);
			else {
                if (!strcmp(argv[i], "automatic"))
                    return kAutoLines;
                else if (!strcmp(argv[i], "noline"))
                    return kNoLine;
                /* Complete with other choices*/
            }
		}
	}

	return defaultvalue;
}

int main(int argc, char **argv)
{
 	SVGSystem sys;
    SVGDevice dev(cout, &sys, 0, 0);
	
	checkusage(argc, argv);
	
	GuidoInitDesc gd = { &dev, 0, 0, 0 };
    GuidoInit(&gd);

    const char* fileName = getInputFile(argc, argv);

	int  w               = lintopt       (argc, argv, kOptions[kWidth],           kDefaultWidth);
	int  h               = lintopt       (argc, argv, kOptions[kHeight],          kDefaultHeight);
    int  minPitch        = lintopt       (argc, argv, kOptions[kMinPitch],        kDefaultMinPitch);
    int  maxPitch        = lintopt       (argc, argv, kOptions[kMaxPitch],        kDefaultMaxPitch);
    bool keyboard        = lboolopt      (argc, argv, kOptions[kKeyboard],        kDefaultKeyboard);
    bool voicesAutoColor = lboolopt      (argc, argv, kOptions[kVoicesAutoColor], kDefaultVoicesAutoColor);
    bool measureBars     = lboolopt      (argc, argv, kOptions[kMeasureBars],     kDefaultMeasureBars);
    int  pitchLines      = lPitchLinesopt(argc, argv, kOptions[kPitchLines],      kDefaultPitchLines);

    PianoRollType         pianoRollType = lPianoRollTypeopt(argc, argv, kOptions[kPianoRoll],  kDefaultPianoRoll);

	GuidoDate defDate = {0, 0};
	GuidoDate start   = ldateopt(argc, argv, kOptions[kStart], defDate);
	GuidoDate end     = ldateopt(argc, argv, kOptions[kEnd],   defDate);

	GuidoParser *parser = GuidoOpenParser();
	ARHandler    arh    = GuidoFile2AR(parser, fileName);

    GuidoErrCode err;

	if (arh) {
        PianoRoll *pianoRoll = GuidoAR2PianoRoll(pianoRollType, arh);
        
        LimitParams limitParams;
        limitParams.startDate = start;
        limitParams.endDate   = end;
        limitParams.lowPitch  = minPitch;
        limitParams.highPitch = maxPitch;

        /**** LIMITS ****/
        err = GuidoPianoRollSetLimits(pianoRoll, limitParams);
        error(err);
        /*********************/

        /**** KEYBOARD ****/
        err = GuidoPianoRollEnableKeyboard(pianoRoll, keyboard);
        error(err);

        float keyboardWidth;
        err = GuidoPianoRollGetKeyboardWidth(pianoRoll, h, keyboardWidth);
        error(err);
        /******************/

        /**** VOICES COLOR ****/
        err = GuidoPianoRollEnableAutoVoicesColoration(pianoRoll, voicesAutoColor);
        error(err);
        
        //err = GuidoPianoRollSetHtmlColorToVoice(pianoRoll, 1, kaqua);
        //error(err);
        
        //err = GuidoPianoRollSetHtmlColorToVoice(pianoRoll, 2, knavy);
        //error(err);
        
        //err = GuidoPianoRollSetRGBColorToVoice(pianoRoll, 3, 0, 0, 255, 100);
        //error(err);
        /**********************/

        /**** MEASURE BARS ****/
        err = GuidoPianoRollEnableMeasureBars(pianoRoll, measureBars);
        error(err);
        /**********************/

        /**** PITCH LINES ****/
        err = GuidoPianoRollSetPitchLinesDisplayMode(pianoRoll, pitchLines);
        error(err);
        /*********************/

        /**** MAP ****/
        Time2GraphicMap map;
        err = GuidoPianoRollGetMap(pianoRoll, w, h, map);
        error(err);
        /*************/

        /**** DRAW ****/
        err = GuidoPianoRollOnDraw(pianoRoll, w, h, &dev);
        error(err);
        /**************/

        GuidoDestroyPianoRoll(pianoRoll);

		GuidoFreeAR(arh);
	}
	else {
		int line, col;
		
        err = GuidoParserGetErrorCode(parser, line, col, 0); // REM: l'erreur n'est pas récuperée si l'arh a simplement mal été instancié
		error(err);
	}

	GuidoCloseParser(parser);

    return 0;
}
