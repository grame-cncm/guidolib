/*
    GUIDO Library
    Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
    Copyright (C) 2002-2013 Grame

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
    research@grame.fr
*/

#include <string.h>
#include <iostream>
#include <algorithm>

#include "defines.h" 
#include "ARFactory.h"

#include "ARNote.h"
#include "ARRest.h"
#include "ARTDummy.h"
#include "ARBar.h"
#include "ARDoubleBar.h"
#include "ARFinishBar.h"
#include "ARText.h"
#include "ARLyrics.h"
#include "ARRangeEnd.h"
#include "ARFermata.h"
#include "ARHarmonic.h"
#include "ARTStem.h"
#include "ARKey.h"
#include "ARStaff.h"
#include "ARBreathMark.h"
#include "ARVolta.h"

#include "ARCoda.h"
#include "ARDaCapo.h"
#include "ARDalSegno.h"
#include "ARDaCoda.h"
#include "ARFine.h"
#include "ARSegno.h"

#include "ARIntens.h"
#include "ARBeam.h"
#include "ARBeamState.h"
#include "ARSlur.h"
#include "ARTie.h"
#include "ARColor.h" 
#include "ARCrescendo.h"
#include "ARDiminuendo.h"
#include "ARSpace.h"
#include "ARAlter.h"
#include "ARNewSystem.h"
#include "ARDrHoos.h"
#include "ARDrRenz.h"
#include "ARBembel.h"
#include "ARDummyRangeEnd.h"
#include "ARNewPage.h"
#include "ARPageFormat.h"
#include "ARRepeatBegin.h"
#include "ARRepeatEnd.h"
#include "ARTempo.h"
#include "ARSpecial.h"
#include "ARRepeatEndRangeEnd.h"
#include "ARStaffOff.h"
#include "ARStaffOn.h"
#include "ARMerge.h"
#include "ARTuplet.h"
#include "ARMusicalVoice.h"
#include "ARMusic.h"
#include "ARClef.h"
#include "ARMeter.h"
#include "ARSystemFormat.h"
#include "ARStaffFormat.h"
#include "ARBarFormat.h"
#include "ARUnits.h"
#include "ARAccol.h"
#include "ARAcc.h"
#include "ARDotFormat.h"
#include "ARNoteFormat.h"
#include "ARRestFormat.h"
#include "ARComposer.h"
#include "ARTitle.h"
#include "ARStaccato.h"
#include "ARPizz.h"
#include "ARMarcato.h"
#include "ARAccent.h"
#include "ARTenuto.h"
#include "ARMark.h"
#include "ARLabel.h"
#include "ARFingering.h"
#include "AROctava.h"
#include "ARGrace.h"
#include "ARTremolo.h"
#include "ARTrill.h"
#include "ARInstrument.h"
#include "ARRitardando.h"
#include "ARAccelerando.h"
#include "ARCue.h"
#include "ARAuto.h"
#include "ARDefineTag.h"
#include "ARDisplayDuration.h"
#include "ARShareStem.h"
#include "ARShareLocation.h"
#include "ARTHead.h"
#include "ARChordTag.h"
#include "ARUserChordTag.h"
#include "ARCluster.h"
#include "ARGlissando.h"
#include "ARSymbol.h"
#include "ARFeatheredBeam.h"

#include "TagParameterString.h"
#include "TagParameterInt.h"
#include "TagParameterFloat.h"

#include "benchtools.h"

long ARFactory::sMaxTagId = -1;

using namespace std;

#define ARFTrace	0
#define ARTRACE		0

// ----------------------------------------------------------------------------
ARFactory::ARFactory()
  :
	mCurrentVoice(NULL),
	mCurrentMusic(NULL),
	mCurrentEvent(NULL),
	mCurrentRepeatBegin(NULL),
	mLastEvent(NULL),
	mTagParameterList(1),
	mCurrentRegister(DEFAULT_REGISTER),
	mCurrentNumerator(DEFAULT_NUMERATOR),
	mCurrentDenominator(DEFAULT_DENOMINATOR),
	mCurrentIntensity(DEFAULT_INTENSITY),
	mBeamState(BEAMSAUTO),
	mCurrentOctava(NULL),
	mCurrentGrace(NULL),
	mCurrentCue(NULL),
	mCurrentTrill(NULL),
	mCurrentStem(NULL),
	mCurrentHead(NULL),
	mCurrentNoteFormat(NULL),
	mCurrentRestFormat(NULL),
	mCurrentDotFormat(NULL),
	mCurrentAlter(NULL),
	mSaveCurrentVoice(NULL),
	mCurrentStaff(NULL),
	mCurrentCluster(NULL),
    mCurrentTremolo(NULL),
	mVoiceNum(1),
	mCurrentTags(0),
	mVoiceAdded(false),
    mFilePath()
{
		sMaxTagId = -1;
}

// ----------------------------------------------------------------------------
ARFactory::~ARFactory()
{
	while (mCurrentTags) {
//		addTag();
		endTag();
	}
//	if (mCurrentVoice && !mVoiceAdded)	addVoice();
	delete mSaveCurrentVoice;	
	delete mCurrentEvent;
	delete mCurrentVoice;
	delete mCurrentMusic;
}

// ----------------------------------------------------------------------------
/** \brief Creates and open a brand new music segment object. It becomes
  	the current music segment.
  	
  	You cannot create a new music segment if the current music segment
	already exists.
*/
void ARFactory::createMusic()
{
#if ARFTrace
 	cout << "ARFactory::createMusic " << endl;
#endif
	assert(!mCurrentMusic);
	mCurrentMusic = new ARMusic;
}

// ----------------------------------------------------------------------------
/** \brief Perform automatic adjustements on the current music, and returns it.

	You must not have pending events nor pending voice at this point.
	The current music is then set to NULL
*/
ARMusic * ARFactory::getMusic()
{
	assert(!mCurrentEvent);
	assert(!mCurrentVoice);

    if (!mCurrentMusic)
        return NULL;

    ARMusic * music = mCurrentMusic;

    // this sets the maxtagid in the ARMusic.
    // this can be used by all auto-routines...
    music->mMaxTagId = ARFactory::sMaxTagId + 1;

#if ARTRACE
    cout << "ARFactory::getMusic before auto stuff: " << endl;
    music->output(cout, true);
#endif

    // this call inserts potential Breaks in
    // the first voice of the piece.
    timebench("ARMusic::doAutoStuff", music->doAutoStuff());

#if ARTRACE
    cout << "\nARFactory::getMusic after auto stuff: " << endl;
    music->output(cout, true);
    cout << "\nARFactory::getMusic - end ----------- " << endl;
#endif


    mCurrentMusic = 0;
    ARFactory::sMaxTagId = -1;
    return music;
}


// ----------------------------------------------------------------------------
/** \brief Creates a new musical voice. It becomes the current voice.

	The first created voice has a voice number equal to 1, next voice's
	numbers are then incremented. 

	You cannot create a new music segment if the current music segment
	already exists.
*/
void ARFactory::createVoice()
{
#if ARFTrace
 	cout << "ARFactory::createVoice " << endl;
#endif
	assert(!mCurrentVoice);
	mCurrentVoice= new ARMusicalVoice;
	mCurrentVoice->setVoiceNum(mVoiceNum++);
	mVoiceAdded = false;
}

