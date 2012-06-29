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
#include <fstream>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "HTTPDServer.h"
#include "json.h"
#include "guido2img.h"
#include "guidosession.h"

using namespace std;

namespace guidohttpd
{

    static int printchannel(void *userdata, const char *data, uint32_t length)
    {
        stringstream *foo = (stringstream *)userdata;
        *foo << data;
    }
    
//--------------------------------------------------------------------------
// static functions
// provided as callbacks to mhttpd
//--------------------------------------------------------------------------
static int _answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, 
                          const char *upload_data, size_t *upload_data_size, void **con_cls)
{
	HTTPDServer* server = (HTTPDServer*)cls;
	return server->answer(connection, url, method, version, upload_data, upload_data_size, con_cls); 
}

//--------------------------------------------------------------------------
static int _get_params (void *cls, enum MHD_ValueKind , const char *key, const char *value)
{
	HTTPDServer::TArgs* args = (HTTPDServer::TArgs*)cls;
	HTTPDServer::TArg arg(key, (value ? value : ""));
	args->push_back (arg);
	return MHD_YES;
}

//--------------------------------------------------------------------------
// the http server
//--------------------------------------------------------------------------
HTTPDServer::HTTPDServer(int port, guido2img* g2svg)  
	: fPort(port), fServer(0), fConverter(g2svg), fDebug(true)
{
    anonymousSession.initialize();
}

HTTPDServer::~HTTPDServer() { stop(); }

//--------------------------------------------------------------------------
bool HTTPDServer::start(int port)
{
	fServer = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, port, NULL, NULL, _answer_to_connection, this, MHD_OPTION_END);
	return fServer != 0;
}

//--------------------------------------------------------------------------
void HTTPDServer::stop ()
{ 
	if (fServer) MHD_stop_daemon (fServer); 
	fServer=0;
}

//--------------------------------------------------------------------------
int HTTPDServer::send (struct MHD_Connection *connection, const char *page, int length, const char* type, int status)
    {
	struct MHD_Response *response = MHD_create_response_from_buffer (length, (void *) page, MHD_RESPMEM_MUST_COPY);
	if (!response) {
		cerr << "MHD_create_response_from_buffer error: null response\n";
		return MHD_NO;
	}
	MHD_add_response_header (response, "Content-Type", type ? type : "text/plain");
	int ret = MHD_queue_response (connection, status, response);
	MHD_destroy_response (response);
	return ret;
}

//--------------------------------------------------------------------------
int HTTPDServer::send (struct MHD_Connection *connection, const char *page, const char* type, int status)
{
	return send (connection, page, strlen (page), type, status);
}

//--------------------------------------------------------------------------
const char* HTTPDServer::getMIMEType (const string& page)
{
	size_t n = page.find_last_of ('.');
	if (n != string::npos) {
		string ext = page.substr (n+1);
		if (ext == "css")	return "text/css";
		if (ext == "html")	return "text/html";
		if (ext == "js")	return "application/javascript";
	}
	return "text/plain";		// default MIME type
}

//--------------------------------------------------------------------------
int HTTPDServer::sendGuido (struct MHD_Connection *connection, const char* url, const TArgs& args)
{
    string suburl = string(url).substr(1,string::npos);
    guidosession *currentSession;
    if (suburl.size() == 0)
        currentSession = &anonymousSession;
    else {
        // we first check to see if the session exists
        map<string, guidosession>::iterator it;
        it = namedSessions.find(suburl);
        if (it == namedSessions.end ())
        {
            namedSessions[suburl]; // we initialize
            namedSessions[suburl].initialize();
            namedSessions[suburl].url = suburl;            
        }
        currentSession = &namedSessions[suburl];
    }
    /*
     const char* gmn = 0;
     int page = 0;			// default page: all pages
     int width = 0;
     int height = 0;
     float zoom = 0;
     */
    GuidoSessionParsingError parseError;
    unsigned int n = 0;
    unsigned int argumentsToAdvance;
    const char* data;
    int size;
    const char* format;
    const char* errstring;
    callback_function callback;
    
	while (n < args.size()) {
		if (args[n].first == "get")
			callback = &HTTPDServer::handleGet;
		else if (args[n].first == "page")
			callback = &HTTPDServer::handlePage;
		else if (args[n].first == "width")
			callback = &HTTPDServer::handleWidth;
		else if (args[n].first == "height")
			callback = &HTTPDServer::handleHeight;
        else if (args[n].first == "marginleft")
			callback = &HTTPDServer::handleMarginLeft;
        else if (args[n].first == "marginright")
			callback = &HTTPDServer::handleMarginRight;
        else if (args[n].first == "margintop")
			callback = &HTTPDServer::handleMarginTop;
        else if (args[n].first == "marginbottom")
			callback = &HTTPDServer::handleMarginBottom;
		else if (args[n].first == "zoom")
			callback = &HTTPDServer::handleZoom;
		else if (args[n].first == "resizepagetomusic")
			callback = &HTTPDServer::handleResizePageToMusic;
        else if (args[n].first == "gmn")
			callback = &HTTPDServer::handleGMN;
        else if (args[n].first == "format")
			callback = &HTTPDServer::handleFormat;
        else
            callback = &HTTPDServer::handleFaultyInput;

        parseError = (this->*callback)(currentSession, &size, &data, &format, &errstring, &argumentsToAdvance, args, n);

        if (parseError == GUIDO_SESSION_PARSING_SUCCESS)
        {
            n += argumentsToAdvance;
        }
        else
        {
            n += 1;
            data = strdup(errstring);
            size = strlen(data);
        }
	}
    if (currentSession == &anonymousSession)
        anonymousSession.initialize();

    // Only the final result gets sent.
    return send (connection, data, size, format);
}

