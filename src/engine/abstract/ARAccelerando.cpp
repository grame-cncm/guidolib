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

#include <string.h>
#include <iostream>

#include "ARAccelerando.h"
#include "TagParameterString.h"
#include "TagParameterList.h"
#include "TagParameterFloat.h"
#include "ListOfStrings.h"

using namespace std;

ListOfTPLs ARAccelerando::ltpls(1);

ARAccelerando::ARAccelerando()
{
    rangesetting = ONLY;
    
    s1 = 0;
    s2 = 0;
	font = 0;
	fattrib = 0;
	fsize = 0;
}

ARAccelerando::~ARAccelerando()
{
	delete s1;
	delete s2;
	// delete TagParameterPointer ...
}

void ARAccelerando::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.empty())
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,tempo,,o;S,abstempo,,o;S,font,Times new roman,o;S,fattrib,,o;U,fsize,9pt,o"
			));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
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

void ARAccelerando::print() const
{
}

void ARAccelerando::PrintName(ostream &os) const
{
	if (getRange())
		os << "\\accel";
	else 
		os << "\\accelBegin";
}

void ARAccelerando::PrintParameters(ostream &os) const
{
		//	"S,tempo,,o;S,abstempo,,o"
	int first = 1;
	if (s1 && s1->TagIsSet())
	{
		if (first)
		{
			os << "<";
			first = 0;
		}


		os << "\"" << s1->getValue() << "\"";

	}
	if (s2 && s2->TagIsSet())
	{
		if (first)
		{
			os << "<";
			first = 0;
		}
		else 
			os << ",";
		os << "\"" << s2->getValue() << "\"";

	}
	if (!first)
		os << ">";
}


bool ARAccelerando::MatchEndTag(const char *s)
{
	if (ARMusicalTag::MatchEndTag(s))
		return 1;

	if (!getRange() &&
		!strcmp("\\accelEnd",s))
		return 1;
	return 0;
}
