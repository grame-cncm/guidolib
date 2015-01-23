/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef json_array__
#define json_array__

#include <vector>
#include "export.h"
#include "json_printable.h"

namespace json
{
 
class json_stream;
class json_value;
class osc_stream;
// --------------------------------------------------------------
/*!
	\brief a json array, as defined by the json spec (see http://json.org/)
*/
class jsonexport json_array : public json_printable
{
	std::vector<const json_value *>	fValues;
	
	public:
				 json_array() {}
		virtual ~json_array();

		const std::vector<const json_value *>&	values() const	{ return fValues; }
		virtual void	add(const json_value * val)				{ fValues.push_back(val); }
		virtual void	print(json_stream& out) const;
#ifndef JSON_ONLY
		virtual void	print(osc_stream& out) const;
#endif
};

} // end namespace

#endif
