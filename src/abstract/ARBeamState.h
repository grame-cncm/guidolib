#ifndef ARBeamState_H
#define ARBeamState_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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

		virtual void print() const { }

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

  protected:

		beamstate state;
};


#endif

