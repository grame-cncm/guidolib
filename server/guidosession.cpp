/*

 Copyright (C) 2011 Grame

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
#include <sstream>
#include <cstring>
#include <stdlib.h> 

#include "guidosession.h"
#include "GUIDOEngine.h"
#include "GUIDO2Midi.h"
#include "GUIDOScoreMap.h"
#include "GUIDOParse.h"
#include "utilities.h"
#include "engine.h"

// json
#include "json_object.h"
#include "json_array.h"
#include "json_stream.h"
#include "JSONTime2GraphicMap.h"

#include "date_tools.h"

using namespace std;
using namespace json;

namespace guidohttpd
{

// CONSTRUCTORS, DESTRUCTORS AND INITIALIZERS
// ................................................

guidoAPIresponse::guidoAPIresponse()
  : fError(guidoNoErr), fLine(-1), fColumn(-1), fMsg("")
{
}

guidoAPIresponse::guidoAPIresponse(GuidoErrCode error)
  : fError(error), fLine(-1), fColumn(-1), fMsg("")
{
}

guidoAPIresponse::guidoAPIresponse (GuidoErrCode error, int line, int column, string msg)
  : fError(error), fLine(line), fColumn(column), fMsg(msg)
{
}

guidoAPIresponse guidoAPIresponse::make_happy_response() {
  return guidoAPIresponse(guidoNoErr);
}

bool guidoAPIresponse::is_happy() {
  return fError == guidoNoErr;
}

string guidoAPIresponse::errorMsg() {
  stringstream out;
  out << GuidoGetErrorString(fError) << ".";
  if (fLine > 0) {
	out << " At line " << fLine << ", column " << fColumn << ": " << fMsg << ".";
  }
  return out.str();
}

guidosessionresponse::guidosessionresponse (const char* data, unsigned int size, string format, int http_status)
{
	fData = new char[size + 1];
	memcpy (fData, data, size);
	fSize = size;
	fFormat = format;
	fHttpStatus = http_status;
}

guidosessionresponse::guidosessionresponse (string data, string format, int http_status)
{
	int size = data.size();
	const char *cc = data.c_str();
	fData = new char[size + 1];
	memcpy (fData, cc, size + 1);
	fSize = size;
	fFormat = format;
	fHttpStatus = http_status;
}

guidosessionresponse::guidosessionresponse ()
{
	fData = new char[1];
	strcpy(fData, "");
	fSize = 0;
	fFormat = "text/plain";
	fHttpStatus = 404;
}

guidosessionresponse::guidosessionresponse (const guidosessionresponse &copy)
{
	fSize = copy.fSize;
	fData = new char[fSize + 1];
	memcpy (fData, copy.fData, fSize + 1);
	fFormat = copy.fFormat;
	fHttpStatus = copy.fHttpStatus;
	fErrorString = copy.fErrorString;
	fStatus = copy.fStatus;
}

guidosessionresponse::~guidosessionresponse ()
{
  delete[] fData;
}

GuidoSessionScoreParameters guidosession::sDefaultParameters;

guidosession::guidosession(string svgfontfile, string gmn, string id)
	: fSessionId(id), fGmnCode(gmn)
{
	fConverter = makeConverter(svgfontfile);
	fArh = 0;
	fGrh = 0;
	whyIFailed_ = 0;
    initializeARHandGRH();
}

guidosession::~guidosession()
{
	// must be done in this order
	// grh_ depends on arh_
	if (fGrh)
		GuidoFreeGR(fGrh);
	if (fArh)
		GuidoFreeAR(fArh);
	if (whyIFailed_)
		delete whyIFailed_;

	delete fConverter;
}

void guidosession::initializeARHandGRH() {
	// Create Abstract representation
    GuidoParser *parser = GuidoOpenParser();
	fArh = GuidoString2AR (parser, fGmnCode.c_str());
	if (!fArh) {
      int line;
      int col;
      const char *msg;
      GuidoParserGetErrorCode (parser, line, col, &msg);
      GuidoCloseParser(parser);
      whyIFailed_ = new guidoAPIresponse(guidoErrParse, line, col, string(msg));
      return;
    }
    GuidoCloseParser(parser);

	// Create Graphic representation
	fGrh = GuidoAR2GRParameterized(fArh, &sDefaultParameters.guidoParameters);
	if (!fGrh) {
		whyIFailed_ = new guidoAPIresponse(guidoErrActionFailed);
        return;
    }

	// Resize page to music.
	if (sDefaultParameters.guidoParameters.layoutSettings.resizePage2Music) {
		GuidoErrCode err = GuidoResizePageToMusic (fGrh);
		if (err != guidoNoErr) {
			whyIFailed_ = new guidoAPIresponse(err);
			return;
		}
	}
}

PianoRoll* guidosession::createPianoRoll(PianoRollType type) {
	PianoRoll* pr = 0;
	if(fArh) {
		pr = GuidoAR2PianoRoll(type, fArh);
		applyPianoRollSettings();
	}
	return pr;
}

void guidosession::updateGRH(GuidoSessionScoreParameters &parameters) {
	GuidoUpdateGRParameterized(fGrh, &parameters.guidoParameters);

	// Resize page to music.
	if (parameters.guidoParameters.layoutSettings.resizePage2Music) {
		GuidoErrCode err = GuidoResizePageToMusic (fGrh);
		if (err != guidoNoErr) {
			whyIFailed_ = new guidoAPIresponse(err);
			return;
		}
	}
}

const GRHandler guidosession::getGRHandler() const {
  return fGrh;
}

bool guidosession::success() {
  return whyIFailed_ ? false : true;
}

string guidosession::errorMsg() {
  return whyIFailed_ ? whyIFailed_->errorMsg() : "";
}

GuidoSessionScoreParameters guidosession::getScoreParameters(const TArgs &args)
{
	GuidoSessionScoreParameters params;
	// Get GuidoLayoutSettings from request parameters
	params.guidoParameters.layoutSettings = getLayoutSettings(args);
	// Get GuidoPageFormat from request parameters
	params.guidoParameters.pageFormat = getPageFormat(args);

	// Generated image format
	if (args.find("format") != args.end()) {
		params.format = formatToWebApiFormat(args.find("format")->second);
	} else {
		params.format = sDefaultParameters.format;
	}

	// page number
	if (args.find("page") != args.end()) {
		params.page = atoi(args.find("page")->second.c_str());
	} else {
		params.page = sDefaultParameters.page;
	}
	return params;
}

GuidoLayoutSettings guidosession::getLayoutSettings(const TArgs &args)
{
	GuidoLayoutSettings ls;

	// systemsDistance
	if (args.find("systemsDistance") != args.end()) {
		ls.systemsDistance = atof(args.find("systemsDistance")->second.c_str());
	} else {
		ls.systemsDistance = sDefaultParameters.guidoParameters.layoutSettings.systemsDistance;
	}

	// systemsDistribution
	if (args.find("systemsDistribution") != args.end()) {
		ls.systemsDistribution = systemsDistributionToFloat(args.find("systemsDistribution")->second);
	} else {
		ls.systemsDistribution = sDefaultParameters.guidoParameters.layoutSettings.systemsDistribution;
	}

	// systemsDistribLimit
	if (args.find("systemsDistribLimit") != args.end()) {
		ls.systemsDistribLimit = atof(args.find("systemsDistribLimit")->second.c_str());
	} else {
		ls.systemsDistribLimit = sDefaultParameters.guidoParameters.layoutSettings.systemsDistribLimit;
	}

	// force
	if (args.find("force") != args.end()) {
		ls.force = atof(args.find("force")->second.c_str());
	} else {
		ls.force = sDefaultParameters.guidoParameters.layoutSettings.force;
	}

	// spring
	if (args.find("spring") != args.end()) {
		ls.spring = atof(args.find("spring")->second.c_str());
	} else {
		ls.spring = sDefaultParameters.guidoParameters.layoutSettings.spring;
	}

	// neighborhoodSpacing
	if (args.find("neighborhoodSpacing") != args.end()) {
		ls.neighborhoodSpacing= atoib(args.find("neighborhoodSpacing")->second.c_str());
	} else {
		ls.neighborhoodSpacing = sDefaultParameters.guidoParameters.layoutSettings.neighborhoodSpacing;
	}

	// optimalPageFill
	if (args.find("optimalPageFill") != args.end()) {
		ls.optimalPageFill = atoib(args.find("optimalPageFill")->second.c_str());
	} else {
		ls.optimalPageFill = sDefaultParameters.guidoParameters.layoutSettings.optimalPageFill;
	}

	// resize
	if (args.find("resize") != args.end()) {
		ls.resizePage2Music = atob(args.find("resize")->second);
	} else {
		ls.resizePage2Music = sDefaultParameters.guidoParameters.layoutSettings.resizePage2Music;
	}

	// proportional rendering
	if (args.find("proportional") != args.end()) {
		ls.proportionalRenderingForceMultiplicator = atof(args.find("proportional")->second.c_str());
	} else {
		ls.proportionalRenderingForceMultiplicator = sDefaultParameters.guidoParameters.layoutSettings.proportionalRenderingForceMultiplicator;
	}
	return ls;
}

GuidoPageFormat guidosession::getPageFormat(const TArgs &args)
{
	GuidoPageFormat pf;
	// width
	if (args.find("width") != args.end()) {
		pf.width = GuidoCM2Unit(atoi(args.find("width")->second.c_str()));
	} else {
		pf.width = sDefaultParameters.guidoParameters.pageFormat.width;
	}

	// height
	if (args.find("height") != args.end()) {
		pf.height = GuidoCM2Unit(atoi(args.find("height")->second.c_str()));
	} else {
		pf.height = sDefaultParameters.guidoParameters.pageFormat.height;
	}
	// marginleft
	if (args.find("marginleft") != args.end()) {
		pf.marginleft = GuidoCM2Unit(atof(args.find("marginleft")->second.c_str()));
	} else {
		pf.marginleft = sDefaultParameters.guidoParameters.pageFormat.marginleft;
	}

	// margintop
	if (args.find("margintop") != args.end()) {
		pf.margintop = GuidoCM2Unit(atof(args.find("margintop")->second.c_str()));
	} else {
		pf.margintop = sDefaultParameters.guidoParameters.pageFormat.margintop;
	}

	// marginright
	if (args.find("marginright") != args.end()) {
		pf.marginright = GuidoCM2Unit(atof(args.find("marginright")->second.c_str()));
	} else {
		pf.marginright = sDefaultParameters.guidoParameters.pageFormat.marginright;
	}

	// marginbottom
	if (args.find("marginbottom") != args.end()) {
		pf.marginbottom = GuidoCM2Unit(atof(args.find("marginbottom")->second.c_str()));
	} else {
		pf.marginbottom = sDefaultParameters.guidoParameters.pageFormat.marginbottom;
	}

	return pf;
}

// FORMATTING
// ................................................
    
string guidosession::formatToMIMEType(GuidoWebApiFormat format)
{
	switch(format) {
	case GUIDO_WEB_API_PNG :
		return "image/png";
	case GUIDO_WEB_API_BINARY :
		return "application/octet-stream";
	case GUIDO_WEB_API_JPEG :
		return "image/jpeg";
	case GUIDO_WEB_API_GIF :
		return "image/gif";
	case GUIDO_WEB_API_SVG :
		return "image/xml+svg";
	case GUIDO_WEB_API_UNDEFINED :
		return "application/undefined";
	default :
		return "image/png";
	}
	return "image/png";
}

GuidoWebApiFormat guidosession::formatToWebApiFormat(string format)
{
    if (strcmp("png", format.c_str()) == 0) {
        return GUIDO_WEB_API_PNG;
    } else if (strcmp("binary", format.c_str()) == 0) {
        return GUIDO_WEB_API_BINARY;
    } else if (strcmp("jpeg", format.c_str()) == 0) {
        return GUIDO_WEB_API_JPEG;
    } else if (strcmp("jpg", format.c_str()) == 0) {
        return GUIDO_WEB_API_JPEG;
    } else if (strcmp("gif", format.c_str()) == 0) {
        return GUIDO_WEB_API_GIF;
    } else if (strcmp("svg", format.c_str()) == 0) {
        return GUIDO_WEB_API_SVG;
    } else {
        return GUIDO_WEB_API_UNDEFINED;
    }
    return GUIDO_WEB_API_PNG;
}

void guidosession::applyPianoRollSettings()
{
	/* TODO GGX
	GuidoPianoRollSetLimits(PianoRoll *pr, LimitParams limitParams);
	GuidoPianoRollEnableKeyboard(PianoRoll *pr, bool enabled);
	GuidoPianoRollGetKeyboardWidth(PianoRoll *pr, int height, float &keyboardWidth);
	GuidoPianoRollEnableAutoVoicesColoration(PianoRoll *pr, bool enabled);
	*/
}

