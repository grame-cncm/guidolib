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

#include "ARAlter.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
#include "secureio.h"

using namespace std;

ListOfTPLs ARAlter::ltpls(1);

ARAlter::ARAlter(ARAlter * p_savealter, ARAlter * copyalter ) 
	: ARMTParameter(-1,copyalter) 
{
	relativeTimePosition = TYPE_TIMEPOSITION(-1,1);
	duration = DURATION_0;
	rangesetting = RANGEDC;	// RANGEDC replaced by ONLY while impl. finalzed [DF 2001-04-20];
	savealter = p_savealter;
	alterstring = NULL;
	altervalue = NULL;

	if (copyalter)
	{
		if (copyalter->getTPAlterString())
			alterstring = TagParameterString::cast(copyalter->getTPAlterString()->getCopy());
		if (copyalter->getTPAlterValue())
			altervalue = TagParameterFloat::cast(copyalter->getTPAlterValue()->getCopy());
	}
	else altervalue = new TagParameterFloat(0);
}

ARAlter::~ARAlter()
{ 
	delete altervalue;
	delete alterstring;
}

void ARAlter::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail(( "F,detune,0.0,r;S,text,,o"));
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			delete altervalue;
			altervalue = TagParameterFloat::cast(rtpl->RemoveHead());
			assert(altervalue);

			delete alterstring;
			alterstring = TagParameterString::cast(rtpl->RemoveHead());
			assert(alterstring);
	
			if (alterstring->TagIsNotSet())
			{
				// then we can set it to the value ...
				char tmp[10];
				snprintf(tmp, 10, "%.2f", altervalue->getValue());
				alterstring->setValue(tmp);
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

std::string ARAlter::getAlterText() const
{ 
	return alterstring ? alterstring->getValue() : "";
}

float ARAlter::getDetune() const
{ 
	return altervalue ? altervalue->getValue() : 0;
}

void ARAlter::print() const
{
	PrintName(cout);
	PrintParameters(cout);
	cout << endl;
}

void ARAlter::PrintName(std::ostream & os) const
{
	os << "\\alter";
}
void ARAlter::PrintParameters(std::ostream & os) const
{
	if (altervalue)
	{		
		os << "<detune=" << altervalue->getValue();
		if (alterstring && alterstring->TagIsSet())
			os << ",";
		else  
			os << ">";
	}
	if (alterstring && alterstring->TagIsSet())
	{
		assert(altervalue && altervalue->TagIsSet());
		os << "text=\"" << alterstring->getValue() << "\">";
	}
}

/* ostream & ARAlter::operator<<(ostream &os) const
{
	os << "\\alter<" << alterv << ">";
	if (getRange())
		os << "(";
	return os << " ";
} */