// ----------------------------------------------------------------------------
/** \brief Adds the current voice to the current music segment.

	The voice is first converted to its normal form.
	You must not have pending event nor pending voice at this point.
	The current voice and each other voice state variables are then set to NULL.
*/
void ARFactory::addVoice()
{
#if ARFTrace
 	cout << "ARFactory::addVoice " << endl;
#endif
	assert(!mCurrentEvent);

    if (!mCurrentVoice || !mCurrentMusic)
        return;

    mCurrentVoice->ConvertToNormalForm();

    // old: this is now done
    // when returning the complete voice ...

    // this does automatic conversion,
    // for example autobeaming ...
    // mCurrentVoice->doAutoStuff();

    mCurrentMusic->AddTail(mCurrentVoice);
    mCurrentVoice=NULL;
    mCurrentDenominator = DEFAULT_DENOMINATOR;
    mCurrentIntensity = DEFAULT_INTENSITY;
    mCurrentNumerator = DEFAULT_NUMERATOR;
    mCurrentRegister = DEFAULT_REGISTER;
    mCurrentStaff = NULL;
    mCurrentOctava = NULL;
    mCurrentStem = NULL;
    mCurrentHead = NULL;
    mCurrentNoteFormat = NULL;
    mCurrentAlter = NULL;
    mCurrentRestFormat = NULL;
    mCurrentDotFormat = NULL;
    mCurrentCue = NULL;
    mCurrentGrace = NULL;
    mCurrentTrill = NULL;
    mCurrentCluster = NULL;
    mCurrentTremolo = NULL;
    mVoiceAdded = true;
}

// ----------------------------------------------------------------------------
/** \brief Tells the current voice that you want to begin a new chord.

	When you're finished adding notes to this chord, you must call addChord().
*/
void ARFactory::createChord()
{
#if ARFTrace
 	cout << "ARFactory::createChord " << endl;
#endif
	// now, we have to save the position of the voice...
	assert(mCurrentVoice);

    mCurrentVoice->BeginChord();
}

// ----------------------------------------------------------------------------
/** \brief Notifies the current voice that you have finished adding notes
	to the current chord. 
	
	To begin a chord, call createChord()
*/
void ARFactory::addChord()
{
#if ARFTrace
 	cout << "ARFactory::addChord " << endl;
#endif
    if (!mCurrentVoice)
        return;

    // then we are finished reading the chord.
    // we have to deal with trills somewhere else...

    // now we have to go through the events within this chord once more and set the duration of the 
    // first empty event and reset the timepositions of the following events (and tags/ptags)
    // we also have to add the \chord \dispDur and \shareStem tags...
    // one thought: these Tags need one additional parameter (not only auto) but also for saving if the chord
    // was automatically created or already in the GUIDO description.	
    if (mCurrentTrill)
    {
        ARMusicalVoice::CHORD_TYPE chord_type = ARMusicalVoice::UP_SIMPLE;
        ARMusicalVoice::CHORD_ACCIDENTAL chord_accidental = ARMusicalVoice::NATURAL;
        ARNote * FirstNote = mCurrentVoice->setTrillChord(chord_type, chord_accidental);

        mCurrentTrill->setChordType(chord_type);
        mCurrentTrill->setChordAccidental(chord_accidental);

        FirstNote->setOrnament(mCurrentTrill);
        //		delete mCurrentTrill;
        //		mCurrentTrill = 0;
    
        mCurrentVoice->FinishChord(true);
    }
    else
    {
        if (mCurrentCluster)
            mCurrentVoice->setClusterChord(mCurrentCluster);

        mCurrentVoice->FinishChord(false);
    }
}

// ----------------------------------------------------------------------------
/** \brief Begins a new note commata.

	Called to tell the factory that a new chord-voice
	 is beginning. This is important for the ranges that need to
	  be added (dispdur and shareStem)
*/
void ARFactory::initChordNote()
{
#if ARFTrace
 	cout << "ARFactory::initChordNote " << endl;
#endif
	assert(mCurrentVoice);
	mCurrentVoice->initChordNote();
}


void ARFactory::createEvent( const char * name )
{
#if ARFTrace
 	cout << "ARFactory::createEvent " << endl;
#endif
	assert(!mCurrentEvent);
	assert(name);
	if(!strcmp(name,"_") || !strcmp(name,"rest"))
	{
		mCurrentEvent = new ARRest(mCurrentNumerator,mCurrentDenominator );
	}
	else
	{
		mCurrentEvent = new ARNote(name, 0, mCurrentRegister,
				mCurrentNumerator,mCurrentDenominator,mCurrentIntensity);
	}
	assert(mCurrentEvent);
	mLastEvent = NULL;
}

// ----------------------------------------------------------------------------
/** \brief Adds the current event to the current voice.

	The "last" event becomes the "current" event, and 
	the "current" event is set to NULL.

*/
void ARFactory::addEvent()
{
#if ARFTrace
 	cout << "ARFactory::addEvent " << endl;
#endif
    if (!mCurrentVoice || !mCurrentEvent)
        return;
    
    if (mCurrentGrace)
    {
        assert(mCurrentGrace->getRange());

        // maybe introduce a display-duration?
        // we have to introduce a display duration for sure ....
        // only set the display-duration if the event
        // within the grace-note has a duration greater then zero 

        if (mCurrentEvent->getDuration() > DURATION_0)
        {
            
            ARDisplayDuration * tmpdspdur = 0;
            tmpdspdur = new ARDisplayDuration;
            tmpdspdur->setDisplayDuration( mCurrentEvent->getDuration());

            mCurrentVoice->AddPositionTag(tmpdspdur);

            // the display duration tag will get its position 
            // through the voice::AddTail operation.
            // now we need an end ...
            mCurrentEvent->setDuration(DURATION_0);
            
            mCurrentVoice->AddTail( mCurrentEvent );
        
            if (tmpdspdur)
            {
                ARDummyRangeEnd * dummy = new ARDummyRangeEnd("\\dispDurEnd");
                mCurrentVoice->setPositionTagEndPos(-1,dummy,tmpdspdur);
            }
        }
    }
    else if(mCurrentTremolo)
    {
        assert(mCurrentTremolo->getRange());
        
        ARNote * n = dynamic_cast<ARNote*>(mCurrentEvent);
        if(n)
        {
            Fraction totalDuration = n->getDuration();
            Fraction duration = Fraction(mCurrentEvent->getDuration().getNumerator(), mCurrentEvent->getDuration().getDenominator()*2);
            
            std::vector<ARNote*> notes = mCurrentTremolo->createSecondNotes(duration, n->getOctave());
            
            if(notes.size()) // if we have at least one note in the vector, we'll have to devide the duration of our main note and adjust the graphical aspect with DisplayDuration
            {
                ARDisplayDuration * tmpdspdur1 = new ARDisplayDuration;
                tmpdspdur1->setDisplayDuration( totalDuration);
                mCurrentVoice->AddPositionTag(tmpdspdur1);
                
                mCurrentEvent->setDuration(duration);
                mCurrentVoice->AddTail( mCurrentEvent );
                
                ARDummyRangeEnd * dummy = new ARDummyRangeEnd("\\dispDurEnd");
                mCurrentVoice->setPositionTagEndPos(-1,dummy,tmpdspdur1);
            }
            else
                mCurrentVoice->AddTail( mCurrentEvent );
        }
        else
            mCurrentVoice->AddTail( mCurrentEvent );
    }
    else
        mCurrentVoice->AddTail( mCurrentEvent );
 

    // In the "toadd" list are all musical objects (Ties, Fermatas, ...) stored which
    // must be definitly stored AFTER the event.
    // otherwise an update of the graphical position is very hard todo
    // The position of the last note is needed before 
    // the calculation of the  position can be done
    // ATTENTION: This  is not true anymore! Ties and slurs are now
    // handled totally differently. Only for fermatas this solution
    // maybe needed now (I believe).

    /*	while (!mToAddList.empty())
    {
    // old
    // mCurrentVoice->AddTailNoDuration(mToAddList.RemoveHead());
    mCurrentVoice->AddTail(mToAddList.RemoveHead());
    }*/

    
    mLastEvent = mCurrentEvent;
    mCurrentEvent = 0;

}

