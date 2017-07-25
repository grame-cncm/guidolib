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
	float n = detune / 0.5;		// count of quarter tone in detune
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
//void GRAccidental::setPosition(const NVPoint & inPos)
//{
//	GRNotationElement::setPosition(inPos);
//}

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
//void GRAccidental::GGSOutput() const
//{
//	GRNotationElement::GGSOutput();
//}

//____________________________________________________________________________________
unsigned int GRAccidental::quarters2symbol(int quarters) const
{
	if (quarters > 4)  quarters = 4;
	if (quarters < -4) quarters = -4;
	switch (quarters) {
		case 1:		return AC_QSHARP;
		case 2:		return AC_SHARP;
		case 3:		return AC_3QSHARP;
		case 4:		return AC_DSHARP;
		case -1:	return AC_QFLAT;
		case -2:	return AC_FLAT;
		case -3:	return AC_3QFLAT;
		case -4:	return AC_DFLAT;
		case 0:		return AC_NATURAL;
	}
	return AC_NONE; // unknown type
}

//____________________________________________________________________________________
unsigned int GRAccidental::accidentalDetunedID2symbol(float inAccidentalID) const
{
	int id = int(inAccidentalID);			// old id
	int quarters = ARNote::detune2Quarters(inAccidentalID - id);	// detune in rounded quarter tones

	bool cautionary;
	int acc = accidentalID2acc (id, cautionary);
	if (acc == kNotAnID) {
		if (!quarters) return AC_NONE;
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
		case 1:		return AC_SHARP;
		case 2:		return AC_DSHARP;
		case 3:		return AC_CSHARP;
		case -1:	return AC_FLAT;
		case -2:	return AC_DFLAT;
		case -3:	return AC_CFLAT;
		case 10:	return AC_CNATURAL;
		case -10:	return AC_NATURAL;
	}
	return AC_NONE; // unknown type
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
		case AC_SHARP:
		case AC_QSHARP:
		case AC_3QSHARP:
		case AC_C3QSHARP:
			mBoundingBox.top = -1.5f*curLSPACE;
			mBoundingBox.bottom = 1.52f*curLSPACE;
			break;
		case AC_DSHARP:
		case AC_CDSHARP:
			mBoundingBox.top = -curLSPACE;
			mBoundingBox.bottom = curLSPACE;
			break;
		case AC_CSHARP:
			mBoundingBox.top = -1.5f*curLSPACE;
			mBoundingBox.right = 2.43f*curLSPACE-halfExtent;
			mBoundingBox.bottom = 1.52f*curLSPACE;
			break;
//		case AC_CNATURAL:
//			mBoundingBox.top = -1.4f*curLSPACE;
////			mBoundingBox.right = 2.38f*curLSPACE-halfExtent;
//			mBoundingBox.bottom = 1.4f*curLSPACE;
//			break;
		case AC_FLAT:
		case AC_QFLAT:
		case AC_3QFLAT:
		case AC_DFLAT:
		case AC_CFLAT:
		case AC_CQFLAT:
		case AC_C3QFLAT:
		case AC_CDFLAT:
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
		case AC_NATURAL:
		case AC_CNATURAL:
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
		case AC_NONE	: return AC_CNATURAL;
		case AC_SHARP 	: return AC_CSHARP;
		case AC_DSHARP 	: return AC_CDSHARP;
		case AC_FLAT 	: return AC_CFLAT;
		case AC_DFLAT 	: return AC_CDFLAT;
		case AC_NATURAL : return AC_CNATURAL;
		case AC_QSHARP	: return AC_CQSHARP;
		case AC_3QSHARP	: return AC_C3QSHARP;
		case AC_QFLAT	: return AC_CQFLAT;
		case AC_3QFLAT	: return AC_C3QFLAT;
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
		case AC_NONE:		return sRefposNone;
		case AC_SHARP:		return sRefposSharp;
		case AC_DSHARP:		return sRefposDSharp;
		case AC_CSHARP:		return sRefposCSharp;
		case AC_FLAT:		return sRefposFlat;
		case AC_DFLAT:		return sRefposDFlat;
		case AC_CFLAT:		return sRefposCFlat;
		case AC_NATURAL:	return sRefposNatural;
		case AC_CNATURAL :	return sRefposCNatural;

		case AC_QSHARP	:	return sRefposQSharp;
		case AC_3QSHARP	:	return sRefpos3QSharp;
		case AC_QFLAT	:	return sRefposQFlat;
		case AC_3QFLAT	:	return sRefpos3QFlat;
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
