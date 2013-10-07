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

#include "TagParameter.h"

TagParameter::TagParameter( const TagParameter & tp )
{
	name = tp.name;
	pflag = tp.pflag;
	required = tp.required;
}

void 
TagParameter::set( const TagParameter & in )
{
	name = in.name;
	pflag = in.pflag;
	required = in.required;
}
