/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___GRPrintVisitor___
#define ___GRPrintVisitor___

#include <iostream>
#include "GRVisitor.h"


class GRPrintVisitor : public GRVisitor
{
	std::ostream& fOut;

	public:
				 GRPrintVisitor(std::ostream& out) : fOut(out) {}
		virtual ~GRPrintVisitor() {}
	
		virtual void visitStart (GRNotationElement* o);
		virtual void visitEnd   (GRNotationElement* o);

		virtual void visitStart (GRMusic* o);
		virtual void visitEnd   (GRMusic* o);

		virtual void visitStart (GRPage* o);
		virtual void visitEnd   (GRPage* o);

		virtual void visitStart (GRSystem* o);
		virtual void visitEnd   (GRSystem* o);

		virtual void visitStart (GRStaff* o);
		virtual void visitEnd   (GRStaff* o);

		virtual void visitStart (GRSingleNote* o);
		virtual void visitEnd   (GRSingleNote* o);

		virtual void visitStart (GRBeam* o);
		virtual void visitStart (GRRange* o);
		virtual void visitStart (GRText* o);
		virtual void visitStart (GREmpty* o);
		virtual void visitStart (GRClef* o);
		virtual void visitStart (GRKey* o);
		virtual void visitStart (GRSlur* o);
		virtual void visitStart (GRDynamics* o);
		virtual void visitStart (GRIntens* o);
		virtual void visitStart (GRMeter* o);
		virtual void visitStart (GRArticulation* o);
		virtual void visitStart (GRBar* o);
		virtual void visitStart (GRSingleRest* o);
};

#endif
