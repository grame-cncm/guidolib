
#ifndef WIN32
#include <libgen.h>
#include <unistd.h>
#include <stdio.h>
#endif

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "GUIDOPianoRollAPI.h"
#include "SVGDevice.h"
#include "SVGSystem.h"

using namespace std;

const int  kDefaultWidth           = 1024;
const int  kDefaultHeight          = 400;
const int  kDefaultMinPitch        = -1;
const int  kDefaultMaxPitch        = -1;
const bool kDefaultKeyboard        = false;
const bool kDefaultVoicesAutoColor = false;
const bool kDefaultMeasureBars     = false;

const PianoRollType         kDefaultPianoRoll  = simplePianoRoll;
const PitchLinesDisplayMode kDefaultPitchLines = automatic;

const char* kOptions[] = { "-help", "-o", "-pianoroll", "-width", "-height", "-start", "-end", "-minpitch", "-maxpitch", "-keyboard", "-voicesautocolor", "-measurebars", "-pitchlines" };
enum { kHelp, kOutput, kPianoRoll, kWidth, kHeight, kStart, kEnd, kMinPitch, kMaxPitch, kKeyboard, kVoicesAutoColor, kMeasureBars, kPitchLines, kMaxOpt };

static void usage(char* name)
{
#ifndef WIN32
	const char* tool = basename (name);
#else
	const char* tool = name;
#endif
	cerr << "usage: " << tool << " gmnfile [options] " << endl;
    cerr << "options: -o                      : set the output file (if not, output is standard output)" << endl;
	cerr << "         -pianoroll       string : set the pianoroll type (default is " << kDefaultPianoRoll << ")" << endl;
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
    cerr << "                                        oneline" << endl;
    cerr << "                                        twolines" << endl;
    cerr << "                                        diatonic" << endl;
    cerr << "                                        chromatic" << endl;
	system("PAUSE"); // REM: tmp
    exit(1);
}

static void error(GuidoErrCode err)
{
    if (err != guidoNoErr) {
        cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
        system("PAUSE"); // REM: tmp
        exit(err);
    }
}

static void checkusage(int argc, char **argv)
{
	if (argc == 1 || *(argv[1]) == '-')
        usage(argv[0]);
    else {
        for (int i = 1; i < argc; i++) {
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
            }
        }
    }
}

static const char* getInputFile(int argc, char *argv[])
{
	int i;

	for (i = 1; i < argc; i++) {
		if (*(argv[i]) != '-')
            break;
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
                    return simplePianoRoll;
                else if (!strcmp(argv[i], "trajectory"))
                    return trajectoryPianoRoll;
                else
                    return simplePianoRoll;
            }
		}
	}

	return defaultvalue;
}

static PitchLinesDisplayMode lPitchLinesopt(int argc, char **argv, const char* opt, PitchLinesDisplayMode defaultvalue)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], opt)) {
			i++;

			if (i >= argc)
                usage(argv[0]);
			else {
                if (!strcmp(argv[i], "automatic"))
                    return automatic;
                else if (!strcmp(argv[i], "noline"))
                    return no_line;
                else if (!strcmp(argv[i], "oneline"))
                    return one_line;
                else if (!strcmp(argv[i], "twolines"))
                    return two_lines;
                else if (!strcmp(argv[i], "diatonic"))
                    return diatonic;
                else if (!strcmp(argv[i], "chromatic"))
                    return chromatic;
                else
                    return automatic;
            }
		}
	}

	return defaultvalue;
}

static char *getOutputFileOpt(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-o")) {
			i++;

			if (i >= argc || *(argv[i]) == '-')
                usage(argv[0]);
			else
				return argv[i];
		}
	}

    return NULL;
}

