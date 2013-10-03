#ifndef __GuidoStream__
#define __GuidoStream__

/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include <istream>

#include "GuidoStreamBuf.h"

using namespace std;


class GuidoStream : public istream
{
public:
             GuidoStream(GuidoStreamBuf *inStreamBuf);
    virtual ~GuidoStream();

    void WriteToStream(const char* str);

    bool IsParserJobFinished()  { return fParserJobFinished; }
    void CloseStream()          { fGuidoStreamBuffer->SetHaveToCloseStream(); }

    void SetParserJobFinished() { fParserJobFinished = true; }

    bool GetAreAllDataRead()    { return fGuidoStreamBuffer->GetAreAllDataRead(); }

protected:
    GuidoStreamBuf *fGuidoStreamBuffer;

    bool fParserJobFinished;
};


#endif
