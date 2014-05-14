/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "json_value.h"
#include "json_object.h"
#include "json_array.h"
#include "json_stream.h"

using namespace std;

namespace json
{

//--------------------------------------------------------------------------
json_object_value::~json_object_value()					{ delete fValue; }
void json_object_value::print(json_stream& out) const	{ out << fValue; }
#ifndef JSON_ONLY
void json_object_value::print(osc_stream& out) const	{ fValue->print(out); }
#endif

//--------------------------------------------------------------------------
json_array_value::~json_array_value()					{ delete fValue; }
void json_array_value::print(json_stream& out) const	{ out << fValue; }
#ifndef JSON_ONLY
void json_array_value::print(osc_stream& out) const		{ fValue->print(out); }
#endif


//--------------------------------------------------------------------------
json_string_value::json_string_value(const char* val)
{
	while (*val) {
		if ((*val == '\\') && (val[1])) {
			if ((val[1] == '"') || (val[1] == '\\') || (val[1] == '/')) {
				fValue += val[1];
				val++;
			}
			else fValue += *val;
		}
		else fValue += *val;
		val++;
	}
}

//--------------------------------------------------------------------------
string	json_string_value::to_json() const
{
	const char * ptr = fValue.c_str();
	string outstr;
	while (*ptr) {
		switch (*ptr) {
			case '\\' :
				outstr += "\\\\";
				break;
			case '"' :
				outstr += "\\\"";
				break;
			default:
				outstr += *ptr;
		}
		ptr++;
	}
	return outstr;
}


} // end namespoace
