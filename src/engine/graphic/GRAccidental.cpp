/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002-2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>

#include "ARNote.h"
#include "GRAccidental.h"
#include "GREvent.h"
#include "TagParameterString.h"

NVPoint GRAccidental::sRefposNone;
NVPoint GRAccidental::sRefposSharp;
NVPoint GRAccidental::sRefposDSharp;
NVPoint GRAccidental::sRefposCSharp;
NVPoint GRAccidental::sRefposFlat;
NVPoint GRAccidental::sRefposDFlat;
NVPoint GRAccidental::sRefposCFlat;
NVPoint GRAccidental::sRefposNatural;
NVPoint GRAccidental::sRefposCNatural;

NVPoint GRAccidental::sRefposQSharp;
NVPoint GRAccidental::sRefpos3QSharp;
NVPoint GRAccidental::sRefposQFlat;
NVPoint GRAccidental::sRefpos3QFlat;

using namespace std;

//____________________________________________________________________________________
GRAccidental::GRAccidental( GREvent * sngnot, float notebreite, float inAccidentalID, float p_size, float curLSPACE)
	: offsetset(false)
{
	initialize(sngnot, p_size);
 	setAccidental( inAccidentalID, notebreite, curLSPACE);
}

//____________________________________________________________________________________
GRAccidental::GRAccidental( float detune, bool cautionnary, float p_size )
	: offsetset(false)
{
	initialize(0, p_size);
	float n = detune / 0.5f;		// count of quarter tone in detune
	int id = int(n);
	float remain = n - id;
	if (remain > 0.25) id += 1;
	else if ((detune < 0) && (remain < -0.25)) id -= 1;
	mSymbol = quarters2symbol (id);
	if (cautionnary) mSymbol = getCautionary(mSymbol);
	setAccidentalLayout (0, LSPACE);
}

//____________________________________________________________________________________
GRAccidental::GRAccidental()
	: offsetset(false)
{
	initialize(0, 1);
	mSymbol = kNoneSymbol;
	setAccidentalLayout (0, LSPACE);
}

//____________________________________________________________________________________
void GRAccidental::initialize( GREvent * sngnot, float p_size)
{
	mColRef = 0;
    
    if (sngnot) {
		mGrStaff = sngnot->getGRStaff();
		mAccidentalSize = sngnot->getSize();
		offset = sngnot->getOffset();
		const unsigned char * tmpcolref = sngnot->getColRef();

		if (tmpcolref) {
			mColRef = new unsigned char[4];
			mColRef[0] = tmpcolref[0];
			mColRef[1] = tmpcolref[1];
			mColRef[2] = tmpcolref[2];
			mColRef[3] = tmpcolref[3];
		}
	}
	else
        mAccidentalSize = p_size;
}

//____________________________________________________________________________________
GRAccidental::~GRAccidental()
{
	delete [] mColRef;
    mColRef = 0;
}

//____________________________________________________________________________________
void GRAccidental::setStyleNone()	{ mAccidentalSize = 0; }

//____________________________________________________________________________________
void GRAccidental::OnDraw(VGDevice & hdc) const
{
//cout << "GRAccidental::OnDraw refpos: " << getReferencePosition().x << " offset: " << offset.x << endl;
	if (mAccidentalSize)
		GRNotationElement::OnDraw(hdc);
}

//____________________________________________________________________________________
unsigned int GRAccidental::quarters2symbol(int quarters) const
{
	if (quarters > 4)  quarters = 4;
	if (quarters < -4) quarters = -4;
	switch (quarters) {
		case 1:		return kQSharpSymbol;
		case 2:		return kSharpSymbol;
		case 3:		return k3QSharpSymbol;
		case 4:		return kDSharpSymbol;
		case -1:	return kQFlatSymbol;
		case -2:	return kFlatSymbol;
		case -3:	return k3QFlatSymbol;
		case -4:	return kDFlatSymbol;
		case 0:		return kNaturalSymbol;
	}
	return 0; // unknown type
}

