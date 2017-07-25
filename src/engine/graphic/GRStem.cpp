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

#include "ARMusicalObject.h"	// for DURATIONs
#include "ARNote.h"				// To get noteFormat parameters
#include "GRStem.h"
#include "GRGlobalStem.h"
#include "GREvent.h"
#include "GRMusic.h" 
#include "VGDevice.h" 
#include "secureio.h"
#include "GRSingleNote.h"
#include "GRStdNoteHead.h"

#include "GUIDOEngine.h"		// for AddGGSOutput

using namespace std;

NVPoint GRStem::sRefpos;

GRStem::GRStem(GRGlobalStem * gstem) : mColRef(NULL), fOffsetStartPosition(0), fDrawActivated(true),
    fNoteHeadType(kFullHeadSymbol), fHeadOrientation(ARTHead::NOTSET), fLastHeadOrientation(ARTHead::NOTSET)
{
	mStemDir = dirOFF;
	setColRef(gstem->getColRef());
    mOffset = gstem->getOffset();
	mSize   = gstem->getSize();
	sRefpos.x = -30;		// HARDCODED !!!
}

GRStem::GRStem(GREvent * sngnot, const TYPE_DURATION& dur, GDirection dir, float length, float notebreite)
	: mColRef(NULL), fNoteHeadType(kFullHeadSymbol), fHeadOrientation(ARTHead::NOTSET),
      fLastHeadOrientation(ARTHead::NOTSET), fSngnot(sngnot), fDir(dir), fLength(length), fNotebreite(notebreite)
{
    configureStem(dur);
}

GRStem::~GRStem()
{
	delete [] mColRef;
    mColRef = 0;
}

void GRStem::configureStem(const TYPE_DURATION& dur)
{
    // Init variables (or reinit if not called from constructor)
    fOffsetStartPosition = 0;
    fDrawActivated = true;

    if (dur >= DURATION_1) {
		mStemDir = dirOFF;
		return; 
	}

	mStemDir = fDir;
	mStemLen = fLength;

	setColRef(fSngnot->getColRef());
	mOffset = fSngnot->getOffset();
	mSize = fSngnot->getSize();

	// dependant on the direction, I have different types (do I?)
	mLeftSpace = 0;
	mRightSpace = 0;

	if (fDir == dirUP)			mBoundingBox.top = (GCoord)(-mStemLen);
	else if (fDir == dirDOWN)	mBoundingBox.bottom = (GCoord)(mStemLen);
	sRefpos.x = (GCoord)(- fNotebreite * 0.5f);

	GRSingleNote *singleNote = dynamic_cast<GRSingleNote *>(fSngnot);
    if (singleNote) {
        fHeadOrientation = singleNote->getHeadState();
		GRStdNoteHead *noteHead = singleNote->getNoteHead();
		if (noteHead) {
			noteHead->setGlobalStemDirection(fDir);
			fNoteHeadType = noteHead->getSymbol();

			// - Adjust stem length if it's a cross notehead
			if (fNoteHeadType == kFullXHeadSymbol) {
				setFirstSegmentDrawingState(false);

				if (fDir == dirUP)			setOffsetStartPosition(4);
				else if (fDir == dirDOWN)	setOffsetStartPosition(-4);
			}
            // - Adjust stem length if it's a triangle notehead
			else if (fNoteHeadType == kFullTriangleHeadSymbol || fNoteHeadType == kHalfTriangleHeadSymbol) {
				if (fDir == dirUP)			setOffsetStartPosition(47);
				else if (fDir == dirDOWN)	setFirstSegmentDrawingState(false);
			}
            // - Adjust stem length if it's a reversed triangle notehead
			else if (fNoteHeadType == kFullReversedTriangleHeadSymbol || fNoteHeadType == kHalfReversedTriangleHeadSymbol) {
				if (fDir == dirUP)			setFirstSegmentDrawingState(false);
				else if (fDir == dirDOWN)	setOffsetStartPosition(-47);
			}
		}
	}
}

void GRStem::setStemLength(float inLen)
{
	mStemLen = inLen;
	mBoundingBox.top = 0;
	mBoundingBox.bottom = 0;
	if (mStemDir == dirUP)
		mBoundingBox.top = (GCoord)(-mStemLen);
	else if (mStemDir == dirDOWN)
		mBoundingBox.bottom = (GCoord)(mStemLen);
}
	