// ----------------------------------------------------------------------------
/** \brief Adds a sharp to the current event. The current event must be a note.
*/
void ARFactory::addSharp()
{
	ARNote * note = dynamic_cast<ARNote *>(mCurrentEvent);
	assert( note );
	note->addSharp();
}

// ----------------------------------------------------------------------------
/** \brief Add a flat to the current event.
	The current event must be a note.
*/
void ARFactory::addFlat()
{
	ARNote * note = dynamic_cast<ARNote *>(mCurrentEvent);
	assert( note );
	note->addFlat();
}

// ----------------------------------------------------------------------------
/** \brief Sets dots of duration of the current event.
*/
void ARFactory::setPoints( int pointCount )
{
	assert(mCurrentEvent);
	mCurrentEvent->setPoints(pointCount);
}

// ----------------------------------------------------------------------------
/** \brief Sets the accidentals of the current event.
*/
void ARFactory::setAccidentals(TYPE_ACCIDENTALS accidentals)
{
	ARNote * note = dynamic_cast<ARNote *>(mCurrentEvent);
	assert(note);
	note->setAccidentals(accidentals);
}

// ----------------------------------------------------------------------------
/** \brief Sets the register (octave) of the current and future events. 
	The current  event must be a note.

	This new register becomes the current register.
*/
void ARFactory::setRegister(TYPE_REGISTER newRegister)
{
	/* was:	if (!mCurrentEvent || !dynamic cast<ARNote *>(mCurrentEvent))
				return;

		assert(dynamic cast<ARNote*>(mCurrentEvent)); */
	
	ARNote * note = dynamic_cast<ARNote *>(mCurrentEvent);
	if( note == 0 ) return;
	
	note->setRegister( newRegister );
	mCurrentRegister = newRegister;
}

// ----------------------------------------------------------------------------
/** \brief Sets the numerator of the current event and 
	 futur events duration fractions.
*/
void ARFactory::setNumerator(int newNumerator)
{
	mCurrentEvent->setNumerator(newNumerator);
	mCurrentNumerator = newNumerator;
}

// ----------------------------------------------------------------------------
/** \brief Sets the denominator of the current event and 
	 futur events duration fractions.
*/
void ARFactory::setDenominator(int newDenominator)
{
	mCurrentEvent->setDenominator(newDenominator);
	mCurrentDenominator = newDenominator;
}

