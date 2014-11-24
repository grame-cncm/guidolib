/*
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr


	-- Rules of ornaments --
	
	(1) Ornament symbols are placed above the staff regardless of stem direction
	(2) When two parts share a staff, an ornament symbol applied to the lower part is placed
		below the staff

*/

#include "ARTrill.h"
#include "GRAccidental.h"
#include "GRStaff.h"
#include "GRTrill.h"
#include "GRSingleNote.h"

// #include "NEPointerList.h"
#include "VGDevice.h"
#include "VGFont.h"
#include <iostream>
#include <map>


using namespace std;

GRTrill::GRTrill(GRStaff * inStaff, ARTrill * artrem ) : GRPTagARNotationElement(artrem)
{
	assert(inStaff);
	GRSystemStartEndStruct * sse= new GRSystemStartEndStruct;
	sse->grsystem = inStaff->getGRSystem(); 
	sse->startflag = GRSystemStartEndStruct::LEFTMOST;
	
	sse->p = (void *) getNewGRSaveStruct();
	mStartEndList.AddTail(sse);

	ARTrill::TYPE mytype = artrem->getType();
	bool ShowCautionaryAccidentals = artrem->getCautionary();
	ARMusicalVoice::CHORD_TYPE chordType = artrem->getChordType();
	ARMusicalVoice::CHORD_ACCIDENTAL chordAccidental = artrem->getChordAccidental();
	begin = artrem->getStatus();
	fShowTR = artrem->getShowTR();
	fDrawOnNoteHead = artrem->getAnchor();
	widthOfTilde = GObject::GetSymbolExtent(kTilde)*mTagSize;
	
	// - Creation of the accidental symbol -	
	switch (chordAccidental)
	{
		case ARMusicalVoice::NATURAL :
			fAccidental = new GRAccidental(0, 0, -10, 0.5f);
			break;
			
		case ARMusicalVoice::FLAT :
			fAccidental = new GRAccidental(0, 0, -1, 0.5f);
			break;
			
		case ARMusicalVoice::SHARP :
			fAccidental = new GRAccidental(0, 0, 1, 0.5f);
			break;
			
		case ARMusicalVoice::CAU_NATURAL :
			if (ShowCautionaryAccidentals)
				fAccidental = new GRAccidental(0, 0, 10, 0.5f);
			else
				fAccidental = new GRAccidental(0, 0, 0, 0.5f);
			break;
			
		case ARMusicalVoice::CAU_FLAT :
			if (ShowCautionaryAccidentals)
				fAccidental = new GRAccidental(0, 0, -3, 0.5f);
			else
				fAccidental = new GRAccidental(0, 0, 0, 0.5f);
			break;
		case ARMusicalVoice::CAU_SHARP :
			if (ShowCautionaryAccidentals)
				fAccidental = new GRAccidental(0, 0, 3, 0.5f);
			else
				fAccidental = new GRAccidental(0, 0, 0, 0.5f);
			break;
			
		default :
			fAccidental = new GRAccidental(0, 0, 0, 0.5f);
	}
	fAccidental->setOffsetX(this->getOffset().x + artrem->getadx());
	fAccidental->setOffsetY(this->getOffset().y - artrem->getady()); // this offset affects each type of ornament
	
	switch ( mytype )
	{
		case ARTrill::TRILL:
			switch (chordType)
			{
				case ARMusicalVoice::CHORDERROR:
					setTrillSymbol( kNoneSymbol );
					break;
					
				default :
					setupTrill();
			}
			break;

		case ARTrill::TURN:
			switch (chordType)
			{
				case ARMusicalVoice::DOWN:
					setupInvertedTurn();
					break;
					
				case ARMusicalVoice::DOWN_COMPLEX:
					setupInvertedTurnComplex();
					break;
					
				case ARMusicalVoice::UP_COMPLEX:
					setupTurnComplex();
					break;
					
				case ARMusicalVoice::CHORDERROR:
					setTrillSymbol( kNoneSymbol );
					break;
					
				default :
					setupTurn();
			}
			break;

		case ARTrill::MORD:
			switch (chordType)
			{
				case ARMusicalVoice::DOWN_SIMPLE:
					setupInvertedMord();
					break;
					
				case ARMusicalVoice::DOWN:
					setupPrallMordent();
					break;
					
				case ARMusicalVoice::UP:
					setupPrallPrall();
					break;
					
				case ARMusicalVoice::CHORDERROR:
					setTrillSymbol( kNoneSymbol );
					break;
					
				default :
					setupMord();
			}
			break;
	}
	
}

