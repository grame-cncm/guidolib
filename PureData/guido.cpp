/* guido externals for pd
 * copyright 2010 D. Fober - Grame fober@grame.fr

* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 */

#ifdef WIN32
#pragma warning(disable: 4091 4996)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "m_pd.h"
#include "g_canvas.h"

#include "GUIDOEngine.h"
#include "VGDevice.h"

#ifdef WIN32
# include "GSystemWin32GDIPlus.h"
# include "GDeviceWin32GDIPlus.h"
# include <GDIPlus.h>
# define export __declspec( dllexport )
#elif __APPLE__
# include "GSystemOSX.h"
# define export
#elif __linux__
# include "CairoSystem.h"
# define export
#endif

#define MAX_PATH_CHARS	2048
#define	MAX_ID_CHARS	128
#define EXTBOXWIDTH		10
#define INLETWIDTH		8
#define INLETHEIGHT		2

#ifdef MUSICXML
#include "musicxml.h"						// for musicxml import
#endif

VGSystem *	gSystem;
bool		gMusicXML;


static t_class *guido_class;


typedef struct queue_node{
	float time;
	int argc;
	t_atom *argv;
	struct queue_node * next;
	struct queue_node * previous;
}t_queue;

typedef struct {
	t_object x_obj;
	t_glist *  x_glist;
	t_queue *  queue;
	t_outlet * u_out;

	char canvasID[MAX_ID_CHARS];
	char imgID[MAX_ID_CHARS];

	bool	initialized;
	int		width;
	int		height;
	VGColor	u_background;
	VGColor	u_color;

	ARHandler	ar;
	GRHandler	gr;
	long		pageNum;
	char *		gmn;
}t_guido;

#ifdef __cplusplus
extern "C" {
#endif

export void	guido_setup	(void);
export void	guido_free	(t_guido *x);
export void *	guido_new	(t_floatarg tempo, t_floatarg tpq);
export void	guido_bang	(t_guido *x);
export void	guido_text	(t_guido *x, t_symbol *s);
export void	guido_page	(t_guido *x, float num);
export void	guido_file	(t_guido *x, t_symbol *s, short argc, t_atom *argv);
export void	guido_reset	(t_guido *x, t_symbol *s, short argc, t_atom *argv);


#ifdef __cplusplus
}
#endif

#define DEBUG(a)	a



//------------------------------------------------------------------------------
static void getXY (t_object* o, t_glist *glist,  int& x, int& y)
{
	x = text_xpix(o, glist);
	y = text_ypix(o, glist);
}

//------------------------------------------------------------------------------
static bool guidoerror (void* o, const char* f,  GuidoErrCode err)
{
	if (err == guidoErrParse)
		pd_error (o, "%s error: %s line %d", f, GuidoGetErrorString (err), GuidoGetParseErrorLine());
	else
		pd_error (o, "%s error: %s", f, GuidoGetErrorString (err));
	return false;
}

//------------------------------------------------------------------------------
const char * color2string (int rgba)
{
	static char cstr[32];
	sprintf (cstr, "\"#%02x%02x%02x\" ", (rgba >> 16) & 0xff, (rgba >> 8) & 0xff, rgba & 0xff);
	return cstr;
}

#ifdef WIN32
Gdiplus::BitmapData bdata;
static int * bitmapData (VGDevice * dev, int w, int h)
{
	GDeviceWin32GDIPlus *gdi = (GDeviceWin32GDIPlus*)dev;
	Gdiplus::Bitmap * bitmap = gdi->GetBitmap();
	Gdiplus::Rect rect(0, 0, w, h);
	bdata.Reserved = 0;
	bitmap->LockBits( &rect, Gdiplus::ImageLockModeRead, PixelFormat32bppPARGB, &bdata);
	return (int*)bdata.Scan0;
}
static void unlock(VGDevice * dev)
{
	GDeviceWin32GDIPlus *gdi = (GDeviceWin32GDIPlus*)dev;
	Gdiplus::Bitmap * bitmap = gdi->GetBitmap();
	bitmap->UnlockBits(&bdata);
}

#elif !linux

static void clear (VGDevice * dev, int w, int h)
{
	CGContextRef context = CGContextRef(dev->GetNativeContext());
	int*  data = (int *)::CGBitmapContextGetData(context);
	for (int i=0, n=w*h; i<n; i++) *data++ = 0;
}
static int * bitmapData (VGDevice * dev, int, int)
{
	CGContextRef context = CGContextRef(dev->GetNativeContext());
	return (int *)::CGBitmapContextGetData(context);
}
static void unlock(VGDevice * dev)	{}

