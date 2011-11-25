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

void GRDrHoos::print() const
{
}

void GRDrHoos::OnDraw( VGDevice & hdc) const
{
	// Here now will be something painted ...
	if (face)
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
