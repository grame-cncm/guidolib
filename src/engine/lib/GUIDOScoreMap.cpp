/*
  GUIDO Library
  Copyright (C) 2004  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/** \file GUIDOMap.cpp

	The time <-> graphic mapping Guido API

*/

#include <iostream>
#include <algorithm>

#include "GUIDOScoreMap.h"
#include "GUIDOInternal.h"
#include "ARMusic.h"
#include "GRMusic.h"
#include "SVGDevice.h"
#include "GuidoMapCollector.h"

using namespace std;
using namespace guido;

class SVGMapCollector : public MapCollector
{
	vector<MapElement>& fMap;

	public:
				 SVGMapCollector (vector<MapElement>& map) : fMap (map) {}
		virtual ~SVGMapCollector() {}
		
		virtual void Graph2TimeMap( const FloatRect& box, const TimeSegment& dates, const GuidoElementInfos& infos )
		{
			fMap.push_back(make_pair(box, RectInfos(dates, infos)));
		}
};

inline float fdate (const GuidoDate& d)		{ return d.num / (float)d.denom; }
//----------------------------------------------------------------------
void TimeSegment::print(ostream& out) const
{
	out << "[" << this->first << ", " << this->second << "]";
}

bool TimeSegment::empty() const
{
	float d1 = fdate(first);
	float d2 = fdate(second);
	float interval = (d1 > d2) ? d1 - d2 : d2 - d1;
	return (interval < 0.0001);
}

bool TimeSegment::intersect(const TimeSegment& ts) const
{
	float s1a = fdate(this->first);
	float s1b = fdate(this->second);
	float s2a = fdate(ts.first);
	float s2b = fdate(ts.second);
	return ((s1a >= s2a) && (s1a < s2b)) || ((s2a >= s1a) && (s2a < s1b));
}

bool TimeSegment::include(const GuidoDate& date) const
{
	float start = fdate(this->first);
	float end = fdate(this->second);
	float d = fdate(date);
	return (d >= start) && (d < end);
}

bool TimeSegment::include(const TimeSegment& ts) const
{
	float s1a = fdate(this->first);
	float s1b = fdate(this->second);
	float s2a = fdate(ts.first);
	float s2b = fdate(ts.second);
	return (s1a <= s2a) && (s1b >= s2b);
}

bool TimeSegment::operator < (const TimeSegment& ts) const
{
	if (fdate(this->first) < fdate(ts.first)) return true;
	if (fdate(this->first) > fdate(ts.first)) return false;
    if (fdate(this->second) < fdate(ts.second)) return true;    // C.D. Bug fix for windows
    /*if (fdate(this->second) >= fdate(ts.second))*/ return false;
	/*return ts.include (*this);*/
}

bool TimeSegment::operator == (const TimeSegment& ts) const
{
	return this->first.num == ts.first.num \
		&& this->first.denom == ts.first.denom \
		&& this->second.num == ts.second.num \
		&& this->second.denom == ts.second.denom;
}

TimeSegment TimeSegment::operator & (const TimeSegment& ts) const
{
	const GuidoDate& a = (fdate(first) > fdate(ts.first)) ? first : ts.first;
	const GuidoDate& b = (fdate(second) < fdate(ts.second)) ? second : ts.second;
	return TimeSegment (a, (fdate(a) < fdate(b)) ? b : a);
}

