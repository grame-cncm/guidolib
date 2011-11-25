#ifndef __TimeMapper__
#define __TimeMapper__

/*
	GUIDO Library
	Copyright (C) 2009  Grame

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

#include "TimeUnwrap.h"

#include <map>

class ARMusicalObject;
class ARMusicalVoice;
//------------------------------------------------------------------------------
// mapping collector abstract class definition
class TimeMapper : public TimeUnwrap
{
	private:
		TimeMapCollector&	fCollector;		// the final mapping consumer
		TYPE_TIMEPOSITION	fUPosition;		// the current unrolled time position

	public:
				 TimeMapper(TimeMapCollector& f, ARMusicalVoice* voice) 
												: TimeUnwrap(voice), fCollector(f)  {}
		virtual ~TimeMapper() {}

	protected:
		virtual void		Note (const ARMusicalObject * ev);
		virtual void		Rest (const ARMusicalObject * ev);

	private:
		GuidoDate	UnrolledDate() const;
		void		NewEv (const ARMusicalObject * ev);
		void		MoveUnrolledDate(TYPE_DURATION dur);
		bool		FlushMap (const ARMusicalObject * ev);
};


#endif