// GUIDO SESSION RESPONSE RETURNERS
// ................................................

guidosessionresponse guidosession::wrapObjectInId(json_object *obj)
{
    json_object *wrapper = new json_object;
	wrapper->add(new json_element(fSessionId.c_str(), new json_object_value(obj)));
    ostringstream mystream;
    json_stream jstream(mystream);
    wrapper->print(jstream);
    // important! as everything is pointers, need to delete here
    delete wrapper;
    return guidosessionresponse(mystream.str(), "application/json", 200);
}

guidosessionresponse guidosession::handleSimpleIntQuery(string name, int myint)
{
    json_object *obj = new json_object();
    obj->add (new json_element(name.c_str(), new json_int_value(myint)));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj->print(jstream);
    // important! as everything is pointers, need to delete here
    delete obj;
    return guidosessionresponse(mystream.str(), "application/json", 200);
}

guidosessionresponse guidosession::handleSimpleBoolQuery(string name, bool mybool)
{
    json_object *obj = new json_object();
    if (mybool)
        obj->add (new json_element(name.c_str(), new json_true_value()));
    else
        obj->add (new json_element(name.c_str(), new json_false_value()));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj->print(jstream);
    // important! as everything is pointers, need to delete here
    delete obj;
    return guidosessionresponse(mystream.str(), "application/json", 200);
}

