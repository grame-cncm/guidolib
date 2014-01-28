#ifndef ARRepeatBegin_H
#define ARRepeatBegin_H

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

//#include "ARMTParameter.h"
// #include "ARMusicalObject.h"
#include "ARBar.h"

class ARRepeatEnd;
class ARFactory;

/** \brief The right facing repeat sign.
*/
class ARRepeatBegin : public ARMTParameter	// ,public ARMusicalObject
//class ARRepeatBegin : public ARBar
{
	public:

		friend class ARRepeatEnd;
		friend class ARFactory;

					 ARRepeatBegin() : numrepeat(0) { dur.setNumerator(-1); }
		virtual		~ARRepeatBegin()	{ }

		virtual void	setTagParameterList(TagParameterList & tpl);

		virtual void	print() const { }
		virtual void	browse(TimeUnwrap& mapper) const;

		virtual std::ostream & operator << ( std::ostream & os ) const;
		virtual void	setRepeatEnd( const ARRepeatEnd * );
	
	protected:

		static ListOfTPLs ltpls;
		int numrepeat;
		TYPE_DURATION dur;
};

#endif
