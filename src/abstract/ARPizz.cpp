/*
 *  ARPizz.cpp
 *  GuidoSceneComposer
 *
 *  Created by Samuel Brochot on 26/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include <iostream>
#include "ARPizz.h"
#include "TagParameterList.h"
#include "TagParameterString.h"
#include "ListOfStrings.h"

ListOfTPLs ARPizz::ltpls(1);

void ARPizz::setTagParameterList(TagParameterList & tpl)
{
	if (ltpls.GetCount() == 0)
	{
		ListOfStrings lstrs;
		lstrs.AddTail("S,type,lefthand,o");
		CreateListOfTPLs(ltpls, lstrs);
	}
	
	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	
	if (ret>=0 && rtpl)
	{
		//we found a match !
		if (ret == 0)
		{
			TagParameterString * str = TagParameterString::cast(rtpl->RemoveHead());
			assert(str);
			if (str->TagIsSet())
			{
				std::string val(str->getValue());
				if (val == "buzz")
					type = BUZZ;
				else if (val == "snap" || val == "bartok")
					type = SNAP;
				else if (val == "fingernail")
					type = FINGERNAIL;
				else type = LEFTHAND;
			}
			else type = LEFTHAND;
			
			delete str;
		}
		
		delete rtpl;
	}

	tpl.RemoveAll();
}

void ARPizz::print() const
{
}

void ARPizz::PrintName(std::ostream &os) const
{
	os << "\\pizzicato";
	if (getRange()) os << "(";
}

void ARPizz::PrintParameters(std::ostream & ) const
{
}
