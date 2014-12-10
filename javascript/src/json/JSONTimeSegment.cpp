/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#include "JSONTimeSegment.h"
#include "json_value.h"
#include "json_element.h"
#include "date_tools.h"
#include "GUIDOEngine.h"
#include <sstream>

using namespace json;

JSONTimeSegment::JSONTimeSegment() : TimeSegment()
{
}

JSONTimeSegment::JSONTimeSegment(const TimeSegment &segment) : TimeSegment(segment)
{
}

JSONTimeSegment::JSONTimeSegment(const json_object &object)
{
	// json object content
	const json_element * start = object.getKey("start");
	string startValue = static_cast<const json_string_value *>(start->value())->getValue();
	GuidoDate startDate;
	stringToDate(startValue, startDate);

	const json_element * end = object.getKey("end");
	string endValue = static_cast<const json_string_value *>(end->value())->getValue();
	GuidoDate endDate;
	stringToDate(endValue, endDate);

	this->first = startDate;
	this->second = endDate;
}

JSONTimeSegment::~JSONTimeSegment()
{

}

std::string JSONTimeSegment::toString()
{
	json_object * jsonObject = toJsonObject();
	std::ostringstream mystream;
	json_stream jstream(mystream);
	jsonObject->print(jstream);
	delete jsonObject;
	return mystream.str();
}

json::json_object * JSONTimeSegment::toJsonObject()
{
	json_object * mytime = new json_object();
	mytime->add(new json_element("start", new json_string_value(dateToString(this->first).c_str())));
	mytime->add(new json_element("end", new json_string_value(dateToString(this->second).c_str())));
	return mytime;
}
