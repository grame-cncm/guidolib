#ifndef GRBreathMark_H
#define GRBreathMark_H

/*
  GUIDO Library
  Copyright (C) 2004 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "GRTagARNotationElement.h"

class ARBreathMark;


/** \brief A Breath mark graphical element.
*/



class GRBreathMark : public GRTagARNotationElement
{
	public:

						GRBreathMark( ARBreathMark * ar );
		virtual			~GRBreathMark();
		
		virtual void	OnDraw( VGDevice & hdc ) const;

		
		virtual const	NVPoint & getReferencePosition() const
			{ return refpos; }

	protected:

		static unsigned int sBreathMarkSymbol; 
		static NVPoint refpos;
};

#endif
