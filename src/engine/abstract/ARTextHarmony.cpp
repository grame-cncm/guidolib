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

#include "ARTextHarmony.h"

#include "ARLyrics.h"
#include "GRDefine.h"
#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

ListOfTPLs ARTextHarmony::ltpls(1);

ARTextHarmony::ARTextHarmony(const NVstring & p_txt, float)
{
    ypos         = NULL;
    text         = new TagParameterString(p_txt.c_str());
    textformat   = NULL;
    font         = NULL;
    fsize        = NULL;
    fattrib      = NULL;
    rangesetting = RANGEDC;
}

ARTextHarmony::ARTextHarmony() : ARMTParameter()
{
    relativeTimePosition = TYPE_TIMEPOSITION(-1,1);
    setDuration( DURATION_0 );
    ypos                 = NULL;
    text                 = NULL;
    textformat           = NULL;
    font                 = NULL;
    fsize                = NULL;
    fattrib              = NULL;
    rangesetting         = RANGEDC;
}

ARTextHarmony::~ARTextHarmony()
{
    delete text;
    delete textformat;
    delete font;
    delete fsize;
    delete fattrib;
    delete ypos;
};

const char *
ARTextHarmony::getTagFormat() const	 // (JB) new
{
    const char * const outFormat = "S,text,,r;U,dy,-1,o";
    return outFormat;
}

void
ARTextHarmony::setTagParameterList(TagParameterList & tpl)
{
    if (ltpls.GetCount() == 0)
    {
        // create a list of string ...
        ListOfStrings lstrs; // (1); std::vector test impl
        lstrs.AddTail( getTagFormat());
        //"S,text,,r;U,dy,-1,o"));
        CreateListOfTPLs(ltpls,lstrs);
    }

    TagParameterList * rtpl = NULL;
    int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

    if (ret>=0 && rtpl)
    {
        // we found a match!
        if (ret == 0)
        {
            // then, we now the match for the first ParameterList
            delete text;
            text = TagParameterString::cast(rtpl->RemoveHead());
            assert(text);

            delete mDy;
            mDy = TagParameterFloat::cast(rtpl->RemoveHead());
            assert(mDy);
        }
        delete rtpl;
    }
    else
    {
        // failure
    }
    tpl.RemoveAll();
}

/** \brief Grabs the added Text-Parametersif a match has been found.
 */
int ARTextHarmony::MatchListOfTPLsWithTPL(const ListOfTPLs &ltpls,TagParameterList &tpl,
                                   TagParameterList **rtpl)
{
    int ret = ARMusicalTag::MatchListOfTPLsWithTPL(ltpls,tpl,rtpl);
    if (ret >= 0)
    {
        TagParameterList * mytpl = *rtpl;
        // then we have to grab the values at the end ...

        // the last one is fattrib ...
        fattrib = TagParameterString::cast(mytpl->RemoveTail());
        assert(fattrib);
        if (fattrib->TagIsNotSet())
        {
            delete fattrib;
            fattrib = 0;
        }

        // the size ...
        fsize = TagParameterFloat::cast(mytpl->RemoveTail());
        assert(fsize);
        // the font
        font = TagParameterString::cast(mytpl->RemoveTail());
        assert(font);

        // the textformat
        textformat = TagParameterString::cast(mytpl->RemoveTail());
        assert(textformat);
        if (textformat->TagIsNotSet())
        {
            delete textformat;
            textformat = 0;
        }

        ypos = TagParameterFloat::cast(mytpl->RemoveTail());
        assert(ypos);
        if (ypos->TagIsNotSet())
        {
            delete ypos;
            ypos = 0;
        }
    }

    return ret;
}

/** \brief Creates the ListOfTPLs (tag parameters) if it is not already present.

	It adds the default TEXT-Parameters and then calls the default in ARMusicalTag.
 */
void ARTextHarmony::CreateListOfTPLs( ListOfTPLs &ltpl,ListOfStrings & lstrs)
{
    ListOfStrings::iterator ptr;
    for( ptr = lstrs.begin(); ptr != lstrs.end(); ++ ptr )
    {
        NVstring & mystr = *ptr;
        mystr.append(";U,ypos,0hs,o;S,textformat,cc,o;"
                     "S,font,Arial,o;U,fsize,18pt,o;"
                     "S,fattrib,,o"); // TODO: replace "Times" ?
    }
    ARMusicalTag::CreateListOfTPLs(ltpl,lstrs);
}


void ARTextHarmony::setTextformat(const TagParameterString * tf)
{
    delete textformat;
    textformat = TagParameterString::cast(tf->getCopy());
}

void ARTextHarmony::setFont(const TagParameterString * ft)
{
    delete font;
    font = TagParameterString::cast(ft->getCopy());
}

void ARTextHarmony::setFAttrib(const TagParameterString * fa)
{
    delete fattrib;
    fattrib = TagParameterString::cast(fa->getCopy());
}

void ARTextHarmony::setFSize(const TagParameterFloat *fs)
{
    delete fsize;
    fsize = TagParameterFloat::cast(fs->getCopy());
}

int ARTextHarmony::getFSize(float curLSPACE) const
{
    return fsize ? (int) fsize->getValue(curLSPACE) : 0;
}

void ARTextHarmony::printName(std::ostream& os) const
{
    os << "ARTextHarmony";
}

void ARTextHarmony::printGMNName(std::ostream& os) const
{
    os << "\\text";
}

void ARTextHarmony::printParameters(std::ostream& os) const
{
    if (text)
        os << "text: \"" << text->getValue() << "\"; ";
    
    if (textformat)
        os << "textformat: " << textformat->getValue() << "; ";
    
    if (font)
        os << "font: " << font->getValue() << "; ";
    
    if (fsize)
        os << "fsize: " << fsize->getValue() << "; ";
    
    if (fattrib)
        os << "fattrib: " << fattrib->getValue() << "; ";
    
    ARMusicalTag::printParameters(os);
}



