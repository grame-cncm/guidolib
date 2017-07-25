#ifndef ARMerge_H
#define ARMerge_H

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

#include "ARMusicalTag.h"
#include "ARPositionTag.h"

/** \brief not yet documented
*/
class ARMerge : public ARMusicalTag, public ARPositionTag
{
	public:
					 ARMerge()  { rangesetting = ONLY; }
		virtual	    ~ARMerge()	{ }

		virtual void setError(int i = 1)
		{
			ARMusicalTag::setError(i);
			if (i!=0)
			{
				setAssociation(ARMusicalTag::ER);
				ARPositionTag * pt = getCorrespondence();
				if (pt) {
					ARMusicalTag * armt = dynamic_cast<ARMusicalTag *>(pt);
					if (armt) armt->setAssociation(ARMusicalTag::EL);
				}
			}
		}

		virtual const char*	getParamsStr() const	{ return ""; };
		virtual const char*	getTagName() const		{ return "ARMerge"; };
		virtual std::string getGMNName() const		{ return "\\merge"; };
};

#endif