#else
static void clear (VGDevice * dev, int w, int h)
{
	cairo_t * cr = (cairo_t *)dev->GetNativeContext();
	cairo_surface_t * surface = cairo_get_group_target (cr);
	int*  data = (int *)cairo_image_surface_get_data (surface);
	for (int i=0, n=w*h; i<n; i++) *data++ = 0;
}
static int * bitmapData (VGDevice * dev, int, int)
{
	cairo_t * cr = (cairo_t *)dev->GetNativeContext();
	cairo_surface_t * surface = cairo_get_group_target (cr);
	return (int *)cairo_image_surface_get_data (surface);
}
static void unlock(VGDevice * dev)	{}
#endif


//------------------------------------------------------------------------------
#ifdef WIN32
static char * stpcpy(char *dst, const char* str)
{
	strcpy (dst, str);
	return dst + strlen (str);
}
#endif

//------------------------------------------------------------------------------
static void make_img(t_guido *guido, int* bitmap, int w, int h)
{
	if (!bitmap) return;

	const char * list = "[list ";
	const char * endlist = "] ";
	size_t nlist = strlen (list);
	size_t elist = strlen (endlist);
	size_t ncolor= strlen (color2string(0));

	size_t memsize = ((w * ncolor) + nlist + elist + 1) * h;
	char * buff = (char*)malloc (memsize);
	char * ptr = buff;

	ptr = stpcpy (ptr, list);
	for (int i=0; i<h; i++) {
		ptr = stpcpy (ptr, list);
		for (int n=0; n < w; n++) {
			ptr = stpcpy (ptr, color2string (*bitmap++));
		}
		ptr = stpcpy (ptr, endlist);
	}
	stpcpy (ptr, "]");
	sys_vgui((char*)"%s put %s\n", guido->imgID, buff);
	free (buff);
}

//------------------------------------------------------------------------------
static bool draw_score (t_guido *guido, int w, int h)
{
	VGDevice * dev = gSystem->CreateMemoryDevice (w, h);
#if !defined(WIN32) && !linux
	clear (dev, w, h);
#endif
	dev->SelectFillColor(guido->u_background);
	dev->Rectangle(0,0,w,h);

	VGColor sc = guido->u_color;
	dev->SelectFillColor(sc);
	dev->SelectPenColor(sc);
	dev->SetFontColor (sc);

	if (guido->gr) {
		GuidoOnDrawDesc desc;
		desc.handle = guido->gr;
		desc.hdc = dev;
		desc.page = guido->pageNum;
		desc.scrollx = desc.scrolly = 0;
		desc.isprint = false;
		desc.sizex = w;
		desc.sizey = h;
		desc.updateRegion.erase = true;
		GuidoErrCode err = GuidoOnDraw (&desc);
		if (err) return guidoerror ((t_object*)guido, "GuidoOnDraw", err);
	}
	make_img (guido, bitmapData(dev, w, h), w, h);
	unlock(dev);
	delete dev;
	return true;
}

//------------------------------------------------------------------------------
#ifdef MUSICXML
static GuidoErrCode guidosetxml (t_guido *x, const char* s, ARHandler* ar)
{
	char * guidoCode = musicxml2guido ( s );
	GuidoErrCode err;
	if (guidoCode) {
		err = GuidoParseString (guidoCode, ar);
		delete guidoCode;
		if (err == guidoNoErr) return err;
	}
	else err = guidoErrMemory;
	pd_error ((t_object*)x, "%s: MusicXML to Guido convertion failed", s);
	return err;
}
#endif

