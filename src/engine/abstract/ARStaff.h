#ifndef ARStaff_H
#define ARStaff_H

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

#include "ARMTParameter.h"
class TagParameterInt;

class TagParameterString;
class ARMusicalObject;

/** \brief not yet documented
*/
class ARStaff : // public ARMusicalObject,
 	public ARMTParameter
{
  public:
				 ARStaff(const TYPE_TIMEPOSITION & timeposition);
				 ARStaff(int p_staffnumber);
				 ARStaff();
			 	 ARStaff(const ARStaff * stf);

		virtual ~ARStaff();

		virtual ARMusicalObject * Copy() const;
	    virtual bool IsStateTag() const;

	    virtual void printName(std::ostream& os) const;
	    virtual void printGMNName(std::ostream& os) const;
	    virtual void printParameters(std::ostream& os) const;
	
		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		int			getStaffNumber() const;
		const char* getStaffID() const;
        
    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARStaff() { return this; }
    /*****************************************/

  protected:
	  TagParameterString * ids;
	  TagParameterInt * idi;

	  static ListOfTPLs ltpls;
  };


#endif

