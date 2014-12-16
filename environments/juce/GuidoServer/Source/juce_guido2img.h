/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __juce_guido2img__
#define __juce_guido2img__

#include "guidosession.h"
#include "../JuceLibraryCode/JuceHeader.h"
#include "guido2img.h"
#include "GuidoComponent.h"

namespace guidohttpd
{
class guidosession;
//--------------------------------------------------------------------------
class juce_guido2img :
                       public guido2img,
                       public GuidoComponent
{
    MemoryOutputStream fBuffer;
public:
    juce_guido2img(string svgfontfile) : guido2img(svgfontfile) {}
    virtual ~juce_guido2img() {}

    int convert (guidosession *currentSession);

    const char* data()	{
        return (const char *)fBuffer.getData();
    }
    int			size()	{
        return fBuffer.getDataSize();
    }
};

} // end namespoace

#endif
