/*
  GUIDO Library
  Copyright (C) 2003-2006 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "DebugDevice.h"
#include "SingletonGSystem.h"
#include <assert.h>

VGSystem* SingletonGSystem::mInstance = 0;

void SingletonGSystem::SetInstance(VGSystem* instance)
{
	mInstance = instance;
}

#if 1

VGDevice* SingletonGSystem::SCreateDisplayDevice()
{
	assert(mInstance);
	return mInstance->CreateDisplayDevice();
}

VGDevice* SingletonGSystem::SCreateMemoryDevice(int inWidth, int inHeight)
{
	assert(mInstance);
	return mInstance->CreateMemoryDevice(inWidth, inHeight);
}

VGDevice* SingletonGSystem::SCreateAntiAliasedMemoryDevice(int inWidth, int inHeight)
{
	assert(mInstance);
	return mInstance->CreateAntiAliasedMemoryDevice(inWidth, inHeight);
}	

VGDevice* SingletonGSystem::SCreateMemoryDebugDevice(int inWidth, int inHeight, const char* outfile)
{
	assert(mInstance);
	return new DebugDevice(mInstance->CreateMemoryDevice(inWidth, inHeight), outfile);
}

VGDevice* SingletonGSystem::SCreateMemoryDevice(const char * inPath)
{
	assert(mInstance);
	return mInstance->CreateMemoryDevice(inPath);
}

VGDevice* SingletonGSystem::SCreatePrinterDevice()
{
	assert(mInstance);
	return mInstance->CreatePrinterDevice();
}

const VGFont* SingletonGSystem::SCreateVGFont(const char * faceName, int size, int properties)
{
	assert(mInstance);
	return mInstance->CreateVGFont(faceName, size, properties);
}

#endif
