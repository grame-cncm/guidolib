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


#include "GUIDOEngine.h"
#include "../device_specific_functions.h"
#include "VGSystem.h"
#include "VGDevice.h"
#include "VGColor.h"

# include "CairoSystem.h"


extern VGSystem * gSystem;

namespace device_specific {
	VGDevice * getInitDevice(bool antialiased) {
		gSystem = new CairoSystem (0);
		return gSystem->CreateMemoryDevice(20,20);
	}

	static void clear (VGDevice * dev, int w, int h)
	{
		cairo_t * cr = (cairo_t *)dev->GetNativeContext();
		cairo_surface_t * surface = cairo_get_group_target (cr);
		int*  data = (int *)cairo_image_surface_get_data (surface);
		for (int i=0, n=w*h; i<n; i++) *data++ = 0;
	}
	static void bimap_copy (VGDevice * dev, jint* dstBitmap, int w, int h)
	{
		cairo_t * cr = (cairo_t *)dev->GetNativeContext();
		cairo_surface_t * surface = cairo_get_group_target (cr);
		int*  data = (int *)cairo_image_surface_get_data (surface);
		for (int i=0, n=w*h; i<n; i++) {
			*dstBitmap++ = *data++;
		}
	}



	int getBitmap (jint* dstBitmap, int w, int h, GuidoOnDrawDesc& desc, const VGColor& color)
	{
		VGDevice * dev = gSystem->CreateMemoryDevice (w, h);
		desc.hdc = dev;
		dev->SelectFillColor(color);
		dev->SelectPenColor(color);
		dev->SetFontColor (color);

		GuidoErrCode err = GuidoOnDraw (&desc);
		if (err == guidoNoErr)
			bimap_copy (dev, dstBitmap, w, h);
		else fprintf (stderr, "GuidoOnDraw error %d: %s\n", err, GuidoGetErrorString (err));
		delete dev;
		return err;

	}

}