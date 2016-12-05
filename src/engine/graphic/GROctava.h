#ifndef GROctava_H
#define GROctava_H

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

#include "TagParameterString.h"
#include "GRARNotationElement.h"
#include "GRPositionTag.h"

class NVstring;
class GRStaff;
class NVPoint;
class VGDevice;
class AROctava;

/** \brief not yet documented
*/
class GROctava : public GRARNotationElement, public GRPositionTag
{
	public:
					 GROctava( GRStaff *, const NVstring & txt, AROctava* ar, bool bassa );
		virtual 	~GROctava();

		virtual void tellPosition(GObject *caller, const NVPoint & );
//		virtual void addAssociation(GRNotationElement *el);
		virtual void GGSOutput() const;
		virtual void OnDraw( VGDevice & hdc ) const;
		virtual void setColRef(const TagParameterString *tps);
		virtual bool DeleteStaff(GRStaff * grstaff);
	
	private:
		int		countSegments();
		NVRect	getExtensionLine (const NEPointerList * assoc, int num) const;

		GRStaff *	fStaff;
		NVstring 	fText;
		bool		fBassa;
		float		fTextHeight;
		int			fSegmentsCount;
};

#endif