GRTrill::~GRTrill()
{
	delete fAccidental;
}


/** \brief Manage the drawing of trill line
*/
void GRTrill::OnDraw(VGDevice & hdc , float right, float noteY, int nVoice)
{
	VGColor oldColor = hdc.GetFontColor();
	
	if (fType == 0) { //TRILL=0
		NVRect r = mBoundingBox;
		r += getPosition();
		float left;
		float lastPos = GRTrill::getLastPosX(nVoice);

		// we check if the trill line is begining or continuing another

		if (begin) {
			// the 'tr' is drawn only at the begining of the trill, if the parameter 'tr' isn't set as false
			if (fShowTR)
				GRNotationElement::OnDraw(hdc);

			// in order to adapt the accidental to the size of the trill :
			fAccidental->setPosition(fAccidental->getPosition() + NVPoint(mBoundingBox.Width()/2*(mTagSize-1), - mBoundingBox.Height()/2*(mTagSize-1)));
			fAccidental->setSize(mTagSize/2);
			fAccidental->OnDraw(hdc);

			NVRect rAcc = fAccidental->getBoundingBox();
			rAcc += fAccidental->getPosition();
			float leftR = r.left + r.Width()*mTagSize;
			float leftA = rAcc.left + rAcc.Width()*mTagSize + fAccidental->getOffset().x - mTagOffset.x;

			// check the position of the accidental to know where to begin the trill line
			if (leftA > leftR && (fAccidental->getOffset().y - mTagOffset.y) > 0 && (fAccidental->getOffset().y - mTagOffset.y) < r.Height()+rAcc.Height())
				left = leftA;
			else
				left = leftR;
		}
		else {	// continue the line from the last position (or at the begining of a new system)
			if (lastPos < right)
				left = lastPos;
			else
				left = r.left - LSPACE;
		}

		// now we iterates the symbol kTilde as many times as posible from "left" to "right"
		float x = (left - r.left);

		if (!begin)
			x -= mTagOffset.x;
		else
			left += mTagOffset.x;

		while (left + widthOfTilde <= right) {
			if (fDrawOnNoteHead)
				GRNotationElement::OnDrawSymbol(hdc, kTilde, x, noteY - getPosition().y, mTagSize);
			else
				GRNotationElement::OnDrawSymbol(hdc, kTilde, x, 0, mTagSize);

			x    += widthOfTilde;
			left += widthOfTilde;
		}

		GRTrill::getLastPosX(nVoice) = left;		
	}
    else {	
		if (fShowTR)
			GRNotationElement::OnDraw(hdc);

		fAccidental->OnDraw(hdc);
	}
	
	hdc.SetFontColor(oldColor);
}


void GRTrill::OnDraw( VGDevice & hdc) const
{
	GRNotationElement::OnDraw( hdc );
	fAccidental->OnDraw(hdc);
}

const NVPoint & GRTrill::getReferencePosition() const
{
	return sRefPos;
}

void GRTrill::setupTrill()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kTrillSymbol);
	NVPoint offsetAccidental = NVPoint(0.5f * hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 1.17f;
	sRefPos = NVPoint(-mLeftSpace, 0);	
	mBoundingBox.Set(-mLeftSpace, -height, mRightSpace + 0.24f * LSPACE, 0);
}

void GRTrill::setupTurn()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kTurnSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.98f;
	sRefPos = NVPoint(-mLeftSpace, 0); 
	mBoundingBox.Set(-mLeftSpace, -height/2, mRightSpace, height/2);
}

void GRTrill::setupInvertedTurn()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kInvertedTurnSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.98f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height/2, mRightSpace, height/2);
}

void GRTrill::setupTurnComplex()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kTurnSymbol);
	NVPoint offsetAccidental = NVPoint(2*hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.98f;
	sRefPos = NVPoint(LSPACE * 1.5f, 0);
	mBoundingBox.Set(1.5f*LSPACE, -height/2, 2*mRightSpace+1.5f*LSPACE, height/2);
}

void GRTrill::setupInvertedTurnComplex()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kInvertedTurnSymbol);
	NVPoint offsetAccidental = NVPoint(2*hs, 0);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.98f;
	sRefPos = NVPoint(LSPACE * 1.5f, 0);
	mBoundingBox.Set(1.5f*LSPACE, -height/2, 2*mRightSpace+1.5f*LSPACE, height/2);
}

