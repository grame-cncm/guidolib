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

#include "ARNote.h"
#include "GRNote.h"
#include "GRSingleNote.h"
#include "GRStaff.h"
#include "GRTrill.h"
#include "GRCluster.h"
#include "GRGlobalStem.h"


GRNote::GRNote( GRStaff * grstaf,
	ARNote * abstractRepresentationOfNote,
	const TYPE_TIMEPOSITION & relativeTimePositionOfGRNote,
	const TYPE_DURATION & durationOfGRNote)
  : GREvent(grstaf,abstractRepresentationOfNote,relativeTimePositionOfGRNote,durationOfGRNote), mClusterNote(false),
  mARNote(abstractRepresentationOfNote), mCluster(NULL)
{
    // builds a graphical "part" of abstractRepresentation
    assert(mARNote);
    if (mARNote->getOrnament())
        mOrnament = new GRTrill( mGrStaff, mARNote->getOrnament());
    else
        mOrnament = NULL;

    if (mARNote->getARCluster())
    {
        mClusterNote = true;
        mIsLonelyInCluster = mARNote->isLonelyInCluster();
    }
}

GRNote::GRNote(GRStaff * grstaf, const TYPE_DURATION & inDuration )
  : GREvent(grstaf,new ARNote( inDuration ),1)
{
	// dor "Dummys": not with duration theDuration, matching ARNote will be created
	// ARNote will be not deleted automatically!!
}

GRNote::GRNote(GRStaff * grstaf, ARNote * abstractRepresentationOfNote)
  							: GREvent(grstaf,abstractRepresentationOfNote)
{
	assert(abstractRepresentationOfNote);
}

GRNote::~GRNote()
{
	delete mOrnament;
}


/*void GRNote::setHPosition( float nx )
{
	// - Call inherited
	GREvent::setHPosition(nx);

	// - Notify ornament
	if (mOrnament)
	{
		mOrnament->tellPosition(this, getPosition());
	}

	updateBoundingBox();
}*/

ARNote * GRNote::getARNote()
{
	return /*dynamic*/static_cast<ARNote*>(getAbstractRepresentation());
}

const ARNote * GRNote::getARNote() const
{
	return /*dynamic*/static_cast<const ARNote*>(getAbstractRepresentation());
}

void GRNote::print() const
{
}

bool GRNote::isSplit()
{
	return (getARNote()->getRelativeTimePosition() != mRelativeTimePositionOfGR);
}

void GRNote::setNoteFormat(const ARNoteFormat * frmt)
{

}

/*
	outAccidental will be negative if note has flats and positive if note has sharps
*/
void GRNote::getPitchAndOctave( int * outPitch,
								int * outOctave,
								int * outAccidentals ) const
{
	const ARNote * ar = getARNote();

	*outPitch = ar->getPitch();
	*outOctave = ar->getOctave();
	*outAccidentals = ar->getAccidentals();
}

// -----------------------------------------------------------------------------
GDirection
GRNote::getDefaultThroatDirection() const
{
	if( mGrStaff == 0 ) return dirOFF;

	int pitch;
	int octave;
	int acc;
	getPitchAndOctave( &pitch, &octave, &acc );

	return mGrStaff->getDefaultThroatDirection( pitch, octave );
}

// -----------------------------------------------------------------------------
/** \brief Gives the actual or possible stem direction of a note.

	Return a correct result event if the note don't have stems (whole notes).

	on return: -1: downward, 1 : upward.
*/
GDirection
GRNote::getThroatDirection() const
{
	GDirection dir = getStemDirection();
	if(( dir != dirUP ) && ( dir != dirDOWN ))
		dir = getDefaultThroatDirection();

	return dir;
}


/*
inline void GRNote::setDotFormat(const ARDotFormat * frmt)
{

}
*/

GRCluster *GRNote::createCluster(ARNoteFormat *inCurnoteformat)
{
    GRSingleNote *singleNote = dynamic_cast<GRSingleNote *>(this);
    
    mCluster = new GRCluster(mGrStaff, mARNote->getARCluster(), singleNote->getDurTemplate(), inCurnoteformat);

    return mCluster;
}

void GRNote::setGRCluster(GRCluster *inCluster)
{
    mCluster = inCluster;

    mCluster->setSecondNoteYPosition();
}