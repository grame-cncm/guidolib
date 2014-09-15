
#ifndef WIN32
#include <libgen.h>
#include <unistd.h>
#include <stdio.h>
#endif

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "SVGDevice.h"
#include "SVGSystem.h"

using namespace std;

const int kDefaultWith	= 1024;
const int kDefaultHeight= 400;

const char* kOptions[] = { "-help", "-width", "-height", "-start", "-end", "-nodur" };
enum { kHelp, kWidth, kHeight, kStart, kEnd, kNoDur, kMaxOpt };

static void usage (char* name)
{
#ifndef WIN32
	const char* tool = basename (name);
#else
	const char* tool = name;
#endif
	cerr << "usage: " << tool << " midifile [options] " << endl;
	cerr << "options: -width value  : set the output width (default is " << kDefaultWith << ")" << endl;
	cerr << "         -height value : set the output height (default is " << kDefaultHeight << ")" << endl;
	cerr << "         -start date   : set time zone start" << endl;
	cerr << "         -end date     : set time zone end" << endl;
	cerr << "         -nodur		: don't draw duration lines" << endl;
	exit (1);
}

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit (err);
}

static void checkusage (int argc, char **argv)
{
	if (argc == 1) usage(argv[0]);
	for (int i=2; i< argc; i++) {
		if (!strcmp (argv[i], kOptions[kHelp])) usage(argv[0]);
		else if (*argv[i] == '-') {
			bool unknownOpt = true;
			for (int n=1; (n < kMaxOpt) && unknownOpt; n++) {
				if (!strcmp (argv[i], kOptions[n])) unknownOpt = false;
			}
			if (unknownOpt) usage (argv[0]);
		}
	}
}

static GuidoDate ldateopt (int argc, char **argv, const char* opt, GuidoDate defaultvalue)
{
	for (int i=2; i< argc; i++) {
		if (!strcmp (argv[i], opt)) {
			i++;
			if (i >= argc) usage(argv[0]);
			else {
				int n,d;
				if (sscanf(argv[i], "%d/%d", &n, &d) == 2) {
					GuidoDate ret = {n,d};
					return ret;
				}
			}
		}
	}
	return defaultvalue;
}

static bool lopt (int argc, char **argv, const char* opt)
{
	for (int i=2; i< argc; i++) {
		if (!strcmp (argv[i], opt))
			return true;
	}
	return false;
}

static int lintopt (int argc, char **argv, const char* opt, int defaultvalue)
{
	for (int i=2; i< argc; i++) {
		if (!strcmp (argv[i], opt)) {
			i++;
			if (i >= argc) usage(argv[0]);
			else {
				return atoi (argv[i]);
			}
		}
	}
	return defaultvalue;
}


int main(int argc, char **argv)
{
 	SVGSystem sys;
	SVGDevice dev (cout, &sys, 0);
	
	checkusage (argc, argv);
	
	int w = lintopt(argc, argv, kOptions[kWidth], kDefaultWith);
	int h = lintopt(argc, argv, kOptions[kHeight], kDefaultHeight);
	GuidoDate defDate = {0,1};
	GuidoDate start = ldateopt(argc, argv, kOptions[kStart], defDate);
	GuidoDate end = ldateopt(argc, argv, kOptions[kEnd], defDate);
	bool drawdur = lopt(argc, argv, kOptions[kNoDur]) ? false : true;
	dev.NotifySize (w, h);

	/*GuidoErrCode err = GuidoMIDI2RProportional (argv[1], w, h, start, end, drawdur, &dev);
	if (err) error (err);*/
	return 0;
}


