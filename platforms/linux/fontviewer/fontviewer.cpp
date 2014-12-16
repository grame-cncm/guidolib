
#include <iostream>

#include <cairo.h>
#include <gtk/gtk.h>
#include <math.h>

#include "CairoSystem.h"
#include "CairoDevice.h"

using namespace std;


//-------------------------------------------------------------------------------------------
static const char* to_utf8 (int n)
{
	static char utf[5];
	if (n < 128) {
		utf[0] = n;
		utf[1] = 0;
	}
	else if (n < 2048) {
		utf[0] = 192 + (n / 64);
		utf[1] = 128 + (n % 64);

//		utf[0] = 0xC0 | ((n >> 6) & 0x1f);
//		utf[1] = 0x80 | (n & 0x3f);
		utf[2] = 0;
	}
	else utf[0] = 0;
	return utf;
}

//-------------------------------------------------------------------------------------------
static void print_font (cairo_t *cr, int w , int h)
{
	cairo_set_source_rgba (cr, 0., 0., 0., 1.);
	cairo_select_font_face (cr, "guido2", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 24);

	int x=15, y=35;
	unsigned short n=0;
	for (int i=0; i<19; i++) {
		for (int j=0; j<18; j++) {
			cairo_move_to (cr, x, y);
			cairo_show_text(cr, to_utf8(n));
			x += 33;
			n++;
		}
		x  = 10;
		y += 45;
	}

	cairo_select_font_face (cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 10);
	x=15, y=35;
	n=0;
	for (int i=0; i<19; i++) {
		for (int j=0; j<18; j++) {
			char buff[5];
			sprintf (buff, "%d", n);
			cairo_move_to (cr, x, y+18);
			cairo_show_text(cr, buff);
			x += 33;
			n++;
		}
		x  = 10;
		y += 45;
	}
//	for (int i=0; i<19; i++) {
//		for (int j=0; j<18; j++) {
//			char buff[5];
//			sprintf (buff, "%d", n);
//
//			cairo_select_font_face (cr, "guido2", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//			cairo_set_font_size (cr, 24);
//			cairo_move_to (cr, x, y);
//			cairo_show_text(cr, to_utf8(n));
//
//			cairo_select_font_face (cr, "Arial", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//			cairo_set_font_size (cr, 10);
//			cairo_move_to (cr, x, y+18);
//			cairo_show_text(cr, buff);
//			x += 33;
//			n++;
//		}
//		x  = 10;
//		y += 45;
//	}
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
	print_font (cr, w, h);

	delete dev;
	cairo_destroy(cr);
	return FALSE;
}

//-------------------------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *darea;  

	gtk_init(&argc, &argv);

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