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

class GRPitchYVisitor : public GRVisitor
{
	int fBasePitch;
	int fBaseLine;
	int fBaseOct;
	int fOctava;
	float fLastX;
	TYPE_TIMEPOSITION fLastDate = 0;
	TYPE_TIMEPOSITION fTargetDate = 0;
	bool fDone = false;

	public:
				 GRPitchYVisitor() {}
		virtual ~GRPitchYVisitor() {}

		NVPoint getPitchPos (GRStaff* staff, int pitch, TYPE_TIMEPOSITION date);
		
		virtual bool voiceMode () { return true; }
	
		virtual void visitStart (GRClef* o);
		virtual void visitStart (GROctava* o);
		virtual void visitStart (GRSingleNote* o);
		virtual void visitStart (GRSingleRest* o);
		virtual void visitStart (GREmpty* o);
};

#endif
