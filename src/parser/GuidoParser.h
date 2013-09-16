/*
 GUIDO Library
 Copyright (C) 2013 Grame
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 
 */

#ifndef __GuidoParser__
#define __GuidoParser__

#include <iostream>
#include <string>
#include <locale.h>

#include "GuidoFactory.h"

class ARFactory;

namespace guido
{

/* \brief a class for reading gmn streams
*/
class GuidoParser {
		
	void initScanner();
	void destroyScanner();
	
	public:
		void*			fScanner;	// the flex scanner
		std::istream*	fStream;    // input stream
		ARFactory *		fFactory;
		std::string		fText;		// the current text
		

				 GuidoParser(std::istream* stream);
		virtual ~GuidoParser();
		
		ARHandler           parse();
        const ARFactory *   getFactory() const  { return fFactory; }
};

} // end namespace

#endif
