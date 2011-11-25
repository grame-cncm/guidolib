#ifndef GRSText_H
#define GRSText_H

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

#include "GRNotationElement.h"
#include "GRTag.h"
// #include "GRDefine.h"

class NVstring;
class GRStaff;
class NVPoint;
class VGDevice;

/** \brief not yet documented
*/
class GRSText : public GRNotationElement, public GRTag
{

  public:
						GRSText( GRStaff *, const NVstring & txt, int p_offsety );
		virtual 		~GRSText();

	 	virtual void 	tellPosition(GObject *caller, const NVPoint & );
	  	virtual void 	addAssociation(GRNotationElement *el);
		virtual void 	GGSOutput() const;
		virtual void 	OnDraw( VGDevice & hdc ) const;
			
		virtual void print() const;

  protected:
  	
  	GRStaff * grstaff;
    NVstring * txt;
	GRNotationElement * velement;
	int  offsety;
};

#endif

