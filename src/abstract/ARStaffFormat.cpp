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
#include "ARStaffFormat.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

#include "GRDefine.h"
//#include "../parser/gmntools.h" // for gd_convertUnits
#include "gmntools.h" // for gd_convertUnits

ListOfTPLs ARStaffFormat::ltpls(1);

ARStaffFormat::ARStaffFormat(const ARStaffFormat &stffrmt)
{
	style = NULL;
	if (stffrmt.style!= NULL)
		style = TagParameterString::cast(stffrmt.style->getCopy());
	if (stffrmt.size != NULL)
		size = TagParameterFloat::cast(stffrmt.size->getCopy());
}

ARStaffFormat::~ARStaffFormat()
{
	delete style;
}

ARStaffFormat::ARStaffFormat()
{
	style = NULL;
}

void ARStaffFormat::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0) {
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( ( "S,style,standard,o;U,size,3pt,o"));
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
				const int Iintunits = (int) (intunits + 0.5);
				const double cmunits = Iintunits * kVirtualToCm;
				const char * unit = size->getUnit();
				intunits = (float)gd_convertUnits(cmunits,"cm",unit);
				size->setValue( intunits );
			}
		}
		delete rtpl;
	}
	else {
		// failure
	}
	tpl.RemoveAll();
}

void ARStaffFormat::PrintName(std::ostream & os) const
{
	os << "\\staffFormat";
}

void ARStaffFormat::PrintParameters(std::ostream & os) const
{
	if (style)
		os << "<style=\"" << style->getValue() << "\"";
	if (style && size)
		os << ",";
	else if (size)
		os << "<";
	else
		os << ">";
	if (size)
		os << "size=" << size->getUnitValue() << 
		size->getUnit() << ">";
	
}
