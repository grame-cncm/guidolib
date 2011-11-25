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

using namespace std;

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



GUIDOAPI(GuidoErrCode)	GuidoGetMap( CGRHandler handle, int page, float w, float h, GuidoeElementSelector sel, MapCollector& f)
{
	if( handle == 0 ) 	return guidoErrInvalidHandle;
  	if( handle->grmusic == 0 ) return guidoErrInvalidHandle;
  	if( page <= 0 )		return guidoErrBadParameter;
	if ((sel < 0) || (sel >= kGuidoScoreElementEnd)) return guidoErrBadParameter; 
	handle->grmusic->GetMap( page, w, h, sel, f );
	return guidoNoErr;
}

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


