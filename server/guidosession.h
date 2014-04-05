/*

 Copyright (C) 2012 Grame

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

#ifndef __guidosession__
#define __guidosession__

#include <string>
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"
#include "utilities.h"
#include "guido2img.h"

#include "json/json_value.h"

using namespace std;

namespace guidohttpd
{
enum GuidoSessionMapType {PAGE, STAFF, VOICE, SYSTEM, NO_TYPE};
enum GuidoSessionParsingError { GUIDO_SESSION_PARSING_SUCCESS, GUIDO_SESSION_PARSING_FAILURE };
enum GuidoWebApiFormat { GUIDO_WEB_API_PNG, GUIDO_WEB_API_JPEG, GUIDO_WEB_API_GIF, GUIDO_WEB_API_SVG,
                         GUIDO_WEB_API_UNDEFINED
                       };
//--------------------------------------------------------------------------
class guido2img;

class guidoAPIresponse {
  private :
    GuidoErrCode error_;
    int line_;
    int column_;
    string msg_;
  public :
    guidoAPIresponse (GuidoErrCode error, int line, int column, string msg);
    guidoAPIresponse (GuidoErrCode error);
    guidoAPIresponse ();
    bool is_happy();
    string errorMsg();
    static guidoAPIresponse make_happy_response();
};

struct guidosessionresponse {
    char* data_;
    unsigned int size_;
    string format_;
    string errstring_;
    GuidoSessionParsingError status_;
    int http_status_;
    guidosessionresponse ();
    guidosessionresponse (const char* data, unsigned int size, string format, int http_status = 200);
    guidosessionresponse (string, string format, int http_status = 200);
    ~guidosessionresponse ();
};


class GuidoServerTimeMap : public TimeMapCollector
{
  public:
    vector<pair<TimeSegment, TimeSegment> > segments_;
    virtual ~GuidoServerTimeMap() {}
    virtual void Time2TimeMap( const TimeSegment& from, const TimeSegment& to ) {
      segments_.push_back (pair<TimeSegment, TimeSegment> (from, to));
    }
};

class guidosession
{
    guido2img* fConverter;

private :
    string id_;
    ARHandler arh_;
    GRHandler grh_;
    guidoAPIresponse *whyIFailed_; // only for very problematic code
    // these are the current values
    bool resizeToPage_;
    GuidoWebApiFormat format_;
    string gmn_;
    int page_;
    float width_;
    float height_;
    float zoom_;
    float marginleft_;
    float margintop_;
    float marginbottom_;
    float marginright_;
    float systemsDistance_;
    int systemsDistribution_;
    float systemsDistribLimit_;
    float force_;
    float spring_;
    int neighborhoodSpacing_;
    int optimalPageFill_;
    
    // and these are the default values
    bool dresizeToPage_;
    GuidoWebApiFormat dformat_;
    string dgmn_;
    float dpage_;
    float dwidth_;
    int dheight_;
    float dzoom_;
    float dmarginleft_;
    float dmargintop_;
    float dmarginbottom_;
    float dmarginright_;
    float dsystemsDistance_;
    int dsystemsDistribution_;
    float dsystemsDistribLimit_;
    float dforce_;
    float dspring_;
    int dneighborhoodSpacing_;
    int doptimalPageFill_;

    // format conversion
    string formatToMIMEType ();
    string formatToLayType ();
    static GuidoWebApiFormat formatToWebApiFormat(string format);

    // used for graphical representation building
    void fillCurrentSettingsUsingGuidoPageFormat(GuidoPageFormat *pf);

    // private function to wrap json in the ID of a current session
    guidosessionresponse wrapObjectInId(json::json_object *obj);
    
public :

    // constructors, destructor, and initialzer
    guidosession(guido2img* g2img, string gmn, string id);
    virtual ~guidosession();
    void initializeUserSettableParameters();
    void initializeARHandGRH();
    void maybeResize();
    void updateValuesFromDefaults();
    void updateValuesFromDefaults(const TArgs& args);
    void changeDefaultValues(const TArgs &args);
    bool success();
    string errorMsg();

    // gets GR Handler
    const GRHandler getGRHandler() const;

    // used for graphical representation building
    void fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf);
    void fillGuidoLayoutSettingsUsingCurrentSettings(GuidoLayoutSettings *ls);

    // returns session responses with information for server to send
    static guidosessionresponse handleSimpleIntQuery(string, int);
    static guidosessionresponse handleSimpleBoolQuery(string, bool);
    static guidosessionresponse handleSimpleFloatQuery(string, float);
    static guidosessionresponse handleSimpleStringQuery(string, string);

    // a bit of a kludge for special cases...
    static guidosessionresponse welcomeMessage();
    static guidosessionresponse getGuidoAndServerVersions();
    
    guidosessionresponse handleSimpleIDdIntQuery(string, int);
    guidosessionresponse handleSimpleIDdBoolQuery(string, bool);
    guidosessionresponse handleSimpleIDdFloatQuery(string, float);
    guidosessionresponse handleSimpleIDdStringQuery(string, string);

    // more advanced handling
    guidosessionresponse datePageJson(string, int);
    guidosessionresponse mapJson (string thingToGet, Time2GraphicMap &outmap);
    guidosessionresponse timeMapJson (GuidoServerTimeMap &outmap);

    // queries
    guidoAPIresponse countVoices(int &n);
    guidoAPIresponse getPageCount(int &p);
    guidoAPIresponse duration(string &d);
    guidoAPIresponse findPageAt(GuidoDate date, int &p);
    guidoAPIresponse getPageDate(int page, GuidoDate &date);
    guidoAPIresponse getMap (GuidoSessionMapType map, int aux, Time2GraphicMap& outmap);
    guidoAPIresponse getTimeMap (GuidoServerTimeMap& outmap);
    static string getVersion();
    static string getServerVersion();
    static float getLineSpace();
    static guidoAPIresponse verifyGMN(string gmn);

    // -----------------------------
    int simpleSVGHelper(string svgfontfile, string *output);
    guidosessionresponse genericReturnImage();
    guidosessionresponse genericReturnMidi();
    guidosessionresponse genericReturnId();
    static guidosessionresponse genericFailure(string errorstring, int http_status = 400, string id = "");
    guidosessionresponse mapGet (const TArgs& args, unsigned int n, string thingToGet);
    guidosessionresponse pointGet (const TArgs& args, unsigned int n);

    // getters ---------------------
    float getWidth() { return width_; }
    float getHeight() { return height_; }
    int getPage() { return page_; }
    float getZoom() { return zoom_; }
    string getGMN() { return gmn_; }
    bool getResizeToPage() { return resizeToPage_; }
    GuidoWebApiFormat getFormat() { return format_; }

};

} // end namespoace

#endif