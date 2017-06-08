/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRShowVisitor.h"

#include "GRRange.h"
#include "GRText.h"
#include "GRSlur.h"
#include "GRDynamics.h"
#include "GRIntens.h"
#include "GRArticulation.h"

using namespace std;

//-------------------------------------------------------------------------------
void GRShowVisitor::visitStart (GRRange* o)			{ if (fTarget == kArticulations) o->Show(fState); }
void GRShowVisitor::visitStart (GRSlur* o)			{ if (fTarget == kSlur) o->Show(fState); }
void GRShowVisitor::visitStart (GRDynamics* o)		{ if (fTarget == kDynamics) o->Show(fState); }
void GRShowVisitor::visitStart (GRIntens* o)		{ if (fTarget == kDynamics) o->Show(fState); }
void GRShowVisitor::visitStart (GRArticulation* o)	{ if (fTarget == kArticulations) o->Show(fState); }

void GRShowVisitor::visitStart (GRText* o)
{
	if (fTarget == kText) o->Show(fState);
	else if (fTarget == kLyrics) o->Show(fState);
}