// ----------------------------------------------------------------------------
/** \brief Introduces a tag into the current voice.
	input no is the number that the parser generates for advanced GUIDO

	(JB) added a switch to optimize things a little, because:
		"Switching on a consecutively numbered enum will likely be 
		optimized to a jump table"

*/
void ARFactory::createTag( const char * name, int no )
{
#if ARFTrace
 	cout << "ARFactory::createTag " << name << " " << no << endl;
#endif
	assert(mCurrentMusic);
	assert(mCurrentVoice);
	assert(!mCurrentEvent);
	assert(name);

	if (no > sMaxTagId)
		sMaxTagId = no;

	const int prevTagCount = mTags.GetCount();

	const char c = name[0];
	switch( c )
	{
		case 'a':
			if (!strcmp(name,"accelerando") || !strcmp(name,"accel"))
			{
				ARAccelerando * tmp = new ARAccelerando();
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if(!strcmp(name,"accent"))
			{
				ARAccent * tmp = new ARAccent;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"accelBegin"))
			{
				ARAccelerando * tmp = new ARAccelerando();
				tmp->setID(no);
				tmp->setAllowRange(0);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if(!strcmp(name,"alter"))
			{
				ARAlter * tmp = new ARAlter(mCurrentAlter);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}	
			else if (!strcmp(name,"auto"))
			{
				ARAuto * tmp = new ARAuto();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"accolade") || !strcmp(name,"accol"))
			{
				ARAccol * tmp = new ARAccol;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
				
			}
			else if (!strcmp(name,"acc") || !strcmp(name,"accidental"))
			{
				ARAcc * tmp = new ARAcc();
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"accelEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\accelEnd");
				tmp->setID(no); // no ist die ID
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);
			}
			break;

		case 'b':
			if(!strcmp(name,"beam") || !strcmp(name,"bm") 
				|| !strcmp(name,"b")) // ATTENTION needs to be removed later
			{
				ARBeam * tmp = new ARBeam();
				mTags.AddHead(tmp); // push();
				mCurrentVoice->AddPositionTag(tmp);				
			}
			else if (!strcmp(name,"beamBegin"))
			{
				ARBeam * tmp = new ARBeam;
				tmp->setID(no);
				tmp->setAllowRange(0);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"beamEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\beamEnd");
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);
			}
			else if (!strcmp(name,"beamsAuto"))
			{
				ARBeamState * tmp = new ARBeamState(ARBeamState::AUTO);
				mTags.AddHead(tmp);
				mBeamState = BEAMSAUTO;
				mCurrentVoice->AddTail(tmp);
				
			}
			else if (!strcmp(name,"beamsOff"))
			{
				ARBeamState * tmp = new ARBeamState(ARBeamState::OFF);
				mTags.AddHead(tmp);
				mBeamState = BEAMSOFF;
				mCurrentVoice->AddTail(tmp);				
			}
			else if(!strcmp(name,"bar"))
			{
				assert(!mCurrentEvent);
				assert(mCurrentVoice);
				ARBar * tmp = new ARBar;
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"barFormat"))
			{
				ARBarFormat * tmp = new ARBarFormat;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"bembel"))
			{
				ARBembel * tmp = new ARBembel;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);				
			}
			else if(!strcmp(name,"breathMark"))	
			{
				ARBreathMark * tmp = new ARBreathMark;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			break;

		case 'c':
			// (JB) experimental implementation of Coda
			if(!strcmp(name,"coda"))	
			{
				ARCoda * tmp = new ARCoda;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"cue"))
			{
				if (mCurrentCue)
				{
					// then we have to ignore this new cue .... 
					ARTDummy * tmp = new ARTDummy;
					mTags.AddHead(tmp); // push()
					GuidoWarn("There is a cue already active" );				
				}
				else
				{
					ARCue * tmp = new ARCue;
					mTags.AddHead(tmp);
					// mCurrentVoice->AddPositionTag(tmp);
					mCurrentCue = tmp;

					mSaveCurrentVoice = mCurrentVoice;
					mCurrentVoice= new ARMusicalVoice;

					mCurrentVoice->setVoiceNum(-1);
					ARStaff * tmpstaff ;
					if (mCurrentStaff)	
						tmpstaff = new ARStaff(mCurrentStaff);
					else tmpstaff = 
						new ARStaff(mSaveCurrentVoice->getVoiceNum());				

					mCurrentVoice->AddTail(tmpstaff);

					// create an empty-event that has the same duration as 
					// the current voice ... set the staff ....
					const TYPE_DURATION curdur (mSaveCurrentVoice->getRelativeEndTimePosition());

					// here, we have to create an empty note
					ARNote * tmpnote = new ARNote(curdur);

					mCurrentVoice->AddTail(tmpnote);

					ARNoteFormat * tmpnf = new ARNoteFormat;
					tmpnf->setSize((float) 0.6);
					tmpnf->setColor("blue");

					mCurrentVoice->AddTail(tmpnf);
				}
			}
			else if (!strcmp(name,"composer"))
			{
				ARComposer * tmp = new ARComposer();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
				
			}
			else if(!strcmp(name,"clef"))
			{
				assert(!mCurrentEvent);
				assert(mCurrentVoice);
				ARClef * tmp = new ARClef;
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddTail(tmp);
				
			}
			else if (!strcmp(name,"color") || !strcmp(name,"colour"))
			{
				ARColor * tmp = new ARColor;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
				
			}
            else if (!strcmp(name,"cresc") || !strcmp(name,"crescendo"))
			{
				ARCrescendo * tmp = new ARCrescendo();
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddPositionTag(tmp);
            }
			else if (!strcmp(name,"crescBegin"))
			{
				ARCrescendo * tmp = new ARCrescendo();
                tmp->setAssociation(ARMusicalTag::ER);
                tmp->setAllowRange(0);
				tmp->setID(no);
                mTags.AddHead(tmp);

				mCurrentVoice->AddPositionTag(tmp);		
			}
			else if (!strcmp(name,"crescEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\crescEnd");
				tmp->setID(no);
                mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);
			}
			else if (!strcmp(name,"cluster"))
			{
                ARCluster *tmp = new ARCluster();

				if (mCurrentCluster == 0)
					mCurrentCluster = tmp;
				else delete tmp;
			}
			break;

		case 'd':
			if (!strcmp(name,"daCapo")) {
				ARDaCapo * tmp = new ARDaCapo();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"daCapoAlFine")) {
				ARDaCapoAlFine * tmp = new ARDaCapoAlFine();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"daCoda")) {
				ARDaCoda * tmp = new ARDaCoda();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"dalSegno")) {
				ARDalSegno * tmp = new ARDalSegno();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"dalSegnoAlFine")) {
				ARDalSegnoAlFine * tmp = new ARDalSegnoAlFine();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"defineTag")) {
				// a new tag-definition ...
				ARDefineTag * tmp = new ARDefineTag();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"dispDur")) {
				ARDisplayDuration * tmp = new ARDisplayDuration();
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"dotFormat")) {
				ARDotFormat * tmp = new ARDotFormat(mCurrentDotFormat);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);				
			}
			else if (!strcmp(name,"doubleBar")) {
				assert(!mCurrentEvent);
				assert(mCurrentVoice);
				ARDoubleBar * tmp = new ARDoubleBar;
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"decresc") || !strcmp(name,"decrescendo") || !strcmp(name,"dim") || !strcmp(name,"diminuendo")) {
				ARDiminuendo * tmp = new ARDiminuendo;
				mTags.AddHead(tmp); // push();
				mCurrentVoice->AddPositionTag(tmp);			
			}
			else if (!strcmp(name,"decrescBegin") || !strcmp(name,"dimBegin") || !strcmp(name,"diminuendoBegin")) {
				ARDiminuendo * tmp = new ARDiminuendo;
                tmp->setAssociation(ARMusicalTag::ER);
				tmp->setAllowRange(0);
				tmp->setID(no);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"decrescEnd") || !strcmp(name,"dimEnd") || !strcmp(name,"diminuendoBegin")) {
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\dimEnd");			
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos( no, tmp );
				mTags.AddHead(tmp);
			}
			break;

		case 'e':
			if (!strcmp(name,"endBar"))
			{
				ARFinishBar * tmp = new ARFinishBar;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			break;

		case 'f':
			if (!strcmp(name,"fingering"))
			{
				ARFingering * tmp = new ARFingering;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"fine"))
			{
				ARFine * tmp = new ARFine;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"fermata"))
			{
				// Fermata-Tag
				assert(mCurrentVoice);
				assert(!mCurrentEvent);
				ARFermata * tmp = new ARFermata;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if(!strcmp(name,"fBeam"))
			{
				ARFeatheredBeam * tmp = new ARFeatheredBeam();
				mTags.AddHead(tmp); // push();
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if(!strcmp(name,"fBeamBegin"))
			{
				ARFeatheredBeam * tmp = new ARFeatheredBeam();
				tmp->setID(no);
				tmp->setAllowRange(0);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if(!strcmp(name,"fBeamEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\fBeamEnd");
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);
			}
			break;

		case 'g':
			if (!strcmp(name,"grace"))
			{
				// this is somewhat different  from the other tags ....
				// when a grace-tag is active, the time is not increased,
				// all Events are "grace-events"

				if (mCurrentGrace)
				{
					// then we have to ignore this
					// new grace .... 
					ARTDummy * tmp = new ARTDummy;
					// tmp->setStartPosition(mCurrentVoice->GetTailPosition());
					mTags.AddHead(tmp); // push()
					GuidoWarn("There is a grace already active");				
				}
				else
				{
					ARGrace * tmp = new ARGrace;
					mTags.AddHead(tmp);
					mCurrentVoice->AddPositionTag(tmp);
					mCurrentGrace = tmp;
				}
			}
			else if (!strcmp(name,"glissando"))
			{
				ARGlissando * tmp = new ARGlissando();
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddPositionTag(tmp);
				
			}
			else if (!strcmp(name,"glissandoBegin"))
			{
				// this is the id-number!
				ARGlissando * tmp = new ARGlissando;
				tmp->setAssociation(ARMusicalTag::ER);
				tmp->setAllowRange(0);
				tmp->setID(no);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);				
			}
			else if (!strcmp(name,"glissandoEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\glissandoEnd");
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);				
			} 
			
			break;

		case 'h':
			if (!strcmp(name, "harmonic"))
			{
				ARHarmonic * tmp = new ARHarmonic;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"headsNormal"))
			{
				ARTHead * tmp = new ARTHead(ARTHead::NORMAL,mCurrentHead);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"headsReverse"))
			{
				ARTHead * tmp = new ARTHead(ARTHead::REVERSE,mCurrentHead);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"headsCenter"))
			{
				ARTHead * tmp = new ARTHead(ARTHead::CENTER,mCurrentHead);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"headsLeft"))
			{
				ARTHead * tmp = new ARTHead(ARTHead::LEFT,mCurrentHead);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"headsRight"))
			{
				ARTHead * tmp = new ARTHead(ARTHead::RIGHT,mCurrentHead);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			break;

		case 'i':
			if (!strcmp(name,"instrument") || !strcmp(name,"instr"))
			{
				ARInstrument * tmp = new ARInstrument;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"intensity") || !strcmp(name,"intens") || !strcmp(name,"i"))
			{
				ARIntens * tmp = new ARIntens;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
				
			}
			break;

		case 'j':	break;

		case 'k':	
			if (!strcmp(name,"key"))
			{
				assert(mCurrentVoice);
				assert(!mCurrentEvent);
				ARKey * tmp = new ARKey;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			break;

		case 'l':	
			if (!strcmp(name,"lyrics"))
			{
				assert(mCurrentVoice);
				assert(!mCurrentEvent);
				ARLyrics * tmp = new ARLyrics;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"label"))
			{
				ARLabel * tmp = new ARLabel;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			break;
			
		case 'm':	
			if(!strcmp(name,"marcato"))
			{
				ARMarcato * tmp = new ARMarcato;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"mordent") || !strcmp(name,"mord"))
			{
				ARTrill * tmp =new ARTrill(ARTrill::MORD);
				//mTags.AddHead(tmp);
				//mCurrentVoice->AddPositionTag(tmp);
				if (mCurrentTrill == 0)
					mCurrentTrill = tmp;
				else delete tmp;
			}
			else if(!strcmp(name,"meter"))
			{
				assert(!mCurrentEvent);
				assert(mCurrentVoice);
				ARMeter * tmp = new ARMeter;
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddTail(tmp);
				
			}
			else if (!strcmp(name,"mark"))
			{
				ARMark * tmp = new ARMark;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name,"merge"))
			{
				ARMerge * tmp = new ARMerge;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
				
			}
			break;

		case 'n':	
			if (!strcmp(name,"newLine") || !strcmp(name,"newSystem"))
			{
				ARNewSystem * tmp = new ARNewSystem;
				mTags.AddHead(tmp); // push();
				mCurrentVoice->AddTail(tmp);
				
			}
			else if (!strcmp(name,"newPage") )
			{
				ARNewPage * tmp = new ARNewPage;
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddTail(tmp);
				
			}
			else if (!strcmp(name,"noteFormat"))
			{
				ARNoteFormat * tmp = new ARNoteFormat(mCurrentNoteFormat);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);				
			}
			break;

		case 'o':	
			if (!strcmp(name,"oct") || !strcmp(name,"octava"))
			{
				AROctava * tmp = new AROctava(mCurrentOctava);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			break;

		case 'p':	
			if (!strcmp(name,"pageFormat"))
			{
				ARPageFormat * tmp = new ARPageFormat;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if (!strcmp(name, "pizzicato") || !strcmp(name, "pizz"))
			{
				ARPizz * tmp = new ARPizz;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if(!strcmp(name,"port"))
			{
				ARTDummy * tmp = new ARTDummy;
				mTags.AddHead(tmp); // push()
			}
			break;

		case 'q':		break;
	
		case 'r':		
			if (!strcmp(name,"restFormat"))
			{
				ARRestFormat * tmp = new ARRestFormat(mCurrentRestFormat);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
				
			}
			else if (!strcmp(name,"ritardando") || !strcmp(name,"rit"))
			{
				ARRitardando * tmp = new ARRitardando;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"ritBegin"))
			{
				ARRitardando * tmp = new ARRitardando;
				tmp->setID(no);
				tmp->setAllowRange(0);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);

			}
			else if (!strcmp(name,"ritEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\ritEnd");
				tmp->setID(no);

				// no ist die ID
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);
			}
			else if (!strcmp(name,"repeatBegin"))
			{
				// right now, IDs are ignored for this!
				// think about it?
				ARRepeatBegin * tmp = new ARRepeatBegin;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
				// only one nesting allowed
				mCurrentRepeatBegin = tmp;				
			}
			else if (!strcmp(name,"repeatEnd"))
			{
				ARRepeatEnd * tmp = new ARRepeatEnd( mCurrentRepeatBegin );
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
//				mCurrentVoice->AddPositionTag(tmp);
				if (mCurrentRepeatBegin)
					mCurrentRepeatBegin->setRepeatEnd(tmp);				
			}
			break;

		case 's':	
			if(!strcmp(name,"slur") || !strcmp(name,"sl"))
			{
				ARSlur * tmp = new ARSlur;
				// ARTSlurBegin * tmp = new ARTSlurBegin;
		//		tmp->setStartPosition(mCurrentVoice->GetTailPosition());
				mTags.AddHead(tmp); // push()

				mCurrentVoice->AddPositionTag(tmp);
				// mCurrentVoice->AddTail(tmp);				
			}
			/*else if (!strcmp(name,"shortFermata"))
			{
				// Fermata-Tag
				assert(mCurrentVoice);
				assert(!mCurrentEvent);
				ARShortFermata * tmp = new ARShortFermata;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}*/
			else if (!strcmp(name,"slurBegin"))
			{
				ARSlur * slur = new ARSlur;
				slur->setID(no);
				slur->setAllowRange(0);
				mTags.AddHead(slur);
				mCurrentVoice->AddPositionTag(slur);
			}
			else if (!strcmp(name,"slurEnd"))
			{
				ARDummyRangeEnd * slend = new ARDummyRangeEnd("\\slurEnd");
				slend->setID(no);
				mCurrentVoice->setPositionTagEndPos( no, slend );
				mTags.AddHead(slend);
			}
            else if(!strcmp(name,"set"))
            {
				ARAuto * tmp = new ARAuto();
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if(!strcmp(name,"staccato") || !strcmp(name,"stacc"))
			{
				ARStaccato * tmp = new ARStaccato;
				mTags.AddHead(tmp);

				// it is a position-Tag (or it should be one)
				mCurrentVoice->AddPositionTag(tmp);				
			}
			else if (!strcmp(name,"staccBegin"))
			{
				ARStaccato * tmp = new ARStaccato;
				tmp->setID(no);
				tmp->setAllowRange(0);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"staccEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\staccEnd");
				tmp->setID(no);

				// no ist die ID
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);
			}
			else if (!strcmp(name,"staff"))
			{
				// das Staff-Tag
				assert(mCurrentVoice);
				assert(!mCurrentEvent); // what does that mean exactly?
				ARStaff * tmp = new ARStaff;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
				mCurrentStaff = tmp;				
			}
			else if (!strcmp(name,"stemsUp"))
			{
				ARTStem * tmp = new ARTStem(ARTStem::UP,mCurrentStem);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);				
			}
			else if (!strcmp(name,"stemsDown"))
			{
				ARTStem * tmp = new ARTStem(ARTStem::DOWN,mCurrentStem);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);				
			}
			else if (!strcmp(name,"stemsAuto"))
			{
				ARTStem * tmp = new ARTStem(ARTStem::AUTO,mCurrentStem);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
				
			}
			else if (!strcmp(name,"stemsOff"))
			{
				ARTStem * tmp = new ARTStem(ARTStem::OFF,mCurrentStem);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);			
			}
			else if (!strcmp(name,"space") )
			{
				ARSpace * tmp = new ARSpace;
				mTags.AddHead(tmp); // push();
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"staffOff"))
			{
				ARStaffOff * tmp = new ARStaffOff;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"staffOn"))
			{
				ARStaffOn * tmp = new ARStaffOn;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"special"))
			{
				ARSpecial * tmp = new ARSpecial;
				mTags.AddHead(tmp);
				// this is NOT a position-Tag!
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"systemFormat"))
			{
				ARSystemFormat * tmp = new ARSystemFormat;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"staffFormat"))
			{
				ARStaffFormat * tmp = new ARStaffFormat;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"shareLocation"))
			{
				ARShareLocation * tmp = new ARShareLocation;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"splitChord"))	// same as "chord"
			{
//				GuidoTrace("splitChord-Tag is no longer supported!");
//				ARUserChordTag * tmp = new ARUserChordTag;
//				mTags.AddHead(tmp);
//				mCurrentVoice->AddPositionTag(tmp);
			}
			// (JB) experimental implementation of Signa
			else if (!strcmp(name,"segno"))	
			{
				ARSegno * tmp = new ARSegno;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
            else if(!strcmp(name,"symbol") || !strcmp(name,"s"))
			{
				assert(mCurrentVoice);
				assert(!mCurrentEvent);
				ARSymbol *tmp = new ARSymbol;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);

                tmp->setCurrentARMusic(mCurrentMusic);
            }
			break;

		case 't':	
			if(!strcmp(name,"tie"))
			{
				ARTie * tmp = new ARTie;
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"tieBegin"))
			{
				// this is the id-number!
				ARTie * tmp = new ARTie;
				tmp->setAssociation(ARMusicalTag::ER);
				tmp->setAllowRange(0);
				tmp->setID(no);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);				
			}
			else if (!strcmp(name,"tieEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\tieEnd");
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);				
			} 
			else if(!strcmp(name,"tenuto") || !strcmp(name,"ten"))
			{
				ARTenuto * tmp = new ARTenuto;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"trill"))
			{
				ARTrill * tmp =new ARTrill(ARTrill::TRILL);
				//mTags.AddHead(tmp);
				//mCurrentVoice->AddPositionTag(tmp);

				if (!mCurrentTrill)
					mCurrentTrill = tmp;
				else delete tmp;
			}
			else if (!strcmp(name,"trillBegin"))
			{
				ARTrill * tmp = new ARTrill(ARTrill::TRILL);
				
				tmp->setID(no);
				tmp->setAllowRange(0);
				if (!mCurrentTrill)
					mCurrentTrill = tmp;
				else delete tmp;
			}
			else if (!strcmp(name,"trillEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\trillEnd");
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);
			}
			else if (!strcmp(name,"turn"))
			{
				ARTrill * tmp =new ARTrill(ARTrill::TURN);
				//mTags.AddHead(tmp);
				//mCurrentVoice->AddPositionTag(tmp);

				if (mCurrentTrill == 0)
					mCurrentTrill = tmp;
				else delete tmp;
			}
			else if(!strcmp(name,"tremolo") || !strcmp(name,"trem"))
			{
				ARTremolo * tmp = new ARTremolo;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
                
				if (!mCurrentTremolo)
					mCurrentTremolo = tmp;
				else delete tmp;
			}
			else if (!strcmp(name,"tremoloBegin") || !strcmp(name,"tremBegin"))
			{
				// this is the id-number!
				ARTremolo * tmp = new ARTremolo;
				tmp->setAssociation(ARMusicalTag::ER);
				tmp->setAllowRange(0);
				tmp->setID(no);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
				if (!mCurrentTremolo)
					mCurrentTremolo = tmp;
				else delete tmp;
			}
			else if (!strcmp(name,"tremoloEnd") || !strcmp(name,"tremEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\tremoloEnd");
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos(no, tmp);
				mTags.AddHead(tmp);				
			} 
			else if(!strcmp(name,"tempo"))
			{
				assert(mCurrentVoice); //...and also it must be the first voice 

				ARTempo * tmp = new ARTempo;
				mTags.AddHead(tmp); // push()
				mCurrentVoice->AddTail(tmp);
				
			}
			else if(!strcmp(name,"text") || !strcmp(name,"t"))
			{ // Text-Tag
				assert(mCurrentVoice);
				assert(!mCurrentEvent);
				ARText * tmp = new ARText;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);		
			}
			else if (!strcmp(name,"tuplet"))
			{
				ARTuplet * tmp = new ARTuplet;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
				
			}
			else if (!strcmp(name,"title"))
			{
				ARTitle * tmp = new ARTitle;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
				
			}
			break;
		
		case 'u':
			if (!strcmp(name,"units"))
			{
				ARUnits * tmp = new ARUnits;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);		
			}
			break;

		case 'v':
			if (!strcmp(name,"volta"))
			{
				ARVolta * tmp = new ARVolta;
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"voltaBegin"))
			{
				ARVolta * tmp = new ARVolta;
				tmp->setID(no);
				tmp->setAllowRange(0);
				mTags.AddHead(tmp);
				mCurrentVoice->AddPositionTag(tmp);
			}
			else if (!strcmp(name,"voltaEnd"))
			{
				ARDummyRangeEnd * tmp = new ARDummyRangeEnd("\\voltaEnd");
				tmp->setID(no);
				mCurrentVoice->setPositionTagEndPos( no, tmp );
				mTags.AddHead(tmp);
			}
            break;
		default:
			if (!strcmp(name,"DrHoos"))
			{
				ARDrHoos * tmp = new ARDrHoos;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);				
			}
			else if (!strcmp(name,"DrRenz"))
			{
				ARDrRenz * tmp = new ARDrRenz;
				mTags.AddHead(tmp);
				mCurrentVoice->AddTail(tmp);
			}
			else if(!strcmp(name,"..."))
			{
				ARTDummy * tmp = new ARTDummy;
				mTags.AddHead(tmp); // push()
			}
			break;
	}	// switch

	const int newTagCount = mTags.GetCount();
	if( newTagCount == prevTagCount ) // The tag was not handled (unknown)
	{
		// Generates a ARTDummy if the name did not match any tag.
		
		// ATTENTION, kf, generate warning or ignore
		// check whether the tag is a GENERIC-GGS-TAG ...
		ARTDummy * tmp = new ARTDummy;
		mTags.AddHead(tmp); // push()
	}

	assert(mTagParameterList.empty());

	ARMusicalTag * musicalTag = mTags.GetHead();
	assert(musicalTag);

	// now we can add those tags to the voice, that can either be range-Tags or simple-tags ...
	// this works as follows:
	// if the tag is closed without a range, it is just left in the mCurrentVoice
	// and the tag in the ptaglist is removed.
	// if it has a correct range, it is removed from the mCurrentVoice and
	// just stored as a PositionTag (just like it was added above)

	if (dynamic_cast<ARPositionTag *>(musicalTag) && musicalTag->getRangeSetting() ==  ARMusicalTag::RANGEDC)
	{
		assert(mCurrentVoice->GetTail() != musicalTag);
		mCurrentVoice->AddTail(musicalTag);
	}
	if (musicalTag->IsStateTag())
	{
		// then we have to save the current state somehow ....
		void * temp;
		if ((temp = dynamic_cast<AROctava *>(musicalTag)) != 0 )
			mCurrentOctava = static_cast<AROctava *>(temp);

		else if ((temp = dynamic_cast<ARTStem *>(musicalTag)) != 0 )
			mCurrentStem = static_cast<ARTStem *>(temp);

		else if ((temp = dynamic_cast<ARTHead *>(musicalTag)) != 0 )
			mCurrentHead = static_cast<ARTHead *>(temp);

		else if ((temp = dynamic_cast<ARNoteFormat *>(musicalTag)) != 0 )
			mCurrentNoteFormat = static_cast<ARNoteFormat *>(temp);

		else if ((temp = dynamic_cast<ARAlter *>(musicalTag)) != 0 )
			mCurrentAlter = static_cast<ARAlter *>(temp);

		else if ((temp = dynamic_cast<ARRestFormat *>(musicalTag)) != 0 )
			mCurrentRestFormat = static_cast<ARRestFormat *>(temp);

		else if ((temp = dynamic_cast<ARDotFormat *>(musicalTag)) != 0 )
			mCurrentDotFormat = static_cast<ARDotFormat *>(temp);
	}
	if (no != -1) musicalTag->setID((int) no);
	mCurrentTags++;
}

