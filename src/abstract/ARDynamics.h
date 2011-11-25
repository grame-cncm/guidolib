#ifndef ARDynamics_H
#define ARDynamics_H

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
// #include "ARNote.h"
#include "ARMTParameter.h"
#include "ARPositionTag.h"


/** \brief not yet documented
*/
class ARDynamics : 
	public ARMTParameter,
		//  public ARMusicalObject,
	public ARPositionTag
{
	public:

					ARDynamics();
		virtual 	~ARDynamics();

		virtual void print() const;
		virtual void setTagParameterList(TagParameterList& tpl);

		virtual const NVstring & getText() const { return text; }

		virtual float getValue() const { return (float) value; }

		int location; // 0 for upper, 1 fuer lower ... should be bool?

protected:

	float value;
	NVstring text;

	static ListOfTPLs ltpls;
};

#endif
