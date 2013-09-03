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

#include "guidosession.h"
#include "GUIDOEngine.h"
#include "GUIDO2Midi.h"
#include "GUIDOScoreMap.h"
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

guidosessionresponse::guidosessionresponse (const char* data, unsigned int size, string format, int http_status)
{
    data_ = data;
    size_ = size;
    format_ = format;
    http_status_ = http_status;
}

guidosessionresponse::guidosessionresponse ()
{
    data_ = "";
    size_ = 0;
    format_ = "text/plain";
    http_status_ = 404;
}

guidosession::guidosession(guido2img* g2svg, string gmn, string id)
    : fConverter(g2svg), id_(id), dgmn_(gmn)
{
    initialize();
}

guidosession::~guidosession()
{
}

void guidosession::initialize()
{
    // note that we do nothing to the GMN when we initalize because
    // we set it in the constructor
    dresizeToPage_ = false;
    dformat_ = GUIDO_WEB_API_PNG;
    dpage_ = 1;
    dwidth_ = 400;
    dheight_ = 100;
    dzoom_ = 2.0f;
    dmarginleft_ = 10.0f;
    dmargintop_ = 10.0f;
    dmarginright_ = 10.0f;
    dmarginbottom_ = 10.0f;
    dsystemsDistance_ = 75.0f;
    dsystemsDistribution_ = kAutoDistrib;
    dsystemsDistribLimit_ = 0.25f;
    dforce_ = 750.0f;
    dspring_ = 1.1f;
    dneighborhoodSpacing_ = 0;
    doptimalPageFill_ = 1;
    updateValuesFromDefaults();
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
        dsystemsDistribution_ = atof(args.find("systemsDistribution")->second.c_str());
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
        dneighborhoodSpacing_ = atoi(args.find("neighborhoodSpacing")->second.c_str());
    }
    
    // optimalPageFill
    if (args.find("optimalPageFill") != args.end()) {
        doptimalPageFill_ = atoi(args.find("optimalPageFill")->second.c_str());
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
        systemsDistribution_ = atof(args.find("systemsDistribution")->second.c_str());
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
        neighborhoodSpacing_ = atoi(args.find("neighborhoodSpacing")->second.c_str());
    } else {
        neighborhoodSpacing_ = dneighborhoodSpacing_;
    }
    
    // optimalPageFill
    if (args.find("optimalPageFill") != args.end()) {
        optimalPageFill_ = atoi(args.find("optimalPageFill")->second.c_str());
    } else {
        optimalPageFill_ = doptimalPageFill_;
    }    
}

// FORMATTING
// ................................................
    
const char* guidosession::formatToMIMEType ()
{
    return ("image/" + string(formatToLayType())).c_str();
}
    
const char* guidosession::formatToLayType ()
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
    } else {
        return GUIDO_WEB_API_PNG;
    }
    return GUIDO_WEB_API_PNG;
}

GuidoErrCode guidosession::verifyGMN(string gmn) {
    ARHandler arh;
    return GuidoParseString (gmn.c_str(), &arh);
}

