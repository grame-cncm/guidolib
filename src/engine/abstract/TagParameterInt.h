#ifndef TagParameterInt_H
#define TagParameterInt_H

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

#include "TagParameterFloat.h"

typedef int TYPE_INTPARAMETER;

/** \brief not yet documented
*/
class TagParameterInt : public TagParameterFloat
{
	public:
					 TagParameterInt( const TagParameterInt & tpi ) : TagParameterFloat(tpi) { }
					 TagParameterInt( int val )	: TagParameterFloat((float) val) {}
		virtual 	~TagParameterInt()	{}

		virtual void 	setValue( int i )				{ fValue = (float) i; }
		virtual void 	setValue( const char * p )		{ fValue = (float) atoi(p); }

		virtual TagParameter *	getCopy()  const		{ return new TagParameterInt( *this ); }
		virtual bool			isInt()    const		{ return true; } // note that is derived from TagParameterFloat
		const TYPE_INTPARAMETER getValue() const		{ return (TYPE_INTPARAMETER) fValue; }

		virtual bool			copyValue( const TagParameter * tp );

	static TagParameterInt * cast( TagParameter * inTag )  { return inTag->isInt() ? static_cast<TagParameterInt *>(inTag) : 0; }
	static const TagParameterInt * cast( const TagParameter * inTag )  { return inTag->isInt() ? static_cast<const TagParameterInt *>(inTag) : 0; }
	
	private:
		using TagParameterFloat::setValue;
};


#endif


