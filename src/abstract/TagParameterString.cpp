/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
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

#include <cstring>
#include <cassert>

#include "secureio.h"
#include "TagParameterString.h"

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

// (JB) Changed the short[3] rgb table to unigned char [4] rgba
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
	int size = value.size();
	// - Check for 0xRRGGBB	
	if (size == 8 && hexformat)
	{
		unsigned int r, g, b;
		sscanf(value.c_str(), "0x%2x%2x%2x", &r, &g, &b);
		colref[0] = (unsigned char)r;
		colref[1] = (unsigned char)g;
		colref[2] = (unsigned char)b;
	}
	// (JB) Proposal: alpha component for transparency: 0xRRGGBBAA
	else if(size == 10 && hexformat)
	{
		unsigned int r, g, b, a;
		sscanf(value.c_str(), "0x%2x%2x%2x%2x", &r, &g, &b, &a);
		colref[0] = (unsigned char)r;
		colref[1] = (unsigned char)g;
		colref[2] = (unsigned char)b;
		colref[3] = (unsigned char)a;
	}
	else if (value == "red")
		colref[0] = 200;

	else if (value == "green")
		colref[1] = 200;

	else if (value == "blue")
		colref[2] = 200;

	else if (value == "black")
		;

	else if (value == "white")
		colref[0] = colref[1] = colref[2] = 255;

	else if (value == "yellow")
		colref[0] = colref[1] = 200;
	else return false;

	return true;
}
