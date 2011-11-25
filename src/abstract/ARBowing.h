#ifndef ARBowing_H
#define ARBowing_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

	virtual		~ARBowing();

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
		virtual void PrintParameters(std::ostream & os) const;
		virtual void print() const;

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


