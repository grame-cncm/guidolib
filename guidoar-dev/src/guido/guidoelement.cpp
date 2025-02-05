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

#include <stdlib.h>
#include <iostream>

#include <string>
#include <sstream>
#include "guidoelement.h"
#include "gmnvisitor.h"
#include "gmnvisitor.h"
#include "tree_browser.h"
#include "visitor.h"

using namespace std;

namespace guido 
{

//______________________________________________________________________________
// guidoattribute
//______________________________________________________________________________
void guidoattribute::setName (const string& name) 		{ fName = name; }
void guidoattribute::setUnit (const string& unit)		{ fUnit = unit; }
void guidoattribute::setValue (const string& value, bool quote) { fValue = value; fQuoteVal=quote; }
void guidoattribute::setQuoteVal (bool quote)			{ fQuoteVal=quote; }

//______________________________________________________________________________
void guidoattribute::setValue (long value)
{
	stringstream s;
	s << value;
	s >> fValue;
}

//______________________________________________________________________________
void guidoattribute::setValue (double value)
{
	stringstream s;
	s << value;
	s >> fValue;
}

guidoattribute::operator int () const	{ return atoi(fValue.c_str()); }
guidoattribute::operator long () const	{ return atol(fValue.c_str()); }
guidoattribute::operator float () const	{ return (float)atof(fValue.c_str()); }

//______________________________________________________________________________
bool guidoattribute::operator ==(const Sguidoattribute& elt) const { 
	return  elt 
			&& (getName() == elt->getName())
			&& (getValue() == elt->getValue())
			&& (getUnit() == elt->getUnit())
			&& (quoteVal() == elt->quoteVal());
}

//______________________________________________________________________________
// guidoelement
//______________________________________________________________________________
void guidoelement::setName (const string& name) 	{ fName = name; }

//______________________________________________________________________________
Sguidoattribute guidoattribute::create()    
	{ guidoattribute * o = new guidoattribute; assert(o!=0); return o; }
Sguidoelement guidoelement::create()		
	{ guidoelement * o = new guidoelement; assert(o!=0); return o; }

//______________________________________________________________________________
// attributes access by name
//______________________________________________________________________________
const Sguidoattribute guidoelement::getAttribute (const std::string& attrname) const
{
	Sguidoattributes::const_iterator it;
	for (it = attributes().begin(); it != attributes().end(); it++) {
		if ((*it)->getName() == attrname)
			return *it;
	}
	return 0;
}

void guidoelement::delAttribute (const std::string& attrname)
{
	Sguidoattributes::iterator it;
	for (it = attributes().begin(); it != attributes().end(); it++) {
		if ((*it)->getName() == attrname) {
			attributes().erase(it);
			break;
		}
	}
}

const std::string guidoelement::getAttributeValue (const std::string& attrname) const
{
	Sguidoattribute attribute = getAttribute(attrname);
	return attribute ? attribute->getValue() : "";
}

long guidoelement::getAttributeLongValue	(const std::string& attrname, long defaultvalue) const
{
	Sguidoattribute attribute = getAttribute(attrname);
	return attribute ? long(*attribute) : defaultvalue;
}

int guidoelement::getAttributeIntValue	(const std::string& attrname, int defaultvalue) const
{
	Sguidoattribute attribute = getAttribute(attrname);
	return attribute ? int(*attribute) : defaultvalue;
}

float guidoelement::getAttributeFloatValue	(const std::string& attrname, float defaultvalue) const
{
	Sguidoattribute attribute = getAttribute(attrname);
	return attribute ? float(*attribute) : defaultvalue;
}

//______________________________________________________________________________
// attributes access by index
//______________________________________________________________________________
const Sguidoattribute guidoelement::getAttribute (unsigned int index) const
{
	Sguidoattributes attrs = attributes();
	return (index < attrs.size()) ? attrs[index] : 0;
}

const std::string guidoelement::getAttributeValue (unsigned int index) const
{
	Sguidoattribute attribute = getAttribute(index);
	return attribute ? attribute->getValue() : "";
}

long guidoelement::getAttributeLongValue	(unsigned int index, long defaultvalue) const
{
	Sguidoattribute attribute = getAttribute(index);
	return attribute ? long(*attribute) : defaultvalue;
}

int guidoelement::getAttributeIntValue	(unsigned int index, int defaultvalue) const
{
	Sguidoattribute attribute = getAttribute(index);
	return attribute ? int(*attribute) : defaultvalue;
}

float guidoelement::getAttributeFloatValue	(unsigned int index, float defaultvalue) const
{
	Sguidoattribute attribute = getAttribute(index);
	return attribute ? float(*attribute) : defaultvalue;
}

//______________________________________________________________________________
long guidoelement::add (const Sguidoattribute& attr)
{ 
	long n = fAttributes.size();
	fAttributes.push_back(attr);
	return n;
}

//______________________________________________________________________________
long guidoelement::add (const Sguidoattributes& attr)
{ 
	long n = fAttributes.size();
	for (Sguidoattributes::const_iterator i = attr.begin(); i != attr.end(); i++)
		fAttributes.push_back(*i);
	return n;
}

//______________________________________________________________________________
void guidoelement::acceptIn(basevisitor& v) {
	if (visitor<Sguidoelement>* p = dynamic_cast<visitor<Sguidoelement>*>(&v)) {
		Sguidoelement ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoelement::acceptOut(basevisitor& v) {
	if (visitor<Sguidoelement>* p = dynamic_cast<visitor<Sguidoelement>*>(&v)) {
		Sguidoelement ge = this;
		p->visitEnd (ge);
	}
}

//______________________________________________________________________________
bool guidoelement::operator ==(const Sguidoattributes& attlist) const
{
	for (Sguidoattributes::const_iterator i = attributes().begin(); i != attributes().end(); i++) {
		// position attributes are ignored for comparison
		string name = (*i)->getName();
		if ((name.find ("dx", 0) != 0) && (name.find ("dy", 0) != 0) && (name != "h")) {
			Sguidoattribute first = *i;
			Sguidoattributes::const_iterator j = attlist.begin();
			for (; j != attlist.end(); j++) {
				Sguidoattribute second = *j;
				if (*first == second) break;
			}
			// check if we found any matching attribute
			if (j == attlist.end()) return false;
		}
	}
	return true;
}

//______________________________________________________________________________
bool guidoelement::operator ==(const Sguidoelement& elt) const { 
	return elt && (getName() == elt->getName()) && (*this) == elt->attributes();
}

//______________________________________________________________________________
void guidoelement::print(ostream& os) {
	gmnvisitor gv(os);
	tree_browser<guidoelement> browser(&gv);
	browser.browse(*this);
}

//______________________________________________________________________________
ostream& operator << (ostream& os, const Sguidoelement& elt) {
	elt->print(os);
	return os;
}

} // namespace
