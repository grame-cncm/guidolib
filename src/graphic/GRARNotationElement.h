#ifndef GRARNotationElement_H
#define GRARNotationElement_H

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

class ARMusicalObject;

/** \brief This class is the base class for all graphical elements that 
	have a direct counterpart in the Abstract Representation.
*/
class GRARNotationElement : public GRNotationElement
{
  public:
		GRARNotationElement(ARMusicalObject * abstractRepresentationOfNotationElement, bool p_ownsAR = false );
		virtual ~GRARNotationElement();

		virtual ARMusicalObject * getAbstractRepresentation();
		virtual ARMusicalObject * getAbstractRepresentation() const;

		// sets the ownership between abstract and grafical object
		virtual void setOwnership(bool ownsAR)      { mOwnsAR = ownsAR; }

  protected:
		ARMusicalObject * mAbstractRepresentation;
		bool mOwnsAR;   // is 1 if abstract object is owned by the grafical object
                        // In that case the abstract object will be deleted when deleting the grafical object
};

#endif


