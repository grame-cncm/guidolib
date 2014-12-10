/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "map2json.h"

#include "JSONFriendlyTimeMap.h"
#include "JSONTime2GraphicMap.h"

using namespace std;

//----------------------------------------------------------------------
string Map2json::getPageMap(CGRHandler gr, int pagenum, float w, float h)
{
	JSONTime2GraphicMap outmap = scoreMap.getPageMap(gr, pagenum, w, h);
	return outmap.toString();
}

//----------------------------------------------------------------------
string Map2json::getStaffMap(CGRHandler gr, int pagenum, float w, float h, int staff)
{
	JSONTime2GraphicMap outmap = scoreMap.getStaffMap(gr, pagenum, w, h, staff);
	return outmap.toString();
}

//----------------------------------------------------------------------
string Map2json::getVoiceMap(CGRHandler gr, int pagenum, float w, float h, int voice)
{
	JSONTime2GraphicMap outmap = scoreMap.getVoiceMap(gr, pagenum, w, h, voice);
	return outmap.toString();
}

//----------------------------------------------------------------------
string Map2json::getSystemMap(CGRHandler gr, int pagenum, float w, float h)
{
	JSONTime2GraphicMap outmap = scoreMap.getSystemMap(gr, pagenum, w, h);
	return outmap.toString();
}
//----------------------------------------------------------------------
string Map2json::getTime(const GuidoDate& date, std::string jsonTime2GraphicMap)
{
	Time2GraphicMap map = JSONTime2GraphicMap(jsonTime2GraphicMap);
	TimeSegment s;
	FloatRect r;
	if(scoreMap.getTime(date, map, s, r)) {
		JSONTime2GraphicMap map;
		map.push_back(pair<TimeSegment, FloatRect>(s, r));
		return map.toString();
	}
	return "";
}

//----------------------------------------------------------------------
string Map2json::getPoint(float x, float y, std::string jsonTime2GraphicMap)
{
	Time2GraphicMap map = JSONTime2GraphicMap(jsonTime2GraphicMap);
	TimeSegment s;
	FloatRect r;
	if(scoreMap.getPoint(x, y, map, s, r)) {
		JSONTime2GraphicMap map;
		map.push_back(pair<TimeSegment, FloatRect>(s, r));
		return map.toString();
	}
	return "";
}

//----------------------------------------------------------------------
string Map2json::getTimeMap(CARHandler gr)
{
	JSONFriendlyTimeMap timeMap;
	scoreMap.getTimeMap(gr, timeMap);
	return timeMap.toString();
}

//----------------------------------------------------------------------
string Map2json::getPianoRollMap(PianoRoll *pr, int width, int height)
{
	JSONTime2GraphicMap timeMap;
	GuidoPianoRollGetMap(pr, width, height, timeMap);
	return timeMap.toString();
}