// ----------------------------------------------------------------------------
/** \brief Called when the end of a tag's range has been reached. 
	
	If the tag has no range, it must be called directly after the tag

 	- \\staff<1> c d : call before parsing the c "c"
	- \\slur(c d e) f : call before parsing the "f"
*/
void ARFactory::endTag()
{
#if ARFTrace
 	cout << "ARFactory::endTag " << endl;
#endif
//	assert(!mCurrentEvent);
//	assert(mCurrentVoice);
//	assert(!mTags.empty());
	
    if (mCurrentTags)
    {
        mCurrentTags--;
        ARMusicalTag * tag = mTags.RemoveHead(); // pop()

        // some of the tags need to be checked. As they end here, the behaviour of ARFactory needs to be "reset"
        // check for range ...
        if (tag == mCurrentGrace)
        {
            if (tag->getRange() == false)
            {
                // then we have to delete the grace (it will be ignored completely)
                GuidoTrace("Grace-tag without range ignored!");
            }
            // then we also delete the dispdur-tag ....
            mCurrentGrace = NULL;
        }
        else if (tag == mCurrentTrill)
        {
            if (tag->getRange() == false)
                GuidoTrace("trill-tag without range ignored!");
        }
        else if (tag == mCurrentCue)
        {
            mCurrentVoice->ConvertToNormalForm();
            mCurrentMusic->AddTail(mCurrentVoice);
            mCurrentVoice = mSaveCurrentVoice;
            mSaveCurrentVoice = NULL;
            mCurrentCue = NULL;

            ARCue * cue = dynamic_cast<ARCue *>(tag);
            const TagParameterString * tps = cue->getName();
            if (tps)
            {
                // then we have to add the name ....
                ARText * txt = new ARText(tps->getValue(),0);
                if (cue->getDY())
                {
                    TagParameterFloat * tpf = TagParameterFloat::cast(cue->getDY()->getCopy());
                    txt->setDY(tpf);
                }
                if (cue->getDX())
                {
                    TagParameterFloat * tpf = TagParameterFloat::cast(cue->getDX()->getCopy());
                    txt->setDX(tpf);
                }
                if (cue->getColor())		txt->setColor(cue->getColor()->getValue());				
                if (cue->getSize())			txt->setSize(cue->getSize()->getValue());
                mCurrentVoice->AddTail(txt);
                // we need to be able to specify the offset!
            }
            delete tag;
            tag = NULL;
            return;
        }
        else if (tag == mCurrentCluster)
        {
            if (tag->getRange() == false)
                GuidoTrace("cluster-tag without range ignored!");
        }
        else if (tag == mCurrentTremolo)
        {
            if (tag->getRange() == false)
            {
                // then we have to delete the grace (it will be ignored completely)
                GuidoTrace("Tremolo-tag without range ignored!");
            }
            
            //GuidoPos posTrem = mCurrentTremolo->getPosition();
            GuidoPos lastEventPos = mCurrentVoice->getLastEventPosition();
            TYPE_TIMEPOSITION timePos;
            bool found = false;
            int oct;
            while(lastEventPos && !found)
            {
                ARNote * n = dynamic_cast<ARNote*>(mCurrentVoice->GetPrev(lastEventPos));
                if(n && n->getPitch())
                    oct = n->getOctave();
                if(n && n->getDuration())
                {
                    found = true;
                    Fraction duration = n->getDuration();
                    Fraction totalDuration = Fraction(n->getDuration().getNumerator()*2, n->getDuration().getDenominator());
                    std::vector<ARNote*> notes = mCurrentTremolo->createSecondNotes(duration, oct);
                    if(notes.size()) // if we have at least one note in the vector, we'll have to devide the duration of our main note and adjust the graphical aspect with DisplayDuration
                    {
                        ARDisplayDuration * tmpdspdur = new ARDisplayDuration;
                        tmpdspdur->setDisplayDuration( totalDuration);
                        mCurrentVoice->AddPositionTag(tmpdspdur);
                
                
                        ARNote * note = notes[0];
                
                        if(notes.size()>1)
                            createChord();
                
                        mCurrentVoice->AddTail(note);
                        int i = 1;
                        while(i < notes.size())
                        {
                            ARNote * chordNote = notes[i];
                            mCurrentVoice->AddTail(chordNote);
                            i++;
                        }
                
                        if(notes.size()>1)
                            addChord();
                
                        ARDummyRangeEnd * dummy = new ARDummyRangeEnd("\\dispDurEnd");
                        mCurrentVoice->setPositionTagEndPos(-1,dummy,tmpdspdur);
                    }
                }
            }
            
            // then we also delete the dispdur-tag ....
            mCurrentTremolo = NULL;
        }

        ARPositionTag * myptag = dynamic_cast<ARPositionTag *>(tag); 
        if (tag->getRange() && tag->getRangeSetting() == ARMusicalTag::NO)
        {
            if (mCurrentTrill)
            {
                delete mCurrentTrill;
                mCurrentTrill = NULL;
            }
            else if (mCurrentCluster)
            {
                delete mCurrentCluster;
                mCurrentCluster = NULL;
            }

            GuidoWarn("Tag range ignored (1)");
            tag->setRange( false );	
        }

        if (!tag->getRange() && tag->getRangeSetting() == ARMusicalTag::ONLY)
        {
            GuidoWarn("Tag has no range - ignored");
            if (myptag) mCurrentVoice->RemovePositionTag(myptag);
            delete tag;
            return;
        }

        // now we need to set the correct end-positions for range tags ...
        if (myptag)
        {
            //		ARAlter * alter = dynamic_cast<ARAlter *>(tag); 
            if (/*!alter &&*/ ((tag->getRange() && myptag->getStartPosition() == NULL)
                || (!tag->getRange() && tag->getRangeSetting() == ARMusicalTag::RANGEDC)))
            {
                // we do not have a start-position yet ...
                // then the range is empty ! 
                if (tag->getRangeSetting() == ARMusicalTag::ONLY)
                {
                    // then the Tag must be completly discarded ....
                    mCurrentVoice->RemovePositionTag(myptag);
                    delete tag;
                    return;
                }

                // otherwise we replace the tag as a regular tag ... no positiontag any longer ...
                assert(tag->getRangeSetting() == ARMusicalTag::RANGEDC);

                // otherwise we have to remove the position tag ....
                // we need this (if range was empty ..)
                tag->setRange(0);
                mCurrentVoice->RemovePositionTag(myptag);
                // sets the Don't Care association 
                tag->setAssociation(ARMusicalTag::DC);			
                // redo this so that \text<"bla">(  /i<"p"> ) works in the correct order!
                if(!mTagParameterList.IsEmpty()) {		// here was an assert
                    // removed to allow dynamic editing
                    mTagParameterList.RemoveAll();
                }
                return;
            }

            if (/*alter ||*/ (tag->getRange() && (dynamic_cast<ARTagEnd *>(tag) == 0)))
            {
                if (tag->IsStateTag())
                {
                    // then we have to split
                    // the tag in two ordinary tags...
                    tag->setRange(0);
                    mCurrentVoice->RemovePositionTag(myptag);

                    AROctava * myoct;
                    ARTStem * mystem;
                    ARTHead * myhead;
                    ARNoteFormat * mynf;
                    ARDotFormat * mydf;
                    ARRestFormat * myrf;
                    ARAlter * myal;

                    if ((myoct = dynamic_cast<AROctava *>(tag)) != 0 )
                    {
                        AROctava * noct = dynamic_cast<AROctava *>( myoct->getEndTag());
                        mCurrentOctava = noct;
                        mCurrentVoice->AddTail(noct);
                    }
                    else if ((mystem = dynamic_cast<ARTStem *>(tag)) != 0 )
                    {
                        ARTStem * nstem = dynamic_cast<ARTStem *>(mystem->getEndTag());
                        mCurrentStem = nstem;
                        mCurrentVoice->AddTail(nstem);
                    }
                    else if ((myhead = dynamic_cast<ARTHead *>(tag)) != 0 )
                    {
                        ARTHead * nhead = dynamic_cast<ARTHead *>(myhead->getEndTag());
                        mCurrentHead = nhead;
                        mCurrentVoice->AddTail(nhead);

                    }
                    else if ((mynf = dynamic_cast<ARNoteFormat *>(tag)) != 0 )
                    {
                        ARNoteFormat * nnf = dynamic_cast<ARNoteFormat *>(mynf->getEndTag());
                        mCurrentNoteFormat = nnf;
                        mCurrentVoice->AddTail(nnf);
                    }
                    else if ((mydf = dynamic_cast<ARDotFormat *>(tag)) != 0 )
                    {
                        ARDotFormat * ndf = dynamic_cast<ARDotFormat *>(mydf->getEndTag());
                        mCurrentDotFormat = ndf;
                        mCurrentVoice->AddTail(ndf);
                    }
                    else if ((myrf = dynamic_cast<ARRestFormat *>(tag)) != 0 )
                    {
                        ARRestFormat * nrf = dynamic_cast<ARRestFormat *>(myrf->getEndTag());
                        mCurrentRestFormat = nrf;
                        mCurrentVoice->AddTail(nrf);
                    }
                    else if ((myal = dynamic_cast<ARAlter *>(tag)) != 0 )
                    {
                        ARAlter * nal = dynamic_cast<ARAlter *>(myal->getEndTag());
                        mCurrentAlter = nal;
                        mCurrentVoice->AddTail(nal);
                    }
                    else
                    {
                        assert(false);
                    }
                }
                else
                {
                    // in this case we can just close the range .. the position is set ...
                    // Introduce a dummy RangeEnd ..
                    // That is a ")" ...
                    ARDummyRangeEnd * ard = new ARDummyRangeEnd();
                    // is the dummy range end added to the voice?
                    mCurrentVoice->setPositionTagEndPos( tag->getID(), ard, tag );
                    // then we have to delete the tag in the voice ...
                    mCurrentVoice->removeTag(tag);
                }
            }				
        }

        ARRepeatEnd  * arre;
        ARText * atext;
        ARSymbol *asymbol;
        if(( atext = dynamic_cast<ARText *>(tag)) != 0 )
        {
            // bereich ueber den der Text laueft ?
            // why is this? -> check it out later!
            atext->setRelativeEndTimePosition(mCurrentVoice->getDuration());
            tag = NULL;
        }
        else if(( asymbol = dynamic_cast<ARSymbol *>(tag)) != 0 )
        {
            asymbol->setRelativeEndTimePosition(mCurrentVoice->getDuration());
            tag = NULL;
        }
        else if ((arre = dynamic_cast<ARRepeatEnd *>(tag)) != 0 )
        {
            if (arre->repbeg) // a repeat-end
            {
                TYPE_DURATION dur (arre->repbeg->dur);
                int numrepeat = arre->numrepeat;
                if (numrepeat == 0) numrepeat = 1;

                ++numrepeat;
                dur.setNumerator(dur.getNumerator() * numrepeat);
                dur.normalize();
                //mCurrentVoice->setDuration(arre->repbeg->getRelativeTimePosition() + dur);
            }
            if (arre->getRange()) // the tag has a range ...
            {
                ARRepeatEndRangeEnd * tmp = new ARRepeatEndRangeEnd(arre); // !
                mCurrentVoice->AddTail(tmp);
            }
            tag = NULL;
        }
        else if (dynamic_cast<ARTDummy *>(tag))
        {
            // tag is not set to NULL -> it will be deleted
        }
        else tag = NULL;

	    // now delete tags which are not needed anymore
        //	assert(mTagParameterList.empty());
	
        if (tag != NULL)  delete tag;
    }
}

