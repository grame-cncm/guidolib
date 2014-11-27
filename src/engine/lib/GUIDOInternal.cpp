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

/** \file GUIDOInternal.cpp

	Internal Guido functions.

*/

#include <iostream>
#include <sstream>

#include "defines.h"		// for GuidoWarn and GuidoTrace
#include "guido.h"			
#include "ARPageFormat.h"
#include "GUIDOInternal.h"

#include "GRStaffManager.h"
#include "FontManager.h"

using namespace std;

extern bool gInited;			// GuidoInit() Flag
extern ARPageFormat * gARPageFormat;

// --------------------------------------------------------------------------
// Prototypes are in "defines.h"
void GuidoWarn( const char * inMessage, const char * info )	
{ 
	cerr << "Guido Warning: " << inMessage << " " << (info ? info : "") << endl;
}

// --------------------------------------------------------------------------
void GuidoTrace( const char * inMessage )	
{ 
	cerr << "Guido Trace: " << inMessage <<endl; 
}

// --------------------------------------------------------------------------
void guido_applySettings( const GuidoLayoutSettings * settings )
{
	if( settings == 0 ) return;

	// - System distance
	GRStaffManager::sDefaultSystemDistance = settings->systemsDistance;
	
	// - System distribution on/off
	GRStaffManager::sSystemDistribution = settings->systemsDistribution;

	// - System distribution maximum distance
	GRStaffManager::sSystemDistribLimit = settings->systemsDistribLimit;
	
	// - Space opt force
	GRSpaceForceFunction2::setOptForce( settings->force );
	
	// - Spring parameter
	GRSpring::funcpar = settings->spring;

	// - NeighborhoodSpacing on/off
	GRStaffManager::sNeedSpecialSpacing = (settings->neighborhoodSpacing == 1 ? true : false);

	// - Optimal page fill on/off
	GRStaffManager::sOptPageFill = (settings->optimalPageFill == 1 ? true : false);

	// - Optimal page fill on/off
	GRStaffManager::sPropRender = (settings->proportionalRenderingForceMultiplicator < 0.0001 ? 0 : settings->proportionalRenderingForceMultiplicator);
}

// --------------------------------------------------------------------------
void guido_deinit()
{
	FontManager::ReleaseAllFonts();
	gInited = false;
}

// --------------------------------------------------------------------------
void guido_cleanup()
{
	delete gARPageFormat;
	gARPageFormat = 0;
}

// --------------------------------------------------------------------------
/** \brief Adds an ARMusic (abstract representation) object to the global list,
	then returns the corresponding guido AR handler.

	The reference counter of the AR is still sets to zero at this point, to 
	maintain the behaviour of previous version of the library.
		
	result > 0: guido AR handle
	result < 0: error code
			
*/
ARHandler guido_RegisterARMusic( ARMusic * inMusic )
{
	if( inMusic == 0 ) return 0;
	
	// - Create and setup a new NodeARMusic node
	NodeAR * newNode = new NodeAR;
	newNode->refCount = 1;
	newNode->armusic = inMusic;
	
	return newNode; // returns the brand new guido AR handle.
}

// --------------------------------------------------------------------------
/** \brief Adds an GRMusic (graphical representation) object to the global list,
	then returns the corresponding guido handle.

	The reference counter of the AR object is left untouched.

	\return a Guido opaque handle to the newly resistered GR structure.
			
*/
GRHandler guido_RegisterGRMusic( GRMusic * inMusic, ARHandler inHandleAR )
{
	if( inMusic == 0 ) return 0;
	if( inHandleAR == 0 ) return 0;
	
	// - Create and setup a new NodeGr
	NodeGR * newNode = new NodeGR;
	if (newNode == 0) 
		return 0;

	newNode->page = 1;	// page is obsolete. Just here for compatibility.
	newNode->grmusic = inMusic;
	newNode->arHandle = inHandleAR;

	return newNode;  // returns the brand new guido handle.
}

// --------------------------------------------------------------------------
void AddGGSOutput(const char * s)
{
	// we do nothing ....
}

void AddGuidoOutput(const char * s)
{
}

//} // extern "C" 
