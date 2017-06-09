/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___GRShowVisitor___
#define ___GRShowVisitor___

#include <iostream>
#include "GUIDOEngine.h"
#include "GRVisitor.h"


class GRShowVisitor : public GRVisitor
{
	bool	  fState;
	GRElement fTarget;

	public:
				 GRShowVisitor(GRElement elt, bool state) : fState(state), fTarget(elt) {}
		virtual ~GRShowVisitor() {}
	
		virtual void visitStart (GRRange* o);
		virtual void visitStart (GRText* o);
		virtual void visitStart (GRSlur* o);
		virtual void visitStart (GRDynamics* o);
		virtual void visitStart (GRIntens* o);
		virtual void visitStart (GRArticulation* o);
};

#endif
