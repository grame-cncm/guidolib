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

#include <arpa/inet.h>

#include "tinydir.h"

#ifdef WIN32
  #include <libgen.h>
#else
  #include <sys/stat.h>
#endif


#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <algorithm>

#include "utilities.h"
#include "server.h"
#include "guido2img.h"
#include "guidosession.h"

// json
#include "json_object.h"
#include "json_array.h"
#include "json_parser.h"
#include "json_stream.h"
#include "JSONTime2GraphicMap.h"

#include "date_tools.h"

// curl
#include <curl/curl.h>

#include "openssl/sha.h"

using namespace json;
using namespace std;

#define COOKIE_NAME "guidoserver"
#define kInetAddrLen	128

namespace guidohttpd
{

// mymkdir is system dependent
int mymkdir(string name) {
  #ifdef WIN32
    return _mkdir(name.c_str());
  #else
    return mkdir(name.c_str(), 0777);
  #endif
}

//--------------------------------------------------------------------------
// static functions
// provided as callbacks to mhttpd
//--------------------------------------------------------------------------

static int _on_client_connect(void* cls, const sockaddr* addr, socklen_t addrlen)
{
   (void) cls;
   (void) addr;
   (void) addrlen;
   // do nothing.
   // one day, if we want to limit nefarious connections,
   // this is the place
   return MHD_YES;
}

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
    (void) cls;
    (void) connection;
    (void) toe;
    struct connection_info_struct *con_info = (connection_info_struct *)(*con_cls);

    if (NULL == con_info) {
        return;
    }

    if (con_info->connectiontype == POST) {
        MHD_destroy_post_processor (con_info->postprocessor);
        //if (con_info->answerstring) free (con_info->answerstring);
    }

	delete con_info;
    *con_cls = NULL;
}

static int _get_params (void *cls, enum MHD_ValueKind , const char *key, const char *data)
{
    TArgs* args = (TArgs*)cls;
    TArg arg(key, (data ? data : ""));
    args->insert (arg);
    return MHD_YES;
}


static int _post_params (void *coninfo_cls, enum MHD_ValueKind , const char *key,
              const char *filename, const char *content_type,
              const char *transfer_encoding, const char *data,
              uint64_t off, size_t size)
{
    (void) filename;
    (void) content_type;
    (void) transfer_encoding;
    (void) off;
    (void) size;
    struct connection_info_struct *con_info = (connection_info_struct *)coninfo_cls;
    if (strcmp (key, "data") == 0) {
        if (con_info->args.find("data") != con_info->args.end()) {
          con_info->args["data"] += string(data);
        }
        else {
          con_info->args.insert(TArg("data", string(data)));
        }
    }
    return MHD_YES;
}

//--------------------------------------------------------------------------
// the http server
//--------------------------------------------------------------------------
HTTPDServer::HTTPDServer(const string & svgfontfile, int verbose, int logmode, string cachedir, bool alloworigin, int maxSession, bool useCache)
	: fSvgFontFile(svgfontfile), fAccessControlAllowOrigin(alloworigin), fVerbose(verbose), fLogmode(logmode), fCachedir(cachedir), fServer(0),
	  fMaxSessions(maxSession), fUseCache(useCache)
{
	// Initialize default value of guidosession
	GuidoGetDefaultLayoutSettings(&guidosession::sDefaultScoreParameters.guidoParameters.layoutSettings);

	//GuidoGetDefaultPageFormat(&guidosession::sDefaultParameters.guidoParameters.pageFormat);
	guidosession::sDefaultScoreParameters.guidoParameters.pageFormat.width = GuidoCM2Unit(15);
	guidosession::sDefaultScoreParameters.guidoParameters.pageFormat.height = GuidoCM2Unit(5);
	guidosession::sDefaultScoreParameters.guidoParameters.pageFormat.marginleft = GuidoCM2Unit(1);
	guidosession::sDefaultScoreParameters.guidoParameters.pageFormat.margintop = GuidoCM2Unit(1);
	guidosession::sDefaultScoreParameters.guidoParameters.pageFormat.marginright = GuidoCM2Unit(1);
	guidosession::sDefaultScoreParameters.guidoParameters.pageFormat.marginbottom = GuidoCM2Unit(1);

	guidosession::sDefaultScoreParameters.page = 1;
	guidosession::sDefaultScoreParameters.format = GUIDO_WEB_API_PNG;

	// Initialize piano roll settings
	guidosession::sDefaultPianorollParameters.limitParams.startDate = {0, 0};
	guidosession::sDefaultPianorollParameters.limitParams.endDate = {0, 0};
	guidosession::sDefaultPianorollParameters.limitParams.lowPitch = -1;
	guidosession::sDefaultPianorollParameters.limitParams.highPitch = -1;

	guidosession::sDefaultPianorollParameters.enableKeyboard = false;
	guidosession::sDefaultPianorollParameters.enableAutoVoicesColoration = false;
	guidosession::sDefaultPianorollParameters.enableMeasureBars = false;
	guidosession::sDefaultPianorollParameters.pitchLinesDisplayMode = kAutoLines;

	guidosession::sDefaultPianorollParameters.width = 1024;
	guidosession::sDefaultPianorollParameters.height = 512;

	guidosession::sDefaultPianorollParameters.format = GUIDO_WEB_API_PNG;
	guidosession::sDefaultPianorollParameters.type = kSimplePianoRoll;

	// Create cache folder if it not already exist
	if(fUseCache) {
		tinydir_dir myDir;
		int success = tinydir_open_sorted(&myDir, fCachedir.c_str());
		if (success != 0) {
			// It doesn't exist
			success = mymkdir(fCachedir);
		}
		tinydir_close(&myDir);
	}
}

