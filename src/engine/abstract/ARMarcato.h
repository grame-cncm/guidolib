#ifndef ARMarcato_H
#define ARMarcato_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief The marcato articulation tag
*/
class ARMarcato : public ARMTParameter, public ARPositionTag
{		
	public:			
		enum     TPosition { ABOVE, BELOW, NOTSET };

				 ARMarcato();
		virtual ~ARMarcato() {}

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARMarcatoParams; };
		virtual const char*	getTagName() const		{ return "ARMarcato"; };
		virtual std::string getGMNName() const		{ return "\\marcato"; };
		virtual void browse(TimeUnwrap& mapper) const;
	
		TPosition getMPosition() const				{ return fPosition;};

	protected:

		TPosition fPosition;
};

#endif
