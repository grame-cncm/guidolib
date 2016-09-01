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

#include "ARDrHoos.h"
#include "GRDrHoos.h"
#include "GuidoDefs.h"

#include "Bitmap.h"

GRDrHoos::GRDrHoos( ARDrHoos * par )
	: GRTagARNotationElement(/*dynamic cast<ARMusicalObject *>*/(par), LSPACE)
{
	mBoundingBox.left = 0;
	mBoundingBox.top = 0;

	// no Spring here!
	mNeedsSpring = 1;

	face = new Bitmap("HOLGER");

	if (face)
	{
	  mBoundingBox.right = (GCoord)(face->GetProportionalWidth(4*LSPACE));
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
	mRightSpace = mBoundingBox.right;


	// no reference position ...
}

GRDrHoos::~GRDrHoos()
{
	delete face;
}

void GRDrHoos::OnDraw( VGDevice & hdc) const
{
	// Here now will be something painted ...
	if (face && mDraw)
	{
		ARDrHoos * myar = static_cast<ARDrHoos *>(mAbstractRepresentation);

		if (myar && myar->getInverse() == -1)
		{
			/* NVPoint np;
			np.x = position.x + face->getProportionalWidth(4*LSPACE);
			np.y = position.y + 4*LSPACE;
			
			face->OnDrawH(hdc,np,-4*LSPACE); */

			NVRect where;
			where.left = mPosition.x + face->GetProportionalWidth((4*LSPACE));
			where.top = mPosition.y;
			where.right = - face->GetProportionalWidth((4*LSPACE));
			where.bottom = 4*LSPACE;
			
			face->OnDraw( hdc, where ); 
		}
		else
			face->OnDrawH( hdc, mPosition, (4*LSPACE));
	}
}
