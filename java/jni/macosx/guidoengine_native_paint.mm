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

#import <Cocoa/Cocoa.h>

#include <jawt_md.h>
#include "../guidoengine_native_paint.h"
#include "GSystemOSX.h"
#include "GDisplayDeviceOSX.h"

#define kErrPaint	-10

//--------------------------------------------------------------------------------------------------
int paint (JNIEnv* env, jobject canvas, GuidoDrawF draw, GuidoOnDrawDesc& desc, const VGColor& c)
{
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    jint lock;
	NSAutoreleasePool *pool;
	CGContextRef contextRef;

	awt.version = JAWT_VERSION_1_3;
    if (JAWT_GetAWT(env, &awt) == JNI_FALSE) {
        printf("AWT Not found\n");
        return kErrPaint;
    }

    /* Get the drawing surface */
    ds = awt.GetDrawingSurface(env, canvas);
    if (ds == NULL) {
        printf("NULL drawing surface\n");
        return kErrPaint;
    }

    /* Lock the drawing surface */
    lock = ds->Lock(ds);
    if((lock & JAWT_LOCK_ERROR) != 0) {
        printf("Error locking surface\n");
        awt.FreeDrawingSurface(ds);
        return kErrPaint;
    }

    /* Get the drawing surface info */
    dsi = ds->GetDrawingSurfaceInfo(ds);
    if (dsi == NULL) {
        printf("Error getting surface info\n");
        ds->Unlock(ds);
        awt.FreeDrawingSurface(ds);
        return kErrPaint;
    }

	pool = [[NSAutoreleasePool alloc] init];

    JAWT_MacOSXDrawingSurfaceInfo * dsi_mac = (JAWT_MacOSXDrawingSurfaceInfo*)dsi->platformInfo;
	NSView * view = dsi_mac->cocoaViewRef;
	NSWindow *	win	= [view window];
	NSGraphicsContext *	nsGraphicsContext = [win graphicsContext];
	
	contextRef	= (CGContextRef) [nsGraphicsContext graphicsPort];
	NSRect r = [view bounds];

	CGContextSaveGState (contextRef);
	GSystemOSX sys(contextRef, 0);
	GDisplayDeviceOSX dev (contextRef, &sys);
	dev.NotifySize (r.size.width, r.size.height);
	dev.SetFontColor (c);
	dev.SelectFillColor(c);
	dev.SelectPenColor(c);
	desc.hdc = &dev;
	int ret = draw (&desc);
	CGContextRestoreGState (contextRef);
	
	[pool release];
    ds->FreeDrawingSurfaceInfo(dsi);
    ds->Unlock(ds);
    awt.FreeDrawingSurface(ds);
	return ret;
}

