/*
  GUIDO Library
  Copyright (C) 2004	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "FormatStringParser.h"
using namespace std;

void FormatStringParser::parse( const char * str, FormatStringParserResult& res)
{
	switchTo (kLitteral);

	const char start = getSpecialStart();	// "["
	const char end   = getSpecialEnd();		// "]"
	const char esc   = getEscapeChar();		// "\\"
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
