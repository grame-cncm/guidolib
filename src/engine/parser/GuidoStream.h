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
#include <sstream>
#include <stack>
#include "ARFactory.h"

using namespace std;


class GuidoStream : public stringstream
{
public:
             GuidoStream();
    virtual ~GuidoStream();

    void     WriteToStream(const char* str);
    

    /* Clears this' state flags */
    void     Prepare();
    void     ReinitStream();
    stringstream * getGlobalStringStream() {return fTheGlobalStringStream;}

protected:
    stringstream *fTheGlobalStringStream; // The stringstream which contains the real string
                                          // written by the user since the beginning

    /* Builds the tags ( "{", "[", "(", "<" ) stack from inStr */
    stack<char> *AnalyzeString(stringstream *inStr);

    /* Complete stringToComplete with appropriate tags contained in inStack */
    void         WriteNewString(stack<char> *inStack, string *stringToComplete);
};


#endif
