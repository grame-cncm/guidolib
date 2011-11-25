/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

