/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __ARChord__
#define __ARChord__

#include <vector>

#include "arexport.h"
#include "guidoelement.h"
#include "guidorational.h"

namespace guido 
{

/*!
\addtogroup abstract
@{
*/

class basevisitor;
//______________________________________________________________________________
/*!
\brief A Guido chord
*/
class gar_export ARChord : public guidoelement
{ 
	public:
		static SMARTP<ARChord> create();
        virtual void	acceptIn(basevisitor& v);
        virtual void	acceptOut(basevisitor& v);
		// gives the list of notes durations
		virtual void	duration(rationals&) const;
		// gives the list of notes total durations in the given context
		virtual void	duration(rationals&, rational current, int currentdots) const;
		// gives the list of the chord midi pitch in ascending ordered
		virtual void	midiPitch(int& currentoctave, std::vector<int>& pitches) const;

		/// a chord duration is the max of its notes durations
		/// it returns 0 when all notes have implicit duration 
		/// or -max if one of the notes has an implicit duration
		rational		duration() const;
		/// gives the chord duration within the given context
		rational		totalduration(rational current, int currentdots) const;

		ARChord& operator =  (const rational& d);		///< force all notes duration to d
		ARChord& operator += (const rational& d);		///< add d to all notes duration
		ARChord& operator -= (const rational& d);		///< substract d from all notes duration
		ARChord& operator *= (const rational& d);		///< multiplies all notes duration by d
		ARChord& operator /= (const rational& d);		///< divides all notes duration by d

		bool	implicitDuration(const rational& d)		{ return d.getNumerator() <= 0; }

    protected:	
				 ARChord() {}
		virtual ~ARChord() {}
};

/*! @} */

} // namespace

#endif
