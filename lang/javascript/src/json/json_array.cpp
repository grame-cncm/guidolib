/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "json_array.h"
#include "json_value.h"
#include "json_stream.h"

using namespace std;

namespace json
{
 
// --------------------------------------------------------------
json_array::~json_array()
{
	unsigned int n = fValues.size();
	for (unsigned int i=0; i < n; i++) {
		delete fValues[i];
	}
}

// --------------------------------------------------------------
#ifndef JSON_ONLY
void json_array::print(osc_stream& out) const
{
	unsigned int n = fValues.size();
	for (unsigned int i=0; i < n; i++) {
		fValues[i]->print(out);
	}	
}
#endif

// --------------------------------------------------------------
void json_array::print(json_stream& out) const
{
	out << "[" << out.nl()++;
	if (fValues.size() > 0) {
		unsigned int n = fValues.size() - 1;
		for (unsigned int i=0; i < n; i++) {
			out << *fValues[i] << ',' << out.nl();
		}
		out << *fValues[n] << out.nl()--;
	}
	out << "]";
}

} // end namespoace
