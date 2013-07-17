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
#include <string.h>
#include "ARRitardando.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "ListOfStrings.h"

ListOfTPLs ARRitardando::ltpls(1);

ARRitardando::ARRitardando() : ARMTParameter()
{
	rangesetting = ONLY;
	s1 = 0;
	s2 = 0;
}

ARRitardando::~ARRitardando() 
{
	delete s1;
	delete s2;
};

void ARRitardando::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			( "S,tempo,,o;S,abstempo,,o;S,font,Times new roman,o;S,fattrib,,o;U,fsize,9pt,o" ));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// Get The TagParameters ...
			s1 = TagParameterString::cast(rtpl->RemoveHead());
			//assert(text);
			s2 = TagParameterString::cast(rtpl->RemoveHead());
			
			// the font
			font = TagParameterString::cast(rtpl->RemoveHead());
			assert(font);
			// the font attrib ...
			fattrib = TagParameterString::cast(rtpl->RemoveHead());
			assert(fattrib);
			if (fattrib->TagIsNotSet())
			{
				delete fattrib;
				fattrib = 0;
			}
			// the size ...
			fsize = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(fsize);
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

void ARRitardando::print() const
{
}

void ARRitardando::PrintName(std::ostream &os) const
{
	os << "\\rit";
	if (!getRange())
		os << "Begin";
//	if (getRange()) os << "(";

}
void ARRitardando::PrintParameters(std::ostream & ) const
{
}


bool ARRitardando::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return true;

	if (!getRange() && !strcmp("\\ritEnd",s))
		return true;
	return false;
}
