/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARTHead.h"

using namespace std;

ARTHead::ARTHead(int st , const ARTHead * p_savehead, const ARTHead * p_copyhead )
	: ARMTParameter(-1,p_copyhead)
{
	rangesetting = RANGEDC;
	fHeadState = (ARTHead::HEADSTATE) st;
	fSavedHead = p_savehead;

	if (p_copyhead)
		fHeadState = p_copyhead->getHeadState();
}

ARTHead::ARTHead(const ARTHead & arthead)
{
	fHeadState = arthead.fHeadState;
	fSavedHead = NULL;
}

string ARTHead::getGMNName() const
{
	string state = "\\heads";
	switch (fHeadState) {
		case NORMAL:	state += "Normal";
			break;
		case REVERSE:	state += "Reverse";
			break;
		case CENTER:	state += "Center";
			break;
		case RIGHT:		state += "Right";
			break;
		case LEFT:		state += "Left";
			break;
		case NOTSET:
			break;
	}
	return state;
}

