/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2003 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <iostream>
using namespace std;

#include "GRStdNoteHead.h"
#include "GRMusic.h"
#include "GRSingleNote.h"

#include "GRDefine.h"

GRStdNoteHead::GRStdNoteHead( GREvent * sngnot, const TYPE_DURATION & inDur, GDirection inStemDirection ) :
singleStemDirection(inStemDirection),
	globalStemDirection(dirAUTO),
	halfExtent(0),
	mBracketsType(None)
{
	mSize = sngnot->getSize();
	const unsigned char * tmpcolref = sngnot->getColRef();
	if (tmpcolref) {
		mColRef = new unsigned char[4];
		mColRef[0] = tmpcolref[0];
		mColRef[1] = tmpcolref[1];
		mColRef[2] = tmpcolref[2];
		mColRef[3] = tmpcolref[3];
	}
	mOffset = sngnot->getOffset();
	mStyle = sngnot->getStyle();

	// -- Find the symbol that must be used, depending on the style and the duration of the event.
	mSymbol = kNoneSymbol;

	// - First, look if the kind of the note head has been explicitely specified by the style parameter.
	if(( mStyle == "" ) || ( mStyle == "standard" ))
		mSymbol = durationToHeadSymbol( inDur );
	else {
		const bool full = (inDur < DURATION_2);
		if( mStyle == "diamond" )
			mSymbol = full ? kFullDiamondHeadSymbol : kHalfDiamondHeadSymbol;
		else if( mStyle == "x" )
			mSymbol = full ? kFullXHeadSymbol : kHalfXHeadSymbol;
		else if( mStyle == "square" )
			mSymbol = full ? kFullSquareHeadSymbol : kHalfSquareHeadSymbol;
		else if( mStyle == "round" )
			mSymbol = full ? kFullRoundHeadSymbol : kHalfRoundHeadSymbol;
		else if( mStyle == "triangle" )
			mSymbol = full ? kFullTriangleHeadSymbol : kHalfTriangleHeadSymbol;
		else if( mStyle == "reversedTriangle" )
			mSymbol = full ? kFullReversedTriangleHeadSymbol : kHalfReversedTriangleHeadSymbol;
		else if( mStyle == "noHeads" ) {
#ifndef WIN32	// transparency not supported on WIN32
			if (!mColRef) mColRef = new unsigned char[4];
			if (mColRef) mColRef[3] = 255;
			mSymbol = durationToHeadSymbol( inDur );
#else
			mSymbol = kNoneSymbol;
#endif
		}

		// - Noteheads ornaments

		else if( mStyle == "()" )
		{
			mBracketsType = Round; // - Round brackets
			mSymbol = durationToHeadSymbol( inDur );
		}
		else if( mStyle == "[]" )
		{
			mBracketsType = Square; // - Square brackets
			mSymbol = durationToHeadSymbol( inDur );
		}
		else if( mStyle == "<>" )
		{
			mBracketsType = Angled; // - Angled brackets
			mSymbol = durationToHeadSymbol( inDur );
		}

		// - Ornament for non-standard notehead types
		// - Round brackets
		else if( mStyle == "(diamond)" )
		{
			mBracketsType = Round;
			mSymbol = full ? kFullDiamondHeadSymbol : kHalfDiamondHeadSymbol;
		}
		else if( mStyle == "(x)" )
		{
			mBracketsType = Round;
			mSymbol = full ? kFullXHeadSymbol : kHalfXHeadSymbol;
		}
		else if( mStyle == "(square)" )
		{
			mBracketsType = Round;
			mSymbol = full ? kFullSquareHeadSymbol : kHalfSquareHeadSymbol;
		}
		else if( mStyle == "(round)" )
		{
			mBracketsType = Round;
			mSymbol = full ? kFullRoundHeadSymbol : kHalfRoundHeadSymbol;
		}
		else if( mStyle == "(triangle)" )
		{
			mBracketsType = Round;
			mSymbol = full ? kFullTriangleHeadSymbol : kHalfTriangleHeadSymbol;
		}
		else if( mStyle == "(reversedTriangle)" )
		{
			mBracketsType = Round;
			mSymbol = full ? kFullReversedTriangleHeadSymbol : kHalfReversedTriangleHeadSymbol;
		}

		// - Square brackets
		else if( mStyle == "[diamond]" )
		{
			mBracketsType = Square;
			mSymbol = full ? kFullDiamondHeadSymbol : kHalfDiamondHeadSymbol;
		}
		else if( mStyle == "[x]" )
		{
			mBracketsType = Square;
			mSymbol = full ? kFullXHeadSymbol : kHalfXHeadSymbol;
		}
		else if( mStyle == "[square]" )
		{
			mBracketsType = Square;
			mSymbol = full ? kFullSquareHeadSymbol : kHalfSquareHeadSymbol;
		}
		else if( mStyle == "[round]" )
		{
			mBracketsType = Square;
			mSymbol = full ? kFullRoundHeadSymbol : kHalfRoundHeadSymbol;
		}
		else if( mStyle == "[triangle]" )
		{
			mBracketsType = Square;
			mSymbol = full ? kFullTriangleHeadSymbol : kHalfTriangleHeadSymbol;
		}
		else if( mStyle == "[reversedTriangle]" )
		{
			mBracketsType = Square;
			mSymbol = full ? kFullReversedTriangleHeadSymbol : kHalfReversedTriangleHeadSymbol;
		}

		// - Angled brackets
		else if( mStyle == "<diamond>" )
		{
			mBracketsType = Angled;
			mSymbol = full ? kFullDiamondHeadSymbol : kHalfDiamondHeadSymbol;
		}
		else if( mStyle == "<x>" )
		{
			mBracketsType = Angled;
			mSymbol = full ? kFullXHeadSymbol : kHalfXHeadSymbol;
		}
		else if( mStyle == "<square>" )
		{
			mBracketsType = Angled;
			mSymbol = full ? kFullSquareHeadSymbol : kHalfSquareHeadSymbol;
		}
		else if( mStyle == "<round>" )
		{
			mBracketsType = Angled;
			mSymbol = full ? kFullRoundHeadSymbol : kHalfRoundHeadSymbol;
		}
		else if( mStyle == "<triangle>" )
		{
			mBracketsType = Angled;
			mSymbol = full ? kFullTriangleHeadSymbol : kHalfTriangleHeadSymbol;
		}
		else if( mStyle == "<reversedTriangle>" )
		{
			mBracketsType = Angled;
			mSymbol = full ? kFullReversedTriangleHeadSymbol : kHalfReversedTriangleHeadSymbol;
		}
	}

	// - Calculate the bounding box
	if (mSymbol != kNoneSymbol) {
		halfExtent = GetSymbolExtent( mSymbol ) * 0.5f;
		mBoundingBox.left = (GCoord)( -halfExtent * 1.05f );
		mBoundingBox.right = (GCoord)( halfExtent * 1.1f );
		mBoundingBox.top = 	(GCoord)( -LSPACE * 0.5f );
		mBoundingBox.bottom = (GCoord)( LSPACE * 0.5f );
	}
	else mBoundingBox.Set( 0, 0, 0, 0 );

	// set the referenceposition ....
	mLeftSpace = (GCoord) (halfExtent * getSize());
	mRightSpace = (GCoord) (halfExtent * getSize());

	switch (mSymbol)
	{
	case kFullHeadSymbol:
	case kHalfNoteHeadSymbol:
	case kWholeNoteHeadSymbol:
	case kLongaHeadSymbol:
	case kFullDiamondHeadSymbol:
	case kHalfDiamondHeadSymbol:
		sRefPosNotehead.x = (GCoord)(- halfExtent);
		break;

	case kFullXHeadSymbol:
	case kHalfXHeadSymbol:
	case kFullSquareHeadSymbol:
	case kHalfSquareHeadSymbol:
	case kFullRoundHeadSymbol:
	case kHalfRoundHeadSymbol:
		if (singleStemDirection == dirUP)
			sRefPosNotehead.x = (GCoord)(- halfExtent) + 5;
		else if (singleStemDirection == dirDOWN)
			sRefPosNotehead.x = (GCoord)(- halfExtent) - 5;
		else
			sRefPosNotehead.x = (GCoord)(- halfExtent);
		break;

	case kFullTriangleHeadSymbol:
	case kHalfTriangleHeadSymbol:
	case kFullReversedTriangleHeadSymbol:
	case kHalfReversedTriangleHeadSymbol:
		if (singleStemDirection == dirUP)
			sRefPosNotehead.x = (GCoord)(- halfExtent) + 2;
		else if (singleStemDirection == dirDOWN)
			sRefPosNotehead.x = (GCoord)(- halfExtent) - 2;
		else
			sRefPosNotehead.x = (GCoord)(- halfExtent);
		break;
	}

	sRefPosNotehead.y = 0;
}

