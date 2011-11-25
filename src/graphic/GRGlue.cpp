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

#include "GRGlue.h"
#include "GRStaff.h"
#include "GuidoDefs.h"

GRGlue::GRGlue(GRStaff * grstf, int _isstart)	// (JB) Something obscur: sometimes, GRGlue is
												// created with _isstart = -1, why is that ?
												// I'd like to replace it by a bool, but I can't...
{
	mGrStaff = grstf;

	mIsStart = _isstart;
	mNeedsSpring = 1;

	float curLSPACE = LSPACE;
	if (mGrStaff)
		curLSPACE = mGrStaff->getStaffLSPACE();
	if (mIsStart == 1)
	{
		mSConst = SCONST_GLUESTART;

		// this does not work?
		mRightSpace = GCoord(curLSPACE);
	}
	else
	{
		mSConst = 100; // SCONST_GLUENOSTART;
		mRightSpace = GCoord(curLSPACE);
		// leftSpace = 5*LSPACE;
	}

	mPartner = NULL;

	// this is the left and right margin that need
	// to be maintained in all cases ...

	mBoundingBox.right = 0;
	mBoundingBox.left = 0;
	/* if (isstart)
		mBoundingBox.right = LSPACE;
	else
		mBoundingBox.left = -LSPACE;
		*/
}

GRGlue::~GRGlue()
{
	if (mPartner)
		mPartner->removeAssociation(this);
}

void GRGlue::tellPosition(GObject * caller, const NVPoint & point)
{
	setPosition(point);
}
