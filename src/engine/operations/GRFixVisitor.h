/*
  GUIDO Library
  Copyright (C) 2020 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#pragma once

#include "GRVisitor.h"

class GRSingleNote;
class GRSlur;

/*
	A visitor intended to fix tellPosition order issues
 */
class GRFixVisitor : public GRVisitor
{
	public:
				 GRFixVisitor() {}
		virtual ~GRFixVisitor() {}

		virtual bool voiceMode () 						{ return true; }
		virtual void visitStart (GRSingleNote* note);
		virtual void visitStart (GRSlur* slur);
};
