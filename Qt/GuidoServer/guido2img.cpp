/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <QtCore/QBuffer>

#include "guido2img.h"
#include "Guido2Image.h"

#include <assert.h>

using namespace std;
namespace guidohttpd
{

//--------------------------------------------------------------------------
int guido2img::convert (guidosession* const currentSession)
{
    Guido2Image::Params p;

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

    GuidoPageFormat pf;
    currentSession->fillGuidoPageFormatUsingCurrentSettings(&pf);
    p.pageFormat = &pf;

    p.resizePageToMusic = currentSession->resizeToPage_;

    GuidoLayoutSettings ls;
    currentSession->fillGuidoLayoutSettingsUsingCurrentSettings(&ls);
    p.layout = &ls;

    p.pageIndex = currentSession->page_;
    p.sizeConstraints = QSize (GuidoCM2Unit(currentSession->width_), GuidoCM2Unit(currentSession->height_));
    p.zoom = currentSession->zoom_;

    fBuffer.open(QIODevice::ReadWrite);
    fBuffer.reset();
    p.device = &fBuffer;
    Guido2ImageErrorCodes err = Guido2Image::gmnString2Image (p);
    fBuffer.close();
    return err == GUIDO_2_IMAGE_SUCCESS ? 0 : 1;
}

} // end namespoace
