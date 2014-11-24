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
#include "ARTitle.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "ListOfStrings.h"


ListOfTPLs ARTitle::ltpls(1);


ARTitle::ARTitle()
{
	name         = NULL;
	pageformat   = NULL;
	rangesetting = NO;
}

ARTitle::~ARTitle()
{
	delete name;
	delete pageformat;
}

const char * ARTitle::getName() 
{ 
	if (name == NULL)
		return NULL;
	else
		return name->getValue();
}

const char * ARTitle::getPageFormat()
{
	if (pageformat == NULL)
		return NULL;
	else
		return pageformat->getValue();
}

void ARTitle::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail((
			"S,name,,r;"
			"S,pageformat,c2,o;S,textformat,cc,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList name
			delete name;
			delete pageformat;
			delete textformat;

			name = TagParameterString::cast(rtpl->RemoveHead());
			assert(name);

			// pageformat
			pageformat = TagParameterString::cast(rtpl->RemoveHead());
			assert(pageformat);	

			textformat = TagParameterString::cast(rtpl->RemoveHead());
			assert(textformat);
		}

		if (fsize && fsize->TagIsNotSet())
		{
			fsize->setValue(24);
			fsize->setUnit("pt");
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARTitle::printName(std::ostream& os) const
{
    os << "ARTitle";
    ARMusicalTag::printName(os);
}

void ARTitle::printParameters(std::ostream& os) const
{
    if (name)
        os << "name: \"" << name->getValue() << "\"; ";

    if (pageformat)
        os << "pageformat: " << pageformat->getValue() << "; ";

    if (textformat)
        os << "textformat" << textformat->getValue() << ";";

    ARMusicalTag::printParameters(os);
}
