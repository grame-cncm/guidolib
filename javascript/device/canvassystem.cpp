/*
  GUIDO Library
  Copyright (C) 2014	Grame

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
	// This system is used when we can't calculate font metrics
	// A arbitrary text font metrics (Times) is used for text font
	if(strcmp("Guido2", faceName) == 0) {
		return new GuidoFont(faceName, size, properties);
	} else {
		return new TimesFont(faceName, size, properties);
	}
}
