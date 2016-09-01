/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef json_object__
#define json_object__

#include <vector>
#include "json_element.h"
#include "json_printable.h"
#include "export.h"

namespace json
{

class json_stream;
class osc_stream;
// --------------------------------------------------------------
/*!
	\brief a json object, as defined by the json spec (see http://json.org/)
*/
class jsonexport json_object : public json_printable
{
	std::vector<const json_element*>	fElements;
	
	public:
				 json_object() {}
		virtual ~json_object();

		/// \brief add an element to the object
		virtual void	add(const json_element* elt)		{ fElements.push_back(elt); }

		/// \brief retrieve an element by name
		virtual const json_element* getKey (const char* name) const;
		
		/// \brief print the element to the json stream (for indented output)
		virtual void	print(json_stream& out) const;
#ifndef JSON_ONLY
		/// \brief print the element to the osc stream
		virtual void	print(osc_stream& out) const;
#endif
		/// \brief print the element to the output stream
		virtual void	print(std::ostream& out) const;
};

/// \private
inline std::ostream& operator<< (std::ostream& os, const json_object& obj) { obj.print(os); return os; }
/// \private
inline std::ostream& operator<< (std::ostream& os, const json_object* obj) { obj->print(os); return os; }


} // end namespoace

#endif
