#ifndef ARClef_H
#define ARClef_H

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

/** \brief Abstract representation of a staff clef.
*/
class ARClef : 
// public ARMusicalObject,
	public ARMTParameter
{
	public:
			enum cleftype  { OFF, AUTO, VIOLIN, BASS, BRATSCHE, PERC, DOUBLEG };
			enum octavatype  { LOW15 = -2, LOW8 = -1, NONE = 0, HIGH8 = 1, HIGH15 = 2 };

						ARClef();
						ARClef(const ARClef & clef);
						ARClef(const TYPE_TIMEPOSITION & timeposition);
						ARClef(const std::string& theName);

		virtual 		~ARClef();

	  	virtual bool 	IsStateTag() const;

		virtual void 	print() const;
		// virtual ostream & operator<<(ostream &os) const;
		virtual void 	PrintName(std::ostream &os) const;
		virtual void 	PrintParameters(std::ostream &os) const;

		virtual bool operator==(const ARClef & clef);

		virtual void setTagParameterList(TagParameterList & theTagParameterList);

		void				setName(const std::string& theName);
		const std::string&	getName() const	{ return fName; }

	
		// staffline where the clef starts
		int			getLine() const 		{ return mStaffLine; }
		cleftype	getClefType() const 	{ return mClef;  }
		octavatype	getOctavaType() const 	{ return fOctava; }


	protected:
			
		static ListOfTPLs ltpls;

	private:

		std::string	fName;

		cleftype 	mClef;
		int 		mStaffLine; // Die Notenlinie
		octavatype 	fOctava;
};

#endif


