/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
	Copyright (C) 2003, 2004 Grame

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

#include <cmath>
#include <typeinfo>
#include <iostream>
using namespace std;

#include "ARMusicalEvent.h"
#include "ARDotFormat.h"
#include "TagParameterFloat.h"

#include "GREvent.h"
#include "GRStaff.h"
#include "GRArticulation.h"
#include "GRTrill.h"
#include "GRGlobalStem.h"
#include "GRGlobalLocation.h"
#include "GRNoteDot.h"

#include "GuidoDefs.h" // for LSPACE

// ----------------------------------------------------------------------------
GREvent::GREvent(GRStaff * inStaff,
				 ARMusicalEvent * abstractRepresentationOfEvent, bool p_ownsAR)
  : GRARCompositeNotationElement(abstractRepresentationOfEvent,p_ownsAR),
  mGlobalStem(NULL), mArtilist(0)
{
	mFillsBar = false;
	mBeamCount = 0;
	mArticulationFlags = 0;
	mGrStaff = inStaff;
	mColRef = 0;
	if (mGrStaff)
	{
		mSize = mGrStaff->getSizeRatio();
		mCurLSPACE = mGrStaff->getStaffLSPACE();
	}
	else
	{
		mSize = 1.0f;
		mCurLSPACE = LSPACE;
	}
}

// ----------------------------------------------------------------------------
GREvent::GREvent( GRStaff * inStaff,
				 ARMusicalEvent * abstractRepresentationOfEvent,
	const TYPE_TIMEPOSITION & theRelativeTimePositionOfGR,
	const TYPE_DURATION & theDurationOfGR)
  : GRARCompositeNotationElement(abstractRepresentationOfEvent),
  mGlobalStem(NULL), mArtilist(0)
{
	assert(abstractRepresentationOfEvent);
	assert(theRelativeTimePositionOfGR>=abstractRepresentationOfEvent->getRelativeTimePosition());

	mColRef = 0;
	mFillsBar = false;
	mBeamCount = 0;
	mArticulationFlags = 0;
	mGrStaff = inStaff;

	if (mGrStaff)
	{
		mCurLSPACE = mGrStaff->getStaffLSPACE();
		mSize = mGrStaff->getSizeRatio();
	}
	else
	{
		mCurLSPACE = LSPACE;
		mSize = float(1.0);
	}

	mRelativeTimePositionOfGR = theRelativeTimePositionOfGR;
	mDurationOfGR = theDurationOfGR;

	mNeedsSpring = 1;

// !!! kf ... is important for laenge0.svw (wcht)
// durations will be treated as fractions in future
//	assert(theDurationOfGR>0);

	// ATTENTION, this can be bad for Chords ...
	// Graphical-Elements are longer than the
	// abstract-representations!
	
	// DF - 24/08/2009 - assert commented to avoid spurious exit
	// due to dynamic score coding or multiple fermata in a chord with variable length notes
	// assert(getRelativeEndTimePosition()<=abstractRepresentationOfEvent->getRelativeEndTimePosition());
}

GREvent::~GREvent()
{
	DeleteContent( mArtilist );
	delete mArtilist; mArtilist = 0;
	delete [] mColRef;
}

/* 
int GREvent::getSpacing() const
{
  //## begin GREvent::getStandardSpacing%856979595.body preserve=yes
	TYPE_DURATION d=getDuration();
// An dieser Stelle wird das Spacing bestimmt!
// Diese Funktion ist von Matthias Huber, ich
// (kf) habe nur das LSPACE*4 eingefuegt (anstelle von
// 80)
	int s=(int)(log(1+d)*8*LSPACE);
	//int s=(int)(sqrt(d)*100);
// ACHTUNG kf ....
	if (s<=0)
		return 1;
//	assert(s>0);
	return s;
  //## end GREvent::getStandardSpacing%856979595.body
} */

void GREvent::setBeamStem(GRBeam *, GCoord )
{ // ignore/ do nothing for rests and composite note 
}

float GREvent::setStemLength( float inLen )
{
	return 0;
}

NVPoint GREvent::getCrescStart() // may be obsolete
{
	return getTieEnd();
}

