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

#include "ARArticulation.h"

/*@mkdoc

@group:Articulations

@tagname:\fermata
@tagalias:
@tagtype:RP
@tagnotation:fermata signs
@tagdesc
@tagend

@params:
@param:type:string:short, regular or long:regular:true
@param:position:string:above or below:above:true
@paramdesc
See the [Articulations](/examples/articulations/) example.
@paramend

*/

/* \brief Class ARFermata : abstract representation of a fermata. 

	A fermata is linked to an object (then event is set) or it is 
	between two objects and has no duration.
*/

class ARFermata : public ARArticulation
{
	public:
		enum			 tTypeFermata {SHORT, REGULAR, LONG};

						  ARFermata();
		virtual 		 ~ARFermata() {}

		virtual void	 setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARFermataParams; };
		virtual const char*	getTagName() const		{ return "ARFermata"; };
		virtual std::string getGMNName() const		{ return "\\fermata"; };

		virtual void	 browse(TimeUnwrap& mapper) const;
		tTypeFermata	 getType() const {return fType;};

	protected:
		tTypeFermata fType;
};

#endif
