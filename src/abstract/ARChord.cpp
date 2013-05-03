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

#include <iostream>
#include "ARChord.h"

using namespace std;

ARChord::ARChord() : ObjectList(1) // ownselements!
{ 
  relativeTimePosition = DURATION_0;
}

ARChord::ARChord(const ARChord & archord)
: ARMusicalEvent( (const ARMusicalEvent &) archord),
  ObjectList(1) // ownselements
{
}

ARChord::~ARChord()
{
}

ARMusicalObject * ARChord::Copy() const
{
	ARChord * tmp = new ARChord(*this);

	// now we make a DEEP copy of the elements ...
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		ARMusicalObject * o = GetNext(pos);
		tmp->AddTail(o->Copy());
	}

	return tmp;
}

void ARChord::print() const
{
  	cout << "ARChord" << endl;
}

std::ostream & ARChord::operator<<(std::ostream & os) const
{
	GuidoPos pos = GetHeadPosition();
	int first = 1;
	while (pos)
		{
		if (first)
			{
			first = 0;
			os << " { ";
			}
		  else
			{
			os << ",";
			}
		GetNext(pos)->operator<<(os);
		}
	return os << " } ";
}

GuidoPos ARChord::AddTail(ARMusicalObject * newMusicalObject)
{
	assert(newMusicalObject);

	newMusicalObject->setRelativeTimePosition(relativeTimePosition);
	if (duration < newMusicalObject->getDuration() )
		duration = newMusicalObject->getDuration();
	return ObjectList::AddTail(newMusicalObject);
}


void ARChord::setRelativeTimePosition(const TYPE_TIMEPOSITION  & nrtp)
{
	ARMusicalEvent::setRelativeTimePosition(nrtp);
	// now set the timeposition of each of the elements ...
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		ARMusicalObject *o = GetNext(pos);
		o->setRelativeTimePosition(nrtp);
	}
}
void ARChord::setRelativeEndTimePosition(const TYPE_TIMEPOSITION & )
{
	// not implemented yet ....
	assert(false);
}

void ARChord::setDuration(const TYPE_DURATION & dur)
{
	ARMusicalEvent::setDuration(dur);
	// now set the duration of each one of the elements ...
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		ARMusicalObject * o = GetNext(pos);
		o->setDuration(dur);
	}
}

bool ARChord::CanBeMerged(const ARMusicalEvent * ev2)
{
	const ARChord * chord2 = dynamic_cast<const ARChord *>(ev2);
	if (chord2 == 0) return false;

	GuidoPos pos = GetHeadPosition();
	GuidoPos pos2 = chord2->GetHeadPosition();
	while (pos && pos2)
	{
		ARMusicalEvent * ev1 = ARMusicalEvent::cast(GetNext(pos));
		ARMusicalEvent * ev2 = ARMusicalEvent::cast(GetNext(pos2));
		
		if (!ev1->CanBeMerged(ev2))
			return false;
	}

	if ( (pos && !pos2 )  || (pos2 && !pos) )
		return false;

	return true;
}

/** \brief Removes all but the first voice.
*/
void ARChord::setTrillChord()
{
	while (GetCount() > 1)
	{
		ARMusicalObject * o = RemoveTail();
		delete o;
	}
}
