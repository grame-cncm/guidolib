#ifndef __FormatStringParser__
#define __FormatStringParser__

/*
  GUIDO Library
  Copyright (C) 2004	Grame


  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include <string>
#include <vector>

class FormatStringParser;

typedef std::pair<std::string, int> FormatStringAssoc;
typedef std::vector<FormatStringAssoc> FormatStringParserResult;
/*! parse a Guido format string

	many Guido format string may include special characters described
	using a bracket bounded string. This class parse such a string and
	returns its result as a vector of string and type associations.
*/
class FormatStringParser
{
	public:
		enum type { kLitteral, kSpecial };
					 FormatStringParser() {}
		virtual		~FormatStringParser() {}

		virtual void	parse( const char * str, FormatStringParserResult& res);

	protected:
		virtual char	getSpecialStart() const		{ return '['; }
		virtual char	getSpecialEnd()	const		{ return ']'; }
		virtual char	getEscapeChar()	const		{ return '\\'; }
		virtual void	store (std::string str, type t, FormatStringParserResult& res) const;
		virtual void	switchTo (type t);

	private:
		std::string		mCurrString;
		type			mCurrType;
};

#endif
