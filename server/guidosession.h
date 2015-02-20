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

/*!
 * \brief The guidoAPIresponse class. The guido engine response and error.
 * This class is used to wrap guido engine's error.
 */
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

/*!
 * \brief The guidosessionresponse class.
 * Wrap a response.
 */
class guidosessionresponse {
	public:
	/*!
	 * \brief fData data of the response.
	 */
	char* fData;

	/*!
	 * \brief fSize Data size of the response.
	 */
	unsigned int fSize;

	/*!
	 * \brief fFormat MIMEType of the response
	 */
	std::string fFormat;

	/*!
	 * \brief fHttpStatus http status of the response.
	 */
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
typedef struct scoreParameters {
		GuidoGrParameters guidoParameters;
		int page;
		GuidoWebApiFormat format;
} GuidoSessionScoreParameters;

/*!
 * Structure or all parameters for pianoroll.
 */
typedef struct pianorollParameters {
		LimitParams limitParams;

		PianoRollType type;

		GuidoWebApiFormat format;

		int width;
		int height;
		bool enableKeyboard;
		bool enableAutoVoicesColoration;
		bool enableMeasureBars;
		int pitchLinesDisplayMode;
} GuidoSessionPianorollParameters;


class guidosession
{	
	protected :
		/*!
		 * \brief fConverter-> Can perform guido to image. One object per session to allow multithreading
		 * when we have multiple clients
		 */
		guido2img* fConverter;

	std::string fSessionId;
	ARHandler fArh;
	GRHandler fGrh;
    guidoAPIresponse *whyIFailed_; // only for very problematic code

	std::string fGmnCode;

	guidosession(const std::string & svgfontfile, std::string id);

	/*!
	 * \brief formatToMIMEType Convert a GuidoWebApiFormat to a mime type.
	 * \param format
	 * \return a mime type
	 */
	std::string formatToMIMEType(GuidoWebApiFormat format);

	/*!
	 * \brief formatToWebApiFormat. Give a GuidoWebApiFormat from a format of a request
	 * \param format the format receive in a request
	 * \return a GuidoWebApiFormat
	 */
	static GuidoWebApiFormat formatToWebApiFormat(std::string format);

	/*!
	 * \brief wrapObjectInId wrap a json object in the id of the session.
	 * \param obj
	 * \return
	 */
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
	 * \brief getLimitParams create a LimitParams for pianoroll from request parameters.
	 * \param args request parameters.
	 * \return a LimitParams object
	 */
	LimitParams getLimitParams(const TArgs &args);

	/*!
	 * \brief initializeARHandGRH. Create a new AR and a new GR.
	 * The GR is created with default page format and default layout settings.
	 */
	void initializeARHandGRH();

	/*!
	 * \brief createPianoRoll Create a PianoRoll from the current Abstract representation.
	 * \param params Parameters to be used for the pianor roll creation.
	 * \return a new PianoRoll object
	 */
	PianoRoll* createPianoRoll(GuidoSessionPianorollParameters &params);

public :
	/*!
	 * \brief sDefaultLayoutSettings. Default parameters to create score.
	 */
	static GuidoSessionScoreParameters sDefaultScoreParameters;

	/*!
	 * \brief sDefaultPianorollParameters. Defaults parameters to create PianoRoll.
	 */
	static GuidoSessionPianorollParameters sDefaultPianorollParameters;

	/*!
	 * \brief guidosession. Create a new guido session.
	 * The session have to be test with the method success.
	 * \param svgfontfile font in svg format.
	 * \param gmn guido code of the score.
	 * \param id the unique id of the new session.
	 */
	guidosession(const std::string &svgfontfile, std::string gmn, std::string id);

	/*!
	 * \brief ~guidosession
	 */
    virtual ~guidosession();

	/*!
	 * \brief getScoreParameters. Create score parameters structure from request parameters.
	 * \param args request parameters
	 * \return the GuidoSessionScoreParameters structure
	 */
	GuidoSessionScoreParameters getScoreParameters(const TArgs &args);

	/*!
	 * \brief getPianoRollParameters. Create pianoroll parameters struture from request parameters.
	 * \param args request parameters
	 * \return the GuidoSessionPianorollParameters structure.
	 */
	GuidoSessionPianorollParameters getPianoRollParameters(const TArgs &args);

