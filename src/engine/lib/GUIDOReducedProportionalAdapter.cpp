/*
  GUIDO Library
  Copyright (C) 2015	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <sstream>
#include "GUIDOReducedProportionalAdapter.h"
#include "SVGDevice.h"
#include "SVGSystem.h"
#ifdef CANVASSYSTEM
#include "canvassystem.h"
#include "canvasdevice.h"
#endif


GUIDOReducedProportionalAdapter::GUIDOReducedProportionalAdapter() {}
GUIDOReducedProportionalAdapter::~GUIDOReducedProportionalAdapter() {}

RProportional * ar2RProportional(ARHandler arh)
{
	return GuidoAR2RProportional (arh);
}

RProportional * midi2RProportional(const std::string &midiFileName)
{
	return GuidoMidi2RProportional (midiFileName.c_str());
}

GuidoErrCode GUIDOReducedProportionalAdapter::destroyRProportional(RProportional *pr)
{
	return GuidoDestroyRProportional (pr);
}

GuidoErrCode GUIDOReducedProportionalAdapter::setLimits(RProportional *pr, GuidoDate start, GuidoDate end, int lowpitch, int highpitch)
{
	return GuidoRProportionalSetLimits (pr, start, end, lowpitch, highpitch);
}

GuidoErrCode GUIDOReducedProportionalAdapter::drawDurationLines(RProportional *pr, bool enabled)
{
	return GuidoRProportionalDrawDurationLines (pr, enabled);
}

GuidoErrCode GUIDOReducedProportionalAdapter::enableAutoVoicesColoration(RProportional *pr, bool enabled)
{
	return GuidoRProportionalEnableAutoVoicesColoration (pr, enabled);
}

GuidoErrCode GUIDOReducedProportionalAdapter::setRGBColorToVoice(RProportional *pr, int voiceNum, int r, int g, int b, int a)
{
	return GuidoRProportionalSetRGBColorToVoice (pr, voiceNum, r, g, b, a);
}

GuidoErrCode GUIDOReducedProportionalAdapter::setHtmlColorToVoice(RProportional *pr, int voiceNum, const char* color)
{
	GuidoRProportionalSetHtmlColorToVoice (pr, voiceNum, color);
}

GuidoErrCode GUIDOReducedProportionalAdapter::removeColorToVoice(RProportional *pr, int voiceNum)
{
	GuidoRProportionalRemoveColorToVoice (pr, voiceNum);
}

GuidoErrCode GUIDOReducedProportionalAdapter::enableMeasureBars(RProportional *pr, bool enabled)
{
	return GuidoRProportionalEnableMeasureBars(pr, enabled);
}

Time2GraphicMap GUIDOReducedProportionalAdapter::getMap(RProportional *pr, int width, int height)
{
	Time2GraphicMap outMap;
	GuidoRProportionalGetMap(pr, width, height, outMap);
	return outMap;
}

GuidoErrCode GUIDOReducedProportionalAdapter::onDraw(RProportional *pr, int width, int height, VGDevice* dev)
{
	return GuidoRProportionalOnDraw (pr, width, height, dev);
}

std::string GUIDOReducedProportionalAdapter::svgExport(RProportional *pr, int width, int height)
{
	std::stringstream out;
	SVGSystem sys(0);
	SVGDevice dev (out, &sys, 0);
	dev.BeginDraw();
	dev.SelectPenColor(VGColor(100, 100, 100));
	dev.SelectFillColor(VGColor(0, 0, 0));
	GuidoRProportionalOnDraw(pr, width, height, &dev);
	dev.EndDraw();
	return out.str();
}


#ifdef CANVASSYSTEM
GuidoErrCode GUIDOReducedProportionalAdapter::javascriptExport(RProportional *pr, int width, int height)
{
	CanvasSystem sys(0);
	CanvasDevice dev(&sys);
	return GuidoRProportionalOnDraw(pr, width, height, &dev);
}
#endif

