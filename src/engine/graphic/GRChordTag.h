#ifndef GRChordTag_H
#define GRChordTag_H

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


#include "GRPTagARNotationElement.h"
#include "GRSystemTagInterface.h"

class GRStaff;
class GRSystem;
class ARUserChordTag;

/** \brief The Chord Tag
*/

class GRChordTag : 	public GRPTagARNotationElement,
					public GRSystemTagInterface
{
	public:
		GRChordTag(GRStaff * pstaff, ARUserChordTag * pchordtag);
		virtual ~GRChordTag();

		virtual void removeAssociation(GRNotationElement * grnot);

		virtual void checkPosition(const GRSystem * grsys);
	    virtual void addAssociation(GRNotationElement * grnot);
		

		const char* getLabel() const;


	virtual void RangeEnd(GRStaff * grstaff);

	virtual void tellPosition(GObject *, const NVPoint & );


};

#endif
