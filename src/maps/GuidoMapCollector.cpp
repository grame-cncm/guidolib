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
static _Interval xintersect (const FloatRect& r1, const FloatRect& r2)
{
	float a = ( r1.left > r2.left ) ? r1.left : r2.left;
	float b = ( r1.right < r2.right ) ? r1.right : r2.right;
	return _Interval(a, (a < b) ? b : a);
}

//----------------------------------------------------------------------
static GuidoDate addtomap (const TimeSegment& ts, const _Interval& xi, const _Interval& yi, Time2GraphicMap& outmap)
{
	outmap[ts] = FloatRect(xi.first, yi.first, xi.second, yi.second);
	return ts.second;
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
	(*fOutMap)[dates] = box;
}

//----------------------------------------------------------------------
void GuidoMapCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	fOutMap = outmap;
	if (!fOutMap) return;
	GuidoGetMap( fGRHandler, page, w, h, fSelector, *this );
}

ostream& operator << (ostream& out, const GuidoDate& d) {
	out << d.num << "/" << d.denom;
	return out;
}

ostream& operator << (ostream& out, const TimeSegment& s) {
	out << "[" << s.first << ", " << s.second << "]";
	return out;
}

//----------------------------------------------------------------------
// staff map is made using a cmbination of basic staff map and events map
// actually, it splits the basic staff map in vertical slices using the
// events x coordinate
void GuidoStaffCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	GuidoMapCollector staffCollector(fGRHandler, kGuidoStaff, fFilter);
	GuidoVoiceCollector eventsCollector(fGRHandler, fStaffFilter.fNum);

	Time2GraphicMap staffMap, evtsMap;
	staffCollector.process (page, w, h, &staffMap);		// collects the basic staff map
	eventsCollector.process(page, w, h, &evtsMap);		// collects the events map

	Time2GraphicMap::const_iterator eventsIter = evtsMap.begin();
	Time2GraphicMap::const_iterator staffIter  = staffMap.begin();
	// and next iterate thru both maps to omcpute the combination
	while (eventsIter != evtsMap.end()) {
		if (staffIter == staffMap.end()) {
			cerr << "unexpected staff segmentation end while collecting staff map" << endl;
			break;
		}

		Time2GraphicMap::const_iterator current = eventsIter;		// get the current event map
		Time2GraphicMap::const_iterator next = ++eventsIter;		// and the next event map
		TimeSegment evTime = current->first;						// get the time segment of the current event
		while (!staffIter->first.include(evTime) && (staffIter != staffMap.end())) {
			staffIter++;					// skip staff segments that are included in the current event
		}
		if (staffIter == staffMap.end()) {
			cerr << "unexpected staff segmentation end while while looking for evTime " << evTime << endl;
			break;
		}

		Time2GraphicMap::const_iterator currstaff = staffIter;		// get the current staff map
		Time2GraphicMap::const_iterator nextstaff = currstaff;
		nextstaff++;												// and the next staff map

		// at this point, we know that the current staff map has to be splitted
		// and we compute the split point
		float startx = current->second.left;		// left position is the current event left pos
		float endx = 0;
		if (next == evtsMap.end())					// last event ?
			endx = currstaff->second.right;			// right position is the staff end pos

		else {										// not the last event
			TimeSegment nextTime = next->first;		// get the next event time segment
			// is the next event on the same staff ?
			if (staffIter->first.include(nextTime) ||					// when the event is included in the staff time range
				(nextstaff == staffMap.end()) ||						// or when this is the last staff
				( currstaff->second.top == nextstaff->second.top)) {	// or when staff and next staff are on the same line
				endx = next->second.left;			// right position is the next event start pos
			}
			else 
				endx = currstaff->second.right;		// end zone is end staff
		}
		(*outmap)[current->first] = FloatRect(startx, currstaff->second.top, endx, currstaff->second.bottom);
	}
}

//----------------------------------------------------------------------
void GuidoSystemCollector::process (int page, float w, float h, Time2GraphicMap* outmap)
{
	Time2GraphicMap map, submap, merged;
	processNoDiv (page, w, h, &map);

	int i = 1;
	while (true) {
		Time2GraphicMap staffmap;
		GuidoStaffCollector staffcollector (fGRHandler, i++);
		staffcollector.process(page, w, h, &staffmap);
		if (!staffmap.size()) break;
		
		merge (map, staffmap, submap);
		map = submap;
	}
	*outmap = map;
}


