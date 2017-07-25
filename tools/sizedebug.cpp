

#include <string>
#include <math.h>

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"
#include "SVGSystem.h"
#include "VGDevice.h"

using namespace std;

#define kSize	500

const float InfiniteFloat = powf(2.0, 25);

//------------------------------------------------------------
static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit (err);
}

//------------------------------------------------------------
static FloatRect getMapSize(const Time2GraphicMap& map) {
	FloatRect out;
	for (int i=0; i<map.size(); i++) {
		FloatRect r = map[i].second;
		if (r.right > out.right) out.right = r.right;
		if (r.bottom > out.bottom) out.bottom = r.bottom;
	}
	return out;
}

//------------------------------------------------------------
static GRHandler AR2GR (ARHandler ar) {
	GRHandler gr;
#if 0
	GuidoErrCode err =  GuidoAR2GR (ar, 0 , &gr);
	if (err != guidoNoErr) error (err);
#else
	GuidoGrParameters params;
	params.layoutSettings.force = 750;        // default was 750
	params.layoutSettings.spring = 1.12f;      // default was 1.1f
	params.layoutSettings.systemsDistribution = kNeverDistrib;
	params.layoutSettings.systemsDistance = 320; // adjusted until the score was beautiful
	params.layoutSettings.neighborhoodSpacing = 0;
	params.layoutSettings.optimalPageFill = 0;
	params.layoutSettings.checkLyricsCollisions = false;
	params.pageFormat.width = GuidoInches2Unit(kSize / 72.0);
	params.pageFormat.height = GuidoInches2Unit(InfiniteFloat / 72.0);
	params.pageFormat.marginbottom = 0;
	params.pageFormat.margintop = 0;
	params.pageFormat.marginleft = 0;
	params.pageFormat.marginright = 0;
	GuidoGetDefaultLayoutSettings(&params.layoutSettings);
	GuidoSetDefaultPageFormat(&params.pageFormat);

	gr = GuidoAR2GRParameterized (ar, &params);
	if (!gr) error (guidoErrActionFailed);
#endif
	return gr;
}

//------------------------------------------------------------
int main(int argc, char **argv)
{
	SVGSystem sys;
	VGDevice* dev = sys.CreateMemoryDevice(kSize, kSize);
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit (&gd);
	
	GuidoParser *parser = GuidoOpenParser();
	for (int i=1; i < argc; i++) {
		string file = argv[i];
		cout << file << ": " << endl;
		ARHandler arh = GuidoFile2AR (parser, file.c_str());
		if (arh) {
			GRHandler gr;
			for (int n=0; n < 15; n++) {
				Time2GraphicMap map;
				GRHandler gr = AR2GR (arh);
				GuidoErrCode err = GuidoGetSystemMap(gr, 1, kSize, InfiniteFloat, map);
				if (err != guidoNoErr) error (err);
				FloatRect r = getMapSize (map);
				cout << n << "\t: " << r << endl;
				GuidoFreeGR (gr);
			}
			GuidoFreeAR (arh);
		}
		else {
			int line, col;
			error (GuidoParserGetErrorCode (parser, line, col, 0));
		}
	}
	GuidoCloseParser(parser);
	delete dev;
	return 0;
}