GRStdNoteHead::~GRStdNoteHead() {}

unsigned int GRStdNoteHead::durationToHeadSymbol( const TYPE_DURATION & inDur ) const
{
	unsigned int outSymbol;

	// - Quarter notes and less
	if( inDur < DURATION_2 )
		outSymbol = kFullHeadSymbol;

	// - Half notes
	else if( inDur < DURATION_1 )
		/* was: if( duration == DURATION_2 
		|| duration == DURATION_3_4 
		|| duration == DURATION_7_8) */
		outSymbol = kHalfNoteHeadSymbol;

	// - Whole notes (semi-breves)	
	else if( inDur < Fraction( 2, 1 ))
		outSymbol = kWholeNoteHeadSymbol;

	// - Longa notes (breves)
	else if( inDur == Fraction( 2, 1 ))
		outSymbol = kLongaHeadSymbol;

	// - Unknown, use a full head
	else
		outSymbol = kFullHeadSymbol;

	return outSymbol;
}


NVPoint GRStdNoteHead::getNoteHeadPosition() const
{
	NVPoint pos (mPosition);
	pos += mOffset;
	return pos;
}

const NVPoint & GRStdNoteHead::getPosition() const
{
	return GRNoteHead::getPosition();
}

const NVPoint & GRStdNoteHead::getReferencePosition() const
{
	if (mSymbol != kNoneSymbol)
		return sRefPosNotehead;
	else
		return sRefposNone;
}