guidosessionresponse guidosession::handleSimpleFloatQuery(string name, float myfloat)
{
    json_object *obj = new json_object();
    obj->add (new json_element(name.c_str(), new json_float_value(myfloat)));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj->print(jstream);
    // important! as everything is pointers, need to delete here
    delete obj;
    return guidosessionresponse(mystream.str(), "application/json", 200);
}

guidosessionresponse guidosession::handleSimpleStringQuery(string name, string mystring)
{
    json_object *obj = new json_object();
    obj->add (new json_element(name.c_str(), new json_string_value(mystring.c_str())));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj->print(jstream);
    // important! as everything is pointers, need to delete here
    delete obj;
    return guidosessionresponse(mystream.str(), "application/json", 200);
}

guidosessionresponse guidosession::getGuidoAndServerVersions()
{
    json_object *obj = new json_object();
    obj->add (new json_element("guidolib", new json_string_value(getVersion().c_str())));
    obj->add (new json_element("guidoserver", new json_string_value(getServerVersion().c_str())));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj->print(jstream);
    // important! as everything is pointers, need to delete here
    delete obj;
    return guidosessionresponse(mystream.str(), "application/json", 200);
}

guidosessionresponse guidosession::welcomeMessage()
{
    json_object *obj = new json_object();
    obj->add (new json_element("description", new json_string_value(
    "The HTTP GUIDO Engine is a web server that provides music score layout\n"
    "services, based on the the GUIDO Engine library and on the GUIDO Music\n"
    "Notation format. These services are availble using HTTP requests. The\n"
    "corresponding API is inspired by a RESTful design and is close to the\n"
    "C/C++ API.")));
    obj->add (new json_element("doc", new json_string_value("http://guido.grame.fr")));
    obj->add (new json_element("guidolib", new json_string_value(getVersion().c_str())));
    obj->add (new json_element("guidoserver", new json_string_value(getServerVersion().c_str())));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj->print(jstream);
    // important! as everything is pointers, need to delete here
    delete obj;
    return guidosessionresponse(mystream.str(), "application/json", 200);
}

