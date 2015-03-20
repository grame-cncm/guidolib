#ifndef JSONMAPELEMENT_H
#define JSONMAPELEMENT_H
/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOScoreMap.h"
#include "json_object.h"
#include <string>

/*!
 * \addtogroup jsonMapping
 *  @{
 */

/*!
 * \brief The JSONMapElement class add JSON capabilities to MapElement class. This class is used to work in json environnement.
 *
 * The JSON representation of a MapElement is a JSONFloatRect called "rect" and a JSONRectInfos called "rectInfos".<br/>
 * Example of JSON representation :<br/>
 * <code>
 * {<br/>
 * "rect": {"left": 80, "top": 0, "right": 122, "bottom": 451 }<br/>
 * "rectInfos": {"staffNum": 1, "voiceNum": 2, "type": 5 }<br/>
 * }
 * </code>
 */
class JSONMapElement : public MapElement
{
	public:
		JSONMapElement();
		JSONMapElement(const MapElement &element);
		virtual ~JSONMapElement();

		/*!
		 * \brief toJsonObject convert to json object.
		 * It's user responsability to delete the json_object
		 * \return a jsonObject
		 */
		json::json_object * toJsonObject();

		/*!
		 * \brief toString convert in json string
		 * \return a json string
		 */
		std::string toString();

		/*!
		 * \brief toString static method to simply convert a vector of MapElement in
		 * a json string wich represent an array of MapElement.
		 * \param vect
		 * \return a json string
		 */
		static std::string toString(const std::vector<MapElement>& vect);
};
/*! @} */
#endif // JSONMAPELEMENT_H
