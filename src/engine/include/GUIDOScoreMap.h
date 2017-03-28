#ifndef GUIDOScoreMap_H
#define GUIDOScoreMap_H

/*
  GUIDO Library
  Copyright (C) 2004  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4661)		// don't know how to solve it
#endif

#include "GUIDOExport.h"
#include "GUIDOEngine.h"
#include "TRect.h"

#include <utility>
#include <vector>
#include <map>
#include <iostream>


/*!
\addtogroup Mapping GUIDO Mapping
@{
 */

//------------------------------------------------------------------------------
// graphic elements selector definitions
typedef enum { 
	kGuidoPage, kGuidoSystem, kGuidoSystemSlice, kGuidoStaff, /*kGuidoMeasure,*/ kGuidoBar, kGuidoBarAndEvent, kGuidoEvent,
	kGuidoScoreElementEnd
} GuidoElementSelector;

// graphic elements type definitions
typedef enum { 
	kNote = 1, kRest, kEmpty, kBar, kRepeatBegin, kRepeatEnd, kStaff, kSystemSlice, kSystem, kPage, kGraceNote
} GuidoElementType;

// elements infos struct
typedef struct {
	GuidoElementType type;		///< the element type
	int			     staffNum;	///< the element staff number or 0 when na
	int			     voiceNum;	///< the element voice number or 0 when na
    int              midiPitch; ///< the element midi pitch, or -1 when na
} GuidoElementInfos;


//----------------------------------------------------------------------
/*!
	\brief a time segment definition and operations
*/
class_export TimeSegment: public std::pair<GuidoDate, GuidoDate>
{
	public:
				 TimeSegment () {}
				 TimeSegment (const TimeSegment& s) : std::pair<GuidoDate, GuidoDate>(s.first, s.second) {}
				 TimeSegment (const GuidoDate& a, const GuidoDate& b) : std::pair<GuidoDate, GuidoDate>(a, b) {}
		virtual ~TimeSegment () {}

		void print(std::ostream& out) const;					///< print the time segment 
		bool empty() const;										///< check for empty segment
		bool intersect(const TimeSegment& ts) const;			///< check for segments intersection
		bool include(const GuidoDate& date) const;				///< check for date inclusion
		bool include(const TimeSegment& ts) const;				///< check for segment inclusion
		bool startEqual(const TimeSegment& ts) const;			///< check if start date is the same
		bool operator < (const TimeSegment& ts) const;			///< order relationship: the smaller is the smaller first date
		bool operator == (const TimeSegment& ts) const;
		TimeSegment operator & (const TimeSegment& ts) const;	///< intersection operation (may return an arbitrary empty segment)
};

typedef std::pair<TimeSegment, FloatRect>	TMapSegments;
typedef std::vector<TMapSegments>			Time2GraphicMap;
typedef Time2GraphicMap::const_iterator		Time2GraphicMapIterator;

inline std::ostream& operator << (std::ostream& out, const GuidoDate& d) {
	out << d.num << "/" << d.denom;
	return out;
}

inline std::ostream& operator << (std::ostream& out, const TimeSegment& s) { 
	s.print(out); 
	return out;
}


//------------------------------------------------------------------------------
// mapping collector abstract class definition
class MapCollector
{
	public:
		virtual ~MapCollector() {}
		
		/** \brief a method called by the GuidoGetMap function

			\param box a graphic rectangle expressed with no scaling and no coordinates offset.
			\param dates a time segment containing the corresponding start and end dates
			\param infos information about the corresponding element.
		*/
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos ) = 0;
};

//------------------------------------------------------------------------------
class RectInfos {
		TimeSegment			fTime;
		GuidoElementInfos	fInfos;
	public:
				 RectInfos (const TimeSegment& ts, const GuidoElementInfos& infos) : fTime(ts), fInfos(infos) {}
		virtual ~RectInfos () {}
		
		const TimeSegment&			time() const	{ return fTime; }
		const GuidoElementInfos&	infos() const	{ return fInfos; }
};

typedef std::pair<FloatRect, RectInfos>	 MapElement;



//------------------------------------------------------------------------------
// time mapping collector abstract class definition
class TimeMapCollector
{
	public:
		virtual ~TimeMapCollector() {}

		/** \brief a method called by the GuidoGetTimeMap function

			\param from a time segment expressed in score time i.e. rolled time.
			\param to a time segment expressed in unrolled time
		*/
		virtual void Time2TimeMap( const TimeSegment& from, const TimeSegment& to ) = 0;
};

