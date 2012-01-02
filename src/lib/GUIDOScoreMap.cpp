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

/** \file GUIDOMap.cpp

	The time <-> graphic mapping Guido API

*/

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
	return ts.include (*this);
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
GUIDOAPI(GuidoErrCode)	GuidoGetMap( CGRHandler handle, int page, float w, float h, GuidoeElementSelector sel, MapCollector& f)
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
GUIDOAPI(GuidoErrCode)	GuidoGetVoiceMap( CGRHandler gr, int pagenum, float w, float h, int voice, Time2GraphicMap& outmap)
{
	GuidoErrCode err = checkParams (gr, pagenum);
	if (err != guidoNoErr) return err;
	if (voice < 1) return guidoErrBadParameter; 
	GuidoVoiceCollector getmap (gr, voice);
	getmap.process (pagenum, w, h, &outmap);
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
GUIDOAPI(GuidoErrCode)	GuidoGetSVGMap( GRHandler handle, int page, GuidoeElementSelector sel, vector<MapElement>& outMap)
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


