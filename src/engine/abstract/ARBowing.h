#ifndef ARBowing_H
#define ARBowing_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/
class NVPoint;

// #include "ARNote.h"
// #include "ARMusicalTag.h"

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief Implement bowing abstract objects (slur, ties...)
*/
class ARBowing : public ARMTParameter, public ARPositionTag
{
  public:
			// should be used by ARFactory. It must
			// also be ensured, that begin/end are set correctly
				ARBowing();
				ARBowing(const ARBowing * bowing);

	   virtual ~ARBowing();

		const TagParameterFloat * getDX1() const	{ return dx1; }
		const TagParameterFloat * getDY1() const	{ return dy1; }
		const TagParameterFloat * getDX2() const	{ return dx2; }
		const TagParameterFloat * getDY2() const	{ return dy2; }
		const TagParameterFloat * getR3() const		{ return r3; }
		const TagParameterFloat * getH() const		{ return h; }
	
		const TagParameterString * getCurve() const	{ return mCurve; }

		bool getParSet() const	{ return mParSet; }
		virtual void setTagParameterList(TagParameterList & tlist);
		virtual TagParameterList * getTagParameterList() const;

	    virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;

		virtual void setCurve(int curve, const NVPoint & p1, const NVPoint & p2);

  protected:
	  TagParameterFloat * dx1;	// offset to the first control point
	  TagParameterFloat * dy1;
	  TagParameterFloat * dx2;	// offset to the last control point
	  TagParameterFloat * dy2;
	  TagParameterFloat * r3;	// factor defining the position of the middle control point (default is 0.5).
	  TagParameterFloat * h;	// y offset to the middle control point.
	  TagParameterString * mCurve;	// <curve= "down" or "up">

	  bool mParSet;	// true if at least one of the 6 params has been set.

	  static ListOfTPLs ltpls;
};

#endif
