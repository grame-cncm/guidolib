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
#include "GUIDOPianoRoll.h"
#include <string>

/*!
 * \addtogroup jsonMapping GUIDO Score Map to json
 * \brief This set of classes is used in web environnement to return json string instead of complex C++ structure.
 *   @{
 */
/*! \brief The Map2json class is used in web environnement to have time map in javascript.
 *
 * This class is bind to javascript with emscripten with the name GUIDOScoreMap.
 * The real GUIDOScoreMapAdapter is not ported to javascript, it return complex C++ object.
 * All the API of GuidoScoreMap is include here but return json string.
 * The GUIDOPianoRollAdapter.getMap method is called getPianoRollMap in this class.
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

		/** \brief Retrieves the rolled to unrolled time mapping

			\param gr a Guido opaque handle to a GR structure.
			\return a json string TimeMapCollector object or a empty string if error.
		*/
		std::string getTimeMap(CARHandler gr);

		/*!
			\brief Gets the piano roll map
			\param pr a pianoroll previously created with ar2PianoRoll or midi2PianoRoll
			\param width the width of the piano roll (-1 to set the default width : 1024)
			\param height the height of the canvas (-1 to set the default height : 512)
			\return a json string which represent a Time2GraphicMap instance
		*/
		std::string getPianoRollMap(PianoRoll *pr, int width, int height);

	private:
		GuidoScoreMapAdapter fScoreMap;
};
/*! @} */
#endif
