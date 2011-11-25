#ifndef GRPTagARNotationElement_H
#define GRPTagARNotationElement_H

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

class ARMusicalObject;

#include "GRPositionTag.h"
#include "GRARNotationElement.h"

/** \brief not yet documented
*/
class GRPTagARNotationElement :
		public GRPositionTag, public GRARNotationElement
{
public:
			GRPTagARNotationElement(GuidoPos p_ep, ARMusicalObject * el, bool ownsAR = false);

			GRPTagARNotationElement(ARMusicalObject * el, bool ownsAR = false);

	virtual ~GRPTagARNotationElement();

	virtual const NVPoint &getOffset() const
			{ return GRPositionTag::getOffset(); }

	virtual float getSize() const
			{ return GRPositionTag::getSize(); }

	virtual const NVstring * getFont() const
			{ return GRPositionTag::getFont(); }
	
	virtual const NVstring *getFontAttrib() const
			{ return GRPositionTag::getFontAttrib(); }
	
	virtual int getFontSize() const
			{ return GRPositionTag::getFontSize(); }
	
	const unsigned char * getColRef() const
			{ return GRPositionTag::getColRef(); }
			
	virtual void addAssociation( GRNotationElement * grnot );
	virtual void removeAssociation( GRNotationElement * el );

};

#endif
