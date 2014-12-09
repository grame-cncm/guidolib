#ifndef TagParameterString_H
#define TagParameterString_H

/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>

#include "TagParameter.h"


/** \brief A string-type tag parameter.
*/
class TagParameterString : public TagParameter
{
	public:
					TagParameterString( const char * theString );
					TagParameterString( const std::string& theString );
					TagParameterString( const TagParameterString & tps );

		virtual 	~TagParameterString();
		
		virtual void set( const TagParameterString & in );

		virtual void setValue(const char * p)		{ fValue = p; }
		virtual TagParameter * getCopy() const		{ return new TagParameterString(*this); }
	
		virtual bool copyValue( const TagParameter * tp ); 	// returns 1 if successful,
        virtual bool isString() const				{ return true; }

		const char* getValue() const				{ return fValue.c_str(); }
		bool		getRGB( unsigned char colref[4] ) const; 	// returns 1 if successful
        bool        getBool(bool &result) const;
		bool operator == (const char * inCString ) const;

		static TagParameterString * cast( TagParameter * inTag ) 
				{ return inTag->isString() ? static_cast<TagParameterString *>(inTag) : 0; }
		static const TagParameterString * cast( const TagParameter * inTag ) 
				{ return inTag->isString() ? static_cast<const TagParameterString *>(inTag) : 0; }

  private:
		std::string fValue;
		using TagParameter::set;
};


#endif