//----------------------------------------------------------------------
// merge 2 maps using the smallest segmentation
// the principle of the merge is the following:
// the 2 maps are brwosed and 3 different cases are considered for each segments pair:
// let t1 and t2 be segments from map1 and map2
// 1) both segments t1 and t2 start at the same date:
//        t1 : |-------|
//        t2 : |---|......|
//    in this case we check the graphical position due to whole measure rests layout pb
//    when the position is (roughly) the same, inserts intersect t1 & t2 
//		   get next t2 and continue
//	  when the position is different, insert t1 and skip t2 until t2 > t1

// 2) the segment t1 starts before t2 and t2 starts in t1:
//        t1 : |-------|
//        t2 :   |---|......|
//    in this case, inserts intersect t1 & t2
//		   get next t2 and continue

// 3) the segment t2 starts after t1:
//        t1 : |-------|
//        t2 :           |---|
//    in this case, depending on the last date, inserts t1 
//			get next t1 and continue
//----------------------------------------------------------------------
void GuidoSystemCollector::merge (const Time2GraphicMap& map1, const Time2GraphicMap& map2, Time2GraphicMap& outmap)
{
	outmap.clear();
	Time2GraphicMap::const_iterator i1 = map1.begin();
	Time2GraphicMap::const_iterator i2 = map2.begin();

	_GuidoDate rightdate;
	while (i1 != map1.end()) {				// browse each map1 relation
		TimeSegment t1 = i1->first;
		FloatRect	s1 = i1->second;
		_Interval xinterval (s1.left, s1.right);
		_Interval yinterval (s1.top, s1.bottom);
		_GuidoDate t1start (t1.first);
		
		if (i2 != map2.end()) {	
			TimeSegment t2 = i2->first;
			FloatRect   s2 = i2->second;
			_GuidoDate t2start (t2.first);
			
			if (t1start == t2start) {					// case 1: both segments at the same date
				float proximity = s2.left - s1.left;
				if (proximity < 0) proximity = -proximity;
				if (proximity < 10) {						// check for graphic position
					// it (roughly) matches : inserts t1 and t2 intersection
					_Interval xinter = xintersect (s2, s1);
					rightdate = addtomap(t1 & t2, xinter, yinterval, outmap);
					i2++;									// and go to next t2
				}
				else {										// different positions: skip the rightmost segment
					if (s2.left > s1.left) {
						rightdate = addtomap(t1, xinterval, yinterval, outmap);
						while (++i2 != map2.end()) {
							_GuidoDate start (i2->first.first); 
							if (start >= t1.second) break;
						}
						i1++;
					}
					else {
						rightdate = addtomap(t2, _Interval(s2.left, s2.right), yinterval, outmap);
						i2++;
					}
				}
			}
			else if (t2start > t1start) {
				_GuidoDate t1end (t1.second);
				if (t2start < t1end) {				// case 2: t2 starts in t1
					TimeSegment inter = t1 & t2;
					_Interval xinter = xintersect (s2, s1);
					if (xinter.empty()) xinter = _Interval(s2.left, s2.right);
					else xinter.first = s2.left;

					_GuidoDate interstart (inter.first);
					if (interstart > rightdate) {			// check for missing t1 start segment
						TimeSegment beg = TimeSegment(t1.first, inter.first);
						outmap[beg] = FloatRect(s1.left, yinterval.first, xinter.first, yinterval.second);
					}
					// inserts t1 and t2 intersection
					rightdate = addtomap(inter, xinter, yinterval, outmap);
					i2++;									// and go to next t2
				}
				else {										// case 3: segment t2 starts after t1
					_GuidoDate t1start (t1.first);
					if (t1start == rightdate) {				// check for monotonicity
						rightdate = addtomap(t1, xinterval, yinterval, outmap);
					}
					i1++;									// and go to next t1
				}
			}			
			else {
				if (t2start < t1start) {									// we should never go there
					cerr << "unexpected guido system map case"  << endl;	// but in case we do :-(
					_GuidoDate t2end (t2.first);
					if (t2end > t1start) i2++;								// try to continue 
					else i2++;
				}
			}			
		}
		else {												// end of map2 (or map2 empty)
			if (t1start == rightdate)						// check for monotonicity
				rightdate = addtomap(t1, xinterval, yinterval, outmap);
			i1++;
		}
	}
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
		(*outmap)[systemIter->first] = adjusted;

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
