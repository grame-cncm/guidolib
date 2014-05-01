/*
	GUIDO Library - JNI Interface
	Copyright (C) 2010 Sony CSL Paris
	All rights reserved.
	Author D. Fober

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


#ifndef __device_specific_functions__
#define __device_specific_functions__

#include "GUIDOEngine.h"
#include "VGDevice.h"
#include <jni.h>

namespace device_specific {
  VGDevice * getInitDevice(bool antialiased);
  void clear (VGDevice * dev, int w, int h);
  void bimap_copy (VGDevice * dev, jint* dstBitmap, int w, int h);
}

#endif