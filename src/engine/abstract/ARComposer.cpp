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

#include "ARComposer.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"


ListOfTPLs ARComposer::ltpls(1);


ARComposer::ARComposer()
{
	name = pageformat = NULL;
	rangesetting = NO;
}


ARComposer::~ARComposer()
{
	delete name;
	delete pageformat;
}

void ARComposer::print(std::ostream& os) const
{
    os << "ARComposer: ";

    if (name)
        os << "name: \"" << name->getValue() << "\"; ";

    if (pageformat)
        os << "pageformat: " << pageformat->getValue() << ";";

    os << std::endl;
}

void ARComposer::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,name,,r;"
			"S,pageformat,53,o"));
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
			name =  TagParameterString::cast(rtpl->RemoveHead());
			assert(name);

			// pageformat
			pageformat = TagParameterString::cast(rtpl->RemoveHead());
			assert(pageformat);

			if (!textformat)
				textformat = new TagParameterString("rb");

			if (fsize && fsize->TagIsNotSet())
			{
				fsize->setValue(14);
				fsize->setUnit("pt");
			}
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

std::ostream & ARComposer::operator<<(std::ostream & os) const
{
	return os << "\\composer<\"" << name << "\"> ";
}

const char* ARComposer::getName() const			{ return name ? name->getValue() : 0; }
const char* ARComposer::getPageFormat() const	{ return pageformat ? pageformat->getValue() : 0; }



