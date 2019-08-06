#ifndef ARUnits_H
#define ARUnits_H

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

#include <string>

#include "ARMTParameter.h"


/*
ignored, the tag is not used
@mkdoc

@group:Miscellaneous

@tagname:\unit
@tagalias:
@tagtype:P
@tagnotation:changes the default unit
@tagdesc
@tagend

@params:
@param:type:string:m, cm, mm, in, pt, pc, hs:cm:false
@paramdesc
- *m* is for meter
- *cm* is for centimeter
- *mm* is for millimeter
- *pt* is for point
- *pc* is for pica
- *hs* is for half space

See also the [Tag Parameters](/refs/tagsparams/#units) section.
@paramend

*/


/** \brief not yet documented
*/

class ARUnits : public ARMTParameter
{
	public:
					 ARUnits();
		virtual 	~ARUnits() {}

		static const char* getUnit()				{ return sType; }

		virtual bool IsStateTag() const				{ return true; }
		
		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARUnitsParams; };
		virtual const char*	getTagName() const		{ return "ARUnits"; };
		virtual std::string getGMNName() const		{ return "\\unit"; };

	protected:
		static const char* sType;
};

#endif 
