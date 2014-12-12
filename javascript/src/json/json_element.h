/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef json_element__
#define json_element__

#include <string>
#include "json_value.h"
#include "json_stream.h"
#include "export.h"

namespace json
{

class osc_stream;
// --------------------------------------------------------------
/*!
	\brief a json element, actually a pair name/value
*/
class jsonexport json_element
{
	std::string			fName;
	const json_value *	fValue;
	
	public:
				 json_element(const char* name, const json_value* val=0) : fName(name), fValue(val) {}
		virtual ~json_element() { delete fValue; }

		const std::string&	name() const				{ return fName; }
		const json_value *	value() const				{ return fValue; }

		virtual void	print(json_stream& out) const	{ out << '"' << fName << "\": " << *fValue; }
};

/// \private
inline json_stream& operator<< (json_stream& os, const json_element& elt)	{ elt.print(os); return os; }

} // end namespoace

#endif
