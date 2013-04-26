/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

*/

#include <iostream>
#include "ARCluster.h"
#include "TagParameterList.h"
#include "TagParameterInt.h"

using namespace std;

ListOfTPLs ARCluster::ltpls(1);

ARCluster::ARCluster() : ARMTParameter()
{
	rangesetting = ONLY;

    for(int i = 0; i <= 2; i++)
    {
        mFirstNote[i] = 0;
        mSecondNote[i] = 0;
    }
}

ARCluster::~ARCluster() 
{
}

void ARCluster::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs;
		lstrs.AddTail("U,adx,0hs,o;U,ady,0hs,o"); //REM: ajouter des paramètres ?
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			/*TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet() && (str->getValue() == std::string("cautionary")))
				fShowCautionaryAccidentals = true;
			else 
				fShowCautionaryAccidentals = false;
			delete str;

			TagParameterInt * dur = TagParameterInt::cast(rtpl->RemoveHead());
			// Todo
			delete dur;

			TagParameterFloat* f = TagParameterFloat::cast(rtpl->RemoveHead());
			adx = f->getValue();
			delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
			ady = f->getValue();
			delete f;
			*/
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

void ARCluster::PrintName(std::ostream & os) const
{
	os << "\\cluster";
}

void ARCluster::PrintParameters(std::ostream & os) const
{
	/*if (!num) return;
	
	os << "<i=\"" << num->getValue() << "\">";*/
}

void ARCluster::setNotePitchAndOctave(int inPitch, int inOctave, int inAccidental)
{
    if (mFirstNote[0] == 0)
    {
        mFirstNote[0] = inPitch;
        mFirstNote[1] = inOctave;
        mFirstNote[2] = inAccidental;
    }
    else
    {
        mSecondNote[0] = inPitch;
        mSecondNote[1] = inOctave;
        mSecondNote[2] = inAccidental;
    }
}