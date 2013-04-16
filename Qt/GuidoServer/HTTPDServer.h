/*

  Copyright (C) 2012 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef __HTTPDServer__
#define __HTTPDServer__

#include <string>
#include <ostream>
#include <vector>
#include <microhttpd.h>

namespace guidohttpd
{

class guido2img;
//--------------------------------------------------------------------------
/*!
	\brief a specific thread to listen incoming osc packets
*/
class HTTPDServer
{
	int					fPort;
	struct MHD_Daemon *	fServer;
	guido2img*			fConverter;
	bool				fDebug;
	
	int page (struct MHD_Connection *connection, const char *page);
	const char* getMIMEType (const std::string& page);

	public:
		typedef std::pair<std::string, std::string>	TArg;
		typedef std::vector<TArg>					TArgs;

				 HTTPDServer(int port, guido2img* g2img);
		virtual ~HTTPDServer();

		/// \brief starts the httpd server
		bool start (int port);
		void stop ();
		int answer (struct MHD_Connection *connection, const char *url, const char *method, const char *version, 
					const char *upload_data, size_t *upload_data_size, void **con_cls);
		int sendGuido (struct MHD_Connection *connection, const TArgs& args);

		static int send (struct MHD_Connection *connection, const char *page, int length, const char *type, int status=MHD_HTTP_OK);
		static int send (struct MHD_Connection *connection, const char *page, const char *type, int status=MHD_HTTP_OK);
};

} // end namespoace

#endif
