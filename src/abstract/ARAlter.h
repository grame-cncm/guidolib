#ifndef ARAlter_H
#define ARAlter_H

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

#include "ARMTParameter.h"
#include "ARPositionTag.h"

/** \brief The Alter tag
*/
class ARAlter : public ARMTParameter, public ARPositionTag
{
	public:
				ARAlter(ARAlter * p_savealter = NULL, ARAlter * copyalter = NULL); 
		virtual ~ARAlter();

		virtual bool IsStateTag() const									{ return true; }
		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		virtual const TagParameterString *getTPAlterString() const		{ return alterstring; }
		virtual const TagParameterFloat *getTPAlterValue() const		{ return altervalue; }

		virtual std::string getAlterText() const;
		virtual float getDetune() const;

		virtual void print() const;
        // virtual ostream & operator<<(ostream &os) const;
		virtual void PrintName(std::ostream & os) const;
		virtual void PrintParameters(std::ostream & os) const;

		virtual ARAlter * getEndTag() const								{ return new ARAlter(NULL, savealter); }

	protected:

		ARAlter * savealter;

		TagParameterString * alterstring;
		TagParameterFloat * altervalue;
		static ListOfTPLs ltpls;
};

#endif
