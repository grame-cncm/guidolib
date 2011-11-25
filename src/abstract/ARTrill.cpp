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
#include "ARTrill.h"
#include "ListOfStrings.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"

using namespace std;

ListOfTPLs ARTrill::ltpls(1);

ARTrill::ARTrill(TYPE typ) : ARMTParameter(), mDur(NULL), mTrillType(typ)
{
	rangesetting = ONLY;
	fShowCautionaryAccidentals = false;
}

ARTrill::ARTrill(int pid, const ARTrill* copy) : ARMTParameter(pid, copy), mDur(NULL)
{
	mTrillType = copy->getType();
	chordType = copy->getChordType();
	chordAccidental = copy->getChordAccidental();
	fShowCautionaryAccidentals = copy->getCautionary();
	adx = copy->getadx();
	ady = copy->getady();
	//TODO : copy TagParameterInt* mDur
}

ARTrill::~ARTrill() 
{
	delete mDur;
}

void ARTrill::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,mode,,o;I,dur,32,o;U,adx,0hs,o;U,ady,0hs,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
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
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

float ARTrill::getadx() const	
{
	return adx;
}

float ARTrill::getady() const
{
	return ady;
}

void ARTrill::print() const
{
}

void ARTrill::PrintName(std::ostream & os) const
{
	if		(mTrillType == TRILL)	os << "\\trill";
	else if (mTrillType == TURN)	os << "\\turn";
	else if (mTrillType == MORD)	os << "\\mord";
	//if (getRange()) os << "(";

}
void ARTrill::PrintParameters(std::ostream & os) const
{
	/*if (mDur && mDur->TagIsSet())
	{
		os << "<dur=" << mDur->getValue() << ">";
	}*/
}