//------------------------------------------------------------------------------
// sets the gmn code and builds the ar and gr representations
//------------------------------------------------------------------------------
static bool guidoset (t_guido *x, const char* s, bool file)
{
	ARHandler ar;
	GRHandler gr;
	GuidoErrCode err;

	if (file) {
		err = GuidoParseFile (s, &ar);
#ifdef MUSICXML
		if ((err == guidoErrParse) && gMusicXML)
			err = guidosetxml (x, s, &ar);
#endif
		if (err) return guidoerror ((t_object*)x, "GuidoParseFile", err);
	}
	else {
		err = GuidoParseString (s, &ar);
		if (err) return guidoerror ((t_object*)x, "GuidoParseString", err);
	}
	err = GuidoAR2GR (ar, 0, &gr);
	if (err) {
		GuidoFreeAR (ar);
		return guidoerror ((t_object*)x, "GuidoAR2GR", err);
	}
	err = GuidoResizePageToMusic (gr);
	if (err) {
		GuidoFreeGR (gr);
		GuidoFreeAR (ar);
		return guidoerror ((t_object*)x, "GuidoResizePageToMusic", err);
	}
	if (x->gr) GuidoFreeGR (x->gr);
	x->gr = gr;
	if (x->ar) GuidoFreeAR (x->ar);
	x->ar = ar;
	if (x->gmn) free (x->gmn);
	x->gmn = (char *)malloc (strlen(s)+1);
	strcpy (x->gmn, s);
	x->pageNum = 1;
	draw_score(x, x->width, x->height);
	return true;
}



//------------------------------------------------------------------------------
void guido_text(t_guido *x, t_symbol *s)
{
	guidoset (x, s->s_name, false);
}

//------------------------------------------------------------------------------
void guido_page(t_guido *x, float fnum)
{
	int num = int(fnum);
	if (x->gr) {
		int max = GuidoGetPageCount (x->gr);
		if (num < 1) num = 1;
		if (num > max) num = max;
		if (x->pageNum != num) {
			x->pageNum = num;
			draw_score(x, x->width, x->height);
		}
	}
}

//------------------------------------------------------------------------------
void guido_reset(t_guido *x, t_symbol *s, short argc, t_atom *argv)
{
	if (x->gr) GuidoFreeGR (x->gr);
	if (x->ar) GuidoFreeAR (x->ar);
	if (x->gmn) free(x->gmn);
	x->ar = 0;
	x->gr = 0;
	x->gmn = 0;
	draw_score(x, x->width, x->height);
}

//------------------------------------------------------------------------------
void guido_file(t_guido *x, t_symbol *s, short argc, t_atom *argv)
{
	char filename[MAX_PATH_CHARS], fullname[MAX_PATH_CHARS];
	short vol = 0;
	long *type, ntype,outtype;

	ntype = 0;
	type = 0;
	if (!argc) {
post ("guido_file argc null");

//		if (open_dialog(filename, &vol, &outtype, type, ntype))
			return;
	}
	else {
		atom_string(argv, filename, MAX_PATH_CHARS);
//		strcpy(filename, argv->a_w.w_sym->s_name);
//		if (locatefile_extended(filename, &vol, &outtype, type, ntype)) {
//			pd_error (x, "%s: not found", filename);
//			return;
//		}
	}
//	path_topathname (vol, filename, fullname);
//	path_nameconform (fullname, filename, PATH_STYLE_NATIVE, PATH_TYPE_PATH);
	guidoset (x, filename, true);
}


void guido_bang(t_guido *x)
{
	if (x->gmn) outlet_anything(x->u_out, gensym(x->gmn), 0, 0);
}

void *guido_new ()
{
	t_guido *x =(t_guido *)pd_new(guido_class);
	x->queue = NULL;

	x->width = 150;
	x->height = 200;

	x->ar = 0;
	x->gr = 0;
	x->pageNum = 1;
	x->u_out=outlet_new(&x->x_obj,0);
	x->u_background = VGColor(0xff,0xff,0xff);
	x->u_color = VGColor(0,0,0);
	return (x);
}

void guido_free(t_guido *x)
{
	if (x->gr) GuidoFreeGR (x->gr);
	if (x->ar) GuidoFreeAR (x->ar);
	if (x->gmn) free(x->gmn);
}

//------------------------------------------------------------------------------
static void draw_inlets(t_guido *guido, t_glist *glist, int firsttime)
{
	int x, y;
	getXY (&guido->x_obj, glist, x, y);
	int yo = y + guido->height;

	if (firsttime) {
		sys_vgui((char*)"%s create rectangle %d %d %d %d -tags %xIN\n", guido->canvasID, x, y, x+INLETWIDTH, y+INLETHEIGHT, guido);
		sys_vgui((char*)"%s create rectangle %d %d %d %d -tags %xOUT\n", guido->canvasID, x, yo, x+INLETWIDTH, yo-INLETHEIGHT, guido);
    }
     else {
		sys_vgui((char*)"%s coords %xIN %d %d %d %d\n", guido->canvasID, guido, x, y, x+INLETWIDTH, y+INLETHEIGHT);
		sys_vgui((char*)"%s coords %xOUT %d %d %d %d\n", guido->canvasID, guido, x, yo, x+INLETWIDTH, yo-INLETHEIGHT);
     }
}

