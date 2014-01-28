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

#include <cstdlib> 		// for atoi
#include <cstring> 		// for strncmp
#include <iostream>

#include "secureio.h"
#include "nvstring.h"

#include "ARClef.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

using namespace std;


ListOfTPLs ARClef::ltpls(1);

// Class ARClef
ARClef::ARClef( const TYPE_TIMEPOSITION & timeposition) : ARMTParameter(timeposition)	{}
ARClef::ARClef(const string& theName)  : ARMTParameter(), fName (theName)				{}

ARClef::ARClef()
{
	mClef = VIOLIN;
	mStaffLine = 2;
	fOctava = NONE;
}

ARClef::ARClef(const ARClef & clef)
{
	this->mClef = clef.mClef;
	mStaffLine = clef.mStaffLine;
	fOctava = clef.fOctava;
}

ARClef::~ARClef()		{}

void ARClef::print() const
{
}

void ARClef::PrintName(std::ostream &os) const
{
	os << "\\clef";
}

void ARClef::PrintParameters(std::ostream &os) const
{
	os << "<\"" << fName << "\"> ";
}

void ARClef::setTagParameterList(TagParameterList& tpl)
{
	if (ltpls.GetCount() == 0)
	{
		// create a list of string ...

		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail( (
			"S,type,treble,r")); // old:;F,size,1.0,o"
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = NULL;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret>=0 && rtpl)
	{
		// we found a match!
		if (ret == 0)
		{
			TagParameterString * tps =  TagParameterString::cast(rtpl->RemoveHead());
			assert(tps);
			setName(tps->getValue());
			delete tps;
		}
		delete rtpl;
	}
	else
	{
		// failure
	}
	tpl.RemoveAll();
}

void ARClef::setName(const string& theName)
{
	fName = theName;
  	fOctava = NONE;
	string clef;

  	// search for octava- Information
  	size_t pos = fName.find("-");
	if (pos == std::string::npos)
	 	pos = fName.find("+");

  	if (pos != string::npos)	// position found!
	{
		switch (atoi(&fName[pos])) {
			case -15:	fOctava = LOW15; break;
			case -8:	fOctava = LOW8; break;
			case 0:		fOctava = NONE; break;
			case 8:		fOctava = HIGH8; break;
			case 15:	fOctava = HIGH15; break;
		}
		clef = fName.substr(0, pos);
	}
	else clef = fName;

	if ((clef == "bass") || (clef == "basso") || (clef == "f4"))
	{
		mClef = BASS;
		mStaffLine = 4;
	}
	else if ((clef == "tenor") || (clef == "c4"))
	{
		mClef = BRATSCHE;
		mStaffLine = 4;
	}
	else if ((clef == "alto") || (clef == "c3"))
	{
		mClef = BRATSCHE;
		mStaffLine = 3;
	}
	else if ((clef == "violino") || (clef == "treble") || (clef == "g2"))
	{
		mClef = VIOLIN;
		mStaffLine = 2;
	}
	else if ((clef == "none") || (clef == "off") || (clef == ""))
	{
		mClef = OFF;
		mStaffLine = 2;
	}
	else
	{
		int line = 0;
		// from here search for general clefs
	 	if ( clef.substr(0,2) == "gg")		// double g-clef
		{  
			mClef = DOUBLEG;
			if(clef.size()>2)
				line = atoi(&clef[2]);
		}
		else if (clef[0] == 'f')
		{
			mClef = BASS;
			if(clef.size()>1)
				line = atoi(&clef[1]);
		}
		else if (clef[0] == 'c')
		{
			mClef = BRATSCHE;
			if(clef.size()>1)
				line = atoi(&clef[1]);
		}
	 	else if ( clef.substr(0,4) == "perc")
		{
			mClef = PERC;
			if(clef.size()>4)
				line = atoi(&clef[4]);
		}
		else if (clef[0] == 'g')
		{
			mClef = VIOLIN;
			if(clef.size()>1)
				line = atoi(&clef[1]);
		}
	  	else mClef = VIOLIN;

	  	// search for line-information
		if ( (line > 0) && (line <= 5 ) )
			mStaffLine = line;
		else {
			// depends on clef-information
			// in case of C-key the third line (Bratsche) will be the default setting
			if (mClef == BRATSCHE)		mStaffLine = 3;
			else if (mClef == BASS)		mStaffLine = 4;
			else if (mClef == PERC)		mStaffLine = 3;
			else						mStaffLine = 2;
		}
	}
}

bool ARClef::operator==(const ARClef &clef)
{
	return (mStaffLine == clef.mStaffLine) && (mClef == clef.mClef) && (fOctava == clef.fOctava);
}

bool ARClef::IsStateTag() const
{
	return true;
}
