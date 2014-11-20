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
#include "ARNoteFormat.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARNoteFormat::ltpls(1);

ARNoteFormat::ARNoteFormat(ARNoteFormat *p_savenf,ARNoteFormat *copynf)
	: ARMTParameter(-1,copynf)
//const ARNoteFormat *curnf)
//: savenf(curnf)
{
	// this probably should be changed to RANGEDC
	// but then we have to take care of saving state-information ....
	rangesetting = ARMusicalTag::RANGEDC;
	savenf = p_savenf;
	style = NULL;
	if (copynf) {
		if (copynf->getTPStyle())
			style = TagParameterString::cast(copynf->getTPStyle()->getCopy());
	}
}


ARNoteFormat::~ARNoteFormat()
{
	delete style;
}

void ARNoteFormat::setTagParameterList(TagParameterList &tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,style,standard,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			style = TagParameterString::cast(rtpl->RemoveHead());
			assert(style);
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

const char* ARNoteFormat::getStyle() const
{
	if (style)
		return style->getValue();
	return "";		
}

void ARNoteFormat::PrintParameters(std::ostream & os) const
{
	// todo 
	if (style)
	{	os << "<style=\"" << style->getValue() << "> "; }
//		\",size=" 
//		<< size << ",dx=" << dx << "> ";
}

void ARNoteFormat::PrintName(std::ostream & os) const
{
	os << "\\noteFormat";
	if (assoc == ARMusicalTag::RA)
	{
		if (!getRange())
			os << "Begin";
		else
			os << "(";
	}
}

void ARNoteFormat::print(std::ostream& os) const
{    
    os << "ARNoteFormat: ";

    if (style)
        os << "style: " << style->getValue() << ";";

    os << std::endl;
}