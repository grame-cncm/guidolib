#ifndef ARMusicalEvent_H
#define ARMusicalEvent_H

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

#include "GUIDOTypes.h"
#include "ARMusicalObject.h"

class Fraction;

/** \brief Class for musical events (events have an own duration tags not)
*/

class ARMusicalEvent : public ARMusicalObject
{
	public:
    
						ARMusicalEvent() : mPoints(0) { }
						ARMusicalEvent(int theNumerator, int theDenominator);
						ARMusicalEvent(const TYPE_TIMEPOSITION & relativeTimePositionOfEvent,
									const TYPE_DURATION & durationOfEvent);

						ARMusicalEvent(const TYPE_DURATION & durationOfEvent);
						ARMusicalEvent(const ARMusicalEvent &armev);
		virtual			~ARMusicalEvent();

		virtual bool	CanBeMerged(const ARMusicalEvent * ev2);
				void	setPointsNoDurationChange( int pointCount );
 
 
		virtual ARMusicalObject * Copy() const;

		virtual void	print() const;

				void	setDenominator(int newDenominator);
				void	setNumerator(int newNumerator);

		//	Setzt die Anzahl der Punktierungen und passt duration
      //	und fraction entsprechend an...

				void setPoints( int pointCount );
		int		getPoints() const;	// duration dots

		virtual bool isEventClass() const { return true; }

	static ARMusicalEvent * cast( ARMusicalObject * inObj )
			{ return (inObj->isEventClass() ? static_cast<ARMusicalEvent *>(inObj) : 0 ); }

   protected:

  		int mPoints;
};

#endif


