#ifndef GRStaccato_H
#define GRStaccato_H

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

#include "ARStaccato.h"
#include "GRARNotationElement.h"
#include "GRDefine.h"

class GRStaff;


/** \brief Graphical representation af a Staccato
*/
class GRStaccato : public GRARNotationElement
{

  public:
		GRStaccato( ARStaccato * arstacc, bool ownsar = false);

		virtual ~GRStaccato();

		// ignoriere Positionssetzungen ...
		virtual void setPosition(const NVPoint &point);

		// this will be done by the event to which
		// the staccato belongs ....
		virtual void tellPosition(GObject * caller,const NVPoint & pos);

		virtual void OnDraw( VGDevice & hdc ) const;

		virtual void print() const;





};

#endif


