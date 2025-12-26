/*
  GUIDO Library
  Copyright (C) 2006-2008  Grame

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

#ifndef __guidoelement__
#define __guidoelement__

#include <string>
#include <vector>

#include "arexport.h"
#include "visitable.h"
#include "ctree.h"
#include "gar_smartpointer.h"

namespace guido 
{

class guidoelement;
class guidoattribute;

typedef SMARTP<guidoattribute> 	Sguidoattribute;
typedef SMARTP<guidoelement> 	Sguidoelement;

/*!
\brief A generic guido attribute representation.

	An attribute is represented by its name and its value.
	Attributes are mainly used with tags. Names are often omitted.	
*/
class gar_export guidoattribute : public smartable {
	//! the attribute name
	std::string	fName;
	//! the attribute value
	std::string 	fValue;
	//! the optional attribute unit
	std::string 	fUnit;
	//! force quotes for string values
	bool fQuoteVal;
	
    protected:
		guidoattribute() : fQuoteVal(false) {}
		virtual ~guidoattribute() {}
    public:
		static	Sguidoattribute create();
				Sguidoattribute	clone() const;

		void setName (const std::string& name);
		void setValue (const std::string& value, bool quote=false);
		void setUnit (const std::string& unit);
		void setValue (long value);
		void setValue (double value);
		void setQuoteVal (bool);

		const std::string& getName () const		{ return fName; }
		const std::string& getValue () const	{ return fValue; }
		const std::string& getUnit () const		{ return fUnit; }
		bool			quoteVal () const		{ return fQuoteVal; }

		//! returns the attribute value as a int
		operator int () const;
		//! returns the attribute value as a long
		operator long () const;
		//! returns the attribute value as a float
		operator float () const;

		//________________________________________________________________________
		virtual bool operator ==(const Sguidoattribute& i) const;
		virtual bool operator !=(const Sguidoattribute& i) const		{ return !(*this == i); }
};

typedef std::vector<Sguidoattribute> Sguidoattributes;
/*!
\brief A generic guido element representation.

	An element is represented by its name and
	the lists of its attributes.
	Attributes and elements must be added in the required order.
*/
class gar_export guidoelement : public ctree<guidoelement>, public visitable
{
	//! the element name
	std::string	fName;
	//! list of the element attributes
	Sguidoattributes fAttributes;
	//! list of the element attributes
	bool	fAuto;
	
    protected:
		guidoelement() : fAuto(false) {}
		virtual ~guidoelement() {}
		// check if 2 elements have the same attributes
		virtual bool operator ==(const Sguidoattributes& attributes) const;

	public:
        static Sguidoelement create();

		virtual void		acceptIn(basevisitor& visitor);
		virtual void		acceptOut(basevisitor& visitor);
		virtual	void		print(std::ostream& os);

		void				setName (const std::string& name);
		const std::string&	getName () const		{ return fName; }
		void				setAuto(bool v)			{ fAuto = v; }
		bool				getAuto() const			{ return fAuto; }

		long add (const Sguidoattribute& attr);        
		long add (const Sguidoattributes& attr);        
        const Sguidoattributes& attributes() const	{ return fAttributes; }
        Sguidoattributes& attributes()				{ return fAttributes; }

        void				delAttribute			(const std::string& attrname);

        const Sguidoattribute getAttribute			(const std::string& attrname) const;
		const std::string	getAttributeValue		(const std::string& attrname) const;
		long				getAttributeLongValue	(const std::string& attrname, long defaultvalue) const;
		int					getAttributeIntValue	(const std::string& attrname, int defaultvalue) const;
		float				getAttributeFloatValue	(const std::string& attrname, float defaultvalue) const;

        const Sguidoattribute getAttribute			(unsigned int index) const;
		const std::string	getAttributeValue		(unsigned int index) const;
		long				getAttributeLongValue	(unsigned int index, long defaultvalue) const;
		int					getAttributeIntValue	(unsigned int index, int defaultvalue) const;
		float				getAttributeFloatValue	(unsigned int index, float defaultvalue) const;

		//________________________________________________________________________
		virtual bool operator ==(const Sguidoelement& i) const;
		virtual bool operator !=(const Sguidoelement& i) const		{ return !(*this == i); }
};

gar_export std::ostream& operator << (std::ostream& os, const Sguidoelement& elt);

} // namespace

#endif
