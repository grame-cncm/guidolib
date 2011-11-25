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

#include <iostream>
using namespace std;

#include "GRStdNoteHead.h"
#include "GRMusic.h"
#include "GRSingleNote.h"

#include "GRDefine.h"

NVPoint GRStdNoteHead::sRefposFull;
NVPoint GRStdNoteHead::sRefposHalf;
NVPoint GRStdNoteHead::sRefposWhole;
NVPoint GRStdNoteHead::sRefposLonga;
NVPoint GRStdNoteHead::sRefposDiamond;
NVPoint GRStdNoteHead::sRefposCrossHead;


GRStdNoteHead::GRStdNoteHead( GREvent * sngnot, const TYPE_DURATION & inDur )
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
			mSymbol = kXHeadSymbol;
		else if( mStyle == "square" ) // TODO: look if the "square" symbol exists.
			mSymbol = full ? kFullHeadSymbol : kHalfNoteHeadSymbol;	

		else if( mStyle == "noHeads" ) {
#ifndef WIN32	// transparency not supported on WIN32
			if (!mColRef) mColRef = new unsigned char[4];
			if (mColRef) mColRef[3] = 255;
			mSymbol = durationToHeadSymbol( inDur );
#else
			mSymbol = kNoneSymbol;
#endif
		}
		else if( mStyle == "()" )
			mSymbol = durationToHeadSymbol( inDur );
		else if( mStyle == "[]" )
			mSymbol = durationToHeadSymbol( inDur );
		else if( mStyle == "<>" )
			mSymbol = durationToHeadSymbol( inDur );
		// to be implemented: S represents one of the above styles
		else if( mStyle == "(S)" )
			mSymbol = durationToHeadSymbol( inDur );
		else if( mStyle == "[S]" )
			mSymbol = durationToHeadSymbol( inDur );
		else if( mStyle == "<S>" )
			mSymbol = durationToHeadSymbol( inDur );
	}

	float halfExtent = 0;

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

	switch (mSymbol) {
		case kFullHeadSymbol:
			sRefposFull.x = (GCoord)(- halfExtent);
			sRefposFull.y = 0;
			break;

		case kHalfNoteHeadSymbol:
			sRefposHalf.x = (GCoord)(- halfExtent);
			sRefposHalf.y = 0;
			break;
					
		case kWholeNoteHeadSymbol:
			sRefposWhole.x = (GCoord)(- halfExtent);
			sRefposWhole.y = 0;
			break;
		
		case kLongaHeadSymbol:
			sRefposLonga.x = (GCoord)(- halfExtent);
			sRefposLonga.y = 0;
			break;
		
		case kFullDiamondHeadSymbol:
		case kHalfDiamondHeadSymbol:
			sRefposDiamond.x = (GCoord)(- halfExtent) + 2;
			sRefposDiamond.y = 0;
			break;

		case kXHeadSymbol:
			sRefposCrossHead.x = (GCoord)(- halfExtent);
			sRefposCrossHead.y = 0;
			break;
	}
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
	switch (mSymbol)
	{
		case kFullHeadSymbol:		return sRefposFull;		break;
		case kHalfNoteHeadSymbol:	return sRefposHalf;		break;
		case kWholeNoteHeadSymbol:	return sRefposWhole;	break;
		case kLongaHeadSymbol:		return sRefposLonga;	break;
		case kFullDiamondHeadSymbol:
		case kHalfDiamondHeadSymbol:return sRefposDiamond;	break;
		case kXHeadSymbol:			return sRefposCrossHead; break;
		default: 					return sRefposNone;		break;
	}
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
}

void GRStdNoteHead::print() const
{
}


