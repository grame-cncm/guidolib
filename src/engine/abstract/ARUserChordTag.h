#ifndef ARUserChordTag_H
#define ARUserChordTag_H

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

class TagParameterString;
class TagParameterInt;

/** \brief not yet documented
*/
class ARUserChordTag : 	public ARMTParameter, public ARPositionTag
{
	public:
				ARUserChordTag(const ARUserChordTag * uct);
				ARUserChordTag() 
				{
					rangesetting = ONLY;
					labels = NULL;
					labeli = NULL;
					labelistr = NULL;
				}
		virtual ~ARUserChordTag();

		virtual ARMusicalObject * Copy() const;

		virtual const char*	getTagName () const		{ return "ARUserChordTag"; };
		virtual std::string getGMNName () const		{ return "\\userChordTag"; };

		const char* getLabelValue() const;
		const ARMusicalObject *isARUserChordTag() const  { return this; }

	protected:
		TagParameterString *labels;
		TagParameterInt *labeli;
		NVstring * labelistr;
};


#endif
