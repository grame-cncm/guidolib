#ifndef SingletonGSystem_H
#define SingletonGSystem_H

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
