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
#include "GUIDOScoreMap.h"
#include "json.h"
#include "jsonhelper.h"

using namespace std;

namespace guidohttpd
{

    static int printchannel(void *userdata, const char *data, uint32_t)
    {
        stringstream *foo = (stringstream *)userdata;
        *foo << data;
        return 1;
    }
    
    guidosessionresponse::guidosessionresponse (const char* data, unsigned int size, string format, string errstring, unsigned int argumentsToAdvance, GuidoSessionParsingError status)
    {
        data_ = data;
        size_ = size;
        format_ = format;
        errstring_ = errstring;
        argumentsToAdvance_ = argumentsToAdvance;
        status_ = status;
    }

    guidosessionresponse::guidosessionresponse ()
    {
        data_ = "";
        size_ = 0;
        format_ = "text/plain";
        errstring_ = "";
        argumentsToAdvance_ = 0;
        status_ = GUIDO_SESSION_PARSING_FAILURE;
    }
    
    guidosession::guidosession(guido2img* g2svg)  
	: fConverter(g2svg)
    {
        initialize();
    }
    
    guidosession::~guidosession() {

    }
    
    void guidosession::initialize() {
        resizeToPage_ = false;
        gmn_  = "[c]";	
        url_ = "";
        format_ = GUIDO_WEB_API_PNG;
        page_ = 1;
        width_ = 400;
        height_ = 100;
        zoom_ = 2.0f;
        marginleft_ = 10.0f;
        margintop_ = 10.0f;
        marginright_ = 10.0f;
        marginbottom_ = 10.0f;
        systemsDistance_ = 75.0f;
        systemsDistribution_ = kAutoDistrib;
        systemsDistribLimit_ = 0.25f;
        force_ = 750.0f;
        spring_ = 1.1f;
        neighborhoodSpacing_ = 0;
        optimalPageFill_ = 1;
    }
    const char* guidosession::formatToMIMEType ()
    {
        return ("image/" + string(formatToLayType())).c_str();
    }
    const char* guidosession::formatToLayType ()
    {
        switch(format_)
        {
            case GUIDO_WEB_API_PNG : return "png";
            case GUIDO_WEB_API_JPEG : return "jpeg";
            case GUIDO_WEB_API_GIF : return "gif";
            case GUIDO_WEB_API_SVG : return "xml+svg";
            default : return "png";
        }
        return "png";
    }
    const char* guidosession::getStringRepresentationOf (string toget)
    {
        stringstream out;
        if (toget.compare("resizepagetomusic") == 0)
            out << (resizeToPage_ ? "true" : "false");
        else if (toget.compare("gmn") == 0)
            out << gmn_;
        else if (toget.compare("format") == 0)
            out << formatToLayType();
        else if (toget.compare("page") == 0)
            out << page_;
        else if (toget.compare("width") == 0)
            out << width_;
        else if (toget.compare("height") == 0)
            out << height_;
        else if (toget.compare("zoom") == 0)
            out << zoom_;
        else if (toget.compare("marginleft") == 0)
            out << marginleft_;
        else if (toget.compare("margintop") == 0)
            out << margintop_;
        else if (toget.compare("marginright") == 0)
            out << marginright_;
        else if (toget.compare("marginbottom") == 0)
            out << marginbottom_;
        else
            out << "";
        return out.str().c_str();
    }
    
    void guidosession::fillGuidoPageFormatUsingCurrentSettings(GuidoPageFormat *pf)
    {
        pf->height = height_;
        pf->width = width_;
        pf->margintop = margintop_;
        pf->marginleft = marginleft_;
        pf->marginright = marginright_;
        pf->marginbottom = marginbottom_;
    }
    
    GuidoErrCode guidosession::getmap (GuidoSessionMapType map, int aux, Time2GraphicMap& outmap)
    {
        GuidoErrCode err;
        ARHandler arh;
        err = GuidoParseString (gmn_.c_str(), &arh);
        if (err != guidoNoErr)
            return err;
        GRHandler grh;

        GuidoPageFormat pf;
        fillGuidoPageFormatUsingCurrentSettings(&pf);

        GuidoSetDefaultPageFormat(&pf);
        err = GuidoAR2GR (arh, 0, &grh);
        if (err != guidoNoErr)
            return err;
        
        /*
         In order to export a map, the score needs to be drawn on something.
         SVG export is a really quick way to do this drawing.
         It is a temporary solution
         */
        stringstream mystream;
        err = GuidoSVGExport(grh, page_, mystream, ""); 
        
        switch(map){
            case PAGE : err = GuidoGetPageMap(grh, page_, width_, height_, outmap); break;
            case STAFF : err = GuidoGetStaffMap(grh, page_, width_, height_, aux, outmap); break;
            case VOICE : err = GuidoGetVoiceMap(grh, page_, width_, height_, aux, outmap); break;
            case SYSTEM : err = GuidoGetSystemMap(grh, page_, width_, height_, outmap); break;
            default : err = guidoErrActionFailed;
        }
        //cout << "GMN " << gmn << " SIZE " << outmap.size () << endl;
        return err;
    }
    