//____________________________________________________________________________________
unsigned int GRAccidental::accidentalDetunedID2symbol(float inAccidentalID) const
{
	int id = int(inAccidentalID);			// old id
	int quarters = ARNote::detune2Quarters(inAccidentalID - id);	// detune in rounded quarter tones

	bool cautionary;
	int acc = accidentalID2acc (id, cautionary);
	if (acc == kNotAnID) {
		if (!quarters) return 0;
		else acc = 0;
	}
	acc  *= 2;	// accidents in quarter tones
	unsigned int symbol = quarters2symbol(acc + quarters);
	return cautionary ? getCautionary (symbol) : symbol;
}

//____________________________________________________________________________________
int GRAccidental::accidentalID2acc(int inAccidentalID, bool& cautionary) const
{
	cautionary = false;
	switch (inAccidentalID)
	{
		case 1:		return 1;
		case 2:		return 2;
		case 3:		cautionary = true; return 1;
		case -1:	return -1;
		case -2:	return -2;
		case -3:	cautionary = true; return -1;
		case 10:	cautionary = true; return 0;
		case -10:	return 0;
	}
	return kNotAnID; // unknown type
}

//____________________________________________________________________________________
unsigned int GRAccidental::accidentalID2symbol(int inAccidentalID) const
{
	switch (inAccidentalID)
	{
		case 1:		return kSharpSymbol;
		case 2:		return kDSharpSymbol;
		case 3:		return kCauSharpSymbol;
		case -1:	return kFlatSymbol;
		case -2:	return kDFlatSymbol;
		case -3:	return kCauFlatSymbol;
		case 10:	return kCNaturalSymbol;
		case -10:	return kNaturalSymbol;
	}
	return 0; // unknown type
}

//____________________________________________________________________________________
void GRAccidental::setAccidentalLayout(float notebreite, float curLSPACE)
{
	const float extent = mSymbol ? GetSymbolExtent( mSymbol ) : 0;
//	const float halfExtent = extent * 0.5f;
	float halfExtent = extent * 0.5f;

	mBoundingBox.left = -halfExtent;
	mBoundingBox.right = halfExtent;

//  the following looks to be useless since changing the values doesn't change events bounding boxes
	switch (mSymbol)
	{
		case kSharpSymbol:
		case kQSharpSymbol:
		case k3QSharpSymbol:
#ifndef SMUFL
		case kCau3QSharpSymbol:
#endif
			mBoundingBox.top = -1.5f*curLSPACE;
			mBoundingBox.bottom = 1.52f*curLSPACE;
			break;
		case kDSharpSymbol:
#ifndef SMUFL
		case kCauDSharpSymbol:
#endif
			mBoundingBox.top = -curLSPACE;
			mBoundingBox.bottom = curLSPACE;
			break;
#ifndef SMUFL
		case kCauSharpSymbol:
			mBoundingBox.top = -1.5f*curLSPACE;
			mBoundingBox.right = 2.43f*curLSPACE-halfExtent;
			mBoundingBox.bottom = 1.52f*curLSPACE;
			break;
		case kCNaturalSymbol:
			mBoundingBox.top = -1.4f*curLSPACE;
//			mBoundingBox.right = 2.38f*curLSPACE-halfExtent;
			mBoundingBox.bottom = 1.4f*curLSPACE;
			break;
#endif
		case kFlatSymbol:
		case kQFlatSymbol:
		case k3QFlatSymbol:
		case kDFlatSymbol:
#ifndef SMUFL
		case kCauFlatSymbol:
		case kCauQFlatSymbol:
		case kCau3QFlatSymbol:
		case kCauDFlatSymbol:
#endif
			mBoundingBox.top = -1.76f*curLSPACE;
			mBoundingBox.bottom = 0.71f*curLSPACE;
			break;
//		case AC_DFLAT:
//			mBoundingBox.top = -curLSPACE;
//			mBoundingBox.bottom = curLSPACE;
//			break;
//		case AC_CFLAT:
//			mBoundingBox.top = -1.2f*curLSPACE;
////			mBoundingBox.right = 2.26f*curLSPACE-halfExtent;
//			mBoundingBox.bottom = 1.19f*curLSPACE;
//			break;
		case kNaturalSymbol:
//		case kCNaturalSymbol:
			mBoundingBox.top = -1.4f*curLSPACE;
//			mBoundingBox.right = 0.7f*curLSPACE-halfExtent;
			mBoundingBox.bottom = 1.4f*curLSPACE;
			break;
	}

	mBoundingBox.left *= float(mAccidentalSize);
	mBoundingBox.top *= float(mAccidentalSize);
	mBoundingBox.right *= float(mAccidentalSize);
	mBoundingBox.bottom *= float(mAccidentalSize);

	halfExtent *= 1.2f;
	getRefPos().x = -halfExtent;

	mLeftSpace = (GCoord)(( halfExtent * mAccidentalSize ) + 0.2f * curLSPACE );
	mRightSpace = (GCoord)( halfExtent * mAccidentalSize );

	offset.x -= (GCoord) ((notebreite * 0.5f +	halfExtent + 0.2f * curLSPACE) * mAccidentalSize );
	if (notebreite)
		mLeftSpace += (GCoord) ((halfExtent + notebreite * 0.5f + 0.2f * curLSPACE) * mAccidentalSize );
}