void GRStdNoteHead::GGSOutput() const
{
	// so that the notehead is put at the correct position
	// this is a workaround ....
	// tell Christian.
	GRNoteHead::GGSOutput();
}

void GRStdNoteHead::OnDraw( VGDevice & hdc ) const
{
	// the note head -> the note
	if (mSymbol == kNoneSymbol) return;

	GRNoteHead::OnDraw(hdc);

	if (mBracketsType == Round)
	{
		int xOffset = -25;
		int yOffset = 55;
		GRNotationElement::OnDrawSymbol(hdc, kRoundLeftBracket, xOffset, yOffset, 0);
		xOffset = 50;
		GRNotationElement::OnDrawSymbol(hdc, kRoundRightBracket, xOffset, yOffset, 0);
	}
	else if (mBracketsType == Square)
	{
		int xOffset = -25;
		int yOffset = 55;
		GRNotationElement::OnDrawSymbol(hdc, kSquareLeftBracket, xOffset, yOffset, 0);
		xOffset = 50;
		GRNotationElement::OnDrawSymbol(hdc, kSquareRightBracket, xOffset, yOffset, 0);
	}
	else if (mBracketsType == Angled)
	{
		int xOffset = -55;
		int yOffset = 55;
		GRNotationElement::OnDrawSymbol(hdc, kAngledLeftBracket, xOffset, yOffset, 0);
		xOffset = 70;
		GRNotationElement::OnDrawSymbol(hdc, kAngledRightBracket, xOffset, yOffset, 0);
	}
}

void GRStdNoteHead::print() const
{
}

unsigned int GRStdNoteHead::getSymbol() const
{
	return mSymbol;
}

