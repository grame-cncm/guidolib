/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
#include "JSONRectInfos.h"
#include "JSONTimeSegment.h"
#include <sstream>

using namespace json;

JSONGuidoElementInfos::JSONGuidoElementInfos(const GuidoElementInfos &elementInfos)
{
	staffNum = elementInfos.staffNum;
	voiceNum = elementInfos.voiceNum;
	type = elementInfos.type;
}

std::string JSONGuidoElementInfos::toString()
{
	json_object * jsonObject = toJsonObject();
	std::ostringstream mystream;
	json_stream jstream(mystream);
	jsonObject->print(jstream);
	delete jsonObject;
	return mystream.str();
}

json::json_object * JSONGuidoElementInfos::toJsonObject()
{
	json_object * jsonObject = new json_object();
	jsonObject->add(new json_element("staffNum", new json_int_value(this->staffNum)));
	jsonObject->add(new json_element("voiceNum", new json_int_value(this->voiceNum)));
	jsonObject->add(new json_element("type", new json_int_value(this->type)));
	return jsonObject;
}

JSONRectInfos::JSONRectInfos(const RectInfos &infos) : RectInfos(infos.time(), infos.infos())
{
}

json::json_object * JSONRectInfos::toJsonObject()
{
	json_object * jsonObject = new json_object();
	jsonObject->add(new json_element("time", new json_object_value(JSONTimeSegment(this->time()).toJsonObject())));
	jsonObject->add(new json_element("infos", new json_object_value(JSONGuidoElementInfos(this->infos()).toJsonObject())));
	return jsonObject;
}

std::string JSONRectInfos::toString()
{
	json_object * jsonObject = toJsonObject();
	std::ostringstream mystream;
	json_stream jstream(mystream);
	jsonObject->print(jstream);
	delete jsonObject;
	return mystream.str();
}
