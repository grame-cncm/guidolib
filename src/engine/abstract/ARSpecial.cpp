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
#include "secureio.h"

#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"
#include "GuidoDefs.h"

#include "ARSpecial.h"


ListOfTPLs ARSpecial::ltpls(1);

ARSpecial::ARSpecial()
{
}

ARSpecial::~ARSpecial()
{
}

void ARSpecial::setTagParameterList(TagParameterList & tpl)
{

	// character (auch code)
	// dx (offset from x-position, must be positive!?
	// dy (y-position)
	// space (the space after the symbol)
	// scale (for smaller or larger symbols)

	if (ltpls.GetCount() == 0) {
		ListOfStrings lstrs; // (1); std::vector test impl
		lstrs.AddTail("S,char,,r;F,scale,1.0,o");
		CreateListOfTPLs(ltpls,lstrs);
	}

	TagParameterList * rtpl = 0;
	int ret = MatchListOfTPLsWithTPL(ltpls,tpl,&rtpl);

	if (ret >= 0 && rtpl) {
		// we found a match!
		if (ret == 0) {
			// then, we now the match for
			// the first ParameterList
			// w, h, ml, mt, mr, mb
			GuidoPos pos = rtpl->GetHeadPosition();

			TagParameterString * tps = TagParameterString::cast(rtpl->GetNext(pos));
			assert(tps);
			
			// now we need to find out, which character
			// to draw ...
			
			const char * p = tps->getValue();
			int num;

			if (p[0] == '\\') { // it is a decimal or octal value
				if (p[1] == 'x') {
					// its a hex number
					if (sscanf(&(p[2]), "%x", &num) != 1)
						error = 1;
				}
				else if (p[1] == 'o') {  // its an octal number
					if (sscanf(&(p[2]), "%o", &num) != 1)
						error = 1;
				}
				else {
					if (sscanf(&(p[1]), "%d", &num) != 1)
						error = 1;
				}
				if (error == 0 && num > 0 && num <= 255)
					mDrawChar = (unsigned char) num;	// (JB) was (char)num
				else
					error = 1;
			}
			else
				mDrawChar = (unsigned char) p[0];
		}

		delete rtpl;
	}
	else
	{
		// failure
	}

	tpl.RemoveAll();
}

/*std::ostream & ARSpecial::operator<<(std::ostream& os) const
{
	os << "\\special<";
	os << "\"\\" << (unsigned int) mDrawChar
		<< "\"," << dx * kVirtualToCm << "cm," 
		<< dy * kVirtualToCm << "cm,"
		<< space * kVirtualToCm << "cm," << scale << "> ";
	return os;
}*/

void ARSpecial::printName(std::ostream& os) const
{
    os << "ARSpecial";
}

void ARSpecial::printGMNName(std::ostream& os) const
{
    os << "\\special";
}

void ARSpecial::printParameters(std::ostream& os) const
{
    /* TODO (cf. plus haut) */

    ARMusicalTag::printParameters(os);
}