NVPoint GREvent::getCrescEnd()	// may be obsolete
{
	return getTieStart();
}

// ----------------------------------------------------------------------------
/** \brief Looks for a dot in the notation elements list.
*/
GRNoteDot * GREvent::getDot()
{
	GuidoPos pos = First();
	GRNotationElement * e;
	while (pos)
	{
		e = GetNext(pos);
		if (typeid(*e) == typeid(GRNoteDot))
		{
			return static_cast<GRNoteDot *>(e); // was dynamic cast<GRNoteDot *>(e); 
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
/** \brief Setup the dot notation elements of the event, if any.
*/
void GREvent::setDotFormat(const ARDotFormat * inFormat)
{
	setDotFormat( getDot(), inFormat );
}

// ----------------------------------------------------------------------------
/** \brief Setup a given dot notation element.
*/
void GREvent::setDotFormat( GRNoteDot * inDot, const ARDotFormat * inFormat )
{
	if( inDot == 0 || inFormat == 0 ) return;

	if (inFormat->getDX())
	{
		inDot->mOffset.x += (float)(inFormat->getDX()->getValue(mCurLSPACE));
	}

	if (inFormat->getDY() && inFormat->getDY()->TagIsSet())
	{
		inDot->mOffset.y -= (float)(inFormat->getDY()->getValue(mCurLSPACE));
	}
	else if (inFormat->getDY())
	{
		if( positionIsOnStaffLine( mPosition.y, mCurLSPACE ))
			inDot->mOffset.y -= (GCoord)TagParameterFloat::convertValue(0.8f,"hs",mCurLSPACE);
	}

	if (inFormat->getSize())
	{
		inDot->size *= inFormat->getSize()->getValue();
	}
	// what about dd ?
}

// ----------------------------------------------------------------------------
/** \brief Creates the duration dot and sets its position.
*/
void GREvent::createDots( const TYPE_DURATION & inDuration, float inNoteBreite,
							const NVPoint & inPos )
{
	const int dotCount = GRNoteDot::DurationToDotCount( inDuration );
	if( dotCount > 0 )
	{
		GRNoteDot * noteDot = new GRNoteDot ( this, inNoteBreite, dotCount );
		noteDot->setPosition( inPos );
		AddTail( noteDot );
		updateBoundingBox();
	}
}

// ----------------------------------------------------------------------------
/** \brief not implemented here.
*/
int GREvent::adjustLength( const TYPE_DURATION & ndur )
{
	assert(false);
	return -1;
}

// ----------------------------------------------------------------------------
void 
GREvent::setPosition( const NVPoint & inPos )
{
	// - Call inherited
	GRARCompositeNotationElement::setPosition( inPos );
	
	// - Notify articulation using 'tell'
	if (mArtilist)
	{
		GRNEList::iterator ptr;
		for( ptr = mArtilist->begin(); ptr != mArtilist->end(); ++ptr )
		{
			GRNotationElement * el = *ptr;
			el->tellPosition( this, getPosition());
		}
	}

	// - 
	updateBoundingBox();
}

// ----------------------------------------------------------------------------
void GREvent::setHPosition( float nx )
{
	// - Call inherited
	GRARCompositeNotationElement::setHPosition(nx);
	
	// - Notify articulation using 'tell'
	if (mArtilist)
	{
		GRNEList::iterator ptr;
		for( ptr = mArtilist->begin(); ptr != mArtilist->end(); ++ptr )
		{
			GRNotationElement * el = *ptr;
			el->tellPosition( this, getPosition());
		}
	}

	updateBoundingBox();
}

// ----------------------------------------------------------------------------
/** \brief Adds articulation symbols elements to the event.
	
	Called by the class GRRange 
	GREvent maintains an articulation list that holds all articulations.
*/
void 
GREvent::addArticulation( ARMusicalTag * inTag )
{
	if( mArtilist == 0 )
		mArtilist = new GRNEList; // owns elements.

	const float space = mCurLSPACE; // (JB) was LSPACE
	GRArticulation * newArticulation = new GRArticulation( inTag, space );
	mArtilist->push_back( newArticulation );
	
	// At this point, the articulation is not yet positionned, 
	// it is just attached to the event.

	// DEBUG, try to place the articulation correctly. (OK, this fixes bugs)
	newArticulation->tellPosition( this, getPosition());

	mArticulationFlags |= newArticulation->getArticulationType(); // Avoids the use of 2 identical articulations 

	updateBoundingBox(); // ok ?
}


// ----------------------------------------------------------------------------
void GREvent::updateBoundingBox()
{
	GRARCompositeNotationElement::updateBoundingBox();
	return;

	// - (JB) new: add articulations
	const GRNEList * articulations = getArticulations();
	if( articulations )
	{
		for( GRNEList::const_iterator ptr = articulations->begin(); ptr != articulations->end(); ++ptr )
		{
			GRArticulation * el = static_cast<GRArticulation *>(*ptr);
//			if( el->getArticulationType() != GRArticulation::kFlagBreathMark )
//			{
				addToBoundingBox( el ); // ok
				// mBoundingBox.Merge( el->getBoundingBox());
//			}
		}
	}
}

// ----------------------------------------------------------------------------
int GREvent::getNumFaehnchen() const	// number of tags ?
{
	return mGlobalStem ? mGlobalStem->getNumFaehnchen() : 0;
}

/** \brief The event gets a global stem.

	The event needs a spring if it is either the firstEl of the
	globalStem or if the duration is not zero (then it is an empty-event
	in another voice)
*/
void GREvent::setGlobalStem( GRGlobalStem * inGlobalStem )
{
	mGlobalStem = inGlobalStem;
	if( inGlobalStem && inGlobalStem->getFirstEl() != 0
					&& inGlobalStem->getFirstEl() != this )
		mNeedsSpring = -1;
}

// ----------------------------------------------------------------------------
void GREvent::setGlobalLocation(GRGlobalLocation * pgloc)
{
	if (pgloc && pgloc->getFirstEl() != NULL
		&& pgloc->getFirstEl() != this)
		mNeedsSpring = -1;
}

// ----------------------------------------------------------------------------
void GREvent::addToOffset(const NVPoint & offs)
{
	mOffset += offs;
	GRARCompositeNotationElement::addToOffset(offs);
}

// ----------------------------------------------------------------------------
void GREvent::setFlagOnOff(bool i)
{
	if (mGlobalStem)
		mGlobalStem->setFlagOnOff(i);
}

GDirection GREvent::getStemDirection() const
{ 
	return mGlobalStem ? mGlobalStem->getStemDir() : dirOFF;
}

// ----------------------------------------------------------------------------
void GREvent::setStemDirection(GDirection dir)
{
	if (mGlobalStem)
		mGlobalStem->setStemDirection(dir);
}

// ----------------------------------------------------------------------------
NVPoint GREvent::getStemStartPos() const
{ 
	return mGlobalStem ? mGlobalStem->getStemStartPos() : NVPoint(0,0);
}

NVPoint GREvent::getStemEndPos() const
{
	if (mGlobalStem)
		return mGlobalStem->getStemEndPos();
	return NVPoint(mBoundingBox.right,0); 
}

float GREvent::changeStemLength( float inLen )
{ 
	return mGlobalStem ? mGlobalStem->changeStemLength( inLen ) : 0;
}

bool GREvent::getStemDirSet() const
{ 
	return mGlobalStem ? mGlobalStem->getStemDirSet() : false;
}

bool GREvent::getStemLengthSet() const
{ 
	return mGlobalStem ? mGlobalStem->getStemLengthSet() : false; 
}

float GREvent::getStemLength() const
{
	return mGlobalStem ? mGlobalStem->getStemLength() : 0;
}

bool	
GREvent::isSyncopated() const
{ 
	Fraction duration ( mDurationOfGR );
	duration.normalize();
	return  ( mRelativeTimePositionOfGR.getDenominator() < 
										duration.getDenominator());
}

bool GREvent::hasArticulation( int inArticulationFlag ) const
{
	return (( mArticulationFlags & inArticulationFlag ) != 0 );
}

