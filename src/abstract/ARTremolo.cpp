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
std::vector<ARNote*> ARTremolo::createSecondNotes(TYPE_DURATION dur, int oct)
{
    std::vector<ARNote*> notes;
    int octave = oct;
    if(fPitch->TagIsSet())
    {
        std::string pitch = fPitch->getValue();
        int it = 0;
        char c = pitch[it];
        if(c == '{')
            it++;
        
        while(it < pitch.size())
        {
            c = pitch[it];
            int pitchInt = 0;
            if(c == 'a')
                pitchInt = NOTE_A;
            else if(c == 'b')
                pitchInt = NOTE_H;
            else if(c == 'c')
                pitchInt = NOTE_C;
            else if(c == 'd')
                pitchInt = NOTE_D;
            else if(c == 'e')
                pitchInt = NOTE_E;
            else if(c == 'f')
                pitchInt = NOTE_F;
            else if(c == 'g')
                pitchInt = NOTE_G;
            else if(c == 'h')
                pitchInt = NOTE_H;
            else
                pitchInt = EMPTY;
        
        
            if(pitchInt != EMPTY)
            {
                ARNote * note = new ARNote(dur);
                //if(notes.empty()) // if it is the first note we add, we set its duration, otherwise it is a chord and the other notes have no duration
                  //  note->setDuration(dur);
                note->setPitch(pitchInt);
            
                it++;
                c = pitch[it];
                if(c == '#')
                {
                    note->setAccidentals(ARMusicalVoice::SHARP);
                    it++;
                }
                else if (c == '&')
                {
                    note->setAccidentals(ARMusicalVoice::FLAT);
                    it++;
                }
                else
                    note->setAccidentals(ARMusicalVoice::NATURAL);
            
                c = pitch[it];
                std::string octStr;
                std::string possibleOct = "0123456789";
                if(c == '-')
                {
                    octStr.push_back(c);
                    it++;
                    c = pitch[it];
                }
                while(c && possibleOct.find(c) != std::string::npos)
                {
                    octStr.push_back(c);
                    it++;
                    c = pitch[it];
                }
                
                if(octStr.size())
                    octave = atoi( octStr.c_str() );
                    
                note->setOctave(octave);
        
                notes.push_back(note);
            }
            it++;
        }
    }
    return notes;
}
