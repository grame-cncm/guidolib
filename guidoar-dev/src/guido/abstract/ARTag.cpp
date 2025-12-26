/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include <sstream>
#include "ARTag.h"

using namespace std;

namespace guido 
{

//______________________________________________________________________________
bool guidotag::beginTag() const {
	return getName().find("Begin", 0) != string::npos;
}

//______________________________________________________________________________
bool guidotag::endTag() const {
	return getName().find("End", 0) != string::npos;
}

//______________________________________________________________________________
string guidotag::matchTag () const {
	string name = getName();
	size_t n = name.find("Begin", 0);
	if (n != string::npos) {
		name.replace (n, name.length(), "End");
		return name;
	}
	else {
		n = name.find("End", 0);
		if (n != string::npos) {
			name.replace (n, name.length(), "Begin");
			return name;
		}
	}
	return "";
}

//______________________________________________________________________________
bool guidotag::matchTag (const Sguidotag& tag) const {
	return matchTag() == tag->getName();
}

//______________________________________________________________________________
bool guidotag::operator ==(const Sguidotag& elt) const { 
	return (guidoelement::operator == (elt)) && (getID() ==  elt->getID());
}

//______________________________________________________________________________
void guidotag::acceptIn(basevisitor& v) {
	if (visitor<SMARTP<guidotag> >* p = dynamic_cast<visitor<SMARTP<guidotag> >*>(&v)) {
		SMARTP<guidotag> sptr = this;
		p->visitStart(sptr);
	}
	else guidoelement::acceptIn(v);
}		

//______________________________________________________________________________
void guidotag::acceptOut(basevisitor& v) {
	if (visitor<SMARTP<guidotag> >* p = dynamic_cast<visitor<SMARTP<guidotag> >*>(&v)) {
		SMARTP<guidotag> sptr = this;
		p->visitEnd(sptr);
	}
	else guidoelement::acceptOut(v);
}		

//______________________________________________________________________________
string guidotag::escape(const std::string& val) const
{
	string esc;
	size_t n = val.size();
	for (int i=0; i<n; i++) {
		if ((val[i] == '\\') && (val[i+1] == '"'))
			esc += '\\';
		else if (val[i] == '"')
			esc += '\\';
		esc += val[i];	
	}
	return esc;
}

//______________________________________________________________________________
guidotag::operator string() const {
    ostringstream str;

	str << '\\' << getName();
	long n = getID();
	if (n) str << ':' << n;

	if (attributes().size()) {
		str << '<';

		Sguidoattributes::const_iterator attr = attributes().begin();
		while (attr != attributes().end()) {
			n = (*attr)->getName().size();
			if (n) str << (*attr)->getName() << "=";

			const string quote = (*attr)->quoteVal() ? "\"" : "";
			string value = escape((*attr)->getValue());
			
			str << quote << value << quote;
			
			n = (*attr)->getUnit().size();
			if (n) str << (*attr)->getUnit();

			attr++;
			if (attr != attributes().end())
				str << ',';
		}
		str << '>';
	}
	return  str.str();
}

} // namespace

