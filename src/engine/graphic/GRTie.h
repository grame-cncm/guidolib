#ifndef GRTie_H
#define GRTie_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#include "GRBowing.h"

class ARTie;
class GRStaff;

/** \brief Graphical representation of a tie.
*/
class GRTie : public GRBowing
{
	public:
				 GRTie(GRStaff * grstaff, const ARTie * ar, bool hideAccidentals) : GRBowing(grstaff, ar), fHideAccidentals(hideAccidentals) {}
		virtual	~GRTie() {}

		virtual void addAssociation(GRNotationElement *el);

	protected:
		virtual void automaticAnchorPoints( GRBowingContext * bowContext, const ARBowing * arBow, GRSystemStartEndStruct * sse );
		virtual void automaticControlPoints( GRBowingContext * bowContext, const ARBowing * arBow, GRSystemStartEndStruct * sse );

	private:
		bool	fHideAccidentals;		// used to hide accidentals after a bar
		bool 	fAssoc = false;			// becomes true at first association
		int	 	fPitch = -1;			// used for chords associations
};

#endif