HTTPDServer::~HTTPDServer()
{
	// Delete all session.
    for (map<string, guidosession *>::iterator it = fSessions.begin();
            it != fSessions.end(); it++) {
        delete it->second;
    }
    stop();
}

//--------------------------------------------------------------------------
bool HTTPDServer::start(int port)
{
	// USE_SELECT_INTERALLY makes the server single threaded
    // this guarantees that operations will be queued, which prevents
	// issues involving the use of shared resources.
	// TODO GGX make guido engine multithread complient and change this option.
    fServer = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, port,
                                _on_client_connect,
                                NULL, _answer_to_connection, this,
                                MHD_OPTION_NOTIFY_COMPLETED,
                                _request_completed,
                                NULL, MHD_OPTION_END);
    return fServer != 0;
}

//--------------------------------------------------------------------------
void HTTPDServer::stop ()
{
    if (fServer) {
        MHD_stop_daemon (fServer);
    }
    fServer=0;
}

//--------------------------------------------------------------------------
int HTTPDServer::send (struct MHD_Connection *connection, guidosessionresponse &response)
{
	const char *format =  response.fFormat.c_str();
	return send (connection, response.fData, response.fSize, format, response.fHttpStatus);
}

//--------------------------------------------------------------------------
int HTTPDServer::send (struct MHD_Connection *connection, const char *page, int length, const char* type, int status)
{
    if (fVerbose > 0) {
      if (fLogmode == 0) {
        const char *sep = " ; ";
        if (fVerbose & CODE_VERBOSE) {
          log << status << sep;
        }
        if (fVerbose & MIME_VERBOSE) {
          log << type << sep;
        }
        if (fVerbose & LENGTH_VERBOSE) {
          log << length << sep;
        }
        log << logend;
      }
      else if (fLogmode == 1) {
        const char *tab = "  ";
        if (fVerbose & CODE_VERBOSE) {
          log << tab << "<code>" << logend;
          log << tab << tab << status << logend;
          log << tab << "</code>" << logend;
        }
        if (fVerbose & MIME_VERBOSE) {
          log << tab << "<mime>" << logend;
          log << tab << tab << type << logend;
          log << tab << "</mime>" << logend;
        }
        if (fVerbose & LENGTH_VERBOSE) {
          log << tab << "<length>" << logend;
          log << tab << tab << length << logend;
          log << tab << "</length>" << logend;
        }
        log << "</entry>" << logend;
      }
    }
    struct MHD_Response *response = MHD_create_response_from_buffer (length, (void *) page, MHD_RESPMEM_MUST_COPY);
    if (!response) {
        cerr << "MHD_create_response_from_buffer error: null response\n";
        return MHD_NO;
    }
    MHD_add_response_header (response, MHD_HTTP_HEADER_CONTENT_TYPE, type ? type : "text/plain");
    if (fAccessControlAllowOrigin)
		MHD_add_response_header (response, MHD_HTTP_HEADER_ACCESS_CONTROL_ALLOW_ORIGIN, "*");
    int ret = MHD_queue_response (connection, status, response);
    MHD_destroy_response (response);
    return ret;
}

