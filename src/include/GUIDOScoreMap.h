#ifndef GUIDOMap_H
#define GUIDOMap_H

/*
	GUIDO Library
	Copyright (C) 2004  Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "GUIDOExport.h"
#include "GUIDOEngine.h"
#include "TRect.h"

#include <utility>
#include <vector>


/*!
\addtogroup Mapping GUIDO Mapping
@{
 */

//------------------------------------------------------------------------------
// graphic elements selector definitions
typedef enum { 
	kGuidoPage, kGuidoSystem, kGuidoSystemSlice, kGuidoStaff, /*kGuidoMeasure,*/ kGuidoBar, kGuidoEvent, 
	kGuidoScoreElementEnd
} GuidoeElementSelector;

// graphic elements type definitions
typedef enum { 
	kNote = 1, kRest, kEmpty, kBar, kRepeatBegin, kRepeatEnd, kStaff, kSystemSlice, kSystem, kPage
} GuidoElementType;

typedef struct {
	GuidoElementType type;		///< the element type
	int			staffNum;		///< the element staff number or 0 when na
	int			voiceNum;		///< the element voice number or 0 when na
} GuidoElementInfos;

typedef std::pair<GuidoDate, GuidoDate>		TimeSegment;

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

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Retrieves the graphic to time mapping

	\param inHandleGR a Guido opaque handle to a GR structure.
	\param inPage a page index, starting from 1.
	\param width the page width.
	\param height the page height.
	\param sel GuidoeElementSelector to filter undesired objects out.
	\param f a MapCollector object that will be called for each selected element.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetMap( CGRHandler inHandleGR, int inPage, float width, float height, 
									 GuidoeElementSelector sel, MapCollector& f);

/** \brief Retrieves the graphic to time mapping corresponding to the SVG output

	\param inHandleGR a Guido opaque handle to a GR structure.
	\param inPage a page index, starting from 1.
	\param sel GuidoeElementSelector to filter undesired objects out.
	\param outMap on output: a vector containing the map elements
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetSVGMap( GRHandler inHandleGR, int inPage, GuidoeElementSelector sel, std::vector<MapElement>& outMap);

/** \brief Retrieves the rolled to unrolled time mapping

	\param inHandleGR a Guido opaque handle to a GR structure.
	\param f a TimeMapCollector object that will be called for each time segment.
	\return an error code.
*/
GUIDOAPI(GuidoErrCode)	GuidoGetTimeMap( CARHandler inHandleGR, TimeMapCollector& f);


/*! @} */

#ifdef __cplusplus
}
#endif

#endif



