#ifndef __GUIDOInternal__
#define __GUIDOInternal__

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003, 2004  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 */

#include "GUIDOEngine.h"
#include "VGSystem.h"
 

// 06/2004: Version 1.3.1: big changes in the API, new entry points. 

class GRMusic;
class ARMusic;
class VGDevice;
class GuidoFeedback;

struct NodeAR
{
	int refCount;			// Number of GRMusic using this NodeAR.
	ARMusic * armusic;
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

	// if 1: staff creates rods  [default]
	// if 2: GRVoice creates rods
	int gStaffVoiceRods;

	// if 1: elements are sorted in the staff  [default]
	// if 0: elements are not sorted in the staff
	int gSortElements;

	// if 1: barlines are introduced automatically  [default]
	// if 0: no automatic barlines.
	int gAutoBarlines;

	// if 1: notes are tied together  [default]
	// if 0: a merge-tag is used instead
	// this parameter is important
	// for automatic barline-introcution
	int gAutoTies;

	// TODO: what are these for?
	VGDevice * gDevice;
	GuidoFeedback * gFeedback;
	
};

// these are the default global settings
extern GuidoGlobalSettings gGlobalSettings;
extern int gBoundingBoxesMap;
#define kPageBBColor		VGColor( 0, 0, 255 )
#define kSystemBBColor		VGColor( 0, 0, 0 )
#define kSystemSliceBBColor	VGColor( 0, 0, 200 )
#define kStaffBBColor		VGColor( 255, 0, 0 )
#define kMeasureBBColor		VGColor( 0, 255, 0 )
#define kEventBBColor		VGColor( 255, 0, 0 )


// - Internal function declarations
void 			guido_deinit();
void 			guido_cleanup();

ARHandler		guido_RegisterARMusic( ARMusic * inMusic );
GRHandler		guido_RegisterGRMusic( GRMusic * inMusic, ARHandler inHandleAR );

void			guido_applySettings( const GuidoLayoutSettings * settings );

#endif
