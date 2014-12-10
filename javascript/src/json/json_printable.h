/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef json_printable__
#define json_printable__

#include "json_stream.h"

namespace json
{
 
class jsonexport json_printable
{
  public :
		virtual ~json_printable() {}
		virtual void	print(json_stream& out) const = 0;
#ifndef JSON_ONLY
		virtual void	print(osc_stream& out) const = 0;
#endif
};

} // end namespace

#endif