int main(int argc, char **argv)
{
 	SVGSystem sys;
    
    std::streambuf *buf = std::cout.rdbuf();

    /**** Check output file ****/
    const char *outputFile = getOutputFileOpt(argc, argv);

    ofstream fileOutput;

    if (outputFile != NULL) {
        fileOutput = ofstream(outputFile, ios::out | ios::trunc);

        if (!fileOutput.fail())
            buf = fileOutput.rdbuf();
    }
    /***************************/

    std::ostream out(buf);

    SVGDevice dev(out, &sys, 0);
	
	checkusage(argc, argv);
	
	GuidoInitDesc gd = { &dev, 0, 0, 0 };
    GuidoInit(&gd);

    const char* fileName = getInputFile(argc, argv);

	int  w               = lintopt (argc, argv, kOptions[kWidth],           kDefaultWidth);
	int  h               = lintopt (argc, argv, kOptions[kHeight],          kDefaultHeight);
    int  minPitch        = lintopt (argc, argv, kOptions[kMinPitch],        kDefaultMinPitch);
    int  maxPitch        = lintopt (argc, argv, kOptions[kMaxPitch],        kDefaultMaxPitch);
    bool keyboard        = lboolopt(argc, argv, kOptions[kKeyboard],        kDefaultKeyboard);
    bool voicesAutoColor = lboolopt(argc, argv, kOptions[kVoicesAutoColor], kDefaultVoicesAutoColor);
    bool measureBars     = lboolopt(argc, argv, kOptions[kMeasureBars],     kDefaultMeasureBars);
    
    PianoRollType         pianoRollType = lPianoRollTypeopt(argc, argv, kOptions[kPianoRoll],  kDefaultPianoRoll);
    PitchLinesDisplayMode pitchLines    = lPitchLinesopt(argc, argv, kOptions[kPitchLines], kDefaultPitchLines);

	GuidoDate defDate = {0, 0};
	GuidoDate start   = ldateopt(argc, argv, kOptions[kStart], defDate);
	GuidoDate end     = ldateopt(argc, argv, kOptions[kEnd],   defDate);

	GuidoParser *parser = GuidoOpenParser();
	ARHandler    arh    = GuidoFile2AR(parser, fileName);

    GuidoErrCode err;

	if (arh) {
        GuidoPianoRoll *pianoRoll = GuidoCreatePianoRoll(pianoRollType);

        err = GuidoPianoRollSetAR(pianoRoll, arh);
        error(err);

        
        /**** SIZE ****/
        err = GuidoPianoRollSetCanvasDimensions(pianoRoll, w, h);
        error(err);

        int width, height;
        err = GuidoPianoRollGetSize(pianoRoll, width, height);
        error(err);
        /**************/
        
        /**** TIME LIMITS ****/
        err = GuidoPianoRollSetTimeLimits(pianoRoll, start, end);
        error(err);
        /*********************/

        /**** PITCH LIMITS ****/
        err = GuidoPianoRollSetPitchLimits(pianoRoll, minPitch, maxPitch);
        error(err);
        /**********************/

        /**** KEYBOARD ****/
        err = GuidoPianoRollEnableKeyboard(pianoRoll, keyboard);
        error(err);

        float keyboardWidth;
        err = GuidoPianoRollGetKeyboardWidth(pianoRoll, keyboardWidth);
        error(err);
        /******************/

        /**** VOICES COLOR ****/
        err = GuidoPianoRollEnableAutoVoicesColoration(pianoRoll, voicesAutoColor);
        error(err);
        
        //err = GuidoPianoRollSetColorToVoice(pianoRoll, 1, 255, 0, 0, 100);
        //error(err);
        
        //err = GuidoPianoRollSetColorToVoice(pianoRoll, 2, 0, 255, 0, 100);
        //error(err);
        
        //err = GuidoPianoRollSetColorToVoice(pianoRoll, 3, 0, 0, 255, 100);
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


        err = GuidoPianoRollGetRenderingFromAR(pianoRoll, &dev);
        error(err);

        GuidoDestroyPianoRoll(pianoRoll);

		GuidoFreeAR(arh);
	}
	else {
		int line, col;
		
        err = GuidoParserGetErrorCode(parser, line, col, 0); // REM: l'erreur n'est pas récupérée si l'arh a simplement mal été instancié
		error(err);
	}

	GuidoCloseParser(parser);

    return 0;
}