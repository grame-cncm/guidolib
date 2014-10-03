#ifndef MAP2JSON_H
#define MAP2JSON_H

#include "GUIDOScoreMap.h"
#include "json_array.h"
#include "json_object.h"
#include "json_element.h"
#include "json_value.h"
#include "json_printable.h"

using namespace json;
using namespace std;

class JSONFriendlyTimeMap : public TimeMapCollector
{
  public:
    vector<pair<TimeSegment, TimeSegment> > segments_;
    virtual ~JSONFriendlyTimeMap() {}
    virtual void Time2TimeMap( const TimeSegment& from, const TimeSegment& to ) {
      segments_.push_back (pair<TimeSegment, TimeSegment> (from, to));
    }
};

void stringToDate(string, GuidoDate &date);
string dateToString(GuidoDate &date);
float dateToFloat(const GuidoDate &date);

json_array * populate_json_array_with_time_map_collector(TimeMapCollector &info);
json_array * populate_json_array_with_time_2_graphic_map(Time2GraphicMap &info);

string print_and_free_printable(json_printable *printable);

char * Time2GraphicMap2JSON(Time2GraphicMap &info);

#ifdef __cplusplus
extern "C" {
#endif
char *	GuidoGetPageMap_JSON( CGRHandler gr, int pagenum, float w, float h);
char *	GuidoGetStaffMap_JSON( CGRHandler gr, int pagenum, float w, float h, int staff);
char *	GuidoGetVoiceMap_JSON( CGRHandler gr, int pagenum, float w, float h, int voice);
char *	GuidoGetSystemMap_JSON( CGRHandler gr, int pagenum, float w, float h);
#ifdef __cplusplus
}
#endif

#endif