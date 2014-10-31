/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003, 2004 	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
using namespace std;

#include "ARMusicalObject.h"	// For DURATIONS_

#include "GRNoteDot.h"
#include "GREvent.h"
#include "GuidoDefs.h"

NVPoint GRNoteDot::refpos;

// ------------------------------------------------------------------------
GRNoteDot::GRNoteDot( GREvent * inNote, const TYPE_DURATION & duration, 
						float notebreite ) : mColRef( 0 )
{
	mDots = DurationToDotCount( duration );
	GRNoteDotInit( inNote, notebreite );
}

// ------------------------------------------------------------------------
GRNoteDot::GRNoteDot( GREvent * inNote, float notebreite, int inDotCount ) 
						: mColRef( 0 )
{
	mDots = inDotCount;
	GRNoteDotInit( inNote, notebreite );
}

// ------------------------------------------------------------------------
void 
GRNoteDot::GRNoteDotInit( GREvent * inNote, float notebreite )
{
	mBoundingBox.left = (GCoord)(notebreite * 0.5f + LSPACE * 0.33f );
	mBoundingBox.top = (GCoord)(LSPACE/8);
	mBoundingBox.bottom = (GCoord)(LSPACE * 0.33f );
	mBoundingBox.right = (GCoord)(notebreite * 0.5f + LSPACE * 0.33f + LSPACE * 0.25f);

	size = inNote->getSize();

	const unsigned char * tmpcolref = inNote->getColRef();
	if (tmpcolref)
	{
		  mColRef = new unsigned char[4];
		  mColRef[0] = tmpcolref[0];
		  mColRef[1] = tmpcolref[1];
		  mColRef[2] = tmpcolref[2];
		  mColRef[3] = tmpcolref[3];
	}

	mOffset = inNote->getOffset();

	mLeftSpace = 0;
	if (mDots == 1)
	{
		mRightSpace = (GCoord)(notebreite * size);
	}
	else if (mDots == 2)
	{
		mRightSpace = (GCoord)(1.5f * notebreite * size);
	}
	
	mSymbol = kNoteDotSymbol;

	refpos.x = 0; //  (int) (notewidth/2 + 0.1*LSPACE);
	refpos.y = 0; // (int) (0.2 * LSPACE);

	// setPosition(position);
}

// ------------------------------------------------------------------------
GRNoteDot::~GRNoteDot()
{
	delete [] mColRef;
    mColRef = 0;
}

// ------------------------------------------------------------------------
void GRNoteDot::GGSOutput() const
{
}

// ------------------------------------------------------------------------
void GRNoteDot::OnDraw(VGDevice & hdc) const
{
	int n = mDots;
	if (!n ) return;
	if(!mDraw)
		return;
	float scale = getSize();
	float offset = LSPACE * 0.5f * scale;
	float dx = LSPACE * 0.5f * (scale - 1);
	while (n-- > 0) {
		GRNotationElement::OnDrawSymbol( hdc, mSymbol, dx);
		dx += offset;
	}	
}

// ------------------------------------------------------------------------
void GRNoteDot::print() const
{
}

// ------------------------------------------------------------------------
int	GRNoteDot::DurationToDotCount( const TYPE_DURATION & duration )
{
	// WARNING ! the method expect to receive normalized durations
	switch (duration.getNumerator()) {
		case 3: return 1;
		case 7: return 2;
		case 13: return 3;
		default: return 0;
	}
}

void  GRNoteDot::adjustHorizontalDotPosition(float inNoteSize, ARTHead::HEADSTATE inHeadState, GDirection inGlobalStemDirection)
{
    mOffset.x += 45;

	if (inGlobalStemDirection == dirDOWN)
	{
		if (inHeadState == ARTHead::LEFT)
		{
			mOffset.x -= inNoteSize * 55; //hardcoded
		}
	}
	else if (inGlobalStemDirection == dirUP)
	{
		if (inHeadState == ARTHead::RIGHT)
		{
			mOffset.x += inNoteSize * 55; //hardcoded
		}
	}
}