//------------------------------------------------------------------------------
static void guido_drawme(t_guido *guido, t_glist *glist, int firsttime)
{
	t_canvas *canvas=glist_getcanvas(glist);

	int x, y;
	getXY (&guido->x_obj, glist, x, y);

	if (firsttime) {
		sprintf (guido->canvasID, ".x%lx.c", (long)canvas);
		sprintf (guido->imgID, "%lxIMG", (long)guido);

		guido->x_glist = canvas;
		sys_vgui((char*)"image create photo %s\n", guido->imgID);
		draw_score (guido, guido->width, guido->height);
		sys_vgui((char*)"%s create image %d %d -tags %xIMG -anchor nw -image %s\n", guido->canvasID, x, y, guido, guido->imgID);
		sys_vgui((char*)"%s create rectangle %d %d %d %d -tags %xFRAME -outline black\n", guido->canvasID, x, y, x+guido->width, y+guido->height, guido);
     }
     else {
		sys_vgui((char*)"%s coords %xFRAME %d %d %d %d\n", guido->canvasID, guido, x, y, x + guido->width, y + guido->height);
		sys_vgui((char*)"%s coords %xIMG %d %d\n", guido->canvasID, guido, x, y);
     }
	 draw_inlets (guido, glist, firsttime);
}


/* ------------------------ guido widgetbehaviour----------------------------- */
static void guido_getrect(t_gobj *z, t_glist *owner, int *xp1, int *yp1, int *xp2, int *yp2)
{
	t_guido* guido = (t_guido*)z;
	int x, y;
	getXY (&guido->x_obj, owner, x, y);

    *xp1 = x;
    *yp1 = y;
    *xp2 = *xp1 + guido->width;
    *yp2 = *yp1 + guido->height;
}

static void guido_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
	t_guido *guido = (t_guido *)z;
    guido->x_obj.te_xpix += dx;
    guido->x_obj.te_ypix += dy;
    if (glist_isvisible(glist))
    {
		int x, y;
		getXY (&guido->x_obj, glist, x, y);

		int xa = x+guido->width;
		int ya = y+guido->height;
		guido_drawme (guido, glist, 0);
		sys_vgui((char*)"%s coords %xEXT %d %d %d %d\n", guido->canvasID, guido, xa-EXTBOXWIDTH, ya-EXTBOXWIDTH, xa, ya);
		canvas_fixlinesfor(glist_getcanvas(glist),(t_text*)guido);
    }
}

static void guido_delete_ui(t_guido *guido)
{
	sys_vgui((char*)"%s delete  %xIMG\n", guido->canvasID, guido);
	sys_vgui((char*)"%s delete  %xFRAME\n", guido->canvasID, guido);
	sys_vgui((char*)"%s delete  %xIN\n", guido->canvasID, guido);
	sys_vgui((char*)"%s delete  %xOUT\n", guido->canvasID, guido);
	sys_vgui((char*)"%s delete  %xEXT\n", guido->canvasID, guido);
}

static void guido_size(t_gobj *z, t_float width, t_float height)
{
    t_guido *guido = (t_guido *)z;
	guido->height = height;
    guido->width = width;
    if(glist_isvisible(guido->x_glist))
    {
 		guido_delete_ui (guido);
		guido_drawme (guido, guido->x_glist, 1);
		canvas_fixlinesfor(guido->x_glist, (t_text *)z);  // 2nd inlet
    }
}

