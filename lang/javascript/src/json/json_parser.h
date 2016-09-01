/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef json_parser__
#define json_parser__

#include <iostream>
#include <string>
#include "export.h"

namespace json
{

class json_object;

//--------------------------------------------------------------------------
/* \brief a class for reading json streams
*/
class jsonexport json_parser {
		
	void initScanner();
	void destroyScanner();
	
	public:
		void*			fScanner;	// the flex scanner
		std::istream*	fStream;    // input stream
		std::string		fText;		// the current text
		int				fInt;		// the current int
		float			fFloat;		// the current float
		json_object*	fJSON;		// on parse() output, contains a json object or null is case of error

				 /// \brief constructor, initialized with an input stream
				 json_parser(std::istream* stream);
		virtual ~json_parser();
		
		json_object* parse();		
};

} // end namespace

#endif
