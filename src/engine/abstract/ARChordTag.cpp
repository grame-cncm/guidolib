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

void ARChordTag::print(std::ostream& os) const
{
    os << "ARChordTag;" << std::endl;
}

