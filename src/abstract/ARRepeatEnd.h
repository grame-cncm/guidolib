#ifndef ARRepeatEnd_H
#define ARRepeatEnd_H

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

// #include <iosfwd>

#include "ARPositionTag.h"
#include "ARBar.h"
//#include "ARMTParameter.h"
// #include "ARMusicalObject.h"

class ARFactory;
class ARRepeatBegin;

/** \brief not yet documented
*/
//class ARRepeatEnd : public ARMTParameter, public ARPositionTag
//class ARRepeatEnd : public ARBar, public ARPositionTag
class ARRepeatEnd : public ARBar
{
		friend class ARFactory;
		friend class ARRepeatBegin;

	public:

					ARRepeatEnd(ARRepeatBegin * curbeg = 0);
		virtual		~ARRepeatEnd() { }

		int			getNumRepeat() const { return numrepeat; }
		virtual void setTagParameterList( TagParameterList & tpl);

		virtual void	print() const { };
		virtual void	browse(TimeUnwrap& mapper) const;

		virtual std::ostream & operator<<(std::ostream & os) const;

	protected:

		static ListOfTPLs ltpls;
		ARRepeatBegin * repbeg;
		int numrepeat;
};

#endif