static void guido_select(t_gobj *z, t_glist *glist, int state)
{
//	 DEBUG(post("select start");)
	t_guido *guido = (t_guido *)z;
	t_canvas *canvas = glist_getcanvas(glist);

	int x, y;
	getXY(&guido->x_obj, glist, x, y);
	if (state) {
		sys_vgui((char*)"%s itemconfigure %xFRAME -outline blue\n", guido->canvasID, guido);

		int xa = x+guido->width;
		int ya = y+guido->height;
		sys_vgui((char*)"%s create rectangle %d %d %d %d -tags %xEXT\n", guido->canvasID, xa-EXTBOXWIDTH, ya-EXTBOXWIDTH, xa, ya, guido);
		sys_vgui((char*)"%s bind %xEXT <Enter> { %s config -cursor bottom_right_corner }\n", guido->canvasID, guido, guido->canvasID);
		sys_vgui((char*)"%s bind %xEXT <Leave> { %s config -cursor {} }\n", guido->canvasID, guido, guido->canvasID);
//		sys_vgui((char*)"%s bind %xEXT <B1 Motion> { %s itemconfigure %xFRAME -width \%x -height \%y }\n", guido->canvasID, guido);
	}
	else {
		sys_vgui((char*)"%s itemconfigure %xFRAME -outline black\n", guido->canvasID, guido);
		sys_vgui((char*)".x%x.c delete %xEXT\n", canvas, guido);
	}
}

static void guido_activate(t_gobj *z, t_glist *glist, int state)
{
//    DEBUG(post("activate commented out!");)
}

static void guido_delete(t_gobj *z, t_glist *glist)
{
 	t_guido *guido = (t_guido *)z;
	canvas_deletelinesfor(glist, (t_text *)z);
	guido_delete_ui (guido);
}


static void guido_vis (t_gobj *z, t_glist *glist, int vis)
{
    t_guido* s = (t_guido*)z;
    t_rtext *y;
    if (vis) {
#ifdef PD_MINOR_VERSION
      	y = (t_rtext *) rtext_new(glist, (t_text *)z);
#else
        y = (t_rtext *) rtext_new(glist, (t_text *)z,0,0);
#endif
		guido_drawme(s, glist, 1);
    }
    else {
		y = glist_findrtext(glist, (t_text *)z);
//		guido_erase(s,glist);
		rtext_free(y);
    }
}

#ifdef WIN32
struct _widgetbehavior guido_widgetbehavior = {
	guido_getrect,
	guido_displace,
	guido_select,
	guido_activate,
	guido_delete,
	guido_vis,
	NULL
};
#else
t_widgetbehavior guido_widgetbehavior = {
  w_getrectfn:  guido_getrect,
  w_displacefn: guido_displace,
  w_selectfn:   guido_select,
  w_activatefn: guido_activate,
  w_deletefn:   guido_delete,
  w_visfn:      guido_vis,
  w_clickfn:    NULL,
};
#endif

//------------------------------------------------------------------------------
static void guidoinit(void)
{
#ifdef WIN32
	GSystemWin32GDIPlus * sys = new GSystemWin32GDIPlus(0,0);
	sys->Start();
	gSystem = sys;
#elif __APPLE__
	gSystem = new GSystemOSX(0,0);
#elif __linux__
	gSystem = new CairoSystem(0);
#endif
	GuidoInitDesc desc;
	desc.graphicDevice = gSystem->CreateMemoryDevice (50,50);
	desc.musicFont = "Guido2";
	desc.textFont  = "Times";
	GuidoInit (&desc);

	int a, b, c;
	GuidoGetVersionNums (&a, &b, &c);
	post ("Guido external v.1.01 using Engine v.%d.%d%d", a, b, c);

#ifdef MUSICXML
	gMusicXML = loadMusicxml();
	if (gMusicXML)
		post ("MusicXML lib v.%s with Guido converter v.%s", musicxmllibVersionStr(), musicxml2guidoVersionStr());
#endif
}

void guido_setup(void)
{
	guido_class = class_new(gensym("guido"), (t_newmethod)guido_new,  (t_method)guido_free, sizeof(t_guido), CLASS_DEFAULT, A_NULL);

	class_addmethod (guido_class, (t_method)guido_text,	gensym("set"),	A_SYMBOL, A_NULL);
	class_addmethod (guido_class, (t_method)guido_page,	gensym("page"),	A_FLOAT, A_NULL);
	class_addmethod (guido_class, (t_method)guido_file,	gensym("read"),	A_GIMME, A_NULL);
	class_addmethod (guido_class, (t_method)guido_reset,gensym("reset"),A_GIMME, A_NULL);
	class_addmethod (guido_class, (t_method)guido_size, gensym("size"), A_DEFFLOAT,  A_DEFFLOAT, A_NULL);
	class_addbang	(guido_class, (t_method) guido_bang);
	class_sethelpsymbol(guido_class,gensym("help-guido"));
    class_setwidget (guido_class,&guido_widgetbehavior);
	guidoinit();
}

