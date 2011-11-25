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

#include "ARBembel.h"

#include "GRBembel.h"
#include "GRDefine.h"

unsigned int GRBembel::sBembelSymbol = kBembelSymbol; // SCR_BEMBEL	was 164
NVPoint GRBembel::refpos;

GRBembel::GRBembel(ARBembel * par)
		: GRTagARNotationElement(/*dynamic cast<ARMusicalObject *>*/(par), LSPACE)
{
	// No!
	mNeedsSpring = 1;

	// obsolete
	// spacing = 0;
	mSymbol = sBembelSymbol;

	const float extent = (float)GetSymbolExtent( mSymbol );

	// obsolete ..
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;
	mBoundingBox.right = (GCoord)extent;
	mBoundingBox.bottom = (GCoord)(3 * LSPACE);

	mLeftSpace = (GCoord)(extent * 0.5f);
	mRightSpace = (GCoord)(extent * 0.5f); // extent;


	// no referencePosition?
	refpos = NVPoint( (GCoord)(-extent * 0.5f) ,0);
}

GRBembel::~GRBembel()
{
}

void GRBembel::print() const
{
}

void GRBembel::OnDraw(VGDevice & hdc) const
{
	GRTagARNotationElement::OnDraw(hdc);
}

