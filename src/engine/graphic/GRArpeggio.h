#ifndef GRArpeggio_H
#define GRArpeggio_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2018 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <vector>

#include "GRPTagARNotationElement.h"
#include "NVRect.h"


class ARArpeggio;
class GRStaff;

/** \brief The Arpeggio notation element
*/
class GRArpeggio : public GRPTagARNotationElement
{
	public:
						 GRArpeggio( GRStaff * stf, const ARArpeggio * artrem );
		virtual			~GRArpeggio() {}

		virtual void	OnDraw( VGDevice & hdc) const;
		virtual void	tellPosition( GObject * caller,const NVPoint & np );

	private:
		float fDx, fDy, fLSpace;
		std::vector<NVRect> fPos;	// list of the chords bounding boxes

		void 	DrawUpArrow 	(VGDevice & hdc, float x, float y) const;
		void 	DrawDownArrow 	(VGDevice & hdc, float x, float y) const;
};

#endif
