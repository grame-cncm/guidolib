#ifndef TagParameterFloat_H
#define TagParameterFloat_H

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

		static bool     convertValue(float value, double &toValue, const char * unit, float curLSPACE = 50.0f);

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
