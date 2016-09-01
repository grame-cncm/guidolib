
#include <iostream>

#include "GUIDOEngine.h"
#include "GDevicePostScript.h"

using namespace std;
// ---------------------------------------------------------------------------
//	This example of code is intended to show the minimum steps to read a
//  GMN file and to display it.
//  Take care that for simplicity, this example doesn't check return codes
// ---------------------------------------------------------------------------
void DrawGMNFile (const char * filename) 
{
    ARHandler arh;
    // use a GDevicePostScript with a 72 dpi resolution
    GDevicePostScript dev ((int)(21*72/2.54), (int)(29.7*72/2.54), "outfile.eps", "", "");
    
    // declare a data structure for engine initialisation
    // we'll make use of the default graphic device (embedded in the library)
    // Guido font is guido2 and text font is times
    GuidoInitDesc gd = { &dev, 0, "guido2", "Times" };
    GuidoOnDrawDesc desc;              // declare a data structure for drawing        
    GuidoInit (&gd);                   // Initialise the Guido Engine first
    
    // and parse the GMN file to get a GGR handle directly stored in the drawing struct
    GuidoParseFile (filename, &arh);
    GuidoAR2GR (arh, 0, &desc.handle);
    desc.hdc = &dev;                    // we'll draw on the postscript device
    desc.page = 1;                      // draw the first page only
    desc.updateRegion.erase = true;     // and draw everything
	desc.scrollx = desc.scrolly = 0;    // from the upper left page corner
    desc.sizex = desc.sizey = 500;      // size of the drawing region
    
    GuidoOnDraw (&desc);
}

int main(int argc, char **argv)
{
	const char * file = argv[1];
	DrawGMNFile (file);
	return 0;
}