//____________________________________________________________________________________
void GRAccidental::setAccidental(float inAccidentalID, float notebreite, float curLSPACE)
{
	mSymbol = accidentalDetunedID2symbol (inAccidentalID);
	setAccidentalLayout (notebreite, curLSPACE);
}

//____________________________________________________________________________________
unsigned int GRAccidental::getCautionary (unsigned int symbol) const
{
	switch (symbol) {
		case kNaturalSymbol	: return kCNaturalSymbol;
		case kSharpSymbol 	: return kCauSharpSymbol;
		case kDSharpSymbol 	: return kCauDSharpSymbol;
		case kFlatSymbol 	: return kCauFlatSymbol;
		case kDFlatSymbol 	: return kCauDFlatSymbol;
		case kQSharpSymbol	: return kCauQSharpSymbol;
		case k3QSharpSymbol	: return kCau3QSharpSymbol;
		case kQFlatSymbol	: return kCauQFlatSymbol;
		case k3QFlatSymbol	: return kCau3QFlatSymbol;
	}
	return 0;
}

//____________________________________________________________________________________
void GRAccidental::setCautionary(int noteoffset, float notebreite, float curLSPACE)
{
	unsigned int caution = getCautionary (mSymbol);
	if (caution) {
		mSymbol = caution;
		offset.x = (float)noteoffset;
		setAccidentalLayout (notebreite, curLSPACE);
	}
}

//____________________________________________________________________________________
void GRAccidental::setColor(const TagParameterString *tps)
{
    if (!mColRef)
        mColRef = new unsigned char[4];

    tps->getRGB(mColRef);
}

//____________________________________________________________________________________
void GRAccidental::setColRef(unsigned char *colRef)
{
    if (!mColRef)
        mColRef = new unsigned char[4];

    mColRef[0] = colRef[0];
    mColRef[1] = colRef[1];
    mColRef[2] = colRef[2];
    mColRef[3] = colRef[3];
}

//____________________________________________________________________________________
void GRAccidental::setAccidentalByQuarter(int quarters, int noteoffset, float notebreite, float curLSPACE)
{
	mSymbol = quarters2symbol (quarters);
	offset.x = (float)noteoffset;
	setAccidentalLayout (notebreite, curLSPACE);
}

//____________________________________________________________________________________
const NVPoint & GRAccidental::getReferencePosition() const
{
	return getRefPos();
}

//____________________________________________________________________________________
NVPoint & GRAccidental::getRefPos() const
{
	static NVPoint p;
	switch ( mSymbol )
	{
		case 0:		return sRefposNone;
		case kSharpSymbol:		return sRefposSharp;
		case kDSharpSymbol:		return sRefposDSharp;
		case kFlatSymbol:		return sRefposFlat;
		case kDFlatSymbol:		return sRefposDFlat;
		case kNaturalSymbol:	return sRefposNatural;
#ifndef SMUFL
		case kCauFlatSymbol:	return sRefposCFlat;
		case kCauSharpSymbol:	return sRefposCSharp;
		case kCNaturalSymbol :	return sRefposCNatural;
#endif
		case kQSharpSymbol	:	return sRefposQSharp;
		case k3QSharpSymbol	:	return sRefpos3QSharp;
		case kQFlatSymbol	:	return sRefposQFlat;
		case k3QFlatSymbol	:	return sRefpos3QFlat;
	}
	return p;
}

//____________________________________________________________________________________
void GRAccidental::addToOffset(const NVPoint & pt)
{
	offset += pt;
	// recalculate left and rightspace ....
	mLeftSpace -= pt.x;
}
