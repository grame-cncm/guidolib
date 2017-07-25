/*

  GUIDO Library
  Copyright (C) 2011  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4661)		// don't know how to solve it
#endif


#include <iostream>
#include <utility>
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

	// We see if we have to process data or not (RAW)
	if (!fFilter || !(*fFilter).fGetRAWdata)
	{
		if ( dates.empty() )	return;					// empty time segments are filtered out
		if ( !box.IsValid() )	return;					// empty graphic segments are filtered out

		for (Time2GraphicMap::const_iterator i = fOutMap->begin(); i != fOutMap->end(); i++) {
			if ( i->first.intersect( dates ) )
				return;									// intersecting segments are filtered out
			if ( intersect( i->second, box ) )
				return;
		}
	}
	add (dates, box);
}

//----------------------------------------------------------------------
void GuidoVoiceCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
	// Guido mapping filtering: the Guido map doesn't fit the Interlude mapping constraints and should be reworked.
	if (fFilter && !(*fFilter)(infos))	return;		// element is filtered out 

	// We see if we have to process data or not (RAW)
	if (!fFilter || !(*fFilter).fGetRAWdata)
	{
		if ( dates.empty() )	return;					// empty time segments are filtered out
		if ( !box.IsValid() )	return;					// empty graphic segments are filtered out

		for (Time2GraphicMap::const_iterator i = fOutMap->begin(); i != fOutMap->end(); i++) {
			if ( intersect( i->second, box ) )
				return;
		}
	}
	add (dates, box);
}

//----------------------------------------------------------------------
// For each kBar retrieved, stores its position. If a kRest is met just after a kBar,
// on the same staff, its box left is aligned with the barline.
//----------------------------------------------------------------------
void GuidoVoiceAndBarCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
    if ( fStaffNum != 0 && (fStaffNum != infos.staffNum))   return; // other staves are filtered out if fStaffNum is defined
    if ( !box.IsValid() )	return;				// empty graphic segments are filtered out
    if ( infos.type == kEmpty) return;          // empty events are filtered out
    if ( infos.type == kGraceNote)      return; // grace notes are filtered out

    bool isBar = (infos.type == kBar || infos.type == kRepeatBegin || infos.type == kRepeatEnd);
	
    // If current element is a rest, and previous one on the same staff is a bar, we adjust its bounding box
    if (infos.type == kRest && fPrevBarX[infos.staffNum] != 0) {
        // We check if we're after a line break
        if (box.left < fPrevBarX[infos.staffNum]) {
            // TODO: After a line break, we have to set the correct left rect value
            fPrevBarX[infos.staffNum] = int(box.left); // For now, we don't change the olf rect left value
        }
        
        // We adjust new rect according to stored fPrevBarX and add new element
        FloatRect newRect ((float)fPrevBarX[infos.staffNum], box.top, box.right, box.bottom);
        add (dates, newRect);
    }
    // Otherwise, if element is not a bar, we directly add current dates/box to the list
    else if (!isBar)
        add (dates, box);
    
    // We set fPrevBarX if current element is a bar, otherwise we reset it
    fPrevBarX[infos.staffNum] = (isBar ? (int)box.right : 0);
}

//----------------------------------------------------------------------
void GuidoMapCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	fOutMap = outmap;
	if (!fOutMap) return;
	GuidoGetMap( fGRHandler, page, w, h, fSelector, *this );
}

//----------------------------------------------------------------------
// merge all the segments from one graphic line in a single segment
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
			linetime = t;							// and prepare the next line
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
	// We see if we have to process data or not (RAW)
	if (!fGetRAWdata)
	{
		if ( fNoEmpty && dates.empty() )	return;				// empty time segments are filtered out
		if ( !box.IsValid() )				return;				// empty graphic segments are filtered out
        if ( infos.type == kGraceNote)      return;             // grace notes are filtered out
		if ( infos.type == kEmpty)			return;             // empty events are filtered out
	}

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


//----------------------------------------------------------------------
// sort functions
//----------------------------------------------------------------------
static bool mCompare( const pair<TimeSegment, FloatRect>& a, const pair<TimeSegment, FloatRect>& b)
{
    return a.first < b.first;
}
    
static bool mTimeAndBoxLeftmostCompare( const pair<TimeSegment, FloatRect>& a, const pair<TimeSegment, FloatRect>& b)
{
    if (a.first < b.first) return true;
    return a.first.startEqual(b.first) && (a.second.left < b.second.left);
}
    
static bool mTimeAndBoxRightmostCompare( const pair<TimeSegment, FloatRect>& a, const pair<TimeSegment, FloatRect>& b)
{
    if (a.first < b.first) return true;
	if (a.first.startEqual(b.first)) {
//		if (a.first.duration() > b.first.duration()) return false;
		if (a.second.left > b.second.left) return true;
	}
	return false;
}

static bool scompare( const pair<TimeSegment, FloatRect>& a, const pair<TimeSegment, FloatRect>& b)
{
	if (a.second.top < b.second.top) return true;
	if (a.second.top == b.second.top) return a.first < b.first;
	return false;
}

//----------------------------------------------------------------------
// staff map is made using a combination of basic staff map and events map
// actually, it splits the basic staff map in vertical slices using the
// events x coordinate. New behaviour: if a rest is at a bar start, mapping
// is extended toward left barline. For old behaviour, use GuidoStaffCollector::processV1.
void GuidoStaffCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	// We see if we have to process data or not (RAW)
	if (fGetRAWdata)
	{
		if (!outmap) return;
		GuidoGetMap( fGRHandler, page, w, h, kGuidoStaff, *this );	// collect the staves map
		*outmap = fMap;
	}
	else
	{
		Time2GraphicMap map, evmap;
		outmap->clear();
	
		fNoEmpty = false;
		GuidoGetMap( fGRHandler, page, w, h, kGuidoStaff, *this );	// collect the staves map
		sort (fMap.begin(), fMap.end(), scompare);					// sort by lines, smaller date first
		mergelines (fMap, map);										// merge the graphic segments on a single line basis
    
        ///> A rest at a measure beginning has its segment box left value adjusted to the measure left.
        getEvents (page, w, h, true, evmap, fStaffNum);
        
        staffmerge (map, evmap, *outmap); // and split the staff lines using the events segments
	}
}
	
//----------------------------------------------------------------------
// staff map is made using a combination of basic staff map and events map
// actually, it splits the basic staff map in vertical slices using the
// events x coordinate. To use new behaviour, see GuidoStaffCollector::process.
void GuidoStaffCollector::processV1 (int page, float w, float h, Time2GraphicMap* outmap)
{
    // We see if we have to process data or not (RAW)
    if (fGetRAWdata)
    {
        if (!outmap) return;
        GuidoGetMap( fGRHandler, page, w, h, kGuidoStaff, *this );	// collect the staves map
        *outmap = fMap;
    }
    else
    {
        Time2GraphicMap map, evmap;
        outmap->clear();
            
        fNoEmpty = false;
        GuidoGetMap( fGRHandler, page, w, h, kGuidoStaff, *this );	// collect the staves map
        sort (fMap.begin(), fMap.end(), scompare);					// sort by lines, smaller date first
        mergelines (fMap, map);										// merge the graphic segments on a single line basis
            
        fNoEmpty = true;
        fMap.clear();
        GuidoGetMap( fGRHandler, page, w, h, kGuidoEvent, *this );	// collect the events map
        sort (fMap.begin(), fMap.end(), mCompare);					// sort first date, smaller duration first
        reduce (fMap, evmap);										// retains only one segment per starting date
        staffmerge (map, evmap, *outmap); // and split the staff lines using the events segments
    }
}

//----------------------------------------------------------------------
// Gets all events through GuidoVoiceAndBarCollector, which are timely + leftmost/rightmost sorted
// according to keepLeftmost value. Keeps only events on staffNum if defined.
//----------------------------------------------------------------------
void GuidoMapCollector::getEvents (int page, float w, float h, bool keepLeftmost, Time2GraphicMap& outmap, int staffNum) {
    Time2GraphicMap evMap;
    GuidoVoiceAndBarCollector voiceAndBarCollector(fGRHandler, staffNum);
    voiceAndBarCollector.process(page, w, h, &evMap);
 
    // mTimeAndBoxLeftCompare/mTimeAndBoxRightmostCompare sorts events by date
    // and by left/right position too (the left/right-most the higher in the list)
    sort(evMap.begin(), evMap.end(), keepLeftmost ? mTimeAndBoxLeftmostCompare : mTimeAndBoxRightmostCompare);
    // Like this, since reduce() keeps only the first element, the box conserved for each event is the one of the rest at the bar start, if any
   reduce (evMap, outmap);
}

//----------------------------------------------------------------------
// New behaviour: if all staves have a rest at a same bar start, mapping is extended toward left
// barline. For old behaviour, use GuidoSystemCollector::processV1.
void GuidoSystemCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	Time2GraphicMap map, evmap, merged;
	processNoDiv (page, w, h, &map);
    ///> A rest at a measure beginning has its segment box left value adjusted to the measure left.
    ///> All concurrent event at the same date have their mapping aligned to the measure left too, to keep consistency over staves.
    getEvents (page, w, h, false, evmap);
	
    staffmerge (map, evmap, *outmap); // we split the staff lines using the events segments
}
	
//----------------------------------------------------------------------
// To use the new behaviour, see GuidoSystemCollector::process.
void GuidoSystemCollector::processV1 (int page, float w, float h, Time2GraphicMap* outmap)
{
    Time2GraphicMap map, evmap, merged;
    processNoDiv (page, w, h, &map);
	
    GuidoGetMap( fGRHandler, page, w, h, kGuidoEvent, *this );	// collect the events map
    sort (fMap.begin(), fMap.end(), mCompare);					// sort first date, smaller duration first
	reduce (fMap, evmap);										// retains only one segment per starting date
    staffmerge (map, evmap, *outmap);                           // we split the staff lines using the events segments
}

//----------------------------------------------------------------------
void GuidoSystemCollector::Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
{
	if ( dates.empty() )				return;				// empty time segments are filtered out
	if ( !box.IsValid() )				return;				// empty graphic segments are filtered out
	if ( infos.type == kGraceNote)      return;             // grace notes are filtered out
	if ( infos.type == kEmpty)			return;             // empty events are filtered out
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
		float prevSliceBot = slicerect.bottom;
        float sysy         = sysrect.top;
        slicesIter++;
        while (slicesIter != slicesMap.end()) {
            float currSliceTop = slicesIter->second.top;
            if ((currSliceTop > prevSliceBot) || (currSliceTop < sysy)) break;
            prevSliceBot = currSliceTop;	
            slicesIter++;	    
		}
		systemIter++;
	}
}

} // end namespoace
