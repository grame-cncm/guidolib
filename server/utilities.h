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


#ifndef __utilities__
#define __utilities__

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

#include "GUIDOEngine.h"

#define POSTBUFFERSIZE 512

#define GET 0
#define POST 1

namespace guidohttpd
{

typedef std::pair<std::string, std::string> TArg;
typedef std::map<std::string, std::string> TArgs;
    //typedef std::vector<TArg> TArgs;
    
bool atob(std::string name);
void stringToDate(std::string, GuidoDate &date);
std::string dateToString(GuidoDate &date);
float dateToFloat(const GuidoDate &date);


std::string rand_alnum_str (std::string::size_type sz);
long lopt(char *argv[], const char *name, long def);
bool bopt(char *argv[], const char *name, bool def);
std::string sopt(char *argv[], const char *name, std::string def);

struct _logend {};

class logstream
{
    bool		fDaemon;
    bool		fPrintDate;
    std::fstream fFileStream;

    inline std::ostream& stream() {
        return fDaemon ? fFileStream : std::cout;
    }
    std::string date();
    void		printdate(std::ostream& out);

public:
    logstream () : fDaemon (false), fPrintDate(false) {}
    logstream (const char * logfile);
    virtual ~logstream () {
        fFileStream.close();
    }


    template <typename T>
    void print (const T& val) {
        std::ostream& out = this->stream();
        printdate(out);
        out << val;
    }

    void print (struct sockaddr * addr) {
        std::ostream& out = this->stream();
        printdate(out);
        char buff[INET6_ADDRSTRLEN];
        switch(addr->sa_family) {
        case AF_INET:
            stream() << inet_ntop(AF_INET, &(((struct sockaddr_in *)addr)->sin_addr), buff, INET6_ADDRSTRLEN);
            break;
        case AF_INET6:
            out << inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)addr)->sin6_addr), buff, INET6_ADDRSTRLEN);
            break;
        default:
            out << "Unexpectes address family";
        }
    }

    void print (_logend) {
        stream() << std::endl;
        fPrintDate = fDaemon;
    }


};
extern logstream *	gLog;
extern _logend		gLogEndl;

template <typename T>
logstream& operator <<(logstream& out, const T& val)
{
    out.print(val);
    return out;
}

#define log	(*gLog)
#define logend	(gLogEndl)

//    void write_to_log(std::string message, std::string logfile, bool daemon);

} // end namespoace

#endif
