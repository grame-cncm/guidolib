/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "JSONFriendlyTimeMap.h"
#include <sstream>
#include "json_object.h"
#include "json_stream.h"
#include "date_tools.h"

using namespace json;

JSONFriendlyTimeMap::JSONFriendlyTimeMap()
{

}

JSONFriendlyTimeMap::~JSONFriendlyTimeMap()
{

}

void JSONFriendlyTimeMap::Time2TimeMap( const TimeSegment& from, const TimeSegment& to ) {
	fSegments.push_back (TimeMapElement(from, to));
}

json_array * JSONFriendlyTimeMap::toJsonArray()
{
	// Create a new json array
	json_array * arr = new json_array();
	for (int i = 0; i < (int)(fSegments.size()); i++) {
		json_object*  holder = new json_object();
		json_object*  score = new json_object();
		json_object*  perf = new json_object();
		score->add(new json_element("start", new json_string_value(dateToString(fSegments[i].first.first).c_str())));
		score->add(new json_element("end", new json_string_value(dateToString(fSegments[i].first.second).c_str())));
		perf->add(new json_element("start", new json_string_value(dateToString(fSegments[i].second.first).c_str())));
		perf->add(new json_element("end", new json_string_value(dateToString(fSegments[i].second.second).c_str())));
		holder->add(new json_element("score", new json_object_value(score)));
		holder->add(new json_element("perf", new json_object_value(perf)));
		arr->add(new json_object_value(holder));
	}
	return arr;
}

std::string JSONFriendlyTimeMap::toString()
{
	// Convert into json_array (printable)
	json_array * arr = toJsonArray();

	// Print in a json stream (with specific end line) and return the string
	std::ostringstream mystream;
	json_stream jstream(mystream);
	arr->print(jstream);
	delete arr;
	return mystream.str();
}
