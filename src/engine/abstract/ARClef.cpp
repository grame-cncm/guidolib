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

#include <iostream>

#include "ARClef.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"

using namespace std;


std::map<std::string, std::pair<ARClef::cleftype,int> > ARClef::fClefsMap;

ARClef::ARClef( const TYPE_TIMEPOSITION & timeposition) : ARMTParameter(timeposition)	{}
ARClef::ARClef(const string& theName)  : ARMTParameter(), fName (theName)				{}

ARClef::ARClef()
{
	setupTagParameters (gMaps->sARClefMap);
	if (fClefsMap.empty()) buildMap();

	fClef = VIOLIN;
	fStaffLine = 2;
	fOctava = NONE;
}

ARClef::ARClef(const ARClef & clef)
{
	setupTagParameters (gMaps->sARClefMap);
	if (fClefsMap.empty()) buildMap();

	this->fClef = clef.fClef;
	fStaffLine = clef.fStaffLine;
	fOctava = clef.fOctava;
}

void ARClef::buildMap ()
{
	fClefsMap["bass"]	= make_pair(BASS, 4);
	fClefsMap["basso"]	= make_pair(BASS, 4);
	fClefsMap["f"]		= make_pair(BASS, 4);
	fClefsMap["f5"]		= make_pair(BASS, 5);
	fClefsMap["f4"]		= make_pair(BASS, 4);
	fClefsMap["f3"]		= make_pair(BASS, 3);
	fClefsMap["f2"]		= make_pair(BASS, 2);
	fClefsMap["f1"]		= make_pair(BASS, 1);
	fClefsMap["tenor"]	= make_pair(BRATSCHE, 4);
	fClefsMap["c"]		= make_pair(BRATSCHE, 3);
	fClefsMap["c5"]		= make_pair(BRATSCHE, 5);
	fClefsMap["c4"]		= make_pair(BRATSCHE, 4);
	fClefsMap["c3"]		= make_pair(BRATSCHE, 3);
	fClefsMap["c2"]		= make_pair(BRATSCHE, 2);
	fClefsMap["c1"]		= make_pair(BRATSCHE, 1);
	fClefsMap["c0"]		= make_pair(BRATSCHE, 0);
	fClefsMap["alto"]	= make_pair(BRATSCHE, 3);
	fClefsMap["violin"]	= make_pair(VIOLIN, 2);
	fClefsMap["treble"]	= make_pair(VIOLIN, 2);
	fClefsMap["g"]		= make_pair(VIOLIN, 2);
	fClefsMap["g5"]		= make_pair(VIOLIN, 5);
	fClefsMap["g4"]		= make_pair(VIOLIN, 4);
	fClefsMap["g3"]		= make_pair(VIOLIN, 3);
	fClefsMap["g2"]		= make_pair(VIOLIN, 2);
	fClefsMap["g1"]		= make_pair(VIOLIN, 1);
	fClefsMap["gg"]		= make_pair(DOUBLEG, 2);
	fClefsMap["gg5"]	= make_pair(DOUBLEG, 5);
	fClefsMap["gg4"]	= make_pair(DOUBLEG, 4);
	fClefsMap["gg3"]	= make_pair(DOUBLEG, 3);
	fClefsMap["gg2"]	= make_pair(DOUBLEG, 2);
	fClefsMap["gg1"]	= make_pair(DOUBLEG, 1);
	fClefsMap["perc"]	= make_pair(PERC, 3);
	fClefsMap["perc5"]	= make_pair(PERC, 5);
	fClefsMap["perc4"]	= make_pair(PERC, 4);
	fClefsMap["perc3"]	= make_pair(PERC, 3);
	fClefsMap["perc2"]	= make_pair(PERC, 2);
	fClefsMap["perc1"]	= make_pair(PERC, 1);
	fClefsMap["TAB"]	= make_pair(TAB, 2);
	fClefsMap["none"]	= make_pair(OFF, 2);
	fClefsMap["off"]	= make_pair(OFF, 2);
	fClefsMap[""]		= make_pair(OFF, 2);
}

void ARClef::setTagParameters (const TagParameterMap& params)
{
	const TagParameterString * tps = getParameter<TagParameterString>(kTypeStr, true);
	if (tps) setName(tps->getValue());
}

std::string ARClef::decodeOctava (const std::string& name)
{
	fOctava = NONE;
	size_t pos = name.find("-");
	if (pos == std::string::npos)
	 	pos = name.find("+");

  	if (pos != string::npos) {
		switch (atoi(&name[pos])) {
			case -15:	fOctava = LOW15; break;
			case -8:	fOctava = LOW8; break;
			case 0:		fOctava = NONE; break;
			case 8:		fOctava = HIGH8; break;
			case 15:	fOctava = HIGH15; break;
		}
		return name.substr(0, pos);
	}
	return name;
}

void ARClef::setName(const string& name)
{
	fName = name;
	string clef = decodeOctava (name);
	const pair<cleftype,int>& v = fClefsMap[clef];
	if (v.first != UNDEFINED) {
		fClef = v.first;
		fStaffLine = v.second;
	}
	else {
		fClef = OFF;
		fStaffLine = 2;
	}
}

bool ARClef::operator==(const ARClef &clef) const
{
	return (fStaffLine == clef.fStaffLine) && (fClef == clef.fClef) && (fOctava == clef.fOctava);
}

