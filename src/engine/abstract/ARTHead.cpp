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
#include "ARTHead.h"
#include "TagParameterList.h"
#include "ListOfStrings.h"

ListOfTPLs ARTHead::ltpls(1);

ARTHead::ARTHead(int st , ARTHead * p_savehead, ARTHead * p_copyhead )
	: ARMTParameter(-1,p_copyhead)
{
	rangesetting = RANGEDC;
	headstate = (ARTHead::HEADSTATE) st;
	savehead = p_savehead;

	if (p_copyhead)
	{
		headstate = p_copyhead->getHeadState();
	}
}

ARTHead::ARTHead(const ARTHead & arthead)
{
	headstate = arthead.headstate;
	
	savehead = NULL;
}

ARMusicalObject *ARTHead::Copy() const
{
	return new ARTHead(*this);
}

std::ostream & ARTHead::operator<<(std::ostream & os) const
{
	switch (headstate) {
		case NORMAL:
		case NOTSET:	os << "\\headsNormal";		break;
		case REVERSE: 	os << "\\headsReverse";		break;
		case CENTER: 	os << "\\headsCenter";		break;
		case RIGHT: 	os << "\\headsRight";		break;
		case LEFT: 		os << "\\headsLeft";		break;
	}

	if (getRange())
		os << "(";

	return os << " ";
}

void ARTHead::print(std::ostream& os) const
{
    os << "ARTHead: state: ";
    
	switch (headstate) {
		case NORMAL:
		case NOTSET:	os << "normal";  break;
		case REVERSE: 	os << "reverse"; break;
		case CENTER: 	os << "center";  break;
		case RIGHT: 	os << "right";   break;
		case LEFT: 		os << "left";    break;
	}

    os << ";" << std::endl;
}