void GRStem::setStemDir(GDirection dir)
{
	mStemDir = dir;
	mBoundingBox.top = 0;
	mBoundingBox.bottom = 0;
	if (mStemDir == dirUP)
		mBoundingBox.top = (GCoord)(- mStemLen);
	else if (mStemDir == dirDOWN)
		mBoundingBox.bottom = (GCoord)(mStemLen);
}

// TODO: draw only one scaled symbol
void GRStem::OnDraw( VGDevice & hdc ) const
{
    DrawWithGlyph(hdc);
    //DrawWithLine(hdc);
}

//-------------------------------------------------------------------
// computes the start y position offset
// it depends on the beam direction and on the note head orientation
//-------------------------------------------------------------------
float GRStem::GetStartYOffset (bool up, float lineSpace) const
{
	float yoffset = 0;
	bool notset = (fHeadOrientation == ARTHead::NOTSET && fLastHeadOrientation == ARTHead::NOTSET) || fHeadOrientation != ARTHead::NOTSET;
	bool reverse 		= fHeadOrientation == ARTHead::REVERSE;
	bool lastreverse 	= fLastHeadOrientation == ARTHead::REVERSE;
	ARTHead::HEADSTATE headside = up ? ARTHead::RIGHT : ARTHead::LEFT;

	if (fDrawActivated && (fNoteHeadType == kFullHeadSymbol || fNoteHeadType == kHalfNoteHeadSymbol)) {
		if (notset) {
		   if (reverse || fHeadOrientation == headside)
				yoffset = lineSpace / 6;
			else     
				yoffset = - lineSpace / 6;
		}
		else {
			if (lastreverse || fLastHeadOrientation == headside)
				yoffset = lineSpace / 6;
			else     
				yoffset = - lineSpace / 6;
		}
    }
    return up ? yoffset : -yoffset;
}

//-----------------------------------------------------------------
// effective stem drawing
// the starty position must be the note head position
//-----------------------------------------------------------------
void GRStem::DrawStem( VGDevice & hdc, unsigned int symbol1, unsigned int symbol2, float starty, float length ) const
{
	const float spaceBySize = LSPACE * mSize;
	const float halfSpaceBySize = 0.5f * spaceBySize;
	if (fDrawActivated) GRNotationElement::DrawSymbol(hdc, symbol1, 0, starty);
	
	int steps = int(length / halfSpaceBySize);
	float offset = -halfSpaceBySize;
	if (length < 0) {
		steps = -steps;
		offset = -halfSpaceBySize;
	}
	starty = fOffsetStartPosition + offset;
	for (int i=1; i < steps; i++) {
		GRNotationElement::DrawSymbol( hdc, symbol2, 0, starty );
		starty += offset;
		length -= offset;
	}
//	if (length) {
//		starty -= length;
//		GRNotationElement::DrawSymbol( hdc, symbol2, 0, starty );
//	}
}

//-------------------------------------------------------------------
void GRStem::DrawWithGlyph( VGDevice & hdc ) const
{
    if(!mDraw || !mShow)		return;
	if (mStemDir == dirOFF)		return;
	if (mSize < kMinNoteSize)	return;			// size is too small, don't draw

	// - Setup colors
	const unsigned char * colref = getColRef();
	const VGColor prevTextColor = hdc.GetFontColor();
  	if (colref) hdc.SetFontColor( VGColor( colref ));

	// - Setup text align 
	hdc.SetFontAlign(getTextAlign());

	// dependant on the direction, we do different things ...
	const float spaceBySize = LSPACE * mSize;
	const float halfSpaceBySize = 0.5f * spaceBySize;

	float stemOffsetY = GetStartYOffset (mStemDir == dirUP, spaceBySize);
	if (mStemDir == dirUP) {

		unsigned int tmpSymbol =  kStemUp1Symbol;
        if (fDrawActivated) GRNotationElement::DrawSymbol(hdc, tmpSymbol, 0, stemOffsetY);
		
		tmpSymbol = kStemUp2Symbol;  // wrong in EPS font...
		// Draws until the length has been completed ...
		float offsy = - halfSpaceBySize + fOffsetStartPosition;
		while (- offsy < mStemLen) {
			if ((spaceBySize - offsy ) > mStemLen) {
				offsy = - mStemLen /* * mSize)*/ + spaceBySize;
				GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy);
				break;
			}
			GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy );
			offsy -= halfSpaceBySize;
		}
	}
	else if (mStemDir == dirDOWN) {
		unsigned int tmpSymbol = kStemDown1Symbol;
        if (fDrawActivated) GRNotationElement::DrawSymbol(hdc, tmpSymbol, 0, stemOffsetY);
		
		tmpSymbol = kStemDown2Symbol;
		// Draws until the length has been completed ...		
		float offsy = halfSpaceBySize + fOffsetStartPosition;
		while( offsy < mStemLen ) {
			if(( offsy + spaceBySize ) > mStemLen ) {
				offsy = mStemLen - spaceBySize;
				GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy );
				break;
			}
			GRNotationElement::DrawSymbol( hdc, tmpSymbol, 0, offsy );
			offsy += halfSpaceBySize;
		}
	}
	// - Restore context
	if (colref) hdc.SetFontColor( prevTextColor );  //(TODO: in a parent method)
}

