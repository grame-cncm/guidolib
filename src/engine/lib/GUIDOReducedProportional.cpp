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

#include "GUIDOPianoRoll.h"
#include "GUIDOReducedProportional.h"

#include "GUIDOInternal.h"
#include "RProportional.h"
#include "HtmlColors.h"

#ifdef MIDIEXPORT
#include "midifile.h"
#endif


// ------------------------------------------------------------------------
GUIDOAPI(RProportional *) GuidoAR2RProportional(ARHandler arh)
{
    if (!arh)
        return NULL;

    ARMusic *arMusic = arh->armusic;
    if (!arMusic)
        return NULL;

	return new RProportional(arMusic);
}

// ------------------------------------------------------------------------
GUIDOAPI(RProportional *) GuidoMidi2RProportional(const char *midiFileName)
{
#ifdef MIDIEXPORT
    if (!midiFileName)
        return NULL;

    MIDIFile mf;
    if (!mf.Open(midiFileName, MidiFileRead))
        return NULL;

    return new RProportional(midiFileName);
#else
    return NULL;
#endif
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoDestroyRProportional(RProportional *pr)
{
    if (!pr)
        return guidoErrBadParameter;

    delete pr;
	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoRProportionalSetLimits(RProportional *pr, GuidoDate start, GuidoDate end, int minPitch, int maxPitch)
{
	LimitParams p = { start, end, minPitch, maxPitch };
	return GuidoPianoRollSetLimits (pr, p);

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



    if (!start.denom || !end.denom )
        return guidoErrBadParameter;

	float startTime = (float) start.num / start.denom;
	float endTime   = (float) end.num / end.denom;
    if (startTime < 0  || endTime < startTime)
        return guidoErrBadParameter;

	pr->setLimitDates(start, end);

    /**** SET PITCH LIMIT ****/
    if (minPitch < -1 || maxPitch < -1 || minPitch > 127 || maxPitch > 127)
        return guidoErrBadParameter;

    if (minPitch != -1 && maxPitch != -1 && minPitch > (maxPitch - 11))
        return guidoErrBadParameter;

    pr->setPitchRange(minPitch, maxPitch);
	return guidoNoErr;
}


// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)      GuidoRProportionalDrawDurationLines(RProportional *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;
	pr->SetDrawDurationLines(enabled);
	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoRProportionalEnableAutoVoicesColoration(RProportional *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;

    pr->enableRandomVoicesColor(enabled);
	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoRProportionalSetRGBColorToVoice(RProportional *pr, int voiceNum, int r, int g, int b, int a)
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
GUIDOAPI(GuidoErrCode) GuidoRProportionalRemoveColorToVoice(RProportional *pr, int voiceNum)
{
	if(pr->removeColorToVoice(voiceNum))
		return guidoNoErr;
	return guidoErrBadParameter;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoRProportionalSetHtmlColorToVoice(RProportional *pr, int voiceNum, long color)
{
    if (!pr || voiceNum < 1)
        return guidoErrBadParameter;

    unsigned char colref[4] = {0, 0, 0, 255};
	HtmlColor::get(color, colref);
    pr->setColorToVoice(voiceNum, colref[0], colref[1], colref[2], colref[3]);
	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoRProportionalEnableMeasureBars(RProportional *pr, bool enabled)
{
    if (!pr)
        return guidoErrBadParameter;

    pr->enableMeasureBars(enabled);
	return guidoNoErr;
}

// ------------------------------------------------------------------------
GUIDOAPI(GuidoErrCode) GuidoRProportionalOnDraw(RProportional *pr, int width, int height, VGDevice *dev)
{
    if (!pr || !dev || width < -1 || height < -1 || width == 0 || height == 0)
        return guidoErrBadParameter;

    int w  = (width  == -1 ? kDefaultWidth : width);
    int h = (height == -1 ? kDefaultHeight : height);

    pr->onDraw(w, h, dev);
	return guidoNoErr;
}

