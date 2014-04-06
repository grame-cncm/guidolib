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

// json
#include "json/json_object.h"
#include "json/json_array.h"
#include "json/json_stream.h"

using namespace std;
using namespace json;

namespace guidohttpd
{

// CONSTRUCTORS, DESTRUCTORS AND INITIALIZERS
// ................................................

guidoAPIresponse::guidoAPIresponse()
  : error_(guidoNoErr), line_(-1), column_(-1), msg_("")
{
}

guidoAPIresponse::guidoAPIresponse(GuidoErrCode error)
  : error_(error), line_(-1), column_(-1), msg_("")
{
}

guidoAPIresponse::guidoAPIresponse (GuidoErrCode error, int line, int column, string msg)
  : error_(error), line_(line), column_(column), msg_(msg)
{
}

guidoAPIresponse guidoAPIresponse::make_happy_response() {
  return guidoAPIresponse(guidoNoErr);
}

bool guidoAPIresponse::is_happy() {
  return error_ == guidoNoErr;
}

string guidoAPIresponse::errorMsg() {
  stringstream out;
  out << GuidoGetErrorString(error_) << ".";
  if (line_ > 0) {
    out << " At line " << line_ << ", column " << column_ << ": " << msg_ << ".";
  }
  return out.str();
}

guidosessionresponse::guidosessionresponse (const char* data, unsigned int size, string format, int http_status)
{
    data_ = new char[size + 1];
    memcpy (data_, data, size);
    size_ = size;
    format_ = format;
    http_status_ = http_status;
}

guidosessionresponse::guidosessionresponse (string data, string format, int http_status)
{
    int size = data.size();
    const char *cc = data.c_str();
    data_ = new char[size + 1];
    memcpy (data_, cc, size + 1);
    size_ = size;
    format_ = format;
    http_status_ = http_status;
}

guidosessionresponse::guidosessionresponse ()
{
    data_ = new char[1];
    strcpy(data_, "");
    size_ = 0;
    format_ = "text/plain";
    http_status_ = 404;
}

guidosessionresponse::~guidosessionresponse ()
{
  delete data_;
}

guidosession::guidosession(guido2img* g2svg, string gmn, string id)
    : fConverter(g2svg), id_(id), dgmn_(gmn)
{
    arh_ = 0;
    grh_ = 0;
    whyIFailed_ = 0;
    initializeUserSettableParameters();
    initializeARHandGRH();
    maybeResize();
}

guidosession::~guidosession()
{
  // must be done in this order
  // grh_ depends on arh_
  if (grh_)
    GuidoFreeGR(grh_);
  if (arh_)
    GuidoFreeAR(arh_);
  if (whyIFailed_)
    delete whyIFailed_;
}

void guidosession::initializeUserSettableParameters()
{
    // note that we do nothing to the GMN when we initalize because
    // we set it in the constructor
    dresizeToPage_ = true;
    dformat_ = GUIDO_WEB_API_PNG;
    dpage_ = 1;
    dwidth_ = 15.0f;
    dheight_ = 5.0f;
    dzoom_ = 2.0f;
    dmarginleft_ = 1.0f;
    dmargintop_ = 1.0f;
    dmarginright_ = 0.5f;
    dmarginbottom_ = 0.5f;
    // we always want our settings to
    // be the default as guido evolves, so
    // we don't hardcode them here
    GuidoLayoutSettings gls;
    GuidoGetDefaultLayoutSettings (&gls);
    dsystemsDistance_ = gls.systemsDistance;
    dsystemsDistribution_ = gls.systemsDistribution;
    dsystemsDistribLimit_ = gls.systemsDistribLimit;
    dforce_ = gls.force;
    dspring_ = gls.spring;
    dneighborhoodSpacing_ = gls.neighborhoodSpacing;
    doptimalPageFill_ = gls.optimalPageFill;
    updateValuesFromDefaults();
}

void guidosession::initializeARHandGRH() {
    GuidoParser *parser = GuidoOpenParser();
    arh_ = GuidoString2AR (parser, gmn_.c_str());
    if (!arh_) {
      int line;
      int col;
      const char *msg;
      GuidoParserGetErrorCode (parser, line, col, &msg);
      GuidoCloseParser(parser);
      whyIFailed_ = new guidoAPIresponse(guidoErrParse, line, col, string(msg));
      return;
    }

    GuidoCloseParser(parser);
    GuidoErrCode err;
    
    GuidoPageFormat pf;
    fillGuidoPageFormatUsingCurrentSettings(&pf);

    GuidoSetDefaultPageFormat(&pf);
    err = GuidoAR2GR (arh_, 0, &grh_);
    if (err != guidoNoErr) {
        whyIFailed_ = new guidoAPIresponse(err);
        return;
    }
}

void guidosession::maybeResize() {
    GuidoErrCode err;
    if (resizeToPage_) {
      err = GuidoResizePageToMusic (grh_);
      if (err != guidoNoErr) {
        whyIFailed_ = new guidoAPIresponse(err);
        return;
      }
      GuidoPageFormat npf;
      GuidoGetPageFormat(grh_, 1, &npf);
      fillCurrentSettingsUsingGuidoPageFormat(&npf);
    }
}

const GRHandler guidosession::getGRHandler() const {
  return grh_;
}

bool guidosession::success() {
  return whyIFailed_ ? false : true;
}

string guidosession::errorMsg() {
  return whyIFailed_ ? whyIFailed_->errorMsg() : "";
}

void guidosession::changeDefaultValues(const TArgs &args)
{
    // resize
    if (args.find("resize") != args.end()) {
        dresizeToPage_ = atob(args.find("resize")->second);
    }
    
    // gmn
    if (args.find("gmn") != args.end()) {
        dgmn_ = args.find("gmn")->second;
    }
    
    // format
    if (args.find("format") != args.end()) {
        dformat_ = formatToWebApiFormat(args.find("format")->second);
    }
    
    // page
    if (args.find("page") != args.end()) {
        dpage_ = atoi(args.find("page")->second.c_str());
    }
    
    // width
    if (args.find("width") != args.end()) {
        dwidth_ = atoi(args.find("width")->second.c_str());
    }
    
    // height
    if (args.find("height") != args.end()) {
        dheight_ = atoi(args.find("height")->second.c_str());
    }
    
    // zoom
    if (args.find("zoom") != args.end()) {
        dzoom_ = atof(args.find("zoom")->second.c_str());
    }
    
    // marginleft
    if (args.find("marginleft") != args.end()) {
       dmarginleft_ = atof(args.find("marginleft")->second.c_str());
    }
    
    // margintop
    if (args.find("margintop") != args.end()) {
       dmargintop_ = atof(args.find("margintop")->second.c_str());
    }
    
    // marginright
    if (args.find("marginright") != args.end()) {
        dmarginright_ = atof(args.find("marginright")->second.c_str());
    }
    
    // marginbottom
    if (args.find("marginbottom") != args.end()) {
        dmarginbottom_ = atof(args.find("marginbottom")->second.c_str());
    }
    
    // systemsDistance
    if (args.find("systemsDistance") != args.end()) {
        dsystemsDistance_ = atof(args.find("systemsDistance")->second.c_str());
    }
    
    // systemsDistribution
    if (args.find("systemsDistribution") != args.end()) {
        dsystemsDistribution_ = systemsDistributionToFloat(args.find("systemsDistribution")->second);
    }
    
    // systemsDistribLimit
    if (args.find("systemsDistribLimit") != args.end()) {
        dsystemsDistribLimit_ = atof(args.find("systemsDistribLimit")->second.c_str());
    }
    
    // force
    if (args.find("force") != args.end()) {
        dforce_ = atof(args.find("force")->second.c_str());
    }
    
    // spring
    if (args.find("spring") != args.end()) {
        dspring_ = atof(args.find("spring")->second.c_str());
    }
    
    // neighborhoodSpacing
    if (args.find("neighborhoodSpacing") != args.end()) {
        dneighborhoodSpacing_ = atoib(args.find("neighborhoodSpacing")->second.c_str());
    }
    
    // optimalPageFill
    if (args.find("optimalPageFill") != args.end()) {
        doptimalPageFill_ = atoib(args.find("optimalPageFill")->second.c_str());
    }   
}
    
void guidosession::updateValuesFromDefaults()
{
    TArgs args;
    updateValuesFromDefaults(args);
}
    
void guidosession::updateValuesFromDefaults(const TArgs &args)
{
    // resize
    if (args.find("resize") != args.end()) {
        resizeToPage_ = atob(args.find("resize")->second);
    } else {
        resizeToPage_ = dresizeToPage_;
    }
    
    // gmn
    if (args.find("gmn") != args.end()) {
        gmn_ = args.find("gmn")->second;
    } else {
        gmn_ = dgmn_;
    }

    // format
    if (args.find("format") != args.end()) {
        format_ = formatToWebApiFormat(args.find("format")->second);
    } else {
        format_ = dformat_;
    }

    // page
    if (args.find("page") != args.end()) {
        page_ = atoi(args.find("page")->second.c_str());
    } else {
        page_ = dpage_;
    }

    // width
    if (args.find("width") != args.end()) {
        width_ = atoi(args.find("width")->second.c_str());
    } else {
        width_ = dwidth_;
    }

    // height
    if (args.find("height") != args.end()) {
        height_ = atoi(args.find("height")->second.c_str());
    } else {
        height_ = dheight_;
    }

    // zoom
    if (args.find("zoom") != args.end()) {
        zoom_ = atof(args.find("zoom")->second.c_str());
    } else {
        zoom_ = dzoom_;
    }

    // marginleft
    if (args.find("marginleft") != args.end()) {
        marginleft_ = atof(args.find("marginleft")->second.c_str());
    } else {
        marginleft_ = dmarginleft_;
    }

    // margintop
    if (args.find("margintop") != args.end()) {
        margintop_ = atof(args.find("margintop")->second.c_str());
    } else {
        margintop_ = dmargintop_;
    }
    
    // marginright
    if (args.find("marginright") != args.end()) {
        marginright_ = atof(args.find("marginright")->second.c_str());
    } else {
        marginright_ = dmarginright_;
    }

    // marginbottom
    if (args.find("marginbottom") != args.end()) {
        marginbottom_ = atof(args.find("marginbottom")->second.c_str());
    } else {
        marginbottom_ = dmarginbottom_;
    }
  
    // systemsDistance
    if (args.find("systemsDistance") != args.end()) {
        systemsDistance_ = atof(args.find("systemsDistance")->second.c_str());
    } else {
        systemsDistance_ = dsystemsDistance_;
    }
    
    // systemsDistribution
    if (args.find("systemsDistribution") != args.end()) {
        systemsDistribution_ = systemsDistributionToFloat(args.find("systemsDistribution")->second);
    } else {
        systemsDistribution_ = dsystemsDistribution_;
    }
    
    // systemsDistribLimit
    if (args.find("systemsDistribLimit") != args.end()) {
        systemsDistribLimit_ = atof(args.find("systemsDistribLimit")->second.c_str());
    } else {
        systemsDistribLimit_ = dsystemsDistribLimit_;
    }

    // force
    if (args.find("force") != args.end()) {
        force_ = atof(args.find("force")->second.c_str());
    } else {
        force_ = dforce_;
    }
    
    // spring
    if (args.find("spring") != args.end()) {
        spring_ = atof(args.find("spring")->second.c_str());
    } else {
        spring_ = dspring_;
    }
    
    // neighborhoodSpacing
    if (args.find("neighborhoodSpacing") != args.end()) {
        neighborhoodSpacing_ = atoib(args.find("neighborhoodSpacing")->second.c_str());
    } else {
        neighborhoodSpacing_ = dneighborhoodSpacing_;
    }
    
    // optimalPageFill
    if (args.find("optimalPageFill") != args.end()) {
        optimalPageFill_ = atoib(args.find("optimalPageFill")->second.c_str());
    } else {
        optimalPageFill_ = doptimalPageFill_;
    }    
}

// FORMATTING
// ................................................
    
string guidosession::formatToMIMEType ()
{
    return ("image/" + string(formatToLayType())).c_str();
}

string guidosession::formatToLayType ()
{
    switch(format_) {
    case GUIDO_WEB_API_PNG :
        return "png";
    case GUIDO_WEB_API_JPEG :
        return "jpeg";
    case GUIDO_WEB_API_GIF :
        return "gif";
    case GUIDO_WEB_API_SVG :
        return "xml+svg";
    case GUIDO_WEB_API_UNDEFINED :
        return "undefined";
    default :
        return "png";
    }
    return "png";
}
    
GuidoWebApiFormat guidosession::formatToWebApiFormat(string format)
{
    if (strcmp("png", format.c_str()) == 0) {
        return GUIDO_WEB_API_PNG;
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

// GUIDO PAGE FORMAT
// ................................................
void guidosession::fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf)
{
    pf->height = GuidoCM2Unit(height_);
    pf->width = GuidoCM2Unit(width_);
    pf->margintop = GuidoCM2Unit(margintop_);
    pf->marginleft = GuidoCM2Unit(marginleft_);
    pf->marginright = GuidoCM2Unit(marginright_);
    pf->marginbottom = GuidoCM2Unit(marginbottom_);
}

void guidosession::fillCurrentSettingsUsingGuidoPageFormat(GuidoPageFormat *pf)
{
    height_ = GuidoUnit2CM(pf->height);
    width_ = GuidoUnit2CM(pf->width);
    margintop_ = GuidoUnit2CM(pf->margintop);
    marginleft_ = GuidoUnit2CM(pf->marginleft);
    marginbottom_ = GuidoUnit2CM(pf->marginbottom);
    marginright_ = GuidoUnit2CM(pf->marginright);
}

void guidosession::fillGuidoLayoutSettingsUsingCurrentSettings(GuidoLayoutSettings *ls)
{
    ls->systemsDistance = systemsDistance_;
    ls->systemsDistribution = systemsDistribution_;
    ls->systemsDistribLimit = systemsDistribLimit_;
    ls->force = force_;
    ls->spring = spring_;
    ls->neighborhoodSpacing = neighborhoodSpacing_;
    ls->optimalPageFill = optimalPageFill_;
}
    
// GUIDO SESSION RESPONSE RETURNERS
// ................................................

guidosessionresponse guidosession::wrapObjectInId(json_object *obj)
{
    json_object *wrapper = new json_object;
    wrapper->add(new json_element(id_.c_str(), new json_object_value(obj)));
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

guidosessionresponse guidosession::mapJson (string thingToGet, Time2GraphicMap &outmap)
{
    json_object *obj = new json_object();
    json_array *arr = new json_array();
    for (int i = 0; i < (int)(outmap.size()); i++) {
        json_object*  holder = new json_object();
        json_object*  graph = new json_object();
        json_object*  mytime = new json_object();
        graph->add(new json_element("left", new json_float_value(outmap[i].second.left)));
        graph->add(new json_element("top", new json_float_value(outmap[i].second.top)));
        graph->add(new json_element("right", new json_float_value(outmap[i].second.right)));
        graph->add(new json_element("bottom", new json_float_value(outmap[i].second.bottom)));
        mytime->add(new json_element("start", new json_string_value(dateToString(outmap[i].first.first).c_str())));
        mytime->add(new json_element("end", new json_string_value(dateToString(outmap[i].first.second).c_str())));
        holder->add(new json_element("graph", new json_object_value(graph)));
        holder->add(new json_element("time", new json_object_value(mytime)));
        arr->add(new json_object_value(holder));
    }
    obj->add(new json_element(thingToGet.c_str(), new json_array_value(arr)));
    return wrapObjectInId(obj);
}

guidosessionresponse guidosession::timeMapJson (GuidoServerTimeMap &outmap)
{
    json_object *obj = new json_object();
    json_array *arr = new json_array();
    for (int i = 0; i < (int)(outmap.segments_.size()); i++) {
        json_object*  holder = new json_object();
        json_object*  score = new json_object();
        json_object*  perf = new json_object();
        score->add(new json_element("start", new json_string_value(dateToString(outmap.segments_[i].first.first).c_str())));
        score->add(new json_element("end", new json_string_value(dateToString(outmap.segments_[i].first.second).c_str())));
        perf->add(new json_element("start", new json_string_value(dateToString(outmap.segments_[i].second.first).c_str())));
        perf->add(new json_element("end", new json_string_value(dateToString(outmap.segments_[i].second.second).c_str())));
        holder->add(new json_element("score", new json_object_value(score)));
        holder->add(new json_element("perf", new json_object_value(perf)));
        arr->add(new json_object_value(holder));
    }
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

    vc = GuidoCountVoices(arh_);
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

    err = GuidoDuration(grh_, &date);
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

    pc = GuidoGetPageCount(grh_);
    if (pc < 0) {
      return guidoAPIresponse((GuidoErrCode) pc);
    }
    return guidoAPIresponse::make_happy_response();
}

guidoAPIresponse guidosession::findPageAt(GuidoDate date, int &findpageat)
{
    if (whyIFailed_)
      return *whyIFailed_;

    findpageat = GuidoFindPageAt(grh_, date);
    if (findpageat < 0) {
      return guidoAPIresponse((GuidoErrCode) findpageat);
    }
    return guidoAPIresponse::make_happy_response();
}

guidoAPIresponse guidosession::getPageDate(int page, GuidoDate &date)
{
    if (whyIFailed_)
      return *whyIFailed_;

    return guidoAPIresponse(GuidoGetPageDate(grh_, page, &date));
}

guidoAPIresponse guidosession::getTimeMap (GuidoServerTimeMap& outmap)
{
    if (whyIFailed_)
      return *whyIFailed_;

    return guidoAPIresponse(GuidoGetTimeMap(arh_, outmap));
}
    
guidoAPIresponse guidosession::getMap (GuidoSessionMapType map, int aux, Time2GraphicMap& outmap)
{
    if (whyIFailed_)
      return *whyIFailed_;

    GuidoErrCode err;

    switch(map) {
        case PAGE :
            err = GuidoGetPageMap(grh_, page_, GuidoCM2Unit(width_), GuidoCM2Unit(height_), outmap);
            break;
        case STAFF :
            err = GuidoGetStaffMap(grh_, page_, GuidoCM2Unit(width_), GuidoCM2Unit(height_), aux, outmap);
            break;
        case VOICE :
            err = GuidoGetVoiceMap(grh_, page_, GuidoCM2Unit(width_), GuidoCM2Unit(height_), aux, outmap);
            break;
        case SYSTEM :
            err = GuidoGetSystemMap(grh_, page_, GuidoCM2Unit(width_), GuidoCM2Unit(height_), outmap);
            break;
        default :
            err = guidoErrActionFailed;
    }
    return guidoAPIresponse(err);
}

// ---- Abstractions

int guidosession::simpleSVGHelper (string svgfontfile, string *output)
{
    GuidoErrCode err;
    stringstream mystream;
    const char *fontfile = svgfontfile != ""
                           ? svgfontfile.c_str ()
                           : 0;

    if (!fontfile) {
      cerr << "No svg font file found." << endl;
    }

    err = GuidoSVGExport(grh_, page_, mystream, fontfile);
    *output = mystream.str();
    return err;
}

guidosessionresponse guidosession::genericReturnImage()
{
    if (whyIFailed_) {
      return genericFailure(whyIFailed_->errorMsg(), 400, id_);
    }

    int err = fConverter->convert(this);
    if (err == guidoNoErr) {
        const char *fcd = fConverter->data();
        return guidosessionresponse(fcd, fConverter->size(), formatToMIMEType(), 201);
    }
    
    return genericFailure (format_ == GUIDO_WEB_API_UNDEFINED
                           ? "Return file format is undefined."
                           : "Server could not generate an image of type "+formatToLayType()+".", 400, id_);
}


guidosessionresponse guidosession::genericReturnMidi()
{
    //Guido2MidiParams midiparams;
    
    if (whyIFailed_) {
      return genericFailure(whyIFailed_->errorMsg(), 400, id_);
    }

    string filename = rand_alnum_str(20)+".midi";
    GuidoErrCode err;
    err = GuidoAR2MIDIFile(arh_, filename.c_str(), 0);
    
    if (err != guidoNoErr) {
        return genericFailure ("Could not convert the Midi.", 400, id_);
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

    return genericFailure ("Could not convert the Midi.", 400, id_);
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
    obj.add (new json_element("ID", new json_string_value(id_.c_str())));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj.print(jstream);
    return guidosessionresponse(mystream.str(), "application/json", 201);
}

} // end namespoace
