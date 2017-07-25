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

// #include "defines.h"

#include <iostream>

#include "secureio.h"
#include "ARUserChordTag.h"
#include "TagParameterString.h"
#include "TagParameterInt.h"

using namespace std;

ARUserChordTag::ARUserChordTag(const ARUserChordTag * uct)
{
#ifndef WIN32
#warning ("check if ARUserChordTag is obsolete")
#endif

cerr << "====>>>>>>>   ARUserChordTag::ARUserChordTag" << endl;
	rangesetting = ONLY;

	labels = NULL;
	labeli = NULL;
	labelistr = NULL;

	if (uct->labels)
		labels = TagParameterString::cast
		(uct->labels->getCopy());
	else if (uct->labeli) {
		labeli = TagParameterInt::cast
		(uct->labeli->getCopy());

		if (uct->labelistr)
			labelistr = new NVstring(*uct->labelistr);
		else {
			labelistr = new NVstring("intlabel");
			char buf[100];			
			snprintf(buf, 100, "%d", labeli->getValue()); 
			labelistr->append(buf);
		}
	}
}

ARUserChordTag::~ARUserChordTag()
{
	delete labels;
	delete labeli;
	delete labelistr;
}

ARMusicalObject * ARUserChordTag::Copy() const
{
	return new ARUserChordTag(this);
}

const char* ARUserChordTag::getLabelValue() const
{
	if (labels)
		return labels->getValue();

	else if (labeli && labelistr)
		return labelistr->c_str();

	return "";
}

