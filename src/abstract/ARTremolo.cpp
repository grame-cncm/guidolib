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

#include <iostream>
#include "ARTremolo.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
#include "ARMusicalVoice.h"

ListOfTPLs ARTremolo::ltpls(1);

ARTremolo::ARTremolo()
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);
	
	dx = dy = fThickness = 0;
	fStyle = NULL;
	fSpeed = NULL;
	fPitch = NULL;
	fText = NULL;

	mParSet = false;
}

// -----------------------------------------------------------------------------
ARTremolo::ARTremolo(const ARTremolo * tremolo)	: ARMTParameter(-1, tremolo)
{
	rangesetting = ONLY;
	setAssociation(ARMusicalTag::RA);

	dx = dy = fThickness = 0;
	fStyle = NULL;
	fSpeed = NULL;
	fPitch = NULL;
	fText = NULL;
	
	mParSet = false;


	if(tremolo->fStyle)
		fStyle = TagParameterString::cast( tremolo->fStyle->getCopy());
	if(tremolo->fSpeed)
		fSpeed = TagParameterInt::cast( tremolo->fSpeed->getCopy());
	if(tremolo->fPitch)
		fPitch = TagParameterString::cast( tremolo->fPitch->getCopy());
	if(tremolo->dx)
		dx = TagParameterFloat::cast( tremolo->dx->getCopy());
	if(tremolo->dy)
		dy = TagParameterFloat::cast( tremolo->dy->getCopy());
	if(tremolo->fThickness)
		fThickness = TagParameterFloat::cast( tremolo->fThickness->getCopy());
	if(tremolo->fText)
		fText = TagParameterString::cast( tremolo->fText->getCopy());
}



// -----------------------------------------------------------------------------
ARTremolo::~ARTremolo()
{
    delete fStyle;
    delete fSpeed;
    delete fPitch;
    delete dx;
    delete dy;
    delete fThickness;
    delete fText;
};

// -----------------------------------------------------------------------------
void ARTremolo::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail((
            "S,style,///,o;"
			"I,speed,32,o;"
            "S,pitch,,o;"
            "U,dx,0,o;U,dy,0,o;"
            "U,thickness,0.75,o;"
            "S,text,,o"
			));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			fStyle = TagParameterString::cast(rtpl->RemoveHead());
			assert(fStyle);
			if (fStyle->TagIsSet())
				mParSet = true;

			fSpeed = TagParameterInt::cast(rtpl->RemoveHead());
			assert(fSpeed);
			if (fSpeed->TagIsSet())
				mParSet = true;

			fPitch = TagParameterString::cast(rtpl->RemoveHead());
			assert(fPitch);
			if (fPitch->TagIsSet())
				mParSet = true;
            
			dx = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dx);
			if (dx->TagIsSet())
				mParSet = true;

			dy = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(dy);
			if (dy->TagIsSet())
				mParSet = true;

			fThickness = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(fThickness);
			if (fThickness->TagIsSet())
				mParSet = true;

			fText = TagParameterString::cast(rtpl->RemoveHead());
			assert(fText);
			if (fText->TagIsSet())
				mParSet = true;
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

// -----------------------------------------------------------------------------
TagParameterList * ARTremolo::getTagParameterList() const
{
	TagParameterList * tpl = new TagParameterList(1);
	if (fStyle && fStyle->TagIsSet())
	{
		tpl->AddTail(fStyle->getCopy());
	}
	if (fSpeed && fSpeed->TagIsSet())
	{
		tpl->AddTail(fSpeed->getCopy());
	}
	if (fPitch && fPitch->TagIsSet())
	{
		tpl->AddTail(fPitch->getCopy());
	}
	if (dx && dx->TagIsSet())
	{
		tpl->AddTail(dx->getCopy());
	}
	if (dy && dy->TagIsSet())
	{
		tpl->AddTail(dy->getCopy());
	}
	if (fThickness && fThickness->TagIsSet())
	{
		tpl->AddTail(fThickness->getCopy());
	}
	if (fText && fText->TagIsSet())
	{
		tpl->AddTail(fText->getCopy());
	}
	if (color && color->TagIsSet())
	{
		tpl->AddTail(color->getCopy());
	}

	return tpl;
}

// -----------------------------------------------------------------------------
void ARTremolo::print() const
{
}

// -----------------------------------------------------------------------------
void ARTremolo::PrintName(std::ostream & os) const
{
	os << "\\trem";

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
bool ARTremolo::isSecondPitchCorrect()
{
    bool isReadable = false;
    if(fPitch && fPitch->TagIsSet())
    {
        std::string pitch = fPitch->getValue();
        int it = 0;
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
int ARTremolo::getNumberOfStrokes()
{
    int nbOfStrokes = 3;

    std::string type = fStyle->getValue();
    if( type == "/" )
        nbOfStrokes = 1;
    else if( type == "//" )
        nbOfStrokes = 2;
    else if (type == "///")
        nbOfStrokes = 3;
    else if( type == "////")
        nbOfStrokes = 4;
    
    return nbOfStrokes;
}
