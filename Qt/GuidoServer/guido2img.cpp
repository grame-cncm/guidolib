/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <QBuffer>

#include "guido2img.h"
#include "Guido2Image.h"

using namespace std;
namespace guidohttpd
{

//--------------------------------------------------------------------------
Guido2ImageErrorCodes guido2img::convert (const char* gmn, int page, int width, int height, float zoom)
{
	Guido2Image::Params p;
	p.input  = gmn;	
	p.output = 0;
	p.format = GUIDO_2_IMAGE_PNG;
	p.layout = 0;
	p.pageIndex = page;
	p.sizeConstraints = QSize (width, height);
	p.zoom = zoom;

	fBuffer.reset();
	p.device = &fBuffer;
	return Guido2Image::gmnString2Image (p);
}

} // end namespoace
