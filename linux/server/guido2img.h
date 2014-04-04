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

#include <cairo.h>
#include "guidosession.h"

namespace guidohttpd
{
class guidosession;
//--------------------------------------------------------------------------

typedef struct
{
    char *data;
    char *current;
    char *start;
    int size;
} png_stream_to_byte_array_closure_t;

class guido2img
{
  png_stream_to_byte_array_closure_t  fBuffer;
public:
    guido2img();
    virtual ~guido2img();

    int convert (guidosession *currentSession);

    const char* data()	{
        return fBuffer.start;
    }
    int			size()	{
        return fBuffer.size;
    }
};

} // end namespoace

#endif
