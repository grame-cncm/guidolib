

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#ifndef WIN32
#include <libgen.h>
#endif
#include <stdlib.h>

#include "SVGSystem.h"
#include "SVGDevice.h"
#include "SVGFont.h"

#include "GUIDOParse.h"
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"

using namespace std;


static void usage (char* name)
{
#ifndef WIN32
	cerr << "usage: " << basename (name) << " <gmn file> pageNum map" << endl;
#else
	cerr << "usage: " << name << " <gmn file> pageNum map" << endl;
#endif
	cerr << "       where map = page|system|slice|staff|bar|events" << endl;
	exit(1);
}

static void error (GuidoErrCode err)
{
	cerr << "error #" << err << ": " << GuidoGetErrorString (err) << endl;
	exit(1);
}

static GuidoElementSelector mapname2selector (string name) 
{
	if (name == "page")		return kGuidoPage;
	if (name == "system")	return kGuidoSystem;
	if (name == "slice")	return kGuidoSystemSlice;
	if (name == "staff")	return kGuidoStaff;
	if (name == "bar")		return kGuidoBar;
	if (name == "events")	return kGuidoEvent;
	return kGuidoScoreElementEnd;
}

int main(int argc, char **argv)
{
	if (argc != 4) usage(argv[0]);
	const char* filename = argv[1];
	int page = atoi(argv[2]);
	if (page <= 0) usage(argv[0]);
	GuidoElementSelector sel = mapname2selector (argv[3]);
	if (sel == kGuidoScoreElementEnd) usage(argv[0]);


	SVGSystem sys;
	VGDevice *dev = sys.CreateDisplayDevice();
    GuidoInitDesc gd = { dev, 0, 0, 0 };
    GuidoInit(&gd);                   // Initialise the Guido Engine first

	GuidoErrCode err;
	ARHandler arh;

    GuidoParser *parser = GuidoOpenParser();

    std::ifstream ifs(filename, ios::in);
    if (!ifs)
        return 0;

    std::stringstream streamBuffer;
    streamBuffer << ifs.rdbuf();
    ifs.close();

    arh = GuidoString2AR(parser, streamBuffer.str().c_str());
    if (!arh) {
		int line, col;
		err = GuidoParserGetErrorCode (parser, line, col, 0);
		error (err);
		return 1;
	}

	GRHandler grh;
    err = GuidoAR2GR (arh, 0, &grh);

	if (err != guidoNoErr)
        error(err);

	vector<MapElement> map;
	err = GuidoGetSVGMap( grh, page, sel, map);
	if (err != guidoNoErr) error (err);
	
	for (size_t i = 0; i < map.size(); i++)
    {
		FloatRect r = map[i].first;
		TimeSegment time = map[i].second.time();
		cout << "( [" << int(r.left) << "," << int(r.right) << "[ [" << int(r.top) << "," << int(r.bottom) << "[ ) "
		<< " ( [" << time.first << ", " << time.second << "[ )" << endl;
	}

    GuidoCloseParser(parser);

	return 0;
}


