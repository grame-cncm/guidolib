/*

  Copyright (C) 2012 Grame

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#include "guido2img.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "../../Device/GuidoComponent.h"
#include <assert.h>

using namespace std;
namespace guidohttpd
{

//--------------------------------------------------------------------------
/*
Guido2ImageErrorCodes guido2img::convert (guidosession* const currentSession)
{
    Guido2Image::Params p;

    GuidoPageFormat pf;
    currentSession->fillGuidoPageFormatUsingCurrentSettings(&pf);
    GuidoSetDefaultPageFormat(&pf);

    p.input  = currentSession->gmn_.c_str ();
    p.output = 0;
    switch (currentSession->format_) {
    case GUIDO_WEB_API_PNG :
        p.format = GUIDO_2_IMAGE_PNG;
        break;
    case GUIDO_WEB_API_JPEG :
        p.format = GUIDO_2_IMAGE_JPEG;
        break;
    case GUIDO_WEB_API_GIF :
        p.format = GUIDO_2_IMAGE_GIF;
        break;
    case GUIDO_WEB_API_SVG :
        assert (false);
    default :
        p.format = GUIDO_2_IMAGE_PNG;
        break;
    }

    p.layout = 0;
    p.pageIndex = currentSession->page_;
    p.sizeConstraints = QSize (currentSession->width_, currentSession->height_);
    p.zoom = currentSession->zoom_;

    fBuffer.open(QIODevice::ReadWrite);
    fBuffer.reset();
    p.device = &fBuffer;
    Guido2ImageErrorCodes err = Guido2Image::gmnString2Image (p, currentSession->resizeToPage_);
    fBuffer.close();
    return err;
}
*/
//void GuidoViewer::export2Image (const File& file, ImageFileFormat* format)
int guido2img::convert (guidosession* const currentSession)
{

  ImageFileFormat *format;
  switch (currentSession->format_) {
    case GUIDO_WEB_API_PNG :
        format = new PNGImageFormat ();
        break;
    case GUIDO_WEB_API_JPEG :
        format = new JPEGImageFormat ();
        break;
    case GUIDO_WEB_API_GIF :
        format = new GIFImageFormat ();
        break;
    case GUIDO_WEB_API_SVG :
        assert (false);
    default :
        format = new PNGImageFormat ();
        break;
  }
  setGMNCode (currentSession->gmn_.c_str());
  setPage (currentSession->page_);
  setResizePageToMusic (currentSession->resizeToPage_);
  GuidoPageFormat pagef;
  currentSession->fillGuidoPageFormatUsingCurrentSettings(&pagef);
  setGuidoPageFormat(pagef);

  Image::PixelFormat pf = Image::RGB;
  if (currentSession->format_ == GUIDO_WEB_API_PNG)
    pf = Image::ARGB;	

  SoftwareImageType type;
  Image img (pf, currentSession->width_, currentSession->height_, true, type);
  Graphics g (img);
  if (pf == Image::RGB) {
    g.setColour (Colours::white);
    g.fillRect (0, 0, currentSession->width_, currentSession->height_);
  }
  GuidoComponent::paint(g);
  format->writeImageToStream (img, fBuffer);
  delete format;
  return 0; // need to fix this...
}
} // end namespoace