std::string generate_sha1(std::string setter)
{
  unsigned char obuf[20];
  SHA1((const unsigned char *)(setter.c_str()), setter.length(), obuf);
  char buffer[20];
  std::stringstream ss;

  for (int i=0; i < 20; i++) {
     sprintf(buffer, "%02x", obuf[i]);
     ss << buffer;
  }

  return ss.str();
}

void HTTPDServer::readFromCache(string target)
{
  tinydir_dir myDir;
  tinydir_open_sorted(&myDir, fCachedir.c_str());

  for (int i = 0; i < myDir.n_files; i++) {
    if (i >= fMaxSessions) {
      tinydir_close(&myDir);
      return;
    }

    tinydir_file maybeSubDir;
    tinydir_readfile_n(&myDir, &maybeSubDir, i);
    if (!maybeSubDir.is_dir) {
      continue;
    }
    
    string folder(maybeSubDir.name);
    if ((folder == ".") || (folder == "..")) {
      continue;
    }

    if ((target != "") && (folder.substr(0,2) != target.substr(0,2))) {
      continue;
    }

    tinydir_dir subDir;
    tinydir_open_sorted(&subDir, (fCachedir + "/" +string(maybeSubDir.name)).c_str());
    for (int j = 0; j < subDir.n_files; j++) {
      tinydir_file file;
      tinydir_readfile_n(&subDir, &file, j);

      string fn(file.name);
      if ((fn == ".") || (fn == "..")) {
        continue;
      }

      if ((target != "") && (fn.substr(0,40) != target)) {
        continue;
      }

      if (fn.length() != 44)
        continue;

      if (fn.substr(41,3) != "gmn")
        continue;

      string unique_id = fn.substr(0,40);
      ifstream myfile ((fCachedir+"/"+unique_id.substr(0,2)+"/"+unique_id+".gmn").c_str());
      if (!myfile.is_open())
        continue;

      stringstream ss;
      string line;
      while (getline(myfile, line))
      {
        ss << line << '\n';
      }
      myfile.close();
      
      string all = ss.str();

	  (void) registerGMN(unique_id, all, false);
      if (target != "") {
        tinydir_close(&myDir);
        tinydir_close(&subDir);
        return;
      }
    }
    tinydir_close(&subDir);
  }
  tinydir_close(&myDir);
}

guidosessionresponse HTTPDServer::registerGMN(string unique_id, string gmn, bool useCache)
{
    if (fSessions.find (unique_id) == fSessions.end ()) {
	  guidosession *maybe = new guidosession(this->getSvgFontFile(), gmn, unique_id);
      if (!maybe->success()) {
        string error = maybe->errorMsg();
        delete maybe;
        return guidosession::genericFailure(error, 400);
      }
      if (fSessions.size() >= fMaxSessions) {
        guidosession *toDelete = fSessions.begin()->second;
        fSessions.erase(fSessions.begin());
        delete toDelete;

		if(useCache) {
			// delete file in cache
			std::remove((fCachedir+'/'+unique_id.substr(0,2)+'/'+unique_id+".gmn").c_str());
		}
      }
      fSessions[unique_id] = maybe;
    }
	if(useCache) {
		tinydir_dir myDir;
		int success = tinydir_open_sorted(&myDir, (fCachedir+'/'+unique_id.substr(0,2)).c_str());
		if (success != 0) {
			success = mymkdir(fCachedir+'/'+unique_id.substr(0,2));
			if (success != 0) {
				// can't do io
				return fSessions[unique_id]->genericReturnId();
			}
		}

		tinydir_close(&myDir);
		ofstream myfile((fCachedir+'/'+unique_id.substr(0,2)+'/'+unique_id+".gmn").c_str());
		if (myfile.is_open()) {
			myfile << gmn;
			myfile.close();
		}
	}
    return fSessions[unique_id]->genericReturnId();
}

int HTTPDServer::sendGuidoPostRequest(struct MHD_Connection *connection, const TArgs& args, vector<string> &elems)
{
	if (elems.size()) {
		guidosessionresponse response = guidosession::genericFailure("POST Requests MUST have no element in URL.", 403);
		return send (connection, response);
	}

    if (args.size() != 1) {
        guidosessionresponse response = guidosession::genericFailure("Requests without scores MUST only contain one field called `data'.", 403);
        return send (connection, response);
    }
    if (args.begin()->first != "data") {
        guidosessionresponse response = guidosession::genericFailure("Requests without scores MUST only contain one field called `data'.", 403);
        return send (connection, response);
    }
    std::string unique_id;
    unique_id = generate_sha1(args.begin()->second);
    // if the score does not exist already, we put it there
	guidosessionresponse response = registerGMN(unique_id, args.begin()->second, fUseCache);
    return send (connection, response);
}

