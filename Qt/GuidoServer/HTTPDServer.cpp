/*

  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
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
int HTTPDServer::page (struct MHD_Connection *connection, const char * page)
{
	int ret = 0;
	char * root =  getenv("FAUSTDocumentRoot");
	string file = root ? root : ".";
	file += page;
	const char* type = getMIMEType (file);

	int fd = open (file.c_str(), O_RDONLY);
	if (fd != -1) {
		int length = lseek(fd, 0, SEEK_END);
		lseek(fd, 0, SEEK_SET);
		
		struct MHD_Response *response = MHD_create_response_from_fd (length, fd);
		if (!response ) {
			cerr << "MHD_create_response_from_fd error: null response\n";
			return MHD_NO;
		}
		MHD_add_response_header (response, "Content-Type", type ? type : "text/plain");
		ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
		MHD_destroy_response (response);
	}
	else {
		ret = send (connection, "", 0, MHD_HTTP_NOT_FOUND);
	}
	return ret;
}

//--------------------------------------------------------------------------
int HTTPDServer::sendGuido (struct MHD_Connection *connection, const TArgs& args)
{
	const char* gmn = 0;
	int page = 0;			// default page: all pages
	int width = 0;
	int height = 0;
	float zoom = 0;
	for (unsigned int n = 0; n < args.size(); n++) {
		if (args[n].first == "gmn")
			gmn = args[n].second.c_str();
		if (args[n].first == "page")
			page = atoi (args[n].second.c_str());
		else if (args[n].first == "width")
			width = atoi (args[n].second.c_str());
		else if (args[n].first == "height")
			height = atoi (args[n].second.c_str());
		else if (args[n].first == "zoom")
			zoom = atof (args[n].second.c_str());
	}
	if (!width) width = 400;
	if (!height) height = 600;
	if (!zoom) zoom = 2.0f;
	
	if (!gmn)
		return send (connection, "invalide gmn code", "text/plain");
		
	Guido2ImageErrorCodes err = fConverter->convert (gmn, page, width, height, zoom);
	if (err == GUIDO_2_IMAGE_SUCCESS) {
		return send (connection, fConverter->data(), fConverter->size(), "image/png");
	}
	const char * errstring = Guido2Image::getErrorString (err);
	return send (connection, errstring, "text/plain");
}

//--------------------------------------------------------------------------
int HTTPDServer::answer (struct MHD_Connection *connection, const char *url, const char *method, const char *, const char *, size_t *, void **)
{
	int ret = 0;
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

	if (args.size() >= 1) 
		return sendGuido (connection, args);

	else if (args.size() == 0) {
		const char * file = strcmp (url, "/") ? url : "/guido.html";
		return page (connection, file );
	}
	ret = send (connection, "", 0, MHD_HTTP_NOT_FOUND);
	return ret;
}


} // end namespoace
