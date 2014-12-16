/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "JSONTime2GraphicMap.h"
#include "json_array.h"
#include "json_stream.h"
#include "date_tools.h"
#include "json_object.h"
#include "json_parser.h"
#include "JSONFloatRect.h"
#include "JSONTimeSegment.h"

#include <sstream>
#include <iostream>

using namespace json;

JSONTime2GraphicMap::JSONTime2GraphicMap()
{
}

JSONTime2GraphicMap::JSONTime2GraphicMap(std::string jsonString) {
	// Jsonparser don't work with an anonymous array, it must be encapsulate in an object
	jsonString.insert(0, "{\"array\":");
	jsonString.append("}");

	// Create a stream
	istringstream stream(jsonString);
	// Initialise parser
	json_parser parser(&stream);
	// Parse the string
	json_object * json = parser.parse();

	// Create the Map with json_object.
	const json_element * mainArray = json->getKey("array");
	const json_array_value *v= dynamic_cast<const json_array_value *>(mainArray->value());
	const json_array  *a = v->getValue();


	std::vector<const json_value *> vect =  a->values();
	for (std::vector<const json_value *>::const_iterator it = vect.begin(); it != vect.end(); it++) {
		// Parse elements
		const json_object_value *jv = (*it)->value<json_object_value>();
		const json_object *o = jv->getValue();

		// Graph object
		const json_element * graph = o->getKey("graph");
		const json_object_value * graphOv = dynamic_cast<const json_object_value*>(graph->value());
		const json_object *graphObj = graphOv->getValue();

		// Time object
		const json_element * time = o->getKey("time");
		const json_object_value * timeOv = dynamic_cast<const json_object_value*>(time->value());
		const json_object *timeObj = timeOv->getValue();

		// Add real object to intern vector
		this->push_back(std::pair<TimeSegment, FloatRect>(JSONTimeSegment(*timeObj), JSONFloatRect(*graphObj)));
	}
	delete json;
}

JSONTime2GraphicMap::JSONTime2GraphicMap(Time2GraphicMap map) : Time2GraphicMap(map)
{
}


json_array * JSONTime2GraphicMap::toJsonArray()
{
	json_array * arr = new json_array();
	for (const_iterator i = this->begin(); i != this->end(); i++) {
		// Create float rectangle
		JSONFloatRect rect(i->second);
		json_object*  graph = rect.toJsonObject();

		// Create time segment
		JSONTimeSegment segment(i->first);
		json_object*  mytime = segment.toJsonObject();

		// Add in holder object
		json_object*  holder = new json_object();
		holder->add(new json_element("graph", new json_object_value(graph)));
		holder->add(new json_element("time", new json_object_value(mytime)));
		arr->add(new json_object_value(holder));
	}
	return arr;
}

std::string JSONTime2GraphicMap::toString()
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