guidosessionresponse guidosession::handleSimpleIDdIntQuery(string name, int myint)
{
    json_object *obj = new json_object();
    obj->add (new json_element(name.c_str(), new json_int_value(myint)));
    return wrapObjectInId(obj);
}

guidosessionresponse guidosession::handleSimpleIDdBoolQuery(string name, bool mybool)
{
    json_object *obj = new json_object();
    if (mybool)
        obj->add (new json_element(name.c_str(), new json_true_value()));
    else
        obj->add (new json_element(name.c_str(), new json_false_value()));
    return wrapObjectInId(obj);
}

guidosessionresponse guidosession::handleSimpleIDdFloatQuery(string name, float myfloat)
{
    json_object *obj = new json_object();
    obj->add (new json_element(name.c_str(), new json_float_value(myfloat)));
    return wrapObjectInId(obj);
}
    
guidosessionresponse guidosession::handleSimpleIDdStringQuery(string name, string mystring)
{
    json_object *obj = new json_object();
    obj->add (new json_element(name.c_str(), new json_string_value(mystring.c_str())));
    return wrapObjectInId(obj);
}

guidosessionresponse guidosession::datePageJson(string date, int page)
{
    json_object *obj = new json_object();
    obj->add(new json_element("page", new json_int_value(page)));
    obj->add(new json_element("date", new json_string_value(date.c_str())));
    return wrapObjectInId(obj);
}

