
#pragma once

/*
  GUIDO Library
  Copyright (C) 2019 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include "ARFontAble.h"
#include "ARPositionTag.h"
#include "TagParameterString.h"

/** \brief the base class for accelerando and ritardando
*/

class TempoChange : public ARFontAble,  public ARPositionTag
{		
	public:			
						 TempoChange() {}
		virtual 		~TempoChange() {}

		const char * getBefore() const	{ return getParameter<TagParameterString>(kBeforeStr, true)->getValue(); }
		const char * getAfter() const	{ return getParameter<TagParameterString>(kAfterStr, true)->getValue();}
};
