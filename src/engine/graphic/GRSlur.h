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
#include "GRVisitor.h"

/** \brief Graphical representation of a slur.
*/
class GRSlur : public GRBowing
{
	public:
	 		  GRSlur (GRStaff * grstaff)					: GRBowing(grstaff) {}
	 		  GRSlur (GRStaff * grstaff, const ARSlur * inAR )	: GRBowing(grstaff, inAR ) {}
	 virtual ~GRSlur() {}

		virtual void	tellPositionEnd(GRNotationElement * caller);
		virtual void 	OnDraw( VGDevice & hdc ) const;

		GRNotationElement*	nextDeferred()		{ GRNotationElement* elt= fDeferredTellPostion.front(); fDeferredTellPostion.pop(); return elt; }
		size_t				countDeferred()		{ return fDeferredTellPostion.size(); }

	protected:
		virtual void automaticCurveDirection( GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse );
		virtual void automaticAnchorPoints	( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse );
		virtual void automaticControlPoints	( const GRBowingContext * context, const ARBowing * arBow, GRSystemStartEndStruct * sse );
		virtual void accept (GRVisitor& visitor);

		virtual GRSlur *	isGRSlur() 			{ return this; }

	private:
		float	getEltOffset 	(const GRNotationElement* el ) const;
		NVRect	getElementBox 	(const GRBowingContext * context, const GRNotationElement* el ) const;
};

#endif
