#ifndef GRSlur_H
#define GRSlur_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARSlur.h"
#include "GRBowing.h"

/** \brief Graphical representation of a slur.
*/
class GRSlur : public GRBowing
{
	public:
	 		  GRSlur (GRStaff * grstaff)					: GRBowing(grstaff) {}
	 		  GRSlur (GRStaff * grstaff, ARSlur * inAR )	: GRBowing(grstaff, inAR ) {}
	 virtual ~GRSlur() {}

	protected:
		virtual void automaticCurveDirection( GRBowingContext * context, ARBowing * arBow, GRSystemStartEndStruct * sse );
		virtual void automaticAnchorPoints	( GRBowingContext * context, ARBowing * arBow, GRSystemStartEndStruct * sse );
		virtual void automaticControlPoints	( GRBowingContext * context, ARBowing * arBow, GRSystemStartEndStruct * sse );

	private:
		float	getEltOffset (const GRNotationElement* el ) const;
};

#endif
