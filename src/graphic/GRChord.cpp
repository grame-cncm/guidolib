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

#include <typeinfo> 

#include "ARDotFormat.h"
#include "ARChord.h"
#include "ARRest.h"
#include "ARNote.h"
#include "TagParameterFloat.h"

#include "GRChord.h"
#include "GRNote.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GRStaff.h"
#include "GRStem.h"
#include "GRNoteDot.h"
#include "GRFlag.h"
#include "GRAccidental.h"
#include "GRStdNoteHead.h"
//#include "NEPointerList.h"

// ATTENTION what about the Staff!?
// It seems like GREvent does not know about 
// Staffs -> why is that?
GRChord::GRChord(GRStaff * grstaff,
		ARChord * archord,
		const TYPE_TIMEPOSITION & von,
		const TYPE_TIMEPOSITION & bis,
		const TYPE_TIMEPOSITION & dtempl)
	: GREvent(grstaff,archord,von,bis)
{

	mPosYTop = 0;
	mPosYBottom = 0;
	int first = 1;
	const float notebreite = 60;
	GuidoPos pos = archord->GetHeadPosition();
	while (pos)
	{
		ARMusicalEvent * arev = ARMusicalEvent::cast(archord->GetNext(pos));
		TYPE_DURATION nbis (bis);

		// different durations of notes are allowed within chords ...
		// not yet ...! -> different lengths are
		// ignored for now ...
		if (arev->getDuration() < bis)
			nbis = arev->getDuration();
			
		// Unterscheide zwischen Noten und Pausen ...
//		GREvent * grev;
		ARRest * arRest;
		ARNote * arNote = dynamic_cast<ARNote *>(arev);
		if( arNote )
		{
			// now we will build a note ...
			// (guild a notehead with accidentals ...)

			GRStdNoteHead * notehead = new GRStdNoteHead(this,nbis);

			float tmp = grstaff->getNotePosition( arNote->getPitch(), arNote->getOctave());
			notehead->setPosition(NVPoint(0,tmp));

			if (first)
			{
				mPosYTop = tmp;
				mPosYBottom = mPosYTop;
				first = 0;
			}
			else
			{
				if (tmp < mPosYTop)		mPosYTop = tmp;
				if (tmp > mPosYBottom)	mPosYBottom = tmp;
			}
			const float notebreite = ((notehead->getLeftSpace() + notehead->getRightSpace()) / mSize);

			AddTail( notehead );

			// dots
			createDots( nbis, notebreite, NVPoint(0,tmp));

			// accidentals
			mGrStaff->checkSystemBar (arNote->getRelativeTimePosition());
			AccList * mylist = grstaff->askAccidentals(arNote->getPitch(),
				arNote->getOctave(),arNote->getAccidentals());
			
			if (!mylist->empty())
			{
				GuidoPos pos = mylist->GetHeadPosition();
				while (pos)
				{
					GRAccidental * tmpacc = new GRAccidental(this, notebreite,
															mylist->GetNext(pos));
					tmpacc->setPosition(NVPoint(0,tmp));
					AddTail(tmpacc);
				}
			}
			delete mylist;

		} 
		else if ((arRest = dynamic_cast<ARRest *>(arev)) != 0 )
		{
//			grev = new GRSingleRest( grstaff, arRest, von, nbis, dtempl);
		}
	}

	const float twoLSpace = 2 * LSPACE;

	float tmpdist1 = twoLSpace - mPosYTop;
	float tmpdist2 = mPosYBottom - twoLSpace;

	if (mPosYTop <= twoLSpace && mPosYBottom <= twoLSpace )
		mStemDir = dirDOWN;
	else if (mPosYTop > twoLSpace && mPosYBottom > twoLSpace )
		mStemDir = dirUP;
	else if (tmpdist1 > tmpdist2)
	{
		mStemDir = dirUP;
	}
	else
		mStemDir = dirDOWN;

	// now we build a stem ...
	// do the direction upwards ....

	GRStem * stem = new GRStem( this, bis, mStemDir,
								 (mPosYBottom - mPosYTop + 3.5f * LSPACE), notebreite);

	mStemDir = stem->mStemDir;
	if (mStemDir == dirUP)
	{
		stem->setPosition(NVPoint(0,(GCoord)mPosYBottom));
		mPosition.y = (GCoord)mPosYBottom;
	}
	else
	{
		stem->setPosition(NVPoint(0,(GCoord)mPosYTop));
		mPosition.y = mPosYTop;
	}
	mStemLen = stem->mStemLen;

	AddTail(stem);

	// now we build the Flag ...
	// here we have to add the flags ...
	GRFlag * tmpflag = new GRFlag( this, bis,mStemDir,mStemLen, notebreite );

	if (mStemDir == dirUP)
		tmpflag->setPosition(NVPoint(0, mPosYBottom));
	else
		tmpflag->setPosition(NVPoint(0, mPosYTop));

	 AddTail(tmpflag);

	 // collision-detection ....
	 pos = First();
	 while (pos)
	 {
		 GRStdNoteHead * nh = dynamic_cast<GRStdNoteHead *>(GetNext(pos));
		 if (nh)
		 {
			 GuidoPos pos2 = pos;
			 while (pos2)
			 {
				GRStdNoteHead * nh2 = dynamic_cast<GRStdNoteHead *>(GetNext(pos2));
				if (nh2)
				{
					float dist = nh2->getPosition().y - nh->getPosition().y;
					if (dist == 0)
					{
						if (mStemDir == dirUP)
							nh2->setOffset(NVPoint(55,0)); // hardcoded
						else
							nh2->setOffset(NVPoint(-55,0));
					}
					else if (dist <0 && dist > - LSPACE)
					{
						// nh2 is max value
						if (mStemDir == dirUP)
						{
							if (nh->getOffset().x > 0)
							{}
							else
								nh2->setOffset(NVPoint(55,0));
						}
						else
						{
							if (nh2->getOffset().x < 0 )
							{}
							else
							{
								nh->setOffset(NVPoint(-55,0));
								break;
							}
						}						
					}
					else if (dist > 0 && dist < LSPACE)
					{
						if (mStemDir == dirUP)
						{
							if (nh2->getOffset().x > 0)
							{}
							else
							{
								nh->setOffset(NVPoint(55,0));
								break;
							}
						}
						else
						{
							if (nh->getOffset().x < 0)
							{}
							else
								nh2->setOffset(NVPoint(-55,0));
						}						
					}
				}
			 }
		 }
	 }
	updateBoundingBox();
}

