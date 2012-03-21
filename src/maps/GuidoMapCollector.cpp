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

#include <iostream>
#include <algorithm>

#include "GuidoMapCollector.h"

using namespace std;

namespace guido
{

//----------------------------------------------------------------------
// tools 
//----------------------------------------------------------------------
class _GuidoDate : public GuidoDate
{
	public:
			_GuidoDate () { num = 0; denom = 1; }
			_GuidoDate (const GuidoDate& d) : GuidoDate(d) {}
			_GuidoDate (int n, int d)  { num = n; denom = d;}

			 operator float () const { return num / (float)denom; }
		bool operator == (const _GuidoDate& d) const { 
			float diff = float(*this) - float(d);
			if (diff < 0) diff = -diff; 
			return diff < 0.0001; 
		}
		bool operator > (const _GuidoDate& d) const { return float(*this) > float(d); }
		bool operator >= (const _GuidoDate& d) const { return (*this == d) || (*this > d); }
};

//----------------------------------------------------------------------
// tools for computing the system map
//----------------------------------------------------------------------
class _Interval : public std::pair<float, float>
{
	public:
		_Interval (float a, float b) : std::pair<float, float>(a, b) {}
		bool empty ()				{ return (second - first) <= 0; }
};

//----------------------------------------------------------------------
static bool intersect (const FloatRect& r1, const FloatRect& r2)
{
	return ( r1.left < r2.right ) && ( r1.right > r2.left ) && ( r1.top < r2.bottom ) && ( r1.bottom > r2.top );
}

//----------------------------------------------------------------------
void GuidoMapCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
	// Guido mapping filtering: the Guido map doesn't fit the Interlude mapping constraints and should be reworked.
	if (fFilter && !(*fFilter)(infos))	return;		// element is filtered out 
	if ( dates.empty() )	return;					// empty time segments are filtered out
	if ( !box.IsValid() )	return;					// empty graphic segments are filtered out

	for (Time2GraphicMap::const_iterator i = fOutMap->begin(); i != fOutMap->end(); i++) {
		if ( i->first.intersect( dates ) )
			return;									// intersecting segments are filtered out
		if ( intersect( i->second, box ) )
			return;
	}
	add (dates, box);
}

//----------------------------------------------------------------------
void GuidoVoiceCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
	// Guido mapping filtering: the Guido map doesn't fit the Interlude mapping constraints and should be reworked.
	if (fFilter && !(*fFilter)(infos))	return;		// element is filtered out 
	if ( dates.empty() )	return;					// empty time segments are filtered out
	if ( !box.IsValid() )	return;					// empty graphic segments are filtered out

	for (Time2GraphicMap::const_iterator i = fOutMap->begin(); i != fOutMap->end(); i++) {
		if ( intersect( i->second, box ) )
			return;
	}
	add (dates, box);
}

//----------------------------------------------------------------------
void GuidoMapCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	fOutMap = outmap;
	if (!fOutMap) return;
	GuidoGetMap( fGRHandler, page, w, h, fSelector, *this );
}

//----------------------------------------------------------------------
// merge all the semgents from one graphic line in a single segment
//----------------------------------------------------------------------
void GuidoStaffCollector::mergelines (const std::vector<TMapElt>& elts, Time2GraphicMap& outmap) const
{
	if (elts.empty()) return;
	if (elts.size() == 1) {
		outmap.push_back (make_pair(elts[0].first, elts[0].second));
		return;
	}

	TimeSegment linetime;
	FloatRect linerect;	
	bool start = true; 
	float ypos = 0;
	for (std::vector<TMapElt>::const_iterator i = elts.begin(); i != elts.end(); i++) {
		TimeSegment t = i->first;
		FloatRect r = i->second;					// get the current staff rect
		bool linechange = start || (ypos != r.top);	// same line ?
		ypos = r.top;
		if (linechange) {
			if (start) start = false;				// first time: nothing to store
			else outmap.push_back(make_pair(linetime, linerect));	// store the previous line rect
			linetime.first = t.first;				// and prepare the next line
			linerect = r;
		}
		else {
			linetime.second = t.second;				// extend the current time segment
			linerect.right = r.right;				// and the current line rect
		}
	}
	outmap.push_back(make_pair(linetime, linerect));
}

//----------------------------------------------------------------------
// copy the sorted map elements to the outmap 
// from elements starting at the same date, only the first one is retained
//----------------------------------------------------------------------
static void reduce (const std::vector<std::pair<TimeSegment, FloatRect> >& elts, Time2GraphicMap& outmap)
{
	float last = -1.;
	for (std::vector<std::pair<TimeSegment, FloatRect> >::const_iterator i = elts.begin(); i != elts.end(); i++) {
		const GuidoDate current = i->first.first;
		float d = current.num / (float)current.denom;
		float diff = (current.num / (float)current.denom) - last;
		if (diff > 0.0001)	outmap.push_back( make_pair(i->first, i->second));	// skip all segments at the same date
		last = d;
	}
}

//----------------------------------------------------------------------
void GuidoStaffCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
	if ( fNoEmpty && dates.empty() )	return;				// empty time segments are filtered out
	if ( !box.IsValid() )				return;				// empty graphic segments are filtered out
	if ( infos.type == kEmpty)			return;
	if (infos.staffNum == fStaffNum)
		fMap.push_back (make_pair(dates, box));
}