// Not used for now, but use it would avoid some stem length problems (cf. regression-tests/pending/badStemLength.gmn)
void GRStem::DrawWithLine( VGDevice & hdc ) const
{
	if(!mDraw || !mShow)
		return;
	if (mStemDir == dirOFF)
        return;
	if (mSize < kMinNoteSize)
        return;			// size is too small, don't draw

	// - Setup colors
	const unsigned char *colref = getColRef();
	
	const float spaceBySize = LSPACE * mSize;
	const float halfSpaceBySize = 0.5f * spaceBySize;

    if (mStemLen >= halfSpaceBySize) {
        float width       = 6.0f * mSize;
        float noteXextent = 60 * mSize; // REM: harcoded
        float noteYextent = 50 * mSize; // REM: harcoded
        float x1, y1, x2, y2;

        if (colref)
            hdc.PushPenColor(VGColor(colref));

        hdc.PushPenWidth(width);

        if (mStemDir == dirUP)
        {
            x1 = mPosition.x + mOffset.x + noteXextent * 0.5f - width * 0.5f;
            y1 = mPosition.y + mOffset.y - width * 0.5f - (noteYextent * 0.5f / 6);
            x2 = mPosition.x + mOffset.x + noteXextent * 0.5f - width * 0.5f;
            y2 = mPosition.y + mOffset.y - mStemLen + width * 0.5f;
        }
        else // if (mStemDir == dirDOWN)	assume dir is down
        {
            x1 = mPosition.x + mOffset.x - noteXextent * 0.5f + width * 0.5f,
            y1 = mPosition.y + mOffset.y + width * 0.5f + (noteYextent * 0.5f / 6),
            x2 = mPosition.x + mOffset.x - noteXextent * 0.5f + width * 0.5f,
            y2 = mPosition.y + mOffset.y + mStemLen - width * 0.5f;
        }

        hdc.Line(x1, y1, x2, y2);

        hdc.PopPenWidth();

        if (colref)
            hdc.PopPenColor();
    }
}

void GRStem::GGSOutput() const
{
	if (mStemDir == dirOFF) return;

	/* this has to be changed so the correct
	   GGS-String is produced ... */

	char buffer[200];
	if (mStemDir == dirUP)
	{
		snprintf(buffer, 200, "\\draw_stem<%ld,%d,%d,%d>\n",
			getID(),
			(int)(mPosition.x + mOffset.x + (GCoord)(sRefpos.x * mSize) 
				+ (GCoord) (60 * mSize)+ ggsoffsetx) , // HARDCODED 60 is the size of the notehead ...
			(int)(mPosition.y + mOffset.y + (GCoord)(sRefpos.y * mSize) + ggsoffsety), 
			(int)mStemLen);

		AddGGSOutput(buffer);
	}
	else if (mStemDir == dirDOWN)
	{
		snprintf(buffer, 200, "\\draw_stem<%ld,%d,%d,%d>\n",
			getID(),
			(int)(mPosition.x + mOffset.x + (GCoord)(sRefpos.x * mSize) + ggsoffsetx),
			(int)(mPosition.y + mOffset.y + (GCoord)(sRefpos.y * mSize) + ggsoffsety),
			(int)(-mStemLen));

		AddGGSOutput(buffer);
	}
}

const NVPoint & GRStem::getReferencePosition() const
{
	return sRefpos;
}

void GRStem::setColRef( const unsigned char * inColor )
{
	if( inColor == 0 ) return;
	
	if (mColRef == 0)
		mColRef = new unsigned char [4];

	mColRef[0] = inColor[0];
	mColRef[1] = inColor[1];
	mColRef[2] = inColor[2];
	mColRef[3] = inColor[3];
}
