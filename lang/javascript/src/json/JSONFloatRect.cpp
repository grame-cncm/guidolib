/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#include "JSONFloatRect.h"

#include "json_value.h"
#include "json_element.h"
#include <sstream>

using namespace json;

JSONFloatRect::JSONFloatRect() : FloatRect()
{
}

JSONFloatRect::JSONFloatRect(const json_object &object)
{
	//Graph object content
	const json_element * left = object.getKey("left");
	this->left = static_cast<const json_float_value *>(left->value())->getValue();

	const json_element * top = object.getKey("top");
	this->top = static_cast<const json_float_value *>(top->value())->getValue();

	const json_element * right = object.getKey("right");
	this->right = static_cast<const json_float_value *>(right->value())->getValue();

	const json_element * bottom = object.getKey("bottom");
	this->bottom = static_cast<const json_float_value *>(bottom->value())->getValue();
}

JSONFloatRect::JSONFloatRect(const FloatRect &rect) : FloatRect(rect)
{
}

JSONFloatRect::~JSONFloatRect()
{
}

std::string JSONFloatRect::toString()
{
	json_object * jsonObject = toJsonObject();
	std::ostringstream mystream;
	json_stream jstream(mystream);
	jsonObject->print(jstream);
	delete jsonObject;
	return mystream.str();
}

json::json_object * JSONFloatRect::toJsonObject()
{
	json_object*  graph = new json_object();
	graph->add(new json_element("left", new json_float_value(this->left)));
	graph->add(new json_element("top", new json_float_value(this->top)));
	graph->add(new json_element("right", new json_float_value(this->right)));
	graph->add(new json_element("bottom", new json_float_value(this->bottom)));
	return graph;
}
