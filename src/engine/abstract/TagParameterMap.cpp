/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <algorithm>

#include "TagParameterMap.h"
#include "TagParameterInt.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"
#include "TagParameterInt.h"

#include "defines.h"	// For GuidoWarn

using namespace std;

/** \brief Manual to using TPL (TagParameterLists)

 A TagParameterList contains a list of TagParameters. 
 It can either be created by supplying a string or by adding one tag after another.
 The second mechanism is used in ARFactory while parsing the tagparameters. 
 The first mechanism is used statically for each parameter-Tag to find matching tag-parameters.
 The string is in the following format:
 "TYPE,NAME,DEFVALUE,REQUIRED" where
 TYPE is one of 
	"U" : a unit parameter (having a unit ("cm","pt" ..)
		  or if none the DEFAULT UNIT set with the units tag.
	"S" : a string parameter
	"I" : an Integer parameter
	"F" : a Float parameter
 NAME is a name that can be used when naming tag parameters instead of using the posiitons
 DEFVALUE is the default value of the parameter
  (this makes only sense, if the parameter is optional, in which
   case the defvalue is stored when parsing the tagparameters)
 REQUIRED is either "r" or "o" for required or optional parameters.

 Each (Parameter)Tag should define a list of TagParameterLists in their desired matching order.
 When the tagparameters are parsed, a match of parameters is tried out (by name, by position, by type and so forth)
 The first TPL in the LTPL which matches is returned by number and the values are set accordingly.
 Each tag than should handle the values in its own fashion.
*/

//-------------------------------------------------------------------------------------------
/** \brief Builds a TagParameterMap out of a definition string.
*/
TagParameterMap::TagParameterMap() { clear(); }
TagParameterMap::TagParameterMap (const std::string & str)
{
	clear();
	vector<string> defs = split (str, ';');
	for_each (defs.begin(), defs.end(), [this] (string str) -> void {
		STagParameterPtr p = str2tagParam (str);
		if (p) (*this)[p->getName()] = p;
	} );
}

//-------------------------------------------------------------------------------------------
//STagParameterPtr TagParameterMap::find (const std::string & str) const
//{
//	TPMap::const_iterator i = fMap.find(str);
//	return i == fMap.end() ? 0 : i->second;
//}

//-------------------------------------------------------------------------------------------
bool TagParameterMap::Match (const TagParameterMap& list, const char* tagName) const
{
	return checkExist (list) && checkRequired (list, tagName);
}

//-------------------------------------------------------------------------------------------
void TagParameterMap::checkUnit (const TagParameterMap& list) const
{
	for (TPMap::const_iterator i = begin(); i != end(); i++) {
		STagParameterPtr p = i->second;
		if (p->isFloat()) {
			const TagParameterFloat* f = list.get<TagParameterFloat>(i->first.c_str());
			if (f && f->TagIsUnitTag()) {
				TagParameterFloat* pf = dynamic_cast<TagParameterFloat*>(p.get());
				pf->setUnitTag();
			}
		}
	}
}

//-------------------------------------------------------------------------------------------
bool TagParameterMap::checkRequired (const TagParameterMap& list, const char* tagName) const
{
	for (TPMap::const_iterator i = list.begin(); i != list.end(); i++) {
		if (i->second->TagIsRequired()) {
			if (find(i->first) == end()) {
				cerr << tagName << ": required parameter '" << i->first << "' is missing" << endl;
				return false;
			}
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------
bool TagParameterMap::checkExist (const TagParameterMap& list) const
{
	for (TPMap::const_iterator i = begin(); i != end(); i++) {
		if (list.find(i->first) == list.end()) {
			cerr << "unsupported tag parameter '" << i->first << "'" << endl;
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------
vector<std::string> TagParameterMap::getKeys() const
{
	vector<string> out;
	for_each (begin(), end(), [&out] (const TIMap& elt) -> void { out.push_back(elt.first); });
	return out;
}

//-------------------------------------------------------------------------------------------
void TagParameterMap::Add (const TagParameterMap& map)
{
	for_each (map.begin(), map.end(), [this] (const TIMap& elt) -> void { (*this)[elt.first] = elt.second; });
}

//-------------------------------------------------------------------------------------------
void TagParameterMap::Remove (const char* key)
{
	erase (key);
}

//-------------------------------------------------------------------------------------------
void TagParameterMap::Add (STagParameterPtr param)
{
	if (!param) return;
	(*this)[param->getName()] = param;
}

//-------------------------------------------------------------------------------------------
/** \brief Decode a single parameter string
*/
STagParameterPtr TagParameterMap::str2tagParam (const std::string & str) const
{
	if (str.empty()) return 0;

	vector<string> parts = split(str, ',');
	if (parts.size() != 4) {				// 4 parts are expected
		cerr << "incorrect parts in tag parameter string: " << str << endl;
		return 0;
	}

	STagParameterPtr out;
	if (parts[0] == "U")	  out = STagParameterPtr(new TagParameterFloat(true));
	else if (parts[0] == "S") out = STagParameterPtr(new TagParameterString);
	else if (parts[0] == "I") out = STagParameterPtr(new TagParameterInt);
	else if (parts[0] == "F") out = STagParameterPtr(new TagParameterFloat(false));
	else {
		cerr << "incorrect type in parameter string: " << str << endl;
		return 0;
	}
	out->setName (parts[1].c_str());
	out->setValue(parts[2].c_str());
	out->setRequired( parts[3] == "r" );
	return out;
}

//-------------------------------------------------------------------------------------------
/** \brief Split a string into a strings list using a given separator
*/
vector<string> TagParameterMap::getKeys (const std::string & str)
{
	vector<string> out;
	vector<string> elts = split (str, ';');
	for_each( elts.begin(), elts.end(), [&out] (const string& str) -> void {
		vector<string> parts = split (str, ',');
		if (parts.size() == 4)	// check for corectness
			out.push_back (parts[1]);
	} );
	return out;
}

//-------------------------------------------------------------------------------------------
/** \brief Split a string into a strings list using a given separator
*/
vector<string> TagParameterMap::split (const std::string & str, char sep)
{
	vector<string> out;
	size_t n = str.size();

	size_t pos = 0;
	while (pos < n) {
		size_t next = str.find_first_of (sep, pos);
		if (next == std::string::npos) break;
		out.push_back (str.substr(pos, next-pos));
		pos = next + 1;
	}
	out.push_back(str.substr(pos));
	return out;
}

//-------------------------------------------------------------------------------------------
void TagParameterMap::print (std::ostream& out, bool gmnform) const
{
	if (!gmnform)
		for_each (begin(), end(), [&out] (const TIMap p) -> void { out << p.first << " -> " << p.second << endl; } );
	else if (size()) {
		out << "<";
		const char* sep = "";
		for (TPMap::const_iterator i = begin(); i!=end(); i++) {
			out << sep;
			i->second->print(out);
			sep = ", ";
		}
		out << ">";
	}
}

