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
