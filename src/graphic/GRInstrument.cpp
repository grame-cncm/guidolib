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

#include <string.h>

#include "GRInstrument.h"
#include "GRDefine.h"
#include "ARMusicalObject.h"
#include "ARInstrument.h"

GRInstrument::GRInstrument(ARInstrument * par)
		: GRTagARNotationElement(/*dynamic cast<ARMusicalObject *>*/(par), LSPACE)
{
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;

	// No!
	mNeedsSpring = 1;


	// obsolete
	// spacing = 0;

	// to do: read real text ...
	const GCoord extent = 100;

	mBoundingBox.right = extent;
	mBoundingBox.bottom = (GCoord)(3*LSPACE);

	// no Space needed !
	mLeftSpace = 0;
	mRightSpace = 0;

	// no reference position ...
}

GRInstrument::~GRInstrument()
{
}

void GRInstrument::print() const
{
}

void GRInstrument::OnDraw(VGDevice & hdc) const
{
	// Now draw something
	//HFONT hfontold = (HFONT) SelectObject(hdc,hfontscriab);
	//PS_SELECTSCRIABIN ;

	const char* nv = static_cast/*dynamic cast*/<ARInstrument *>(mAbstractRepresentation)->getName();
	if (nv == 0) return;

	GRTagARNotationElement::OnDrawText(hdc, nv, strlen(nv));
}

