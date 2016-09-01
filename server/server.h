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


#ifndef __HTTPDServer__
#define __HTTPDServer__

#include <string>
#include <ostream>
#include <vector>
#include <map>
#include <microhttpd.h>

#include "guidosession.h"
#include "guido2img.h"

// json
#include "json_object.h"
#include "json_array.h"
#include "json_parser.h"

#define IP_VERBOSE 1
#define HEADER_VERBOSE 2
#define REQUEST_VERBOSE 4
#define URL_VERBOSE 8
#define QUERY_VERBOSE 16
#define CODE_VERBOSE 32
#define MIME_VERBOSE 64
#define LENGTH_VERBOSE 128

#define HEAD 3
#define DELETE 2
#define POST 1
#define GET 0

namespace guidohttpd
{

class guido2img;
//--------------------------------------------------------------------------
/*!
 \brief a specific thread to listen incoming osc packets
 */

struct connection_info_struct {
    int connectiontype;
    TArgs args;
    struct MHD_PostProcessor *postprocessor;
};
class HTTPDServer
{
	const std::string & fSvgFontFile;
	bool fAccessControlAllowOrigin;
    int fVerbose;
    int fLogmode;
	std::string fCachedir;
	struct MHD_Daemon *	fServer;
    int fMaxSessions;
    std::map<std::string, guidosession *> fSessions;
	bool fUseCache;

public:

	/*!
	 * \brief HTTPDServer. The guido engine has to be initailized to create the HTTPDserver.
	 * \param verbose flags for logging
	 * \param logmode mode of log (0 for Apache-like log or 1 for XML log file)
	 * \param cachedir
	 * \param g2img
	 * \param allowOrigin if true, Access-Control-Allow-Origin is set to '*' in http response header to allow cross domain request.
	 * \param maxSession. The maximum session hold in the server. The session are hold in a circular buffer. The session have no
	 * expiration time but when maxSession is reach, the first session is deleted.
	 */
	HTTPDServer(const std::string &svgfontfile, int verbose, int logmode, std::string cachedir, bool allowOrigin, int maxSession = 100, bool useCache = true);
    virtual ~HTTPDServer();

    /// \brief starts the httpd server
    bool start (int port);
	/// \brief stop the httpd server
    void stop ();

	/*!
	 * \brief answer : Main method to respond to a request.
	 * \param connection
	 * \param url
	 * \param method
	 * \param version
	 * \param upload_data
	 * \param upload_data_size
	 * \param con_cls
	 * \return #MHD_NO on error (i.e. reply already sent),
	 *         #MHD_YES on success or if message has been queued
	 */
	int answer (struct MHD_Connection *connection, const char *url, const char *method, const char *version,
                const char *upload_data, size_t *upload_data_size, void **con_cls);

	/*!
	 * \brief readFromCache. Read data from cache.
	 * \param target a session id to reada particular session. If no arg is passed, this method load all session save in cache
	 */
	void readFromCache(std::string target = "");

	std::string getSvgFontFile() { return fSvgFontFile; }

private:
	/*!
	 * \brief sendGuidoGetHead. Perform a request with method get or head.
	 * \param connection
	 * \param url
	 * \param args
	 * \param type
	 * \return #MHD_NO on error (i.e. reply already sent),
	 *         #MHD_YES on success or if message has been queued
	 */
	int sendGuidoGetHead (struct MHD_Connection *connection, const char* url, const TArgs& args, int type, const std::vector<std::string> &elems);

	/*!
	 * \brief sendGuidoPostRequest. Perform POST request to register guido code. Only request with 'data' argument are valid.
	 * All other generate an error. If the request is correct registerGMN is called and the response is send with int send (struct MHD_Connection *, guidosessionresponse &);
	 * \param connection
	 * \param args
	 * \return #MHD_NO on error (i.e. reply already sent),
	 *         #MHD_YES on success or if message has been queued
	 */
	int sendGuidoPostRequest (struct MHD_Connection *connection, const TArgs& args, const std::vector<std::string> &elems);

	/*!
	 * \brief sendGuidoDeleteRequest. Delete a session on server.
	 * If the server use cache, the session is deleted in cache too.
	 * \param connection
	 * \param args
	 * \return
	 */
	int sendGuidoDeleteRequest (struct MHD_Connection *connection, const TArgs& args, const std::vector<std::string> &elems);

	/*!
	 * \brief registerGMN register gmn code for a session. Create a new object guidosession.
	 * Store gmn on cache if it is activated.
	 * \param unique_id id of the session
	 * \param gmn code guido
	 * \param isStreamSession create a session which use stream api.
	 * \param useCache write/delete or not in cache on disk
	 * \return a guidosessionresponse
	 */
	guidosessionresponse registerGMN(std::string unique_id, std::string gmn, bool isStreamSession, bool useCache);

	/*!
	 * \brief send Send reponse with the connection connection.
	 * Retreive response format and call send (struct MHD_Connection*, const char *, int , const char *, int) to
	 *  make a low level response with a guidosessionresponse
	 * \param connection a MHD_Connection to respond to the request
	 * \param response to send.
	 * \return #MHD_NO on error (i.e. reply already sent),
	 *         #MHD_YES on success or if message has been queued
	 */
    int send (struct MHD_Connection *connection, guidosessionresponse &response);

	/*!
	 * \brief send send the response.
	 * Make a MHD_Response.
	 *
	 * \param connection
	 * \param page data to send
	 * \param length size of the data to send
	 * \param type type of the data (or format)
	 * \param status Http status to send
	 * \return #MHD_NO on error (i.e. reply already sent),
	 *         #MHD_YES on success or if message has been queued
	 */
    int send (struct MHD_Connection *connection, const char *page, int length, const char *type, int status=MHD_HTTP_OK);

	/*!
	 * \brief logSendGuido log method used before method send
	 * \param connection
	 * \param url
	 * \param args
	 * \param type
	 */
	void logSendGuido(struct MHD_Connection *connection, const char* url, const TArgs& args, const char * type);

	/*!
	 * \brief findSession find an existing session
	 * \param sessionId id of the session
	 * \return a guidosession pointer or null if not found.
	 */
	guidosession* findSession(std::string sessionId);
};

} // end namespoace

#endif
