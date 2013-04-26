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
				case ARMusicalVoice::ERROR:
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
					
				case ARMusicalVoice::ERROR:
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
					
				case ARMusicalVoice::ERROR:
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


void GRTrill::OnDraw( VGDevice & hdc , float pos, int nVoice) const
{
//	GRStaff * staff;
//	fType = static_cast<ARTrill *>(mAbstractRepresentation)->getType();
//	staff = getGRStaff();
	
	if(fType==0)//TRILL=0
	{
		NVRect r = getBoundingBox();
		r += getPosition ();
		float left;
		float right;
		float lastPos = GRTrill::getLastPosX(nVoice);
		if(begin){
			GRNotationElement::OnDraw( hdc );
			if(fAccidental){
				NVRect rAcc = fAccidental->getBoundingBox();
				rAcc += fAccidental->getPosition();
				left = rAcc.right;
			}
			else
				left = r.right;
		}
		else{
			if(lastPos<pos)
				left = lastPos;
			else
				left = r.left - LSPACE;
		}
		right = pos;
		float x = left;
		int w = 1.35*LSPACE;
		while(x + w <=right){
			hdc.DrawMusicSymbol(x, r.bottom, 126);
			x +=w;
		}
		
		fAccidental->OnDraw(hdc);
		GRTrill::getLastPosX(nVoice) = x;
		
	}else{	
		GRNotationElement::OnDraw( hdc );
		fAccidental->OnDraw(hdc);
	}
}


void GRTrill::OnDraw( VGDevice & hdc) const
{
//	GRStaff * staff;
//	fType = static_cast<ARTrill *>(mAbstractRepresentation)->getType();
//	staff = getGRStaff();

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
	GREvent * ev = dynamic_cast<GREvent *>(caller);
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

float & GRTrill::getLastPosX(int i){
	static std::map<int, float> lastPosX;
	return lastPosX[i];
}
