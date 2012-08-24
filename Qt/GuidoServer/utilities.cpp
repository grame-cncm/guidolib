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

#include "utilities.h"

namespace guidohttpd
{

char
rand_alnum()
 {
     char c;
     while (!std::isalnum(c = static_cast<char>(std::rand())))
         ;
     return c;
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
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return atoi(argv[i+1]);
	return def;
}

bool bopt(char *argv[], const char *name)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return true;
	return false;
}

std::string sopt(char *argv[], const char *name, std::string def)
{
	int	i;
	for (i = 0; argv[i]; i++) if (!strcmp(argv[i], name)) return std::string (argv[i+1]);
	return def;
}

void write_to_log(std::string message, std::string logfile, bool daemon)
{
  if (daemon) {
    FILE *f = fopen (logfile.c_str(), "ab");
    fwrite(message.c_str (), message.size (), sizeof (char), f);
    fclose(f);
  } else {
      std::cout << message << std::endl; // precautionary endl
  }
}

} // end namespoace
