/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#ifndef ___TagParameterMap___
#define ___TagParameterMap___

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "TagParameter.h"

class TagParameter;

//-------------------------------------------------------------------------------------------
/** \brief A list of tag parameters represented as a map.
*/
//-------------------------------------------------------------------------------------------
class TagParameterMap : public std::map<std::string, STagParameterPtr>
{
	public:
				 TagParameterMap();
				// creates a new TagParameterMap out of a definition string ...
				 TagParameterMap(const std::string & str);
		virtual ~TagParameterMap() {}
		
		void	Add (STagParameterPtr param);
		void	Add (const TagParameterMap& map);
		void	Remove (const char* key);
//		void	clear ()						{ fMap.clear(); }
//		size_t	size () const					{ return fMap.size(); }
		// check a parameters list againts with current one:
		// - check that current params include required param in list
		// - check that current params exist in list
		// - check float parameters type for unit settings
		bool	Match (const TagParameterMap& list, const char* tagName) const;
		void	checkUnit (const TagParameterMap& list) const;
	
		std::vector<std::string> getKeys() const;


		template<typename T> const T*	get (const char* param) const {
			const_iterator i = find (param);
			if (i == end()) return 0;
			return dynamic_cast<const T*>(i->second.get());
		}

		void	print (std::ostream& out, bool gmnform=true) const;
	
	static	std::vector<std::string> split (const std::string & str, char sep);
	static	std::vector<std::string> getKeys (const std::string & str);

	private:
		typedef std::map<std::string, STagParameterPtr>		TPMap;
		typedef std::pair<std::string, STagParameterPtr>	TIMap;

//		TPMap	fMap;
//
//		TPMap::const_iterator	 begin() const		{ return fMap.begin(); }
//		TPMap::const_iterator	 end() const		{ return fMap.end(); }

//		STagParameterPtr		 find (const std::string & str) const;
		STagParameterPtr		 str2tagParam (const std::string & str) const;
		bool					 checkRequired (const TagParameterMap& list, const char* tagName) const;
		bool					 checkExist (const TagParameterMap& list) const;
};
inline std::ostream& operator << ( std::ostream & os, const TagParameterMap& m) { m.print(os); return os; }
inline std::ostream& operator << ( std::ostream & os, const TagParameterMap* m) { m->print(os); return os; }


#endif
