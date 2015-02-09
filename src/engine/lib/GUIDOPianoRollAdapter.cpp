/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#include <sstream>
#include "GUIDOPianoRollAdapter.h"
#include "SVGDevice.h"
#include "SVGSystem.h"
#ifdef CANVASSYSTEM
#include "canvassystem.h"
#include "canvasdevice.h"
#endif

GUIDOPianoRollAdapter::GUIDOPianoRollAdapter()
{
}

GUIDOPianoRollAdapter::~GUIDOPianoRollAdapter()
{
}

PianoRoll * GUIDOPianoRollAdapter::ar2PianoRoll(PianoRollType type, ARHandler arh)
{
	return GuidoAR2PianoRoll(type, arh);
}

PianoRoll * GUIDOPianoRollAdapter::midi2PianoRoll(PianoRollType type, const std::string &midiFileName)
{
	return GuidoMidi2PianoRoll(type, midiFileName.c_str());
}

GuidoErrCode GUIDOPianoRollAdapter::destroyPianoRoll(PianoRoll *pr)
{
	return GuidoDestroyPianoRoll(pr);
}

GuidoErrCode GUIDOPianoRollAdapter::setLimits(PianoRoll *pr, LimitParams &limitParams)
{
	return GuidoPianoRollSetLimits(pr, limitParams);
}

GuidoErrCode GUIDOPianoRollAdapter::enableKeyboard(PianoRoll *pr, bool enabled)
{
	return GuidoPianoRollEnableKeyboard(pr, enabled);
}

float GUIDOPianoRollAdapter::getKeyboardWidth(PianoRoll *pr, int height)
{
	float width;
	if(GuidoPianoRollGetKeyboardWidth(pr, height, width) == guidoNoErr) {
		return width;
	}
	return 0;
}

GuidoErrCode GUIDOPianoRollAdapter::enableAutoVoicesColoration(PianoRoll *pr, bool enabled)
{
	return GuidoPianoRollEnableAutoVoicesColoration(pr, enabled);
}

GuidoErrCode GUIDOPianoRollAdapter::setRGBColorToVoice(PianoRoll *pr, int voiceNum, int r, int g, int b, int a)
{
	return GuidoPianoRollSetRGBColorToVoice(pr, voiceNum, r, g, b, a);
}

GuidoErrCode GUIDOPianoRollAdapter::setHtmlColorToVoice(PianoRoll *pr, int voiceNum, long color)
{
	return GuidoPianoRollSetHtmlColorToVoice(pr, voiceNum, color);
}

GuidoErrCode GUIDOPianoRollAdapter::enableMeasureBars(PianoRoll *pr, bool enabled)
{
	return GuidoPianoRollEnableMeasureBars(pr, enabled);
}

GuidoErrCode GUIDOPianoRollAdapter::setPitchLinesDisplayMode(PianoRoll *pr, int mode)
{
	return GuidoPianoRollSetPitchLinesDisplayMode(pr, mode);
}

Time2GraphicMap GUIDOPianoRollAdapter::getMap(PianoRoll *pr, int width, int height)
{
	Time2GraphicMap outMap;
	GuidoPianoRollGetMap(pr, width, height, outMap);
	return outMap;
}

GuidoErrCode GUIDOPianoRollAdapter::onDraw(PianoRoll *pr, int width, int height, VGDevice* dev)
{
	return GuidoPianoRollOnDraw(pr, width, height, dev);
}

std::string GUIDOPianoRollAdapter::svgExport(PianoRoll *pr, int width, int height)
{
	std::stringstream out;
	SVGSystem sys(0);
	SVGDevice dev (out, &sys, 0);
	GuidoPianoRollOnDraw(pr, width, height, &dev);
	return out.str();
}

#ifdef CANVASSYSTEM
GuidoErrCode GUIDOPianoRollAdapter::javascriptExport(PianoRoll *pr, int width, int height)
{
	CanvasSystem sys(0);
	CanvasDevice dev(&sys);
	return GuidoPianoRollOnDraw(pr, width, height, &dev);
}
#endif