guidosessionresponse guidosession::mapJson (string thingToGet, JSONTime2GraphicMap &outmap)
{
    json_object *obj = new json_object();
	json_array *arr = outmap.toJsonArray();
    obj->add(new json_element(thingToGet.c_str(), new json_array_value(arr)));
    return wrapObjectInId(obj);
}

guidosessionresponse guidosession::timeMapJson (JSONFriendlyTimeMap &outmap)
{
    json_object *obj = new json_object();
	json_array *arr = outmap.toJsonArray();
    obj->add(new json_element("timemap", new json_array_value(arr)));
    return wrapObjectInId(obj);
}

// GETTING GUIDO INFORMATION
// ................................................

string guidosession::getVersion() {
    return string (GuidoGetVersionStr());
}

string guidosession::getServerVersion() {
    // TODO - un-hardcode this
    return "0.61";
}

float guidosession::getLineSpace() {
    return GuidoGetLineSpace();
}

guidoAPIresponse guidosession::countVoices(int &vc)
{
    if (whyIFailed_)
      return *whyIFailed_;

	vc = GuidoCountVoices(fArh);
    if (vc < 0) {
      return guidoAPIresponse((GuidoErrCode) vc);
    }

    return guidoAPIresponse::make_happy_response();
}

guidoAPIresponse guidosession::duration(string &dur)
{
    if (whyIFailed_)
      return *whyIFailed_;

    GuidoErrCode err;
    GuidoDate date;

	err = GuidoDuration(fGrh, &date);
    if (err != guidoNoErr) {
        return guidoAPIresponse(err);
    }
    
    dur = dateToString(date);
    return guidoAPIresponse::make_happy_response();
}
    
guidoAPIresponse guidosession::getPageCount(int &pc)
{
    if (whyIFailed_)
      return *whyIFailed_;

	pc = GuidoGetPageCount(fGrh);
    if (pc < 0) {
      return guidoAPIresponse((GuidoErrCode) pc);
    }
    return guidoAPIresponse::make_happy_response();
}

guidoAPIresponse guidosession::findPageAt(GuidoDate date, int &findpageat)
{
    if (whyIFailed_)
      return *whyIFailed_;

	findpageat = GuidoFindPageAt(fGrh, date);
    if (findpageat < 0) {
      return guidoAPIresponse((GuidoErrCode) findpageat);
    }
    return guidoAPIresponse::make_happy_response();
}

guidoAPIresponse guidosession::getPageDate(int page, GuidoDate &date)
{
    if (whyIFailed_)
      return *whyIFailed_;

	return guidoAPIresponse(GuidoGetPageDate(fGrh, page, &date));
}

guidoAPIresponse guidosession::getTimeMap (JSONFriendlyTimeMap& outmap)
{
    if (whyIFailed_)
      return *whyIFailed_;

	return guidoAPIresponse(GuidoGetTimeMap(fArh, outmap));
}
    
guidoAPIresponse guidosession::getMap (GuidoSessionMapType map, int aux, GuidoSessionScoreParameters &scoreParameters, Time2GraphicMap& outmap)
{
    if (whyIFailed_)
      return *whyIFailed_;

    GuidoErrCode err;

    switch(map) {
        case PAGE :
			err = GuidoGetPageMap(
						fGrh,
						scoreParameters.page,
						scoreParameters.guidoParameters.pageFormat.width,
						scoreParameters.guidoParameters.pageFormat.height,
						outmap);
			break;
        case STAFF :
			err = GuidoGetStaffMap(
						fGrh,
						scoreParameters.page,
						scoreParameters.guidoParameters.pageFormat.width,
						scoreParameters.guidoParameters.pageFormat.height,
						aux,
						outmap);
            break;
        case VOICE :
			err = GuidoGetVoiceMap(
						fGrh,
						scoreParameters.page,
						scoreParameters.guidoParameters.pageFormat.width,
						scoreParameters.guidoParameters.pageFormat.height,
						aux,
						outmap);
			break;
        case SYSTEM :
			err = GuidoGetSystemMap(
						fGrh,
						scoreParameters.page,
						scoreParameters.guidoParameters.pageFormat.width,
						scoreParameters.guidoParameters.pageFormat.height,
						outmap);
            break;
        default :
            err = guidoErrActionFailed;
    }
    return guidoAPIresponse(err);
}



