/*
  GUIDO Library
  Copyright (C) 2020 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "GRFingering.h"
#include "GRFixVisitor.h"
#include "GRSingleNote.h"
#include "GRSlur.h"


//--------------------------------------------------------------------------
void GRFixVisitor::visitStart (GRSingleNote* note)
{
	NEPointerList* assoc = note->getAssociations();
	if (assoc) {
		GuidoPos pos = assoc->GetHeadPosition();
		while(pos) {
			GRFingering * el = assoc->GetNext(pos)->isGRFingering();
			if (el)
				el->tellPositionEnd (note, note->getPosition());
		}
	}
}

//--------------------------------------------------------------------------
void GRFixVisitor::visitStart (GRSlur* slur)
{
	size_t n = slur->countDeferred();
	while (n--) {
		slur->tellPositionEnd (slur->nextDeferred());
	}
}
