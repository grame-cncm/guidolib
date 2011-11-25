/*
	GUIDO Library
	Copyright (C) 2003, 2006	Grame
	
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
