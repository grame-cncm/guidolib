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
class GuidoMapCollector: public MapCollector
{
	public :
		struct Filter { virtual ~Filter() {} virtual bool operator() (const GuidoElementInfos& infos) const { return true; } } ;

				 GuidoMapCollector(CGRHandler gr, GuidoeElementSelector selector, const Filter* filter=0) 
					: fGRHandler(gr), fSelector(selector), fFilter(filter), fOutMap(0) {}
		virtual ~GuidoMapCollector() {}

		///< the method called by guido for each graphic segment
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
		
		void add (const TimeSegment& ts, const FloatRect& r)	{ fOutMap->push_back(make_pair(ts, r)); }

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
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );

	public :
				 GuidoVoiceCollector(CGRHandler gr, int num) : GuidoMapCollector(gr, kGuidoEvent) { if (num) setFilter(num); }
		virtual ~GuidoVoiceCollector() {}
};


//----------------------------------------------------------------------
/*!
	\brief a guido map collector that combines a guido events and staff mappings
*/
class GuidoStaffCollector: public GuidoMapCollector
{
	private:
		typedef std::pair<TimeSegment, FloatRect>	TMapElt;
		std::vector<TMapElt>	fMap;
		int		fStaffNum;
		bool	fNoEmpty;

	void mergelines (const std::vector<TMapElt>& elts, Time2GraphicMap& outmap) const;

	public :
				 GuidoStaffCollector (CGRHandler gr, int num) : GuidoMapCollector(gr, kGuidoStaff), fStaffNum(num), fNoEmpty(true) {}
		virtual ~GuidoStaffCollector()	{}

		///< overrides the method called by guido for each graphic segment
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
};

//----------------------------------------------------------------------
/*!
	\brief a guido map collector adjusting system to to slices start
*/
class GuidoSystemCollector: public GuidoMapCollector
{
	typedef std::pair<TimeSegment, FloatRect>	TMapElt;
	std::vector<TMapElt>	fMap;
	
	public :
				 GuidoSystemCollector(CGRHandler gr) : GuidoMapCollector(gr, kGuidoSystem) { }
		virtual ~GuidoSystemCollector() {}

		///< overrides the method called by guido for each graphic segment
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
		virtual void processNoDiv (int page, float w, float h, Time2GraphicMap* outmap);
		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
};

/*!@} */

} // end namespoace

#endif