	/*!
	 * \brief success. A method to know if the last operation has failed or not.
	 * \return  a boolean to know if an error occurs.
	 */
	bool success();

	/*!
	 * \brief errorMsg. Get error message.
	 * \return
	 */
	std::string errorMsg();

	/*!
	 * \brief updateGRH Update the graphic representation with passed parameters.
	 * \param parameters new parameters for the graphics representation.
	 */
	void updateGRH(GuidoSessionScoreParameters &parameters);

	/*!
	 * \brief getGRHandler
	 * \return
	 */
    const GRHandler getGRHandler() const;

	/*!
	 * \brief handleSimpleIntQuery handle a integer object in json object with property name name.
	 * \param name name of the json property
	 * \param myint the integer to handle
	 * \return the guidosessionresponse
	 */
	static guidosessionresponse handleSimpleIntQuery(std::string name, int myint);

	/*!
	 * \brief handleSimpleBoolQuery handle a boolean object in json object with property name name.
	 * \param name name of the json property
	 * \param mybool the boolean to handle
	 * \return the guidosessionresponse
	 */
	static guidosessionresponse handleSimpleBoolQuery(std::string name, bool mybool);

	/*!
	 * \brief handleSimpleFloatQuery handle a float object in json object with property name name.
	 * \param name name of the json property
	 * \param myfloat the float to handle
	 * \return the guidosessionresponse
	 */
	static guidosessionresponse handleSimpleFloatQuery(std::string name, float myfloat);

	/*!
	 * \brief handleSimpleStringQuery handle a string object in json object with property name name.
	 * \param name name of the json property
	 * \param mystring the string to handle
	 * \return the guidosessionresponse
	 */
	static guidosessionresponse handleSimpleStringQuery(std::string name, std::string mystring);

	/*!
	 * \brief handleSimpleIDdIntQuery Create a json response with a integer property with name name wrapped in the id of the session.
	 * \return a guidosessionresponse
	 */
	guidosessionresponse handleSimpleIDdIntQuery(std::string, int);

	/*!
	 * \brief handleSimpleIDdBoolQuery Create a json response with a boolean property with name name wrapped in the id of the session.
	 * \return a guidosessionresponse
	 */
	guidosessionresponse handleSimpleIDdBoolQuery(std::string, bool);

	/*!
	 * \brief handleSimpleIDdFloatQuery Create a json response with a float property with name name wrapped in the id of the session.
	 * \return a guidosessionresponse
	 */
	guidosessionresponse handleSimpleIDdFloatQuery(std::string, float);

	/*!
	 * \brief handleSimpleIDdStringQuery Create a json response with a string property with name name wrapped in the id of the session.
	 * \return a guidosessionresponse
	 */
	guidosessionresponse handleSimpleIDdStringQuery(std::string, std::string);

	/*!
	 * \brief datePageJson Create a json response with a page and a date wrapped in the id of the session.
	 * \return
	 */
	guidosessionresponse datePageJson(std::string, int);

	/*!
	 * \brief mapJson Create a json response with a Time2GraphicMap handle in the id of the session.
	 * \param thingToGet
	 * \param outmap the map to convert in json object.
	 * \return
	 */
	guidosessionresponse mapJson (std::string thingToGet, JSONTime2GraphicMap &outmap);

	/*!
	 * \brief timeMapJson Create a json response with a TimeMap wrapped in the id of the session.
	 * \param outmap the map to convert in json object.
	 * \return a guidosessionresponse
	 */
	guidosessionresponse timeMapJson (JSONFriendlyTimeMap &outmap);

	/*!
	 * \brief genericFailure Create a failure response.
	 * \param errorstring The text of the error
	 * \param http_status the Http status (default is 400)
	 * \param id A session id (default is empty).
	 * \return a guidosessionresponse
	 */
	static guidosessionresponse genericFailure(std::string errorstring, int http_status = 400, std::string id = "");

	//**************************** queries *******************************//
	/*!
	 * \brief welcomeMessage Create a guidosessionresponse with a welcome message
	 * \return a guidosessionresponse
	 */
	static guidosessionresponse welcomeMessage();

	/*!
	 * \brief getVersion Get the guido Engine version
	 * \return
	 */
	static std::string getVersion();

