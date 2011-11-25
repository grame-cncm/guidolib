#ifndef TagParameterFloat_H
#define TagParameterFloat_H

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

#include "TagParameter.h"

typedef float TYPE_FLOATPARAMETER;

/** \brief The floating point parameter type for Guido Tags.
*/
class TagParameterFloat : public TagParameter
{
	public:

						 TagParameterFloat( float theFloat );
						 TagParameterFloat( const TagParameterFloat & tpf );
		virtual 		~TagParameterFloat() {}

		virtual void	set( const TagParameterFloat & in );
				void	reset(float theFloat, const char * inUnit); /// resets value, unit & unittag

	  	virtual float 	getUnitValue() const		{ return fValue; }
	  			void 	setUnit(const char *un);
	  	virtual void 	setValue(const char *p);	 	 
	 	virtual void 	setValue(float val)			{ fValue = val; }

		virtual TagParameter*	getCopy() const		{ return new TagParameterFloat(*this); }
		virtual bool			copyValue(const TagParameter *tp); // Returns true on if successful
		virtual bool			isFloat() const		{ return true; }

		const TYPE_FLOATPARAMETER getValue( float curLSPACE = 50.0f ) const;

		static TYPE_FLOATPARAMETER convertValue(float value, const char * unit, float curLSPACE = 50.0f);

		bool			TagIsUnitTag() const 		{ return fUnittag; }
		const char *	getUnit() const				{ return fUnit; }

	static TagParameterFloat * cast( TagParameter * inTag ) 
					{ return inTag->isFloat() ? static_cast<TagParameterFloat *>(inTag) : 0; }
	static const TagParameterFloat * cast( const TagParameter * inTag ) 
					{ return inTag->isFloat() ? static_cast<const TagParameterFloat *>(inTag) : 0; }

		//------------------------
		bool				fUnittag; // true if the tag is a unit tag (offset or width/height)

	protected:
		enum { kUnitLen=3 };
		char				fUnit[kUnitLen];
		TYPE_FLOATPARAMETER fValue;
		
	private:
		using TagParameter::set;
};

#endif