// ----------------------------------------------------------------------------
/** \brief Must be called after parameter and before the range

  example.. 
  - \\tag<1,2,3>(c d e ) 	// call before parsing c
  - \\tag<1,2> c d          // call before parsing c
*/
void ARFactory::addTag()
{
#if ARFTrace
 	cout << "ARFactory::addTag " << endl;
#endif
	// end of parameter list reached
	// set the tag parameter

	ARMTParameter * theTag = dynamic_cast<ARMTParameter *>(mTags.GetHead());	
	if (mCurrentTrill)
		mCurrentTrill->setTagParameterList(mTagParameterList);
    else if (mCurrentCluster)
        mCurrentCluster->setTagParameterList(mTagParameterList);
	else if (theTag)
		theTag->setTagParameterList( mTagParameterList );
	// the remaining will just be stored

	// ATTENTION, if the tag is a repeatEnd
	// tag, the voice-time needs to be adjusted
/*	ARRepeatEnd * arre = dynamic_cast<ARRepeatEnd *>(theTag);
	if (arre)
	{
		const int i = arre->numrepeat;
		if (i>1)
		{
			if (arre->repbeg)
			{
				//mCurrentVoice->setDuration(
				//	arre->repbeg->getRelativeTimePosition() +
				//	arre->repbeg->dur * i);
			}
		}
	}
*/
	mTagParameterList.RemoveAll();
	assert(mTagParameterList.empty());
}

