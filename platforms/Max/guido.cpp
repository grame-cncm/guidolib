/*

  Guido Engine Library
  Copyright (C) 2010  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#include "ext.h"							// standard Max include, always required
#include "ext_obex.h"						// required for new style Max object
#include "jpatcher_api.h"
#include "jgraphics.h"

#include "GUIDOEngine.h"
#include "VGDevice.h"
#ifdef WIN32
# include "GSystemWin32GDIPlus.h"
# include "GDeviceWin32GDIPlus.h"
#else
# include "GSystemOSX.h"
#endif

#include "musicxml.h"						// for musicxml import


typedef struct _guido 
{
	t_jbox		u_box;						// header for UI objects
	void *		u_out;						// outlet pointer
	t_jrgba		u_outline;
	t_jrgba		u_background;
	t_jrgba		u_color;
	ARHandler	ar; 
	GRHandler	gr;
	long		pageNum;
	char *		gmn;
} t_guido;

void *guido_new(t_symbol *s, long argc, t_atom *argv);
void guido_free(t_guido *x);
void guido_assist(t_guido *x, void *b, long m, long a, char *s);
void guido_paint(t_guido *x, t_object *patcherview);
void guido_getdrawparams(t_guido *x, t_object *patcherview, t_jboxdrawparams *params);
void guido_bang(t_guido *x);
void guido_text(t_guido *x, t_symbol *s);
void guido_page(t_guido *x, long num);
void guido_file(t_guido *x, t_symbol *s, short argc, t_atom *argv);
void guido_reset(t_guido *x, t_symbol *s, short argc, t_atom *argv);

static t_class *s_guido_class;


VGSystem *	gSystem;
bool		gMusicXML;

static void guidoinit(void)
{	
#ifdef WIN32
	GSystemWin32GDIPlus * sys = new GSystemWin32GDIPlus(0,0);
	sys->Start();
	gSystem = sys;
#else
	gSystem = new GSystemOSX(0,0);
#endif
	GuidoInitDesc desc;
	desc.graphicDevice = gSystem->CreateMemoryDevice (50,50);;
	desc.musicFont = "Guido2"; 
	desc.textFont  = "Times";
	GuidoInit (&desc);

	int a, b, c;
	GuidoGetVersionNums (&a, &b, &c);
	post ("Guido external v.1.01 using Engine v.%d.%d%d", a, b, c);
	
	gMusicXML = loadMusicxml();
	if (gMusicXML) 
		post ("MusicXML lib v.%s with Guido converter v.%s", musicxmllibVersionStr(), musicxml2guidoVersionStr());
}

int main(void)
{	
	t_class *c;
		
	c = class_new("guido", (method)guido_new, (method)guido_free, sizeof(t_guido), 0L, A_GIMME, 0);

	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_FIXWIDTH | JBOX_COLOR);

	class_addmethod (c, (method)guido_paint,		"paint",		A_CANT, 0);
	class_addmethod (c, (method)guido_assist,		"assist",		A_CANT, 0);  
	
	// messages for state setting / retrieval
	class_addmethod (c, (method)guido_text,		"set",	A_SYM, 0);
	class_addmethod (c, (method)guido_page,		"page",	A_LONG, 0);
	class_addmethod (c, (method)guido_file,		"read",	A_GIMME, 0);
	class_addmethod (c, (method)guido_reset,	"reset",A_GIMME, 0);
	class_addmethod (c, (method)guido_bang,		"bang", 0);
	
	// attributes
	CLASS_STICKY_ATTR					(c, "category", 0, "Color");
	CLASS_ATTR_RGBA						(c, "bgcolor", 0, t_guido, u_background); 
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT	(c, "bgcolor", 0, "1. 1. 1. 1."); 
	CLASS_ATTR_STYLE_LABEL				(c, "bgcolor", 0,"rgba","Background Color");
	
	CLASS_ATTR_RGBA						(c, "bordercolor", 0, t_guido, u_outline); 
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT	(c, "bordercolor", 0, "0.5 0.5 0.5 1."); 
	CLASS_ATTR_STYLE_LABEL				(c, "bordercolor", 0,"rgba","Border Color");
	
	CLASS_ATTR_RGBA						(c, "color", 0, t_guido, u_color); 
	CLASS_ATTR_DEFAULTNAME_SAVE_PAINT	(c, "color", 0, "0. 0. 0. 1."); 
	CLASS_ATTR_STYLE_LABEL				(c, "color", 0, "rgba", "Color");
	
	CLASS_STICKY_ATTR_CLEAR				(c, "category");
	CLASS_ATTR_DEFAULT					(c, "patching_rect",0, "0. 0. 200. 200.");	

	class_register(CLASS_BOX, c);
	s_guido_class = c;
	guidoinit();
	return 0;
}

void guido_assist(t_guido *x, void *b, long m, long a, char *s)
{
	if (m == 1)		//inlet
		sprintf(s, "Text: GMN code, GMN file ");
}

static bool guidoerror (t_object* o, const char* f,  GuidoErrCode err)
{
	if (err == guidoErrParse)
		object_error (o, "%s error: %s line %d", f, GuidoGetErrorString (err), GuidoGetParseErrorLine());
	else
		object_error (o, "%s error: %s", f, GuidoGetErrorString (err));
	return false;
}

//------------------------------------------------------------------------------
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
	object_error ((t_object*)x, "%s: MusicXML to Guido convertion failed", s);
	return err;
}

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
		if ((err == guidoErrParse) && gMusicXML)
			err = guidosetxml (x, s, &ar);
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
	return true;
}

//------------------------------------------------------------------------------
// platform specific drawing stuff
//------------------------------------------------------------------------------
static VGColor jcolor2vgcolor (const t_jrgba& c)
{
#if WIN32
	return VGColor(c.red*255, c.green*255, c.blue*255, c.alpha*255);
#else
	return VGColor(c.blue*255, c.green*255, c.red*255, c.alpha*255);
#endif
}

#ifdef WIN32
static void native_paint (VGDevice * dev, t_jgraphics *g, int w, int h)
{
	GDeviceWin32GDIPlus *gdi = (GDeviceWin32GDIPlus*)dev;
	Bitmap * bitmap = gdi->GetBitmap();
	Gdiplus::Rect rect(0, 0, w, h);
	BitmapData bdata;
	bdata.Reserved = 0;
	bitmap->LockBits( &rect, ImageLockModeRead, PixelFormat32bppPARGB, &bdata);
	t_jsurface* jsurface = jgraphics_image_surface_create_for_data ( (unsigned char*)bdata.Scan0, JGRAPHICS_FORMAT_ARGB32, w, h, bdata.Stride, NULL, NULL);

	t_rect r = {0,0,w,h};
	jgraphics_image_surface_draw(g, jsurface, r, r);
	jgraphics_surface_destroy(jsurface);
	bitmap->UnlockBits(&bdata);
}
#else
static void clear (VGDevice * dev, int w, int h)
{
	CGContextRef context = CGContextRef(dev->GetNativeContext());
	int*  data = (int *)::CGBitmapContextGetData(context);
	for (int i=0, n=w*h; i<n; i++) *data++ = 0;
}
static void native_paint (VGDevice * dev, t_jgraphics *g, int w, int h)
{
	CGContextRef context = CGContextRef(dev->GetNativeContext());
	unsigned char*  data = (unsigned char *)::CGBitmapContextGetData(context);
	t_jsurface* jsurface = jgraphics_image_surface_create_for_data ( data, JGRAPHICS_FORMAT_ARGB32, 
		w, h, ::CGBitmapContextGetBytesPerRow(context), NULL, NULL);

	t_rect r = {0, 0, w, h};
	jgraphics_image_surface_draw(g, jsurface, r, r);
	jgraphics_surface_destroy(jsurface);
}
#endif

//------------------------------------------------------------------------------
// drawing of the score
//------------------------------------------------------------------------------
static bool score (t_guido *x, t_jgraphics *g, int w, int h)
{
	VGDevice * dev = gSystem->CreateMemoryDevice (w, h);
#ifndef WIN32
	clear (dev, w, h);
#endif
	dev->SelectFillColor(jcolor2vgcolor(x->u_background));
	dev->Rectangle(0,0,w,h);

	VGColor sc = jcolor2vgcolor(x->u_color);
	dev->SelectFillColor(sc);
	dev->SelectPenColor(sc);
	dev->SetFontColor (sc);

	if (x->gr) {
		GuidoOnDrawDesc desc;
		desc.handle = x->gr;
		desc.hdc = dev;
		desc.page = x->pageNum;
		desc.scrollx = desc.scrolly = 0;
		desc.isprint = false;
		desc.sizex = w;
		desc.sizey = h;
		desc.updateRegion.erase = true;
		GuidoErrCode err = GuidoOnDraw (&desc);
		if (err) return guidoerror ((t_object*)x, "GuidoOnDraw", err);
	}
	native_paint (dev, g, w, h);

	delete dev;
	return true;
}

//------------------------------------------------------------------------------
void guido_paint(t_guido *x, t_object *patcherview)
{
	t_rect rect;
	t_jgraphics *g = (t_jgraphics*) patcherview_get_jgraphics(patcherview);		// obtain graphics context
	jbox_get_rect_for_view((t_object *)x, patcherview, &rect);
		
	// paint outline
//	jgraphics_set_source_jrgba(g, &x->u_background);
//	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
//	jgraphics_fill(g);
//	jgraphics_set_source_jrgba(g, &x->u_color);

	score (x, g, rect.width, rect.height);

	jgraphics_set_source_jrgba(g, &x->u_outline);
	jgraphics_set_line_width(g, 1.);
	jgraphics_rectangle(g, 0., 0., rect.width, rect.height);
	jgraphics_stroke(g);
}

//------------------------------------------------------------------------------
void guido_text(t_guido *x, t_symbol *s)
{
	guidoset (x, s->s_name, false);
	jbox_redraw((t_jbox *)x);
}

//------------------------------------------------------------------------------
void guido_page(t_guido *x, long num)
{
	if (x->gr) {
		int max = GuidoGetPageCount (x->gr);
		if (num < 1) num = 1;
		if (num > max) num = max;
		if (x->pageNum != num) {
			x->pageNum = num;
			jbox_redraw((t_jbox *)x);
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
	jbox_redraw((t_jbox *)x);
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
		if (open_dialog(filename, &vol, &outtype, type, ntype))
			return;
	} 
	else {
		strcpy(filename, argv->a_w.w_sym->s_name);
		if (locatefile_extended(filename, &vol, &outtype, type, ntype)) {
			object_error((t_object*)x, "%s: not found", filename); 
			return;
		} 
	}
	path_topathname (vol, filename, fullname);
	path_nameconform (fullname, filename, PATH_STYLE_NATIVE, PATH_TYPE_PATH);
	guidoset (x, filename, true);
	jbox_redraw((t_jbox *)x);
}

void guido_bang(t_guido *x)
{
	if (x->gmn) outlet_anything(x->u_out, gensym(x->gmn), 0, 0);
}

void guido_getdrawparams(t_guido *x, t_object *patcherview, t_jboxdrawparams *params)
{
	params->d_bordercolor.alpha = 0;
	params->d_boxfillcolor.alpha = 0;
}

void guido_free(t_guido *x)
{
	jbox_free((t_jbox *)x);
	if (x->gr) GuidoFreeGR (x->gr);
	if (x->ar) GuidoFreeAR (x->ar);
	if (x->gmn) free(x->gmn);
}

void *guido_new(t_symbol *s, long argc, t_atom *argv)
{
	t_guido *x = NULL;
 	t_dictionary *d = NULL;
	long boxflags;
	
	if (!(d = object_dictionaryarg(argc,argv)))
		return NULL;
   
	x = (t_guido *)object_alloc(s_guido_class);
	boxflags = 0 
			| JBOX_DRAWFIRSTIN 
			| JBOX_NODRAWBOX
			| JBOX_DRAWINLAST
			| JBOX_TRANSPARENT	
	//		| JBOX_NOGROW
	//		| JBOX_GROWY
			| JBOX_GROWBOTH
	//		| JBOX_HILITE
	//		| JBOX_BACKGROUND
			| JBOX_DRAWBACKGROUND
	//		| JBOX_NOFLOATINSPECTOR
	//		| JBOX_TEXTFIELD
	//		| JBOX_MOUSEDRAGDELTA
	//		| JBOX_TEXTFIELD
			;

	jbox_new((t_jbox *)x, boxflags, argc, argv);
	x->u_box.b_firstin = (t_object *)x;
	x->ar = 0;
	x->gr = 0;
	x->pageNum = 1;
	x->u_out = outlet_new((t_object *)x, NULL);
	attr_dictionary_process(x,d);
	jbox_ready((t_jbox *)x);
	return x;
}
