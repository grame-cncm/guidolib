#ifndef GRTagARNotationElement_H
#define GRTagARNotationElement_H

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

#include "ARMusicalTag.h"
#include "GRTag.h"
#include "GRARNotationElement.h"

/** \brief GRTagARNotationElement
*/
class GRTagARNotationElement : public GRTag, public GRARNotationElement
{
public:
	GRTagARNotationElement (ARMusicalObject * el, float curLSPACE, bool ownsAR = false )
				: GRTag( dynamic_cast<ARMusicalTag *>(el), curLSPACE), GRARNotationElement(el, ownsAR)
	{}

	virtual ~GRTagARNotationElement();

	virtual const NVPoint &     getOffset() const       { return GRTag::getOffset(); }
	virtual float               getSize() const         { return GRTag::getSize(); }
	virtual const NVstring *    getFont() const         { return GRTag::getFont(); }
	virtual const NVstring *    getFontAttrib() const   { return GRTag::getFontAttrib(); }
	virtual int                 getFontSize() const     { return GRTag::getFontSize(); }
	virtual const unsigned char * getColRef() const     { return GRTag::getColRef(); }
};

#endif
