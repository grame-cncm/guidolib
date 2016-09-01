/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef JSONFRIENDLYTIMEMAP_H
#define JSONFRIENDLYTIMEMAP_H

#include "GUIDOScoreMap.h"
#include "json_array.h"

#include <string>
#include <vector>
#include <utility>

/*!
 * \addtogroup jsonMapping
 *  @{
 */

/*!
 * \brief The JSONFriendlyTimeMap class is a implementation of TimeMapCollector.
 * This class is used to work in json environnement.
 *
 * This class has capabilities to be convert in json_array or in JSON string.
 * The JSON representation of the TimeMapCollector is constituted of pairs of time segment,
 * a time segment for the score and a time segment for the performance respectively called "score" and "perf".<br/>
 * Example of JSON representation of an array of 4 elements :<br/>
 * <code>
 * [<br/>
 * &nbsp;{"score": {"start": "0/1", "end": "1/1"}, "perf":  {"start": "0/1", "end": "1/1"} },<br/>
 * &nbsp;{"score": {"start": "1/1", "end": "2/1"}, "perf": {"start": "1/1", "end": "2/1"} },<br/>
 * &nbsp;{"score": {"start": "2/1", "end": "3/1"}, "perf":  {"start": "0/1", "end": "1/1"} },<br/>
 * &nbsp;{"score": {"start": "3/1", "end": "4/1"}, "perf": {"start": "1/1", "end": "2/1"} }<br/>
 * ]
 * </code>
 */
class JSONFriendlyTimeMap : public TimeMapCollector
{
	public:
		JSONFriendlyTimeMap();
		virtual ~JSONFriendlyTimeMap();

		virtual void Time2TimeMap( const TimeSegment& from, const TimeSegment& to );

		/*!
		 * \brief toJsonArray convert in json_array
		 * It's user responsability to delete json_array.
		 * \return
		 */
		json::json_array * toJsonArray();

		/*!
		 * \brief toString convert in json string
		 * \return a json string
		 */
		std::string toString();

	private :
		std::vector<TimeMapElement> fSegments;
};
/*! @} */
#endif // JSONFRIENDLYTIMEMAP_H
