
#include <iostream>

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

#include "CairoSystem.h"
#include "CairoDevice.h"

#include "GUIDOEngine.h"

//#define DEBUGDEVICE
#ifdef DEBUGDEVICE
#include "DebugDevice.h"
#endif

using namespace std;

GRHandler gGr;

//-------------------------------------------------------------------------------------------
static bool guido_error(const char* where, GuidoErrCode err)
{
	cerr << where << " error " << err << " : " << GuidoGetErrorString(err) << endl;
	return false;
}

//-------------------------------------------------------------------------------------------
static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	cairo_t *cr = gdk_cairo_create (widget->window);
	CairoSystem sys (cr);
	gint w, h;
	gdk_drawable_get_size (gtk_widget_get_parent_window(widget), &w, &h);

	VGDevice * dev = sys.CreateDisplayDevice();
	dev->SelectFillColor(VGColor(255,255,255));
	dev->Rectangle (0, 0, w, h);

	dev->SelectFillColor(VGColor(0,0,0));
	GuidoOnDrawDesc desc;
	desc.handle = gGr;
#ifdef DEBUGDEVICE
	dev = new DebugDevice(dev);
#endif
    desc.hdc = dev;
	desc.page = 1;
	desc.updateRegion.erase = true;
	desc.scrollx = desc.scrolly = 0;
    desc.sizex = w;
	desc.sizey = h;
    desc.isprint = false;
	GuidoErrCode err = GuidoOnDraw (&desc);
	if (err != guidoNoErr) guido_error ("GuidoOnDraw", err);

	delete dev;
	cairo_destroy(cr);
	return FALSE;
}

//-------------------------------------------------------------------------------------------
static bool guido_init()
{
	CairoSystem * sys = new CairoSystem(0);
	VGDevice * dev = sys->CreateMemoryDevice(10,10);

	GuidoInitDesc desc;
	desc.graphicDevice = dev;
	desc.musicFont = "guido2";
	desc.textFont  = "Times";
	GuidoErrCode err = GuidoInit (&desc);
	if (err != guidoNoErr) return guido_error ("GuidoInit", err);
	return true;
}

static bool gmn_init(const char* gmncode)
{
	if (!guido_init()) return false;

	ARHandler ar;
//	GuidoErrCode err = GuidoParseString (gmncode, &ar);
//	if (err != guidoNoErr) return guido_error ("GuidoParseString", err);
	GuidoErrCode err = GuidoParseFile (gmncode, &ar);
	if (err != guidoNoErr) return guido_error ("GuidoParseFile", err);
	err = GuidoAR2GR( ar, 0, &gGr);
	if (err != guidoNoErr) return guido_error ("GuidoAR2GR", err);
	err = GuidoResizePageToMusic (gGr);
	if (err != guidoNoErr) return guido_error ("GuidoResizePageToMusic", err);
	return true;
}

//-------------------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *darea;  

	gtk_init(&argc, &argv);
	gmn_init(argv[1]);
//	gmn_init("gmn/clefs.gmn");

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER (window), darea);

	g_signal_connect(darea, "expose-event", G_CALLBACK(on_expose_event), NULL);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 800); 

	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}