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
#include "ARBarFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "GRDefine.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
//#include "../parser/gmntools.h" // for gd_convertUnits

ListOfTPLs ARBarFormat::ltpls(1);

ARBarFormat::ARBarFormat(const ARBarFormat & barfrmt)
{
	style = NULL;
	if (barfrmt.style!= NULL)
		style = TagParameterString::cast(barfrmt.style->getCopy());
}

ARBarFormat::~ARBarFormat()
{
	delete style;
}

ARBarFormat::ARBarFormat()
{
	style = NULL;
}

void ARBarFormat::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,style,staff,o"));
		CreateListOfTPLs(ltpls,lstrs);

	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb

			style = TagParameterString::cast(rtpl->RemoveHead());
			assert(style);

			if( style->TagIsSet() == false )
			{
				delete style;
				style = NULL;
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

void ARBarFormat::print() const 
{ 
	PrintName (std::cout); 
	PrintParameters (std::cout);
}

void ARBarFormat::PrintName(std::ostream & os) const
{
	os << "\\barFormat";
}

void ARBarFormat::PrintParameters(std::ostream & os) const
{
	if (style)
		os << "<style=\"" << style->getValue() << "\"" << ">";
	
}


