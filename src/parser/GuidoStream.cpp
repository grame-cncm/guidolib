/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#include <locale.h>
#include <istream>
#include <sstream>

using namespace std;

#include "GuidoStreamBuf.h"

#include "GuidoStream.h"


//--------------------------------------------------------------------------
GuidoStream::GuidoStream(GuidoStreamBuf *inStreamBuf) : istream(inStreamBuf),
             fGuidoStreamBuffer(inStreamBuf),
             fParserJobFinished(false),
             fSynchronousString()
{
}

//--------------------------------------------------------------------------
GuidoStream::~GuidoStream() 
{
    delete fGuidoStreamBuffer;
}

//--------------------------------------------------------------------------
void GuidoStream::WriteToStream(const char* str) 
{
    fSynchronousString += str;
    fGuidoStreamBuffer->WriteToBuffer(str);
}