#ifndef ARStaff_H
#define ARStaff_H

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

#include "ARMTParameter.h"

/*@mkdoc

@group:Layout

@tagname:\staff
@tagalias:
@tagtype:P
@tagnotation:set staff assignment
@tagdesc
@tagend

The **\staff** tag is used to write multi-voice music on the same staff.

@params:
@param:id:integer:a staff index:*none*:false
@param:dy:unit:set the distance to the next staff:*none*:false
@paramdesc
'id' indicates a staff index (starting from 1). All the elements between a **\staff** tag and the next one
(or the end of the voice) are assigned to the staff designated by id.

See the [4 voices](@EXAMPLES/4voices/) example.

@paramend

*/

/** \brief not yet documented
*/
class ARStaff : public ARMTParameter
{
  public:
				 ARStaff(const TYPE_TIMEPOSITION & timeposition);
				 ARStaff(int p_staffnumber);
				 ARStaff();
			 	 ARStaff(const ARStaff * stf);

		virtual ~ARStaff() {}

		virtual ARMusicalObject * Copy() const		{ return new ARStaff(this); }
	    virtual bool IsStateTag() const				{ return true; }

		virtual int			getOrder() const		{ return kDontMove; }
		virtual const char*	getParamsStr() const	{ return kARStaffParams; };
		virtual const char*	getTagName() const		{ return "ARStaff"; };
		virtual std::string getGMNName() const		{ return "\\staff"; };

		int			getStaffNumber() const;
		const char* getStaffID() const;
        
		const ARMusicalObject *isARStaff() const	{ return this; }

	private:
		void init();
  };


#endif

