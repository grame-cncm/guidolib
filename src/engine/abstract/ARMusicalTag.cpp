/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <sstream>
#include <iostream>

#include "secureio.h"
#include "kf_ivect.h"

#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterRGBColor.h"
#include "TagParameterStrings.h"

#include "ARMusicalTag.h"

using namespace std;

TagParameterMap ARMusicalTag::fCommonParams;

//static const TagParameterMap sARMusicalTagMap (kCommonParams);

ARMusicalTag::ARMusicalTag(const TYPE_TIMEPOSITION & tp, const ARMusicalTag * copy)
  	: ARMusicalObject(tp), id(-1), isAuto(0), rangesetting(NO), error(0), mHasRange(false), mIsInHeader(false)
{
	init(copy);
};

ARMusicalTag::ARMusicalTag(int pid, const ARMusicalTag * copy) 
  : id(pid), isAuto(0), rangesetting(NO), error(0), mHasRange(false), mIsInHeader(false)
{
	init(copy);
}

void ARMusicalTag::init (const ARMusicalTag * copy)
{
	assoc = DC;
	if (!fCommonParams.size()) {
		TagParameterMap commonMap (kCommonParams);
		fCommonParams.clear();
		fCommonParams.Add (commonMap);
	}
	setupTagParameters (fCommonParams);
	if (copy) {
		copyParameters(copy->getTagParameters());
		setTagParameters(copy->getTagParameters());
		rangesetting = copy->rangesetting;
		mHasRange = copy->mHasRange;
	}
}


// (JB) rewrited with std::ostringstream class instead of deprecated ostrstream.
std::ostream & operator << ( std::ostream & os, const ARMusicalTag* tag )
{
	tag->print (os);
	return os;
}

const	TagParameterRGBColor* ARMusicalTag::getRGBColor() const			{ return fRgbColor.TagIsSet() ? &fRgbColor : 0; }
const	TagParameterString * ARMusicalTag::getColor() const	{ return getParameter<TagParameterString> (kColorStr); }
const	TagParameterFloat * ARMusicalTag::getDX() const		{ return getParameter<TagParameterFloat> (kDxStr, true); }
const	TagParameterFloat * ARMusicalTag::getDY() const		{ return getParameter<TagParameterFloat> (kDyStr, true); }
const	TagParameterFloat * ARMusicalTag::getSize() const	{ return getParameter<TagParameterFloat> (kSizeStr); }


//--------------------------------------------------------------------------------------------------------
void ARMusicalTag::checkUnitParameters	(TagParameterMap& params)
{
	params.checkUnit (fParamsTemplate);
}

//--------------------------------------------------------------------------------------------------------
TagParameterMap ARMusicalTag::checkTagParameters (TagParametersList& params, const std::string pTemplate)
{
	TagParameterMap namedParams;
	vector<string> keys = TagParameterMap::getKeys (pTemplate);
	size_t n = params.size();
	size_t kn = keys.size();
	for (size_t i=0; i < n ; i++) {
		STagParameterPtr p = params[i];
		string key = p->getName();
		if (key.size())
			namedParams.Add (p);
		else if (i < kn) {
			p->setName (keys[i]);
			namedParams.Add (p);
		}
		else {
			cerr << getGMNName() << ": " << "no match in parameters string for unnamed parameter" << endl;
			break;
		}
	}
	return namedParams;
}

//--------------------------------------------------------------------------------------------------------
void  ARMusicalTag::clearTagDefaultParameter (const char* param)
{
	fParamsTemplate.Remove(param);
}

//--------------------------------------------------------------------------------------------------------
void  ARMusicalTag::setTagParameters (TagParametersList& params)
{
	TagParameterMap map = checkTagParameters (params, getParamsStr());
	checkUnitParameters (map);
	map.Match(fParamsTemplate, getGMNName().c_str());
	fParams = map;
	setTagParameters (map);
}


void ARMusicalTag::setRGBColor (unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	fRgbColor.setValue (red, green, blue, alpha);
	fRgbColor.setBySet();
    //if (rgbColor) delete rgbColor;
//    rgbColor = new TagParameterRGBColor(red, green, blue);
}

void ARMusicalTag::setColor(const char * cp)
{
	STagParameterPtr color = STagParameterPtr(new TagParameterString(cp));
	color->setName(kColorStr);
	addTagParameter(color);
}

void ARMusicalTag::setSize(float newsize)
{
	STagParameterPtr size = STagParameterPtr(new TagParameterFloat(newsize));
	size->setName(kSizeStr);
	addTagParameter(size);
}

void ARMusicalTag::setAllowRange(int pallow)
{
	if (pallow == 0)		rangesetting = NO;
	else if (pallow == 1)	rangesetting = RANGEDC;
	else if (pallow == -1)	rangesetting = ONLY;
}

bool ARMusicalTag::MatchEndTag(const char * endstr)
{
	if (getRange() && !strcmp(")",endstr))
		return true;
	return false;
}

void ARMusicalTag::print(std::ostream & os) const
{
    os << getGMNName() << getTagParameters();
}

//void ARMusicalTag::printAttributes(std::ostream & os) const
//{
//    os << (getRange() || rangesetting == ONLY ? " (ranged)" : "") << (getIsAuto() ? " (auto)" : "");
//}
//
//void ARMusicalTag::printParameters(std::ostream & os) const
//{
//	getTagParameters().print (os);	
//}
