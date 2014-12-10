/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "JSONMapElement.h"
#include "JSONFloatRect.h"
#include "JSONRectInfos.h"
#include "json_value.h"
#include "json_array.h"
#include <vector>
#include <sstream>

using namespace json;

JSONMapElement::JSONMapElement(const MapElement &element): pair(element.first, element.second)
{
}

JSONMapElement::~JSONMapElement()
{
}

json_object * JSONMapElement::toJsonObject()
{
	json_object * myElement = new json_object();
	myElement->add(new json_element("rect", new json_object_value(JSONFloatRect(this->first).toJsonObject())));
	myElement->add(new json_element("rectInfos", new json_object_value(JSONRectInfos(this->second).toJsonObject())));
	return myElement;
}

std::string JSONMapElement::toString()
{
	json_object * jsonObject = toJsonObject();
	std::ostringstream mystream;
	json_stream jstream(mystream);
	jsonObject->print(jstream);
	delete jsonObject;
	return mystream.str();
}

std::string JSONMapElement::toString(const std::vector<MapElement> &vect)
{
	json_array * arr = new json_array();
	for (std::vector<MapElement>::const_iterator i = vect.begin(); i != vect.end(); i++) {
		arr->add(new json_object_value(JSONMapElement(*i).toJsonObject()));
	}
	std::ostringstream mystream;
	json_stream jstream(mystream);
	arr->print(jstream);
	delete arr;
	return mystream.str();
}
