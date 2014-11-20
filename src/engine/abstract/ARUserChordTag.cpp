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

// #include "defines.h"

#include <iostream>
#include "secureio.h"
#include "ARUserChordTag.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARUserChordTag::ltpls(1);

ARUserChordTag::ARUserChordTag(const ARUserChordTag * uct)
{
	labels = NULL;
	labeli = NULL;
	labelistr = NULL;

	if (uct->labels)
		labels = TagParameterString::cast
		(uct->labels->getCopy());
	else if (uct->labeli) {
		labeli = TagParameterInt::cast
		(uct->labeli->getCopy());

		if (uct->labelistr)
			labelistr = new NVstring(*uct->labelistr);
		else {
			labelistr = new NVstring("intlabel");
			char buf[100];			
			snprintf(buf, 100, "%d", labeli->getValue()); 
			labelistr->append(buf);
		}
	}
}

ARUserChordTag::~ARUserChordTag()
{
	delete labels;
	delete labeli;
	delete labelistr;
}

void ARUserChordTag::PrintName(std::ostream &os) const
{
	os << "\\splitChord";
	if (!getRange())
		os << "Begin";
}

ARMusicalObject * ARUserChordTag::Copy() const
{
	return new ARUserChordTag(this);
}

const char* ARUserChordTag::getLabelValue() const
{
	if (labels)
		return labels->getValue();

	else if (labeli && labelistr)
		return labelistr->c_str();

	return "";
}

void ARUserChordTag::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,label,,o"));
		lstrs.AddTail(("I,label,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList *rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			// then, we now the match for the first ParameterList
			// GuidoPos pos = rtpl->GetHeadPosition();
			labels = TagParameterString::cast(rtpl->RemoveHead());
		}
		else if (ret == 1)
		{
			labeli = TagParameterInt::cast(rtpl->RemoveHead());
			labelistr = new NVstring("intlabel");

			char buf[100];			
			snprintf(buf, 100, "%d", labeli->getValue()); 
			labelistr->append(buf);
		}
		delete rtpl;
	}
	tpl.RemoveAll();
}

void ARUserChordTag::PrintParameters(std::ostream & os) const
{
	if (labels && labels->TagIsSet())
		os << "<label=\"" << labels->getValue() << "\">";
	else if (labeli && labeli->TagIsSet())
		os << "<label=" << labeli->getValue() << ">";
}

void ARUserChordTag::print(std::ostream& os) const
{
    os << "ARUserChordTag: ";

    if (labels)
        os << "label: \"" << labels->getValue() << "\"; ";
    else if (labeli)
        os << "label: " << labeli->getValue() << "; ";

    os << std::endl;
}

