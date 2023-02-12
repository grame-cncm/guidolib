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

#include "ARNote.h"
#include "GRAccidental.h"
#include "GRNote.h"
#include "GRSingleNote.h"
#include "GRStaff.h"
#include "GRTrill.h"
#include "GRCluster.h"

using namespace std;

GRNote::GRNote( GRStaff * grstaf, const ARNote * note, const TYPE_TIMEPOSITION & date, const TYPE_DURATION & duration)
  : GREvent (grstaf,note,date,duration),
    fOrnament(0), fCluster(NULL), fOwnCluster(false), fClusterNote(false), fClusterHaveToBeDrawn(false)
{
    // builds a graphical "part" of abstractRepresentation
    assert(note);
    if (note->getOrnament())
        fOrnament = new GRTrill( mGrStaff, note->getOrnament());

    if (note->getARCluster())
    {
        fClusterNote = true;
        fClusterHaveToBeDrawn = note->doesClusterHaveToBeDrawn();
    }
    fIsGraceNote = false;
}

GRNote::GRNote(GRStaff * grstaf, const TYPE_DURATION & inDuration )
  : GREvent(grstaf,new ARNote( inDuration ),1), fIsGraceNote(false)
{
	// dor "Dummys": not with duration theDuration, matching ARNote will be created
	// ARNote will be not deleted automatically!!
	fCluster = 0; 
	fOwnCluster = fClusterNote = fClusterHaveToBeDrawn = false;
}

GRNote::GRNote(GRStaff * grstaf, const ARNote * note)
	: GREvent(grstaf,note), fIsGraceNote(false)
{
	assert (note);
	fCluster = 0;
	fOwnCluster = fClusterNote = fClusterHaveToBeDrawn = false;
}

GRNote::~GRNote()
{
	delete fOrnament;
	if (fOwnCluster) delete fCluster;
}

const ARNote * GRNote::getARNote() const 	{ return static_cast<const ARNote*>(getAbstractRepresentation()); }
bool GRNote::isSplit()						{ return (getARNote()->getRelativeTimePosition() != getRelativeTimePosition()); }
void GRNote::setNoteFormat(const ARNoteFormat * frmt)	{}

void GRNote::hideAccidentals()
{
	NEPointerList& sub = GetCompositeElements();
	GuidoPos pos = sub.GetHeadPosition();
	while (pos) {
		GRNotationElement * el = sub.GetNext(pos);
		GRAccidental* acc = dynamic_cast<GRAccidental*>(el);
		if (acc && ! acc->isCautionary()) acc->setDrawOnOff (false);
	}
}

/*
	outAccidental will be negative if note has flats and positive if note has sharps
*/
void GRNote::getPitchAndOctave( int * outPitch, int * outOctave, int * outAccidentals ) const
{
	const ARNote * ar = getARNote();

	*outPitch = ar->getPitch();
	*outOctave = ar->getOctave() - ar->getOctava();
	*outAccidentals = ar->getAccidentals();
}

// -----------------------------------------------------------------------------
GDirection GRNote::getDefaultThroatDirection() const
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
GDirection GRNote::getThroatDirection() const
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

GRCluster *GRNote::createCluster(const ARNoteFormat *inCurnoteformat)
{
    GRSingleNote *singleNote = dynamic_cast<GRSingleNote *>(this);

    const ARNote *arNote = getARNote();

    fCluster = new GRCluster(mGrStaff, arNote->getARCluster(), singleNote, inCurnoteformat);
	fOwnCluster = true;
    return fCluster;
}

void GRNote::setGRCluster(GRCluster *inCluster, bool inSignificativeNote)
{
    fCluster = inCluster;

    if (inSignificativeNote)
        inCluster->setSecondGRNote(this);
}
