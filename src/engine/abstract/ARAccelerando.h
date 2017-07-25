#ifndef ARAccelerando_H
#define ARAccelerando_H

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


#include "ARFontAble.h"
#include "ARPositionTag.h"
#include "TagParameterStrings.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

class TagParameterString;

/** \brief The Accelerando position tag
*/
class ARAccelerando :  public ARFontAble,  public ARPositionTag
{		
	public:			

		virtual bool MatchEndTag(const char * s);

				 ARAccelerando();
		virtual ~ARAccelerando() {}

		virtual const char*	getParamsStr() const	{ return kARAccelerandoParams; };
		virtual const char*	getTagName () const		{ return "ARAccelerando"; };
		virtual std::string getGMNName () const		{ return "\\accelerando"; };

		const TagParameterString * getTempo() const      { return getParameter<TagParameterString>(kTempoStr, true); }
		const TagParameterString * getAbsTempo() const   { return getParameter<TagParameterString>(kAbsTempoStr, true);}
};

#endif
