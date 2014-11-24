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
#include "ARStaffFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "GRDefine.h"
#include "gmntools.h" // for gd_convertUnits

ListOfTPLs ARStaffFormat::ltpls(1);

ARStaffFormat::ARStaffFormat(const ARStaffFormat &stffrmt)
{
	style = NULL;
	if (stffrmt.style!= NULL)
		style = TagParameterString::cast(stffrmt.style->getCopy());
	if (stffrmt.size != NULL)
		size = TagParameterFloat::cast(stffrmt.size->getCopy());
	fLineThickness = stffrmt.getLineThickness();
}

ARStaffFormat::~ARStaffFormat()
{
	delete style;
}

ARStaffFormat::ARStaffFormat() : fLineThickness(kLineThick)
{
	style = NULL;
}

void ARStaffFormat::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0) {
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(("S,style,standard,o;U,size,3pt,o;F,lineThickness,0.08,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);
	if (ret >= 0 && rtpl)
	{
		// we found a match!
		if (ret == 0) {
			// then, we now the match for the first ParameterList
			// w, h, ml, mt, mr, mb
			style = TagParameterString::cast(rtpl->RemoveHead());
			assert(style);
			if (style->TagIsSet() == false) {
				delete style;
				style = NULL;
			}

			size = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(size);
			if (size->TagIsSet() == false) {
				delete size;
				size = NULL;
			}
			else
			{
				// one idea is to adjust the size, so that it matches an integer (internally)
				float intunits = size->getValue();		// per halfspace ...
				// Integer internal units 
				const int Iintunits  = (int)(intunits + 0.5);
				const double cmunits = Iintunits * kVirtualToCm;
				const char *unit     = size->getUnit();

                double result;
				bool conversionOk    = gd_convertUnits(cmunits, "cm", unit, result);

                if (conversionOk)
				    size->setValue((float)result);
                else
                {
                    delete size;
                    size = NULL;
                }
			}

			TagParameterFloat* fval =  TagParameterFloat::cast(rtpl->RemoveHead());
			assert(fval);
			if (fval->TagIsSet())
				fLineThickness = (LSPACE * fval->getValue() >= 0 ? LSPACE * fval->getValue() : 0);

			delete fval;
		}
		delete rtpl;
	}
	else {
		// failure
	}
	tpl.RemoveAll();
}

void ARStaffFormat::printName(std::ostream& os) const
{
    os << "ARStaffFormat";
    ARMusicalTag::printName(os);
}

void ARStaffFormat::printParameters(std::ostream& os) const
{
    if (style)
        os << "style:" << style->getValue() << "; ";

    ARMusicalTag::printParameters(os);
}
