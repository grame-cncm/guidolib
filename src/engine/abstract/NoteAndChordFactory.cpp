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

#include <string.h>
#include <iostream>
#include <algorithm>

#include "NoteAndChordFactory.h"
#include "ARMusicalVoice.h"
#include "ARNote.h"

using namespace std;

#define ARFTrace	0
#define ARTRACE		0

// ----------------------------------------------------------------------------
NoteAndChordFactory::NoteAndChordFactory()
  :
	mCurrentVoice(NULL),
	mCurrentEvent(NULL),
	mCurrentRegister(DEFAULT_REGISTER),
	mCurrentNumerator(DEFAULT_NUMERATOR),
	mCurrentDenominator(DEFAULT_DENOMINATOR),
	mCurrentIntensity(DEFAULT_INTENSITY),
    mDone(false),
    mInChord(false)
{}

// ----------------------------------------------------------------------------
NoteAndChordFactory::~NoteAndChordFactory()
{
}

// ----------------------------------------------------------------------------
/** \brief Sets the numerator of the current event and 
	 futur events duration fractions.
*/
void NoteAndChordFactory::setNumerator(int newNumerator)
{
    if(mCurrentEvent)
        mCurrentEvent->setNumerator(newNumerator);
	mCurrentNumerator = newNumerator;
}


// ----------------------------------------------------------------------------
/** \brief Sets the denominator of the current event and 
	 futur events duration fractions.
*/
void NoteAndChordFactory::setDenominator(int newDenominator)
{
    if(mCurrentEvent)
        mCurrentEvent->setDenominator(newDenominator);
	mCurrentDenominator = newDenominator;
}

// ----------------------------------------------------------------------------
/** \brief Tells the current voice that you want to begin a new chord.

	When you're finished adding notes to this chord, you must call addChord().
*/
void NoteAndChordFactory::createChord()
{
	assert(mCurrentVoice);
    
    if(!mDone && !mInChord)
    {
        mCurrentVoice->BeginChord();
        mInChord = true;
    }
}
// ----------------------------------------------------------------------------
/** \brief Notifies the current voice that you have finished adding notes
	to the current chord. 
	
	To begin a chord, call createChord()
*/
void NoteAndChordFactory::addChord()
{
    if (!mCurrentVoice || mDone)
        return;

    mCurrentVoice->FinishChord();
    mDone = true;
}

// ----------------------------------------------------------------------------
/** \brief Begins a new note commata.

	Called to tell the factory that a new chord-voice
	 is beginning. This is important for the ranges that need to
	  be added (dispdur and shareStem)
*/
void NoteAndChordFactory::initChordNote()
{
	assert(mCurrentVoice);
    if(!mDone)
        mCurrentVoice->initChordNote();
}

void NoteAndChordFactory::setRegister(TYPE_REGISTER newRegister)
{
    mCurrentRegister = newRegister;
    ARNote * note = mCurrentEvent ? dynamic_cast<ARNote*>(mCurrentEvent) : 0;
    if(note)
        note->setOctave(newRegister);
}

// ----------------------------------------------------------------------------
/** \brief Sets the accidentals of the current event.
*/
void NoteAndChordFactory::setAccidentals(TYPE_ACCIDENTALS accidentals)
{
	// this is really ugly and prone to crash bugs
	// depending on the context, mCurrentEvent could belong to ARFactory or to NoteAndChordFactory
	// while it may be initalised in NoteAndChordFactory context,
	// it could be used in ARFactory context (and thus with a null value) by methods that have not been overriden
//	ARMusicalEvent * ev = mCurrentEvent;
	ARNote * note = static_cast<ARNote*>(mCurrentEvent->isARNote());
	assert(note);
	note->setAccidentals(accidentals);
}

// ----------------------------------------------------------------------------
void NoteAndChordFactory::createEvent( const char * name )
{	
    if(mDone)
        return;
    ARNote * note = new ARNote(name, 0, mCurrentRegister, mCurrentNumerator,mCurrentDenominator,mCurrentIntensity);
    note->setOctave(mCurrentRegister);
    note->setNumerator(mCurrentNumerator);
    note->setDenominator(mCurrentDenominator);
    mCurrentEvent = note;
	assert(mCurrentEvent);
}

// ----------------------------------------------------------------------------
/** \brief Adds the current event to the current voice.

	The "last" event becomes the "current" event, and 
	the "current" event is set to NULL.

*/
void NoteAndChordFactory::addEvent()
{
    if (!mCurrentVoice || !mCurrentEvent || mDone)
        return;
    
    mCurrentVoice->AddTail( mCurrentEvent );
    
    if(!mInChord)
        mDone = true;
}

