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

const TagParameterString *	ARTremolo::getStyle() const { return getParameter<TagParameterString>(kStyleStr, true); }

const TagParameterInt *		ARTremolo::getSpeed() const { return getParameter<TagParameterInt>(kSpeedStr, true); }
const TagParameterString *	ARTremolo::getPitch() const { return getParameter<TagParameterString>(kPitchStr, true); }
const TagParameterFloat *	ARTremolo::getThickness() const { return getParameter<TagParameterFloat>(kThicknesStr, true); }
const TagParameterString *	ARTremolo::getText() const { return getParameter<TagParameterString>(kTextStr, true); }

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
bool ARTremolo::isSecondPitchCorrect() const 
{
    bool isReadable = false;

	const TagParameterString * p = getPitch();
    if(p && p->TagIsSet())
    {
        std::string pitch = p->getValue();
        unsigned int it = 0;
        char c = pitch[it];
        
        if(c == '{')
        {
            if(pitch[pitch.size()-1] != '}')
                return isReadable;
            std::string possibleLastChars = "1234567890abcdefghoils#&";
            char l = pitch[pitch.size()-2];
            if(possibleLastChars.find(l) == std::string::npos)
                return isReadable;
            it++;
        }
        if(it < pitch.size())
        {
            c = pitch[it];
            std::string possibleChars = "abcdefgh";
            if(possibleChars.find(c) != std::string::npos)
                isReadable = true;
            else
            {
                char c2 = pitch[it+1];
                if((c == 'r' && c2 == 'e') || ((c == 'm' || c == 't') && c2 == 'i') || (c == 'l' && c2 == 'a') || (c == 's' && (c2 == 'i' || (c2 == 'o' && pitch[it+2] == 'l'))))
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

	const TagParameterString * style = getStyle();
    if (style) {
        std::string type = style->getValue();
        if( type == "/" )
            n = 1;
        else if( type == "//" )
            n = 2;
        else if (type == "///")
            n = 3;
        else if( type == "////")
            n = 4;
    }
    return n;
}