//----------------------------------------------------------------------
static GuidoErrCode checkParams( CGRHandler handle, int page)
{
	if( handle == 0 )			return guidoErrInvalidHandle;
  	if( handle->grmusic == 0 )	return guidoErrInvalidHandle;
  	if( page <= 0 )				return guidoErrBadParameter;
	if (page > GuidoGetPageCount(handle)) return guidoErrBadParameter;
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetMap( CGRHandler handle, int page, float w, float h, GuidoElementSelector sel, MapCollector& f)
{
	GuidoErrCode err = checkParams (handle, page);
	if (err != guidoNoErr) return err;
	if ((sel < 0) || (sel >= kGuidoScoreElementEnd)) return guidoErrBadParameter; 
	handle->grmusic->GetMap( page, w, h, sel, f );
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetPageMap( CGRHandler gr, int pagenum, float w, float h, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	GuidoMapCollector getmap (gr, kGuidoPage);
	getmap.process (pagenum, w, h, &outmap);
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetStaffMap( CGRHandler gr, int pagenum, float w, float h, int staff, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	if (staff < 1) return guidoErrBadParameter; 
	GuidoStaffCollector getmap (gr, staff);
	getmap.process (pagenum, w, h, &outmap);
	return guidoNoErr;
}

//----------------------------------------------------------------------
static bool sortMap(TMapSegments s1, TMapSegments s2)
{
	GuidoDate d1 = s1.first.first;
	GuidoDate d2 = s2.first.first;
	float f1 = d1.num / float(d1.denom);
	float f2 = d2.num / float(d2.denom);
	return f1 < f2;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetVoiceMap( CGRHandler gr, int pagenum, float w, float h, int voice, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	if (voice < 1) return guidoErrBadParameter; 
	GuidoVoiceCollector getmap (gr, voice);
	getmap.process (pagenum, w, h, &outmap);
	std::sort(outmap.begin(), outmap.end(), sortMap);
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetSystemMap( CGRHandler gr, int pagenum, float w, float h, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	GuidoSystemCollector getmap (gr);
	getmap.process (pagenum, w, h, &outmap);
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetRAWStaffMap( CGRHandler gr, int pagenum, float w, float h, int staff, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	GuidoStaffCollector getmap (gr, staff, true);
	getmap.process(pagenum, w, h, &outmap);
	std::sort(outmap.begin(), outmap.end(), sortMap);
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetRAWVoiceMap( CGRHandler gr, int pagenum, float w, float h, int voice, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	if (voice < 1) return guidoErrBadParameter; 
	GuidoVoiceCollector getmap (gr, voice, true);
	getmap.process (pagenum, w, h, &outmap);
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetRAWSystemMap( CGRHandler gr, int pagenum, float w, float h, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	GuidoMapCollector getmap (gr, kGuidoSystem);
	getmap.process(pagenum, w, h, &outmap);
	return guidoNoErr;
}

//----------------------------------------------------------------------
GUIDOAPI(bool)	GuidoGetTime( const GuidoDate& date, const Time2GraphicMap map, TimeSegment& t, FloatRect& r)
{
	for (Time2GraphicMap::const_iterator i = map.begin(); i != map.end(); i++) {
		if (i->first.include(date)) {
			t = i->first;
			r = i->second;
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------
GUIDOAPI(bool)	GuidoGetPoint( float x, float y, const Time2GraphicMap map, TimeSegment& t, FloatRect& r)
{
	for (Time2GraphicMap::const_iterator i = map.begin(); i != map.end(); i++) {
		if (i->second.Contains(x, y)) {
			t = i->first;
			r = i->second;
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------
GUIDOAPI(GuidoErrCode)	GuidoGetSVGMap( GRHandler handle, int page, GuidoElementSelector sel, vector<MapElement>& outMap)
{
	if( handle == 0 ) 	return guidoErrInvalidHandle;
  	if( handle->grmusic == 0 ) return guidoErrInvalidHandle;
  	if( page <= 0 )		return guidoErrBadParameter;
	if ((sel < 0) || (sel >= kGuidoScoreElementEnd)) return guidoErrBadParameter;

	GuidoPageFormat	pf;
	GuidoResizePageToMusic (handle);
	GuidoGetPageFormat (handle, page, &pf);
	
	SVGMapCollector collect(outMap);
	handle->grmusic->GetMap( page, pf.width/SVGDevice::kSVGSizeDivider, pf.height/SVGDevice::kSVGSizeDivider, sel, collect );
	return guidoNoErr;
}

GUIDOAPI(GuidoErrCode)	GuidoGetTimeMap( CARHandler inHandleAR, TimeMapCollector& f)
{
	if( inHandleAR == 0 )			return guidoErrInvalidHandle;
	if ( inHandleAR->armusic == 0 )	return guidoErrInvalidHandle;
	inHandleAR->armusic->getTimeMap(f);
	return guidoNoErr;
}


