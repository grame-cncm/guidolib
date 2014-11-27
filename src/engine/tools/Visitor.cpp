/*
  GUIDO Library
  Copyright (C) 2011 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "Visitor.h"

using namespace std;

//______________________________________________________________________________
// Visitor
//______________________________________________________________________________

Visitor::Visitor()
{
}

Visitor::~Visitor() 
{
}

void Visitor::visit(ARMusicalObject *object) {
}

void Visitor::visit(ARPositionTag *object) {
}
