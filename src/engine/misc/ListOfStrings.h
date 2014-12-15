#ifndef ListOfStrings_H
#define ListOfStrings_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <vector>
#include "nvstring.h"

class ListOfStrings : public std::vector<NVstring>
{ 
	public:
		void AddTail( const char * str )			{ push_back (str); }	
		void AddTail( const NVstring & str )		{ push_back (str); }
};


#endif
