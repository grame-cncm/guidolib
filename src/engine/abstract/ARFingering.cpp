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
#include "ARFingering.h"
// #include "ARFactory.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

// ListOfTPLs ARFingering::ltpls(1);

ARFingering::ARFingering()
{
	rangesetting = ONLY;
}

ARFingering::~ARFingering()
{
}

const char * 
ARFingering::getTagFormat() const	 // (JB) new
{
	const char * const outFormat = "S,text,,r;U,dy,-2,o";
	return outFormat;
}

/* (JB) I did not find any specification for fingering tag
		format. I though is was usefull to have a dy, even for
		default positionning, which is certainly different from
		standard text. So, the fingering has now the same tag format
		than text, only the default value for dy changes
		(see getTagFormat())

void ARFingering::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "S,text,,r"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	const int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			text = TagParameterString::cast (rtpl->RemoveHead());
			assert(text);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}
*/

void ARFingering::printName(std::ostream& os) const
{
    os << "ARFingering";
}

void ARFingering::printGMNName(std::ostream& os) const
{
    os << "\\fingering";
}

void ARFingering::printParameters(std::ostream& os) const
{
    if (text)
        os << "text: \"" << text->getValue() << "\"; ";

    if (fsize)
        os << "fsize: " << fsize->getValue() << ";";

    ARMusicalTag::printParameters(os);
}
