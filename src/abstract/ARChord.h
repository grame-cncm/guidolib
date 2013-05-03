#ifndef ARChord_H
#define ARChord_H

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

#include "ARMusicalEvent.h"
#include "ObjectList.h"
#include "kf_ilist.h"	// for ObjectList

/** \brief Abstract representation of a chord. Unused ?
*/
class ARChord : public ARMusicalEvent, public ObjectList
{
	public:
						ARChord();
						ARChord( const ARChord & archord);
		virtual	 		~ARChord();

		virtual bool 	CanBeMerged(const ARMusicalEvent * ev2);

		virtual 		ARMusicalObject * Copy() const;
		virtual void 	print() const;
     	virtual std::ostream & operator<<( std::ostream & os ) const;

		virtual GuidoPos AddTail(ARMusicalObject *);

		virtual void 	setRelativeTimePosition(const TYPE_TIMEPOSITION  & nrtp);
		virtual void 	setRelativeEndTimePosition(const TYPE_TIMEPOSITION & tp);
		virtual void 	setDuration(const TYPE_DURATION & dur);

				void	setTrillChord();
};


#endif
