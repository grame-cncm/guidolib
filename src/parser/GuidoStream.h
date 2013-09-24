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

#include <iostream>
//#include <string>
#include <locale.h>

/*#include "GuidoFactory.h"

 class ARFactory;*/

namespace guido
{

    class GuidoStream {

        public:

                     GuidoStream();
            virtual ~GuidoStream();
    
            std::istream* getStream() {return fStream;}

            void WriteStream(const char* str);

        protected:

            std::istream* fStream;
    };
    
} // end namespace

#endif
