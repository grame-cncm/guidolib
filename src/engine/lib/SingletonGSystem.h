#ifndef SingletonGSystem_H
#define SingletonGSystem_H

/*
  GUIDO Library
  Copyright (C) 2003, 2006	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "VGSystem.h"
#include "GUIDOExport.h"

// --------------------------------------------------------------
// 		VGSystem class
// --------------------------------------------------------------

class_export SingletonGSystem
{
	public:

		// static method for singleton like behaviour
		static void SetInstance(VGSystem* instance);
		static VGDevice* SCreateDisplayDevice();
		static VGDevice* SCreateMemoryDevice(int inWidth, int inHeight);
		static VGDevice* SCreateMemoryDebugDevice(int inWidth, int inHeight, const char* outfile=0);
		static VGDevice* SCreateMemoryDevice(const char * inPath);
		static VGDevice* SCreatePrinterDevice();
		static const VGFont* SCreateVGFont(const char * faceName, int size, int properties);
		static VGDevice* SCreateAntiAliasedMemoryDevice(int inWidth, int inHeight);
		static VGSystem* mInstance;
};

#endif
