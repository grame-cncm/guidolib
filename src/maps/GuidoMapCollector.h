/*

  GUIDO Library

  Copyright (C) 2011  Grame

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/


#ifndef __GuidoMapCollector__
#define __GuidoMapCollector__

#include <map>
#include <string>

#include "GUIDOScoreMap.h"

namespace guido
{

/*!
\addtogroup guidomap Guido graphic to time maps
@{
*/

//----------------------------------------------------------------------
/*!
	\brief a class to collect guido graphic maps
*/
//class _TimeSegment: public TimeSegment
//{
//	public:
//			_TimeSegment (const TimeSegment& s) : TimeSegment(s) {}
//		bool empty() const;
//		bool intersect(const TimeSegment& ts) const;
//		bool include(const TimeSegment& ts) const;
//		bool operator < (const TimeSegment& ts) const;
//};

//----------------------------------------------------------------------
/*!
	\brief a class to collect guido graphic maps
*/
class GuidoMapCollector: public MapCollector
{
	public :
//		typedef std::map<_TimeSegment, FloatRect> Time2GraphicMap;
		struct Filter { virtual ~Filter() {} virtual bool operator() (const GuidoElementInfos& infos) const { return true; } } ;


				 GuidoMapCollector(CGRHandler gr, GuidoeElementSelector selector, const Filter* filter=0) 
					: fGRHandler(gr), fSelector(selector), fFilter(filter), fOutMap(0) {}
		virtual ~GuidoMapCollector() {}

		///< the method called by guido for each graphic segment
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
		
	protected:
		CGRHandler				fGRHandler;
		GuidoeElementSelector	fSelector;
		const Filter*			fFilter;
		Time2GraphicMap*		fOutMap;
};

//----------------------------------------------------------------------
/*!
	\brief a guido map collector filtering voice events
*/
class GuidoVoiceCollector: public GuidoMapCollector
{
	private:
		struct AcceptVoicePredicat : public Filter { 
			int fNum;
			virtual bool operator() (const GuidoElementInfos& infos) const { return (infos.voiceNum == fNum) && (infos.type != kEmpty); }
		} fVoiceFilter;
		void	setFilter(int num)			{ fVoiceFilter.fNum = num; fFilter = &fVoiceFilter; }

	public :
				 GuidoVoiceCollector(CGRHandler gr, int num) 
					: GuidoMapCollector(gr, kGuidoEvent) { if (num) setFilter(num); }
		virtual ~GuidoVoiceCollector() {}
};


//----------------------------------------------------------------------
/*!
	\brief a guido map collector that combines a guido events and staff mappings
*/
class GuidoStaffCollector: public GuidoMapCollector
{
	private:
		struct AcceptStaffPredicat : public Filter {
			int	fNum;
			virtual bool operator() (const GuidoElementInfos& infos) const { return infos.staffNum == fNum; }
		} fStaffFilter;
		void	setFilter(int num)			{ fStaffFilter.fNum = num; fFilter = &fStaffFilter; }

	public :
				 GuidoStaffCollector (CGRHandler gr, int num) 
					: GuidoMapCollector(gr, kGuidoStaff) { if (num) setFilter(num); }
		virtual ~GuidoStaffCollector()	{}

		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
};

//----------------------------------------------------------------------
/*!
	\brief a guido map collector adjusting system to to slices start
*/
class GuidoSystemCollector: public GuidoMapCollector
{
	void merge (const Time2GraphicMap& map1, const Time2GraphicMap& map2, Time2GraphicMap& outmap);
	
	public :
				 GuidoSystemCollector(CGRHandler gr) 
					: GuidoMapCollector(gr, kGuidoSystem) { }
		virtual ~GuidoSystemCollector() {}

		virtual void processNoDiv (int page, float w, float h, Time2GraphicMap* outmap);
		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
};

/*!@} */

} // end namespoace

#endif
