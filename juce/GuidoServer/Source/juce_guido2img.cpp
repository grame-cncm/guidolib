/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "juce_guido2img.h"

#include <assert.h>

using namespace std;
namespace guidohttpd
{

//--------------------------------------------------------------------------
int juce_guido2img::convert (guidosession* const currentSession)
{
  ImageFileFormat *format;

  switch (currentSession->getFormat()) {
  case GUIDO_WEB_API_SVG : {
      string svg;
      int err = currentSession->simpleSVGHelper(fSvgFontFile, &svg);
      const char* cc_svg = svg.c_str();
      fBuffer.write(cc_svg, svg.size());
      return err;
  }
  case GUIDO_WEB_API_PNG : {
      format = new PNGImageFormat();
      } break;
  case GUIDO_WEB_API_JPEG : {
      format = new JPEGImageFormat();
      } break;
  case GUIDO_WEB_API_GIF : {
      format = new GIFImageFormat();
      } break;
  default :
      return guidoErrActionFailed;
  }

  setGMNCode (currentSession->getGMN().c_str());
  setPage (currentSession->getPage());
  setResizePageToMusic (currentSession->getResizeToPage());
  GuidoPageFormat pagef;
  currentSession->fillGuidoPageFormatUsingCurrentSettings(&pagef);
  setGuidoPageFormat(pagef);

  Image::PixelFormat pf = Image::RGB;
  if (currentSession->getFormat() == GUIDO_WEB_API_PNG)
    pf = Image::ARGB;

  SoftwareImageType type;
  Image img (pf, currentSession->getWidth(), currentSession->getHeight(), true, type);
  Graphics g (img);
  if (pf == Image::RGB) {
    g.setColour (Colours::white);
    g.fillRect (0.0, 0.0, currentSession->getWidth(), currentSession->getHeight());
  }
  GuidoComponent::paint(g);
  format->writeImageToStream (img, fBuffer);
  delete format;
  return 0; // need to fix this...

}

} // end namespoace
