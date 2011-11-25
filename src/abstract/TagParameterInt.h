#ifndef TagParameterInt_H
#define TagParameterInt_H

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

		virtual TagParameter *	getCopy() const			{ return new TagParameterInt( *this ); }
		virtual bool			isInt() const			{ return true; } // note that is derived from TagParameterFloat
		const TYPE_INTPARAMETER getValue() const		{ return (TYPE_INTPARAMETER) fValue; }

		virtual bool			copyValue( const TagParameter * tp );

	static TagParameterInt * cast( TagParameter * inTag )  { return inTag->isInt() ? static_cast<TagParameterInt *>(inTag) : 0; }
	static const TagParameterInt * cast( const TagParameter * inTag )  { return inTag->isInt() ? static_cast<const TagParameterInt *>(inTag) : 0; }
	
	private:
		using TagParameterFloat::setValue;
};


#endif