void GRStdNoteHead::adjustPositionForChords(ARTHead::HEADSTATE inHeadstate, GDirection inGlobalStemDirection)
{
	globalStemDirection = inGlobalStemDirection;

	if (globalStemDirection == dirDOWN)
	{
		if (inHeadstate == ARTHead::LEFT)
		{
			switch (mSymbol)
			{
			case kFullHeadSymbol:
			case kHalfNoteHeadSymbol:
			case kWholeNoteHeadSymbol:
			case kLongaHeadSymbol:
			case kFullDiamondHeadSymbol:
			case kHalfDiamondHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) + 5;
				break;

			case kFullXHeadSymbol:
			case kHalfXHeadSymbol:
			case kFullSquareHeadSymbol:
			case kHalfSquareHeadSymbol:
			case kFullRoundHeadSymbol:
			case kHalfRoundHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) + 10;
				break;

			case kFullTriangleHeadSymbol:
			case kHalfTriangleHeadSymbol:
			case kFullReversedTriangleHeadSymbol:
			case kHalfReversedTriangleHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) + 7;
				break;
			}
		}
		else if (inHeadstate == ARTHead::RIGHT)
		{
			switch (mSymbol)
			{
			case kFullXHeadSymbol:
			case kHalfXHeadSymbol:
			case kFullSquareHeadSymbol:
			case kHalfSquareHeadSymbol:
			case kFullRoundHeadSymbol:
			case kHalfRoundHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) - 5;
				break;

			case kFullTriangleHeadSymbol:
			case kHalfTriangleHeadSymbol:
			case kFullReversedTriangleHeadSymbol:
			case kHalfReversedTriangleHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) - 2;
				break;
			}
		}
	}
	else if (globalStemDirection == dirUP)
	{
		if (inHeadstate == ARTHead::RIGHT)
		{
			switch (mSymbol)
			{
			case kFullHeadSymbol:
			case kHalfNoteHeadSymbol:
			case kWholeNoteHeadSymbol:
			case kLongaHeadSymbol:
			case kFullDiamondHeadSymbol:
			case kHalfDiamondHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) - 5;
				break;

			case kFullXHeadSymbol:
			case kHalfXHeadSymbol:
			case kFullSquareHeadSymbol:
			case kHalfSquareHeadSymbol:
			case kFullRoundHeadSymbol:
			case kHalfRoundHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) - 10;
				break;

			case kFullTriangleHeadSymbol:
			case kHalfTriangleHeadSymbol:
			case kFullReversedTriangleHeadSymbol:
			case kHalfReversedTriangleHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) - 7;
				break;
			}
		}
		else if (inHeadstate == ARTHead::LEFT)
		{
			switch (mSymbol)
			{
			case kFullXHeadSymbol:
			case kHalfXHeadSymbol:
			case kFullSquareHeadSymbol:
			case kHalfSquareHeadSymbol:
			case kFullRoundHeadSymbol:
			case kHalfRoundHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) + 5;
				break;

			case kFullTriangleHeadSymbol:
			case kHalfTriangleHeadSymbol:
			case kFullReversedTriangleHeadSymbol:
			case kHalfReversedTriangleHeadSymbol:
				sRefPosNotehead.x = (GCoord)(- halfExtent) + 2;
				break;
			}
		}
	}
}

void GRStdNoteHead::adjustHorizontalPosition()
{
	if (globalStemDirection == dirDOWN)
	{
		switch (mSymbol)
		{
		case kFullXHeadSymbol:
		case kHalfXHeadSymbol:
		case kFullSquareHeadSymbol:
		case kHalfSquareHeadSymbol:
		case kFullRoundHeadSymbol:
		case kHalfRoundHeadSymbol:
			sRefPosNotehead.x = (GCoord)(- halfExtent) - 5;
			break;

		case kFullTriangleHeadSymbol:
		case kHalfTriangleHeadSymbol:
		case kFullReversedTriangleHeadSymbol:
		case kHalfReversedTriangleHeadSymbol:
			sRefPosNotehead.x = (GCoord)(- halfExtent) - 2;
			break;
		}
	}
	else if (globalStemDirection == dirUP)
	{
		switch (mSymbol)
		{
		case kFullXHeadSymbol:
		case kHalfXHeadSymbol:
		case kFullSquareHeadSymbol:
		case kHalfSquareHeadSymbol:
		case kFullRoundHeadSymbol:
		case kHalfRoundHeadSymbol:
			sRefPosNotehead.x = (GCoord)(- halfExtent) + 5;
			break;

		case kFullTriangleHeadSymbol:
		case kHalfTriangleHeadSymbol:
		case kFullReversedTriangleHeadSymbol:
		case kHalfReversedTriangleHeadSymbol:
			sRefPosNotehead.x = (GCoord)(- halfExtent) + 2;
			break;
		}
	}
}

void GRStdNoteHead::setGlobalStemDirection(GDirection inGlobalStemDirection)
{
	if (globalStemDirection != inGlobalStemDirection)
	{
		globalStemDirection = inGlobalStemDirection;

		this->adjustHorizontalPosition();
	}
}