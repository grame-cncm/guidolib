#ifndef NVstring_H
#define NVstring_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003,2004	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "stdlib.h"
#include <string>

class NVstring : public std::string
{
	public:
						 NVstring();
						 NVstring(const char * p);
						 NVstring(const std::string& str);
						 NVstring(const NVstring & in);
		virtual 		~NVstring();

			void 			to_lower();
			size_t 			replace( char c1, char c2 );
	static	std::string		to_lower(const char* str);
};

#endif
