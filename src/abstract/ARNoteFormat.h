#ifndef ARNoteFormat_H
#define ARNoteFormat_H

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

/** \brief not yet documented
*/
class ARNoteFormat :  public ARMTParameter, public ARPositionTag
{
	public:
		const char* getStyle() const;

		const TagParameterString * getTPStyle() const
		{
			return style;
		}

				 ARNoteFormat(ARNoteFormat * p_savenf = NULL,
				 ARNoteFormat * copynf = NULL);
		virtual ~ARNoteFormat();

		virtual bool IsStateTag() const { return true; }

		virtual void setTagParameterList( TagParameterList & tpl);
		virtual void print() const { }
		virtual void PrintName(std::ostream & os) const;
		virtual void PrintParameters(std::ostream &os) const;

		virtual ARNoteFormat * getEndTag() const		{ return new ARNoteFormat(NULL,savenf); } 

	protected:
		ARNoteFormat * savenf;
		TagParameterString * style;
		static ListOfTPLs ltpls;
};

#endif 
