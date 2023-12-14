/*
  GUIDO Library
  Copyright (C) 2023 D. Fober

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef ___GRPitchYVisitor___
#define ___GRPitchYVisitor___

#include "defines.h"
#include "GUIDOEngine.h"
#include "GRVisitor.h"
#include "NVPoint.h"

class GRNotationElement;

class GRPitchYVisitor : public GRVisitor
{
	int fTargetStaff;
	int fCurrentStaff;
	int fBasePitch;
	int fBaseLine;
	int fBaseOct;
	int fOctava;
	float fNextX;
	TYPE_TIMEPOSITION fTargetDate = 0;
	const GRNotationElement* fTargetElt;
	bool fDone = false;
	const GRStaff * fStaff;

	public:
				 GRPitchYVisitor() {}
		virtual ~GRPitchYVisitor() {}

		NVPoint getPitchPos (GRMusic* music, int staffNum, int pitch, TYPE_TIMEPOSITION date);
		
		virtual bool voiceMode () { return false; }
	
		virtual void visitStart (GRClef* o);
		virtual void visitStart (GROctava* o);
		virtual void visitStart (GRSingleNote* o);
		virtual void visitStart (GRSingleRest* o);
		virtual void visitStart (GREmpty* o);
		virtual void visitStart (GRStaff* o);
		virtual void visitStart (GRBar* o);
		virtual void visitStart (GRRepeatBegin* o);
		virtual void visitStart (GRMeter* o);
		virtual void visitStart (GRKey* o);

	private:
		void 	check (const GRNotationElement* elt);
		bool 	checkTimePos (const GRNotationElement* elt);
		float 	interpolateXPos (const GRNotationElement* elt, TYPE_TIMEPOSITION date, float nextx) const;
};

#endif
