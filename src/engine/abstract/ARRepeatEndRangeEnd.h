#ifndef ARRepeatEndRangeEnd_H
#define ARRepeatEndRangeEnd_H

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


#include "ARMTParameter.h"
// #include "ARMusicalObject.h"

class ARRepeatEnd;
class ARFactory;
class TagParameterList;

/** \brief not yet documented
*/
class ARRepeatEndRangeEnd :  public ARMTParameter // ,public ARMusicalObject
{
	public:

		friend class ARRepeatEnd;
		friend class ARFactory;

				ARRepeatEndRangeEnd( ARRepeatEnd * in );

		virtual ~ARRepeatEndRangeEnd() { }

		int getNumRepeat() const;

		virtual void setTagParameterList(TagParameterList & pl);

		virtual void print(int &indent) const {};

		virtual std::ostream & operator<<( std::ostream & os ) const
		{
			return os;
		}

	protected:

		ARRepeatEnd * mRepeatEnd;
};

#endif
