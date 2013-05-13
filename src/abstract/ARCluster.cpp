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
#include "TagParameterString.h"

using namespace std;

ListOfTPLs ARCluster::ltpls(1);

ARCluster::ARCluster() : ARMTParameter()
{
	rangesetting = ONLY;

    for(int i = 0; i <= 1; i++)
    {
        mFirstNote[i] = 0;
        mSecondNote[i] = 0;
    }

    adx = 0;
    ady = 0;
    ahdx = 0;
    ahdy = 0;
    aSize = 1.0;
    aColor = NULL;
}

ARCluster::ARCluster(ARCluster *inCopyCluster) : ARMTParameter()
{
	rangesetting = ONLY;

    for(int i = 0; i <= 1; i++)
    {
        mFirstNote[i] = 0;
        mSecondNote[i] = 0;
    }

    if (inCopyCluster)
    {
        adx = inCopyCluster->getadx();
        ady = inCopyCluster->getady();
        ahdx = inCopyCluster->getahdx();
        ahdy = inCopyCluster->getahdy();
        aSize = inCopyCluster->getSize();
        aColor = inCopyCluster->getColor();
    }
}

ARCluster::~ARCluster() 
{
}

void ARCluster::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,color,,o;F,size,1.0,o;U,adx,0hs,o;U,ady,0hs,o;U,dx,0hs,o;U,dy,0hs,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
            aColor = TagParameterString::cast(rtpl->RemoveHead());

            TagParameterFloat *f = TagParameterFloat::cast(rtpl->RemoveHead());
			aSize = f->getValue();
			delete f;

            // - dx/dy for cluster head only
			f = TagParameterFloat::cast(rtpl->RemoveHead());
			ahdx = f->getValue();
			delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
			ahdy = f->getValue();
			delete f;

            // - dx/dy for entire cluster
            f = TagParameterFloat::cast(rtpl->RemoveHead());
			adx = f->getValue();
			delete f;

            f = TagParameterFloat::cast(rtpl->RemoveHead());
			ady = f->getValue();
			delete f;
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

float ARCluster::getadx() const	
{
	return adx;
}

float ARCluster::getady() const
{
	return ady;
}

float ARCluster::getahdx() const	
{
	return ahdx;
}

float ARCluster::getahdy() const
{
	return ahdy;
}

float ARCluster::getSize() const
{
	return aSize;
}

TagParameterString *ARCluster::getColor() const
{
	return aColor;
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

void ARCluster::setNotePitchAndOctave(int inPitch, int inOctave)
{
    if (mFirstNote[0] == 0)
    {
        mFirstNote[0] = inPitch;
        mFirstNote[1] = inOctave;
    }
    else
    {
        mSecondNote[0] = inPitch;
        mSecondNote[1] = inOctave;
    }
}