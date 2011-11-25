/*
 *  ARHarmonic.cpp
 *  GuidoSceneComposer
 *
 *  Created by Samuel Brochot on 30/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 
 
#include <iostream>
#include "ARHarmonic.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARHarmonic::ltpls(1);

void ARHarmonic::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		//creates a list of strings
		ListOfStrings lstrs;
		lstrs.AddTail((""));
		CreateListOfTPLs(ltpls, lstrs);
	}
	
	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls, tpl, &rtpl);
	
	if (ret>=0 && rtpl) // we found a match =
	{
		if (ret == 0)
		{
			//...
		}

		delete rtpl;
	}
	else
	{
		//failure
	}
	tpl.RemoveAll();
}

void ARHarmonic::print() const
{
}

void ARHarmonic::PrintName(std::ostream &os) const
{
	os << "\\marcato";
	if (getRange()) os << "(";
}

void ARHarmonic::PrintParameters(std::ostream & ) const
{
}