typedef std::pair<TimeSegment, TimeSegment> TimeMapElement;

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Retrieves the graphic to time mapping, without any processing (RAW)

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param width the page width.
	\param height the page height.
	\param sel a GuidoElementSelector to filter undesired objects out.
	\param f a MapCollector object that will be called for each selected element.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetMap( CGRHandler gr, int pagenum, float width, float height, 
									 GuidoElementSelector sel, MapCollector& f);

/** \brief Retrieves a guido staff graphic to time mapping, without any processing (RAW).

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param w the page width.
	\param h the page height.
	\param staff the staff index (starting from 1).
	\param outmap contains the mapping on output.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetRAWStaffMap( CGRHandler gr, int pagenum, float w, float h, int staff, Time2GraphicMap& outmap);

/** \brief Retrieves a guido voice graphic to time mapping, without any processing (RAW).

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param w the page width.
	\param h the page height.
	\param voice the voice index (starting from 1).
	\param outmap contains the mapping on output.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetRAWVoiceMap( CGRHandler gr, int pagenum, float w, float h, int voice, Time2GraphicMap& outmap);

/** \brief Retrieves a guido system graphic to time mapping, without any processing (RAW). 

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param w the page width.
	\param h the page height.
	\param outmap contains the mapping on output.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetRAWSystemMap( CGRHandler gr, int pagenum, float w, float h, Time2GraphicMap& outmap);

/** \brief Retrieves a guido page graphic to time mapping. 

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param w the page width.
	\param h the page height.
	\param outmap contains the mapping on output.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetPageMap( CGRHandler gr, int pagenum, float w, float h, Time2GraphicMap& outmap);

/** \brief Retrieves a guido staff graphic to time mapping. 

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param w the page width.
	\param h the page height.
	\param staff the staff index (starting from 1).
	\param outmap contains the mapping on output.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetStaffMap( CGRHandler gr, int pagenum, float w, float h, int staff, Time2GraphicMap& outmap);

/** \brief Retrieves a guido voice graphic to time mapping. 

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param w the page width.
	\param h the page height.
	\param voice the voice index (starting from 1).
	\param outmap contains the mapping on output.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetVoiceMap( CGRHandler gr, int pagenum, float w, float h, int voice, Time2GraphicMap& outmap);

/** \brief Retrieves a guido system graphic to time mapping. 

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param w the page width.
	\param h the page height.
	\param outmap contains the mapping on output.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetSystemMap( CGRHandler gr, int pagenum, float w, float h, Time2GraphicMap& outmap);

/** \brief Retrieves a time segment and the associated graphic segment in a mapping.

	\param date a date used to select the container time segment
	\param map a time to graphic map.
	\param t on output, the time segment containing the date (note that segments are right opened)
	\param r on output, the graphic segment associated to the time segment
	\return false when there is no segment containing the date.
*/
GUIDOAPI(bool)	GuidoGetTime( const GuidoDate& date, const Time2GraphicMap map, TimeSegment& t, FloatRect& r);

/** \brief Retrieves a time segment and the associated graphic segment in a mapping.

	\param x a point x coordinate
	\param y a point y coordinate
	\param map a time to graphic map.
	\param r on output, the graphic segment containing the point (note that segments are right and bottom opened)
	\param t on output, the time segment associated to the graphic segment
	\return false when there is no segment containing the point.
*/
GUIDOAPI(bool)	GuidoGetPoint( float x, float y, const Time2GraphicMap map, TimeSegment& t, FloatRect& r);

/** \brief Retrieves the graphic to time mapping corresponding to the SVG output

	\param gr a Guido opaque handle to a GR structure.
	\param pagenum a page index, starting from 1.
	\param sel GuidoElementSelector to filter undesired objects out.
	\param outMap on output: a vector containing the map elements
	\return an error code.
*/
#ifdef WIN32
    __declspec(deprecated("Deprecated function (not maintained code)."))
#endif
GUIDOAPI(GuidoErrCode)	GuidoGetSVGMap( GRHandler gr, int pagenum, GuidoElementSelector sel, std::vector<MapElement>& outMap) GUIDOAPI_deprecated;

/** \brief Retrieves the rolled to unrolled time mapping

	\param gr a Guido opaque handle to a GR structure.
	\param f a TimeMapCollector object that will be called for each time segment.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetTimeMap( CARHandler gr, TimeMapCollector& f);


/*! @} */

#ifdef __cplusplus
}
#endif

#endif



