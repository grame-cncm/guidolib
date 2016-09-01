#ifndef JSONTIME2GRAPHICMAP_H
#define JSONTIME2GRAPHICMAP_H
/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
/*!
 * \addtogroup jsonMapping
 *  @{
 */

#include "GUIDOScoreMap.h"
#include "json_array.h"

#include <string>

/*!
 * \brief The JSONTime2GraphicMap class add JSON capabilities to Time2GraphicMap. This class is used to work in json environnement.
 *
 * This object can be convert in json_object or in JSON string. It can be construct from a JSON string.
 * The JSON representation of a MapElement is an array of pair of JSONTimeSegment and JSONFloatRect called respectively "graph" and "time".<br>
 * Example of JSON representation :<br/>
 * <code>
 * [<br/>
 * &nbsp;{<br/>
 * &nbsp;&nbsp;"graph": {"left": 80, "top": 0, "right": 122, "bottom": 451 },<br/>
 * &nbsp;&nbsp;"time": {"start": "0/1", "end": "4/1"}<br/>
 * &nbsp;},<br/>
 * &nbsp;{<br/>
 * &nbsp;&nbsp;"graph": {"left": 80, "top": 0, "right": 122, "bottom": 451 },<br/>
 * &nbsp;&nbsp;"time": {"start": "0/1", "end": "4/1"}<br/>
 * &nbsp;}<br/>
 * ]
 * </code>
 */
class JSONTime2GraphicMap : public Time2GraphicMap
{
	public:
		/*!
		 * \brief JSONTime2GraphicMap
		 */
		JSONTime2GraphicMap();

		/*!
		 * \brief JSONTime2GraphicMap construct a Time2GraphicMap from a json string.
		 * \param jsonString
		 */
		JSONTime2GraphicMap(std::string jsonString);

		/*!
		 * \brief JSONTime2GraphicMap construct from a Time2GraphicMap
		 * \param map
		 */
		JSONTime2GraphicMap(Time2GraphicMap map);

		/*!
		 * \brief toJsonArray convert in json object. It's user responsability to delete the json_array.
		 * \return
		 */
		json::json_array * toJsonArray();

		/*!
		 * \brief toString convert in json string
		 * \return
		 */
		std::string toString();
};

/*! @} */
#endif // JSONTIME2GRAPHICMAP_H
