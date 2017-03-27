/*

  GUIDO Library
  Copyright (C) 2011  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __GuidoMapCollector__
#define __GuidoMapCollector__

#include <map>
#include <string>
#include <ostream>

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
		struct Filter {
			virtual ~Filter() {}
			virtual bool operator() (const GuidoElementInfos& infos) const { return true; }
			public:
				bool fGetRAWdata;
		};

				 GuidoMapCollector(CGRHandler gr, GuidoElementSelector selector, const Filter* filter=0) 
					: fGRHandler(gr), fSelector(selector), fFilter(filter), fOutMap(0) {}
		virtual ~GuidoMapCollector() {}

		///< the method called by guido for each graphic segment
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
		
		void add (const TimeSegment& ts, const FloatRect& r)	{ fOutMap->push_back(make_pair(ts, r)); }

    protected:
        void getEvents (int page, float w, float h, bool keepLeftmost, Time2GraphicMap& outmap, int staffNum = 0);
    
		CGRHandler				fGRHandler;
		GuidoElementSelector	fSelector;
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
			virtual bool operator() (const GuidoElementInfos& infos) const { return (infos.voiceNum == fNum) && (fGetRAWdata || infos.type != kEmpty); }
		} fVoiceFilter;
		void	setFilter(int num, bool getRAWdata)			{ fVoiceFilter.fNum = num; fVoiceFilter.fGetRAWdata = getRAWdata; fFilter = &fVoiceFilter; }
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );

	public :
				 GuidoVoiceCollector(CGRHandler gr, int num, bool getRAWdata = false) : GuidoMapCollector(gr, kGuidoEvent) { if (num) setFilter(num, getRAWdata); }
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
		bool    fGetRAWdata;

	void mergelines (const std::vector<TMapElt>& elts, Time2GraphicMap& outmap) const;

	public :
				 GuidoStaffCollector (CGRHandler gr, int num, bool getRAWdata = false) : GuidoMapCollector(gr, kGuidoStaff), fStaffNum(num), fNoEmpty(true), fGetRAWdata(getRAWdata) { }
		virtual ~GuidoStaffCollector()	{}

		///< overrides the method called by guido for each graphic segment
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
		virtual void process (int page, float w, float h, Time2GraphicMap* outmap);
};
inline std::ostream& operator<< (std::ostream& os, const std::vector<std::pair<TimeSegment, FloatRect> >& v) {
	for (unsigned int i=0; i<v.size(); i++) os << v[i].first << " -> " << v[i].second << std::endl;
	return os; }

//----------------------------------------------------------------------
/*!
	\brief a guido map collector adjusting system to slices start
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

//----------------------------------------------------------------------
/*!
 \brief a guido map collector retrieving the list of kNote/kRest events. For each
        kRest starting a measure, its box left is aligned on the measure left barline.
 */
class GuidoVoiceAndBarCollector: public GuidoMapCollector
{
    typedef std::pair<TimeSegment, FloatRect>	TMapElt;
    std::vector<TMapElt>	fMap;
    int		fStaffNum;
    
    public :
                 GuidoVoiceAndBarCollector(CGRHandler gr, int staffNum = 0) : GuidoMapCollector(gr, kGuidoBarAndEvent), fStaffNum(staffNum) { }
        virtual ~GuidoVoiceAndBarCollector() {}
    
        ///< overrides the method called by guido for each graphic segment
        virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates,  const GuidoElementInfos& infos );
    
    private:
        std::map<int, int> fPrevBarX; // Associates for each staffnum the x position of the previous bar, and 0 if previous element is not a bar
};

/*!@} */

} // end namespoace

#endif
