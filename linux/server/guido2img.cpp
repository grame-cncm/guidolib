/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "guido2img.h"
#include "CairoSystem.h"
#include "CairoDevice.h"
#include <cairo.h>

#include <assert.h>

using namespace std;
namespace guidohttpd
{

static cairo_status_t
write_png_stream_to_byte_array (void *in_closure, const unsigned char *data,
                                                unsigned int length)
{
    png_stream_to_byte_array_closure_t *closure =
        (png_stream_to_byte_array_closure_t *) in_closure;

    memcpy (closure->data, data, length);
    closure->data += length;
    closure->size += length;
    return CAIRO_STATUS_SUCCESS;
}

//--------------------------------------------------------------------------
guido2img::guido2img () {
  fBuffer.data = new char[1048576];
  fBuffer.start = fBuffer.data;
  fBuffer.size = 0;
}

guido2img::~guido2img () {
  delete[] fBuffer.start;
}

int guido2img::convert (guidosession* const currentSession)
{
    GuidoPageFormat pf;
    currentSession->fillGuidoPageFormatUsingCurrentSettings(&pf);

    int width = (int) pf.width;
    int height = (int) pf.height;

    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cr = cairo_create (surface);

    CairoSystem sys (cr);

    VGDevice * dev = sys.CreateDisplayDevice();
    dev->SelectFillColor(VGColor(255,255,255));
    dev->Rectangle (0, 0, width, height);
    dev->SelectFillColor(VGColor(0,0,0));
    GuidoOnDrawDesc desc;

    desc.handle = currentSession->getGRHandler();
    desc.hdc = dev;
    desc.page = 1;
    desc.updateRegion.erase = true;
    desc.scrollx = desc.scrolly = 0;
    desc.sizex = width;
    desc.sizey = height;
    desc.isprint = false;
    GuidoErrCode err = GuidoOnDraw (&desc);

    fBuffer.size = 0;
    fBuffer.data = fBuffer.start;
    cairo_surface_write_to_png_stream (surface, write_png_stream_to_byte_array, &fBuffer);
    return err;
}

} // end namespoace
