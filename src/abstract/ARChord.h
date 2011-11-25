#ifndef ARChord_H
#define ARChord_H

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
