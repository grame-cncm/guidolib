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
