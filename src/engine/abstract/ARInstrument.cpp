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

#include "ARInstrument.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARInstrument::ltpls(1);

using namespace std;

ARInstrument::ARInstrument(bool autopos)
{
	fName = 0;
	fTransp = 0;
	fTextFormat = 0;
	fFont = 0;
	fSize = 0;
	fTextAttributes = 0;
	fAutoPos = autopos;
}

ARInstrument::~ARInstrument()
{
	delete fName;
	delete fTransp;
	delete fTextFormat;
	delete fFont;
	delete fSize;
	delete fTextAttributes;
}
void ARInstrument::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail (( "S,name,,r;S,transp,,o;S,textformat,rc,o;S,font,Times,o;U,fsize,9pt,o;S,fattrib,,o;S,autopos,off,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			fName   = TagParameterString::cast(rtpl->RemoveHead());
			fTransp = TagParameterString::cast(rtpl->RemoveHead());
			fTextFormat = TagParameterString::cast(rtpl->RemoveHead());
			fFont	= TagParameterString::cast(rtpl->RemoveHead());
			fSize	= TagParameterFloat::cast(rtpl->RemoveHead());
			fTextAttributes	= TagParameterString::cast(rtpl->RemoveHead());
			TagParameterString* autopos = TagParameterString::cast(rtpl->RemoveHead());
			string autoStr = autopos->getValue();
			if (autoStr == "on") fAutoPos = true;
			delete autopos;
		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

const char* ARInstrument::getName() const			{ return fName ? fName->getValue() : NULL; }
const char* ARInstrument::getFont() const			{ return fFont ? fFont->getValue() : "Times"; }
const char* ARInstrument::getTextFormat() const		{ return fTextFormat ? fTextFormat->getValue() : "rc"; }
const char* ARInstrument::getTextAttributes() const	{ return fTextAttributes ? fTextAttributes->getValue() : ""; }
float ARInstrument::getSize() const					{ return fSize ? fSize->getValue() : 10; }

void ARInstrument::printName(std::ostream& os) const
{
    os << "ARInstrument";
}

void ARInstrument::printGMNName(std::ostream& os) const
{
    os << "\\instrument";
}

void ARInstrument::printParameters(std::ostream& os) const
{
    if (fName)
        os << "name: \"" << fName->getValue() << "\"; ";

    if (fTransp)
        os << "transp: " << fTransp->getValue() << "; ";

    ARMusicalTag::printParameters(os);
}