    /* callbacks */
    guidosessionresponse guidosession::handleGet(const TArgs& args, unsigned int n)
    {
        const int SIMPLE_SIZE = 10;
        string simple_ids[SIMPLE_SIZE] = {"width", "height", "margintop", "marginbottom", "marginleft", "marginright", "zoom","format","gmn", "resizepagetomusic"};
        
        for (int i = 0; i < SIMPLE_SIZE; i++)
            if (simple_ids[i].compare(args[n].second) == 0)
                return simpleGet (simple_ids[i]);
        
        const int MAP_SIZE = 4;
        string map_ids[MAP_SIZE] = {"pagemap","staffmap","voicemap","systemmap"};
        for (int i = 0; i < MAP_SIZE; i++)
            if (map_ids[i].compare(args[n].second) == 0)
                return mapGet (args, n, map_ids[i]);
        
        if (strcmp("point", args[n].second.c_str()) == 0)
            return pointGet (args, n);
        return genericFailure("Unidentified get.");
    }
    
    guidosessionresponse guidosession::handlePage(const TArgs& args, unsigned int n)
    {
        page_ = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleWidth(const TArgs& args, unsigned int n)
    {
        width_ = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleHeight(const TArgs& args, unsigned int n)
    {
        height_ = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    guidosessionresponse guidosession::handleMarginLeft(const TArgs& args, unsigned int n)
    {
        marginleft_ = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleMarginRight(const TArgs& args, unsigned int n)
    {
        marginright_ = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleMarginTop(const TArgs& args, unsigned int n)
    {
        margintop_ = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleMarginBottom(const TArgs& args, unsigned int n)
    {
        marginbottom_ = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleZoom(const TArgs& args, unsigned int n)
    {
        zoom_  = atof(args[n].second.c_str());
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleResizePageToMusic(const TArgs& args, unsigned int n)
    {
        if (strcmp("true", args[n].second.c_str()) == 0)
            resizeToPage_ = true;
        else if (strcmp("True", args[n].second.c_str()) == 0)
            resizeToPage_ = true;
        else if (strcmp("false", args[n].second.c_str()) == 0)
            resizeToPage_ = false;
        else if (strcmp("False", args[n].second.c_str()) == 0)
            resizeToPage_ = false;
        else
            return genericFailure("Must specify true or false for resizeToPage");
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleGMN(const TArgs& args, unsigned int n)
    {
        gmn_ = string(args[n].second);
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleBlankRequest(const TArgs& args, unsigned int n)
    {
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleFormat(const TArgs& args, unsigned int n)
    {
        if (strcmp("png", args[n].second.c_str()) == 0)
            format_ = GUIDO_WEB_API_PNG;
        else if (strcmp("jpeg", args[n].second.c_str()) == 0)
            format_ = GUIDO_WEB_API_JPEG;
        else if (strcmp("jpg", args[n].second.c_str()) == 0)
            format_ = GUIDO_WEB_API_JPEG;
        else if (strcmp("gif", args[n].second.c_str()) == 0)
            format_ = GUIDO_WEB_API_GIF;
        else if (strcmp("svg", args[n].second.c_str()) == 0)
            format_ = GUIDO_WEB_API_PNG;//KLUDGE
        else
            return genericFailure("Not a valid format - please choose from png, jpeg, gif or svg.");
        return genericReturnImage();
    }
    
    guidosessionresponse guidosession::handleFaultyInput(const TArgs&, unsigned int)
    {
        return genericFailure ("You have entered insane input.");
    }
    
    // ---- Abstractions
    
    guidosessionresponse guidosession::genericReturnImage()
    {
        Guido2ImageErrorCodes err = fConverter->convert(this);
        if (err == GUIDO_2_IMAGE_SUCCESS)
        {
            return guidosessionresponse(fConverter->data(), fConverter->size(), formatToMIMEType(), "", 1, GUIDO_SESSION_PARSING_SUCCESS);
        }
        return genericFailure (Guido2Image::getErrorString (err));
    }
    
    guidosessionresponse guidosession::genericFailure(const char* errorstring)
    {
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "error", 1);
        json_print_pretty(&printer, JSON_STRING, errorstring, 1);
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        return guidosessionresponse("", 0, "application/json", mystream.str(), 1, GUIDO_SESSION_PARSING_FAILURE);
    }
    
    guidosessionresponse guidosession::simpleGet(string thingToGet)
    {
        string internalRep = getStringRepresentationOf(thingToGet);
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        if (url_.size () != 0)
        {
            json_print_pretty(&printer, JSON_KEY, "score", 1);
            const char* current_url = url_.c_str();
            json_print_pretty(&printer, JSON_STRING, current_url, 1);
        }
        json_type type = swapTypeForName(thingToGet);
        if (type == JSON_NONE)
            return genericFailure ("Cannot find correct JSON type output.");
        const char* constCharVersionOfThingToGet = thingToGet.c_str();
        const char* constCharVersionOfInternalRep = internalRep.c_str();
        json_print_pretty(&printer, JSON_KEY, constCharVersionOfThingToGet, 1);
        json_print_pretty(&printer, type, constCharVersionOfInternalRep, type == JSON_STRING ? 1 : strlen(constCharVersionOfInternalRep));
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_free(&printer);
        return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", "", 1, GUIDO_SESSION_PARSING_SUCCESS);
    }
    
    guidosessionresponse guidosession::mapGet (const TArgs& args, unsigned int n, string thingToGet)
    {
        Time2GraphicMap outmap;
        GuidoErrCode err;
        if (thingToGet.compare("pagemap") == 0)
            err = getmap (PAGE, 0, outmap);
        else if (thingToGet.compare("systemmap") == 0)
            err = getmap (SYSTEM, 0, outmap);
        else
        {
            if (n >= args.size () - 1)
                return genericFailure ("You need an extra argument for this map call.");
            if (thingToGet.compare("systemmap") == 0 && strcmp(args[n + 1].second.c_str(), "voice") != 0)
                return genericFailure ("To get a voicemap, the argument must be followed by a voice argument indicating which voice you want.");
            if (thingToGet.compare("systemmap") == 0 && strcmp(args[n + 1].second.c_str(), "staff") != 0)
                return genericFailure ("To get a staffmap, the argument must be followed by a staff argument indicating which staff you want.");
            int aux = atoi(args[n+1].second.c_str());
            if (thingToGet.compare("staffmap") == 0)
                err = getmap (STAFF, aux, outmap);
            else if (thingToGet.compare("voicemap") == 0)
                err = getmap (VOICE, aux, outmap);
        }
        if (err != guidoNoErr)
            return genericFailure ("Could not generate a map for your request.");
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        if (url_.size () != 0)
        {
            json_print_pretty(&printer, JSON_KEY, "score", 1);
            const char* current_url = url_.c_str();
            json_print_pretty(&printer, JSON_STRING, current_url, 1);
        }
        const char* constCharVersionOfThingToGet = thingToGet.c_str();
        json_print_pretty(&printer, JSON_KEY, constCharVersionOfThingToGet, 1);
        json_print_pretty(&printer, JSON_ARRAY_BEGIN, NULL, 0);
        stringstream buffer;
        for (int i = 0; i < (int)(outmap.size()); i++)
        {
            json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
            GUIDOjson_print_date (&printer, "begintime", outmap[i].first.first.num, outmap[i].first.first.denom);
            GUIDOjson_print_date (&printer, "endtime", outmap[i].first.second.num, outmap[i].first.second.denom);
            GUIDOjson_print_float_rect (&printer , "floatrec", outmap[i].second.left, outmap[i].second.right, outmap[i].second.top, outmap[i].second.bottom);
            json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        }
        json_print_pretty(&printer, JSON_ARRAY_END, NULL, 0);
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_free(&printer);
        int argumentsToAdvance = (thingToGet.compare("pagemap") == 0 || thingToGet.compare("systemmap") == 0) ? 1 : 2;
        return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", "", argumentsToAdvance, GUIDO_SESSION_PARSING_SUCCESS);
    }
    guidosessionresponse guidosession::pointGet (const TArgs& args, unsigned int n)
    {
        if (n >= args.size () - 3)
            return genericFailure ("You need both x and y arguments as well as a mapping specification for get=point.");
        float x;
        float y;
        const char* maptype;
        int aux;
        GuidoSessionMapType type = NO_TYPE;
        int i = 1;
        int movingTarget = 4;
        while (i < movingTarget) {
            if (args[n + i].first == "x")
                x = atof(args[n + i].second.c_str());
            else if (args[n + i].first == "y")
                y = atof(args[n + i].second.c_str());
            else if (args[n + i].first == "map")
            {
                maptype = args[n + i].second.c_str();
                if (strcmp("page", maptype) == 0)
                {
                    if (type != NO_TYPE && type != PAGE)
                        return genericFailure ("Cannot specify a page map with a previously specified type."); 
                    else
                        type = PAGE;
                }
                else if (strcmp("voice", maptype) == 0)
                {
                    if (type != NO_TYPE && type != VOICE)
                        return genericFailure ("Cannot specify a voice map with a previously specified type.");
                    else
                        type = VOICE;
                }
                else if (strcmp("staff", maptype) == 0)
                {
                    if (type != NO_TYPE && type != STAFF)
                        return genericFailure ("Cannot specify a staff map with a previously specified type.");
                    else
                        type = STAFF;
                }
                else if (strcmp("system", maptype) == 0)
                {
                    if (type != NO_TYPE && type != SYSTEM)
                        return genericFailure ("Cannot specify a system map with a previously specified type."); 
                    else {
                        type = SYSTEM;
                    }
                }
                
            }
            else if (args[n + i].first == "voice")
            {
                aux = atoi(args[n + i].second.c_str());
                if (type != NO_TYPE && type != VOICE)
                    return genericFailure ("Cannot specify voice for this type of map.");
                type = VOICE;
            }
            else if (args[n + i].first == "staff")
            {
                aux = atoi(args[n + i].second.c_str());
                if (type != NO_TYPE && type != STAFF   )
                    return genericFailure ("Cannot specify staff for this type of map.");
                type = STAFF;
            }
            if (type && (type == STAFF || type == VOICE) && n >= args.size () - 4)
                return genericFailure ("You're missing an argument for get=point.");
            else if (type && (type == STAFF || type == VOICE) && n < args.size () - 4)
                movingTarget = 5;
            i++;
        }
        if (!x || !y || !maptype)
            return genericFailure ("Necessary argument not specified for getting map type.");
        if (type != NO_TYPE && (type == STAFF || type == VOICE) && !aux)
            return genericFailure ("Necessary argument not specified for getting map type.");
        GuidoErrCode err;
        Time2GraphicMap outmap;
        switch (type) {
            case PAGE : err = getmap (PAGE, 0, outmap); break;
            case STAFF : err = getmap (STAFF, aux, outmap); break;
            case SYSTEM : err = getmap (SYSTEM, 0, outmap); break;
            case VOICE : err = getmap (VOICE, aux, outmap); break;
            default : return genericFailure ("Exotic programming error.");
        }
        
        if (err != guidoNoErr)
            return genericFailure ("Exotic programming error.");
        
        TimeSegment t;
        FloatRect r;
        GuidoGetPoint(x, y, outmap, t, r);
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        if (url_.size () != 0)
        {
            json_print_pretty(&printer, JSON_KEY, "score", 1);
            json_print_pretty(&printer, JSON_INT, url_.c_str(), strlen(url_.c_str()));
        }
        json_print_pretty(&printer, JSON_KEY, "point", 1);
        stringstream buffer;
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        
        GUIDOjson_print_date (&printer, "begintime", t.first.num, t.first.denom);
        GUIDOjson_print_date (&printer, "endtime", t.second.num, t.second.denom);
        GUIDOjson_print_float_rect(&printer, "floatrec", r.left, r.right, r.top, r.bottom);
        
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_free(&printer);
        int argumentsToAdvance = (type == PAGE || type == SYSTEM) ? 4 : 5;
        return guidosessionresponse(strdup(mystream.str().c_str()), mystream.str().size(), "application/json", "", argumentsToAdvance, GUIDO_SESSION_PARSING_SUCCESS);
    }
    
    void guidosession::setUrl(string url)
    {
        url_ = url;
    }
    
    json_type guidosession::swapTypeForName (string type)
    {
        if (type.compare("width") == 0)
            return JSON_FLOAT;
        else if (type.compare("height") == 0)
            return JSON_FLOAT;
        else if (type.compare("margintop") == 0)
            return JSON_FLOAT;
        else if (type.compare("marginbottom") == 0)
            return JSON_FLOAT;
        else if (type.compare("marginleft") == 0)
            return JSON_FLOAT;
        else if (type.compare("marginright") == 0)
            return JSON_FLOAT;
        else if (type.compare("zoom") == 0)
            return JSON_FLOAT;
        else if (type.compare("format") == 0)
            return JSON_STRING;
        else if (type.compare("gmn") == 0)
            return JSON_STRING;
        else if (type.compare("resizepagetomusic") == 0)
            return JSON_STRING;
        return JSON_NONE;
    }
} // end namespoace
