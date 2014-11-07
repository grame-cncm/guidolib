#ifndef ARRangeEnd_H
#define ARRangeEnd_H

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
#include "ARMusicalObject.h"
#include "ARMusicalTag.h"

/** \brief Abstract representation of a range.
*/
class ARRangeEnd : public ARMusicalObject
{
	public:
		 
		 		ARRangeEnd(ARMusicalTag * ptag) : tagref(ptag) { }
	virtual ARMusicalTag * getTagRef();
   	virtual std::ostream &operator<<(std::ostream & os) const;


protected:
	ARMusicalTag * tagref;
};

#endif
