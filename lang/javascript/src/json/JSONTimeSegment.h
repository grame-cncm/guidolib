#ifndef JSONTIMESEGMENT_H
#define JSONTIMESEGMENT_H
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
#include <string>
#include "json_object.h"

/*!
 * \addtogroup jsonMapping
 *  @{
 */

/*!
 * \brief The JSONTimeSegment class add JSON capabilities to TimeSegment class. This class is used to work in json environnement.
 *
 * This class can convert TimeSegment in json string or json_object and can be construct from a json_object.
 * The JSON representation of a TimeSegment is an object of two values " start" and "end".<br/>
 * Example of JSON representation :<br/>
 * <code>{"start": "0/1", "end": "1/1"}</code>
 */
class JSONTimeSegment : public TimeSegment
{
	public:
		/*!
		 * \brief JSONTimeSegment. Construct empty time segment like TimeSegment()
		 */
		JSONTimeSegment();

		/*!
		 * \brief JSONTimeSegment constrcut a JSONTimeSegment from a TimeSegment
		 * \param segment
		 */
		JSONTimeSegment(const TimeSegment & segment);

		/*!
		 * \brief JSONTimeSegment construct from json_object
		 * \param object a json_object
		 */
		JSONTimeSegment(const json::json_object & object);

		virtual ~JSONTimeSegment();

		/*!
		 * \brief toString convert the time segment in json string with "start" and "end" for each GuidoDate
		 * \return a json string
		 */
		std::string toString();

		/*!
		 * \brief toJsonObject convert to json object.
		 * It's user responsability to delete the json_object
		 * \return a jsonObject
		 */
		json::json_object * toJsonObject();
};
/*! @} */
#endif // JSONTIMESEGMENT_H
