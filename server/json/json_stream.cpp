/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "json_stream.h"
#include "json_object.h"
#include "json_array.h"

namespace json
{

//______________________________________________________________________________
json_stream& operator<< (json_stream& os, const json_object* obj)	{ obj->print(os); return os; }
json_stream& operator<< (json_stream& os, const json_array* arr)	{ arr->print(os); return os; }

//______________________________________________________________________________
void json_endl::print(std::ostream& os) const { 
	int i = fIndent;
    os << std::endl;
    while (i-- > 0)  os << "	";
}

} // end namespoace
