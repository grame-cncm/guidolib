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
 * \brief The JSONGuidoElementInfos class
 */
class JSONGuidoElementInfos : public GuidoElementInfos
{
	public:
		/*!
		 * \brief JSONGuidoElementInfos
		 * \param elementInfos
		 */
		JSONGuidoElementInfos(const GuidoElementInfos &elementInfos);
		/*!
		 * \brief toString convert in json string
		 * \return
		 */
		std::string toString();

		/*!
		 * \brief toJsonObject convert to json object.
		 * It's user responsability to delete the json_object
		 * \return a jsonObject
		 */
		json::json_object * toJsonObject();
};

/*!
 * \brief The JSONRectInfos class
 */
class JSONRectInfos : public RectInfos
{
	public:
		/*!
		 * \brief JSONRectInfos
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