void GRChord::print() const
{
}

void GRChord::GGSOutput() const
{
	GuidoPos pos = First();
	while (pos)
	{
		GRNotationElement * el = GetNext(pos);
		el->GGSOutput();
	}
}

void GRChord::OnDraw(VGDevice & hdc) const
{
// DrawBoundingBox( hdc, GColor( 230, 255, 255 )); // DEBUG


	const float lSpace = (float)LSPACE;
	
	// helplines ...
	// depending on mPosYBottom and mPosYTop ...
	const float xPos = ( -60 * 0.85f * (float)mSize );

	const float posY = getPosition().y;
	
	if (mPosYTop < lSpace * 0.5f )
	{
		// helplines above
		float tmpy = - lSpace;
		while (tmpy >= mPosYTop)
		{
			GRNotationElement::DrawSymbol(hdc, kLedgerLineSymbol, xPos, tmpy - posY );
			tmpy -= lSpace;
		}
	}
	if (mPosYBottom > 4.5f * lSpace )
	{
		// helplines below ...
		float tmpy = 5 * lSpace;
		while (tmpy <= mPosYBottom )
		{
			GRNotationElement::DrawSymbol(hdc, kLedgerLineSymbol, xPos, tmpy - posY );
			tmpy += lSpace;
		}
	}
	DrawSubElements( hdc );
}

/*
void GRChord::setPosition( const NVPoint & inPos )
{
	GREvent::setPosition( inPos );

	mElements.setHPosition( mPosition.x );

	if (mAssociated) // ties, slurs, tuplets, ...
		mAssociated->tellPosition(this, mPosition );

	updateBoundingBox();
}
*/
NVPoint GRChord::getTieStart() const
{
	NVPoint tmp;	// TODO
	return tmp;
}

NVPoint GRChord::getTieEnd() const
{
	NVPoint tmp;	// TODO
	return tmp;
}

void GRChord::updateBoundingBox()
{
	mLeftSpace = 0;
	mRightSpace = 0;

	mBoundingBox.Set( 0, 0, 0, 0 );

	GuidoPos pos = First();
	while (pos)
	{
		GRNotationElement * el = GetNext(pos);

		float eltLeft = el->getOffset().x - el->getLeftSpace();
		if( mLeftSpace < - eltLeft )
			mLeftSpace = - eltLeft;
		
		float eltRight = el->getOffset().x + el->getRightSpace();
		if ( mRightSpace < eltRight )
			mRightSpace = eltRight;

		const NVRect & br = el->getBoundingBox();

		// if (br.left < mBoundingBox.left)		mBoundingBox.left = br.left;
		// if (br.right > mBoundingBox.right)		mBoundingBox.right = br.right;
		mBoundingBox.Merge( br );
	}
}

