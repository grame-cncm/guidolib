#include "GUIDOScoreMapAdapter.h"
#include "GuidoMapCollector.h"

GuidoScoreMapAdapter::GuidoScoreMapAdapter()
{
}

GuidoScoreMapAdapter::~GuidoScoreMapAdapter()
{
}

GuidoErrCode GuidoScoreMapAdapter::getMap(CGRHandler gr, int pagenum, float width, float height, GuidoElementSelector sel, MapCollector& f)
{
	return ::GuidoGetMap(gr, pagenum, width, height, sel, f);
}


Time2GraphicMap GuidoScoreMapAdapter::getPageMap( CGRHandler gr, int pagenum, float w, float h)
{
	Time2GraphicMap outMap;
	::GuidoGetPageMap(gr, pagenum, w, h, outMap);
	return outMap;
}

Time2GraphicMap GuidoScoreMapAdapter::getStaffMap( CGRHandler gr, int pagenum, float w, float h, int staff)
{
	Time2GraphicMap outMap;
	::GuidoGetStaffMap(gr, pagenum, w, h, staff, outMap);
	return outMap;
}

Time2GraphicMap GuidoScoreMapAdapter::getVoiceMap(CGRHandler gr, int pagenum, float w, float h, int voice)
{
	Time2GraphicMap outMap;
	::GuidoGetVoiceMap(gr, pagenum, w, h, voice, outMap);
	return outMap;
}

Time2GraphicMap GuidoScoreMapAdapter::getSystemMap(CGRHandler gr, int pagenum, float w, float h)
{
	Time2GraphicMap outMap;
	::GuidoGetSystemMap(gr, pagenum, w, h, outMap);
	return outMap;
}

bool GuidoScoreMapAdapter::getTime(const GuidoDate& date, const Time2GraphicMap map, TimeSegment& t, FloatRect& r)
{
	return ::GuidoGetTime(date, map, t, r);
}

bool GuidoScoreMapAdapter::getPoint(float x, float y, const Time2GraphicMap map, TimeSegment& t, FloatRect& r)
{
	return ::GuidoGetPoint(x, y, map, t, r);
}

GuidoErrCode GuidoScoreMapAdapter::getTimeMap(CARHandler gr, TimeMapCollector& outMap)
{
	return ::GuidoGetTimeMap(gr, outMap);
}
