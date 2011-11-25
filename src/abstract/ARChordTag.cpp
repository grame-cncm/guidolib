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
#include "ARChordTag.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARChordTag::ltpls(1);

ARChordTag::ARChordTag(const ARChordTag * chrd)
{
	labels = NULL;
	labeli = NULL;
	if (chrd->labels)
		labels = TagParameterString::cast(
			chrd->labels->getCopy());
	else if (chrd->labeli)
		labeli = TagParameterInt::cast(
			chrd->labeli->getCopy());
}

ARMusicalObject * ARChordTag::Copy() const
{
	return new ARChordTag(this);
}

ARChordTag::~ARChordTag()
{
	delete labels;
	delete labeli;
}

void ARChordTag::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(
			(
			"S,label,,o"));
		lstrs.AddTail(
			(
			"I,label,,o"));
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
			// GuidoPos pos = rtpl->GetHeadPosition();

			labels = TagParameterString::cast(rtpl->RemoveHead());
		}

		else if (ret == 1)
		{
			// we have a label-number
			labeli = TagParameterInt::cast(rtpl->RemoveHead());
		}

		delete rtpl;
	}
		
	
	tpl.RemoveAll();
}

void ARChordTag::PrintParameters(std::ostream &os) const
{
	if (labels)
		os << "<label=\"" << labels->getValue() << "\">";
	else if (labeli)
		os << "<label=" << labeli->getValue() << ">";
}

void ARChordTag::PrintName(std::ostream & os) const
{
	if (isAuto)
		os << "\\autoChord";
	else
		os << "\\chord";
	if (!getRange())
		os << "Begin";
}

