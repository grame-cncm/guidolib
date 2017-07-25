#ifndef ARArticulation_H
#define ARArticulation_H

/*
  GUIDO Library
  Copyright (C) 2016 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief not yet documented
*/
class ARArticulation : public ARMTParameter,  public ARPositionTag
{		
	public:
		enum { kDefaultPosition, kAbove, kBelow };

				 ARArticulation();
		virtual ~ARArticulation()	{ /*delete TagParameterPointer ...*/ };

		virtual void	setTagParameters (const TagParameterMap& params);
		virtual void	browse(TimeUnwrap& mapper) const;
		virtual int		getArticulationPosition () const	{ return fPosition; }

		virtual const char*	getParamsStr() const	{ return kARArticulationParams; };
		virtual const char*	getTagName () const		{ return "ARArticulation"; };
		virtual std::string getGMNName () const		{ return "\\articulation"; };

	protected:
		int fPosition;
};

#endif
