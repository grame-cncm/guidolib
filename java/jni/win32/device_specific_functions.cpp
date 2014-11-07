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

# include "GSystemWin32.h"
# include "GSystemWin32GDIPlus.h"

extern VGSystem * gSystem;

namespace device_specific {
	VGDevice * getInitDevice(bool antialiased) {
		extern bool gAntiAliasing;
		gAntiAliasing = antialiased;
		if (antialiased) {
			GSystemWin32GDIPlus::Start();
			gSystem = new GSystemWin32GDIPlus (0, 0);
		}
		else {
			gSystem = new GSystemWin32 (0, 0);
		}
		return gSystem->CreateMemoryDevice(20,20);
	}

	void clear (VGDevice * dev, int w, int h)
	{
	}

	void bimap_copy_gdiplus (VGDevice * dev, jint* dstBitmap, int w, int h)
	{
		GDeviceWin32GDIPlus *gdi = (GDeviceWin32GDIPlus*)dev;
		Bitmap * bitmap = gdi->GetBitmap();
		Gdiplus::Rect rect(0, 0, w, h);
		BitmapData bdata;
		bdata.Reserved = 0;
		bitmap->LockBits( &rect, ImageLockModeRead, PixelFormat32bppPARGB, &bdata);
		int* data = (int*)bdata.Scan0;
		for (int i=0, n=w*h; i<n; i++) {
			*dstBitmap++ = *data++;
		}
		bitmap->UnlockBits(&bdata);
	}

	void bimap_copy_gdi (VGDevice * dev, jint* dstBitmap, int w, int h)
	{
		GDeviceWin32* windev = (GDeviceWin32*)dev;

		BITMAPINFO infos;
		infos.bmiHeader.biSize = w*h*4;
		infos.bmiHeader.biWidth = w;
		infos.bmiHeader.biHeight = h;
		infos.bmiHeader.biPlanes = 1;
		infos.bmiHeader.biBitCount = 32;
		infos.bmiHeader.biCompression = BI_RGB;
		infos.bmiHeader.biSizeImage = 0;
		infos.bmiHeader.biXPelsPerMeter = infos.bmiHeader.biYPelsPerMeter = (100 / 2.54) * 72;
		infos.bmiHeader.biClrUsed = 0;
		infos.bmiHeader.biClrImportant = 0;
		int n = GetDIBits(windev->getHDC(), windev->getBitmap(), 0, h, dstBitmap, &infos, DIB_RGB_COLORS);
	}

	void bimap_copy (VGDevice * dev, jint* dstBitmap, int w, int h)
	{
		if (gAntiAliasing) bimap_copy_gdiplus (dev, dstBitmap, w, h);
		else bimap_copy_gdi(dev, dstBitmap, w, h);
	}

} // end namespace
