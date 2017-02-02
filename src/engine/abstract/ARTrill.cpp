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
	rangesetting               = ONLY;
	fShowCautionaryAccidentals = false;
	fShowTR                    = true;
	fDrawOnNoteHead            = false;
	fRepeat                    = kUndefined;
	begin                      = true;
}

ARTrill::ARTrill(int pid, const ARTrill* copy) : ARMTParameter(pid, copy), mDur(NULL)
{
	mTrillType                 = copy->getType();
	chordType                  = copy->getChordType();
	chordAccidental            = copy->getChordAccidental();
	fShowCautionaryAccidentals = copy->getCautionary();
	fShowTR                    = copy->fShowTR;
	fDrawOnNoteHead            = copy->fDrawOnNoteHead;
	fRepeat                    = copy->fRepeat;
	adx                        = copy->getadx();
	ady                        = copy->getady();
	begin                      = copy->getStatus();
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
		lstrs.AddTail("S,mode,,o;I,dur,32,o;U,adx,0hs,o;U,ady,0hs,o;S,tr,true,o;S,anchor,above,o;S,repeat,true,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString *str = TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet() && (str->getValue() == std::string("cautionary")))
				fShowCautionaryAccidentals = true;
			else 
				fShowCautionaryAccidentals = false;
			delete str;

			TagParameterInt *i = TagParameterInt::cast(rtpl->RemoveHead());
			// Todo
			delete i;

			TagParameterFloat* f = TagParameterFloat::cast(rtpl->RemoveHead());
			adx = f->getValue();
			delete f;

			f = TagParameterFloat::cast(rtpl->RemoveHead());
			ady = f->getValue();
			delete f;

			str = TagParameterString::cast(rtpl->RemoveHead());
            fShowTR = str->getBool();
			delete str;

			str = TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet() && (str->getValue() == std::string("note")))
				fDrawOnNoteHead = true;
			else 
				fDrawOnNoteHead = false;
			delete str;

			str = TagParameterString::cast(rtpl->RemoveHead());
			if (str->TagIsSet())
				fRepeat = str->getBool() ? kOn : kOff;
			delete str;
		}

		delete rtpl;
	}

	tpl.RemoveAll();
}

void ARTrill::printName(std::ostream& os) const
{
    os << "ARTrill";

    switch (mTrillType) {
    case TRILL:	os << " (trill)";
        break;
    case TURN:	os << " (turn)";
        break;
    case MORD:	os << " (mord)";
        break;
    }
}

void ARTrill::printGMNName(std::ostream& os) const
{
    os << "\\";

    switch (mTrillType) {
    case TRILL:	os << "trill";
        break;
    case TURN:	os << "turn";
        break;
    case MORD:	os << "mord";
        break;
    }
}

void ARTrill::printParameters(std::ostream& os) const
{
    os << "mode: "   << (fShowCautionaryAccidentals ? "cautionnary" : "none") << "; ";
    os << "adx: "    << adx << "; ";
    os << "ady: "    << ady << "; ";
    os << "tr: "     << (fShowTR ? "true" : "false") << "; ";
    os << "anchor: " << (fDrawOnNoteHead ? "note" : "above") << "; ";

    ARMusicalTag::printParameters(os);
}
