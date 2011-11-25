#ifndef ARStaccato_H
#define ARStaccato_H

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
#include "ARPositionTag.h"

/** \brief Staccato Tag
*/


class ARStaccato : public ARMTParameter,  public ARPositionTag
{		
public:			
	enum		tTypeStacc {REGULAR, HEAVY};

	virtual bool MatchEndTag(const char * s);
	ARStaccato() : ARMTParameter()
	{
		rangesetting = ONLY;
		type = REGULAR;
	}

	virtual ~ARStaccato() 
	{

		// delete TagParameterPointer ...

	};

	virtual void setTagParameterList(TagParameterList& tpl);

	virtual void	browse(TimeUnwrap& mapper) const;

	// routines to access TagParameters ..
	virtual void print() const;
	
	virtual void PrintName(std::ostream &os) const;
	virtual void PrintParameters(std::ostream &os) const;
	
	tTypeStacc getType() const {return type;};

protected:

	// TagParameters ..
	// TagParameterString *textformat;
	// TagParameterFloat  *fsize;
	// TagParameterInt	  *fnumber;


	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;
	tTypeStacc	type;

};

#endif
