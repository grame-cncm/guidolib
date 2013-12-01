/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <cstring>
#include <iostream>
#include <sstream>

#include "ARFeatheredBeam.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARFeatheredBeam::ltpls(1);

ARFeatheredBeam::ARFeatheredBeam()
{
	rangesetting = ONLY;
	feathered = true;
	drawDur = false;
	durationsSet = false;
	
	beginDur = 0;
	endDur = 0;
	beginTimePos = 0;
	endTimePos = 0;
}

ARFeatheredBeam::~ARFeatheredBeam()
{
}

bool ARFeatheredBeam::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;
	if (!getRange() && !strcmp("\\fBeamEnd",s))
		return true;
	return false;
}

void ARFeatheredBeam::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,durations,,o;S,drawDuration,false,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * durations = TagParameterString::cast(rtpl->RemoveHead());
			std::string p = durations->getValue();
			findPoints(p);

			TagParameterString * drawDuration = TagParameterString::cast(rtpl->RemoveHead());
			std::string draw = drawDuration->getValue();
			if(draw == "true")
				drawDur = true;
		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARFeatheredBeam::PrintName(std::ostream & os) const
{
	os << "\\fBeam";
	if (!getRange())
		os << "Begin";
}

void ARFeatheredBeam::findPoints(std::string durations)
{
	std::size_t begin = 0;
	std::size_t commaPos = durations.find(",", begin);
	
	if(commaPos != -1)
	{
		float valor = 0;
		float valor2 = 0;

		durationsSet = true;

		std::string dur = durations.substr(begin, commaPos);
		std::stringstream stream(dur);
		stream >> valor;
		std::size_t slashPos = dur.find("/",begin);
		if(slashPos != -1)
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
		if(slashPos != -1)
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
	float valor = float(beginDur.getNumerator());
	valor /= beginDur.getDenominator();
	float valor2 = float(endDur.getNumerator());
	valor2 /= endDur.getDenominator();
	convertDurationToBeams(valor, valor2);
}

void ARFeatheredBeam::convertDurationToBeams(float valor, float valor2)
{
		
	if(valor >= 0.09375) 
		beams.first=1;
	else if(valor >= 0.046875)
		beams.first=2;
	else if(valor >= 0.0234375)
		beams.first=3;
	else
		beams.first=4;

	if(valor2>=0.09375) 
		beams.second=1;
	else if(valor2>=0.046875)
		beams.second=2;
	else if(valor2>=0.0234375)
		beams.second=3;
	else
		beams.second=4;
}