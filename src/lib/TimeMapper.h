#ifndef __TimeMapper__
#define __TimeMapper__

/*
  GUIDO Library
  Copyright (C) 2009  Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
