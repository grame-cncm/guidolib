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

#include "ARStaff.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "TagParameterStrings.h"


ARStaff::ARStaff(const TYPE_TIMEPOSITION & timeposition)
	: ARMTParameter(timeposition) /*, idi(0), ids(0)*/	{ init(); }

ARStaff::ARStaff(int p_staffnumber) // : idi(0), ids(0)
{
	init();
	STagParameterPtr tpi( new TagParameterInt(p_staffnumber) );
	tpi->setName(kIDStr);
	tpi->setIsAuto();
	addTagParameter(tpi);
}

ARStaff::ARStaff()	{ init(); }

ARStaff::ARStaff(const ARStaff *stff)
		: ARMTParameter(-1,stff) // , idi(0), ids(0)
{
	init();
	copyParameters(stff->getTagParameters());
	setTagParameters(stff->getTagParameters());
}

void ARStaff::init()
{
	setupTagParameters (gMaps->sARStaffMap);
}

const char* ARStaff::getStaffID() const
{
	const TagParameterString* param = getParameter<TagParameterString>(kIDStr);
	return param ? param->getValue() : 0;
}

/** \brief Returns the number if ID, returns -1 if string
*/
int ARStaff::getStaffNumber() const
{ 
	const TagParameterInt* param = getParameter<TagParameterInt>(kIDStr);
	if (param) return param->getValue();
	if (getParameter<TagParameterString>(kIDStr)) return -1;
  	return 0;
}
	

