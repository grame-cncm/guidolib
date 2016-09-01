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

#include <iostream>
using namespace std;

#include "GRFlag.h"
#include "GRGlobalStem.h"
#include "GREvent.h"
#include "ARMusicalObject.h"	// for DURATIONs

NVPoint GRFlag::sRefpos;

GRFlag::GRFlag(GRGlobalStem * gstem, const TYPE_DURATION & duration, GDirection stemdir, float stemlength) 
    : mColRef (NULL), fStemdir(stemdir), fStemlength(stemlength)
{
	fNotebreite = 60;	// Hardcoded
	mOffset = gstem->getOffset();

    configureForChord(gstem, duration);
}

GRFlag::GRFlag( GREvent * sngnot, const TYPE_DURATION & duration, GDirection stemdir, float stemlength, float notebreite) 
	: mColRef (NULL), fStemdir(stemdir), fStemlength(stemlength), fNotebreite(notebreite)
{
	mOffset = sngnot->getOffset();
	setColRef(sngnot->getColRef());

    configureForSingleNote(sngnot, duration);
}

GRFlag::GRFlag(const TYPE_DURATION & duration)
{
	mColRef = NULL;

	// now we have to get the type ...
	mSymbol = NONE;
	if (duration==DURATION_8 || duration == DURATION_3_16 || duration == DURATION_7_32)
		  mSymbol = H8U;
	else if(duration==DURATION_16 || duration == DURATION_3_32 || duration == DURATION_7_64)
			mSymbol = H16U;
	else if(duration == DURATION_32 || duration == DURATION_3_64 || duration == DURATION_7_128)
			mSymbol =  H32U;
	else if (duration == DURATION_64 || duration == DURATION_3_128 || duration == DURATION_7_256)
			mSymbol = H64U;
}

GRFlag::~GRFlag() 
{
	delete [] mColRef;
    mColRef = 0;
}

void GRFlag::configureForChord(GRGlobalStem *gstem, const TYPE_DURATION& duration) {
	mFlagOn   = true;
	mStraight = false;

	mSize   = gstem->getSize();
 	mOffset = gstem->getOffset();
	
	initialize (duration, fStemdir, fNotebreite);

	if (duration == DURATION_32 || duration == DURATION_3_64 || duration == DURATION_7_128)
		fStemlength = gstem->changeStemLength((float)(fStemlength + 0.7f * LSPACE));
	else if (duration == DURATION_64 || duration == DURATION_3_128 || duration == DURATION_7_256)
		fStemlength = gstem->changeStemLength((float)(fStemlength + 1.4 * LSPACE));

	// the stemlength is not changed by size parameter!
	if (fStemdir == dirUP)
		mOffset.y += (GCoord) (- fStemlength);
	else if (fStemdir == dirDOWN)
		mOffset.y += (GCoord) (  fStemlength);
}

void GRFlag::configureForSingleNote(GREvent *sngnot, const TYPE_DURATION& duration) {
	mFlagOn   = true;
	mStraight = false;

	mSize   = sngnot->getSize();
	mOffset = sngnot->getOffset();

	initialize (duration, fStemdir, fNotebreite);

	if (duration == DURATION_32 || duration == DURATION_3_64 || duration == DURATION_7_128)
		fStemlength = sngnot->changeStemLength((float)(fStemlength + 0.7f * LSPACE));
	else if (duration == DURATION_64 || duration == DURATION_3_128 || duration == DURATION_7_256)
		fStemlength = sngnot->changeStemLength((float)(fStemlength + 1.4 * LSPACE));

	// the stemlength is not changed by size parameter!
	if (fStemdir == dirUP)
        mOffset.y -= fStemlength;
	else if (fStemdir == dirDOWN)
        mOffset.y += fStemlength;
}

void GRFlag::initialize(const TYPE_DURATION & duration, GDirection stemdir, float notebreite) 
{
	mFlagOn = true;
	mStraight = 0;
	
	// now we have to get the type ...
	mSymbol = NONE;

    if (duration==DURATION_8 || duration == DURATION_3_16 || duration == DURATION_7_32) {
		if (stemdir == dirUP)		 mSymbol = H8U;
		else if (stemdir == dirDOWN) mSymbol = H8D;
	}
	else if(duration==DURATION_16 || duration == DURATION_3_32 || duration == DURATION_7_64) {
		if (stemdir  == dirUP)		 mSymbol = H16U;
		else if (stemdir == dirDOWN) mSymbol = H16D;
	}
	else if(duration == DURATION_32 || duration == DURATION_3_64 || duration == DURATION_7_128) {
		if (stemdir == dirUP)		 mSymbol =  H32U;
		else if (stemdir == dirDOWN) mSymbol =  H32D;		
		
	}
	else if (duration == DURATION_64 || duration == DURATION_3_128 || duration == DURATION_7_256) {
		if (stemdir == dirUP)		 mSymbol = H64U;
		else if (stemdir == dirDOWN) mSymbol = H64D;
	}

	mLeftSpace = 0;
	mRightSpace = 0;

	// this must be independant on size !
	sRefpos.x = - 30; // HARDCODED!
	sRefpos.y = 0;

	calcFlagExtent( notebreite ); // calculate the spaces .
}

void GRFlag::OnDraw(VGDevice & hdc) const
{
	if (!mDraw)
        return;

	if ( mSymbol == NONE )
        return;

	if (!mFlagOn)
        return;

	if (!mStraight) {
		// this is the regular case ...
		GRNotationElement::OnDraw(hdc);
	}
	else {
		// this is the straight case ..
		// TODO
	}
}

void GRFlag::GGSOutput() const
{
}

int GRFlag::getNumFaehnchen() const
{
	switch (mSymbol)
	{
		case H8U :
		case H8D : return 1;
		case H16U:
		case H16D: return 2;
		case H32U:
		case H32D: return 3;
		case H64D:
		case H64U: return 4;
		default : return 0;
	}
}

void GRFlag::setFlagOnOff(bool p)
{
	mFlagOn = p;

	if (mFlagOn == false) {
		mLeftSpace = 0;
		mRightSpace = 0;
	}
	else {
		const float notebreite = (-sRefpos.x * 2);
		calcFlagExtent( notebreite );
	}
}

void GRFlag::changeStemLength(GREvent * sngnot, float inLen,
							  GDirection stemdir)
{
	mOffset.y = sngnot->getOffset().y;

	if (stemdir == dirUP)
		mOffset.y -= inLen;
	else if (stemdir == dirDOWN)
		mOffset.y += inLen;
}

void	
GRFlag::calcFlagExtent( float inNoteBreite )
{
	switch ( mSymbol ) 
	{
		case H8U:
		case H16U:
		case H32U:
		case H64U:
			mRightSpace = float(1.5) * inNoteBreite * mSize;
			mLeftSpace = 0;
			break;
	
		case H8D:
		case H16D:
		case H32D:
		case H64D:
			mRightSpace = 0;
			mLeftSpace = float(0.5) * inNoteBreite * mSize;
			break;
	}
}


void GRFlag::setColRef(const unsigned char * tmpcolref)
{
	if( tmpcolref == 0 ) return;
	
	if (mColRef == 0)
		mColRef = new unsigned char[4];

	mColRef[0] = tmpcolref[0];
	mColRef[1] = tmpcolref[1];
	mColRef[2] = tmpcolref[2];
	mColRef[3] = tmpcolref[3];
}



