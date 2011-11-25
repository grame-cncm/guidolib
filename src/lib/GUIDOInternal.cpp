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

/** \file GUIDOInternal.cpp

	Internal Guido functions.

*/

#include <iostream>

#include "defines.h"		// for GuidoWarn and GuidoTrace
#include "guido.h"			
#include "ARPageFormat.h"
#include "GUIDOInternal.h"

#include "GRStaffManager.h"
#include "FontManager.h"

using namespace std;

//extern "C" {
	int gd_parse(const char * filename, int mode);
//  }

extern bool gInited;			// GuidoInit() Flag
extern ARPageFormat * gARPageFormat;
extern NodeGR * gFirstNodeGR;
extern NodeAR * gFirstNodeAR;

// - Guido Internal globals
int gIdGeneratorAR = 0;
int gIdGeneratorGR = 0;

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
}

// --------------------------------------------------------------------------
void guido_deinit()
{
	gd_exit();	// for the GUIDO-Parser
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
