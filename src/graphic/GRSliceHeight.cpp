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
/* renz
	Needed more parameters in sliceheight; for once I need keep track
	of the boundingRectTop of the first and boundingRectBottom of the
	last staff. I also need a minHeight (which is set by a begin slice).
	The original idea that I should just remember the maximum heights of the
	staves did not work, because the height is determined by positions assigned
	to the staves be GRSystemSlice. So now the height-vector is just a
	position vector for the staves of the slice.
*/


#include "GRSliceHeight.h" 
#include "GRSystemSlice.h" 
#include "GRStaff.h" 
#include "kf_ivect.h"

GRSliceHeight::GRSliceHeight() : mHVector(0)
{
	boundingRectTop = 0;
	boundingRectBottom = 0;
	minHeight = 0;
}

GRSliceHeight::~GRSliceHeight()
{
}

#include <iostream>
/** Adds a slice to the current slice-height we are friends 
	of GRSystemSlice so we can directly access the staves.
*/
void GRSliceHeight::AddSystemSlice( const GRSystemSlice * slc, bool doboundingrect )
{
	if (slc == 0) return;
	int j;
	int first = 1;

	float laststaffposy = 0;
	float lastvectposy = 0;
	float staffposy = 0;
	float vectposy = 0;
	float cury = 0;

	for (j=slc->mStaffs->GetMinimum();j<=slc->mStaffs->GetMaximum();++j)
	{		
		GRStaff * tmpstaff = slc->mStaffs->Get(j);		
		if (tmpstaff)
		{
			vectposy = mHVector.Get(j);
			float newvectposy = vectposy;
			if (!doboundingrect)
			{
				staffposy = tmpstaff->getPosition().y;
				if (first)
				{
					// The first one just saves the part
					// below ... the upper bounding rectangle
					// is not concerned.
					first = 0;
				}
				else
				{
					if (staffposy - laststaffposy > vectposy - lastvectposy)
					{
						newvectposy = lastvectposy + staffposy - laststaffposy;
						mHVector.Set(j,float(newvectposy));
					}
				}
				
				laststaffposy = staffposy;
				lastvectposy = newvectposy;
			}
			else
			{
				// we do the bounding-rectangle thing ...
				// this is actually not needed here : the bounding box update is
				// already computed by GRSystemSlice::Finish
//				tmpstaff->updateBoundingBox();
				// the boundingBoxPreview method has been temporary use here to test code restructuration
//				tmpstaff->boundingBoxPreview();
				const NVRect & myrect = tmpstaff->getBoundingBox();
				if (!first)
				{
					if (cury - myrect.top > vectposy)
						mHVector.Set(j, float(cury - myrect.top));
					
					cury += myrect.Height();
				}
				else 
				{
					first = 0;
					cury = myrect.bottom;
				}
			}

			if (j == slc->mStaffs->GetMaximum())
			{
				float tmpbr = tmpstaff->getBoundingBox().bottom;
				if (tmpbr > boundingRectBottom)
					boundingRectBottom = tmpbr;

			}
			if (j == slc->mStaffs->GetMinimum())
			{
				float tmpbr = tmpstaff->getBoundingBox().top;
				if (tmpbr < boundingRectTop)
					boundingRectTop = tmpbr;
			}
		}
	}

	if (doboundingrect)
	{
		// it might be that the slice is a beginning slice
		// that has not yet set the staff positions,
		// in this case we have to set the minimum height

		float tmpheight = float(slc->getBoundingBox().Height());
		if (getHeight() < tmpheight)
		{
			minHeight = tmpheight;
		}
	}
}

float GRSliceHeight::getHeight( bool notoprect ) const
{
//	int imin = mHVector.GetMinimum();
	int imax = mHVector.GetMaximum();
	float res = mHVector.Get(imax) + boundingRectBottom;
	if (!notoprect)
		res -= boundingRectTop;
	if (res < minHeight)
		res = minHeight;
	return res;
}

float GRSliceHeight::getPosition(int i) const
{
	return mHVector.Get(i);
}
