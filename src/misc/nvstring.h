#ifndef NVstring_H
#define NVstring_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003,2004	Grame

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
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "stdlib.h"
#include <string>

class NVstring : public std::string
{
	public:
						 NVstring();
						 NVstring(const char * p);
						 NVstring(const std::string& str);
						 NVstring(const NVstring & in);
		virtual 		~NVstring();

			void 			to_lower();
			size_t 			replace( char c1, char c2 );
	static	std::string		to_lower(const char* str);
};

#endif
