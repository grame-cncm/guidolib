/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "json_object.h"
#include "json_stream.h"

using namespace std;
namespace json
{
 
// --------------------------------------------------------------
json_object::~json_object()
{
	unsigned int n = fElements.size();
	for (unsigned int i=0; i < n; i++) {
		delete fElements[i];
	}
}

// --------------------------------------------------------------
const json_element* json_object::getKey (const char* key) const
{
	unsigned int n = fElements.size();
	for (unsigned int i=0; i < n; i++) {
		if (fElements[i]->name() == key) return fElements[i];
	}
	return 0;
}
 
// --------------------------------------------------------------
void json_object::print(std::ostream& os) const
{
	json_stream out (os);
	print (out);
}

// --------------------------------------------------------------
void json_object::print(json_stream& out) const
{
	out << "{" << out.nl()++;
	if (fElements.size() > 0) {
		unsigned int n = fElements.size() - 1;
		for (unsigned int i=0; i < n; i++) {
			out << *fElements[i] << ',' << out.nl();
		}
		out << *fElements[n] << out.nl()--;
	}
	out << "}";
};

} // end namespace
