#ifndef __GUIDOInternal__
#define __GUIDOInternal__

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "GUIDOEngine.h"
#include "VGSystem.h"
 

// 06/2004: Version 1.3.1: big changes in the API, new entry points. 

class GRMusic;
class ARMusic;
class VGDevice;

struct NodeAR
{
    int refCount;			// Number of GRMusic using this NodeAR.
	ARMusic * armusic;
	GuidoLayoutSettings* fEngineSettings;
};

struct NodeGR
{
	int page;				// page is obsolete. Just here for compatibility.
	GRMusic * grmusic;
	NodeAR * arHandle;		// the corresponding AR handle
};

/** \brief Contains all global settings required by engine (moved from nview.h)

*/
struct GuidoGlobalSettings
{
	// if 1: springs and rods are displayed
	// if 0: no display of springs and rods. [default]
	int gDisplaySprings;

	// if 1: force is  displayed at end of system
	// if 0: no display of force. [default]
	int gDisplayForce;

	// if 1: barlines are introduced automatically  [default]
	// if 0: no automatic barlines.
	int gAutoBarlines;

	// TODO: what are these for?
	VGDevice * gDevice;
};

// these are the default global settings
extern GuidoGlobalSettings gGlobalSettings;
extern int gBoundingBoxesMap;
#define kPageBBColor		VGColor(   0,   0, 255 )
#define kSystemBBColor		VGColor(   0,   0,   0 )
#define kSystemSliceBBColor	VGColor(   0,   0, 200 )
#define kStaffBBColor		VGColor( 255,   0,   0 )
#define kMeasureBBColor		VGColor(   0, 255,   0 )
#define kEventBBColor		VGColor( 255,   0,   0 )


// - Internal function declarations
void 			guido_deinit();
void 			guido_cleanup();

ARHandler		guido_RegisterARMusic( ARMusic * inMusic );
GRHandler		guido_RegisterGRMusic( GRMusic * inMusic, ARHandler inHandleAR );

std::ostream & operator << ( std::ostream & os, const GuidoLayoutSettings* );

#endif