//----------------------------------------------------------------------
static void staffmerge (const Time2GraphicMap& staffMap, const Time2GraphicMap& evtsMap, Time2GraphicMap& outmap)
{
	outmap.clear();
	Time2GraphicMap map;

	Time2GraphicMap::const_iterator ev = evtsMap.begin();
	for (Time2GraphicMap::const_iterator i = staffMap.begin(); i != staffMap.end(); i++) {
		// for each staff rect
		TimeSegment t = i->first;
		FloatRect r = i->second;				// get the current staff rect
		bool start = true;						// we'll start splitting this rect
		
		if (ev != evtsMap.end()) {
			Time2GraphicMap::const_iterator next = ev;
			FloatRect evr = ev->second;			// the event rect
			FloatRect subrect = r;				// only to capture the vertical dimension
			TimeSegment tev = ev->first;
			while (t.include (ev->first)) {
				next++;
				if (start) {					// that's the beginning of the staff
					tev.first = t.first;		// time starts at staff start time
					subrect.left = evr.left;		// store the event left position as left pos
					start = false;				// and continue...
				}
				else {
					subrect.right = evr.left;	// here we have the first subrect
					tev.second = ev->first.first;	// time segment ends with this event
					map.push_back( make_pair(tev, subrect));	// insert into the map
					subrect.left = evr.left;	// and prepare for next rect
					tev.first = tev.second;		// prepare for next time segment
				}
				if (next != evtsMap.end()) {	// here there is no more events
					ev = next;
					evr = ev->second;
				}
				else break;
			}
			tev.second = t.second;				// time segment ends with staff element
			subrect.right = r.right;
			map.push_back( make_pair(tev, subrect));
		}
	}	
	outmap = map;
}

static bool mcompare( const pair<TimeSegment, FloatRect>& a, const pair<TimeSegment, FloatRect>& b)
{
	return a.first < b.first;
}

static bool scompare( const pair<TimeSegment, FloatRect>& a, const pair<TimeSegment, FloatRect>& b)
{
	if (a.second.top < b.second.top) return true;
	if (a.second.top == b.second.top) return a.first < b.first;
	return false;
}

//----------------------------------------------------------------------
// staff map is made using a cmbination of basic staff map and events map
// actually, it splits the basic staff map in vertical slices using the
// events x coordinate
void GuidoStaffCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	GuidoMapCollector staffCollector(fGRHandler, kGuidoStaff, fFilter);
	Time2GraphicMap staffMap, map, evmap;
	outmap->clear();
	
	fNoEmpty = false;
	GuidoGetMap( fGRHandler, page, w, h, kGuidoStaff, *this );	// collect the events map
	sort (fMap.begin(), fMap.end(), scompare);					// sort by lines, smaller date first
	mergelines (fMap, map);										// merge the graphic segments on a single line basis

	fNoEmpty = true;
	fMap.clear();
	GuidoGetMap( fGRHandler, page, w, h, kGuidoEvent, *this );	// collect the events map
	sort (fMap.begin(), fMap.end(), mcompare);					// sort first date, smaller duration first
	reduce (fMap, evmap);										// retains only one segment per starting date
	staffmerge (map, evmap, *outmap);							// and split the staff lines using the events segments	
}

//----------------------------------------------------------------------
void GuidoSystemCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	Time2GraphicMap map, evmap, merged;
	processNoDiv (page, w, h, &map);
	GuidoGetMap( fGRHandler, page, w, h, kGuidoEvent, *this );	// collect the events map
	sort (fMap.begin(), fMap.end(), mcompare);					// sort first date, smaller duration first
	reduce (fMap, evmap);										// retains only one segment per starting date
	staffmerge (map, evmap, *outmap);							// and split the staff lines using the events segments	
}

//----------------------------------------------------------------------
void GuidoSystemCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
	if ( dates.empty() )				return;				// empty time segments are filtered out
	if ( !box.IsValid() )				return;				// empty graphic segments are filtered out
	if ( infos.type == kEmpty)			return;
	fMap.push_back (make_pair(dates, box));
}

//----------------------------------------------------------------------
// computes the system map without subdivision
// this map is actually a combination of the basic system and slice maps
// adjusts the system map left border to the beginning of the first slice
//----------------------------------------------------------------------
void GuidoSystemCollector::processNoDiv (int page, float w, float h, Time2GraphicMap* outmap)
{
	GuidoMapCollector systemCollector(fGRHandler, kGuidoSystem);
	GuidoMapCollector slicesCollector(fGRHandler, kGuidoSystemSlice);

	Time2GraphicMap systemMap, slicesMap;
	systemCollector.process (page, w, h, &systemMap);	// collects the basic system map
	slicesCollector.process(page, w, h, &slicesMap);	// collects the basic slice map

	Time2GraphicMap::const_iterator slicesIter = slicesMap.begin();
	Time2GraphicMap::const_iterator systemIter = systemMap.begin();
	
	while (systemIter != systemMap.end()) {
		if (slicesIter == slicesMap.end()) {
			cerr << "unexpected slices segmentation end while collecting system map" << endl;
			break;
		}

		FloatRect sysrect	= systemIter->second;
		FloatRect slicerect = slicesIter->second;
		FloatRect adjusted (slicerect.left, sysrect.top, sysrect.right, sysrect.bottom);
		outmap->push_back( make_pair(systemIter->first, adjusted));

		// skip the remaining slices until the next line
		float prevx = slicerect.left;
		float sysy  = sysrect.top;
		while (slicesIter != slicesMap.end()) {
			float x = slicesIter->second.left;
			float y = slicesIter->second.top;
			if ((x < prevx) || (y < sysy)) break;			
			prevx = x;
			slicesIter++;
		}
		systemIter++;
	}
}

} // end namespoace
