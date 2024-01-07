#ifndef GRInstrument_H
#define GRInstrument_H

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

#include "GRTagARNotationElement.h"

class ARInstrument;
class GRStaff;
class VGFont;

/** \brief not yet documented
*/
class GRInstrument : public GRTagARNotationElement
{
	public:

			 GRInstrument(const ARInstrument * par, GRStaff* staff);
	virtual ~GRInstrument() {}
	
	const ARInstrument* getARInstrument() const;
	virtual void 		OnDraw(VGDevice & hdc ) const;
	virtual void 		OnDraw(VGDevice & hdc, float y ) const;
	virtual const NVPoint & getReferencePosition() const;
	
	private:
		void getMetrics(VGDevice & hdc, float& h, float& w) const;

		const VGFont* 	fFont = 0;
		unsigned int	fTextAlign = 0;
		NVPoint			fRefPos;
		std::vector<std::string> fSubstrings;
};

#endif
