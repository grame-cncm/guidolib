/*

  Copyright (C) 2012  Grame


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

    Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
    research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <string>

#include "markers.h"
#include "guidotags.h"

using namespace std;

namespace guido 
{

static const char* kOpenedStr		= "opened";
static const char* kOpenedBegStr	= "begin";
static const char* kOpenedEndStr	= "end";
static const char* kOpenedBegEndStr	= "begin-end";
static const char* kOpenedClosedStr	= "closed";

//________________________________________________________________________
int markers::opened ( Sguidotag& elt )
{
	Sguidoattribute attr = elt->getAttribute (kOpenedStr);
	if (attr) {
		const string& value = attr->getValue();
		if (value == kOpenedBegStr)		return kOpenedBegin;
		if (value == kOpenedEndStr)		return kOpenedEnd;
		if (value == kOpenedBegEndStr)	return kOpenedBegin + kOpenedEnd;
		if (value == kOpenedClosedStr)	return kClosed;
	}
	return kNoMark;
}

//________________________________________________________________________
void markers::delMark ( Sguidotag& elt )
{
	elt->delAttribute (kOpenedStr);
}

//________________________________________________________________________
void markers::setMark ( Sguidoelement& elt, int type )
{
	Sguidotag tag = dynamic_cast<guidotag*>((guidoelement*)elt);
	if (tag) setMark (tag, type);
}

//________________________________________________________________________
void markers::setMark ( Sguidotag& elt, int type )
{
	Sguidoattribute attr = elt->getAttribute (kOpenedStr);
	if (attr) {
		switch (type) {
			case kOpenedBegin:
				attr->setValue(kOpenedBegStr, true);
				break;
			case kOpenedEnd:
				attr->setValue(kOpenedEndStr, true);
				break;
			case kOpenedBegin + kOpenedEnd:
				attr->setValue(kOpenedBegEndStr, true);
				break;
			default:
				attr->setValue(kOpenedClosedStr, true);
				break;
		}
	}
}

//________________________________________________________________________
void markers::markOpened ( Sguidotag& elt, bool end )
{
	// filters tags where it doesn't make sense
	if (elt->getType() == kTAccent) return;	
	if (elt->getType() == kTLyrics) return;	
	if (elt->getType() == kTText) return;	
	if (elt->getType() == kTFingering) return;	
	if (elt->getType() == kTFermata) return;
	if (elt->getType() == kTPizz) return;
	if (elt->getType() == kTHarmonic) return;

	if (elt->getType() == kTTurn) return;
	if (elt->getType() == kTTrill) return;
	if (elt->getType() == kTTrem) return;
	if (elt->getType() == kTStacc) return;
	if (elt->getType() == kTMord) return;
	if (elt->getType() == kTMarcato) return;
	
	Sguidoattribute attr = elt->getAttribute (kOpenedStr);
	if (attr) {
		if (end && (attr->getValue() == kOpenedBegStr))
			attr->setValue(kOpenedBegEndStr, true);
		if (!end && (attr->getValue() == kOpenedEndStr))
			attr->setValue(kOpenedBegEndStr, true);
	}
	else {
		Sguidoattribute attr = guidoattribute::create();
		attr->setName (kOpenedStr);
		attr->setValue( end ? kOpenedEndStr : kOpenedBegStr, true);
		elt->add (attr);
	}
}

}
