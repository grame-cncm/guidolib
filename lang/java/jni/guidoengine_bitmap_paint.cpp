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
#include "device_specific_functions.h"
#include "VGSystem.h"
#include "VGDevice.h"
#include "VGColor.h"

extern VGSystem * gSystem;


int getBitmap (jint* dstBitmap, int w, int h, GuidoOnDrawDesc& desc, const VGColor& color)
{
	VGDevice * dev = gSystem->CreateMemoryDevice (w, h);
	desc.hdc = dev;
	device_specific::clear (dev, w, h);
	dev->SelectFillColor(color);
	dev->SelectPenColor(color);
	dev->SetFontColor (color);

	GuidoErrCode err = GuidoOnDraw (&desc);
	if (err == guidoNoErr)
		device_specific::bimap_copy (dev, dstBitmap, w, h);
	else fprintf (stderr, "GuidoOnDraw error %d: %s\n", err, GuidoGetErrorString (err));
	delete dev;
	return err;

}

int getBitmapPianoRoll (jint* dstBitmap, int w, int h, PianoRoll * pr, const VGColor& color)
{
	VGDevice * dev = gSystem->CreateMemoryDevice (w, h);
	device_specific::clear (dev, w, h);
	dev->SelectFillColor(color);
	dev->SelectPenColor(color);
	dev->SetFontColor (color);

	GuidoErrCode err = GuidoPianoRollOnDraw(pr, w, h, dev);
	if (err == guidoNoErr)
		device_specific::bimap_copy (dev, dstBitmap, w, h);
	else fprintf (stderr, "GuidoOnDraw error %d: %s\n", err, GuidoGetErrorString (err));
	delete dev;
	return err;
}

VGColor jcolor2VGColor (JNIEnv * env, jobject jcolor)
{
	int color = env->CallIntMethod (jcolor, getRGBID);
	unsigned char r, g, b, a;
	b = (unsigned char)(color & 0xff);
	g = (unsigned char)((color & 0xff00) >> 8);
	r = (unsigned char)((color & 0xff0000) >> 16);
	a = (unsigned char)((color & 0xff000000) >> 24);
#ifdef WIN32
	return VGColor (r, g, b, a);
#else
	return VGColor (b, g, r, a);
#endif
}
