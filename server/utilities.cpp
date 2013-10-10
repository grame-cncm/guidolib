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


#include <cstdlib>   // for rand()
#include <cctype>    // for isalnum()
#include <algorithm> // for back_inserter
#include <string>
#include <iostream>
#include <time.h>
#include <string.h>
#include <sstream>

#include "utilities.h"

using namespace std;

namespace guidohttpd
{

logstream * gLog;
_logend		gLogEndl;

char
rand_alnum()
{
    char c;
    while (!std::isalnum(c = static_cast<char>(std::rand())))
        ;
    return c;
}

bool atob(string name)
{
    if (name == "true")
        return true;
    if (name == "True")
        return true;
    if (name == "yes")
        return true;
    
    return false;
}

int atoib(string name)
{
    if (name == "true")
        return 1;
    if (name == "True")
        return 1;
    if (name == "yes")
        return 1;
    
    return 0;
}

int systemsDistributionToFloat(std::string sd)
{
  if (sd == "auto")
    return kAutoDistrib;
  if (sd == "always")
    return kAlwaysDistrib;
  if (sd == "never")
    return kNeverDistrib;
  return kAutoDistrib;
}

void stringToDate(std::string raw, GuidoDate &date)
{
    // get rid of the encapsulating quotation marks
    std::string str = raw;
    str.erase(str.end() - 1, str.end());
    str.erase(str.begin(), str.begin()+1);

    // first, parse the fraction
    std::stringstream ss(str);
    std::string item;
    vector<string> elems;
    while (getline(ss, item, '/')) {
        if (item != "") {
            elems.push_back(item);
        }
    }
    if (!(elems.size() == 2)) {
        date.num = 0;
        date.denom = 1;
    } else {
        date.num = atoi(elems[0].c_str());
        date.denom = atoi(elems[1].c_str());
        if (!date.denom)
            date.denom = 1;
    }
}
string dateToString(GuidoDate &date)
{
    // first, parse the fraction
    std::stringstream ss;
    ss << "\"";
    ss << date.num;
    ss << "/";
    ss << date.denom;
    ss << "\"";
    return ss.str();
}
    
float dateToFloat(const GuidoDate &date) {
  if ((date.num == 0) && (date.denom == 0))
    return 0.f;
  if (date.denom == 0)
    return 1000000.f;
  return (date.num * 1.f) / (date.denom * 1.f);
}

std::string
rand_alnum_str (std::string::size_type sz)
{
    std::string s;
    s.reserve  (sz);
    generate_n (std::back_inserter(s), sz, rand_alnum);
    return s;
}

long lopt(char *argv[], const char *name, long def)
{
    int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) {
            return atoi(argv[i+1]);
        }
    return def;
}

bool bopt(char *argv[], const char *name, bool def)
{
    int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) {
            return true;
        }
    return def;
}

std::string sopt(char *argv[], const char *name, std::string def)
{
    int	i;
    for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) {
            return std::string (argv[i+1]);
        }
    return def;
}

//----------------------------------------------------------------------------------------
logstream::logstream (const char * logfile) : fDaemon (true), fPrintDate(true)
{
    fFileStream.open(logfile, ios_base::out | ios_base::app);
    if (!fFileStream.is_open()) {
        cerr << "can't open log file " << logfile << endl;
    }
}

string logstream::date()
{
    time_t rawtime;
    time ( &rawtime );
    string datestr (ctime ( &rawtime ));
    return datestr.substr(0, datestr.size()-1);
}

void logstream::printdate(std::ostream& out)
{
    if (fPrintDate) {
        out << "[" << date() << "] ";
        fPrintDate = false;
    }
}


} // end namespoace