// GUIDO PAGE FORMAT
// ................................................
void guidosession::fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf)
{
    pf->height = height_;
    pf->width = width_;
    pf->margintop = margintop_;
    pf->marginleft = marginleft_;
    pf->marginright = marginright_;
    pf->marginbottom = marginbottom_;
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
    return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", 200);
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
    return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", 200);
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
    return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", 200);
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
    return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", 200);
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
    return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", 200);
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
    // ugh...memory management nightmare...need to fix
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

// GETTING GUIDO INFORMATION
// ................................................

string guidosession::getVersion() {
    return string (GuidoGetVersionStr());
}

string guidosession::getServerVersion() {
    // TODO - un-hardcode this
    return "0.50";
}

float guidosession::getLineSpace() {
    return GuidoGetLineSpace();
}

int guidosession::voicesCount()
{
    GuidoErrCode err;
    ARHandler arh;
    err = GuidoParseString (gmn_.c_str(), &arh);
    if (err != guidoNoErr) {
        return -1;
    }

    return GuidoCountVoices(arh);
}

string guidosession::duration()
{
    GuidoErrCode err;
    ARHandler arh;
    err = GuidoParseString (gmn_.c_str(), &arh);
    if (err != guidoNoErr) {
        return "";
    }
    GRHandler grh;
    
    GuidoPageFormat pf;
    fillGuidoPageFormatUsingCurrentSettings(&pf);
    
    GuidoSetDefaultPageFormat(&pf);
    err = GuidoAR2GR (arh, 0, &grh);
    if (err != guidoNoErr) {
        return "";
    }
    
    GuidoDate date;
    err = GuidoDuration(grh, &date);
    if (err != guidoNoErr) {
        return "";
    }

    return dateToString(date);
}
    
int guidosession::pagesCount()
{
    GuidoErrCode err;
    ARHandler arh;
    err = GuidoParseString (gmn_.c_str(), &arh);
    if (err != guidoNoErr) {
        return -1;
    }
    GRHandler grh;
    
    GuidoPageFormat pf;
    fillGuidoPageFormatUsingCurrentSettings(&pf);
    
    GuidoSetDefaultPageFormat(&pf);
    err = GuidoAR2GR (arh, 0, &grh);
    if (err != guidoNoErr) {
        return -1;
    }

    return GuidoGetPageCount(grh);
}
    
int guidosession::pageAt(GuidoDate date)
{
    GuidoErrCode err;
    ARHandler arh;
    err = GuidoParseString (gmn_.c_str(), &arh);
    if (err != guidoNoErr) {
        return -1;
    }
    GRHandler grh;
    
    GuidoPageFormat pf;
    fillGuidoPageFormatUsingCurrentSettings(&pf);
    
    GuidoSetDefaultPageFormat(&pf);
    err = GuidoAR2GR (arh, 0, &grh);
    if (err != guidoNoErr) {
        return -1;
    }
    
    return GuidoFindPageAt(grh, date);
}

int guidosession::pageDate(int page, GuidoDate *date)
{
    GuidoErrCode err;
    ARHandler arh;
    err = GuidoParseString (gmn_.c_str(), &arh);
    if (err != guidoNoErr) {
        return 1;
    }
    GRHandler grh;
    
    GuidoPageFormat pf;
    fillGuidoPageFormatUsingCurrentSettings(&pf);
    
    GuidoSetDefaultPageFormat(&pf);
    err = GuidoAR2GR (arh, 0, &grh);
    if (err != guidoNoErr) {
        return 1;
    }
    
    GuidoGetPageDate(grh, page, date);
    return 0;
}
    
GuidoErrCode guidosession::getTimeMap (TimeMapCollector& outmap)
{
    GuidoErrCode err;
    ARHandler arh;
    err = GuidoParseString (gmn_.c_str(), &arh);
    if (err != guidoNoErr) {
        return err;
    }
    
    err = GuidoGetTimeMap(arh, outmap);

    return err;
}
    
GuidoErrCode guidosession::getMap (GuidoSessionMapType map, int aux, Time2GraphicMap& outmap)
{
    GuidoErrCode err;
    ARHandler arh;
    err = GuidoParseString (gmn_.c_str(), &arh);
    if (err != guidoNoErr) {
        return err;
    }
    GRHandler grh;
    
    GuidoPageFormat pf;
    fillGuidoPageFormatUsingCurrentSettings(&pf);
    
    GuidoSetDefaultPageFormat(&pf);
    err = GuidoAR2GR (arh, 0, &grh);
    if (err != guidoNoErr) {
        return err;
    }
    
    /*
     In order to export a map, the score needs to be drawn on something.
     SVG export is a really quick way to do this drawing.
     It is a temporary solution
     */
    stringstream mystream;
    err = GuidoSVGExport(grh, page_, mystream, "");
    
    switch(map) {
        case PAGE :
            err = GuidoGetPageMap(grh, page_, width_, height_, outmap);
            break;
        case STAFF :
            err = GuidoGetStaffMap(grh, page_, width_, height_, aux, outmap);
            break;
        case VOICE :
            err = GuidoGetVoiceMap(grh, page_, width_, height_, aux, outmap);
            break;
        case SYSTEM :
            err = GuidoGetSystemMap(grh, page_, width_, height_, outmap);
            break;
        default :
            err = guidoErrActionFailed;
    }
    return err;
}

// ---- Abstractions

guidosessionresponse guidosession::genericReturnImage()
{
    int err = fConverter->convert(this);
    if (err == 0) {
        return guidosessionresponse(fConverter->data(), fConverter->size(), formatToMIMEType(), 201);
    }
    return genericFailure ("Could not convert the image.", 400);
}


guidosessionresponse guidosession::genericReturnMidi()
{
    //Guido2MidiParams midiparams;
    ARHandler arh;
    GuidoErrCode err = GuidoParseString (gmn_.c_str(), &arh);
    
    if (err != guidoNoErr) {
        return genericFailure ("Could not convert the Midi.", 400);
    }
    
    string filename = rand_alnum_str(20)+".midi";
    err = GuidoAR2MIDIFile(arh, filename.c_str(), 0);
    
    if (err != guidoNoErr) {
        return genericFailure ("Could not convert the Midi.", 400);
    }
    
    ostringstream sstream;
    ifstream fs(filename.c_str());
    sstream << fs.rdbuf();
    string data = sstream.str();
    fs.close();
    //remove(filename.c_str());
    if (err == 0) {
        return guidosessionresponse(data.c_str(), data.length(), "audio/midi", 201);
    }

    return genericFailure ("Could not convert the Midi.", 400);
}
    
guidosessionresponse guidosession::genericFailure(const char* errorstring, int http_status)
{
    json_object obj;
    obj.add (new json_element("Error", new json_string_value(errorstring)));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj.print(jstream);
    return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", http_status);
}

guidosessionresponse guidosession::genericReturnId()
{
    json_object obj;
    obj.add (new json_element("ID", new json_string_value(id_.c_str())));
    ostringstream mystream;
    json_stream jstream(mystream);
    obj.print(jstream);
    return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", 201);
}

} // end namespoace
