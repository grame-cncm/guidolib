#ifndef TagParameterString_H
#define TagParameterString_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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


