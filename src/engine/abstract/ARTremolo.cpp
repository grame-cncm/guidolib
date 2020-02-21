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
#include <string.h>

#include "ARTremolo.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"


ARTremolo::ARTremolo()
{
	init();
}

// -----------------------------------------------------------------------------
ARTremolo::ARTremolo(const ARTremolo * tremolo)	: ARMTParameter(-1, tremolo)
{
	init();
	copyParameters (tremolo->getTagParameters());
	setTagParameters (tremolo->getTagParameters());
}

// -----------------------------------------------------------------------------
void ARTremolo::init()
{
	setupTagParameters (gMaps->sARTremoloMap);
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
}

// -----------------------------------------------------------------------------
bool ARTremolo::MatchEndTag(const char * s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;
	if (!getRange() && !strcmp("\\tremoloEnd",s))
		return 1;
	return 0;
}

// -----------------------------------------------------------------------------
void ARTremolo::setTagParameters(const TagParameterMap& map)
{
	fPitch 		= getParameter<TagParameterString>(kPitchStr, true)->getValue();
	fText		= getParameter<TagParameterString>(kTextStr, true)->getValue();
	fStyle		= getParameter<TagParameterString>(kStyleStr, true)->getValue();
	fSpeed		= getParameter<TagParameterInt>(kSpeedStr, true)->getValue();
	fThickness	= getParameter<TagParameterFloat>(kThicknesStr, true)->getValue();
}

// -----------------------------------------------------------------------------
bool ARTremolo::isSecondPitchCorrect() const 
{
    bool isReadable = false;

    if (!fPitch.empty())
    {
        unsigned int it = 0;
        char c = fPitch[it];
        
        if(c == '{')
        {
            if(fPitch[fPitch.size()-1] != '}')
                return isReadable;
            std::string possibleLastChars = "1234567890abcdefghoils#&";
            char l = fPitch[fPitch.size()-2];
            if(possibleLastChars.find(l) == std::string::npos)
                return isReadable;
            it++;
        }
        if(it < fPitch.size())
        {
            c = fPitch[it];
            std::string possibleChars = "abcdefgh";
            if(possibleChars.find(c) != std::string::npos)
                isReadable = true;
            else
            {
                char c2 = fPitch[it+1];
                if((c == 'r' && c2 == 'e') || ((c == 'm' || c == 't') && c2 == 'i') || (c == 'l' && c2 == 'a') || (c == 's' && (c2 == 'i' || (c2 == 'o' && fPitch[it+2] == 'l'))))
                    isReadable = true;
            }
        }
    }
    return isReadable;
}

// -----------------------------------------------------------------------------
int ARTremolo::getNumberOfStrokes() const
{
    int n = 3;

	if (!fStyle.empty()) {
        if( fStyle == "/" )
            n = 1;
        else if( fStyle == "//" )
            n = 2;
        else if (fStyle == "///")
            n = 3;
        else if( fStyle == "////")
            n = 4;
    }
    return n;
}

