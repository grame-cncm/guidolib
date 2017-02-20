#ifndef GRMark_H
#define GRMark_H

/*
  GUIDO Library
  Copyright (C) 2016 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>

#include "GRText.h"

class ARMark;
class GRStaff;

/** \brief A rehearsal mark .

	Handle as a specialized text to deal with enclosure

*/
class GRMark : public GRText
{	
	FloatRect	toSquare (const FloatRect& r) const;
	void		toDiamond (const FloatRect& r, VGDevice & hdc) const;
	void		toTriangle (const FloatRect& r, VGDevice & hdc) const;
	void		toEllipse (const FloatRect& r, VGDevice & hdc) const;
	void		toBracket (const FloatRect& r, VGDevice & hdc) const;

	public:
						 GRMark( GRStaff *, ARMark * abstractRepresentationOfText );
		virtual 		~GRMark();

		virtual void		OnDraw( VGDevice & hdc ) const;
		virtual FloatRect	getTextMetrics(VGDevice & hdc, const GRStaff* staff) const;

		const ARMark *	getARMark() const;

	private:
		float yoffset (const GRStaff* staff) const;
};

#endif

