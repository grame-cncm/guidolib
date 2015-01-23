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
#include "json2osc.h"
#ifndef JSON_ONLY
# include "osc_stream.h"
#endif

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
}

// --------------------------------------------------------------
#ifndef JSON_ONLY
void json_object::print(osc_stream& out) const
{
	const json_element* list = getKey (kOSCMarker);

	if (list)								// check for the osc marker
		list->value()->print(out);
	else {
		const json_element* address = getKey (kAddressField);
		const json_element* params = getKey (kParamsField);
		if (address && params) {		// check if osc address and params are present (paramas could be empty)
			const json_string_value* oscaddress = address->value()->value<json_string_value>();
			if (oscaddress) {								// check that address contains a string
				if (out.state() == osc_stream::kIdle) {
					out.start(oscaddress->getValue().c_str());	// starts a new osc packet with this address

					const json_element* dest = getKey (kDestField);
					if (dest) {									// check for an optional destination field
						const json_int_value* ipnum = dest->value()->value<json_int_value>();
						const json_string_value* host = dest->value()->value<json_string_value>();
						if (ipnum) out.setAddress (ipnum->getValue());
						else if (host) out.setAddress (host->getValue());
					}

					const json_element* port = getKey (kPortField);
					if (port) {									// check for an optional port field
						const json_int_value* oscport = port->value()->value<json_int_value>();
						if (oscport) out.setPort (oscport->getValue());
					}
				}
				else out << oscaddress->getValue().c_str();	// its a message inside a message
				params->value()->print (out);				// and put the parameters
				out.end();									// and finally send the message
			}
		}
	}
}
#endif

} // end namespace