	/*!
	 * \brief getServerVersion get the webservice server version.
	 * \return
	 */
	static std::string getServerVersion();

	/*!
	 * \brief getLineSpace Get line space of guido engine.
	 * \return
	 */
	static float getLineSpace();

	/*!
	 * \brief getGuidoAndServerVersions Get the guido engine and webservice server version.
	 * \return a guidosessionresponse
	 */
	static guidosessionresponse getGuidoAndServerVersions();


	/*!
	 * \brief countVoices Count voice of a score.
	 * \param n out value  : the number of voices.
	 * \return
	 */
	guidoAPIresponse countVoices(int &n);

	/*!
	 * \brief getPageCount Count the number of page of a score.
	 * \param p out value
	 * \return
	 */
	guidoAPIresponse getPageCount(int &p);

	/*!
	 * \brief duration
	 * \param d out value
	 * \return
	 */
	guidoAPIresponse duration(std::string &d);

	/*!
	 * \brief findPageAt
	 * \param date out value
	 * \param p
	 * \return
	 */
	guidoAPIresponse findPageAt(GuidoDate date, int &p);

	/*!
	 * \brief getPageDate
	 * \param page
	 * \param date
	 * \return
	 */
	guidoAPIresponse getPageDate(int page, GuidoDate &date);

	/*!
	 * \brief getMap
	 * \param map
	 * \param aux
	 * \param scoreParameters
	 * \param outmap
	 * \return
	 */
	guidoAPIresponse getMap (GuidoSessionMapType map, int aux, GuidoSessionScoreParameters & scoreParameters, Time2GraphicMap& outmap);

	/*!
	 * \brief getTimeMap
	 * \param outmap
	 * \return
	 */
	guidoAPIresponse getTimeMap (JSONFriendlyTimeMap& outmap);


	/*!
	 * \brief svgScoreExport Export the score in svg image format.
	 * \param svgfontfile
	 * \param page
	 * \param output
	 * \return
	 */
	GuidoErrCode svgScoreExport(const std::string &svgfontfile, int page, std::stringstream &output);

	/*!
	 * \brief binaryScoreExport binary export of the score.
	 * The binary export is a set of command to draw the score with an external device.
	 * \param output the output of the export.
	 * \param page
	 * \return
	 */
	GuidoErrCode binaryScoreExport(int page, std::stringstream &output);

	/*!
	 * \brief scoreReturnImage Create a response with the image of the score using the parameters scoreParameters.
	 * \param scoreParameters the parameters used to create the image.
	 * \return a guidosessionresponse
	 */
	guidosessionresponse scoreReturnImage(GuidoSessionScoreParameters & scoreParameters);

	/*!
	 * \brief genericReturnMidi Convert the score in midi.
	 * Mime type of the response is "audio/midi"
	 * \return a guidosessionresponse
	 */
	guidosessionresponse genericReturnMidi();

	/*!
	 * \brief genericReturnId. Return id of a new session created when a new valide gmn is send to the server with a post request.
	 * \return guidosessionresponse with the unique id of the session.
	 */
	guidosessionresponse genericReturnId();

	// getters ---------------------
	std::string getGMN() { return fGmnCode; }

	std::string getId() {return fSessionId; }

	/*!
	 * \brief getPianorollKeyboardWidth Get the piano roll width
	 * \param params the draw parameters of the piano roll.
	 * \param width out value
	 * \return a guidoAPIresponse object
	 */
	guidoAPIresponse getPianorollKeyboardWidth(GuidoSessionPianorollParameters &params, float &width);

	/*!
	 * \brief getPianorollMap get a Time2GraphicMap for the piano roll
	 * \param params the draw parameters of the piano roll.
	 * \param outmap out value.
	 * \return a guidoAPIresponse object
	 */
	guidoAPIresponse getPianorollMap(GuidoSessionPianorollParameters &params, JSONTime2GraphicMap &outmap);

	/*!
	 * \brief pianoRollReturnImage Create a response with the image of the pianoroll using the parameters pianoRollParameters.
	 * \param pianoRollParameters the parameters used to create the image.
	 * \return a guidosessionresponse
	 */
	guidosessionresponse pianoRollReturnImage(GuidoSessionPianorollParameters &pianoRollParameters);
};

} // end namespoace

#endif
