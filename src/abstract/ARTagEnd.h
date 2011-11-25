#ifndef ARTagEnd_H
#define ARTagEnd_H

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
// #include "ARMusicalObject.h"
#include "ARPositionTag.h"

class ARMusicalObject;

/** \brief not yet documented
*/
class ARTagEnd : public ARMTParameter, public ARPositionTag
{

	public:

					ARTagEnd(int id = -1,const ARTagEnd * copy = 0);
		virtual		~ARTagEnd() {}
		
		virtual ARMusicalObject * Copy() const 	{ return 0; }
		
		virtual void print() const {}
		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		virtual void setEndPosition(GuidoPos p_pos);
		virtual GuidoPos getEndPosition() const;
		
		virtual bool	isEndTagClass() const { return true; }
		
		static ARTagEnd * cast( ARPositionTag * inTag )
					{ return ( inTag && inTag->isEndTagClass() ? static_cast<ARTagEnd *>( inTag ) : 0 ); }
};

#endif
