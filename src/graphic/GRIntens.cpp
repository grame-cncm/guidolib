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

#ifdef VC6
#pragma warning (disable : 4786)
#endif

#include <string.h>

#include "ARIntens.h"

#include "GRIntens.h"
#include "GRStaff.h"
#include "GRDefine.h"

#include "GUIDOInternal.h"	// for gGlobalSettings.gDevice
#include "FontManager.h"

#include "VGDevice.h"
#include "VGFont.h"

GRIntens::GRIntens( GRStaff * inStaff ,ARIntens* abstractRepresentationOfIntens)
  : GRTagARNotationElement(abstractRepresentationOfIntens, LSPACE)
{
	assert(abstractRepresentationOfIntens);

	// this is not quite clear!
	mNeedsSpring = 1;
	sconst = SCONST_INTENS;

	// spacing= 0.75 * LSPACE;
	// obsolete
	// spacing = 0;

	mGrStaff = inStaff;

	// Now try to figure out, which intensity was selected:
	const char * cp = getARIntens()->getText().c_str();

//	rcount = 0;

	if (!strcmp(cp,"p"))
	{
		mSymbol = INTENS_P;
	}
	else if (!strcmp(cp,"f") )
	{
		mSymbol = INTENS_F;
	}
	else if (!strcmp(cp,"ff"))
	{
		mSymbol = INTENS_FF;
	}
	else if (!strcmp(cp,"fff"))
	{
		mSymbol = INTENS_FFF;
	}
	else if (!strcmp(cp,"ffff"))
	{
		mSymbol = INTENS_FFFF;
	}
	else if (!strcmp(cp,"mf"))
	{
		mSymbol = INTENS_MF;
	}
	else if (!strcmp(cp,"mp"))
	{
		mSymbol = INTENS_MP;
	}
	else if (!strcmp(cp,"sf"))
	{
		mSymbol = INTENS_SF;
	}
	else if (!strcmp(cp,"pp"))
	{
		mSymbol = INTENS_PP;
	}
	else if (!strcmp(cp,"ppp"))
	{
		mSymbol = INTENS_PPP;
//		rcount = 3;
	}
	else if (!strcmp(cp,"pppp"))
	{
		mSymbol = INTENS_PPPP;
//		rcount = 2;
	}

	if (mSymbol != 0)
	{
		float x = 0;
		float y = 0;
		if( gGlobalSettings.gDevice )
			FontManager::gFontScriab->GetExtent( mSymbol, &x, &y, gGlobalSettings.gDevice );

		mBoundingBox.left = (GCoord)(LSPACE / 4);
		mBoundingBox.right = (GCoord)(x);
		mBoundingBox.top = (GCoord)(y);

//		if (rcount!=0) mBoundingBox.right *= rcount;

		mBoundingBox.bottom = (GCoord)(4*LSPACE);
	}
	else
	{
		mBoundingBox.Set( 0, 0, 0, 0 );
	}

	mLeftSpace = 0;
	mRightSpace = 0;

	// no referencePosition?
	// probably dependant on Type!

	const float curLSPACE = mGrStaff ? mGrStaff->getStaffLSPACE() : LSPACE;

	mPosition.y = (GCoord)(6 * curLSPACE);
}

GRIntens::~GRIntens()
{
}


//## Other Operations (implementation)
void GRIntens::OnDraw(VGDevice & hdc) const
{

	GRTagARNotationElement::OnDraw( hdc );
}


void GRIntens::print() const
{
}

const ARIntens* GRIntens::getARIntens() const
{
	return /*dynamic*/static_cast<const ARIntens*>(getAbstractRepresentation());
}





