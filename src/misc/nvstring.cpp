/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003	Grame

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

	Note: the buffer pointer is always valid (it will always point to the static
			buffer, instead of being null)

*/

#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "nvstring.h"

using namespace std;

// ---------------------------------------------------------------------------
//		* NVstring
// ---------------------------------------------------------------------------
NVstring::NVstring() {}
NVstring::NVstring(const char * p)		: string (p ? p : "")	{}
NVstring::NVstring(const string& str)	: string (str)	{}
NVstring::NVstring(const NVstring & in) : string (in) {}

// ---------------------------------------------------------------------------
//		* ~NVstring
// ---------------------------------------------------------------------------
NVstring::~NVstring()	{}

// ---------------------------------------------------------------------------
//		* to_lower
// ---------------------------------------------------------------------------
void NVstring::to_lower()
{
	this->string::operator = (to_lower(c_str()));
}

std::string NVstring::to_lower(const char* str)
{
	string lower(str);
	for( unsigned int i = 0; i < lower.size(); i++ )
		lower[i] = tolower(*str++);
	return lower;
}

// ---------------------------------------------------------------------------
//		* replace
// ---------------------------------------------------------------------------
/** \brief Replaces char 1 by char 2 in the string.

	return the number of replaced characters.
*/
size_t NVstring::replace( char c1, char c2 )
{
	size_t count = 0;	
	string replaced;
	const char *ptr = c_str();
	for( unsigned int i = 0; i < size(); i++, ptr++ ) {
		if (*ptr == c1) {
			replaced += c2;
			count++;
		}
		else replaced += *ptr;
	}
	this->string::operator = (replaced);
	return count;
}


