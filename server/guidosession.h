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
#include <sstream>
#include "GUIDOEngine.h"
#include "GUIDOScoreMap.h"
#include "utilities.h"
#include "guido2img.h"

#include "json_value.h"
#include "map2json.h"
#include "JSONFriendlyTimeMap.h"
#include "JSONTime2GraphicMap.h"

namespace guidohttpd
{
enum GuidoSessionMapType {PAGE, STAFF, VOICE, SYSTEM, NO_TYPE};
enum GuidoSessionParsingError { GUIDO_SESSION_PARSING_SUCCESS, GUIDO_SESSION_PARSING_FAILURE };
enum GuidoWebApiFormat { GUIDO_WEB_API_PNG,
						 GUIDO_WEB_API_JPEG,
						 GUIDO_WEB_API_GIF,
						 GUIDO_WEB_API_SVG,
						 GUIDO_WEB_API_BINARY,
						 GUIDO_WEB_API_UNDEFINED
					   };
//--------------------------------------------------------------------------
class guido2img;

class guidoAPIresponse {
  private :
	GuidoErrCode fError;
	int fLine;
	int fColumn;
	std::string fMsg;

  public :
	guidoAPIresponse (GuidoErrCode error, int line, int column, std::string msg);
	guidoAPIresponse (GuidoErrCode error);
	guidoAPIresponse ();
	bool is_happy();
	std::string errorMsg();
	static guidoAPIresponse make_happy_response();
};

class guidosessionresponse {
	public:
	char* fData;
	unsigned int fSize;
	std::string fFormat;
	std::string fErrorString;
	GuidoSessionParsingError fStatus;
	int fHttpStatus;
	guidosessionresponse ();
	guidosessionresponse (const char* data, unsigned int size, std::string format, int http_status = 200);
	guidosessionresponse (std::string data, std::string format, int http_status = 200);
	guidosessionresponse (const guidosessionresponse &copy);
	~guidosessionresponse ();
};

/*!
 * Structure for all parameters for standard scores (ie not piano roll).
 */
typedef struct parameters {
		GuidoGrParameters guidoParameters;
		int page;
		GuidoWebApiFormat format;
} GuidoSessionScoreParameters;

class guidosession
{
	/*!
	 * \brief fConverter-> Can perform guido to image. One object per session to allow multithreading
	 * when we have multiple clients
	 */
    guido2img* fConverter;

private :
	std::string fSessionId;
	ARHandler fArh;
	GRHandler fGrh;
    guidoAPIresponse *whyIFailed_; // only for very problematic code

	std::string fGmnCode;

    // format conversion
	std::string formatToMIMEType(GuidoWebApiFormat format);
	static GuidoWebApiFormat formatToWebApiFormat(std::string format);

    // private function to wrap json in the ID of a current session
    guidosessionresponse wrapObjectInId(json::json_object *obj);

	/*!
	 * \brief getLayoutSettings create a layout settings from request parameters.
	 * \param args request parameters.
	 * \return a GuidoLayoutSettings object
	 */
	GuidoLayoutSettings getLayoutSettings(const TArgs &args);

	/*!
	 * \brief getPageFormat create a page format from request parameters.
	 * The request parameters have to be in CM unit.
	 * \param args request parameters.
	 * \return a GuidoPageFormat object
	 */
	GuidoPageFormat getPageFormat(const TArgs &args);

	/*!
	 * \brief initializeARHandGRH. Create a new AR and a new GR.
	 * The GR is created with default page format and default layout settings.
	 */
	void initializeARHandGRH();

	void applyPianoRollSettings();
public :
	/*!
	 * \brief sDefaultLayoutSettings. Default layout settings.
	 */
	static GuidoSessionScoreParameters sDefaultParameters;

    // constructors, destructor, and initialzer
	guidosession(std::string svgfontfile, std::string gmn, std::string id);
    virtual ~guidosession();

	PianoRoll * createPianoRoll(PianoRollType type);

	GuidoSessionScoreParameters getScoreParameters(const TArgs &args);

    bool success();
	std::string errorMsg();

	void updateGRH(GuidoSessionScoreParameters &parameters);

    // gets GR Handler
    const GRHandler getGRHandler() const;

    // used for graphical representation building
    void fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf);
    void fillGuidoLayoutSettingsUsingCurrentSettings(GuidoLayoutSettings *ls);

    // returns session responses with information for server to send
	static guidosessionresponse handleSimpleIntQuery(std::string, int);
	static guidosessionresponse handleSimpleBoolQuery(std::string, bool);
	static guidosessionresponse handleSimpleFloatQuery(std::string, float);
	static guidosessionresponse handleSimpleStringQuery(std::string, std::string);

	// a bit of a kludge for special cases...
	static guidosessionresponse welcomeMessage();
	static guidosessionresponse getGuidoAndServerVersions();

	guidosessionresponse handleSimpleIDdIntQuery(std::string, int);
	guidosessionresponse handleSimpleIDdBoolQuery(std::string, bool);
	guidosessionresponse handleSimpleIDdFloatQuery(std::string, float);
	guidosessionresponse handleSimpleIDdStringQuery(std::string, std::string);

    // more advanced handling
	guidosessionresponse datePageJson(std::string, int);
	guidosessionresponse mapJson (std::string thingToGet, JSONTime2GraphicMap &outmap);
    guidosessionresponse timeMapJson (JSONFriendlyTimeMap &outmap);

	// queries
	guidoAPIresponse countVoices(int &n);
	guidoAPIresponse getPageCount(int &p);
	guidoAPIresponse duration(std::string &d);
	guidoAPIresponse findPageAt(GuidoDate date, int &p);
	guidoAPIresponse getPageDate(int page, GuidoDate &date);
	guidoAPIresponse getMap (GuidoSessionMapType map, int aux, GuidoSessionScoreParameters & scoreParameters, Time2GraphicMap& outmap);
	guidoAPIresponse getTimeMap (JSONFriendlyTimeMap& outmap);
	static std::string getVersion();
	static std::string getServerVersion();
	static float getLineSpace();

    // -----------------------------
	int svgScoreExport(std::string svgfontfile, int page, std::stringstream *output);
	int binaryScoreExport(std::stringstream *output, int page);
	guidosessionresponse scoreReturnImage(GuidoSessionScoreParameters & scoreParameters);
	guidosessionresponse pianoRollReturnImage(GuidoSessionScoreParameters &pianoRollParameters);
	guidosessionresponse genericReturnMidi();

	/*!
	 * \brief genericReturnId. Return id of a new session created when a new valide gmn is send to the server with a post request.
	 * \return guidosessionresponse with the unique id of the session.
	 */
	guidosessionresponse genericReturnId();

	static guidosessionresponse genericFailure(std::string errorstring, int http_status = 400, std::string id = "");

	// getters ---------------------
	const std::string getGMN() { return fGmnCode; }
};

} // end namespoace

#endif
