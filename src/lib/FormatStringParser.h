#ifndef __FormatStringParser__
#define __FormatStringParser__

/*
	GUIDO Library
	Copyright (C) 2004	Grame

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
