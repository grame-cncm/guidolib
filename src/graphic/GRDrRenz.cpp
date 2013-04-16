/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2013 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

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