// (JB) should return bool but may return -1 if "not implemented"
int GRChord::adjustLength(const TYPE_DURATION & ndur)
{
	GuidoPos pos = First();
	int ret;
	while (pos)
	{
		GREvent * ev = GREvent::cast( GetNext(pos));
		if (ev)
		{
			ret = ev->adjustLength(ndur);
			if (ret != 1)
			{
				// handle this?
			}
		}
	}
	
	mDurationOfGR = ndur;

	return 1;

}

float GRChord::changeStemLength( float inLen )
{
	// stemlength = p_length;
	GuidoPos pos = First();
	while (pos)
	{
		GRStem * stem = dynamic_cast<GRStem *>(GetNext(pos));
		if (stem)
		{
			stem->setStemLength( inLen );
			return (mStemLen = stem->mStemLen);
		}
	}
	return mStemLen;
}

int GRChord::getNumFaehnchen() const
{
	const GRFlag * tmp = getGRFlag();
	return tmp ? tmp->getNumFaehnchen() : 0;
}

const GRFlag * GRChord::getGRFlag() const
{
	GuidoPos pos = First();
	while (pos)
	{
		GRFlag * tmp = dynamic_cast<GRFlag *>(GetNext(pos));
		if (tmp)
			return tmp;
	}
	return 0;
}

GRFlag * GRChord::getGRFlag()
{
	GuidoPos pos = First();
	while (pos)
	{
		GRFlag * tmp = dynamic_cast<GRFlag *>(GetNext(pos));
		if (tmp)
			return tmp;
	}
	return 0;
}

void GRChord::setFlagOnOff(bool p)
{
	GRFlag * flag = getGRFlag();
	if (flag)
	{
		flag->setFlagOnOff(p);
		updateBoundingBox();
	}
}

NVPoint GRChord::getStemStartPos() const
{
	const GRStem * stem = getStem();
	NVPoint pnt( getPosition());
	if (stem)
	{
		pnt = stem->mPosition;
		if (mStemDir == dirUP)
		{
			pnt.x += (30 * mSize + mOffset.x - (4 * mSize));
			pnt.y -= (mStemLen - mOffset.y);
		}
		else if (mStemDir == dirDOWN)
		{		
			pnt.x -= (30 * mSize) - mOffset.x;
			pnt.y += (mStemLen + mOffset.y);
		}
	}
	return pnt;
}

NVPoint GRChord::getStemEndPos() const
{
	const GRStem * stem = getStem();
	NVPoint pnt( getPosition());
	if (stem)
	{
		pnt = stem->mPosition;
		if (mStemDir == dirUP)
		{
			pnt.x += ((30 * mSize) + mOffset.x - (1 * mSize)); // Hardcoded
			pnt.y -= (mStemLen - mOffset.y);
		}
		else if (mStemDir == dirDOWN)
		{		
			pnt.x -= (30 * mSize - mOffset.x - (4 * mSize));
			pnt.y += (mStemLen + mOffset.y);
		}
	}
	return pnt;
}

const GRStem * GRChord::getStem() const
{
	GuidoPos pos = First();
	GRNotationElement * e;
	while (pos)
	{
		e = GetNext(pos);
		
		if (typeid(*e) == typeid(GRStem)) // we should consider using a dynamic cast only.
		{
			// was return dynamic cast<GRStem *>(e);
			return static_cast<GRStem *>(e);
		}
	}
	return 0;
}

GRStem * GRChord::getStem()
{
	GuidoPos pos = First();
	GRNotationElement * e;
	while (pos)
	{
		e = GetNext(pos);
		
		if (typeid(*e) == typeid(GRStem)) // we should consider using a dynamic cast only.
		{
			// was return dynamic cast<GRStem *>(e);
			return static_cast<GRStem *>(e);
		}
	} 
	return 0;
}


void GRChord::setStemDirection(GDirection pdir)
{
	GRStem * stem = getStem();		
	if (stem)
	{   
		// then we have to set the stem-direction ...
		// this is somewhat tricky with chords ....
		stem->setStemDir(pdir);
		mStemDir = stem->mStemDir;
		if (mStemDir == dirUP)
		{
			stem->mPosition.y = mPosYBottom;
			mPosition.y = mPosYBottom;
		}
		else if (mStemDir == dirDOWN)
		{
			stem->mPosition.y = mPosYTop;
			mPosition.y = mPosYTop;
		}
		
		updateBoundingBox();
	}
}

void GRChord::setDotFormat( const ARDotFormat * inFormat )
{
	GuidoPos pos = First();
	while (pos)
	{
		GRNoteDot * dot = dynamic_cast<GRNoteDot *>( GetNext(pos));
		GREvent::setDotFormat( dot, inFormat );
	}
}
