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
#include <sstream>

#include "ARFeatheredBeam.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"

using namespace std;

static const TagParameterMap sARFeatheredBeamMap (kARFeatheredBeamParams);

ARFeatheredBeam::ARFeatheredBeam()
{
	setupTagParameters (sARFeatheredBeamMap);

	rangesetting = ONLY;
	fFeathered = true;
	fDrawDur = false;
	fDurationsSet = false;
	
	fBeginDur = 0;
	fEndDur = 0;
	fBeginTimePos = 0;
	fEndTimePos = 0;
}

bool ARFeatheredBeam::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\fBeamEnd",s))
		return true;
	return false;
}

//--------------------------------------------------------------------------------------
void ARFeatheredBeam::setTagParameters(const TagParameterMap & params)
{
	ARBeam::setTagParameters (params);

	const TagParameterString* durations = getParameter<TagParameterString>(kDurationsStr, true);
	findPoints(durations->getValue());

	const TagParameterString* drawDuration = getParameter<TagParameterString>(kDrawDurationStr, true);
	fDrawDur = drawDuration->getBool();
}

void ARFeatheredBeam::findPoints(std::string durations)
{
	std::size_t begin = 0;
	std::size_t commaPos = durations.find(",", begin);
	
	if(commaPos != std::string::npos)
	{
		float valor = 0;
		float valor2 = 0;

		fDurationsSet = true;

		std::string dur = durations.substr(begin, commaPos);
		std::stringstream stream(dur);
		stream >> valor;
		std::size_t slashPos = dur.find("/",begin);
		if(slashPos != std::string::npos)
		{
			std::string denominator = dur.substr(slashPos+1,(commaPos-slashPos));
			std::stringstream stream(denominator);
			float den;
			stream >> den;
			valor /= den;
		}

		dur = durations.substr(commaPos+1);
		std::stringstream stream2(dur);
		stream2 >> valor2;
		slashPos = dur.find("/",begin);
		if(slashPos != std::string::npos)
		{
			std::string denominator = dur.substr(slashPos+1);
			std::stringstream stream2(denominator);
			float den;
			stream2 >> den;
			valor2 /= den;
		}
		convertDurationToBeams(valor, valor2);
	}
}

void ARFeatheredBeam::findDefaultPoints()
{
	//handle the case with no duration parameters
	float valor = float(fBeginDur.getNumerator());
	valor /= fBeginDur.getDenominator();
	float valor2 = float(fEndDur.getNumerator());
	valor2 /= fEndDur.getDenominator();
	convertDurationToBeams(valor, valor2);
}

void ARFeatheredBeam::convertDurationToBeams(float valor, float valor2) 
{
		
	if(valor >= 0.09375) 
		fBeams.first=1;
	else if(valor >= 0.046875)
		fBeams.first=2;
	else if(valor >= 0.0234375)
		fBeams.first=3;
	else
		fBeams.first=4;

	if(valor2>=0.09375) 
		fBeams.second=1;
	else if(valor2>=0.046875)
		fBeams.second=2;
	else if(valor2>=0.0234375)
		fBeams.second=3;
	else
		fBeams.second=4;
}

