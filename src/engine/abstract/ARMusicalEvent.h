#ifndef ARMusicalEvent_H
#define ARMusicalEvent_H

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
             ARMusicalEvent(const TYPE_TIMEPOSITION & relativeTimePositionOfEvent, const TYPE_DURATION & durationOfEvent);
             ARMusicalEvent(const TYPE_DURATION & durationOfEvent);
             ARMusicalEvent(const ARMusicalEvent &armev);
    virtual	~ARMusicalEvent();

    virtual bool	CanBeMerged(const ARMusicalEvent * ev2);
            void    setPointsNoDurationChange( int pointCount );

    virtual ARMusicalObject * Copy() const;

    void	setDenominator(int newDenominator);
    void	setNumerator(int newNumerator);

    //	Setzt die Anzahl der Punktierungen und passt duration
    //	und fraction entsprechend an...

    void setPoints( int pointCount );
    int	 getPoints() const;	// duration dots

    virtual bool isEventClass() const { return true; }

    static ARMusicalEvent * cast( ARMusicalObject * inObj ) { return (inObj->isEventClass() ? static_cast<ARMusicalEvent *>(inObj) : 0 ); }

            void print(std::ostream& os) const;
    virtual void printName(std::ostream& os) const       { os << "printName() needs to be implemented in subclasses; "; }
    virtual void printGMNName(std::ostream& os) const    { os << "printGMNName() needs to be implemented in subclasses; "; }
    virtual void printParameters(std::ostream& os) const { os << "printParameters() needs to be implemented in subclasses; "; }

    virtual void forceNoteAppearance(NVstring noteAppearance) {};

protected:
    int mPoints;
};

#endif
