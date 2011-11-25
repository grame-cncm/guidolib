/*
	GUIDO Library
	Copyright (C) 2004	Grame

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

#include "FormatStringParser.h"
using namespace std;

void FormatStringParser::parse( const char * str, FormatStringParserResult& res)
{
	switchTo (kLitteral);

	const char start = getSpecialStart();	// "["
	const char end = getSpecialEnd();		// "]"
	const char esc = getEscapeChar();		// "\\"
	bool escaped = false;

	while (char c = *str++) {	
		if (escaped) {
			mCurrString += c;		
			escaped = false;	
		}
		else if (c == esc) {
			escaped = true;
		}
		else if (c == start) {
			store ( mCurrString, mCurrType, res); 
			switchTo (kSpecial);
		}
		else if (c == end) {
			store ( mCurrString, mCurrType, res);
			switchTo (kLitteral);
		}
		else mCurrString += c;		
	}	
	store ( mCurrString, mCurrType, res);
}

void FormatStringParser::switchTo (type t)
{
	mCurrString = ""; // clear current string
	mCurrType = t;
}

void FormatStringParser::store (string str, type t, FormatStringParserResult& res) const 
{
	if (!str.empty()) {
		FormatStringAssoc p (str, t);
		res.push_back(p);
	}
}
