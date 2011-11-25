#ifndef ARPageFormat_H
#define ARPageFormat_H

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

class GRPage;

/** \brief not yet documented
*/
class ARPageFormat :  public ARMTParameter
{
		friend class GRPage;
public:

				ARPageFormat( float px, float py, float ml, float mt, float mr, float mb );
				ARPageFormat( char * px, char * py, char * ml, char * mt, char * mr, char * mb );
				ARPageFormat( const ARPageFormat & arp );
				ARPageFormat();

	virtual 	~ARPageFormat();

	void		 getPageFormat(float * sx, float * sy, float * ml, float * mt, 
														float * mr, float * mb );

	virtual bool IsStateTag() const { return true; }
	
	virtual void setTagParameterList( TagParameterList & theTagParameterList );
	virtual void print() const { }

	virtual std::ostream & operator<<(std::ostream &os) const
	  			{ return os;   }

protected:

			void	ClipSize();
			void	AdjustMargins();
	
	float mSizeX;		// All sizes are in internal (logical) units.
	float mSizeY;
	float mLeft;		
	float mTop;
	float mRight;
	float mBottom;
	NVstring mFormat;

	static ListOfTPLs ltpls;

};

#endif
