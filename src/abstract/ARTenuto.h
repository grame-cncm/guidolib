#ifndef ARTenuto_H
#define ARTenuto_H

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

// for TagParameters
//#include "TagParameterString.h"
//#include "TagParameterFloat.h"
//#include "TagParameterInt.h"


/** \brief the Tenuto articulation tag
*/
class ARTenuto : public ARMTParameter, public ARPositionTag
{		
public:			


	ARTenuto() : ARMTParameter()
	{
		rangesetting = ONLY;
	}

	virtual ~ARTenuto() 
	{
		// delete TagParameterPointer ...
	};

	virtual void setTagParameterList(TagParameterList& tpl);

	// routines to access TagParameters ..
	virtual void print() const;

	virtual void	browse(TimeUnwrap& mapper) const;
	
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;
	

protected:

	// TagParameters ..
	// TagParameterString *textformat;
	// TagParameterFloat  *fsize;
	// TagParameterInt	  *fnumber;


	// this is important for allowed
	// TagParameters ....
	static ListOfTPLs ltpls;

};

#endif
