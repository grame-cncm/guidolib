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
	fShowTR = true;
	fDrawOnNoteHead = false;
	begin = true;
}

ARTrill::ARTrill(int pid, const ARTrill* copy) : ARMTParameter(pid, copy), mDur(NULL)
{
	mTrillType = copy->getType();
	chordType = copy->getChordType();
	chordAccidental = copy->getChordAccidental();
	fShowCautionaryAccidentals = copy->getCautionary();
	fShowTR = copy->fShowTR;
	fDrawOnNoteHead = copy->fDrawOnNoteHead;
	adx = copy->getadx();
	ady = copy->getady();
	begin = copy->getStatus();
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
		lstrs.AddTail("S,mode,,o;I,dur,32,o;U,adx,0hs,o;U,ady,0hs,o;S,tr,true,o;S,anchor,above,o");
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

			TagParameterString * tr = TagParameterString::cast(rtpl->RemoveHead());
			assert(tr);
			if (tr->TagIsSet() && (tr->getValue() == std::string("false") || tr->getValue() == std::string("0")))
				fShowTR = false;
			delete tr;

			TagParameterString * anchor = TagParameterString::cast(rtpl->RemoveHead());
			assert(anchor);
			if (anchor->TagIsSet() && (anchor->getValue() == std::string("note")))
				fDrawOnNoteHead = true;
			else 
				fDrawOnNoteHead = false;
			delete anchor;

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

void ARTrill::print(int &indent) const
{
}

void ARTrill::PrintName(std::ostream & os) const
{
	if		(mTrillType == TRILL)	os << "\\trill";
	else if (mTrillType == TURN)	os << "\\turn";
	else if (mTrillType == MORD)	os << "\\mord";
}
void ARTrill::PrintParameters(std::ostream & os) const
{
	/*if (mDur && mDur->TagIsSet())
	{
		os << "<dur=" << mDur->getValue() << ">";
	}*/
}

bool ARTrill::getStatus() const{
	return begin;
}

void ARTrill::setContinue(){
	begin = false;
}
