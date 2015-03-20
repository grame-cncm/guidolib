#ifndef JSONRECTINFOS_H
#define JSONRECTINFOS_H
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
#include <string>
#include "json_object.h"

/*!
 * \brief The JSONGuidoElementInfos class add JSON capabilities to GuidoElementInfos. This class is used to work in json environnement.
 *
 * This object can be convert in json_object or in JSON string.
 * The JSON representation of a JSONGuidoElementInfos object has three elements refering to the GuidoElementInfos C data structure.<br/>
 * Example of JSON representation : <br/>
 * {"staffNum": 1, "voiceNum": 2, "type": 5 }
 */
class JSONGuidoElementInfos : public GuidoElementInfos
{
	public:
		/*!
		 * \brief JSONGuidoElementInfos Create a JSONGuidoElementInfos from a standard GuidoElementInfos.
		 * \param elementInfos
		 */
		JSONGuidoElementInfos(const GuidoElementInfos &elementInfos);
		/*!
		 * \brief toString convert in json string
		 * \return The JSON string repsentation of the object.
		 */
		std::string toString();

		/*!
		 * \brief toJsonObject convert to json object.
		 * It's user responsability to delete the json_object.
		 * \return A pointer on new json_object.
		 */
		json::json_object * toJsonObject();
};

/*!
 * \brief The JSONRectInfos class add JSON capabilities to RectInfos. This class is used to work in json environnement.
 *
 * A RectInfos embed a TimeSegment and a GuidoElementInfos. The Json representation of this object is a json time segment object called "time"
 * and a json GuidoElementInfos object called "infos".<br/>
 * Example of JSON representation :<br>
 * <code>
 * {<br/>
 * "time": {"start": "0/1", "end": "1/1"},<br/>
 * "infos": {"staffNum": 1, "voiceNum": 2, "type": 5 }<br/>
 * }
 * </code>
 */
class JSONRectInfos : public RectInfos
{
	public:
		/*!
		 * \brief JSONRectInfos Create JSONRectInfos from a standard RectInfos.
		 * \param infos
		 */
		JSONRectInfos(const RectInfos &infos);

		/*!
		 * \brief toJsonObject convert to json object.
		 * It's user responsability to delete the json_object
		 * \return a jsonObject
		 */
		json::json_object * toJsonObject();
		/*!
		 * \brief toString convert in json string
		 * \return
		 */
		std::string toString();
};

/*! @} */

#endif // JSONRECTINFOS_H
