
#ifndef WIN32
#include <libgen.h>
#endif
#include <iostream>
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

#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"
#include "VGDevice.h"

using namespace std;

#define kSize	500

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
}

static void draw(GRHandler gr, int page, VGDevice* dev)
{
	GuidoOnDrawDesc desc;
	desc.handle = gr;
	desc.hdc = dev;
	desc.page = page;
	desc.updateRegion.erase = true;
	desc.scrollx = desc.scrolly = 0;
	desc.sizex = desc.sizey = kSize;
	GuidoResizePageToMusic( gr );
	GuidoOnDraw (&desc);
}

int main(int argc, char **argv)
{
	VGDevice* dev = gSys.CreateMemoryDevice(kSize, kSize);
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit (&gd);

	for (int i=1; i < argc; i++) {
		cerr << "# get voice mapping for " << argv[i] << endl;
		GuidoErrCode err;
		ARHandler arh;
		err = GuidoParseFile (argv[i], &arh);
		if (err == guidoNoErr) {
			GRHandler grh;
			err = GuidoAR2GR (arh, 0, &grh);
			if (err != guidoNoErr) error (err);
			else {
				int n = GuidoGetPageCount (grh);
				int voices = GuidoCountVoices (arh);
				for (int page =1; page <= n; page++) {
					draw (grh, page, dev);
					cout << "# page " << page << "/" << n << endl;
					cout << "######################### " << endl;
					for (int v =1; v <= voices; v++) {
						Time2GraphicMap map;
						err = GuidoGetVoiceMap( grh, page, kSize, kSize, v, map);
						if (err != guidoNoErr) error (err);
						else if (map.size()) {
							cout << "# voice " << v << "/" << voices << endl;
							cout << "######################### " << endl;
							for (Time2GraphicMap::const_iterator m = map.begin(); m != map.end(); m++)
								cout << m->first << " " << m->second << endl;
						}
					}
				}
				GuidoFreeGR (grh);
			}
			GuidoFreeAR (arh);
		}
		else error (err);
	}
	delete dev;
	return 0;
}


