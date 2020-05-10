
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#ifdef WIN32
#include "GSystemWin32.h"
GSystemWin32 gSys(0,0);
#elif __APPLE__
#include "GSystemOSX.h"
GSystemOSX gSys (0,0);
#else
#include "CairoSystem.h"
CairoSystem gSys(0);
#endif

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"
#include "GUIDOScoreMapAdapter.h"
#include "VGDevice.h"

using namespace std;

#define kSize	500

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
}

static void print (TimeSegment from, TimeSegment to)
{
	cout << "[" << from.first << ", " << from.second << "]";
	cout << " ";
	cout << "[" << to.first << ", " << to.second << "]";
	cout << endl;
}

//------------------------------------------------------------------------------
// time mapping collector abstract class definition
class TimeMapPrinter : public TimeMapCollector
{
	public:
		virtual ~TimeMapPrinter() {}
		virtual void Time2TimeMap( const TimeSegment& from, const TimeSegment& to ) { print (from, to); }
//                        cout << "[" << from.first << ", " << from.second << "]";
//                        cout << " ";
//                        cout << "[" << to.first << ", " << to.second << "]";
//                        cout << endl;
//		}
};


int main(int argc, char **argv)
{
	VGDevice* dev = gSys.CreateMemoryDevice(kSize, kSize);
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit (&gd);

	for (int i=1; i < argc; i++) {
		GuidoErrCode err;
		ARHandler arh;
        GuidoParser *parser = GuidoOpenParser();

        std::ifstream ifs(argv[i], ios::in);
        if (!ifs)
            return 0;

        std::stringstream streamBuffer;
        streamBuffer << ifs.rdbuf();
        ifs.close();

        arh = GuidoString2AR(parser, streamBuffer.str().c_str());
		if (arh) {
#if 0
			TimeMapPrinter mp;
			err = GuidoGetTimeMap(arh, mp);
			if (err != guidoNoErr) error (err);
#else
			GuidoScoreMapAdapter guido;
			TTime2TimeMap map = guido.getTime2TimeMap(arh);
			for (auto elt: map)
				print (elt.first, elt.second);

#endif
			GuidoFreeAR (arh);
		}
		else {
			int line, col;
			err = GuidoParserGetErrorCode (parser, line, col, 0);
			error (err);
		}
        GuidoCloseParser(parser);
	}
	delete dev;
	return 0;
}


