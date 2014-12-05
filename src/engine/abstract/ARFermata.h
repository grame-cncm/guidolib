#ifndef ARFermata_H
#define ARFermata_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/* \brief Class ARFermata : abstract representation of a fermata. 

	A fermata is linked to an object (then event is set) or it is 
	between two objects and has no duration.
*/

class ARFermata : public ARMTParameter, public ARPositionTag
{
public:
	enum			 tTypeFermata {SHORT, REGULAR, LONG};
	enum			 tPositionFermata {ABOVE, BELOW};

					 ARFermata();
	virtual 		 ~ARFermata();
	virtual void 	 setTagParameterList(TagParameterList & theTagParameterList);

	virtual void     printName(std::ostream& os) const;
    virtual void     printGMNName(std::ostream& os) const;
	virtual void     printParameters(std::ostream& os) const;

	virtual void	 browse(TimeUnwrap& mapper) const;

	tTypeFermata	 getType() const {return type;};
	tPositionFermata getPositionFermata() const {return position;};

protected:

	static ListOfTPLs ltpls;
	tTypeFermata type;
	tPositionFermata position;
};

#endif
