/* 
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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