// ----------------------------------------------------------------------------
/** \brief Must be called if the current tag has a range (can be empty)
*/
void ARFactory::tagRange()
{
#if ARFTrace
 	cout << "ARFactory::tagRange " << endl;
#endif
	// this justs informs the tag, that	it has a range.
	// it is checked in EndTag, whether
	// the tag can actually use the range or not ...
	ARMusicalTag * tag = mTags.GetHead();
	tag->setRange(1);
}

// ----------------------------------------------------------------------------
/** \brief Adds a new string parameter to the current tag.
*/
void ARFactory::addTagParameter(const char * parameter)
{
#if ARFTrace
 	 cout << "ARFactory::addTagParameter TYPE_TAGPARAMETER_STRING " << parameter << endl;
#endif
	if (dynamic_cast<ARMTParameter*>(mTags.GetHead()) || mCurrentTrill || mCurrentCluster)
		mTagParameterList.AddTail(new TagParameterString(parameter));
}

// ----------------------------------------------------------------------------
/** \brief  Adds a new integer parameter to the current tag.
*/
void ARFactory::addTagParameter(TYPE_TAGPARAMETER_INT parameter)
{
#if ARFTrace
	cout << "ARFactory::addTagParameter TYPE_TAGPARAMETER_INT " << parameter << endl;
#endif
	// we have assume the DEFAULT Unit here....
	if (dynamic_cast<ARMTParameter*>(mTags.GetHead()) || mCurrentTrill || mCurrentCluster)
	{
		TagParameterInt * ntpi = new TagParameterInt(parameter);
		// float npar = (float) (parameter * LSPACE/2);
		ntpi->setValue((int) parameter);
		mTagParameterList.AddTail(ntpi);
	}
}

