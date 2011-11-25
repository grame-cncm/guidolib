#ifndef ARMTParameter_H
#define ARMTParameter_H

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
#include "ARMusicalTag.h"

class TagParameterList;

/** \brief not yet documented
*/
class ARMTParameter : public ARMusicalTag
{
	public:
	
	  	ARMTParameter( const TYPE_TIMEPOSITION & tp, const ARMTParameter * copy = 0 )
		  					: ARMusicalTag( tp, copy )
	 	{ }

		ARMTParameter( int pid = -1, const ARMTParameter * copy = 0 ) 
						: ARMusicalTag( pid, copy )
		{ }
	
		virtual ~ARMTParameter();


		virtual void setTagParameterList(TagParameterList & theTagParameterList) = 0;

		virtual TagParameterList * getTagParameterList() const
		{ return 0; }

};

#endif


