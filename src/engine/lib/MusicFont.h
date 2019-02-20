#ifndef GUIDOFONT_H
#define GUIDOFONT_H
/*
  GUIDO Library
  Copyright (C) 2014-2019	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "StaticFont.h"

class MusicFont : public StaticFont
{
    public:
    	enum { kFontSize=200, kGuidoFontHeight=499, kBravuraFontHeight = 804 };

				 MusicFont(const char * name, int size, int properties) : StaticFont(name, size, properties) { initialize(); }
        virtual ~MusicFont() {}

    protected:
        virtual void initialize();		// must initialize the fExtends maps, the height and the ref size
};


#endif
