#ifndef GRSystemTagInterface_H
#define GRSystemTagInterface_H

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


class GRSystem;

/** \brief not yet documented
*/
class GRSystemTagInterface
{
	public:

				GRSystemTagInterface() : mIsSystemCall(false) {}
		virtual ~GRSystemTagInterface() {}		
		virtual void checkPosition(const GRSystem * grsys);

	protected:
		// this variable is set to 1, if
		// a call to tellPosition is made from checkPosition
		// which mas made by the system ....
		// (look into GRBeam)
		bool mIsSystemCall;
};

#endif