//--------------------------------------------------------------------------
int HTTPDServer::answer (struct MHD_Connection *connection, const char *url, const char *method, const char *, const char *, size_t *, void **)
{
	MHD_ValueKind t = MHD_GET_ARGUMENT_KIND;
	if (0 == strcmp (method, "GET"))		t = MHD_GET_ARGUMENT_KIND;
	else if (0 == strcmp (method, "POST"))	t = MHD_POSTDATA_KIND;
	else {
		string msg = "Method ";
		msg += method;
		msg += " is not supported";
		return send (connection, msg.c_str(), 0, MHD_HTTP_BAD_REQUEST);
	}
    
	TArgs args;
	MHD_get_connection_values (connection, t, _get_params, &args);
	if (fDebug) {
		cout << method << ": " << url << " - " << args.size() << " ";
		for (unsigned int i=0; i<args.size(); i++)
			cout << args[i].first << "=" << args[i].second << " ";
		cout << endl;
	}
    
    reverse (args.begin(), args.end());
	return sendGuido (connection, url, args);
}

    // ------ callbacks --------
    
    GuidoSessionParsingError HTTPDServer::handleGet(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        const int SIMPLE_SIZE = 9;
        const char* simple_ids[SIMPLE_SIZE] = {"width", "height", "margintop", "marginbottom", "marginleft", "marginright", "zoom","format","gmn"};
        for (int i = 0; i < SIMPLE_SIZE; i++)
            if (strcmp(simple_ids[i], args[n].second.c_str()) == 0)
                return simpleGet (currentSession, size, data, format, errstring, argumentsToAdvance, simple_ids[i]);

        const int MAP_SIZE = 4;
        const char* map_ids[MAP_SIZE] = {"pagemap","staffmap","voicemap","systemmap"};
        for (int i = 0; i < MAP_SIZE; i++)
            if (strcmp(map_ids[i], args[n].second.c_str()) == 0)
                return mapGet (currentSession, size, data, format, errstring, argumentsToAdvance, args, n, map_ids[i]);

        if (strcmp("point", args[n].second.c_str()) == 0)
            return pointGet (currentSession, size, data, format, errstring, argumentsToAdvance, args, n);
        return genericFailure(size, data, format, errstring, argumentsToAdvance, "Unidentified get.");
    }
    
    GuidoSessionParsingError HTTPDServer::handlePage(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->page = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleWidth(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->width = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleHeight(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->height = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    GuidoSessionParsingError HTTPDServer::handleMarginLeft(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->marginleft = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleMarginRight(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->marginright = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleMarginTop(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->margintop = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleMarginBottom(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->marginbottom = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleZoom(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->zoom  = atof(args[n].second.c_str());
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleResizePageToMusic(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        if (strcmp("true", args[n].second.c_str()) == 0)
            currentSession->resizeToPage = true;
        else if (strcmp("True", args[n].second.c_str()) == 0)
            currentSession->resizeToPage = true;
        else if (strcmp("false", args[n].second.c_str()) == 0)
            currentSession->resizeToPage = false;
        else if (strcmp("False", args[n].second.c_str()) == 0)
            currentSession->resizeToPage = false;
        else
            return genericFailure(size, data, format, errstring, argumentsToAdvance, "Must specify true or false for resizeToPage");
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleGMN(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        currentSession->gmn  = string(args[n].second);
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleFormat(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        if (strcmp("png", args[n].second.c_str()) == 0)
            currentSession->format = GUIDO_WEB_API_PNG;
        else if (strcmp("jpeg", args[n].second.c_str()) == 0)
            currentSession->format = GUIDO_WEB_API_JPEG;
        else if (strcmp("jpg", args[n].second.c_str()) == 0)
            currentSession->format = GUIDO_WEB_API_JPEG;
        else if (strcmp("gif", args[n].second.c_str()) == 0)
            currentSession->format = GUIDO_WEB_API_GIF;
        else if (strcmp("svg", args[n].second.c_str()) == 0)
            currentSession->format = GUIDO_WEB_API_PNG;//KLUDGE
        else
            return genericFailure(size, data, format, errstring, argumentsToAdvance, "Not a valid format - please choose from png, jpeg, gif or svg.");
        return genericReturnImage(currentSession, size, data, format, errstring, argumentsToAdvance);
    }
    
    GuidoSessionParsingError HTTPDServer::handleFaultyInput(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        return genericFailure (size, data, format, errstring, argumentsToAdvance, "You have entered insane input.");
    }
    
    // ---- Abstractions
    
    GuidoSessionParsingError HTTPDServer::genericReturnImage(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance)
    {
        Guido2ImageErrorCodes err = fConverter->convert(currentSession);
        if (err == GUIDO_2_IMAGE_SUCCESS)
        {
            *data = fConverter->data();
            *size = fConverter->size();
            *format = currentSession->formatToMIMEType();
            *errstring = "";
            *argumentsToAdvance = 1;
            return GUIDO_SESSION_PARSING_SUCCESS;
        }
        return genericFailure (size, data, format, errstring, argumentsToAdvance, Guido2Image::getErrorString (err));
    }
    
    GuidoSessionParsingError HTTPDServer::genericFailure(int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const char* errorstring)
    {
        *data = "";
        *size = 0;
        *format = "application/json";
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "error", 1);
        json_print_pretty(&printer, JSON_STRING, errorstring, 1);
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        *errstring = mystream.str().c_str();
        *argumentsToAdvance = 1;
        return GUIDO_SESSION_PARSING_FAILURE;
    }

    GuidoSessionParsingError HTTPDServer::simpleGet (guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const char* thingToGet)
    {
        const char* internalRep = currentSession->getStringRepresentationOf(thingToGet);
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        if (currentSession != &anonymousSession)
        {
            json_print_pretty(&printer, JSON_KEY, "username", 1);
            json_print_pretty(&printer, JSON_STRING, currentSession->url.c_str(), 1);
        }
        json_type type = swapTypeForName(thingToGet);
        if (type == JSON_NONE)
            return genericFailure (size, data, format, errstring, argumentsToAdvance, "Cannot find correct JSON type output.");
        json_print_pretty(&printer, JSON_KEY, thingToGet, 1);
        json_print_pretty(&printer, type, internalRep, type == JSON_STRING ? 1 : strlen(internalRep));
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_free(&printer);
        *data = strdup(mystream.str().c_str());
        *size = strlen(*data);
        *format = "application/json";
        *errstring = "none";
        *argumentsToAdvance = 1;
        return GUIDO_SESSION_PARSING_SUCCESS;
    }
    
    GuidoSessionParsingError HTTPDServer::mapGet (guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n, const char* thingToGet)
    {
        Time2GraphicMap outmap;
        GuidoErrCode err;
        if (strcmp(thingToGet, "pagemap") == 0)
            err = currentSession->getmap (PAGE, 0, outmap);
        else if (strcmp(thingToGet, "systemmap") == 0)
            err = currentSession->getmap (SYSTEM, 0, outmap);
        else
        {
            if (n >= args.size () - 1)
                return genericFailure (size, data, format, errstring, argumentsToAdvance, "You need an extra argument for this map call.");
            if (strcmp(thingToGet, "systemmap") == 0 && strcmp(args[n + 1].second.c_str(), "voice") != 0)
                return genericFailure (size, data, format, errstring, argumentsToAdvance, "To get a voicemap, the argument must be followed by a voice argument indicating which voice you want.");
            if (strcmp(thingToGet, "systemmap") == 0 && strcmp(args[n + 1].second.c_str(), "staff") != 0)
                return genericFailure (size, data, format, errstring, argumentsToAdvance, "To get a staffmap, the argument must be followed by a staff argument indicating which staff you want.");
            int aux = atoi(args[n+1].second.c_str());
            if (strcmp(thingToGet, "staffmap") == 0)
                err = currentSession->getmap (STAFF, aux, outmap);
            else if (strcmp(thingToGet, "voicemap") == 0)
                err = currentSession->getmap (VOICE, aux, outmap);
        }
        if (err != guidoNoErr)
            return genericFailure (size, data, format, errstring, argumentsToAdvance, "Could not generate a map for your request.");
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        if (currentSession != &anonymousSession)
        {
            json_print_pretty(&printer, JSON_KEY, "username", 1);
            json_print_pretty(&printer, JSON_INT, currentSession->url.c_str(), strlen(currentSession->url.c_str()));
        }
        json_print_pretty(&printer, JSON_KEY, thingToGet, 1);
        json_print_pretty(&printer, JSON_ARRAY_BEGIN, NULL, 0);
        stringstream buffer;
        for (int i = 0; i < outmap.size(); i++)
        {
            json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
                json_print_pretty(&printer, JSON_KEY, "begintime", 1);
                json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
                    json_print_pretty(&printer, JSON_KEY, "num", 1);
                        buffer.str("");
                        buffer << outmap[i].first.first.num;
                    json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                    json_print_pretty(&printer, JSON_KEY, "denom", 1);
                        buffer.str("");
                        buffer << outmap[i].first.first.denom;
                    json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
                json_print_pretty(&printer, JSON_KEY, "endtime", 1);
                json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
                    json_print_pretty(&printer, JSON_KEY, "num", 1);
                        buffer.str("");
                        buffer << outmap[i].first.second.num;
                    json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                    json_print_pretty(&printer, JSON_KEY, "denom", 1);
                        buffer.str("");
                        buffer << outmap[i].first.second.denom;
                    json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
                json_print_pretty(&printer, JSON_KEY, "floatrec", 1);
                json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
                    json_print_pretty(&printer, JSON_KEY, "left", 1);
                        buffer.str("");
                        buffer << outmap[i].second.left;
                    json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                    json_print_pretty(&printer, JSON_KEY, "right", 1);
                        buffer.str("");
                        buffer << outmap[i].second.right;
                    json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                    json_print_pretty(&printer, JSON_KEY, "top", 1);
                        buffer.str("");
                        buffer << outmap[i].second.top;
                    json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                    json_print_pretty(&printer, JSON_KEY, "bottom", 1);
                        buffer.str("");
                        buffer << outmap[i].second.bottom;
                    json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
                json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
            json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        }
        json_print_pretty(&printer, JSON_ARRAY_END, NULL, 0);
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_free(&printer);
        *data = strdup(mystream.str().c_str());
        *size = strlen(*data);
        *format = "application/json";
        *errstring = "none";
        *argumentsToAdvance = (strcmp(thingToGet, "pagemap") == 0 || strcmp(thingToGet, "systemmap") == 0) ? 1 : 2;
        return GUIDO_SESSION_PARSING_SUCCESS;
    }
    GuidoSessionParsingError HTTPDServer::pointGet (guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        if (n >= args.size () - 3)
            return genericFailure (size, data, format, errstring, argumentsToAdvance, "You need both x and y arguments as well as a mapping specification for get=point.");
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
                        return genericFailure (size, data, format, errstring, argumentsToAdvance, "Cannot specify a page map with a previously specified type."); 
                    else
                        type = PAGE;
                        }
                else if (strcmp("voice", maptype) == 0)
                {
                    if (type != NO_TYPE && type != VOICE)
                    return genericFailure (size, data, format, errstring, argumentsToAdvance, "Cannot specify a voice map with a previously specified type.");
                    else
                        type = VOICE;
                }
                else if (strcmp("staff", maptype) == 0)
                {
                    if (type != NO_TYPE && type != STAFF)
                    return genericFailure (size, data, format, errstring, argumentsToAdvance, "Cannot specify a staff map with a previously specified type.");
                    else
                        type = STAFF;
                }
                else if (strcmp("system", maptype) == 0)
                {
                    if (type != NO_TYPE && type != SYSTEM)
                    return genericFailure (size, data, format, errstring, argumentsToAdvance, "Cannot specify a system map with a previously specified type."); 
                    else {
                        type = SYSTEM;
                    }
                }
                
            }
            else if (args[n + i].first == "voice")
            {
                aux = atoi(args[n + i].second.c_str());
                if (type != NO_TYPE && type != VOICE)
                    return genericFailure (size, data, format, errstring, argumentsToAdvance, "Cannot specify voice for this type of map.");
                type = VOICE;
            }
            else if (args[n + i].first == "staff")
            {
                aux = atoi(args[n + i].second.c_str());
                if (type != NO_TYPE && type != STAFF   )
                    return genericFailure (size, data, format, errstring, argumentsToAdvance, "Cannot specify staff for this type of map.");
                type = STAFF;
            }
            if (type && (type == STAFF || type == VOICE) && n >= args.size () - 4)
                return genericFailure (size, data, format, errstring, argumentsToAdvance, "You're missing an argument for get=point.");
            else if (type && (type == STAFF || type == VOICE) && n < args.size () - 4)
                movingTarget = 5;
            i++;
        }
        if (!x || !y || !maptype)
            return genericFailure (size, data, format, errstring, argumentsToAdvance, "Necessary argument not specified for getting map type.");
        if (type != NO_TYPE && (type == STAFF || type == VOICE) && !aux)
            return genericFailure (size, data, format, errstring, argumentsToAdvance, "Necessary argument not specified for getting map type.");
        GuidoErrCode err;
        Time2GraphicMap outmap;
        switch (type) {
            case PAGE : err = currentSession->getmap (PAGE, 0, outmap); break;
            case STAFF : err = currentSession->getmap (STAFF, aux, outmap); break;
            case SYSTEM : err = currentSession->getmap (SYSTEM, 0, outmap); break;
            case VOICE : err = currentSession->getmap (VOICE, aux, outmap); break;
            default : return genericFailure (size, data, format, errstring, argumentsToAdvance, "Exotic programming error.");
        }
        
        if (err != guidoNoErr)
            return genericFailure (size, data, format, errstring, argumentsToAdvance, "Exotic programming error.");

        TimeSegment t;
        FloatRect r;
        GuidoGetPoint(x, y, outmap, t, r);
        json_printer printer;
        stringstream mystream;
        json_print_init(&printer, printchannel, &mystream);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        if (currentSession != &anonymousSession)
        {
            json_print_pretty(&printer, JSON_KEY, "username", 1);
            json_print_pretty(&printer, JSON_INT, currentSession->url.c_str(), strlen(currentSession->url.c_str()));
        }
        json_print_pretty(&printer, JSON_KEY, "point", 1);
        stringstream buffer;
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "begintime", 1);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "num", 1);
        buffer.str("");
        buffer << t.first.num;
        json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_KEY, "denom", 1);
        buffer.str("");
        buffer << t.first.denom;
        json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "endtime", 1);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "num", 1);
        buffer.str("");
        buffer << t.second.num;
        json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_KEY, "denom", 1);
        buffer.str("");
        buffer << t.second.denom;
        json_print_pretty(&printer, JSON_INT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "floatrec", 1);
        json_print_pretty(&printer, JSON_OBJECT_BEGIN, NULL, 0);
        json_print_pretty(&printer, JSON_KEY, "left", 1);
        buffer.str("");
        buffer << r.left;
        json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_KEY, "right", 1);
        buffer.str("");
        buffer << r.right;
        json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_KEY, "top", 1);
        buffer.str("");
        buffer << r.top;
        json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_KEY, "bottom", 1);
        buffer.str("");
        buffer << r.bottom;
        json_print_pretty(&printer, JSON_FLOAT, buffer.str().c_str(), strlen(buffer.str().c_str()));
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_pretty(&printer, JSON_OBJECT_END, NULL, 0);
        json_print_free(&printer);
        *data = strdup(mystream.str().c_str());
        *size = strlen(*data);
        *format = "application/json";
        *errstring = "none";
        *argumentsToAdvance = (type == PAGE || type == SYSTEM) ? 4 : 5;
        return GUIDO_SESSION_PARSING_SUCCESS;
    }
    
    json_type HTTPDServer::swapTypeForName (const char* type)
    {
        if (strcmp("width", type) == 0)
            return JSON_FLOAT;
        else if (strcmp("height", type) == 0)
            return JSON_FLOAT;
        else if (strcmp("margintop", type) == 0)
            return JSON_FLOAT;
        else if (strcmp("marginbottom", type) == 0)
            return JSON_FLOAT;
        else if (strcmp("marginleft", type) == 0)
            return JSON_FLOAT;
        else if (strcmp("marginright", type) == 0)
            return JSON_FLOAT;
        else if (strcmp("zoom", type) == 0)
            return JSON_FLOAT;
        else if (strcmp("format", type) == 0)
            return JSON_STRING;
        else if (strcmp("gmn", type) == 0)
            return JSON_STRING;
        return JSON_NONE;
    }
} // end namespoace
