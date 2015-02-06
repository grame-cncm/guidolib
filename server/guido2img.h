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

#include <string>

class PianoRoll;

namespace guidohttpd
{
class guidosession;
typedef struct scoreParameters GuidoSessionScoreParameters;
typedef struct pianorollParameters GuidoSessionPianorollParameters;

//--------------------------------------------------------------------------

class guido2img
{
protected:
    std::string fSvgFontFile;
public:
    guido2img(std::string svgfontfile) : fSvgFontFile(svgfontfile) {}
    virtual ~guido2img() {}

    virtual int convertScore (guidosession *currentSession, GuidoSessionScoreParameters &scoreParameters) = 0;
    virtual int convertPianoRoll (PianoRoll * pr, GuidoSessionPianorollParameters &pianorollParameters) = 0;
    virtual const char* data() = 0;
    virtual int	size() = 0;
};

} // end namespoace

#endif
