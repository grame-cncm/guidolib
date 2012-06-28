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
#include "guido2img.h"
#include "guidosession.h"

using namespace std;

namespace guidohttpd
{

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
    guidosession *currentSession;
    if (!url)
        currentSession = &anonymousSession;
    else {
        // we first check to see if the session exists
        map<string, guidosession>::iterator it;
        it = namedSessions.find(url);
        if (it == namedSessions.end ())
        {
            namedSessions[url]; // we initialize
            namedSessions[url].initialize();
            
        }
        currentSession = &namedSessions[url];
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
    int finalMessageToSend = 0;
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
            finalMessageToSend = send (connection, data, size, format);            
        }
        else
        {
            n += 1;
            cout << errstring << " " << format;
            finalMessageToSend = send(connection, errstring, format); // show something about the error...
        }
	}
    return finalMessageToSend;}

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
    
	return sendGuido (connection, url, args);
}

    // ------ callbacks --------
    
    GuidoSessionParsingError HTTPDServer::handleGet(guidosession* currentSession, int* size, const char** data, const char** format, const char** errstring, unsigned int* argumentsToAdvance, const HTTPDServer::TArgs& args, unsigned int n)
    {
        *data = "All get calls are not yet implemented. Please check back later.";
        *size = strlen(*data);
        *format = "text/plain";
        *errstring = "none";
        *argumentsToAdvance = 1;
        return GUIDO_SESSION_PARSING_SUCCESS;
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
        else if (strcmp("gif", args[n].second.c_str()) == 0)
            currentSession->format = GUIDO_WEB_API_SVG;
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
        *format = "text/plain";
        *errstring = errorstring;
        *argumentsToAdvance = 1;
        return GUIDO_SESSION_PARSING_FAILURE;
    }


} // end namespoace
