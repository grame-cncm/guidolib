/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __guido2img__
#define __guido2img__

#include <QBuffer>
#include "Guido2Image.h"

namespace guidohttpd
{

//--------------------------------------------------------------------------
class guido2img
{
	QBuffer		fBuffer;
	public:
				 guido2img() {}
		virtual ~guido2img() {}

		Guido2ImageErrorCodes convert (const char* gmn, int page, int width, int height, float zoom=1.0f);

		const char* data()	{ return fBuffer.data().constData(); }
		int			size()	{ return fBuffer.size(); }
};

} // end namespoace

#endif