void GRTrill::setupMord()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kMordSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, -hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.97f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height, mRightSpace, 0);
}

void GRTrill::setupInvertedMord()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kInvertedMordSymbol);
	NVPoint offsetAccidental = NVPoint(-3*hs, -1.5f*hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 1.6f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height+0.36f * LSPACE, mRightSpace, 0.36f * LSPACE);
}

void GRTrill::setupPrallMordent()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kPrallMordentSymbol);
	NVPoint offsetAccidental = NVPoint(-4*hs, -hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 1.31f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height+0.19f*LSPACE, mRightSpace, 0.19f*LSPACE);
}

void GRTrill::setupPrallPrall()
{
	const float hs = LSPACE * 0.5f;
	setTrillSymbol (kPrallPrallSymbol);
	NVPoint offsetAccidental = NVPoint(-4*hs, -hs);
	fAccidental->addToOffset(offsetAccidental);
	
	const float height = LSPACE * 0.95f;
	sRefPos = NVPoint(-mLeftSpace, 0);
	mBoundingBox.Set(-mLeftSpace, -height, mRightSpace, 0);
}

void GRTrill::setTrillSymbol( unsigned int inSymbolID )
{
	mSymbol = inSymbolID;
	const float extent = GetSymbolExtent( inSymbolID );
	mLeftSpace = (extent * 0.5f);
	mRightSpace = (extent * 0.5f);
	}

//-------------------------------------------------------------------
void GRTrill::tellPosition( GObject * caller, const NVPoint & np )
{
	fType = static_cast<ARTrill *>(mAbstractRepresentation)->getType();
	GRSingleNote * ev = dynamic_cast<GRSingleNote *>(caller);
	if (ev)
	{
		const int dir = chooseDirection(ev);
		NVPoint newPoint (np);
		const bool upward = (dir == dirUP);

		if( upward )	// if stemsUp
		{
			if (newPoint.y >= ev->getStemLength())
				newPoint.y = -LSPACE;
			else
				newPoint.y -= (ev->getStemLength() + LSPACE);
		}
		else
		{
			newPoint.y = (newPoint.y > 0)? -LSPACE : newPoint.y - 1.5f*LSPACE;
		}
		setPosition(newPoint);

		// Accidental
		NVPoint accidentalPos = NVPoint(newPoint);
		accidentalPos.x += GetSymbolExtent(mSymbol)*0.5f + 0.4f*LSPACE;
		accidentalPos.y -= 1.5f*LSPACE;	
		fAccidental->setPosition(accidentalPos);
	}
    else {
        mDraw = false;
        fAccidental->setDrawOnOff(false);
    }
}


//----------------------------------------------------------------
void GRTrill::placeMord (GREvent * inParent, NVPoint & newPoint)
{
	// Checks the direction of the stems
	const GDirection dir = chooseDirection( inParent ); // dir = stemDir
	const GRStaff * staff = inParent->getGRStaff();
	const float currLSpace = staff->getStaffLSPACE();
	const float halfSpace = float(0.5) * currLSpace;
	
	if( dir == dirUP )
	{
		if (newPoint.y >= inParent->getStemLength())
		{
			newPoint.y = -halfSpace;
		}
		else
		{
			newPoint.y -= (halfSpace + inParent->getStemLength());
		}
	}
	else	// stemDown
	{
		if ( newPoint.y > 0)
		{
			newPoint.y = -halfSpace;
		}
		else
		{
			newPoint.y -= halfSpace;
		}
	}
}

//-------------------------------------------------------------------
GDirection GRTrill::chooseDirection (GREvent * inParent ) const
{
	const GRSingleNote * sng = dynamic_cast<GRSingleNote *>(inParent);
		GDirection direction;
		direction = sng->getStemDirection(); // was getDirection()
		return direction;
}

//-------------------------------------------------------------------
unsigned int GRTrill::getTextAlign() const
{
	return (VGDevice::kAlignLeft | VGDevice::kAlignBase);
}

//-------------------------------------------------------------------
/** \brief Memorize the last position of trill line for each voice
*/
float & GRTrill::getLastPosX(int i){
	static std::map<int, float> lastPosX;
	return lastPosX[i];
}