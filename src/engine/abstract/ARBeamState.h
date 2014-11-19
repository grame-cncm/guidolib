#ifndef ARBeamState_H
#define ARBeamState_H

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

#include "ARMusicalTag.h"

// #include "ARMusicalObject.h"

/** \brief not yet documented
*/
class ARBeamState : public ARMusicalTag  
	// ,public ARMusicalObject
{
  public: 
		enum beamstate { AUTO, OFF };

						ARBeamState(beamstate p_state)
							{ state = p_state; }

		virtual			~ARBeamState() {}
		
		virtual bool IsStateTag() const { return true; }

		virtual void print(int &indent) const { }

/*		virtual ostream &operator<<(ostream &os) const
			{
				os << "\\beams";
				if (state == AUTO)
					os << "Auto";
				else if (state == OFF)
					os << "Off";
				if (getRange())
					os << "(";
				return os << " ";
			}
*/
		virtual void PrintName(std::ostream & os) const;
		virtual void PrintParameters(std::ostream &) const { }
	
			beamstate getBeamState() const { return state; }
        

    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARBeamState() { return this; }
    /****************************************/

  protected:

		beamstate state;
};


#endif

