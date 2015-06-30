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


class GuidoStream
{
public:
             GuidoStream() {}
    virtual ~GuidoStream() {}

    void     WriteToStream(const char* str);

    void     ReinitStream();

		  std::string  getStreamStr() const;
    const std::string  getGuidoCode() const		{ return fGMNCode;}

protected:
    std::stringstream	fInputStream;	// The stringstream which contains the real string
										// written by the user since the beginning
    std::string			fGMNCode;		// The completed input stream (syntactically correct gmn code)

    /* Builds the tags ( "{", "[", "(", "<" ) stack from inStr */
    void	AnalyzeString(const std::stringstream& inStr, std::stack<char>& stack);

    /* Complete stringToComplete with appropriate tags contained in inStack */
    void    WriteNewString(std::stack<char>& inStack, std::string& stringToComplete);
};


#endif
