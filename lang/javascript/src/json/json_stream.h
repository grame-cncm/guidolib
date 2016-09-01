/*
  JSON to OSC Library
  Copyright (C) 2013  Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/


#ifndef json_stream__
#define json_stream__

#include <ostream>
#include "export.h"

namespace json
{

//______________________________________________________________________________
/*!
\internal
\brief to be used in place of std::endl
	to provide indentation of the json output.
*/
class jsonexport json_endl {
	private:
		int fIndent;
	public:
				 json_endl() : fIndent(0) {}
		virtual ~json_endl() {}

		//! increase the indentation
		json_endl& operator++ (int)		{ fIndent++; return *this; }	// prefix
		json_endl& operator++ ()		{ fIndent++; return *this; }	// postfix
		//! decrease the indentation
		json_endl& operator-- (int)		{ fIndent--; return *this; }	// prefix
		json_endl& operator-- ()		{ fIndent--; return *this; }	// postfix
		//! reset the indentation to none
		void print(std::ostream& os) const;
};
/// \private
inline std::ostream& operator<< (std::ostream& os, const json_endl& eol) { eol.print(os); return os; }


//______________________________________________________________________________
/*!
\internal
\brief a stream that embed a special endl for output formatting
*/
class jsonexport json_stream {
	std::ostream&	fStream;
	json_endl		fEndl;
	public:
				 json_stream(std::ostream& stream) : fStream(stream) {}
		virtual ~json_stream() {}
		
		std::ostream& stream() const		{ return fStream; }
		const json_endl& nl() const			{ return fEndl; }
			  json_endl& nl()				{ return fEndl; }
};

class jsonexport json_array;
class jsonexport json_object;
/// \private
json_stream& operator<< (json_stream& os, const json_object* obj);
/// \private
json_stream& operator<< (json_stream& os, const json_array* arr);
/// \private
template<typename T> json_stream& operator<< (json_stream& os, T val)		{ os.stream() << val; return os; }

} // end namespace

#endif