// ---- Abstractions

int guidosession::svgScoreExport (string svgfontfile, int page, stringstream *output)
{
	GuidoErrCode err;
    const char *fontfile = svgfontfile != ""
                           ? svgfontfile.c_str ()
                           : 0;

    if (!fontfile) {
      cerr << "No svg font file found." << endl;
    }

	err = GuidoGR2SVG(fGrh, page, *output, true, 0);
   return err;
}

int guidosession::binaryScoreExport (stringstream *output, int page)
{
    GuidoErrCode err;
	err = GuidoBinaryExport(fGrh, page, *output);
    return err;
}

guidosessionresponse guidosession::scoreReturnImage(GuidoSessionScoreParameters &scoreParameters)
{
    if (whyIFailed_) {
	  return genericFailure(whyIFailed_->errorMsg(), 400, fSessionId);
    }

	int err = fConverter->convertScore(this, scoreParameters);
    if (err == guidoNoErr) {
        const char *fcd = fConverter->data();
		return guidosessionresponse(fcd, fConverter->size(), formatToMIMEType(scoreParameters.format), 201);
    }
    
	return genericFailure (scoreParameters.format == GUIDO_WEB_API_UNDEFINED
                           ? "Return file format is undefined."
						   : "Server could not generate an image of type "+formatToMIMEType(scoreParameters.format)+".", 400, fSessionId);
}

guidosessionresponse guidosession::pianoRollReturnImage(GuidoSessionScoreParameters &pianoRollParameters)
{
	if (whyIFailed_) {
	  return genericFailure(whyIFailed_->errorMsg(), 400, fSessionId);
	}

	int err = fConverter->convertPianoRoll(this);
	if (err == guidoNoErr) {
		const char *fcd = fConverter->data();
		return guidosessionresponse(fcd, fConverter->size(), formatToMIMEType(pianoRollParameters.format), 201);
	}

	return genericFailure (pianoRollParameters.format == GUIDO_WEB_API_UNDEFINED
						   ? "Return file format is undefined."
						   : "Server could not generate an image of type "+formatToMIMEType(pianoRollParameters.format)+".", 400, fSessionId);
}

guidosessionresponse guidosession::genericReturnMidi()
{
    //Guido2MidiParams midiparams;
    
    if (whyIFailed_) {
	  return genericFailure(whyIFailed_->errorMsg(), 400, fSessionId);
    }

    string filename = rand_alnum_str(20)+".midi";
    GuidoErrCode err;
	err = GuidoAR2MIDIFile(fArh, filename.c_str(), 0);
    
    if (err != guidoNoErr) {
		return genericFailure ("Could not convert the Midi.", 400, fSessionId);
    }
    
    ostringstream sstream;
    ifstream fs(filename.c_str());
    sstream << fs.rdbuf();
    string data = sstream.str();
    fs.close();
    //remove(filename.c_str());
    if (err == guidoNoErr) {
        return guidosessionresponse(data, "audio/midi", 201);
    }

	return genericFailure ("Could not convert the Midi.", 400, fSessionId);
}
    
guidosessionresponse guidosession::genericFailure(string errorstring, int http_status, string id)
{
    ostringstream mystream;
    json_object *obj = new json_object;
    obj->add (new json_element("Error", new json_string_value(errorstring.c_str ())));
    if (id != "") {
      json_object *wrapper = new json_object;
      wrapper->add(new json_element(id.c_str(), new json_object_value(obj)));
      json_stream jstream(mystream);
      wrapper->print(jstream);
      delete wrapper;
    }
    else {
      json_stream jstream(mystream);
      obj->print(jstream);
      delete obj;
    }
    return guidosessionresponse(mystream.str(), "application/json", http_status);
}

guidosessionresponse guidosession::genericReturnId()
{
    json_object obj;
	obj.add (new json_element("ID", new json_string_value(fSessionId.c_str())));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj.print(jstream);
    return guidosessionresponse(mystream.str(), "application/json", 201);
}

} // end namespoace
