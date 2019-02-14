#ifndef GRDynamics_H
#define GRDynamics_H

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

#include <map>
#include <vector>

#include "GRPTagARNotationElement.h"
#include "GRVisitor.h"

class ARDynamic;
class GRStaff;


class GRDynamics : public GRPTagARNotationElement
{
	public:
					 GRDynamics(GRStaff * grstaff, const ARDynamic* ar);
		virtual		~GRDynamics();

		virtual void    addAssociation	(GRNotationElement * el);
		virtual void    tellPosition	(GObject * caller, const NVPoint & newPosition);
		virtual void	DrawDynamic		(VGDevice & hdc, bool cresc) const;
		virtual void	accept			(GRVisitor& visitor);

	protected:
		typedef struct TXSegment {
			float fx1, fx2, fy;
			TXSegment() : fx1(0.f), fx2(0.f), fy(0.f) {}
		} TXSegment;
		std::map<const GRSystem*, TXSegment>	fXPoints;
		TXSegment		fCurrentSegment;
		float			fWidth;
		float			fThickness;

		const TXSegment*	getSegment(const GRSystem*) const;
		bool				empty(const TXSegment& seg) const	{ return seg.fx1 == seg.fx2; }
		void				clear(TXSegment& seg) const			{ seg.fx1 = seg.fx2 = 0.f; }

	private:
		const GRNotationElement * getNextEvent (const GRStaff* staff, const GRNotationElement * elt) const;
		const GRNotationElement * fNext;		// next event or bar, used in case of single note dynamic

	static std::map<const std::string, unsigned int>	fDynamic2Symbol;
};

#endif
