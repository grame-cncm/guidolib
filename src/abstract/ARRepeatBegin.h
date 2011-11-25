#ifndef ARRepeatBegin_H
#define ARRepeatBegin_H

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

//#include "ARMTParameter.h"
// #include "ARMusicalObject.h"
#include "ARBar.h"

class ARRepeatEnd;
class ARFactory;

/** \brief The right facing repeat sign.
*/
class ARRepeatBegin : public ARMTParameter	// ,public ARMusicalObject
//class ARRepeatBegin : public ARBar
{
	public:

		friend class ARRepeatEnd;
		friend class ARFactory;

					 ARRepeatBegin() : numrepeat(0) { dur.setNumerator(-1); }
		virtual		~ARRepeatBegin()	{ }

		virtual void	setTagParameterList(TagParameterList & tpl);

		virtual void	print() const { }
		virtual void	browse(TimeUnwrap& mapper) const;

		virtual std::ostream & operator << ( std::ostream & os ) const;
		virtual void	setRepeatEnd( const ARRepeatEnd * );
	
	protected:

		static ListOfTPLs ltpls;
		int numrepeat;
		TYPE_DURATION dur;
};

#endif
