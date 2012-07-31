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
#include <algorithm>

#include "utilities.h"
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

void _request_completed (void *cls, struct MHD_Connection *connection, 
                        void **con_cls,
                        enum MHD_RequestTerminationCode toe)
{
    struct connection_info_struct *con_info = (connection_info_struct *)(*con_cls);
    
    if (NULL == con_info) return;
    if (con_info->connectiontype == 1) //  we make 1 == POST
    {
        MHD_destroy_post_processor (con_info->postprocessor);        
        //if (con_info->answerstring) free (con_info->answerstring);
    }
    
    free (con_info);
    *con_cls = NULL;   
}
    
static int _get_params (void *cls, enum MHD_ValueKind , const char *key, const char *data)
{
	TArgs* args = (TArgs*)cls;
	TArg arg(key, (data ? data : ""));
	args->push_back (arg);
	return MHD_YES;
}

    
static int 
_post_params (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data, 
              uint64_t off, size_t size)
{
    struct connection_info_struct *con_info = (connection_info_struct *)coninfo_cls;
    TArg arg(key, (data ? data : ""));
    con_info->args.push_back (arg);
    return MHD_YES;
}
    
//--------------------------------------------------------------------------
// the http server
//--------------------------------------------------------------------------
HTTPDServer::HTTPDServer(int port, guido2img* g2svg)  
	: fPort(port), fServer(0), fConverter(g2svg), fDebug(false)
{
}

HTTPDServer::~HTTPDServer() {
    for (map<string, guidosession *>::iterator it = fSessions.begin ();
         it != fSessions.end();
         it++)
        delete it->second;
    
    stop();
}

//--------------------------------------------------------------------------
bool HTTPDServer::start(int port)
{
	fServer = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, port, NULL, NULL, _answer_to_connection, this, MHD_OPTION_NOTIFY_COMPLETED, &_request_completed, NULL, MHD_OPTION_END);
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
    // we first check to see if the session exists
    map<string, guidosession *>::iterator it;
    it = fSessions.find(suburl);
    if (it == fSessions.end ())
    {
        fSessions[suburl] = new guidosession(fConverter);
        fSessions[suburl]->initialize();
        fSessions[suburl]->setUrl(suburl);            
    }
    currentSession = fSessions[suburl];

    unsigned int n = 0;
    guidosession::callback_function callback;
    guidosessionresponse response;
    
	while (n < args.size()) {
		if (args[n].first == "get")
			callback = &guidosession::handleGet;
		else if (args[n].first == "page")
			callback = &guidosession::handlePage;
		else if (args[n].first == "width")
			callback = &guidosession::handleWidth;
		else if (args[n].first == "height")
			callback = &guidosession::handleHeight;
        else if (args[n].first == "marginleft")
			callback = &guidosession::handleMarginLeft;
        else if (args[n].first == "marginright")
			callback = &guidosession::handleMarginRight;
        else if (args[n].first == "margintop")
			callback = &guidosession::handleMarginTop;
        else if (args[n].first == "marginbottom")
			callback = &guidosession::handleMarginBottom;
		else if (args[n].first == "zoom")
			callback = &guidosession::handleZoom;
		else if (args[n].first == "resizepagetomusic")
			callback = &guidosession::handleResizePageToMusic;
        else if (args[n].first == "gmn")
			callback = &guidosession::handleGMN;
        else if (args[n].first == "format")
			callback = &guidosession::handleFormat;
        else if (args[n].first == "")
            callback = &guidosession::handleBlankRequest;
        else
            callback = &guidosession::handleFaultyInput;

        response = (currentSession->*callback)(args, n);

        if (response.status_ == GUIDO_SESSION_PARSING_SUCCESS)
        {
            n += response.argumentsToAdvance_;
        }
        else
        {
            n += 1;
            response.data_ = response.errstring_.c_str();
            response.size_ = response.errstring_.size();
        }
	}
    
    if (suburl.size() == 0)
        currentSession->initialize();

    // Only the final result gets sent.
    const char* formatToSend = response.format_.c_str();
    return send (connection, response.data_, response.size_, formatToSend);
}

//--------------------------------------------------------------------------
int HTTPDServer::answer (struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    // <<---- BEGIN POST TESTING
    if (NULL == *con_cls)
    {
        struct connection_info_struct *con_info = new connection_info_struct ();
        
        if (0 == strcmp (method, "POST"))
        {
            con_info->postprocessor =
            MHD_create_post_processor (connection, 1024, // arbitrary, recommeneded by libmicrohttpd
                                       _post_params, (void *) con_info);
            
            if (NULL == con_info->postprocessor)
            {
                delete con_info;
                return MHD_NO;
            }
            /*
              The connectiontype field of con_info currently does nothing:
             it's a placeholder for using POST/GET distinctions as the server
             becomes more sophisticated.
             
             */
            con_info->connectiontype = POST;
        }
        else
            con_info->connectiontype = GET;
        
        *con_cls = (void *) con_info;
        
        return MHD_YES;
    }
    
    if (0 == strcmp (method, "POST"))
    {
        struct connection_info_struct *con_info = (connection_info_struct *)*con_cls;
        
        if (*upload_data_size != 0)
        {
            MHD_post_process (con_info->postprocessor, upload_data,
                              *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
        }
        else {
            struct connection_info_struct *con_info = (connection_info_struct *)*con_cls;
#ifdef __MACH__
            reverse (con_info->args.begin (), con_info->args.end ());
#endif
            return sendGuido (connection, url, con_info->args);
        }
    }
    if (0 == strcmp (method, "GET"))
    {
     	TArgs args;
        MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, _get_params, &args);
#ifdef __MACH__
        reverse (args.begin(), args.end());
#endif
        return sendGuido (connection, url, args);
    }
}

} // end namespoace