// ----------------------------------------------------------------------------
/** \brief Adds a new floating-point parameter to the current tag.
*/
void ARFactory::addTagParameter(TYPE_TAGPARAMETER_REAL parameter)
{
#if ARFTrace
	cout << "ARFactory::addTagParameter TYPE_TAGPARAMETER_REAL " << parameter << endl;
#endif
	if (dynamic_cast<ARMTParameter*>(mTags.GetHead()) || mCurrentTrill || mCurrentCluster)
	{
		TagParameterFloat * ntpf = new TagParameterFloat((float) parameter);
		// float npar = (float) (parameter * LSPACE/2);
		ntpf->setValue((float) parameter);
		mTagParameterList.AddTail(ntpf);
	}
}

// ----------------------------------------------------------------------------
/** \brief Not yet implemented.
*/
void ARFactory::setTagParameterList(TagParameterList)
{
	assert(false); // not implemented
}


// ----------------------------------------------------------------------------
/** \brief Defines the unit (cm, inch, mm...) of the last added tag-paramater
*/
void ARFactory::setUnit( const char * s )
{
	TagParameter * tpar = /*dynamic cast<TagParameter *>*/(mTagParameterList.GetTail());
	if (tpar)
	{
		// tpar->unit = s;
		// now we convert the given unit to
		// internal units (cm and point ...)

		TagParameterFloat * tpf = TagParameterFloat::cast(tpar);
		assert(tpf);

		tpf->setUnit(s);
	}
}

// ----------------------------------------------------------------------------
/** \brief Defines the name (when applicable) of the last added tag-parameter
*/
void ARFactory::setParameterName( const char * name)
{
	TagParameter * tpar =/* dynamic cast<TagParameter *>*/(mTagParameterList.GetTail());
	if (tpar)
		tpar->name = name;
}

// ----------------------------------------------------------------------------
float ARFactory::UndoTransform(const float val)
{
	// this must be refined, if 
	// the default unit changes!
	return (float) (val * 2.0f / (float)LSPACE);
}
