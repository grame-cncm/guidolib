/*
  GUIDO Library
  Copyright(C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "canvassystem.h"

using namespace std;
//______________________________________________________________________________
CanvasSystem::CanvasSystem(const char* guidofontspec) :
	fGuidoFontSpec(guidofontspec)
{
}

//______________________________________________________________________________
VGDevice* CanvasSystem::CreateDisplayDevice(std::ostream& outstream)
{
	return new CanvasDevice(this);
}

//______________________________________________________________________________
VGDevice* CanvasSystem::CreateDisplayDevice()
{
	return new CanvasDevice(this);
}

//______________________________________________________________________________
VGDevice* CanvasSystem::CreateMemoryDevice(int w, int h)
{
	CanvasDevice* device = new CanvasDevice(this);
	device->NotifySize(w, h);
	return device;
}

//______________________________________________________________________________
VGDevice* CanvasSystem::CreateMemoryDevice(const char * inPath)
{
	CanvasDevice* device = new CanvasDevice(this);
	return device;
}

//______________________________________________________________________________
VGDevice* CanvasSystem::CreatePrinterDevice() { return 0; }

//______________________________________________________________________________
VGDevice* CanvasSystem::CreateAntiAliasedMemoryDevice(int w, int h)
{
	CanvasDevice* device = new CanvasDevice(this);
	device->NotifySize(w, h);
	return device;
}

//______________________________________________________________________________
const VGFont* CanvasSystem::CreateVGFont(const char * faceName, int size, int properties) const
{
	if(strcmp("Guido2", faceName) == 0 && size == GuidoFont::kFontSize) {
		return new GuidoFont(faceName, properties);
	} else {
		return new SVGFont (faceName, size, properties, "", fGuidoFontSpec ? fGuidoFontSpec : "");
	}
}
