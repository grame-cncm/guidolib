
#include <jawt_md.h>
#include "../guidoengine_native_paint.h"
#include "GSystemWin32.h"
#include "GDeviceWin32.h"
#include "GSystemWin32GDIPlus.h"
#include "GDeviceWin32GDIPlus.h"

bool gAntiAliasing = false;

#define kErrPaint	-10
int paint (JNIEnv* env, jobject canvas, GuidoDrawF draw, GuidoOnDrawDesc& desc, const VGColor& color)
{
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    jint lock;

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

    JAWT_Win32DrawingSurfaceInfo* dsi_win= (JAWT_Win32DrawingSurfaceInfo*)dsi->platformInfo;	
	int result = 0;
	VGSystem *sys;
	if ( gAntiAliasing ) sys = new GSystemWin32GDIPlus(dsi_win->hdc, 0);
	else sys = new GSystemWin32(dsi_win->hdc, 0);
	VGDevice *dev = sys->CreateDisplayDevice();
	VGDevice *offscreen = gAntiAliasing ? sys->CreateMemoryDevice (dev->GetWidth(), dev->GetHeight()) : 0;
	if (gAntiAliasing) {
		desc.hdc = offscreen;
#if 0
		offscreen->SelectFillColor(VGColor(0, 0, 0, ALPHA_TRANSPARENT));
#else
		offscreen->SelectFillColor(VGColor(255));
#endif
		offscreen->Rectangle(0, 0, dev->GetWidth(), dev->GetHeight());
	}
	else desc.hdc = dev;
	desc.hdc->SetFontColor (color);
	desc.hdc->SelectFillColor(color);
	desc.hdc->SelectPenColor(color);
	result = draw (&desc);
	if (gAntiAliasing) dev->CopyPixels(offscreen);
	delete dev;
	delete offscreen;
	delete sys;

	ds->FreeDrawingSurfaceInfo(dsi);
    ds->Unlock(ds);
    awt.FreeDrawingSurface(ds);
	return result;
}


