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
// GRDrRenz.cpp: implementation of class GRDrRenz.
//
//////////////////////////////////////////////////////////////////////

#include "GRDrRenz.h"
#include "Bitmap.h"
#include "ARDrRenz.h"
#include "GuidoDefs.h"

//////////////////////////////////////////////////////////////////////
// construction/deconstruction
//////////////////////////////////////////////////////////////////////

GRDrRenz::GRDrRenz( ARDrRenz * par )
	: GRTagARNotationElement(/*dynamic cast<ARMusicalObject *>*/(par), LSPACE)
{
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;

	// no Spring here!
	mNeedsSpring = 1;
	mFace = new Bitmap("KAI");
	if (mFace)
	{
	  mBoundingBox.right = (GCoord)(mFace->GetProportionalWidth((4*LSPACE)));
	  mBoundingBox.bottom = (GCoord)(4*LSPACE);
	}
	else
	{
		mBoundingBox.right = 0;
		mBoundingBox.bottom = 0;
	}

	// obsolete
	// spacing = 0;
	// spacing = LSPACE;

	mLeftSpace = 0;
	mRightSpace = (GCoord)mBoundingBox.right;

	// no reference position ...
}

GRDrRenz::~GRDrRenz()
{
	delete mFace;
}

void GRDrRenz::print() const
{
}

void GRDrRenz::OnDraw( VGDevice & hdc) const
{
	if (mFace)
	{
		ARDrRenz * myar = static_cast<ARDrRenz *>(mAbstractRepresentation);

		if (myar && myar->getInverse() == -1)
		{
			NVRect where;
			where.left = mPosition.x + mFace->GetProportionalWidth((4*LSPACE));
			where.top = mPosition.y;
			where.right = -mFace->GetProportionalWidth((4*LSPACE));
			where.bottom = (GCoord)(4*LSPACE);
			
			mFace->OnDraw(hdc,where); 
		}
		else
			mFace->OnDrawH(hdc, mPosition, 4*LSPACE);
	}
}
