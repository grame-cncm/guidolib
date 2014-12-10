#ifndef GUIDOSCOREMAPADAPTER_H
#define GUIDOSCOREMAPADAPTER_H
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


/*!
 * \addtogroup APICplusplus C++ interface
 * @{
 * \defgroup ScoreMapAdapter Guido Score Map Adapter
 * \brief A C++ interface to the Guido score map API
 *
 * A C++ class to manipulate score map of GuidoEngine.
 */
class_export GuidoScoreMapAdapter
{
	public:
		GuidoScoreMapAdapter();

		virtual ~GuidoScoreMapAdapter();

		/** \brief Retrieves the graphic to time mapping

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param width the page width.
			\param height the page height.
			\param sel GuidoeElementSelector to filter undesired objects out.
			\param f a MapCollector object that will be called for each selected element or null if error
			\return a Guido error code
		*/
		GuidoErrCode getMap(CGRHandler gr, int pagenum, float width, float height, GuidoElementSelector sel, MapCollector& f);

		/** \brief Retrieves a guido page graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\return contains the mapping on output or null if error
		*/
		Time2GraphicMap getPageMap( CGRHandler gr, int pagenum, float w, float h);

		/** \brief Retrieves a guido staff graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\param staff the staff index (starting from 1).
			\return outmap contains the mapping on output or null if error
		*/
		Time2GraphicMap getStaffMap( CGRHandler gr, int pagenum, float w, float h, int staff);

		/** \brief Retrieves a guido voice graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\param voice the voice index (starting from 1).
			\return outmap contains the mapping on output or null if error
		*/
		Time2GraphicMap getVoiceMap(CGRHandler gr, int pagenum, float w, float h, int voice);

		/** \brief Retrieves a guido system graphic to time mapping.

			\param gr a Guido opaque handle to a GR structure.
			\param pagenum a page index, starting from 1.
			\param w the page width.
			\param h the page height.
			\return outmap contains the mapping on output or null if error
		*/
		Time2GraphicMap getSystemMap(CGRHandler gr, int pagenum, float w, float h);

		/** \brief Retrieves a time segment and the associated graphic segment in a mapping.

			\param date a date used to select the container time segment
			\param map a time to graphic map.
			\param t on output, the time segment containing the date (note that segments are right opened)
			\param r on output, the graphic segment associated to the time segment
			\return false when there is no segment containing the date.
		*/
		bool getTime(const GuidoDate& date, const Time2GraphicMap map, TimeSegment& t, FloatRect& r);

		/** \brief Retrieves a time segment and the associated graphic segment in a mapping.

			\param x a point x coordinate
			\param y a point y coordinate
			\param map a time to graphic map.
			\param r on output, the graphic segment containing the point (note that segments are right and bottom opened)
			\param t on output, the time segment associated to the graphic segment
			\return false when there is no segment containing the point.
		*/
		bool getPoint(float x, float y, const Time2GraphicMap map, TimeSegment& t, FloatRect& r);

		/** \brief Retrieves the rolled to unrolled time mapping

			\param gr a Guido opaque handle to a GR structure.
			\param f a TimeMapCollector object that will be called for each time segment or null if error.
			\return a Guido error code

		*/
		GuidoErrCode getTimeMap(CARHandler gr, TimeMapCollector& f);
};
/*! @} */
#endif // GUIDOSCOREMAPADAPTER_H
