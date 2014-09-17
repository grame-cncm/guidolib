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
#include "GuidoReducedProportional.h"
#include "midifile.h"

#include "GUIDOPianoRollAPI.h"


// ==========================================================================
// - Guido Piano Roll API
// ==========================================================================

// ------------------------------------------------------------------------
GUIDOAPI(GuidoPianoRoll *) GuidoCreatePianoRoll(/*REM: TMP */bool forReducedProportional)
{
    GuidoPianoRoll *newPianoRoll;

    if (forReducedProportional)
        newPianoRoll = (GuidoPianoRoll *) new GuidoReducedProportional();
    else
        newPianoRoll = new GuidoPianoRoll();

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
GUIDOAPI(GuidoErrCode) GuidoSetMidiToPianoRoll(GuidoPianoRoll *pr, const char *midiFileName)
{
#ifdef MIDIEXPORT
    if (!pr || !midiFileName)
        return guidoErrBadParameter;

    MIDIFile mf;
    
    if (!mf.Open(midiFileName, MidiFileRead))
		return guidoErrFileAccess;

    pr->setMidiFile(midiFileName);

	return guidoNoErr;
#else
    return guidoErrActionFailed;
#endif
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

    if (start.denom < 0 || end.denom < 0 || (start.denom == 0 && start.num != 0) || (end.denom == 0 && end.num != 0))
        return guidoErrBadParameter;
    
    if (start.denom != 0 && end.denom != 0) {
        float startTime = (float) start.num / start.denom;
        float endTime   = (float) end.num / end.denom;

        if (startTime > endTime)
            return guidoErrBadParameter;
        else
            pr->setLimitDates(start, end);
    }
    else
        pr->setLimitDates(start, end);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoSetPianoRollPitchLimits(GuidoPianoRoll *pr, int minPitch, int maxPitch)
{
    if (!pr || minPitch < -1 || maxPitch < -1 || minPitch > 127 || maxPitch > 127)
        return guidoErrBadParameter;

    if (minPitch != -1 && maxPitch != -1 && minPitch > (maxPitch - 11))
        return guidoErrBadParameter;

    pr->setPitchRange(minPitch, maxPitch);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollEnableDurationLines(GuidoPianoRoll *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;

    pr->enableDurationLines(enabled);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollEnableKeyboard(GuidoPianoRoll *pr, bool enabled)
{
    if (!pr || dynamic_cast<GuidoReducedProportional *>(pr))
        return guidoErrBadParameter;

    pr->enableKeyboard(enabled);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollEnableRandomVoicesColor(GuidoPianoRoll *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;

    pr->enableRandomVoicesColor(enabled);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollSetColorToVoice(GuidoPianoRoll *pr, int voiceNum, int r, int g, int b, int a)
{
    if (!pr)
        return guidoErrBadParameter;

    if (voiceNum < 1
        || r < 0 || r > 255
        || g < 0 || g > 255
        || b < 0 || b > 255
        || a < 0 || a > 255)
        return guidoErrBadParameter;

    pr->setColorToVoice(voiceNum, r, g, b, a);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoGetPianoRollRenderingFromAR(GuidoPianoRoll *pr, VGDevice *dev)
{
    if (!pr || !dev)
        return guidoErrBadParameter;

    if (!pr->ownsARMusic())
        return guidoErrInvalidHandle;

    pr->getRenderingFromAR(dev);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoGetPianoRollRenderingFromMidi(GuidoPianoRoll *pr, VGDevice *dev)
{
    if (!pr || !dev)
        return guidoErrBadParameter;

    if (!pr->ownsMidi())
        return guidoErrInvalidHandle;

    pr->getRenderingFromMidi(dev);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoGetRProportionalRenderingFromAR(GuidoPianoRoll *pr, VGDevice *dev)
{
    if (!pr || !dev)
        return guidoErrBadParameter;

    if (!pr->ownsARMusic())
        return guidoErrInvalidHandle;

    pr->getRenderingFromAR(dev);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoGetRProportionalRenderingFromMidi(GuidoPianoRoll *pr, VGDevice *dev)
{
    if (!pr || !dev)
        return guidoErrBadParameter;

    if (!pr->ownsMidi())
        return guidoErrInvalidHandle;

    pr->getRenderingFromMidi(dev);

	return guidoNoErr;
}