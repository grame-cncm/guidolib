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

#include <cmath>

#include "GUIDOInternal.h"
#include "PianoRoll.h"
#include "PianoRollTrajectory.h"
#include "HtmlColors.h"
#ifdef MIDIEXPORT
#include "midifile.h"
#endif

#include "GUIDOPianoRoll.h"


// ==========================================================================
// - Guido Piano Roll API
// ==========================================================================

// ------------------------------------------------------------------------
GUIDOAPI(PianoRoll *) GuidoAR2PianoRoll(PianoRollType type, ARHandler arh)
{
    if (!arh)
        return NULL;

    ARMusic *arMusic = arh->armusic;

    if (!arMusic)
        return NULL;

    PianoRoll *newPianoRoll = 0;

    if (type == kSimplePianoRoll)
        newPianoRoll = new PianoRoll(arMusic);
    else if (type == kTrajectoryPianoRoll)
        newPianoRoll = (PianoRollTrajectory *) new PianoRollTrajectory(arMusic);

	return newPianoRoll;
}

// ------------------------------------------------------------------------
GUIDOAPI(PianoRoll *) GuidoMidi2PianoRoll(PianoRollType type, const char *midiFileName)
{
#ifdef MIDIEXPORT
    if (!midiFileName)
        return NULL;

    MIDIFile mf;

    if (!mf.Open(midiFileName, MidiFileRead))
        return NULL;

    PianoRoll *newPianoRoll = 0;

    if (type == kSimplePianoRoll)
        newPianoRoll = new PianoRoll(midiFileName);
    else if (type == kTrajectoryPianoRoll)
        newPianoRoll = (PianoRollTrajectory *) new PianoRollTrajectory(midiFileName);

    return newPianoRoll;
#else
    return NULL;
#endif
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoDestroyPianoRoll(PianoRoll *pr)
{
    if (!pr)
        return guidoErrBadParameter;

    delete pr;

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollSetLimits(PianoRoll *pr, LimitParams limitParams)
{
    if (!pr)
        return guidoErrBadParameter;

    /**** SET TIME LIMITS ****/

    GuidoDate start = limitParams.startDate;
    GuidoDate end   = limitParams.endDate;

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

    /*************************/

    /**** SET PITCH LIMIT ****/

    int minPitch = limitParams.lowPitch;
    int maxPitch = limitParams.highPitch;

    if (minPitch < -1 || maxPitch < -1 || minPitch > 127 || maxPitch > 127)
        return guidoErrBadParameter;

    if (minPitch != -1 && maxPitch != -1 && minPitch > (maxPitch - 11))
        return guidoErrBadParameter;

    pr->setPitchRange(minPitch, maxPitch);

    /*************************/

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollEnableKeyboard(PianoRoll *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;

    pr->enableKeyboard(enabled);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollGetKeyboardWidth(PianoRoll *pr, int height, float &keyboardWidth)
{
    if (!pr || height < -1 || height == 0)
        return guidoErrBadParameter;

    keyboardWidth = pr->getKeyboardWidth(height);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollEnableAutoVoicesColoration(PianoRoll *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;

    pr->enableRandomVoicesColor(enabled);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollSetRGBColorToVoice(PianoRoll *pr, int voiceNum, int r, int g, int b, int a)
{
    if (!pr || voiceNum < 1)
        return guidoErrBadParameter;

    if (r < 0 || r > 255
        || g < 0 || g > 255
        || b < 0 || b > 255
        || a < 0 || a > 255)
        return guidoErrBadParameter;

    pr->setColorToVoice(voiceNum, r, g, b, a);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollSetHtmlColorToVoice(PianoRoll *pr, int voiceNum, long color)
{
    if (!pr || voiceNum < 1)
        return guidoErrBadParameter;

    unsigned char colref[4] = {0, 0, 0, 255};

	HtmlColor::get(color, colref);

    pr->setColorToVoice(voiceNum, colref[0], colref[1], colref[2], colref[3]);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollEnableMeasureBars(PianoRoll *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;

    pr->enableMeasureBars(enabled);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollSetPitchLinesDisplayMode(PianoRoll *pr, int mode)
{
    if (!pr || mode < -1 || mode > pow(2.0, 12) - 1)
        return guidoErrBadParameter;

    pr->setPitchLinesDisplayMode(mode);

	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollGetMap(PianoRoll *pr, int width, int height, Time2GraphicMap &outmap)
{
    if (!pr || width < -1 || height < -1 || width == 0 || height == 0)
        return guidoErrBadParameter;

    int autoWidth  = (width  == -1 ? kDefaultWidth  : width);
    int autoHeight = (height == -1 ? kDefaultHeight : height);

    if (pr->getKeyboardWidth(autoHeight) > autoWidth)
        return guidoErrBadParameter; // Keyboard takes to much place (create a new error code ?)
    else
        pr->getMap(autoWidth, autoHeight, outmap);

    return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoPianoRollOnDraw(PianoRoll *pr, int width, int height, VGDevice *dev)
{
    if (!pr || !dev || width < -1 || height < -1 || width == 0 || height == 0)
        return guidoErrBadParameter;

    int autoWidth  = (width  == -1 ? kDefaultWidth : width);
    int autoHeight = (height == -1 ? kDefaultHeight : height);

    if (pr->getKeyboardWidth(autoHeight) > autoWidth)
        return guidoErrBadParameter; // Keyboard takes to much place (create a new error code ?)
    else
        pr->onDraw(autoWidth, autoHeight, dev);

	return guidoNoErr;
}