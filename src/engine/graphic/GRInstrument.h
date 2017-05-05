#ifndef GRInstrument_H
#define GRInstrument_H

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

#include "GRTagARNotationElement.h"

class ARInstrument;
class GRStaff;

/** \brief not yet documented
*/
class GRInstrument : public GRTagARNotationElement
{
	public:

			 GRInstrument(ARInstrument * par, GRStaff* staff);
	virtual ~GRInstrument();
	
	const ARInstrument* getARInstrument() const;
	virtual void OnDraw(VGDevice & hdc ) const;
	virtual const NVPoint & getReferencePosition() const;
	
	private:
		std::string		fFont;
		std::string		fFontAttributes;
		unsigned int	fTextFormat;
		float			fSize;
		NVPoint			fRefPos;
};

#endif
