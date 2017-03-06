/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

//#include <iostream>
#include <cstring>
#include <cassert>

#include "secureio.h"
#include "TagParameterString.h"
#include "HtmlColors.h"

TagParameterString::TagParameterString(const char * theString)			: fValue(theString)	{}
TagParameterString::TagParameterString(const std::string& theString)	: fValue(theString) {}
TagParameterString::TagParameterString( const TagParameterString & tps ) 
	: TagParameter( tps ), 	fValue(tps.getValue())
{
}
TagParameterString::~TagParameterString()	{}

void TagParameterString::set( const TagParameterString & in )
{
	TagParameter::set( in );
	fValue = in.getValue();
}

bool TagParameterString::operator == (const char * inCString ) const
{
	return inCString ? (fValue == inCString) : false;
}

bool TagParameterString::copyValue(const TagParameter * tp )
{
	const TagParameterString * tps = TagParameterString::cast(tp);
	if (tps == 0)	return false;	
	fValue = tps->getValue();
	return true;
}

/** \brief convert hex-numbers to "real values"
	tries to convert the string into a color-REG for RGB
	
	returns 1 if successful, 0 if no conversion possible
*/

// (JB) Changed the short[3] rgb table to unsigned char [4] rgba
// where alpha is the transparency of the element do be drawn.
// alpha = 0: opaque
// (YC) corrected to match platforms defs: alpha=255 means opaque
bool TagParameterString::getRGB( unsigned char colref[4] ) const
{
	std::string value = NVstring::to_lower(fValue.c_str());

	// - Start from Black
	colref[0] = 0;
	colref[1] = 0;
	colref[2] = 0;
	colref[3] = 255;	//YC corrected here

	bool hexformat = value.substr(0,2) == "0x";
	size_t size = value.size();
	// - Check for 0xRRGGBB	
	if ((size == 8) && hexformat)
	{
		unsigned int r, g, b;
		sscanf(value.c_str(), "0x%2x%2x%2x", &r, &g, &b);
		colref[0] = (unsigned char)r;
		colref[1] = (unsigned char)g;
		colref[2] = (unsigned char)b;
		return true;
	}

	// (JB) Proposal: alpha component for transparency: 0xRRGGBBAA
	if ((size == 10) && hexformat)
	{
		unsigned int r, g, b, a;
		sscanf(value.c_str(), "0x%2x%2x%2x%2x", &r, &g, &b, &a);
		colref[0] = (unsigned char)r;
		colref[1] = (unsigned char)g;
		colref[2] = (unsigned char)b;
		colref[3] = (unsigned char)a;
		return true;
	}
	return HtmlColor::get (value.c_str(), colref);
}

bool TagParameterString::getBool() const
{
	std::string value = NVstring::to_lower(fValue.c_str());
	return (value == "true") || (value == "yes") || (value == "on") || (value == "1");
}
