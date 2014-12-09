#ifndef MAP2JSON_H
#define MAP2JSON_H

/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GUIDOScoreMapAdapter.h"
#include <string>

/*!
 * \brief The Map2json class.
 * This class is used in web environnement. It's the same API as GuidoScoreMap API but return json string.
 */
class Map2json
{
	public:
		/** \brief Retrieves a guido page graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\return a json string of JSONTime2GraphicMap, it contains the mapping on output or null if error
		*/
		std::string getPageMap(CGRHandler gr, int pagenum, float w, float h);

		/** \brief Retrieves a guido staff graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\param staff the staff index (starting from 1).
			\return a json string of JSONTime2GraphicMap, it contains the mapping on output or null if error
		*/
		std::string getStaffMap(CGRHandler gr, int pagenum, float w, float h, int staff);

		/** \brief Retrieves a guido staff graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\param staff the staff index (starting from 1).
			\return a json string of JSONTime2GraphicMap, it contains the mapping on output or null if error
		*/
		std::string getVoiceMap(CGRHandler gr, int pagenum, float w, float h, int voice);

		/** \brief Retrieves a guido system graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\return a json string of JSONTime2GraphicMap, it contains the mapping on output or null if error
		*/
		std::string getSystemMap(CGRHandler gr, int pagenum, float w, float h);

		/*!
		 * \brief getTime
		 * \param date
		 * \param jsonTime2GraphicMap in json form
		 * \return a JSON string. It represent a Time2GraphicMap with only one element (a pair TimeSegment / FloatRect)
		 */
		std::string getTime(const GuidoDate& date, std::string jsonTime2GraphicMap);
		/*!
		 * \brief getPoint
		 * \param x
		 * \param y
		 * \param jsonTime2GraphicMap
		 * \return a JSON string. It represent a Time2GraphicMap with only one element (a pair TimeSegment / FloatRect)
		 */
		std::string getPoint(float x, float y, std::string jsonTime2GraphicMap);

		/** \brief Retrieves the graphic to time mapping corresponding to the SVG output

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param sel GuidoeElementSelector to filter undesired objects out.
			\return a json string : a vector containing the map elements or empty if error
		*/
		std::string getSVGMap( GRHandler gr, int pagenum, GuidoElementSelector sel);

		/** \brief Retrieves the rolled to unrolled time mapping

			\param gr a Guido opaque handle to a GR structure.
			\return a json string TimeMapCollector object or a empty string if error.
		*/
		std::string getTimeMap(CARHandler gr);

	private:
		GuidoScoreMapAdapter scoreMap;
};

#endif
