/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __qt_guido2img__
#define __qt_guido2img__

#include <QtCore/QBuffer>
#include "Guido2Image.h"
#include "guidosession.h"
#include "guido2img.h"

namespace guidohttpd
{
class guidosession;
//--------------------------------------------------------------------------
class qt_guido2img : public guido2img
{
    QBuffer		fBuffer;
public:
    qt_guido2img(string svgfontfile) : guido2img(svgfontfile) {}
    virtual ~qt_guido2img() {}

    int convert (guidosession *currentSession);

    const char* data()	{
        return fBuffer.data().constData();
    }
    int			size()	{
        return fBuffer.size();
    }
};

} // end namespoace

#endif
