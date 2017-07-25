#ifndef ARBowing_H
#define ARBowing_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief Implement bowing abstract objects (slur, ties...)
*/
class ARBowing : public ARMTParameter, public ARPositionTag
{
	public:
		enum CurveDirection { kUndefined, kUp, kDown, kPUndefined=9999 };

				ARBowing();
				ARBowing(const ARBowing * bowing);
	   virtual ~ARBowing() {}

		static float undefined() { return float(kPUndefined); }

		float getDX1() const	{ return fDx1; }
		float getDX2() const	{ return fDx2; }
		float getDY1() const	{ return fDy1; }
		float getDY2() const	{ return fDy2; }
		float getR3() const		{ return fCtrlPoint; }
		float getH() const		{ return fCtrlPointOffset; }
		bool getParSet() const	{ return fParSet; }
		CurveDirection getCurve() const	{ return fCurveDir; }

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARBowingParams; };
		virtual const char*	getTagName() const		{ return "ARBowing"; };
		virtual std::string getGMNName() const		{ return "\\bowing"; };

  private:
	  float fDx1, fDy1;			// offset to the first control point
	  float fDx2, fDy2;			// offset to the last control point
	  float fCtrlPoint;			// factor defining the position of the middle control point (default is 0.5).
	  float fCtrlPointOffset;	// y offset to the middle control point (expressed in hs).
	  CurveDirection fCurveDir;

	  bool fParSet;				// true if at least one of the 6 params has been set.
};

#endif
