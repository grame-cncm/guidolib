/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <QtCore/QBuffer>

#include "qt_guido2img.h"
#include "Guido2Image.h"

#include <assert.h>

using namespace std;
namespace guidohttpd
{

//--------------------------------------------------------------------------
int qt_guido2img::convert (guidosession* const currentSession)
{
    Guido2Image::Params p;

    p.input  = currentSession->getGMN().c_str ();
    p.output = 0;
    switch (currentSession->getFormat()) {
    case GUIDO_WEB_API_SVG : {
        string svg;
        int err = currentSession->simpleSVGHelper(fSvgFontFile, &svg);
        const char* cc_svg = svg.c_str();
        fBuffer.setData(cc_svg, svg.size());
        return err;
    }
    case GUIDO_WEB_API_BINARY : {
        stringstream stream;
        int err = currentSession->simpleBinaryHelper(&stream);
        string ss_str = stream.str();
        const char *ss_cstr = ss_str.c_str();
        fBuffer.setData(ss_cstr, ss_str.size());
        return err;
    }
    case GUIDO_WEB_API_PNG : {
        p.format = GUIDO_2_IMAGE_PNG;
        } break;
    case GUIDO_WEB_API_JPEG : {
        p.format = GUIDO_2_IMAGE_JPEG;
        } break;
    case GUIDO_WEB_API_GIF : {
        p.format = GUIDO_2_IMAGE_GIF;
        } break;
    default :
        return guidoErrActionFailed;
    }

    GuidoPageFormat pf;
    currentSession->fillGuidoPageFormatUsingCurrentSettings(&pf);
    p.pageFormat = &pf;

    p.resizePageToMusic = currentSession->getResizeToPage();

    GuidoLayoutSettings ls;
    currentSession->fillGuidoLayoutSettingsUsingCurrentSettings(&ls);
    p.layout = &ls;

    p.pageIndex = currentSession->getPage();
    p.sizeConstraints = QSize (GuidoCM2Unit(currentSession->getWidth()), GuidoCM2Unit(currentSession->getHeight()));
    p.zoom = currentSession->getZoom();

    fBuffer.open(QIODevice::ReadWrite);
    fBuffer.reset();
    p.device = &fBuffer;
    Guido2ImageErrorCodes err = Guido2Image::gmnString2Image (p);
    fBuffer.close();
    return err == GUIDO_2_IMAGE_SUCCESS ? 0 : 1;
}

} // end namespoace