int HTTPDServer::sendGuidoDeleteRequest(struct MHD_Connection *connection, const TArgs& args)
{
    if (!args.size () || args.size () > 1 || args.begin()->first != "ID") {
		guidosessionresponse response = guidosession::genericFailure("There may be one and only one argument called \"ID\" passed to DELETE.", 405);
        return send(connection, response);
    } else if (fSessions.find (args.begin()->second) == fSessions.end ()) {
		guidosessionresponse response = guidosession::genericFailure(("Cannot delete the score with ID "+(args.begin()->second)+" because it does not exist.").c_str(), 404);
		return send(connection, response);
	}
	// else
	string sessionId = args.begin()->second;
	guidosession *toErase = fSessions[sessionId];
	fSessions.erase (args.begin()->second);
	delete toErase;

	if(fUseCache) {
		std::remove((fCachedir+'/'+sessionId.substr(0,2)+'/'+ sessionId +".gmn").c_str());
	}

	guidosessionresponse response = guidosession::handleSimpleStringQuery("Success", ("Successfully removed the score with ID "+ sessionId +".").c_str());
    return send (connection, response);
}

void HTTPDServer::logSendGuido(struct MHD_Connection *connection, const char* url, const TArgs& args, const char * type) {
	// LOGFILE.
	if (fVerbose > 0) {
	  if (fLogmode == 0) {
		const char *sep = " ; ";
		log << log.date() << sep;
		if (fVerbose & IP_VERBOSE) {
		  struct sockaddr *so;
		  char buf[kInetAddrLen];
		  so = MHD_get_connection_info (connection,
										MHD_CONNECTION_INFO_CLIENT_ADDRESS)->client_addr;
		  log << inet_ntop(so->sa_family,
						   so->sa_data + 2, buf, kInetAddrLen) << sep;
		}
		if (fVerbose & HEADER_VERBOSE) {
		  TArgs headerArgs;
		  MHD_get_connection_values (connection, MHD_HEADER_KIND, _get_params, &headerArgs);
		  if (headerArgs.size()) {
			bool ampersand = false;
			for(TArgs::const_iterator it = headerArgs.begin(); it != headerArgs.end(); it++) {
			  if (!ampersand)
				ampersand = true;
			  else
				log << "&";
			  log << it->first;
			  log << "=";
			  log << it->second;
			}
		  }
		  log << sep;
		}
		if (fVerbose & REQUEST_VERBOSE) {
		  log << type << sep;
		}
		if (fVerbose & URL_VERBOSE) {
		  log << url << sep;
		}
		if (fVerbose & QUERY_VERBOSE) {
		  if (args.size()) {
			bool ampersand = false;
			for(TArgs::const_iterator it = args.begin(); it != args.end(); it++) {
			  if (!ampersand)
				ampersand = true;
			  else
				log << "&";
			  log << it->first;
			  log << "=";
			  log << curl_escape(it->second.c_str (), 0);
			}
		  }
		  log << sep;
		}
		// we close the entry when we send
	  }
	  else if (fLogmode == 1) {
		const char *tab = "  ";
		log << "<entry>" << logend;
		log << tab << "<date>" << logend;
		log << tab << tab << log.date() << logend;
		log << tab << "</date>" << logend;
		if (fVerbose & IP_VERBOSE) {
		  struct sockaddr *so;
		  char buf[kInetAddrLen];
		  so = MHD_get_connection_info (connection,
										MHD_CONNECTION_INFO_CLIENT_ADDRESS)->client_addr;
		  log << tab << "<ip>" << logend;
		  log << tab << tab << inet_ntop(so->sa_family,
						   so->sa_data + 2, buf, kInetAddrLen)
			  << logend;
		  log << tab << "</ip>" << logend;
		}
		if (fVerbose & HEADER_VERBOSE) {
		  TArgs headerArgs;
		  MHD_get_connection_values (connection, MHD_HEADER_KIND, _get_params, &headerArgs);
		  if (headerArgs.size()) {
			log << tab << "<header>" << logend;
			for(TArgs::const_iterator it = headerArgs.begin(); it != headerArgs.end(); it++) {
			  log << tab << tab << "<pair>" << logend;
			  log << tab << tab << tab << "<name>" << logend;
			  log << tab << tab << tab << tab << it->first << logend;
			  log << tab << tab << tab << "</name>" << logend;
			  log << tab << tab << tab << "<value>" << logend;
			  log << tab << tab << tab << tab << it->second << logend;
			  log << tab << tab << tab << "</value>" << logend;
			  log << tab << tab << "</pair>" << logend;
			}
			log << tab << "</header>" << logend;
		  }
		}
		if (fVerbose & REQUEST_VERBOSE) {
		  log << tab << "<method>" << logend;
		  log << tab << tab << type << logend;
		  log << tab << "</method>" << logend;
		}
		if (fVerbose & URL_VERBOSE) {
		  log << tab << "<url>" << logend;
		  log << tab << tab << url << logend;
		  log << tab << "</url>" << logend;
		}
		if (fVerbose & QUERY_VERBOSE) {
		  if (args.size()) {
			log << tab << "<query>" << logend;
			for(TArgs::const_iterator it = args.begin(); it != args.end(); it++) {
			  log << tab << tab << "<pair>" << logend;
			  log << tab << tab << tab << "<name>" << logend;
			  log << tab << tab << tab << tab << it->first << logend;
			  log << tab << tab << tab << "</name>" << logend;
			  log << tab << tab << tab << "<value>" << logend;
			  log << tab << tab << tab << tab << curl_escape(it->second.c_str (), 0) << logend;
			  log << tab << tab << tab << "</value>" << logend;
			  log << tab << tab << "</pair>" << logend;
			}
			log << tab << "</query>" << logend;
		  }
		}
		// we close the entry when we send
		//log << "</entry>" << logend;
	  }
	}
}

