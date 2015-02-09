/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "string.h"
#include "cairo_guido2img.h"
#include "CairoSystem.h"
#include "CairoDevice.h"
#include "SVGSystem.h"
#include "SVGDevice.h"
#include "BinarySystem.h"
#include "BinaryDevice.h"

#include <cairo.h>
#include <Magick++.h>

#include <assert.h>

using namespace std;
namespace guidohttpd
{

// ------------------
// magic++


// ------------------
static cairo_status_t
write_png_stream_to_byte_array (void *in_closure, const unsigned char *data,
                                                unsigned int length)
{
    png_stream_to_byte_array_closure_t *closure =
        (png_stream_to_byte_array_closure_t *) in_closure;

    memcpy (closure->data_, data, length);
    closure->data_ += length;
    closure->size_ += length;
    return CAIRO_STATUS_SUCCESS;
}

//--------------------------------------------------------------------------
cairo_guido2img::cairo_guido2img (string svgfontfile) : guido2img(svgfontfile) {
  fBuffer.data_ = new char[1048576];
  fBuffer.start_ = fBuffer.data_;
  fBuffer.size_ = 0;
  fBuffer.pos_ = 0;
}

cairo_guido2img::~cairo_guido2img () {
  delete[] fBuffer.start_;
}

int cairo_guido2img::convertScore (guidosession* const currentSession, GuidoSessionScoreParameters &scoreParameters)
{
    fBuffer.reset();
	if (scoreParameters.format == GUIDO_WEB_API_SVG) {
		// return Svg format
	  stringstream svg;
	  int err = currentSession->svgScoreExport(fSvgFontFile, scoreParameters.page, &svg);
	  const char* cc_svg = svg.str().c_str();
	  fBuffer.size_ = svg.str().size();
      strcpy(fBuffer.data_, cc_svg);
      return err;
    }
	else if (scoreParameters.format == GUIDO_WEB_API_BINARY) {
		// Return a binary export (draw commands in binary format)
      stringstream stream;
	  int err = currentSession->binaryScoreExport(&stream, scoreParameters.page);
      string ss_str = stream.str();
      const char *ss_cstr = ss_str.c_str();
      fBuffer.size_ = ss_str.size();
      memcpy(fBuffer.data_, ss_cstr, ss_str.size());
      return err;
    }

	// Create an image with native system drawing. (Default to png format)
	// The page format can have change if the option resizepage to music in used.
	GuidoPageFormat curFormat;
	GuidoGetPageFormat(currentSession->getGRHandler(), scoreParameters.page, &curFormat);
	int width = curFormat.width;
	int height = curFormat.height;

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
	desc.page = scoreParameters.page;
    desc.updateRegion.erase = true;
    desc.scrollx = desc.scrolly = 0;
    desc.sizex = width;
    desc.sizey = height;
    desc.isprint = false;
    GuidoErrCode err = GuidoOnDraw (&desc);

    cairo_surface_write_to_png_stream (surface, write_png_stream_to_byte_array, &fBuffer);

	// If format is not png, convert the image with magick++.
	if ((scoreParameters.format == GUIDO_WEB_API_JPEG)
		|| (scoreParameters.format == GUIDO_WEB_API_GIF)) {
      // magick++
      Magick::Blob in_blob(fBuffer.start_, fBuffer.size_);
      Magick::Image image(in_blob);
      Magick::Blob out_blob;
	  string new_format = ((scoreParameters.format == GUIDO_WEB_API_JPEG) ? "JPEG" : "GIF");
      image.magick(new_format);
      image.write(&out_blob);
      memcpy(fBuffer.start_, out_blob.data(), out_blob.length());
      fBuffer.size_ = out_blob.length();
    }

    return err;
}

int cairo_guido2img::convertPianoRoll(PianoRoll *pr, GuidoSessionPianorollParameters &pianorollParameters)
{
	fBuffer.reset();
	stringstream out;

	VGSystem *sys;
	VGDevice *dev;

	cairo_surface_t *surface;

	int width = pianorollParameters.width;
	int height = pianorollParameters.height;

	if (pianorollParameters.format == GUIDO_WEB_API_SVG) {
		// Create a svg device and svg system
		sys = new SVGSystem(fSvgFontFile.c_str());
		dev = ((SVGSystem*) sys)->CreateDisplayDevice(out, 0);

		dev->NotifySize(width, height);
	}
	else if (pianorollParameters.format == GUIDO_WEB_API_BINARY) {
		// Return a binary export (draw commands in binary format)
		sys = new BinarySystem;
		dev = ((BinarySystem*) sys)->CreateDisplayDevice(out);
		dev->NotifySize(width, height);
	} else {
		// Use host to draw the score.
		cairo_t *cr;
		surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
		cr = cairo_create (surface);

		sys = new CairoSystem(cr);
		dev = sys->CreateDisplayDevice();
		// Init background
		dev->SelectFillColor(VGColor(255,255,255));
		dev->Rectangle (0, 0, width, height);
		dev->SelectFillColor(VGColor(0,0,0));
	}

	// Draw piano roll

	GuidoErrCode error = GuidoPianoRollOnDraw (pr, width, height, dev);

	if(pianorollParameters.format == GUIDO_WEB_API_SVG || pianorollParameters.format == GUIDO_WEB_API_BINARY) {
		string ss_str = out.str();
		const char *ss_cstr = ss_str.c_str();
		fBuffer.size_ = ss_str.size();
		memcpy(fBuffer.data_, ss_cstr, ss_str.size());
	} else {
		cairo_surface_write_to_png_stream (surface, write_png_stream_to_byte_array, &fBuffer);
	}
	// If format is not png, convert the image with magick++.
	if ((pianorollParameters.format == GUIDO_WEB_API_JPEG)
		|| (pianorollParameters.format == GUIDO_WEB_API_GIF)) {
	  // magick++
	  Magick::Blob in_blob(fBuffer.start_, fBuffer.size_);
	  Magick::Image image(in_blob);
	  Magick::Blob out_blob;
	  string new_format = ((pianorollParameters.format == GUIDO_WEB_API_JPEG) ? "JPEG" : "GIF");
	  image.magick(new_format);
	  image.write(&out_blob);
	  memcpy(fBuffer.start_, out_blob.data(), out_blob.length());
	  fBuffer.size_ = out_blob.length();
	}

	delete sys;
	delete dev;

	return error;
}

} // end namespoace
