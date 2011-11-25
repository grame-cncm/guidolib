#ifndef ARInstrument_H
#define ARInstrument_H

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

#include "ARMTParameter.h"

class TagParameterString;

/** \brief not yet documented
*/
class ARInstrument : public ARMTParameter
{
	public:

						ARInstrument();	
		virtual 		~ARInstrument();	

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		virtual void print() const;

		const char* getName() const;
		const char* getSecondName() const;
		const TagParameterString * getPar1() const	 	{ return s1; }
		const TagParameterString *getPar2() const	 	{ return s2; }
		const TagParameterString *getTransp() const	 	{ return transp; }
		virtual bool IsStateTag() const 				{ return true; }
	
		virtual std::ostream &operator<<(std::ostream &os) const;

	protected:

		TagParameterString *s1;
		TagParameterString *s2;
		TagParameterString *transp;

		static ListOfTPLs ltpls;
};

#endif
