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


#include "guidoengine_bitmap_paint.h"
#include "VGSystem.h"
#include "VGDevice.h"
#include "VGColor.h"

#ifdef WIN32
extern bool gAntiAliasing;
# include "GSystemWin32GDIPlus.h"
# include "GDeviceWin32GDIPlus.h"
# include "GDeviceWin32.h"

#elif __APPLE__
# include "GSystemOSX.h"
//static GSystemOSX sys (0,0);

#elif __linux__
# include "CairoSystem.h"
#else
# error "Unknown native system!"
#endif

extern VGSystem * gSystem;


#ifdef WIN32
static void bimap_copy_gdiplus (VGDevice * dev, jint* dstBitmap, int w, int h)
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

static void bimap_copy_gdi (VGDevice * dev, jint* dstBitmap, int w, int h)
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

static void bimap_copy (VGDevice * dev, jint* dstBitmap, int w, int h)
{
	if (gAntiAliasing) bimap_copy_gdiplus (dev, dstBitmap, w, h);
	else bimap_copy_gdi(dev, dstBitmap, w, h);
}

#elif __APPLE__
static void clear (VGDevice * dev, int w, int h)
{
	CGContextRef context = CGContextRef(dev->GetNativeContext());
	int*  data = (int *)::CGBitmapContextGetData(context);
	for (int i=0, n=w*h; i<n; i++) *data++ = 0;
}
static void bimap_copy (VGDevice * dev, jint* dstBitmap, int w, int h)
{
	CGContextRef context = CGContextRef(dev->GetNativeContext());
	int*  data = (int *)::CGBitmapContextGetData(context);
	for (int i=0, n=w*h; i<n; i++) {
		*dstBitmap++ = *data++;
	}
}

#elif __linux__
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
#endif



int getBitmap (jint* dstBitmap, int w, int h, GuidoOnDrawDesc& desc, const VGColor& color)
{
	VGDevice * dev = gSystem->CreateMemoryDevice (w, h);
	desc.hdc = dev;
#if __APPLE__
	clear (dev, w, h);
#endif
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

