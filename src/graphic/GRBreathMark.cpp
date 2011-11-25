/*
	GUIDO Library
	Copyright (C) 2004	Grame

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

//(JB) experimental implementation of breath-marks

#include "ARBreathMark.h"
#include "GRBreathMark.h"
#include "GRDefine.h"

unsigned int GRBreathMark::sBreathMarkSymbol = kBreathMarkSymbol;
NVPoint GRBreathMark::refpos;

GRBreathMark::GRBreathMark( ARBreathMark * ar )
		: GRTagARNotationElement( ar, LSPACE )
{
	mNeedsSpring = 1;

	mSymbol = sBreathMarkSymbol;

	const float extent = (float)GetSymbolExtent( mSymbol );

	// obsolete ..
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;
	mBoundingBox.right = 0;
	mBoundingBox.bottom = 0; // todo

	// - A breath-mark must not change the spacing.
	mLeftSpace = 0;
	mRightSpace = 0;

	// no reference position?
	refpos = NVPoint( - extent - float(0.25) * LSPACE, 0 );

	mPosition.y = - float(1.5) * LSPACE;
}

GRBreathMark::~GRBreathMark()
{
}


void GRBreathMark::OnDraw(VGDevice & hdc) const
{
	GRTagARNotationElement::OnDraw(hdc);
}