//--------------------------------------------------------------------------
int HTTPDServer::sendGuidoGetHead (struct MHD_Connection *connection, const char* url, const TArgs& args, int type, vector<string> &elems)
{
    /*
	   there are four possibilities for the URL : 0, 1, 2 or 3 entries
       in the case of 0, we must be making a new score and the request must be POST
       in the case of 1, we are referencing an existing score and the request must be either GET or DELETE
	   in the case of >= 2, we are referencing an existing score with a precise operation and the request must be GET
    */

    if (elems[0] == "version") {
        guidosessionresponse response = guidosession::handleSimpleStringQuery("guidolib", guidosession::getVersion());
        return send(connection, response);
    } else if (elems[0] == "server") {
        guidosessionresponse response = guidosession::handleSimpleStringQuery("guidoserver", guidosession::getServerVersion());
        return send(connection, response);
    } else if (elems[0] == "versions") {
        guidosessionresponse response = guidosession::getGuidoAndServerVersions();
        return send(connection, response);
    } else if (elems[0] == "linespace") {
        guidosessionresponse response = guidosession::handleSimpleFloatQuery("linespace", guidosession::getLineSpace());
        return send(connection, response);
    }

    map<string, guidosession *>::iterator it = fSessions.find(elems[0]);
    if (it == fSessions.end ()) {
        // first try to read from cache...
		if(fUseCache) {
			readFromCache(elems[0]);
			// redo
			it = fSessions.find(elems[0]);
		}
        if (it == fSessions.end ()) {
          guidosessionresponse response = guidosession::genericFailure("incorrect score ID.", 404, elems[0]);
          return send (connection, response);
        }
    }
    guidosession *currentSession = fSessions[elems[0]];

	// Fill the settings for pianoroll or for score.
	GuidoSessionScoreParameters scoreParameters;
	GuidoSessionPianorollParameters pianoRollParameters;

	if (elems.size() >= 2 && elems[1].find("pianoroll") != string::npos) {
		pianoRollParameters = currentSession->getPianoRollParameters(args);
	} else {
		scoreParameters = currentSession->getScoreParameters(args);
	}

	if (elems.size() == 1) {
		// must be getting the score
		currentSession->updateGRH(scoreParameters);
		guidosessionresponse response = currentSession->scoreReturnImage(scoreParameters);
		return send (connection, response);
	}
	if (elems.size() == 2) {
		// the second element will always specify something we need in JSON
		// is there a way to streamline this...a lot of logic dup but not much code dup...
		// maybe templates...but it is sufficiently different to perhaps warrant writing
		// all this stuff out
		if (elems[1] == "countvoices") {
			int nvoices;
			guidoAPIresponse gar = currentSession->countVoices(nvoices);
			guidosessionresponse response = gar.is_happy()
				? currentSession->handleSimpleIDdIntQuery("voicecount", nvoices)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "getpagecount") {
			int npages;
			currentSession->updateGRH(scoreParameters);
			guidoAPIresponse gar = currentSession->getPageCount(npages);
			guidosessionresponse response = gar.is_happy()
				? currentSession->handleSimpleIDdIntQuery("pagecount", npages)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "duration") {
			string duration;
			guidoAPIresponse gar = currentSession->duration(duration);
			guidosessionresponse response = gar.is_happy()
				? currentSession->handleSimpleIDdStringQuery("duration", duration)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "findpageat") {
			// First, get the date from request parameters.
			GuidoDate date;
			string mydate = "";
			if (args.find("date") != args.end()) {
				mydate = args.find("date")->second;
			}
			stringToDate(mydate, date);

			// Update GR with new page format and layout settings.
			currentSession->updateGRH(scoreParameters);
			int page;
			guidoAPIresponse gar = currentSession->findPageAt(date, page);
			guidosessionresponse response = gar.is_happy()
				? currentSession->datePageJson(mydate, page)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "getpagedate") {
			GuidoDate date;
			int mypage = 1;
			if (args.find("page") != args.end()) {
				mypage = atoi(args.find("page")->second.c_str());
			}
			currentSession->updateGRH(scoreParameters);
			guidoAPIresponse gar = currentSession->getPageDate(mypage, date);
			guidosessionresponse response = gar.is_happy()
				? currentSession->datePageJson(dateToString(date), mypage)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "getpagemap") {
			currentSession->updateGRH(scoreParameters);
			JSONTime2GraphicMap outmap;
			guidoAPIresponse gar = currentSession->getMap(PAGE, 0, scoreParameters, outmap);
			guidosessionresponse response = gar.is_happy()
				? currentSession->mapJson("pagemap", outmap)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "ar2midifile") {
			guidosessionresponse response = currentSession->genericReturnMidi();
			return send(connection, response);
		} else if (elems[1] == "getsystemmap") {
			currentSession->updateGRH(scoreParameters);
			JSONTime2GraphicMap outmap;
			guidoAPIresponse gar = currentSession->getMap(SYSTEM, 0, scoreParameters, outmap);
			guidosessionresponse response = gar.is_happy()
				? currentSession->mapJson("systemmap", outmap)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "getstaffmap") {
			currentSession->updateGRH(scoreParameters);
			JSONTime2GraphicMap outmap;
			int mystaff = 1;
			if (args.find("staff") != args.end()) {
				mystaff = atoi(args.find("staff")->second.c_str());
			}
			guidoAPIresponse gar = currentSession->getMap(STAFF, mystaff, scoreParameters, outmap);
			guidosessionresponse response = gar.is_happy()
				? currentSession->mapJson("staffmap", outmap)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "getvoicemap") {
			currentSession->updateGRH(scoreParameters);
			JSONTime2GraphicMap outmap;
			int myvoice = 1;
			if (args.find("voice") != args.end()) {
				myvoice = atoi(args.find("voice")->second.c_str());
			}
			guidoAPIresponse gar = currentSession->getMap(STAFF, myvoice, scoreParameters, outmap);
			guidosessionresponse response = gar.is_happy()
				? currentSession->mapJson("voicemap", outmap)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "gettimemap") {
			currentSession->updateGRH(scoreParameters);
			JSONFriendlyTimeMap outmap;
			guidoAPIresponse gar = currentSession->getTimeMap(outmap);
			guidosessionresponse response = gar.is_happy()
				? currentSession->timeMapJson(outmap)
				: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[1] == "gmn") {
			guidosessionresponse response =  currentSession->handleSimpleIDdStringQuery("gmn", currentSession->getGMN());
			return send(connection, response);
		} else if (elems[1] == "pianoroll") {
			pianoRollParameters.type = kSimplePianoRoll;
			guidosessionresponse response =  currentSession->pianoRollReturnImage(pianoRollParameters);
			return send(connection, response);
		} else if (elems[1] == "trajectorypianoroll") {
			pianoRollParameters.type = kTrajectoryPianoRoll;
			guidosessionresponse response =  currentSession->pianoRollReturnImage(pianoRollParameters);
			return send(connection, response);
		} else {
			guidosessionresponse response = guidosession::genericFailure("Unidentified GET request.", 404, elems[0]);
			return send(connection, response);
		}
	}
	if (elems.size() == 3) {
		if(elems[1] == "pianoroll") {
			pianoRollParameters.type = kSimplePianoRoll;
		} else if(elems[1] == "trajectorypianoroll") {
			pianoRollParameters.type = kTrajectoryPianoRoll;
		} else {
			guidosessionresponse response = guidosession::genericFailure("Unidentified GET request.", 404, elems[0]);
			return send(connection, response);
		}
		if (elems[2] == "getkeyboardwidth") {
			float width;
			guidoAPIresponse gar = currentSession->getPianorollKeyboardWidth(pianoRollParameters, width);
			guidosessionresponse response = gar.is_happy()
					? currentSession->handleSimpleIDdFloatQuery("keyboardwidth", width)
					: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else if (elems[2] == "getmap") {
			JSONTime2GraphicMap outmap;
			guidoAPIresponse gar = currentSession->getPianorollMap(pianoRollParameters, outmap);
			guidosessionresponse response = gar.is_happy()
					? currentSession->mapJson("pianorollmap", outmap)
					: guidosession::genericFailure(gar.errorMsg().c_str(), 400, elems[0]);
			return send(connection, response);
		} else {
			guidosessionresponse response = guidosession::genericFailure("Unidentified GET request.", 404, elems[0]);
			return send(connection, response);
		}
	}
}

//--------------------------------------------------------------------------
int HTTPDServer::answer (struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls)
{
    (void) version;
    if (NULL == *con_cls) {
        struct connection_info_struct *con_info = new connection_info_struct ();
        if (0 == strcmp (method, "POST")) {
            con_info->postprocessor =
                MHD_create_post_processor (connection, 1024, // arbitrary, recommeneded by libmicrohttpd
                                           _post_params, (void *) con_info);

            if (NULL == con_info->postprocessor) {
                delete con_info;
                return MHD_NO;
            }
            /*
              The connectiontype field of con_info currently does nothing:
             it's a placeholder for using POST/GET distinctions as the server
             becomes more sophisticated.

             */
            con_info->connectiontype = POST;
        } else {
            con_info->connectiontype = GET;
        }

        *con_cls = (void *) con_info;

        return MHD_YES;
    }

    TArgs myArgs;
    MHD_get_connection_values (connection, MHD_COOKIE_KIND, _get_params, &myArgs);

	// first, parse the URL
	std::stringstream ss(url);
	std::string item;
	vector<string> elems;
	while (getline(ss, item, '/')) {
		if (item != "") {
			elems.push_back(item);
		}
	}

	logSendGuido(connection, url, myArgs, method);

    if (0 == strcmp (method, "POST")) {
        struct connection_info_struct *con_info = (connection_info_struct *)*con_cls;

        if (*upload_data_size != 0) {
            MHD_post_process (con_info->postprocessor, upload_data,
                              *upload_data_size);
            *upload_data_size = 0;
            return MHD_YES;
		} else {
            struct connection_info_struct *con_info = (connection_info_struct *)*con_cls;
			return sendGuidoPostRequest(connection, con_info->args, elems);
        }
	}
	else if (0 == strcmp (method, "DELETE")) {
		TArgs args;
		MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, _get_params, &args);
		return sendGuidoDeleteRequest(connection, args);
	}
	else if (!elems.size()) {
		guidosessionresponse response = guidosession::welcomeMessage();
		return send (connection, response);
	}
    else if (0 == strcmp (method, "GET")) {
        TArgs args;
		MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, _get_params, &args);
		return sendGuidoGetHead (connection, url, args, GET, elems);
    }
    else if (0 == strcmp (method, "HEAD")) {
        TArgs args;
        MHD_get_connection_values (connection, MHD_GET_ARGUMENT_KIND, _get_params, &args);
		return sendGuidoGetHead (connection, url, args, HEAD, elems);
    }
    else {
      stringstream ss;
      ss << "The GUIDO server does not support the ";
      ss << method;
      ss << " command";
      guidosessionresponse reallyBadResponse = guidosession::genericFailure(ss.str ().c_str (), 400);
      return send (connection, reallyBadResponse);
    }

    // should never get here
    return MHD_NO;
}

} // end namespoace
