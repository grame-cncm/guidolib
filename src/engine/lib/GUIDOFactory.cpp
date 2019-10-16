/*
  GUIDO Library
  Copyright (C) 2003, 2004  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#define __UseOldGUIDOFactory__

#include "ARFactory.h"
#include "ARMusic.h"
#include "ARMusicalTag.h"	// for template instanciation
#include "ARMusicalVoice.h"	// for template instanciation

#include "GRMusic.h"
#include "GUIDOEngine.h"
#include "GUIDOInternal.h"
#include "GUIDOFactory.h"


static ARFactory * getFactory( ARFactoryHandler in )
{
	return static_cast<ARFactory *>(in);
}

extern "C" {

GUIDOAPI GuidoErrCode GuidoFactoryOpen( ARFactoryHandler * outFactory )
{
	*outFactory = new ARFactory;
	return guidoNoErr;
}

GUIDOAPI void GuidoFactoryClose( ARFactoryHandler inFactory )
{
	delete getFactory( inFactory );
}

GUIDOAPI GuidoErrCode GuidoFactoryOpenMusic( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->createMusic();
	return guidoNoErr;
}

GUIDOAPI ARHandler GuidoFactoryCloseMusic( ARFactoryHandler inFactory )
{
	// - Extracts our new music abstract representation object.
	ARMusic * theMusic = getFactory( inFactory )->getMusic();
		// At this point, gCurArMusic = our new music.

	// - Put it in the global list.
	ARHandler outHandleAR = guido_RegisterARMusic( theMusic );

	 // theMusic->setName( ... );	// TODO

	return outHandleAR;
}

GUIDOAPI GuidoErrCode GuidoFactoryOpenVoice( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->createVoice();
	return guidoNoErr;
}

GUIDOAPI GuidoErrCode GuidoFactoryCloseVoice( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->addVoice();
	return guidoNoErr;
}

// ============================================================================
// 		Chords
// ============================================================================
// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryOpenChord( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->createChord();
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryCloseChord( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->addChord();
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryInsertCommata( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->initChordNote();
	return guidoNoErr;
}

// ============================================================================
// 		Events
// ============================================================================
// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryOpenEvent( ARFactoryHandler inFactory, const char * inEventName )
{
	getFactory( inFactory )->createEvent( inEventName );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryCloseEvent( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->addEvent();
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryAddSharp( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->addSharp();
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryAddFlat( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->addFlat();
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactorySetEventDots( ARFactoryHandler inFactory, int pointCount )
{
	getFactory( inFactory )->setPoints( pointCount );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactorySetEventAccidentals( ARFactoryHandler inFactory, int inValue )
{
	getFactory( inFactory )->setAccidentals( inValue );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactorySetOctave( ARFactoryHandler inFactory, int inValue )
{
	getFactory( inFactory )->setRegister( inValue );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactorySetDuration( ARFactoryHandler inFactory, int numerator, int denominator )
{
	getFactory( inFactory )->setNumerator( numerator );
	getFactory( inFactory )->setDenominator( denominator );
	return guidoNoErr;
}


// ============================================================================
// 		Tags
// ============================================================================

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryCloseTag( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->addTag();
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryAddTagParameterString( ARFactoryHandler inFactory, const char * inParam )
{
	getFactory( inFactory )->addTagParameter( inParam );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryAddTagParameterInt( ARFactoryHandler inFactory, int inParam )
{
	getFactory( inFactory )->addTagParameter( inParam );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryAddTagParameterFloat( ARFactoryHandler inFactory, double inParam )
{
	getFactory( inFactory )->addTagParameter( inParam );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactorySetParameterName( ARFactoryHandler inFactory, const char * inName )
{
	getFactory( inFactory )->setParameterName( inName );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactorySetParameterUnit( ARFactoryHandler inFactory, const char * inUnit )
{
	getFactory( inFactory )->setUnit( inUnit );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryOpenTag( ARFactoryHandler inFactory, const char * name, long tagID )
{
	getFactory( inFactory )->createTag( name, tagID );
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryOpenRangeTag( ARFactoryHandler inFactory, const char * name, long tagID )
{
	ARFactory * factory = getFactory( inFactory );
	factory->createTag( name, tagID );
	factory->tagRange();
	return guidoNoErr;
}

// ----------------------------------------------------------------------------
GUIDOAPI GuidoErrCode GuidoFactoryEndTag( ARFactoryHandler inFactory )
{
	getFactory( inFactory )->endTag();
	return guidoNoErr;
}


	// was GuidoFactoryCreateTag
	#ifdef __UseOldGUIDOFactory__
	GUIDOAPI GuidoErrCode GuidoFactoryCreateTag( ARFactoryHandler inFactory, const char * inTagName, long inTagID )
	{
		getFactory( inFactory )->createTag( inTagName, inTagID );
		return guidoNoErr;
	}

	// ----------------------------------------------------------------------------
	GUIDOAPI GuidoErrCode GuidoFactoryIsRangeTag(ARFactoryHandler inFactory )
	{
		getFactory( inFactory )->tagRange();
		return guidoNoErr;
	}
	#endif //  __UseOldGUIDOFactory__


} // extern "C" 

