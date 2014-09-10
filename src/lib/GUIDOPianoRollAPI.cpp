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

/** \file GUIDOPianoRollAPI.cpp

	The Guido pianoroll API
 */

#include "GUIDOInternal.h"
#include "GuidoPianoRoll.h"

#include "GUIDOPianoRollAPI.h"


// ==========================================================================
// - Guido Piano Roll API
// ==========================================================================

// ------------------------------------------------------------------------
GUIDOAPI(GuidoPianoRoll *) GuidoCreatePianoRoll()
{
    GuidoPianoRoll *newPianoRoll = new GuidoPianoRoll();

	return newPianoRoll;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoDestroyPianoRoll(GuidoPianoRoll *pr)
{
    if (!pr)
        return guidoErrBadParameter;

    delete pr;

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoSetARToPianoRoll(GuidoPianoRoll *pr, ARHandler arh)
{
    if (!pr || !arh)
        return guidoErrBadParameter;

    ARMusic *arMusic = arh->armusic;

    if (!arMusic)
        return guidoErrInvalidHandle;

    pr->setARMusic(arMusic);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoSetPianoRollCanvasDimensions(GuidoPianoRoll *pr, int width, int height)
{
    if (!pr || width < -1 || height < -1)
        return guidoErrBadParameter;

    pr->setCanvasDimensions(width, height);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoSetPianoRollTimeLimits(GuidoPianoRoll *pr, GuidoDate start, GuidoDate end)
{
    if (!pr)
        return guidoErrBadParameter;

    if (start.denom <= 0 || end.denom <= 0 || start.num < -1 || end.num < -1)
        return guidoErrBadParameter;
    
    float startTime = (float) start.num / start.denom;
    float endTime   = (float) end.num / end.denom;

    if ((start.num != -1 || start.denom != 1) && (end.num != -1 || end.denom != 1) && startTime > endTime)
        return guidoErrBadParameter;
    else
        pr->setLimitDates(start, end);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoSetPianoRollPitchLimits(GuidoPianoRoll *pr, int minPitch, int maxPitch)
{
    if (!pr || minPitch < -1 || maxPitch < -1 || minPitch > 127 || maxPitch > 127)
        return guidoErrBadParameter;

    if (minPitch != -1 && maxPitch != -1 && minPitch > maxPitch)
        return guidoErrBadParameter;

    pr->setPitchRange(minPitch, maxPitch);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoGetPianoRollRendering(GuidoPianoRoll *pr, VGDevice* dev)
{
    if (!pr || !dev)
        return guidoErrBadParameter;

    if (!pr->ownsARMusic())
        return guidoErrInvalidHandle;

    pr->getRendering(dev);

	return guidoNoErr;
}