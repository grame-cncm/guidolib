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

#ifdef WIN32
# pragma warning (disable : 4800)
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <typeinfo>
#include <fstream>
#include <cstring>	// strcat
#include <vector>
#include <map>
#include <algorithm>

#include "TagList.h"
#include "GUIDOInternal.h"	// for gGlobalSettings.gAutoBarlines


#include "ARAlter.h"
#include "ARAuto.h"
#include "ARAutoBeam.h"
#include "ARAutoBeamEnd.h"
#include "ARBar.h"
#include "ARBase.h"
#include "ARBeamState.h"
#include "ARChordComma.h"
#include "ARChordTag.h"
#include "ARClef.h"
#include "ARCluster.h"
#include "ARCoda.h"
#include "ARColor.h"
#include "ARDisplayDuration.h"
#include "ARDotFormat.h"
#include "ARDummyRangeEnd.h"
#include "ARFeatheredBeam.h"
#include "ARFinishBar.h"
#include "ARGlissando.h"
#include "ARGrace.h"
#include "ARKey.h"
#include "ARLyrics.h"
#include "ARMerge.h"
#include "ARMeter.h"
#include "ARMMRest.h"
#include "ARMusic.h"
#include "ARMusicalEvent.h"
#include "ARMusicalVoice.h"
#include "ARMusicalVoiceState.h"
#include "ARNaturalKey.h"
#include "ARNewPage.h"
#include "ARNewSystem.h"
#include "ARNote.h"
#include "ARNoteFormat.h"
#include "AROctava.h"
#include "ARPossibleBreak.h"
#include "ARRepeatBegin.h"
#include "ARRepeatEnd.h"
#include "ARRest.h"
#include "ARSecondGlue.h"
#include "ARSegno.h"
#include "ARShareLocation.h"
#include "ARShareStem.h"
#include "ARSlur.h"
#include "ARStaff.h"
#include "ARStaffFormat.h"
#include "ARSymbol.h"
#include "ARText.h"
#include "ARTie.h"
#include "ARTremolo.h"
#include "ARTrill.h"
#include "ARTStem.h"
#include "ARTuplet.h"
#include "GRSingleNote.h"
#include "GRTrill.h"

#include "GMNCodePrintVisitor.h"
#include "benchtools.h"

using namespace std;

#define GLISSANDOEND	"\\glissandoEnd"
#define TIEEND			"\\tieEnd"
#define DISPDUREND		"\\dispDurEnd"
#define SHARESTEMEND	"\\shareStemEnd"


//____________________________________________________________________________________
ARMusicalVoice::ARMusicalVoice()	: ObjectList(1), // owns elements ...
  currentChord(NULL), currentShareLocation(NULL), chordgrouplist(NULL),
  numchordvoice(-1), isInChord(false), voicenum(0),
  endState(NULL), readmode(CHORDMODE), mPosTagList(NULL)
{
	mStartPosTagList = new StartPositionTagList(0); // ownsElements

	lastevposition = (GuidoPos) NULL;
	pitchsum = 0;
	sum = 0;

	mCurVoiceState = new ARMusicalVoiceState();
    repeatBeginList = new std::vector<ARRepeatBegin *>();
}

//____________________________________________________________________________________
ARMusicalVoice::~ARMusicalVoice()
{
	delete chordgrouplist;
	chordgrouplist = 0;

	if (mPosTagList)
	{
		// we have to ignore tags that have no range (because this can only happen if we have
		// a parse-error, otherwise we get an access violation)
		GuidoPos pos = mPosTagList->GetHeadPosition();
		while (pos)
		{
			ARPositionTag * arpt = mPosTagList->GetAt(pos);
			GuidoPos oldpos = pos;
			mPosTagList->GetNext(pos);
			if (arpt && arpt->getPosition() == NULL)
			{
				GuidoWarn("Deleting ptag from mPosTagList");
				mPosTagList->setOwnership(0);
				mPosTagList->RemoveElementAt(oldpos);
				mPosTagList->setOwnership(1);
			}
		}
		delete mPosTagList;
		mPosTagList = 0;
	}

	delete mStartPosTagList;	mStartPosTagList = 0;
	delete mCurVoiceState;		mCurVoiceState = 0;
	delete endState;			endState = 0;
    delete repeatBeginList;     repeatBeginList = 0;
}

//____________________________________________________________________________________
/** \brief Sets the ARMusicalVoiceState at the beginning of the voice.
*/
GuidoPos ARMusicalVoice::GetHeadPosition(ARMusicalVoiceState &vst) const
{
	// we have to deal with chords, just as if we were in the GetNext->routine.
	// if the very first thing in the voice is a chord, then I have to set the chordState ....
	vst.DeleteAll();

	// set the position to the very start.
	vst.vpos = ObjectList::GetHeadPosition();

	if (mPosTagList)
        vst.ptagpos = mPosTagList->GetHeadPosition();
	else
        vst.ptagpos = NULL;

	// there is NOTHING in the voice.
	if (vst.vpos == NULL) {
		vst.curtp = DURATION_0;
		return vst.vpos;
	}

//	ARMusicalTag * mytag;
	ARMusicalObject *obj = GetAt(vst.vpos);

	// now for the new mPosTagList-stuff: the position is remembered in vst.ptagpos ...
	if (mPosTagList) {
		while (vst.ptagpos) {
			ARPositionTag *ptag = mPosTagList->GetAt(vst.ptagpos);
			// remember that we are at the beginning ...
			ARTagEnd *artgend = ARTagEnd::cast(ptag);

			if (artgend)
                break;

			else if (ptag->getPosition() == vst.vpos)
				vst.AddPositionTag(ptag); // add the tag to the current position tags ...
			else
                break;

			mPosTagList->GetNext(vst.ptagpos);
		}
	}

	/* now we are not eating all the state-tags any longer. This needs to be done in the voice-manager.
	Otherwise, no real graphical representations can be made .... */
//	mytag = NULL;
	if (obj)
        vst.curtp = obj->getRelativeTimePosition();
	else
        vst.curtp = DURATION_0;

	// now I have to see whether we are in chordmode ....
	if (readmode == CHORDMODE && vst.curchordtag)
	{
		// then we are at the first event of a chordtag we can save the chordstate ...
		// Because we are at the very start there can be no previous chordState information!
		assert(!vst.chordState);
		assert(!vst.prevchordState);

		// temporarily I save the prevchordState and the chordState because they must
		// not be copied at all (all this is only done to be able to back up one event
		// position when inserting breaks)
		vst.prevchordState = NULL;
		vst.chordState = new ARMusicalVoiceState(vst);
	}

	return vst.vpos;
}

GuidoPos ARMusicalVoice::GetHeadPosition() const
{
    return ObjectList::GetHeadPosition();
}

//____________________________________________________________________________________
/** \brief Backs up the position.

	it really decrements the position. does not return anything.
	It only works if the Event at the previous Position is an Event (Note/Rest).
	It adjusts the PositionTagList this method is called from the ARVoiceManager
	from InsertBreak and CheckBreakPosition. in both cases (hopefully), the routine is called
	right after an event has been read, because breaks are only inserted right after or in the middle
	of events.
*/
void ARMusicalVoice::GetPrevEvent(GuidoPos & pos, ARMusicalVoiceState & vst) const
{
	// we have to deal with Chords!!!!!
	// this is our situation:
	//            ev     ev_or_tag
	//    1|________|6
	//    2|______________________|10
	//          3|__|7
	//          4|________________|11
	//          5|_____________________|14
	//                 8|_________|12
	//                 9|_________________|15
	//                             13|___________
	//                     ^
	//                     |
	//                vst.vpos (=pos)
	//				  vst.ptagpos zeigt auf 10
	//             ^
	//             |tmppos
	// or the situation is totally different and we have pos == NULL
	// (we want to start backwards at the end of the voice)

	vst.DeleteAddedAndRemovedPTags();
	GuidoPos tmppos = pos;
	if (!tmppos)	tmppos = ObjectList::GetTailPosition();
	else			ObjectList::GetPrev(tmppos);

	ARMusicalObject * ar = ObjectList::GetAt(tmppos);
	assert( ARMusicalEvent::cast(ar));

	// this is set, if the previous is event is a chord; if this is the case, we can use
	// the previous-chordstate to set the current voice state.
	int previschord = 0;
	// remember the curchordtag of the VoiceState. this changes, if the previous event is
	// part of another chord.
	ARChordTag * savedchordtag = vst.curchordtag;

	// check the Position-Tags (End) do the following:
	// - remove those tags, that have starting-pos == pos
	// - add those tags, that have ending-pos == pos
	//   AND have not starting-pos == pos
	if (mPosTagList)
	{
		// new stuff: mPosTagList in NormalForm
		GuidoPos ptagtmpprevpos = NULL;
		if (vst.ptagpos == NULL)
		{
			vst.ptagpos = mPosTagList->GetTailPosition();
			while (vst.ptagpos)
			{
				ptagtmpprevpos = vst.ptagpos;
				ARPositionTag * arpt = mPosTagList->GetPrev(vst.ptagpos);
				ARTagEnd * artgend = ARTagEnd::cast(arpt);

				if (artgend && artgend->getPosition() == tmppos )
				{
					vst.AddPositionTag(arpt->getCorrespondence());
					if ((readmode == CHORDMODE) && (savedchordtag != vst.curchordtag))
					{
						// then the previous event is a chord
						if (vst.prevchordState)
						{
							assert(vst.chordState || vst.prevchordState);
							previschord = 1;
							break;
						}
					}
				}
				else
				{
					// then, the position is set to the next following one ...
					mPosTagList->GetNext(ptagtmpprevpos);
					vst.ptagpos = ptagtmpprevpos;
					break;
				}
			}
		}
		else
		{
			while (vst.ptagpos)
			{
				ptagtmpprevpos = vst.ptagpos;
				mPosTagList->GetPrev(vst.ptagpos);

				// it can be, that there is NO previous entry in the position tags ...
				if (!vst.ptagpos)
				{
					vst.ptagpos = ptagtmpprevpos;
					break;
				}

				ARPositionTag * arpt = mPosTagList->GetAt(vst.ptagpos);
				ARTagEnd * artgend = ARTagEnd::cast(arpt);
				if (!artgend && arpt->getPosition() == pos )
				{
					// these are the PositionTags that point to the second eventposition
					vst.RemovePositionTag(arpt);
				}
				else if (artgend && arpt->getPosition() == tmppos )
				{
					// these point to the first event (the one that will be the current event ...)
					vst.AddPositionTag(arpt->getCorrespondence());
					if (readmode == CHORDMODE && savedchordtag != vst.curchordtag)
					{
						// then the previous event is a chord
						assert(vst.chordState || vst.prevchordState);
						previschord = 1;
						break;
					}
				}
				else
				{
					// then we are finished.
					mPosTagList->GetNext(vst.ptagpos);
					break;
				}
			}
		}
	}

	if (previschord)
	{
		ARMusicalVoiceState * tmp = NULL;
		{
			assert(vst.prevchordState);
			tmp = new ARMusicalVoiceState(*vst.prevchordState);
		}
		assert(tmp);
		vst = * tmp;
		vst.chordState = tmp;
		// delete tmp;
		pos = vst.vpos;
		return;
	}

	pos = tmppos;
	vst.vpos = pos;
	vst.curtp = ar->getRelativeTimePosition();
}

//____________________________________________________________________________________
/** \brief Gets the Object at Position and increments the Position-Pointer
	this has to be done according to chordmode.

	if we have CHORDMODE, all events within a chord-range must be overread ....
*/
ARMusicalObject * ARMusicalVoice::GetNext(GuidoPos &pos, ARMusicalVoiceState &vst) const
{
	vst.DeleteAddedAndRemovedPTags();
	int         overreadchord = 0;
	ARChordTag *savedchordtag = NULL;

	if (vst.curchordtag && readmode == CHORDMODE) {
		overreadchord = 1;
		savedchordtag = vst.curchordtag;
	}

	ARMusicalObject *first = 0;
	ARMusicalObject *obj   = 0;
	do {
		if (mPosTagList) {
			while (vst.ptagpos) {
				ARPositionTag *ptag    = mPosTagList->GetAt(vst.ptagpos);
				ARTagEnd      *artgend = ARTagEnd::cast(ptag);
				if (artgend && ptag->getPosition() == pos)
					vst.RemovePositionTag(ptag->getCorrespondence());
				else
					break;
				mPosTagList->GetNext(vst.ptagpos);
			}
		}

		GuidoPos prevpos = pos;
		if (!pos) break;

		obj = ObjectList::GetNext(pos);
		if (first == 0) first = obj;

        if (obj) {
            ARMusicalTag *mytag = dynamic_cast<ARMusicalTag *>(obj);

            if (mytag) {
                if (mytag->IsStateTag()) {
                    const ARStaff *mynewstf = static_cast<const ARStaff *>(mytag->isARStaff());
                    if ((mynewstf) != NULL) {
                        ARStaff *tmp = (ARStaff *)(vst.getCurStateTag(typeid(ARStaff)));
                        if (tmp && tmp->getStaffNumber() != mynewstf->getStaffNumber()) {
                            vst.RemoveCurStateTag(typeid(ARClef));
                            vst.RemoveCurStateTag(typeid(ARKey));
                        }
                    }
                    vst.AddStateTag(mytag);
                }

                if (mytag->isARBar()) {
                    vst.curlastbartp  = obj->getRelativeTimePosition();
                    vst.curlastbarpos = prevpos;
                }
            }
        }

		if (mPosTagList) {
			while (vst.ptagpos) {
				ARPositionTag *ptag    = mPosTagList->GetAt(vst.ptagpos);
				ARTagEnd      *artgend = ARTagEnd::cast(ptag);
				if (!artgend && ptag->getPosition() == pos)
					vst.AddPositionTag(ptag);
				else
                    break;
				mPosTagList->GetNext(vst.ptagpos);
			}
		}
	}
	while (overreadchord && savedchordtag == vst.curchordtag);

	vst.vpos = pos;
	if (first)
		vst.curtp = first->getRelativeEndTimePosition();
	else
		vst.curtp = DURATION_0;

	if (readmode == CHORDMODE) {
		if (vst.chordState) {
			delete vst.prevchordState;
			vst.prevchordState = vst.chordState;
			vst.chordState     = NULL;
		}

		if (vst.curchordtag) {
			ARMusicalVoiceState *tempsave = vst.prevchordState;
			vst.prevchordState = NULL;
			vst.chordState     = new ARMusicalVoiceState(vst);
			vst.prevchordState = tempsave;
		}
	}
	return first;
}

ARMusicalObject * ARMusicalVoice::GetNextObject(GuidoPos &pos) const
{
    return ObjectList::GetNext(pos);
}

//____________________________________________________________________________________
ObjectList * ARMusicalVoice::getARMusicalObjectsAtTimePosition( const TYPE_TIMEPOSITION & timeposition )
{
	// not needed and not tested with new chords...
	assert(false);

	assert(timeposition>=relativeTimePosition);
	assert(timeposition<getRelativeEndTimePosition());
	if(timeposition >= getDuration())
		return NULL; // timeposition ist ausserhalb Staff...
	// ...because oft asserts this should never happen
	// ... must be done that GRStaff::createStaffElements can work also with "empty" Staffs!
	ObjectList * olist = new ObjectList();
	ARMusicalObject * e;
	TYPE_TIMEPOSITION t;
	GuidoPos pos = GetTailPosition();
	// I don't understand why it's done in reverse direction
	while(pos)
	{
		e = GetAt(pos);
		t = e->getRelativeTimePosition();
		if( t <= timeposition)
		{
			if(t < timeposition && e->getRelativeEndTimePosition() > timeposition)
				olist->AddTail(e);
			else if(t == timeposition)
				olist->AddTail(e);
		}
		e = ObjectList::GetPrev(pos);
	}
	return olist;
}

//____________________________________________________________________________________
GuidoPos ARMusicalVoice::getPosAtTimePosition(const TYPE_TIMEPOSITION & timeposition)
{
	assert(false);
	// not needed and not tested with new chords ....

	assert(timeposition>=relativeTimePosition);
	assert(timeposition<getRelativeEndTimePosition());
	if(timeposition >= getDuration())
		return NULL; // timeposition is outside of staff
	// ...should never happen because of asserts
	// ... must be checked that GRStaff::createStaffElements works also with "empty" Staffs!
	ARMusicalObject * e;
	TYPE_TIMEPOSITION t;
	GuidoPos pos = GetTailPosition();
	GuidoPos retpos = NULL;
	// I don't understand why this needs to be done backwards
	while(pos)
	{
		e = GetAt(pos);
		t = e->getRelativeTimePosition();

		if(t <= timeposition)
		{
			if(t == timeposition)
			{
                if (static_cast<ARNewSystem *>(e->isARNewSystem()))
					return retpos;
				retpos = pos;
			}
			else if(t < timeposition && e->getRelativeEndTimePosition() > timeposition)
				retpos = pos;
			else
				return retpos;
		}
		e = ObjectList::GetPrev(pos);
	}
	assert(retpos != NULL);
	return retpos;
}


//____________________________________________________________________________________
GuidoPos ARMusicalVoice::InsertAtTail(ARMusicalObject *newMusicalObject)
{
	assert(newMusicalObject);
	GuidoPos tmp = ObjectList::AddTail(newMusicalObject);
	return tmp;
}

//____________________________________________________________________________________
/** Attention, AddTail does not automatically add at the tail. It depends on numchordvoice.
	If numchordvoice==1, the tags are added at a the saved location of posfirstinchord
	This is necessary, so that State-Information is available before a chord is parsed later in the
	graphical transformation (like stem-direction, meter and keychanges ....)
	Semantically this is completely OK,
	There is no way of restoring the original input later (that is :   \\meter<"3/4"> \\stemsUp { c } is
	identical to { \\meter<"3/4"> \\stemsUp c }
*/
// C.D. optimized 22/10/14
GuidoPos ARMusicalVoice::AddTail(ARMusicalObject *newMusicalObject)
{
	assert(newMusicalObject);

	newMusicalObject->setVoiceNum (getVoiceNum());
	// test, if positiontags are falsely entered here:
//	ARPositionTag *ptag = dynamic_cast<ARPositionTag *>(newMusicalObject);
	ARMusicalTag  *mtag = dynamic_cast<ARMusicalTag *>(newMusicalObject);

	ARChordGroup *group = 0;

	if (isInChord && ARMusicalEvent::cast(newMusicalObject)) {
		// we are within a chord...
		// then I have to deal with displayduration and stem-sharing issues...
		if (!chordgrouplist)
			chordgrouplist = new ChordGroupList(1); // owns elements...

		group = chordgrouplist->GetTail();
        
		if (!group) {
			group = new ARChordGroup();
			group->dur = newMusicalObject->getDuration();
			chordgrouplist->AddTail(group);

			// now I have to insert an empty event
			// because shareStem needs an empty event at the beginning...
			ARNote *tmpnote = new ARNote("empty", 0, 0, 0, 1, 80);
			tmpnote->setAuto(true);
			group->startpos = ObjectList::AddTail(tmpnote);
		}

		if (group->dur != newMusicalObject->getDuration())
		{
			group = new ARChordGroup();
			group->dur = newMusicalObject->getDuration();
			chordgrouplist->AddTail(group);

			// now I have to insert an empty event
			// because shareStem needs an empty event at the beginning...
			ARNote * tmpnote = new ARNote("empty", 0, 0, 0, 1, 80);
			tmpnote->setAuto(true);
			group->startpos = ObjectList::AddTail(tmpnote);
		}
		newMusicalObject->setDuration(DURATION_0);
		++numchordvoice;
	}

	TYPE_DURATION d = getDuration();
	newMusicalObject->setRelativeTimePosition(d);
	d += newMusicalObject->getDuration();
	setDuration(d);
	GuidoPos tmp = NULL;

	if (mtag && numchordvoice == 0 && mtag->IsStateTag())
		tmp = ObjectList::AddElementAt(posfirstinchord, newMusicalObject); // OK, then we have to add the elements at posfirstinchord
		                                                                   // the timeposition is still OK...
	else
		tmp = ObjectList::AddTail(newMusicalObject);

	// now check, if there are and Tags, that need to be added to the current Position

	// mStartPosTagList contains all PositionTags, that have no start-Position yet.
	// During AddTail, these Tags get their StartPositions

	// I am not sure about this... maybe it helps not to rely only on Musical-Events...
	// NO, one golden rule of GUIDO is:
	// a range in GUIDO always has events inside (maybe even an empty-note with zero-duration...)
	if (ARMusicalEvent::cast(newMusicalObject)) {
		while (mStartPosTagList->GetCount() > 0) {
			ARPositionTag * tb = mStartPosTagList->RemoveHead();
			tb->setStartPosition(tmp); // Position
		}

		lastevposition = tmp;
		// we also do a statistics on pitch ...
        ARNote *arnote = static_cast<ARNote *>(newMusicalObject->isARNote());
		if (arnote && arnote->getPitch() != EMPTY) {
			pitchsum += arnote->getOctave() * 7 + arnote->getPitch();
			++ sum;
		}
	}

	if (group)
		group->endpos = tmp;
    
    if (!isInChord) {
        mCurVoiceState->vpos = tmp;
	    mCurVoiceState->curtp = getDuration();
    }

	return tmp;
}


//____________________________________________________________________________________
void ARMusicalVoice::adjustDuration(const TYPE_DURATION & newDuration)
{
	// rigth now no shortening is allowed
	TYPE_DURATION d = getDuration();
	assert(newDuration >= d);
	const TYPE_DURATION fill (newDuration - d);
	if (fill != DURATION_0)
	{
#if 0
		// this could be a workaround for the clef incorrect offset issue (#18)
		// but actually it significantly modifies the layout since hidden 1/8 notes could
		// be present everywhere (especially in multi-voices on a single staff context)
		// A correct solution should analyse the time layout across all voice and
		// insert small duration only at appropriate places, which doesn't solves the
		// problem for hand written scores.
		// But the ideal solution consists probably in score and rods settings of the clef
		// when linked to an empty element.
		//
		// issue can be tracked with GRClef::setHPosition
		//
		TYPE_DURATION remain = fill;
		while (remain != DURATION_0) {
			if (remain >= DURATION_8) {
				AddTail(new ARNote(DURATION_8));
				remain -= DURATION_8;
			}
			else {
				AddTail(new ARNote(remain));
				remain = DURATION_0;
			}
		}
#else
#ifndef WIN32
#warning ("TODO: fix empty elements issue wich clefs");
#endif
		AddTail(new ARNote(fill));
#endif
		// We have to look for the PositionTags here!
		if (mPosTagList)
		{
			GuidoPos pos = mPosTagList->GetHeadPosition();
			ARPositionTag * ptag;
			while (pos)
			{
				ptag = mPosTagList->GetNext(pos);
				if (( ARTagEnd::cast(ptag) == 0 ) && (ptag->getEndPosition() == 0 ))
					ptag->setEndPosition( GetTailPosition() );
			}
		}
	}
	setDuration ( newDuration );
	// if last event is a rest, it could be also made longer inside fill instead of insert a new rest?
}

//____________________________________________________________________________________
void ARMusicalVoice::browse(TimeUnwrap& mapper, ARMusicalVoiceState& state) const
{
	const TagList * ctags = state.getCurTags();
	const TagList * cstags= state.getCurStateTags();
	const PositionTagList * cptags = state.getCurPositionTags();

	GuidoPos pos = ctags ? ctags->GetHeadPosition() : 0;
	while (pos) {
		ARMusicalTag* tag = ctags->GetNext(pos);
		tag->browse(mapper);
	}
	pos = cstags ? cstags->GetHeadPosition() : 0;
	while (pos) {
		ARMusicalTag* tag = cstags->GetNext(pos);
		tag->browse(mapper);
	}
	pos = cptags ? cptags->GetHeadPosition() : 0;
	bool hastie = false;
	while (pos) {
		const ARPositionTag* ptag = cptags->GetNext(pos);
		const ARMusicalTag* tag = dynamic_cast<const ARMusicalTag*>(ptag);
		if (dynamic_cast<const ARTie*>(ptag)) hastie = true;
		if (tag) tag->browse(mapper);
	}
	if (!hastie) mapper.Event(0, TimeUnwrap::kNoTie);
}

//____________________________________________________________________________________
void ARMusicalVoice::browse(TimeUnwrap& mapper) const
{
	ARMusicalVoiceState state;
	GuidoPos pos = GetHeadPosition(state);
	browse(mapper, state);
	while(pos)
	{
		ARMusicalObject * e = GetNext(pos, state);
		e->browse(mapper);
		browse(mapper, state);
	}
}

//____________________________________________________________________________________
void ARMusicalVoice::browse(TimeUnwrap& mapper, const ARMusicalObject * start, const ARMusicalObject * end ) const
{
	ARMusicalVoiceState state;
	GuidoPos pos = GetHeadPosition(state);
	browse(mapper, state);
	while (pos) {
		ARMusicalObject * e = GetNext(pos, state);
		if (e == start) {
			e->browse(mapper);
			browse(mapper, state);
			break;
		}
	}
	while (pos) {
		ARMusicalObject * e = GetNext(pos, state);
		e->browse(mapper);
		browse(mapper, state);
		if (e == end) break;
	}
}

void ARMusicalVoice::printName(std::ostream& os) const
{
    os << "Musical voice";
}

void ARMusicalVoice::printParameters(std::ostream& os) const
{
    os << "voice number: " << getVoiceNum() << "; duration: " << (float) getDuration() << ";";
}

void ARMusicalVoice::accept(ARVisitor& visitor)
{
    visitor.visitIn (this);
	ARMusicalVoiceState state;

	_readmode oldreadmode = readmode;
	readmode = EVENTMODE;

	map<const ARMusicalTag*, bool> doneMap;
	GuidoPos pos = GetHeadPosition(state);
	while (pos) {
		ARMusicalObject *object = GetNext(pos, state);
        object->accept(visitor);
 		const PositionTagList* tags = state.currentPTags();
		if (tags) {
			GuidoPos tpos = tags->GetHeadPosition();
			while (tpos) {
				ARPositionTag* tag  = tags->GetNext (tpos);
				ARMusicalTag* mtag  = dynamic_cast<ARMusicalTag*>(tag);
				if (mtag && (doneMap.find(mtag)==doneMap.end())) {
					mtag->accept(visitor);
					doneMap[mtag] = true;
				}
			}
		}
	}
	readmode = oldreadmode;

    GuidoPos pTagPos = mPosTagList ? mPosTagList->GetHeadPosition() : 0;
    while (pTagPos) {
		ARMusicalTag * tag = dynamic_cast<ARMusicalTag*>(mPosTagList->GetNext(pTagPos));
        if (tag && (doneMap.find(tag) == doneMap.end())) tag->accept(visitor);
//		else cerr << "ARMusicalVoice::accept: pos tag is not a musical tag: ";
	}
    visitor.visitOut (this);
}

//____________________________________________________________________________________
/** \brief Does a lookahead from the current position in the data-stream

	Returns:
	0: no problems, just go ahead and start an autobeam at the
	    given Position with end at the Position indicated by the changed
	    pos-Parameter. Changes pos!
	1: a beam-Tag is starting within the range. Try starting an
	    autobeam at the Position indicated by the changed pos-Parameter.
		  Changes pos.
	2: the beamsOff-Tag occured. LookAhead tries to find the next
	    /beamsAuto-Tag and returns the Position where that is.
		  changes pos.
*/
int ARMusicalVoice::beamLookAhead(GuidoPos & pos,TYPE_DURATION & unit)
{
	assert(false);
	// not needed any longer ....
	ARMusicalObject * e = GetAt(pos);
	TYPE_TIMEPOSITION tp (e->getRelativeTimePosition());

	TYPE_TIMEPOSITION endtp (tp + unit);

	GuidoPos evpos = pos;
	TYPE_TIMEPOSITION evtp;

	ARMusicalTag * mybeam = 0;

	// This indicates a switch in BeamsOff and BeamsAuto
	int hasswitched = 0;
	do
	{
		//const type_info& ti=typeid(*e);
		if (mPosTagList)
		{
			GuidoPos ppos = mPosTagList->GetHeadPosition();
			ARBeam * myb;
			while (ppos)
			{
				myb = dynamic_cast<ARBeam *>(mPosTagList->GetNext(ppos));
				if (myb)
				{
					if (myb->getStartPosition() == pos)
						mybeam = myb;
					if (myb->getEndPosition() == pos)
					{
						mybeam = NULL;
						hasswitched = 2;
					}
				}
			}
		}

		if (typeid(*e) == typeid(ARBeamState))
			{
				ARBeamState *mystate = static_cast<ARBeamState *>(e->isARBeamState());
				if (mystate->getBeamState() == ARBeamState::OFF)
				{
					hasswitched = 1;
					beamstate = BEAMSOFF;
				}
				else if (mystate->getBeamState() == ARBeamState::AUTO)
					beamstate = BEAMSAUTO;
				else if (mystate->getBeamState() == ARBeamState::FULL)
					beamstate = BEAMSFULL;
			}

			evpos = pos;
			e = ObjectList::GetNext(pos);

			if (ARMusicalEvent::cast(e))
			{
				evtp = e->getRelativeEndTimePosition();
			}
			else
				evtp = DURATION_0;

	} while (pos && evtp < endtp);

	// If we get here, everything should be somewhat OK
	if (!hasswitched && mybeam == NULL && beamstate == BEAMSAUTO )
	{

		// thus, start of new Event is at or after end of AutoBeam-End
		// if (e->getRelativeTimePosition() >= endtp)
		// then should here nothing happen anymore ret 2??;
		//	evpos = prev_evpos;
		//else
		//  this is new, does it work?
		//	pos = prevpos;

		// test if timeposition ok
		TYPE_TIMEPOSITION tmptp;
		if (!pos) // are we at end of voice?
		{
			tmptp = getDuration(); // now I'm at end of voice
		}
		else
		{
			e = GetAt(evpos);
			tmptp = e->getRelativeEndTimePosition();
		}

		if (tmptp == endtp)
		{
			pos = evpos;
			return 0;
		}
	}

	// Ansonsten haben wir ein Problem ...
	// Entweder es ist gerade ein Beam aktiv, oder aber es gilt BeamsOff
	while (pos)
	{
		//const type_info& ti=typeid(*e);
		if (mPosTagList)
		{
			GuidoPos ppos = mPosTagList->GetHeadPosition();
			ARBeam * myb;
			while (ppos)
			{
				myb = dynamic_cast<ARBeam *>(mPosTagList->GetNext(ppos));
				if (myb)
				{
					if (myb->getStartPosition() == pos)
						mybeam = myb;
					if (myb->getEndPosition() == pos)
					{
						mybeam = NULL;
					}
				}
			}
		}

	    if (typeid(*e) == typeid(ARBeamState))
			{
				ARBeamState *mystate = static_cast<ARBeamState *>(e->isARBeamState());
				if (mystate->getBeamState() == ARBeamState::OFF)
				{
					beamstate = BEAMSOFF;
				}
				else if (mystate->getBeamState() == ARBeamState::AUTO)
					beamstate = BEAMSAUTO;
				else if (mystate->getBeamState() == ARBeamState::FULL)
					beamstate = BEAMSFULL;
			}

			e = GetAt(pos);

			// multipliziert mit dem Kehrwert
			TYPE_TIMEPOSITION tmptp (unit.getDenominator(), unit.getNumerator());
			tmptp *= e->getRelativeTimePosition();

			if (tmptp.getDenominator() == 1 && beamstate == BEAMSAUTO &&
				mybeam == NULL)
			{
				return 1;
			}

			e = ObjectList::GetNext(pos);
	}
	return 3;
}

//____________________________________________________________________________________
/** \brief report detun from alter tag to arnotes
*/
void ARMusicalVoice::doMicroTonal()
{
	setReadMode (EVENTMODE);
	ARMusicalVoiceState vst;
	GuidoPos pos = GetHeadPosition(vst);
	while (pos)
	{
		ARMusicalObject * o = GetNext(pos, vst);
        ARNote * note	= o ? o->isARNote() : nullptr;

		if (note) {
			float detune=0;
			const TagList * tags = vst.getCurStateTags();
			if (tags) {
				GuidoPos tagpos = tags->GetHeadPosition();
				while (tagpos) {
					ARAlter * alter = dynamic_cast<ARAlter *>(tags->GetNext(tagpos));
					if (alter) {
						detune += alter->getDetune();
						note->setAlter (alter);
					}
				}
			}
//			note->setDetune(detune);
		}
	}
	setReadMode (CHORDMODE);
}

//____________________________________________________________________________________
/** \brief collects octava with their associated staff
*/
void ARMusicalVoice::getOctava(int voice, std::map< int, std::vector<AROctava*> >& list)
{
//cerr << "ARMusicalVoice::getOctava " << voice << endl;
	setReadMode (EVENTMODE);
	
	int curstaff = voice;
	AROctava * last = nullptr;
	ARMusicalVoiceState vst;
	
	GuidoPos pos = GetHeadPosition(vst);
	while (pos) {
		ARMusicalObject * o = GetNext(pos, vst);
        AROctava * oct	= o->isAROctava();

		if (oct) {
			if (oct->getOctava()) {
				list[curstaff].push_back(oct);
				last = oct;
			}
			else if (last) {
				last->setEnd(oct->getRelativeTimePosition());
				last = nullptr;
			}
		}

        const ARStaff * staff	= static_cast<const ARStaff *>(o->isARStaff());
        if (staff) curstaff = staff->getStaffNumber();
	}
	setReadMode (CHORDMODE);
}

//____________________________________________________________________________________
/** \brief fix paired octava on different staff
*/
void ARMusicalVoice::doAutoFixOctavaStaff()
{
//	setReadMode (EVENTMODE);
	ARMusicalVoiceState vst;
	GuidoPos pos = GetHeadPosition(vst);
	GuidoPos lastpos = pos;
	int curStaffNum = getVoiceNum();
	int lastOctStaffNum = curStaffNum;
	while (pos)
	{
		lastpos = pos;
		ARMusicalObject * o = GetNext(pos, vst);
        const ARStaff * staff	= dynamic_cast<const ARStaff*>(o);
        if (staff) {
			curStaffNum = staff->getStaffNumber();
			continue;
		}
        const AROctava * octava	= dynamic_cast<const AROctava*>(o);
        if (octava) {
			if (octava->getOctava()) {
				lastOctStaffNum = curStaffNum;
			}
			else if (lastOctStaffNum != curStaffNum) {
cerr << "Warning! Auto fix octava on different staves: current: " << curStaffNum << " expected: " << lastOctStaffNum << endl;
				ARStaff * staff = new ARStaff (lastOctStaffNum);
				staff->setRelativeTimePosition(octava->getRelativeTimePosition());
				insert(lastpos,staff);
				ARStaff * restore = new ARStaff (curStaffNum);
				restore->setRelativeTimePosition(octava->getRelativeTimePosition());
				insert(pos,restore);
			}
        }
	}
//	setReadMode (CHORDMODE);
}

//____________________________________________________________________________________
/** \brief doAutoStuff1 does automatic Bar-Line-introduction
*/
void ARMusicalVoice::doAutoStuff1()
{
	timebench("doAutoDispatchLyrics", doAutoDispatchLyrics());

	// introduce barlines ...
	// this breaks notes if needed the new barlines are put right before
	// newSystem or newPage-Tags (if present) otherwise right before the event.
	timebench("doAutoBarlines", doAutoBarlines());

    // (jfk) moved 
    // introduce naturalkeys ...
    timebench("doAutoKeys", doAutoKeys());

	// introduce mesures numbering	
	timebench("doAutoMeasuresNumbering", doAutoMeasuresNumbering());
	timebench("doMicroTonal", doMicroTonal());
	timebench("doAutoFixOctavaStaff", doAutoFixOctavaStaff());
}

//____________________________________________________________________________________
/** \brief Called just after the intervoice-AutoStuff has been done.

	Call doAutoDisplayCheck to see, how events fit together in tuplets or are displayable.
	Then call doAutoBeaming to do AutoBeaming.
*/
void ARMusicalVoice::doAutoStuff2()
{
	
    // jfk, new, prototypical, correct the order of ARPossibleBreak and ARKey
    timebench("checkKeys", checkKeys());
    
	// this needs to be done just after the global voice-check has been done ...
	// now we can check whether newSystem and newPage are followed by correct clef/key information ...
	timebench("doAutoCheckStaffStateTags", doAutoCheckStaffStateTags());

	// this routine checks, wether the events can be displayed with single graphical
	// elements. If not, they are broken and tie-tags are included.
	timebench("doAutoDisplayCheck", doAutoDisplayCheck());

	// new function for AutoBeaming (this takes into account the new NormalForm)
	timebench("doAutoBeaming", doAutoBeaming());

	// now we do the TieThing ...
	// all ties are broken into ties that have just two elements.
	// this is \tie ( c c c ) becomes \tieBegin:1 c \tieBegin:2 c \tieEnd:1 c \tieEnd:2
	// this needs to be done, so that all Ties can be broken after the respective Event
	// (and that as well, if a break is introduced later).
	timebench("doAutoTies", doAutoTies());
	timebench("doAutoFeatheredBeam", doAutoFeatheredBeam());
	timebench("doAutoEndBar", doAutoEndBar());
	timebench("doAutoGlissando", doAutoGlissando());
	timebench("checkpbreak", checkpbreak());
}

//____________________________________________________________________________________
PositionTagList * ARMusicalVoice::createPositionTagList()
{
	return new PositionTagList(true);
}

//____________________________________________________________________________________
/** \brief Called from the factory to add a position tag
*/
void ARMusicalVoice::AddPositionTag(ARPositionTag *tag)
{
	if (!mPosTagList)
		mPosTagList = createPositionTagList();		// ownselement!

	// mStartPosTagList contains those PositionTags, that do not yet have a StartPosition.
	mStartPosTagList->AddTail(tag);
	ARMusicalObject *arobj = dynamic_cast<ARMusicalObject *>(tag);
	if (arobj)
		arobj->setRelativeTimePosition( getDuration());
	ARMusicalTag *armtg = dynamic_cast<ARMusicalTag *>(tag);
	if (armtg) {
		// Position-Tags are associated with the following event (rechtsassoziativ)
		armtg->setAssociation(ARMusicalTag::RA);
	}

    mPosTagList->AddTail(tag);

    if (!isInChord) {
        mCurVoiceState->ptagpos = mPosTagList->GetTailPosition();
        mCurVoiceState->AddPositionTag(tag);
    }
}


//____________________________________________________________________________________
/** \brief  Removes a Position-Tag from the list.

	Removed because it does not have any Range.
	The tag is added as a regular tag.
*/
void ARMusicalVoice::RemovePositionTag( ARPositionTag * ptag )
{
	mCurVoiceState->RemovePositionTag(ptag);
	if (mPosTagList)
	{
		// temporarily disown the elements ...
		const ARTagEnd *arte = dynamic_cast<const ARTagEnd *>(ptag);
		if (arte)	assert(false);
		else
		{
			mPosTagList->setOwnership(0);
			mPosTagList->RemoveElement(ptag);
			mPosTagList->setOwnership(1);
		}
	}
	// of course, remove it from the mStartPosTagList as well!!
	if (mStartPosTagList)
		mStartPosTagList->RemoveElement(ptag);
	mCurVoiceState->ptagpos = mPosTagList->GetTailPosition();
}


//____________________________________________________________________________________
/** \brief Sets the End-Position.
	Must be edited for the other begin-end-tags
*/
void ARMusicalVoice::setPositionTagEndPos(int id, ARMusicalTag * end, ARMusicalTag * start)
{
	if (!mPosTagList) return;

	GuidoPos pos		= mPosTagList->GetTailPosition();
	ARTagEnd * artgend	= dynamic_cast<ARTagEnd *>(end);
	assert(artgend);

	ARPositionTag * startTag;					// startTag is the start-Position-Tag!
	end->setAssociation(ARMusicalTag::LA);		// an End-Tag is alway Left-Associated...

	// the End-Tag has a Position (this is set to the current voice position)
	// I am not sure here, this may be some other tag and not the previous event!
	// therefore trying something new:
	// replacing: artgend->setPosition(GetTailPosition()); with
	if (lastevposition == NULL)
			end->setAssociation(ARMusicalTag::EL);	// no Event for this tag
	else	artgend->setPosition(lastevposition);

	int foundmatch = 0;
	while (pos)
	{
		startTag = mPosTagList->GetPrev(pos);
		if (startTag)
		{
			// new tags have to be added here.
			ARMusicalTag * mtag = dynamic_cast<ARMusicalTag *>(startTag);
			int tid = -1;
			if (mtag) tid = mtag->getID();

			ARDummyRangeEnd * dre = dynamic_cast<ARDummyRangeEnd *>(end);
			if (startTag->getCorrespondence() == NULL && tid == id && ((startTag == dynamic_cast<ARPositionTag *>(start)) || dre) )
			{
				int match = 1;
				if (dre)
				{
					// now find out, if we have a valid match ....
					if (!mtag->MatchEndTag(dre->endstr.c_str()))
						match = 0;
				}
				if (match)
				{
					if (startTag->getStartPosition() == NULL)
					{
						// this means: start position for the beginTag has not been set:
						// Setting an error -> Positions may overlap
						startTag->setStartPosition(GetTailPosition());

						// Error Left-association ...
						// Meaning, that now the Association is changed toward ER (error-right)
						ARMusicalTag * armtg = dynamic_cast<ARMusicalTag *>(startTag);
						if (armtg)
						{
							armtg->setAssociation(ARMusicalTag::ER);
							armtg->setError(1);
						}
						// Error-Left ... for the current tag
						end->setAssociation(ARMusicalTag::EL);
					}

					// this makes the correspondence between the begin and end  double-linked list  (is this necessary?)
					startTag->setCorrespondence(artgend);
					mtag->setAssociation(ARMusicalTag::RA);
					artgend->setCorrespondence(startTag);
					foundmatch = 1;
					break;
				}
			}
		}
	}

	if (!foundmatch) {
		// there has not been a match for this tag!  create a warning
		cerr << "Warning: " << end->getGMNName() << " does not match a begin-tag" << endl;
		// set an Error-Association (because it does not match properly)
		end->setAssociation(ARMusicalTag::EL);
	}
	// now we add the tag to the current-tag-list
	ARMusicalObject * arobj = dynamic_cast<ARMusicalObject *>(end);
	if (arobj)		  arobj->setRelativeTimePosition( getDuration() );
	mPosTagList->AddTail(artgend);
}

//____________________________________________________________________________________
GuidoPos ARMusicalVoice::getLastEventPosition()
{
	return lastevposition;
}

//____________________________________________________________________________________
void ARMusicalVoice::checkpbreak()
{
	ownselements = 0;
	GuidoPos pos = ObjectList::GetHeadPosition();
	AROctava* octava = 0;
	while (pos) {
		ARMusicalObject* obj = ObjectList::GetNext(pos);
		if (obj->isAROctava())
			octava = obj->isAROctava();
		if (octava) {		// there is  pending octava
			if (!octava->getOctava()) octava = 0;
			else {
				if (obj->isARNote() || obj->isARRest())
					octava = 0;			// there is a note before the possible break, nothing to reorder
				else if (obj->isARPossibleBreak()) {
					// here we need to reorder put the octava after the break;
					ObjectList::RemoveElement(octava);
					ObjectList::AddElementAt(pos, octava);
				}
			}
		}
	}
	ownselements = 1;
}

//____________________________________________________________________________________
/** \brief Converts the voice into Normal-Form

OK: Attention: What about Tags within Chords (I have not thought about this yet)
OK: Tags in chords are handled as in the regular case .... the chord is splitted
into "chordless" GUIDO and all GNF transformation work seamless on the result.

A Voice is in Normal-Form, if for every segment between events the following order is maintained:
ev_1 tg_1 tg_2 tg_3 ... tg_n ev_2
and forall 1 <= i <= j <= n and 1 <= k <= n:
    (1 <= k <= i) tg_k is LeftAssociated AND
    (i <= k <= j) tg_k is Not Associated AND
    (j <= k <= n) tg_k is Right Associated.
Naturally speaking this means that all left associated tags can be moved as close as possible
towards the left event; right associated tags  can be moved as close as possible towards the
right event; all other tags remain in the middle
(it is, of course, important, that their previous order is maintained).

The routine works as follows:

The following describes an algorithm that would be valid, if all Tags are maintained within
one list. However, right now, a positiontaglist is maintained, so this list needs to be manipulated now as well.

events are found. Two positions are maintained:
the position of the last LA-Tag (LLA) and the position of the first RA-Tag (FRA).
(these positions are NULL when an event is encountered. LLA and FRA are adjusted while sequentielly going
through the system.
when a LA-Tag is encountered it is appended to the LLA and LLA is adjusted.
when a NA-Tag is encountered, it is prepended to the FRA.
when an RA-Tag is encountered, nothing happens.
when an event is encountered LLA and FRA are set to zero.
ATTENTION: the algorithm has not been tested because the list is not in the described form
(position tags are maintained in the mPosTagList)

alternativ (and current algorithm works as follows) : the mPosTagList is sorted, so that
it can be iterated quicker in the following runs ...
sorting always is done between two events It can then be assured, that between ev1 and ev2
we have a structure like LA_1 LA_1 ... RA_2 RA_2

The algorithm works as follows:
remember current mPosTagList-Position (ptagpos)
stop traversing the mPosTagList, when either LA_ev2 or RA_ev? (that is RA != RA_ev2) is encountered.
*/
void ARMusicalVoice::ConvertToNormalForm()
{
	// now we do the "REAL" algorithm for the current working set of the mPosTagList....
	// this temporarily removes the ownership
	_readmode oldreadmode = readmode;
	readmode = EVENTMODE;

	if (mPosTagList) {
		mPosTagList->setOwnership(0);
		GuidoPos ptagpos = mPosTagList->GetHeadPosition();
		if (ptagpos) {
			GuidoPos pos = ObjectList::GetHeadPosition();
			GuidoPos prevpos = NULL;
			while (pos) {
				ARMusicalEvent * armev = ARMusicalEvent::cast(ObjectList::GetAt(pos));
				if (armev) {
					// now we have two eventpositions that is prevpos and pos
					// FRA stores the First Right Association
					GuidoPos FRA = NULL;
					GuidoPos ptagprevpos = NULL;

					while (ptagpos)	{
						ARPositionTag *ptag = mPosTagList->GetAt(ptagpos);
						ARMusicalTag  *armt = dynamic_cast<ARMusicalTag *>(ptag);
						if (armt)
						{
							if ( (armt->getAssociation() != ARMusicalTag::LA && armt->getAssociation() != ARMusicalTag::RA)
								|| ptag->getCorrespondence() == NULL)
							{
								// these position tags are removed .... maybe they should be saved somewhere ?
								// BUT: NormalForm is a SEMANTIC operation anyways!

								mPosTagList->setOwnership(1);
								mPosTagList->RemoveElementAt(ptagpos);
								mPosTagList->setOwnership(0);
								if (ptagprevpos == NULL)
									ptagpos = mPosTagList->GetHeadPosition();
								else
								{
									ptagpos = ptagprevpos;
									mPosTagList->GetNext(ptagpos);
								}
								continue;
							}
						}
						ARTagEnd * artgend = ARTagEnd::cast(ptag);
						if ((artgend == 0) && ptag->getPosition() != pos)
						{
							// we have a continuation ....
							break;
						}
						if (artgend && artgend->getPosition() != prevpos)
						{
							// continuation
							break;
						}

						if (artgend && FRA != NULL)
						{
							// shift the LeftAssociated ...
							assert(ptagprevpos != NULL);
							mPosTagList->RemoveElementAt(ptagpos);
							mPosTagList->AddElementAt(FRA,ptag);

							ptagpos = ptagprevpos;

						}
						if (!artgend && FRA == NULL)
							FRA = ptagpos;
						ptagprevpos = ptagpos;
						mPosTagList->GetNext(ptagpos);
					} // while ptagpos
					prevpos = pos;
				}
				ObjectList::GetNext(pos);
			}
		} // if (ptagpos)
		mPosTagList->setOwnership(1);
	}
	readmode = oldreadmode;
}

//____________________________________________________________________________________
//
// AUTO BEAMING
//____________________________________________________________________________________
// track existing beaming
// return the count of opened (>0) or closed (<0) beams
int	ARMusicalVoice::beamTrackBeam(const ARMusicalVoiceState& vst) const
{
	int beamcount = 0;
	if (vst.removedpositiontags)
	{
		GuidoPos tmppos = vst.removedpositiontags->GetHeadPosition();
		while (tmppos)
		{
			ARBeam * bm = dynamic_cast<ARBeam *>(vst.removedpositiontags->GetNext(tmppos));
			if (bm) --beamcount;
		}
	}

	// the beams that are added
	if (vst.addedpositiontags)
	{
		GuidoPos tmppos = vst.addedpositiontags->GetHeadPosition();
		while (tmppos)
		{
			ARBeam * bm = dynamic_cast<ARBeam *>(vst.addedpositiontags->GetNext(tmppos));
			if (bm) ++beamcount;
		}
	}
	return beamcount;
}

//____________________________________________________________________________________
// auto beaming: compute the new beat unit in case of meter change
TYPE_DURATION ARMusicalVoice::beamBeatUnit(const TYPE_DURATION curmeter) const //, const TYPE_DURATION curbeat) const
//TYPE_DURATION ARMusicalVoice::beamMeterChange(const ARMeter * curmeter, const TYPE_DURATION curbeat) const
{
	TYPE_DURATION beat(1, curmeter.getDenominator());  // default value for the beat
	if (curmeter.getDenominator() == 2 || curmeter.getDenominator() == 4)
	{
		beat.setNumerator(1);
		beat.setDenominator(4);
	}
	else if (curmeter.getDenominator() == 8)
	{
		if (! (curmeter.getNumerator() % 3) ) {
			beat.setNumerator(3);
			beat.setDenominator(8);
		}
		else {
			beat.setNumerator(2);
			beat.setDenominator(8);
		}
	}

	TYPE_DURATION tmp(curmeter.getNumerator(),curmeter.getDenominator());
	if (beat > tmp)
		beat = tmp;
	return beat;
}

//____________________________________________________________________________________
// check, wether the current position can be divided by the beat-structure.
// the current position must be an offset to the last (explicit) barline
bool ARMusicalVoice::beamStartPos(const TYPE_TIMEPOSITION pos, const TYPE_DURATION beat) const
{
	if (pos == DURATION_0) {
		// start of a measure: the position is valid according to lastbartp ...
		return true;
	}
	else if (beat != DURATION_0)
	{
		TYPE_DURATION subsubbeat(1, beat.getDenominator()*4);
		int denom = pos.getDenominator() * subsubbeat.getNumerator();
		TYPE_TIMEPOSITION divider(pos.getNumerator() * subsubbeat.getDenominator(), denom);
		divider.normalize();
		if (divider.getDenominator() == 1) return true;
	}
	return false;
}

//____________________________________________________________________________________
// check, wether the current event can be a beaming start position.
bool ARMusicalVoice::beamStartEv(const ARMusicalEvent* ev, const ARMusicalVoiceState& vst, const TYPE_DURATION beat, const TYPE_TIMEPOSITION lastbartp) const
{
	// check, wheter the displayduration is zero
	if (vst.fCurdispdur && (vst.fCurdispdur->getDisplayDuration() == DURATION_0))
		return false;
	if (ev->getDuration() >= DURATION_4)
		return false;
	// check, wether the ev position can be divided by the beat-structure.
	return beamStartPos(ev->getRelativeTimePosition() - lastbartp, beat);
}

//____________________________________________________________________________________
// create the actual beam
void ARMusicalVoice::beamCreate(ARMusicalVoiceState& vst, const GuidoPos posev1, const GuidoPos posevn, const GuidoPos FLA, const GuidoPos LRA_plus, const TYPE_TIMEPOSITION tpev1, bool full)
{
	// add the beam only, if the two positions are truly different, that is, posev1 != posevn.
	// otherwise, we get autobeams over one single event, which does not make much sense.
	if (posev1 != posevn) {
		// we could determine number of beams here ...
		// (go through voice from start to end keep track of displayduration ....)
		ARAutoBeam *arabeam = new ARAutoBeam(full);
		// this is obsolete, autobeam sets isauto anyways
		// arabeam->setIsAuto(true);
		arabeam->setStartPosition(posev1);
		arabeam->setRelativeTimePosition(tpev1);
		ARAutoBeamEnd * arabmend = new ARAutoBeamEnd();
		arabmend->setPosition(posevn);
		arabeam->setCorrespondence(arabmend);
		arabmend->setCorrespondence(arabeam);

		// Now add the generated position tags
		// to the mPosTagList at the correct positions ...
		// both need to be added before the  LRA_plus and FRA respectivly

		// if LRA_plus is NULL, then there has been no mPosTagList before ...
		if (LRA_plus == NULL)
			mPosTagList->AddTail(arabeam);
		else
			mPosTagList->AddElementAt(LRA_plus,arabeam);

		// if FLA is NULL, there has been no mPosTagList before.
		if (FLA == NULL)
			mPosTagList->AddTail(arabmend);
		else
			mPosTagList->AddElementAt(FLA,arabmend);
	}
}

//____________________________________________________________________________________
// check if a beam cross a beat
// the time position must be relative to the last bar
bool ARMusicalVoice::beamCrossBeat(const TYPE_TIMEPOSITION beamstart, const TYPE_DURATION beamdur, const TYPE_DURATION meter, const std::vector<Fraction>& meters) const
{
	TYPE_TIMEPOSITION beamend = beamstart + beamdur;		// the beam end date
	TYPE_TIMEPOSITION currentBeatPos (0, 4);				// the current beat position
	bool cross = false;
	if (meters.size() == 1) {								// simple meter spec: get the beat duration
		TYPE_DURATION beatdur = beamBeatUnit(meters[0]);
		if (beatdur.getNumerator() == 0) return true;
		while (currentBeatPos <= beamstart) {				// and find the beat following the beam start date
			currentBeatPos += beatdur;
		}
		cross = beamend > currentBeatPos;					// cross is true when the beam end date is greater than the next beat
	}
	else {													// compound meter spec (e.g. 3+2+2/9)
		for (std::vector<Fraction>::const_iterator i = meters.begin(); i != meters.end(); i++) {
			// get the beat duration for each part of meter spec
			TYPE_DURATION beatdur = beamBeatUnit(*i);
			if (beatdur.getNumerator() == 0) return true;
			if (currentBeatPos > beamstart) 				// and find the beat following the beam start date
				break;
			currentBeatPos += beatdur;
		}
		cross = beamend > currentBeatPos;					// cross is true when the beam end date is greater than the next beat
	}
	return cross;
}

//____________________________________________________________________________________
// check if a time position is  on a beat
// the time position should be relative to the last bar
//bool ARMusicalVoice::beamOnBeat(const TYPE_TIMEPOSITION t, const TYPE_DURATION meter, const std::vector<int>& numvec) const
bool ARMusicalVoice::beamOnBeat(const TYPE_TIMEPOSITION t, const TYPE_DURATION meter, const std::vector<Fraction>& meters) const
{
	bool onbeat = false;
	int beat;
	switch (meter.getDenominator()) {
		case 8:
		case 16:
		case 32:
			beat = meter.getNumerator()%3 ? 2 : 3;
			break;
		default:
			beat = 1;
			break;
	}
	TYPE_DURATION tmp (0, meter.getDenominator());
	if (meters.size() > 1) {
		std::vector<Fraction>::const_iterator i = meters.begin();
		while (!onbeat && (tmp < meter)) {
			onbeat = (t == tmp);
			if (i != meters.end()) {
				tmp += *i++;
			}
		}
	}
	else {
		while (!onbeat && (tmp.getNumerator() < meter.getNumerator())) {
			onbeat = (t == tmp);
			tmp.setNumerator(tmp.getNumerator() + beat);
		}
	}
	return onbeat;
}

//____________________________________________________________________________________
// gives the next event from the current position
ARMusicalEvent* ARMusicalVoice::beamNextEv (const GuidoPos pos, const ARMusicalVoiceState vst) const
{
	GuidoPos tmpPos = pos;
	ARMusicalVoiceState tmpvst = vst;
	ARMusicalEvent * nextev = 0;
	GetNext(tmpPos, tmpvst);						// get the next event
	if (tmpPos) {
		ARMusicalObject* o = GetAt(tmpPos);
		if (o) nextev = ARMusicalEvent::cast(o);
	}
	return nextev;
}

//____________________________________________________________________________________
// converts a meter to a beat duration
static TYPE_DURATION meter2beat (const Fraction& m)
{
	TYPE_DURATION out (1,4);
	int d = m.getDenominator();
	int beat = 1;
	switch (d) {
		case 8:
		case 16:
		case 32:
		case 64:
			beat = m.getNumerator()%3 ? 2 : 3;
			break;
	}
	return TYPE_DURATION(beat, d);
}

//____________________________________________________________________________________
// gives the beat at a given time position
// time position should be relative to the last bar
TYPE_DURATION ARMusicalVoice::beamGetBeat(const ARMeter * curmeter, const TYPE_TIMEPOSITION& pos, TYPE_DURATION curbeat) const
{
	const vector<Fraction>& meters = curmeter->getMeters();
	size_t n = meters.size();
	if (n > 1) {
		Fraction currentBeatPos = meters[0];
        for (size_t i=1; i<n ; i++) {
			if (pos >= currentBeatPos) {
				currentBeatPos += meters[i];
			}
			else return meter2beat(meters[i-1]);
		}
		return meter2beat(meters[n-1]);
	}
	else return curbeat;
}

#define TRACKBEAM 0
//____________________________________________________________________________________
/** \brief Perform autobeaming.

	Works as follows:
	the voice is traversed sequentielly the current meter is tracked
	the current BeamState is tracked the current displayDuration is tracked
	the occurence of explicit beam tags are recorded
	a beat-length is maintained (this is dependant on the meter)
	for possible starting events, the LRA (Last Right - Associate)
	position within the mPosTagList is remembered.
	for the ending event, the FRA (First RightAssociated) is found out.

	an autobeam is introduced, iff
	   1. no explicit beam is active (beamcount == 0)
	   2. beamstate is auto
	   3. duration/displayDuration of
	      ev_1 ... ev_n equals
	      beat-length and duration(ev_n) > 0
*/
void ARMusicalVoice::doAutoBeaming()
{
	// assume autobeaming as the default ....
	int bmauto = 1;
	ARBeamState * curbeamstate = NULL;
	// counts the explicit beams.
	int beamcount = 0;

	// the default beat-length. this depends on the curmeter ...
	TYPE_DURATION	beat(1L,4L);		// the current beat length
	ARMeter *		curmeter = NULL;	// the current meter
	TYPE_DURATION	meter (1, 4);		// the current meter duration
	vector<Fraction> meterDivision;		// the meter division
	meterDivision.push_back(Fraction(1,4));
	
	// for the mPosTagList ...
	// this is the Last RightAssociate plus 1 (that is the next....)
	GuidoPos LRA_plus = NULL;
	GuidoPos FLA = NULL;

	// for the voice ...
	GuidoPos posev1 = NULL;			// start beam position
	GuidoPos posevn = NULL;			// end beam position

	TYPE_TIMEPOSITION	beamStart = 0;
	bool				beamStartOnbeat;

	// the timeposition of the last explicit barline...
	// this needs to be inserted, so that the beat-structure can be matched according to this.
	TYPE_TIMEPOSITION lastbartp;
	ARMusicalVoiceState vst;

	// we should be in CHORDMODE for this ....
	assert(readmode == CHORDMODE);

	// this operation eats all but the first non-state events or tags.
	GuidoPos pos = GetHeadPosition(vst);

	while (pos)
	{
		bool tied = false;
		// track the beamstate ....
		if (curbeamstate != vst.curbeamstate)
		{
			int oldbmauto = bmauto;
			curbeamstate = vst.curbeamstate;
			if (curbeamstate) {
				if (curbeamstate->getBeamState() == ARBeamState::OFF)
					bmauto = 0;
				else if (curbeamstate->getBeamState() == ARBeamState::AUTO)
					bmauto = 1;
				else if (curbeamstate->getBeamState() == ARBeamState::FULL)
					bmauto = 2;
				else				// we shouldn't get there: default is beam auto
					bmauto = 1;
			}
			else bmauto = 1; // no beamstate equivalent to auto

			if (oldbmauto != bmauto)
				posev1 = posevn = NULL;
		}
		const PositionTagList * postags = vst.getCurPositionTags();
		if (postags) {
			GuidoPos ptags = postags->GetHeadPosition();
			while (ptags) {
				ARPositionTag * ptag = postags->GetNext(ptags);
				if (dynamic_cast<ARTie*>(ptag)) {
					tied = true;
#if TRACKBEAM
cerr << "==> ARMusicalVoice::doAutoBeaming TIE is true" << endl;
#endif
				}
			}
		}


		// track the meter change to compute the new beat
		if (curmeter != vst.curmeter) {
			// is not needed because a meter change ALWAYS creates a break
			// of the Auto-Beaming. TYPE_DURATION oldbeat = beat;
			curmeter = vst.curmeter;
			beat = beamBeatUnit (TYPE_DURATION(curmeter->getNumerator(), curmeter->getDenominator()));
//			beat = beamMeterChange (curmeter, beat);
			meter = TYPE_DURATION(curmeter->getNumerator(), curmeter->getDenominator());
			meterDivision = curmeter->getMeters();
			// a measure change is always the end of  autobeam (even if equal meter-sig!)
			posev1 = posevn = NULL;

		}

		// track the explicit beams: those that are added or removed
		beamcount += beamTrackBeam(vst);
		// if beamcount, then we have explicit beams ....
		if (beamcount>0)
			posev1 = posevn = NULL;

		ARMusicalObject * o = GetAt(pos);
		ARMusicalEvent * ev = ARMusicalEvent::cast(o);
        const ARBar * arbar = static_cast<const ARBar *>(o->isARBar());
		
		// is it an event? and there are NO explicit beams and the beamstate is auto?
		if (!beamcount && bmauto && ev && !vst.curgracetag)
		{
			// collect first the necessary information regarding the current event
			TYPE_TIMEPOSITION evDate	= ev->getRelativeTimePosition();		// get the event date
			TYPE_TIMEPOSITION evEnd		= evDate + ev->getDuration();			// get the event end date
			TYPE_TIMEPOSITION evBarDate	= evDate - lastbartp;					// the event date relative to the last bar
			// next determine the current beat value (used for specific measure divisions eg. 3+2+2/8)
			if (curmeter) beat = beamGetBeat (curmeter, evBarDate, beat);
			bool evOnbeat = beamOnBeat(evBarDate, meter, meterDivision);		// check if the note is on a beat

#if TRACKBEAM
cerr << "ARMusicalVoice::doAutoBeaming " << ev << endl; // ev->print(cerr);
#endif
			// check for possible Start-Position ...
			// do not start a beam for tied notes when they don't start on a beat
			if (posev1 == NULL && (!tied || evOnbeat))
			{
				if (beamStartEv(ev, vst, beat, lastbartp)) {
					beamStart = evDate;								// store the start date and check for on beat position
					beamStartOnbeat = evOnbeat;
#if TRACKBEAM
cerr << "ARMusicalVoice::doAutoBeaming beamStartEv 1 " << beamStart << endl;
#endif
					posev1 = pos;
					LRA_plus = vst.ptagpos;
				}
			}
			else if (posevn == NULL) {								// check for possible End-Position ...
				TYPE_TIMEPOSITION beamdur = evEnd - beamStart;		// get the beam duration
				TYPE_TIMEPOSITION beamend = evEnd - lastbartp;		// get the beam end position, relative to the last bar
				bool crossBeat = beamCrossBeat(beamStart - lastbartp, beamdur, meter, meterDivision);

				// we have a match when the end correspond to the meter, or when the beam duration is a beat, or when the end is on a beat
				// we don't have a matching position when the beam cross a beat
				if (!crossBeat && ((beamend == meter) || (beamdur == beat) || (beamOnBeat(beamend, meter, meterDivision))))  {
#if TRACKBEAM
cerr << "ARMusicalVoice::doAutoBeaming beam end 1 beamend / meter " << beamend << " " << meter << endl;
#endif
					posevn = pos;
					FLA = vst.ptagpos;
				}
				else {		// otherwise we look at the next event
					ARMusicalEvent * nextev = beamNextEv(pos, vst);
					// in case we have one and beam duration is still smaller than the current beat
					if ((beamdur < beat) && nextev) {
						TYPE_TIMEPOSITION nextDate	= nextev->getRelativeTimePosition();
						TYPE_TIMEPOSITION nextEnd (nextDate + nextev->getDuration());
						bool nextOnbeat = beamOnBeat(nextDate - lastbartp, meter, meterDivision);

						// we have a match when the next end is greater than the beat, or when the next is on a beat
						// a rest triggers the closing of the current beam
						if ( (!evOnbeat && ((nextEnd - beamStart) > beat)) || nextOnbeat || nextev->isARRest()) {
							posevn = pos;
							FLA = vst.ptagpos;
#if TRACKBEAM
cerr << "ARMusicalVoice::doAutoBeaming beam end 2 " <<  endl;
#endif
						}
					}
					// when no autobeam is possible, try to see if current event is a candiate for a new beginning?
					// start a new beam when the event is on a beat or when the beat duration is greater than the beat
					// do not start a beam for tied notes when they don't start on a beat
					if (!posevn && (evOnbeat || ((beamdur > beat) && beamStartOnbeat)) && (!tied || evOnbeat))
					{
						if (beamStartEv(ev, vst, beat, lastbartp)) {
#if TRACKBEAM
cerr << "ARMusicalVoice::doAutoBeaming beamStartEv 2 beat:" << beat << endl;
#endif
							beamStart = ev->getRelativeTimePosition();
							beamStartOnbeat = curmeter ? beamOnBeat(evBarDate, meter, meterDivision) : false;
							posev1 = pos;
							LRA_plus = vst.ptagpos;
						}
						else if (beamdur > beat) posev1 = 0;
					}
				}
			}
		}
		// or a Barline?
		// or a newSystem? -> ignored right now!
		else if (arbar)
		{
			// it is a barline ... -> then, we stop any possible beam ...
			posev1 = posevn = NULL;
			lastbartp = arbar->getRelativeTimePosition();
		}
		else if (vst.curgracetag)
			posev1 = posevn = NULL;

		// now check, wether a BEAM can be added!
		if (posev1 != NULL && posevn != NULL)
		{
			// yes, lets add ...
			if (mPosTagList == NULL)
			{
				// owns the elements ...
				mPosTagList = createPositionTagList();
				LRA_plus = FLA = mPosTagList->GetHeadPosition();
				vst.ptagpos = NULL;
			}
			beamCreate (vst, posev1, posevn, FLA, LRA_plus, beamStart, bmauto==2);
			posev1 = posevn = NULL;
		}
		GetNext(pos,vst);
	}
#if TRACKBEAM
	GMNCodePrintVisitor visitor(cerr);
	goThrough(&visitor);
	cerr << std::endl;
#endif
}

// ============================================================================
//		ARDispatchLyricsClass
// ============================================================================
class ARDispatchLyricsClass
{
public:

	ARLyrics * lyrics;
	NVstring * text;
	int   position;		// can be < 0
	ARDispatchLyricsClass()
	{
		lyrics = NULL;
		text = NULL;
		position = -1;
	}

	virtual ~ARDispatchLyricsClass()	{ delete text; }

	NVstring GetNextSubstring()
	{
		if (text == 0 )	return "";

		const int start = position;
		if (start >= (int)text->length())
			return "";

		int end = position;
		if (end >= 0)
		{
			while (end <= int(text->length()-1) )
			{
				char c = (*text)[end];
				if (c == ' ' || c == '-' || c == '_')
					break;
				++ end;
			}
		}
		size_t length = (size_t)(end-start);
		char c = '\0';
		if ((int)text->size() > end)	c = (*text)[end];
		if (c == '-' || c == '_')		length++;

		NVstring tmp = text->substr((size_t)start,length);
		tmp.replace('~',' ');
		position = end + 1;
		return tmp;
	}
};

typedef KF_IPointerList<ARDispatchLyricsClass> dlclist;


//____________________________________________________________________________________
/** \brief Converts lyrics tags into individual text-tags
	for all events within the range.

	The text-tags get the same parameter as the lyrics-tag
	goes through the voice and looks for
	lyrics tags  mayoverlap of course for different stances
*/
void ARMusicalVoice::doAutoDispatchLyrics()
{
	ARMusicalVoiceState vst;

	// this operation eats all but the first non-state events or tags.
	GuidoPos pos = GetHeadPosition(vst);

	dlclist mydlclist(1);
	while (pos)
	{
		// track lyrics-tags
	 	if (vst.addedpositiontags)
		{
			GuidoPos tmppos = vst.addedpositiontags->GetHeadPosition();
			while (tmppos) {
				ARPositionTag * arpt = vst.addedpositiontags->GetNext(tmppos);
				ARLyrics * arlyrics = dynamic_cast<ARLyrics *>(arpt);
				if (arlyrics) {
					// then we have a new lyrics-tag opening ....
					ARDispatchLyricsClass * dlc = new ARDispatchLyricsClass;
					dlc->lyrics = arlyrics;
					dlc->text = new NVstring(arlyrics->getText());
					dlc->position = 0;
					mydlclist.AddTail(dlc);
				}
			}
		}

		// those that are removed
		if (vst.removedpositiontags)
		{
			GuidoPos tmppos = vst.removedpositiontags->GetHeadPosition();
			while (tmppos)
			{
				ARPositionTag * arpt = vst.removedpositiontags->GetNext(tmppos);
				ARLyrics * arlyrics = dynamic_cast<ARLyrics *>(arpt);
				if (arlyrics) {
					ARDispatchLyricsClass * dlc = NULL;
					GuidoPos mytmppos = mydlclist.GetHeadPosition();
					while (mytmppos) {
						ARDispatchLyricsClass * tmpdlc = mydlclist.GetNext(mytmppos);
						if (tmpdlc && tmpdlc->lyrics == arlyrics) {
							dlc = tmpdlc;
							break;
						}
					}
					if (dlc) mydlclist.RemoveElement(dlc);
				}
			}
		}

		ARMusicalObject * o = GetAt(pos);
		ARMusicalEvent * ev = ARMusicalEvent::cast(o);
		if (ev && mydlclist.GetCount()>0)
		{
			// then we have to dispatch the lyrics to the event ....
			GuidoPos mytmppos = mydlclist.GetHeadPosition();
			while (mytmppos)
			{
				ARDispatchLyricsClass * dlc = mydlclist.GetNext(mytmppos);
				if (dlc)
				{
					NVstring mysubstr = dlc->GetNextSubstring();
					// now we have to add the text-tag to the event
					// keeping everything under control
					// (the curpositiontags and all that)
					if (mysubstr.length()>0)
					{
						// then we actually have something to out under an event ....
						ARText * artext = new ARText(mysubstr,0, true);
						artext->setAutoPos (dlc->lyrics->autoPos());
						artext->setRange(1);
						artext->setPosition(vst.vpos);
						artext->setRelativeTimePosition (ev->getRelativeTimePosition());
						artext->setVoiceNum(ev->getVoiceNum());

						artext->copyLyricsParams(dlc->lyrics);
						mPosTagList->AddElementAt(vst.ptagpos,artext);
						ARDummyRangeEnd *dum = new ARDummyRangeEnd(")");
						dum->setPosition(vst.vpos);
						artext->setCorrespondence(dum);
						dum->setCorrespondence(artext);
						mPosTagList->AddElementAt(vst.ptagpos,dum);
					}
				}
			}
		}
		GetNext(pos,vst);
	}

	mydlclist.RemoveAll();
}

//____________________________________________________________________________________
/** \brief Does a GNF transformation

(from GNF to GNF) introducing Bar-lines dependant on the Meter-Information (if present)

the algorithm works as follows: the current meter is tracked.
whenever a meter change occurs, the current measuretime is reset.
explicit and implicit barlines also reset the measuretime

there are two cases: either events end at the specified meter-time (1) or they overlapp (2).

case 1 can be handled by doing a "lookahead" and finding the next event with duration>0;
if no explicit barline has been found while "looking ahead", a new barline is introduced
right before the found event OR, if a newSystem or newPage is found, right before that!
if a barline has been found, nothing is done.
in any case, the measuretime is reset.
continue with the found event.

case 2 is a little bit more complicated:  case 2 occurs, when measuretime>currentmetertime
we then have to split the current event (ev) into two events ev_1 and ev_2. The lengths are
distributed as follows:
duration(ev_1) = currentmetertime - lastmeasurtime (lastmeasuretime is the accumlated durations of
all previous events in this measure)
duration(ev_2) = duration(ev) - duration(ev_1) a new barline is introduced between ev_1 and ev_2
IMPORTANT: all LA-Tags pointing to ev have to be "repointered", so that they now point towards
ev_2.
A tie-tag (autotie) or a merge-tag starting at ev_1 and ending at ev_2 has to be added, but only
if no other tie/merge-tag is active at the time. then the measuretime is reset and we continue
with looking at ev_2.
*/
void ARMusicalVoice::doAutoBarlines()
{
	// does the user want autobarlines (the default)?
	if (!gGlobalSettings.gAutoBarlines) return;

	// not needed ! Because, autoTies are inserted anywhere, otherwise it just would not make
	// sense. doAutoTies later checks beginnings and endings of ties ..

	// counts the ties/merge-tags
	// int tiemergecount = 0;

	// holds the current meter information for a voice. if curmeter is NULL, no meter has been set.
	// ATTENTION can a meter be turned off?
	ARMeter * curmeter = NULL;
	TYPE_DURATION curmeterlen;

	bool disableAutoBar = true;
	bool lookahead    = false;
	bool foundbarline = false;

	// this is the position of a newSystem or newPage-Tag that was encountered while looking ahead.
	GuidoPos newSystemOrPagepos = NULL;

	// the current evolved time in a measure
	TYPE_DURATION measuretime;
	TYPE_DURATION lastmeasuretime;
	TYPE_TIMEPOSITION lastbartp;
	TYPE_TIMEPOSITION lastnewsyspagep;
	TYPE_TIMEPOSITION lastkeyp;

	ARMusicalVoiceState vst;

	// this operation eats all but the first non-state events or tags.
	GuidoPos pos = GetHeadPosition(vst);
    GuidoPos previousKey = NULL;
	while (pos)
	{
		if (curmeter != vst.curmeter)		// track the meter
		{
			curmeter = vst.curmeter;
			curmeterlen = curmeter->getMeterDuration();
			disableAutoBar = (curmeterlen.getNumerator() == 0) || (curmeter->getAutoBarlines() == 0);
			measuretime = DURATION_0;
		}

		ARMusicalObject * obj = GetAt(pos);
		ARMusicalEvent * ev = ARMusicalEvent::cast(obj);
        const ARBar   *bar   = static_cast<const ARBar *>(obj->isARBar());

		// (DF) tests to inhibit auto barlines when a repeat bar is present
		bool isRepeatBar = false;
        TYPE_TIMEPOSITION currentDate;
        if (obj) {
			currentDate = obj->getRelativeTimePosition();
			if (obj->isARRepeatBegin())
				foundbarline = isRepeatBar = true;
		}
        if( obj->isARKey() ) {
			previousKey = pos;
			lastkeyp = currentDate;
		}

		if (lookahead) {
			// we are in lookahead-mode, that is, we have found a measure end already ...
			// we need to track explicit bars until we have an event with duration>0
            ARNewPage   *page = static_cast<ARNewPage *>(obj->isARNewPage());
			ARNewSystem *sys  = static_cast<ARNewSystem *>(obj->isARNewSystem());

			if (bar || isRepeatBar)
				foundbarline = true;
			else if (page || sys) {
				if (newSystemOrPagepos == NULL) {
					newSystemOrPagepos = pos;
					lastnewsyspagep = currentDate;
				}
			}
			else {
				bool insertbar = obj->isARTempo() || obj->isARGrace() || obj->isARCoda() || obj->isARSegno() || obj->isARNote() || (ev && (ev->getDuration() > DURATION_0));
				if (insertbar) {
					// now, we are at the end of the lookahead ...
					if (!foundbarline) {
						// insert a barline right before the current event
						lastbartp = currentDate;
						ARBar *arbar = new ARBar(lastbartp);
						arbar->setIsAuto( true );

						GuidoPos barPos = pos;
						if (newSystemOrPagepos && (lastnewsyspagep == currentDate))
							barPos = newSystemOrPagepos;
						// the barline has to go before a potential key tag at this time position!
						else if( previousKey && (lastkeyp == currentDate ))
							barPos = previousKey;
						AddElementAt(barPos,arbar);
					}
					foundbarline = false;
					lookahead = false;
					newSystemOrPagepos = NULL;
					measuretime = DURATION_0;
				}
			}
		}

		lastmeasuretime = measuretime;
		if (!disableAutoBar && !lookahead && ev)
		{
			measuretime += ev->getDuration();
			if (measuretime == curmeterlen)
				lookahead = true;		// this is case 1: we need to switch to look-ahead mode.

			else if (measuretime > curmeterlen)
			{
				// now we are in case 2: the event spans over the "new" barline ...

				// this is the position within the positiontags where a tie/merge-tag can be inserted.
				// it lies just after the current event or is NULL (no mPosTagList, end of mPosTagList)

				// now calculate the durations ...
				// we are in CHORDMODE ....
				TYPE_DURATION olddur = ev->getDuration();
				TYPE_DURATION newdur = curmeterlen - lastmeasuretime;

				// this sets the duration to the shorter length ...
				if (vst.curchordtag)
				{
					// then, we have to set the duration also of the dispDur tag that must be present somewhere ....
					if (vst.fCurdispdur)
					{
						// how can we make sure, that the new duration is displayable?
						// must be checked in doAutoDisplayCheck!
						vst.fCurdispdur->setDisplayDuration(newdur);

					}
					// we have to set the timepositions of the other events and tags within the chord-range ....
					TYPE_TIMEPOSITION mytp (ev->getRelativeTimePosition() + newdur);
					GuidoPos startpos = vst.curchordtag->getPosition();
					while (startpos)
					{
						GuidoPos tmppos = startpos;
						ARMusicalObject * obj = ObjectList::GetNext(startpos);

						if (obj && obj->getDuration() == DURATION_0)
							obj->setRelativeTimePosition(mytp);
						if (tmppos == vst.curchordtag->getEndPosition())
							break;
					}

				}
				ev->setDuration(newdur);

				// that is the new duration of the event.
				lastbartp = ev->getRelativeTimePosition() + newdur;
				ARBar * arbar = new ARBar(lastbartp);
				arbar->setIsAuto(true);

				// this adds the element after the event.
				GuidoPos newpos;
				if (vst.curchordtag)
				{
					// if we have a chord(tag), we need to copy the whole chord (including tags ....)
					// how do we do that?
					// copychord returns the position of the first event in the new (copied) chord.
					newpos = CopyChord(vst, lastbartp, olddur - newdur);

					AddElementAt(newpos,arbar);
					// in chordmode ... this should
					// get me to the bar and then to the first event
					// after the bar, which is the chord ....
					GetNext(vst.vpos,vst);
					GetNext(vst.vpos,vst);
					pos = vst.vpos;
				}
				else
				{
					// this adds the new bar right after the current event.
					newpos = AddElementAfter(pos,arbar);

					// now we create a new event, that is a copy of ev and has a new duration ...
					ARNote* note = ev->isARNote();
					ARMusicalEvent *ev2 = note ? note->Clone(true) : static_cast<ARMusicalEvent *>(ev->Copy());
					if (ev2) {
						ev2->setRelativeTimePosition( lastbartp );
						ev2->setDuration( olddur - newdur );
						newpos = AddElementAfter( newpos, ev2 );
					}
					else assert(false);

					// this position remembers the location of the MergeEnd or
					// tieEnd that could be introduced. This is important, so that
					// tie repositioning works ...
					GuidoPos newptagpos = NULL;
					ARNote *nt = ev2->isARNote();
//					if (!nt || !nt->isEmptyNote() || !nt->isARTab()) {
					if (nt && !nt->isEmptyNote() && !nt->isARTab()) {
						// new: just insert a tie anyways ....
						if (mPosTagList == NULL) {
							mPosTagList = createPositionTagList();
							vst.ptagpos = mPosTagList->GetHeadPosition();
						}

						// so we add our tie ...
						ARTie *existingTie = dynamic_cast<ARTie *>(mPosTagList->GetHead());
						ARTie *artie = existingTie ? new ARTie(*existingTie) : new ARTie;
						artie->setID((int)gCurArMusic->mMaxTagId ++);
						artie->setIsAuto(true);
						artie->setRelativeTimePosition(ev->getRelativeTimePosition());
						artie->setPosition(pos);

						ARDummyRangeEnd *arde = new ARDummyRangeEnd(TIEEND);
						arde->setID(artie->getID());
						arde->setPosition(newpos);
						artie->setCorrespondence(arde);
						arde->setCorrespondence(artie);

						if (vst.ptagpos != NULL) {
							// this is OK ....
							mPosTagList->AddElementAt(vst.ptagpos,artie);

							// the position of this element must be saved!
							newptagpos = mPosTagList->AddElementAt(vst.ptagpos,arde);
						}
						else {
							mPosTagList->AddTail(artie);

							// remember this position!
							newptagpos = mPosTagList->AddTail(arde);
						}

						// add the tag to the currentposition-tags, but don't add it to the
						// addedtaglist ... this would otherwise confuse the beginning of this function.
						vst.AddPositionTag(artie,0);
					}

					// now we need to "repointer" the position tags that formerly
					// were attached to ev -> they
					// now need to be attached to ev2 (which is at newpos)
					GuidoPos tmppos = vst.ptagpos;
					while (tmppos) {
						ARPositionTag * arpt = mPosTagList->GetNext(tmppos);
						ARTagEnd * arte = ARTagEnd::cast(arpt);
						if (arte) {
							if (arte->getPosition() == pos) {
								arte->setPosition(newpos);
								continue;
							}
						}
						break;
					}

					// the positiontagpointer now points on the FLA of ev_2 ...
					// (which is the added tie or merge)
					if (newptagpos != NULL)
						vst.ptagpos = newptagpos;

					// now, the added and removedposition tags are removed ...
					// cannot be of interest ....
					vst.DeleteAddedAndRemovedPTags();
					pos = newpos;
				}
				measuretime = DURATION_0;
				continue;
			}
		}

		if (!lookahead && bar) {
			measuretime = DURATION_0;
			lastbartp = bar->getRelativeTimePosition();
		}
		GetNext(pos,vst);
	}
}

//____________________________________________________________________________________
/** \brief Does a GNF transformation

(from GNF to GNF) introducing measures numbering dependant on the Meter-Information (if present)
*/
void ARMusicalVoice::doAutoMeasuresNumbering()
{
	ARMeter * curmeter = NULL;
	TYPE_DURATION curmetertime;
	int measureNumber = 2;
	int currAutoMeasureNum = ARMeter::kNoAutoMeasureNum;

	ARMusicalVoiceState vst;
	GuidoPos pos = GetHeadPosition(vst);
	ARBar * previous = 0;
	const ARMMRest* mrest = 0;
	
	while (pos)
	{
		if (curmeter != vst.curmeter)
		{
			curmeter = vst.curmeter;

			curmetertime.setNumerator(curmeter->getNumerator());
			curmetertime.setDenominator(curmeter->getDenominator());
			curmetertime.normalize();
			currAutoMeasureNum = curmeter->getAutoMeasuresNum();
		}

		ARMusicalObject * o = GetAt(pos);
        ARBar * bar = dynamic_cast<ARBar *>(o);
		const PositionTagList * ptags = vst.getCurPositionTags();
		if (ptags) {
			GuidoPos ppos = ptags->GetHeadPosition();
			while (ppos) {
				const ARMMRest* tmp = dynamic_cast<const ARMMRest *>(ptags->GetNext(ppos));
				if (tmp) mrest = tmp;
			}
		}
		
		if (bar)
		{
			if (mrest) {
				measureNumber += mrest->getMeasuresCount()-1;
				mrest = 0;
			}
            if (bar->isMeasureNumSkipped())
                measureNumber--;
			if (bar->getMeasureNumber())	// measure num is already set at tag level
				measureNumber = bar->getMeasureNumber();
			else bar->setMeasureNumber(measureNumber);
			bar->setPreviousBar (previous);

			if (!bar->isMeasureNumberDisplayedSet()) {
				switch (currAutoMeasureNum) {
					case ARMeter::kAutoMeasureNum:
						bar->setMeasureNumberDisplayed(ARBar::kNumAll);
						break;
					case ARMeter::kAutoMeasureNumPage:
						bar->setMeasureNumberDisplayed(ARBar::kNumPage);
						break;
                    case ARMeter::kAutoMeasureNumSystem:
                        bar->setMeasureNumberDisplayed(ARBar::kNumSystem);
                        break;
					default:
						bar->setMeasureNumberDisplayed(ARBar::kNoNum);
				}
			}
            else {
                if (bar->getMeasureNumberDisplayed()) {
                    if (currAutoMeasureNum == ARMeter::kAutoMeasureNumPage)
                        bar->setMeasureNumberDisplayed(ARBar::kNumPage);
                    else if (currAutoMeasureNum == ARMeter::kAutoMeasureNumSystem)
                        bar->setMeasureNumberDisplayed(ARBar::kNumSystem);
                }
            }
            
            measureNumber++;
			previous = bar;
		}
		GetNext(pos,vst);
	}
}

class PointerClass
{
public:
	ARMusicalObject * pobj;
	GuidoPos		 pos1;
	GuidoPos		 pos2;
};


//____________________________________________________________________________________
/** Works as follows:

	vst is pointing to an event that is actually the "base" note of a chord.

	\\chord( \\dispDur<...> \\shareStem(  empty  ev1*0 ev2 ... ) ) )
	so vst is pointing on the empty event we must traverse the voice from vst and
	copy everything that we encounter (tags, events ...)
	Need to repointer ptags and also set state information at the first event in new chord.
	Also need to add tie-information.
*/
GuidoPos ARMusicalVoice::CopyChord(ARMusicalVoiceState &vst, TYPE_TIMEPOSITION tp, const TYPE_DURATION & newdur)
{
	ARStaff * curstaff = static_cast<ARStaff *>( vst.getCurStateTag(typeid(ARStaff)));

	int staffstatenum = 0;
	if (!curstaff) {
		// then what?
		// we have to remember the voice-num, because the voice is associated with the staff if not set otherwise.
		staffstatenum = getVoiceNum();
	}
	else staffstatenum = curstaff->getStaffNumber();

	TYPE_TIMEPOSITION oldtp = vst.curtp;
	TYPE_TIMEPOSITION newtp;
	ARMusicalObject * obj = ObjectList::GetAt(vst.vpos);
	assert(obj);
	newtp = oldtp + obj->getDuration();

	typedef KF_IPointerList<PointerClass> ptclass;

	GuidoPos posfirstinlastchord = vst.vpos;
	GuidoPos poslastinfirstchord = NULL;

	// now we copy the chord
	ARMusicalVoiceState * mystate = new ARMusicalVoiceState(vst);
	ARMusicalVoiceState & myvst = *mystate;

	_readmode oldreadmode = readmode;
	readmode = EVENTMODE;

	ptclass * mylist = new ptclass(1);
	ARChordTag *savedchordtag = myvst.curchordtag;
	GuidoPos prevpos = NULL;

	// this puts all objects within the chordtag-range
	// in a list.
	while( myvst.vpos && myvst.curchordtag == savedchordtag) {
		prevpos = myvst.vpos;
		ARMusicalObject * tmp = GetNext(myvst.vpos,myvst);
		PointerClass * pc = new PointerClass();
		pc->pobj = tmp;
		pc->pos1 = prevpos;
		mylist->AddTail( pc );
	}
	// then we have everything (besides the positiontags ...)

	// this is the position of the last element in the first chord.
	poslastinfirstchord = prevpos;

	// now we have to check, if the voice-state has changed
	ARStaff * newstafftag = NULL;
	ARStaff * newstaff = static_cast<ARStaff *>( myvst.getCurStateTag(typeid(ARStaff)));
	if (curstaff != newstaff) {
		if ((curstaff && curstaff->getStaffNumber() != newstaff->getStaffNumber()) || ( staffstatenum != newstaff->getStaffNumber() ) ) {
			// then we need a new staff-tag
			newstafftag = new ARStaff(staffstatenum);
			newstafftag->setRelativeTimePosition(newtp + newdur);
		}
	}

	GuidoPos posfirstinnewchord = NULL;
	GuidoPos poslastinnewchord = NULL;
	GuidoPos objpos = mylist->GetHeadPosition();
	int firstafterempty = 0;

	// this loop actually created copies of the
	// events and tags within the chord-range.
	while (objpos) {
		PointerClass * pc = mylist->GetNext(objpos);
		ARMusicalObject * obj = pc->pobj;
		ARNote * note = obj->isARNote();
//		ARMusicalObject * newobj = note ? new ARNote (*note, true) : obj->Copy();
		ARMusicalObject * newobj = note ? note->Clone (true) : obj->Copy();
		newobj->setRelativeTimePosition(tp);
		if (newobj->getDuration() > DURATION_0) {
			newobj->setDuration(newdur);
			tp += newdur;
			firstafterempty = 1;
		}

		if (myvst.vpos)
		  	poslastinnewchord = AddElementAt(myvst.vpos,newobj);
		else
			poslastinnewchord = InsertAtTail(newobj);

		if (!posfirstinnewchord)
			posfirstinnewchord = poslastinnewchord;

		pc->pos2 = poslastinnewchord;
		if (firstafterempty && newstafftag) {
			if (myvst.vpos)	AddElementAt(myvst.vpos,newstafftag);
			else			InsertAtTail(newstafftag);
			firstafterempty = 0;
			newstafftag = NULL;
		}
	}

	typedef KF_IPointerList<ARPositionTag> typeptaglist;
	typeptaglist * myptaglist = new typeptaglist();

	// about the repointering aspect ....
	// I know, that myvst.ptagpos points to a position-tag that is either ending on an event within the range
	// or points to the beginning of the ending tags of the first chord-range.

	// first, I have to copy all the tags that are in vst.curpositiontags and NOT in myvst.curpositiontags ....
	if (vst.curpositiontags) {
		GuidoPos tmpcurpos = vst.curpositiontags->GetHeadPosition();
		while (tmpcurpos) {
			ARPositionTag * ptag = vst.curpositiontags->GetNext(tmpcurpos);
			if (!myvst.curpositiontags->GetElementPos(ptag)) {
				// then we have a positiontag that is completly
				// embedded within the chord.
				// this needs to be copied.

				GuidoPos newstartpos = NULL;
				GuidoPos mypos = mylist->GetHeadPosition();
				while (mypos) {
					PointerClass * pc = mylist->GetNext(mypos);
					if ( ptag->getPosition() == pc->pos1 ) {
						newstartpos = pc->pos2;
						break;
					}
				}

				if (newstartpos) {
					// OK, now we have to copy the ptags and repointer them ....
					// we need other object-functions to handle this !!!!!
					ARMusicalObject * pstart = ptag->Copy();
					if (pstart)
					{
						ARPositionTag * newpstart = dynamic_cast<ARPositionTag *>(pstart);
						assert(newpstart);
						newpstart->setPosition(newstartpos);
						ARMusicalTag * newmtag =  dynamic_cast<ARMusicalTag *>(newpstart);

						TYPE_TIMEPOSITION tagtp (tp);
						if (newstartpos == posfirstinnewchord)
							tagtp = tp - newdur;
						newmtag->setRelativeTimePosition(tagtp);

						// this is a HACK: I save the OLD correspondence
						// to find the correct correspondence when copying the end-tags!
						newpstart->setCorrespondence(ptag->getCorrespondence());
						myptaglist->AddTail(newpstart);
					}
				}
			}
		}
	}

	myvst.ptagpos = vst.ptagpos;
	int found = 0;
	GuidoPos prevptagpos = NULL;
	while ( myvst.ptagpos )
	{
		prevptagpos = myvst.ptagpos;
		ARPositionTag * ptag = mPosTagList->GetNext(myvst.ptagpos);
		ARTagEnd * tgend = ARTagEnd::cast(ptag);
		if (tgend && tgend->getPosition() == poslastinfirstchord) {
			// this means, we are at the end ....
			found = 1;
		}
		else if (tgend && (tgend->getPosition() != poslastinfirstchord) && found) {
			// we have an endtag, but it does not point to
			// the last element in the first chord.
			myvst.ptagpos = prevptagpos;
			break;
		}
		else if (!tgend && found) {
			// we have a beginning tag and have previously
			// found the end of the chord-range.
			myvst.ptagpos = prevptagpos;
			break;
		}

		if (ptag)
		{
			 ARMusicalTag * mtag = dynamic_cast<ARMusicalTag *>(ptag);
			if (dynamic_cast<ARTie *>(ptag)) {
				// then we have tie-tag that begins at an event within the chord. I have
				// to make sure, that this is moved to the copied events of the new chord ....
				GuidoPos newstartpos = NULL;
				GuidoPos mypos = mylist->GetHeadPosition();
				while (mypos) {
					PointerClass * pc = mylist->GetNext(mypos);
					if ( ptag->getPosition() == pc->pos1 ) {
						newstartpos = pc->pos2;
						break;
					}
				}
				if (newstartpos) {
					// now remove the ptag from its position and
					// out it where it belongs in the mPosTagList ....
					ptag->setPosition(newstartpos);
					mPosTagList->setOwnership(0);
					if (vst.ptagpos == prevptagpos)
						vst.ptagpos = myvst.ptagpos;

					mPosTagList->RemoveElementAt(prevptagpos);
					mPosTagList->setOwnership(1);
					myptaglist->AddTail(ptag);

				}
			}
			else {
			  // we need to reset the times of the tags that lie inbetween
			  if (mtag->getRelativeTimePosition() > newtp)
				mtag->setRelativeTimePosition(newtp);
			}

			// this copies everything, that is not an END-tag
			// and that does not last over the end of the chord-range ....
			if (!tgend && myvst.curpositiontags && !myvst.curpositiontags->GetElementPos(ptag)) {
				// then we have a positiontag that is completly embedded within the chord.
				// this needs to be copied.
				GuidoPos newstartpos = NULL;
				//GuidoPos newendpos = NULL;

				GuidoPos mypos = mylist->GetHeadPosition();
				while (mypos)
				{
					PointerClass * pc = mylist->GetNext(mypos);
					if ( ptag->getPosition() == pc->pos1 ) {
						newstartpos = pc->pos2;
						break;
					}
				}

				if (newstartpos) {
					// OK, now we have to copy the ptags and repointer them ....
					// we need other object-functions to handle this !!!!!
					ARMusicalObject * pstart = ptag->Copy();
					ARPositionTag * newpstart = dynamic_cast<ARPositionTag *>(pstart);
					if (newpstart) {
						newpstart->setPosition(newstartpos);
						ARMusicalTag * newmtag = dynamic_cast<ARMusicalTag *>(newpstart);

						TYPE_TIMEPOSITION tagtp = tp;
						if (newstartpos == posfirstinnewchord)
							tagtp = tp - newdur;

						newmtag->setRelativeTimePosition(tagtp);
						newpstart->setCorrespondence(ptag->getCorrespondence());
						myptaglist->AddTail(newpstart);
					}
				}
			}
		}

		// then we are "clean", we have either already found the end or are inbetween ...
		if (tgend) {
			ARPositionTag * myptag = tgend->getCorrespondence();
			if (myptag) {
				// check whether myptag is in the vst.curpositiontaglist.
				if (myvst.curpositiontags && !myvst.curpositiontags->GetElementPos(myptag)) {
					// then this is the end-position, that needs to be copied as well ....
					GuidoPos newstartpos = NULL;
					GuidoPos newendpos = NULL;

					GuidoPos mypos = mylist->GetHeadPosition();
					while (mypos) {
						PointerClass * pc = mylist->GetNext(mypos);
						if ( myptag->getPosition() == pc->pos1 ) 
							newstartpos = pc->pos2;

						if ( tgend->getPosition() == pc->pos1 ) {
							newendpos = pc->pos2;
							break;
						}
					}

					if (newstartpos && newendpos) {
						// OK, now we have to copy the ptags and repointer them ....
						// we need other object-functions to handle this !!!!!
						// Just find the corresponding tag ....
						ARPositionTag *newpstart = NULL;
						GuidoPos tmppos = myptaglist->GetHeadPosition();
						while (tmppos) {
							ARPositionTag * pt = myptaglist->GetNext(tmppos);
							if (pt->getCorrespondence() == tgend) {
								newpstart = pt;
								break;
							}
						}

						if (newpstart) {
							ARMusicalObject * pend = tgend->Copy();
							ARPositionTag * newpend = dynamic_cast<ARPositionTag *>(pend);
							newpend->setPosition(newendpos);
							ARMusicalTag * newmtag = dynamic_cast<ARMusicalTag *>(newpend);
							newmtag->setRelativeTimePosition(tp);
							newpstart->setCorrespondence(newpend);
							newpend->setCorrespondence(newpstart);
							myptaglist->AddTail(newpend);
						}
					}
				}
				else if (myvst.curpositiontags) {
					// OK, now we have a tag that is not completely embedded ...
					if (tgend->getPosition() == posfirstinlastchord) {
						// this tag must be repointered ... and also moved !!!!!
						mPosTagList->setOwnership(0);
						if (vst.ptagpos && vst.ptagpos == prevptagpos)
							mPosTagList->GetNext(vst.ptagpos);
						mPosTagList->RemoveElementAt(prevptagpos);
						mPosTagList->setOwnership(1);
						tgend->setPosition(posfirstinnewchord);
						// tgend has no time information ....
						myptaglist->AddTail(tgend);
					}
				}
			}
		}
	}

	if (found) {
		// if myvst.ptagpos is NULL, we have to add at the very end ....

		// then we have the position of the first ptag after the
		// ending of the last event in the first chord.
		// we have to introduce the \chord, dispdur and shareStem
		// Tag at this location ....

		// now we need a chord, dispDur and shareStem-tag ....
		if (myvst.ptagpos) {
			// at this point we have to add the copied ptags ...
			GuidoPos tmppos = myptaglist->GetHeadPosition();
			while (tmppos) {
				ARPositionTag * ptag = myptaglist->GetNext(tmppos);
				ARDisplayDuration * dispdur = dynamic_cast<ARDisplayDuration *>(ptag);
				if( dispdur )
					dispdur->setDisplayDuration(newdur);

				mPosTagList->AddElementAt(myvst.ptagpos,ptag);
			}
		}
		else {
			// at this point we have to add the copied ptags ...
			GuidoPos tmppos = myptaglist->GetHeadPosition();
			while (tmppos) {
				ARPositionTag * ptag = myptaglist->GetNext(tmppos);
				ARDisplayDuration * dispdur = dynamic_cast<ARDisplayDuration *>(ptag);
				if( dispdur )
					dispdur->setDisplayDuration(newdur);

				mPosTagList->AddTail(ptag);
			}
		}
	}
	delete myptaglist;
	delete mylist;	mylist = 0;

	// now we have to deal with the position tags ...
	// (the chord-tags and also the other tags that might have been added inbetween ... what about repointering?)
	delete mystate;

	// OK, here we add the tie-tags by traversing the voice up until the end of the second chord ....
	mystate = new ARMusicalVoiceState(vst);
	ARMusicalVoiceState & myvst2 = * mystate;

	myptaglist = new typeptaglist();

	// traverse the voice ....
	int firstchord = 1;
	while (myvst2.vpos) {
		GuidoPos prevpos = myvst2.vpos;
		ARMusicalObject * obj = GetAt(myvst2.vpos);

		if (ARMusicalEvent::cast(obj) && obj->getDuration() == DURATION_0) {
            ARNote * nt = obj->isARNote();
			if (nt && (nt->isEmptyNote() || nt->isARTab())) {
			}
			else {
				// then we must add a tieBegin or end
				if (firstchord) {
					// add a tiebegin ....
					ARTie * ntie = new ARTie();
					ntie->setIsAuto(true);
					ntie->setID((int)gCurArMusic->mMaxTagId++);
					ntie->setPosition(myvst2.vpos);
					ntie->setRelativeTimePosition(myvst2.curtp);
					GuidoPos saveptagpos = NULL;
					if (vst.ptagpos == myvst2.ptagpos)	saveptagpos = vst.ptagpos;

					GuidoPos newpos = mPosTagList->AddElementAt(myvst2.ptagpos,ntie);
					if (saveptagpos) 					vst.ptagpos = newpos;
					myptaglist->AddTail(ntie);
				}
				else {
					ARTie *ntie = dynamic_cast<ARTie *>(myptaglist->RemoveHead());
					ARDummyRangeEnd * end = new ARDummyRangeEnd(TIEEND);
					end->setID(ntie->getID());
					end->setIsAuto( true );
					end->setPosition(myvst2.vpos);
					end->setCorrespondence(ntie);
					ntie->setCorrespondence(end);
					mPosTagList->AddElementAt(myvst2.ptagpos,end);
					// add a tieend (must match ...)
				}
			}
		}
        
		GetNext(myvst2.vpos,myvst2);
		if (myvst2.vpos == posfirstinnewchord)
			firstchord = 0;

		if (prevpos == poslastinnewchord)
			break;
	}
	delete myptaglist;
	delete mystate;
	readmode = oldreadmode;
	return posfirstinnewchord;
}

//____________________________________________________________________________________
void ARMusicalVoice::doAutoEndBar()
{
	if (!endState) return; // Can't do it without traversing the whole
		// voice ...

	ARMusicalObject * el = GetTail();
	if (el) {
        if (el->isARRepeatEnd() || el->isARFinishBar()) {
			// nothing to do, the bar is already there
		}
		else
		{
			// check if we need to add a FinishBar ...
			bool finishbar = true;
			ARMusicalTag * mtag = endState->getCurStateTag( typeid(ARAuto) );
			if (mtag) {
				ARAuto * autotag = dynamic_cast<ARAuto *>(mtag);
				if (autotag && autotag->getEndBarState() == ARAuto::kOff)
					finishbar = false;
			}
			bool endchord = (el->isEmptyNote() && el->getDuration() == DURATION_0);
			// now check whether there is a bar tag at the end ...
			// if this is the case, we do not set an endbar ....
			if ((endState->curlastbartp == endState->curtp) && !endchord)
			{
				finishbar = false;
			}
			if (finishbar)
			{
				// add a finishBar at the end of the Voice ...
				ARFinishBar * arfb = new ARFinishBar();
				arfb->setIsAuto(true);
				arfb->setRelativeTimePosition(this->getRelativeEndTimePosition());
				this->AddTail(arfb);
			}
		}
	}
}

//____________________________________________________________________________________
/** \brief Determines whether a given duration fits the given base.

this is needed for tuplet-detection and encapsulation.
The routine works as follows:
The duration is multiplied with the base
it is tested, whether the denominator is a power of 2
if yes, the duration fits the base
if no, the duration does not fit the base. The new base
is returned (it is the denominator divided by any power of two)
this is equivalent to the primfaktorzerlegung and removing the
2^n.
*/
bool ARMusicalVoice::DurationFitsBase( const TYPE_DURATION &dur, const TYPE_DURATION &base, TYPE_DURATION & newbase)
{
	TYPE_DURATION ndur = dur * base;

	// check, whether the calculated duration is a power of two (up to 1/32 == 1/2^5)
	if (IsPowerOfTwoDenom(ndur/*,5*/) && (ndur <= DURATION_1) && (ndur.getNumerator()==1 ))
		return true;// yes, then we are finished ...

	// OK, there is work to do: we need to determine the new base
	int val = ndur.getDenominator();

	// this removes divides l as long as it can be divided (removes powers of 2)
	while ( !(val & 1) )
		val >>= 1;

	// now we set the base ...
	switch (val) {
		case 1:
			newbase.setNumerator(val);
			newbase.setDenominator(1);
			break;
		case 3:
			newbase.setNumerator(val);
			newbase.setDenominator(2);
			break;
		case 5:
			newbase.setNumerator(val);
			newbase.setDenominator(4);
			break;
		case 7:
		case 9:
		case 11:
			newbase.setNumerator(val);
			newbase.setDenominator(8);
			break;
//		default:
//			newbase.setNumerator(val);
//			newbase.setDenominator(dur.getNumerator());
//			break;
	}
	return false;
}

//____________________________________________________________________________________
/** \brief Determines, whether a given
	duration is displayable with a SINGLE
	graphical element.

	The parameter b_punkt determines whether the element can have
	dots associated with it (example (3/4,1) can
	be displayed as a half-note with a dot but
	(3/4,0) can not be displayed, because a dot
	would be needed.
	note, that whole-measure-rests have to
	be checked BEFORE calling this algorithm.
	The algorithm works as follows:
	the duration is examined; the note is
	displayable without a point, if the numerator
	is 1.
	it is displayable with a single or double dot,
	if the numerator is 3 (and denom>=2)
	or 7 (and denom>=4)
	if it is not displayable, the longest displayable
	note without a dot(!) is determined as follows:
	the numerator is decreased, until the numerator
	reaches 1 (when normalized).
	example: 5/4 -> 4/4 == 1/1
	or: 7/2 -> 6/2 == 3/1 -> 2/1 -> 1/1
	or: 3/8 (no points) -> 2/8 == 1/4

	returns 1: the note is displayable
	returns 0: the duration has been shortened to make the
	//			  note displayable ...
	duration and b_punkt give duration and number of points
*/
bool ARMusicalVoice::DurationIsDisplayable(TYPE_DURATION & dur, int & outDotCount )
{
	dur.normalize();
	const int num = dur.getNumerator();
	const bool canBeLongaNote = true;	// (JB) TODO: handle rests, accept longa notes or not...

	if( num == 1 || ( num == 2 && canBeLongaNote )) {
		// finished, the note can be displayed
		// without change
//		outDotCount = 0;
		return true;
	}

	const int denom = dur.getDenominator();
	if (outDotCount) {
		if (num == 3) {
			// displayable with one dot
			if (denom >= 2) {
				outDotCount = 1;
				return true;
			}
		}
		else if (num == 7)
		{
			// displayable with two dots.
			if (denom >=4) {
				outDotCount = 2;
				return true;
			}
		}
	}

	// if we get here, the note is note displayable ...
	// now, we look for the biggest full note.
	TYPE_DURATION tmp ( num-1, denom); // TODO: num-2 for longa notes !?
	tmp.normalize();

	while (tmp.getNumerator() != 1 && tmp.getNumerator() > 0) {
		tmp.setNumerator( tmp.getNumerator()-1);
		tmp.normalize();
	}
	assert(tmp.getNumerator() == 1);
	dur = tmp;

	// no dots for this note!
	outDotCount = 0;
	return false;
}

//____________________________________________________________________________________
/** \brief Checks whether the events in the voice can be displayed as single graphical objects (with and without dots)

it also determines realTuplets (that is a change in base) and groups them together;

This is helpful, because the NoteFactory becomes unemployed that way and the structure of the NoteViewer becomes clearer.
The algorithm works as follows:

Go through the events sequentially. 
if a merge-Tag is encountered, all events within the merge-Tag-Range are collected
(because it should be displayable as a single note!)
note that mergelookahead is then active.
ATTENTION: this is not yet implemented

Check, whether the duration fits the current base (base is initially 1) using the function DurationFitsBase.
if not: close the current base (if it is not 1) and open a new base as returned by DurationFitsBase.
continue in yes-case.
if yes: check, whether the event is displayable as a single graphical element. This check is done in DurationIsDisplayable.
If the event is a rest or if the current base is not one, no dots are allowed (no dots within tuplets!).
(ATTENTION, what happens, if dots are explicitly given in the case of tuplets?)
(ATTENTION also: what happens, if rests have dots?)
(ATTENTION, what is the case with whole-measure-rests?)
if the event is displayable then set the displayDuration (as returned by DurationIsDisplayable)
iff the base is not one (==we are in a tuplet)

if the event is NOT displayable, the event has to be broken (the duration of the first displayable part is returned by
DurationIsDisplayable); a copy of the event is made, and the respective durations are set.
if this case is encountered during a merge-operation (mergelookahead),
then rewind, flag the merge-Tag is "invalid" and continue as if the merge-tag was not there.

continue: add the current duration to the length of the current tuplet-base (if base != 1)
check, if tuplet is full (base != 1), that is,
check, wether the tuplet-duration is a real power of two (n/1 (n>=1), 1/2, 1/4, 1/8 ...)
if it is full, close it und return to base 1.
this makes sure, that the smallest tuplets are found.

continue with the next event.
*/
void ARMusicalVoice::doAutoDisplayCheck()
{
	// counts the tie-tags needs to be remembered, if we break events...
	int tiecount = 0;

	// this remebers, if we are in a merge-lookahead-situation.
	// if merge is encountered, all events in the range are added (durationwise)
	int mergelookahead = 0;
	ARMusicalVoiceState armergestate;
	ARMerge * curmerge = NULL;
	int mergesavetiecount=0;

	// holds the current base. initially this is just 1
	TYPE_DURATION base(1,1);
	// the base-tag that encapsulates real tuplets.
	ARBase * curbase = NULL;

	// this is an automatic tuplet (it is included if a base-change is done and no other tuplet-tag is active ...)
	ARTuplet * autotuplet = NULL;
	GuidoPos lastevpos = NULL;		// the lasteventposition
	GuidoPos FLA = NULL;			// the position for inserting the EndPosition of the base-tag
	int tupletcount = 0;			// the count of for tuplet-tags

	// holds the current meter information for a voice.  if curmeter is NULL, no meter has been set.
	// this is important for whole-measure-rests which can be displayed just as a whole-note-rest.
	// ATTENTION can a meter be turned off?
	ARMeter * curmeter = NULL;
	TYPE_DURATION curmetertime;
	ARMusicalVoiceState vst;
	const ARDisplayDuration* ardisp = nullptr;

	// this operation eats all but the first non-state  events or tags.
	GuidoPos pos = GetHeadPosition(vst);
	while (pos)
	{
		TYPE_DURATION dur = DURATION_0;
		// track tie- and merge-tags


//const PositionTagList * tl = vst.getCurPositionTags();
//GuidoPos tlpos = tl ? tl->GetHeadPosition() : 0;
//bool hasDispDur = false;
//while (tlpos) {
//	ARPositionTag* ptag = tl->GetNext(tlpos);
//	ARDisplayDuration * disp = dynamic_cast<ARDisplayDuration *>(ptag);
//	if (disp) cerr << __LINE__ << " ARMusicalVoice::doAutoDisplayCheck disp " << disp << " " << disp->getIsAuto() << " " << disp->getRelativeTimePosition() << " " << disp->getDuration() << endl;
//	hasDispDur = true;
//}

		// check, whether positiontags have been removed ...
		if (vst.removedpositiontags)
		{
			GuidoPos tmppos = vst.removedpositiontags->GetHeadPosition();
			while (tmppos)
			{
				ARPositionTag * arpt = vst.removedpositiontags->GetNext(tmppos);
				ARTie * artie = dynamic_cast<ARTie *>(arpt);
				ARMerge * armerge;
				if (artie)
					-- tiecount;
				else if((armerge = dynamic_cast<ARMerge *>(arpt)) != 0 )
				{
					// handle this ...
					mergelookahead = 0;
					curmerge = NULL;
				}
				else if(dynamic_cast<ARTuplet *>(arpt) != 0 )
				{
					-- tupletcount;
					// if there is an active base, t is closed ...
					if (curbase != NULL)
					{
						assert(autotuplet == NULL);
						CloseBase(curbase,autotuplet,lastevpos,FLA);
						vst.RemovePositionTag(curbase,0);
						curbase->finish();
						curbase = NULL;
						base = DURATION_1;
					}
				}
			}
		}

		// first the added ones ...
		if (vst.addedpositiontags)
		{
			GuidoPos tmppos = vst.addedpositiontags->GetHeadPosition();
			while (tmppos)
			{
				ARPositionTag * arpt = vst.addedpositiontags->GetNext(tmppos);

				ARTie * artie = dynamic_cast<ARTie *>(arpt);
				ARMerge * armerge;
				if (artie) ++ tiecount;
				else if (dynamic_cast<ARTuplet *>(arpt) != 0 )
				{
					++ tupletcount;
					// close the old base (if one exists)
					if (curbase != NULL)
					{
						CloseBase(curbase,autotuplet,lastevpos,FLA);
						curbase->finish();
						if (autotuplet)
							autotuplet->setupTuplet( curbase );
						vst.RemovePositionTag(curbase,0);
						if (autotuplet)
							vst.RemovePositionTag(autotuplet,0);

						curbase = NULL;
						autotuplet = NULL;
						base = DURATION_1;
					}
				}
				else if(((armerge = dynamic_cast<ARMerge *>(arpt)) != 0 ) && (armerge->getError() == 0))
				{
					mergesavetiecount = tiecount;

					// now we are in a merge-situation
					armergestate = vst;
					curmerge = armerge;
					mergelookahead = 1;
					dur = DURATION_0;

					// now we collect the events and sum up the durations ...
					GuidoPos tmppos = pos;
					ARMusicalEvent * baseev = NULL;
					for( ; ; )
					{
						ARMusicalEvent * ev = ARMusicalEvent::cast(GetAt(tmppos));
						if (ev)
						{
							if (!baseev)
								baseev = ev;
							else
							{
								// check, whether the event can be merged ....
								if (!baseev->CanBeMerged(ev))
								{
									curmerge->setError(1);
									break;
								}
							}
							dur += ev->getDuration();
						}

						if (curmerge->getError() || tmppos == curmerge->getEndPosition())
							break;
						// check, whether tie or merge-tags start and end ...
						// WARNING for overlapping merge and overlapping Ties ....
						GetNext(tmppos,armergestate);
					}

					if (curmerge->getError())
					{
						// delete the merge-tag from the mPosTagList ....
						ARPositionTag * artgend = curmerge->getCorrespondence();

						if (vst.ptagpos)
						{
							if (mPosTagList->GetAt(vst.ptagpos) == artgend)
							{
								// then we need to increment the position
								mPosTagList->GetNext(vst.ptagpos);
							}
						}

						vst.RemovePositionTag(curmerge,0);
						mPosTagList->RemoveElement(curmerge);
						mPosTagList->RemoveElement(artgend);
						curmerge = NULL;
						mergelookahead = 0;
						continue;
					}
				}
			}
		}

		// track the meter
		if (curmeter != vst.curmeter)
		{
			curmeter = vst.curmeter;
			curmetertime.setNumerator(curmeter->getNumerator());
			curmetertime.setDenominator(curmeter->getDenominator());
			curmetertime.normalize();
		}

		ARMusicalObject * o = GetAt(pos);
		ARMusicalEvent * ev = ARMusicalEvent::cast(o);
		bool dontsplit = (tupletcount > 0); //false;
		if (ev && !ev->getAppearance().empty()) dontsplit = true;

		if (!mergelookahead && ev)
			dur = ev->getDuration();

		if (dur > DURATION_0)
		{
			int dots = ev->getPoints();
//cerr << __FILE__ << " " << __LINE__ << " check ev " << ev << " points: " << ev->getPoints() << endl;
			// check, whether the duration fits the current base
			TYPE_DURATION newbase;
			if( DurationFitsBase(dur,base,newbase) == false ) {
				// the duration does not fit close the current base (if base != 1)
				if (curbase != NULL) {
					if (tupletcount > 0) {
						assert(autotuplet == NULL);
//						GuidoWarn("Tuplettag does not coincide with durations");   useless (?)
					}
					// close the base ...
					CloseBase(curbase,autotuplet,lastevpos,FLA);

					// remove the tag from the current position tags (it will not be visited again ...)
					vst.RemovePositionTag(curbase,0);
					if (autotuplet) vst.RemovePositionTag(autotuplet,0);

					curbase = NULL;
					autotuplet = NULL;
					base = DURATION_1;
				}

				// now open a new curbase, if newbase != 1
				if (newbase.getNumerator() > 1) {
					curbase = new ARBase();
					// curbase->setRange(1);
					curbase->setPosition(pos);
					curbase->setBase(newbase);

					if (tupletcount == 0) {
						autotuplet = new ARTuplet();
						autotuplet->setPosition(pos);
						autotuplet->setAuto();
					}
					// now, add the base to the position-tags ...
					// curbase needs a timeposition!
					curbase->setRelativeTimePosition(ev->getRelativeTimePosition());
					if (tupletcount == 0) 	autotuplet->setRelativeTimePosition(ev->getRelativeTimePosition());

					if (!mPosTagList) {
						mPosTagList = createPositionTagList();
						vst.ptagpos = mPosTagList->GetHeadPosition();
					}

					if (vst.ptagpos != NULL) {
						if (tupletcount == 0) 	mPosTagList->AddElementAt(vst.ptagpos,autotuplet);
						mPosTagList->AddElementAt(vst.ptagpos,curbase);
					}
					else {
						if (tupletcount == 0) 	mPosTagList->AddTail(autotuplet);
						mPosTagList->AddTail(curbase);
					}

					// this adds the base to the current-positiontags ...
					if (tupletcount == 0) 	vst.AddPositionTag(autotuplet,0);
					vst.AddPositionTag(curbase,0);

					base = newbase;
				}
			}

			// now we check, whether the duration is displayable ... (this is done according to base)
			TYPE_DURATION dispdur (dur);
			if (base.getNumerator() != 1)	TupletdurToDispdur(dispdur,base);

			// set, whether dots are allowed
			int b_punkt = 1;	// the number of dots.
			if (base.getNumerator() != 1)
				b_punkt = 0;

			if (DurationIsDisplayable(dispdur,b_punkt)) {
				// if there is a base other than 1, we have to tell the dispduration for the event ...
				// we need to do this also, when a merge-tag is active ...
				if (base.getNumerator() != 1 || mergelookahead || (tupletcount && b_punkt && ardisp && dots))
//				if ((!hasDispDur && base.getNumerator() != 1) || mergelookahead)
					ardisp = InsertDisplayDurationTag( dispdur,b_punkt, ev->getRelativeTimePosition(),pos, vst);

				if (mergelookahead) {
					// now set displayDuration for the other events in the merge-range to 0
					GuidoPos tmppos = pos;
					armergestate = vst;
					GetNext(tmppos,armergestate);
					for( ; ; ) {
						ARMusicalEvent * tmpev = ARMusicalEvent::cast(GetAt(tmppos));
						if (tmpev) {
							// the current tagposition is in armergestate.ptagpos ...
							ardisp = InsertDisplayDurationTag(DURATION_0, 0,tmpev->getRelativeTimePosition(), tmppos,armergestate);
						}
						if (tmppos == curmerge->getEndPosition())
							break;
						GetNext(tmppos,armergestate);
					}
				}
//				hasDispDur = false;
			}
			else if (!dontsplit) {
				// not Displayable ... this means, we have to adjust the event ...

				// if we are in mergelookahead, the merge-tag has to be flaged invalid and
				// we have to continue at the same location, as if the merge-tag had not been there.
				if (mergelookahead) {
					// now, we need to change the merge-tag into a Tie-Tag ...
					ARTie * artie = new ARTie();
					artie->setID((int)gCurArMusic->mMaxTagId++);
					artie->setIsAuto(true);

					ReplacePositionTag(curmerge,artie,vst,TIEEND);

					delete curmerge;
					curmerge = NULL;
					mergelookahead = 0;
					tiecount = mergesavetiecount + 1;

					// this ensures, that position-tags are not parsed twice
					// (the tie and merge-tags have been looked at already ...)
					vst.DeleteAddedAndRemovedPTags();
					continue;
				}

				// undo the transformation, so that we can handle the duration correctly ...
				dur = dispdur;
				if (base.getNumerator() != 1)
					DispdurToTupletdur(dur,base);

				// now calculate the durations ...
				const TYPE_DURATION olddur (ev->getDuration());
				TYPE_DURATION newdur = dur;

				if (vst.curchordtag)
				{
					// then we are inside a chord ....
					// then, we have to set the duration also of the dispDur tag that must be present somewhere ....
					if (vst.fCurdispdur)
					{
						// how can we make sure, that the new duration is displayable?
						// must be checked in doAutoDisplayCheck!
						vst.fCurdispdur->setDisplayDuration(newdur);

					}
					// we have to set the timepositions of the other events and tags within the chord-range ....
					TYPE_TIMEPOSITION mytp = ev->getRelativeTimePosition() + newdur;
					GuidoPos startpos = vst.curchordtag->getPosition();
					while (startpos)
					{
						GuidoPos tmppos = startpos;
						ARMusicalObject *obj = ObjectList::GetNext(startpos);

						if (obj && obj->getDuration() == DURATION_0)
							obj->setRelativeTimePosition(mytp);
						if (tmppos == vst.curchordtag->getEndPosition())
							break;
					}
				}

				// this sets the duration to the shorter length ...
				ev->setDuration(newdur);
				// that is the new duration of the event.
				TYPE_TIMEPOSITION ntp = ev->getRelativeTimePosition() + newdur;

				GuidoPos newpos = pos;
				if (vst.curchordtag)
				{
					// if we have a chord(tag), we need to copy the whole chord (including tags ....)
					// how do we do that?
					// copychord returns the position of the first event in the new (copied) chord.
					newpos = CopyChord(vst, ntp, olddur - newdur);
					// in chordmode ... this should get me to the first event after the chord ....
					GetNext(vst.vpos,vst);
					pos = vst.vpos;
				}
				else
				{
					// now we create a new event, that is a copy of ev and has a  new duration ...
					ARMusicalEvent * ev2 = /*dynamic*/static_cast<ARMusicalEvent *>(ev->Copy());
					if (ev2)
					{
						ev2->setRelativeTimePosition(ntp);
						ev2->setDuration( olddur - newdur);
						newpos = AddElementAfter(pos, ev2);
					}
					else
						assert(false);

					// this position remembers the location of the MergeEnd or
					// tieEnd that could be introduced. this is important, so that tie repositioning works ...
					GuidoPos newptagpos = NULL;
                    ARNote * n2 = ev2->isARNote();
					// now look, if there are any ties and merge-tags ..
					if (tiecount == 0 && n2 && !(n2->getName() == ARNoteName::empty) && !(n2->isARTab()) )
					{
						if (mPosTagList == NULL)
						{
							mPosTagList = createPositionTagList();
							vst.ptagpos = mPosTagList->GetHeadPosition();
						}

						// so we add our tie ...
						ARTie * artie = new ARTie();
						artie->setID((int)gCurArMusic->mMaxTagId++);
						artie->setIsAuto(true);
						artie->setRelativeTimePosition( ev->getRelativeTimePosition());
						// the tie has a range!
						// artie->setRange(1);
						artie->setPosition(pos);
						ARDummyRangeEnd * arde = new ARDummyRangeEnd(TIEEND);
						arde->setID(artie->getID());
						arde->setPosition(newpos);
						artie->setCorrespondence(arde);
						arde->setCorrespondence(artie);

						if (vst.ptagpos != NULL)
						{
							mPosTagList->AddElementAt(vst.ptagpos,artie);
							// the position of this element must be saved!
							newptagpos = mPosTagList->AddElementAt(vst.ptagpos,arde);
						}
						else
						{
							mPosTagList->AddTail(artie);
							// remember this position!
							newptagpos = mPosTagList->AddTail(arde);
						}

						// add the tag to the currentposition-tags, but don't add it to the
						// addedtaglist ... this would otherwise confuse the beginning of this function.
						vst.AddPositionTag(artie,0);

						// we set this explicitly, because the tag is not added to the addedtaglist.
						// Otherwise, multiple extensions of the same note would lead to multiple tie or mergtag-increments.
						++ tiecount;
					}

					// now we need to "repointer" the position tags that formerly were attached to ev -> they now need to be attached to ev2
					// (which is at newpos)
					GuidoPos tmppos = vst.ptagpos;
					while (tmppos)
					{
						ARPositionTag * arpt = mPosTagList->GetNext(tmppos);
						ARTagEnd * arte = ARTagEnd::cast(arpt);
						if (arte) {
							if (arte->getPosition() == pos) {
								arte->setPosition(newpos);
								continue;
							}
						}
						break;
					}

					// the positiontagpointer now points on the FLA of ev_2 ...
					// (which is the added tie or merge)
					if (newptagpos != NULL)
						vst.ptagpos = newptagpos;

					// now we have to set a displayDuration tag for the first event ....
					// this determines, wether the event gets a displayDuration-Tag ...
					if (base.getNumerator() != 1)
						ardisp = InsertDisplayDurationTag(dispdur,b_punkt, ev->getRelativeTimePosition(), pos, vst,0);

					// the added and removedpositiontags have to be removed ... otherwise the removedpositiontags can make problems ...
					vst.DeleteAddedAndRemovedPTags();
					pos = newpos;
				}
				if (curbase)
					curbase->addEvent(ev);

				lastevpos = pos;
				FLA = vst.ptagpos;
				continue;
			}
		} // if dur>DURATION_0

		if (curbase && ev) {
			// adds the "real" duration of the event, not the displayDuration ...
			curbase->addEvent(ev);
		}


		// now, we check, wether the length of the curbase is full ....
		// it is only closed, if there are no active tuplet-tags in the area.
		if (curbase != NULL && tupletcount<1)
		{
			const TYPE_DURATION tmp = curbase->getBaseDuration();

			if (IsPowerOfTwoDenom(tmp/*,6*/))
			{
				// OK, we are finished ... close the base ...
				CloseBase(curbase,autotuplet,pos,vst.ptagpos);

				// set the parameter of the curbase?
				curbase->finish();
				if (autotuplet) 	autotuplet->setupTuplet( curbase );
				vst.RemovePositionTag(curbase,0);
				if (autotuplet) 	vst.RemovePositionTag(autotuplet,0);
				curbase = NULL;
				autotuplet = NULL;
				base = DURATION_1;
				ardisp = nullptr;
			}
		}

		if (ev) {
			lastevpos = pos;
			FLA = vst.ptagpos;
		}
		GetNext(pos,vst);
	}

	// if the currentbase is still there, it has to be closed ....
	if (curbase) {
		// OK, we are finished ...
		// close the base ...
		ARDummyRangeEnd * arde = new ARDummyRangeEnd("\\baseEnd");
		ARDummyRangeEnd * ardetpl = NULL;
		if (autotuplet) {
			ardetpl = new ARDummyRangeEnd("\\tupletEnd");
			ardetpl->setPosition(lastevpos);
			autotuplet->setCorrespondence(ardetpl);
			ardetpl->setCorrespondence(autotuplet);
		}
		arde->setPosition(lastevpos);
		curbase->setCorrespondence(arde);
		arde->setCorrespondence(curbase);
		ardisp = nullptr;
		
		// this adds the Element ....
		if (vst.ptagpos != NULL) {
			if (autotuplet) 	mPosTagList->AddElementAt(vst.ptagpos,ardetpl);
			mPosTagList->AddElementAt(vst.ptagpos,arde);
		}
		else {
			if (autotuplet) 	mPosTagList->AddTail(ardetpl);
			mPosTagList->AddTail(arde);
		}

		curbase->finish();
		if (autotuplet) 		autotuplet->setupTuplet( curbase );

		// this removes them from the voice-state but they are still there ...
		vst.RemovePositionTag(curbase);
		if (autotuplet)			vst.RemovePositionTag(autotuplet);
	}
}

//____________________________________________________________________________________
/** \brief Converts tupletdurations into displaydurations.
	This algorithm needs to be adjustable towards all different kinds
	of ideas (one would be Hindemiths mapping)
	right now, the algorithm is simple:
	the displyduration is determined by the given base.
*/
void ARMusicalVoice::TupletdurToDispdur( TYPE_DURATION & dur, const TYPE_DURATION & base)
{
	dur = dur * base;
	//assert(IsPowerOfTwoDenom(dur/*,8*/)); // Commented 21/10/14
}

//____________________________________________________________________________________
// depends heavliy on the base!
// base determines the factor
void ARMusicalVoice::DispdurToTupletdur( TYPE_DURATION & dur, const TYPE_DURATION & base)
{

	TYPE_DURATION tmp(base.getDenominator(), base.getNumerator());
	dur = dur * tmp;
}

//____________________________________________________________________________________
const ARDisplayDuration * ARMusicalVoice::InsertDisplayDurationTag(const TYPE_DURATION &dispdur, int b_punkt,
											  const TYPE_TIMEPOSITION &tp, GuidoPos pos,
											  ARMusicalVoiceState &vst, int setptagpos)
{
	ARDisplayDuration * ardisp = new ARDisplayDuration();
	ardisp->setIsAuto(true);
	ardisp->setDisplayDuration(dispdur, b_punkt);
	ardisp->setRelativeTimePosition(tp);
	ardisp->setPosition(pos);

	ARDummyRangeEnd * ardre = new ARDummyRangeEnd("\\dispDurEnd");
	ardre->setPosition(pos);
	ardre->setCorrespondence(ardisp);
	ardisp->setCorrespondence(ardre);

	GuidoPos newptagpos = NULL;

	// add the position tag ...
	if (!mPosTagList) {
		mPosTagList = createPositionTagList();
		vst.ptagpos = mPosTagList->GetHeadPosition();
	}

	if (vst.ptagpos != NULL) {
		mPosTagList->AddElementAt(vst.ptagpos,ardisp);
		newptagpos = mPosTagList->AddElementAt(vst.ptagpos,ardre);
	}
	else {
		mPosTagList->AddTail(ardisp);
		newptagpos = mPosTagList->AddTail(ardre);
	}

	vst.AddPositionTag(ardisp,0);
	if (setptagpos) vst.ptagpos = newptagpos;
	return ardisp;
}

//____________________________________________________________________________________
/** \brief Replaces positiontags. The correspondence is set accordingly.
*/
void ARMusicalVoice::ReplacePositionTag( const ARPositionTag * ptold, ARPositionTag * ptnew, ARMusicalVoiceState &vst, const char * endtagname)
{
	const ARMusicalObject * mold = dynamic_cast<const ARMusicalObject *>(ptold);
	ARMusicalObject * mnew = dynamic_cast<ARMusicalObject *>(ptnew);

	assert(mold);
	assert(mnew);

	mnew->setRelativeTimePosition( mold->getRelativeTimePosition());
	ptnew->setPosition(ptold->getPosition());
	ARPositionTag * artgend = ptold->getCorrespondence();

	if (artgend)
	{
		ARDummyRangeEnd * ardre = dynamic_cast<ARDummyRangeEnd *>(artgend);
		if (ardre)
		{
			if (endtagname)
				ardre->endstr = endtagname;
			else
				ardre->endstr = ")";

			ARMusicalTag * mt = dynamic_cast<ARMusicalTag *>(ptnew);
			if (mt)
				ardre->setID(mt->getID());
		}
	}

	artgend->setCorrespondence(ptnew);
	ptnew->setCorrespondence(artgend);

	GuidoPos tmppos = mPosTagList->GetElementPos(ptold);
	mPosTagList->SetAt(tmppos,ptnew);
	tmppos = vst.curpositiontags->GetElementPos(ptold);
	vst.curpositiontags->SetAt(tmppos,ptnew);
}


//____________________________________________________________________________________
void ARMusicalVoice::CloseBase(ARBase * curbase, ARTuplet * autotuplet, GuidoPos lastevpos, GuidoPos FLA)
{
	ARDummyRangeEnd * arde = new ARDummyRangeEnd("\\baseEnd");

	ARDummyRangeEnd * ardetpl = NULL;
	if (autotuplet)
		ardetpl = new ARDummyRangeEnd("\\tupletEnd");

	arde->setPosition(lastevpos);
	if (autotuplet)		ardetpl->setPosition(lastevpos);

	curbase->setCorrespondence(arde);
	if (autotuplet)		autotuplet->setCorrespondence(ardetpl);

	arde->setCorrespondence(curbase);
	if (ardetpl)		ardetpl->setCorrespondence(autotuplet);

	// this adds the Element ....
	if (FLA!=NULL)
	{
		mPosTagList->AddElementAt(FLA,arde);
		if (autotuplet)	mPosTagList->AddElementAt(FLA,ardetpl);
	}
	else
	{
		mPosTagList->AddTail(arde);
		if (autotuplet)	mPosTagList->AddTail(ardetpl);
	}
	// set the parameter of the curbase?
	curbase->finish();
	if (autotuplet)		autotuplet->setupTuplet( curbase );
}

//____________________________________________________________________________________
/*
	introduced to restructure the doAutoCheckStaffStateTags method
	DF Apr.20 2011
*/
ARClef* ARMusicalVoice::newAutoClef(ARClef * oldclef, const TYPE_TIMEPOSITION& tp)
{
	ARClef * clef;
	if (!oldclef)
	{
		clef = new ARClef();
		clef->setIsAuto(true);
		if (sum>0)
		{
			float meanpitch = (float)pitchsum/ sum;
			if ( meanpitch >= float(8.0))	clef->setName("treble");
			else							clef->setName("bass");
		}
		else	clef->setName("treble");
	}
	else
	{
		clef = new ARClef(*oldclef);
		if (oldclef->getIsAuto())			clef->setIsAuto(true);
	}
	clef->setRelativeTimePosition(tp);
	return clef;
}

//____________________________________________________________________________________
/*
	introduced to restructure the doAutoCheckStaffStateTags method
	DF Apr.20 2011
*/
ARKey* ARMusicalVoice::newAutoKey(ARKey * oldkey, const TYPE_TIMEPOSITION& tp)
{
	ARKey * key;
	if (!oldkey)
	{
		key = new ARKey(0);
		key->setIsAuto(true);
	}
	else
	{
		key = new ARKey(*oldkey);
		if (oldkey->getIsAuto()) key->setIsAuto(true);
	}
	key->setRelativeTimePosition(tp);
	return key;
}

//____________________________________________________________________________________
/** \brief Makes sure that clef/key/meter-information is provided correctly.

	the routine works as follows: the voice is traversed sequentially.
	At the beginning, it is checked, whether clef(!) information is provided before the first event.
	if not, clef-information is included. (because clef-information is definitly needed for display)
	then, whenever a newSystem or a newPage-Tag is encountered, it is made sure, that clef-information
	// (and possibly key-information) is included properly!
*/
void ARMusicalVoice::doAutoCheckStaffStateTags()
{
	// we do not care about positiontags in this round, but we do take care of state-tags ....
	ARMusicalVoiceState vst;
	TYPE_TIMEPOSITION tp;

	// we are at the beginning.
	bool waitFirstEv = true;
	// at the beginning, we just need a clef.
	int needsclef = 1;
	int needskey  = 1;

	GuidoPos evpos	= NULL;
	GuidoPos keypos = NULL;
	GuidoPos meterpos = NULL;

	GuidoPos pos = GetHeadPosition(vst);
	GuidoPos firstpos = pos;
	GuidoPos lastpos;
	while (pos)
	{
		lastpos = pos;
        
		ARMusicalObject * o = GetNext(pos,vst);
		tp = o->getRelativeTimePosition();
		ARClef      *clef  = static_cast<ARClef *>     (o->isARClef());
        ARKey       *key   = static_cast<ARKey *>      (o->isARKey());
		ARMeter     *meter = static_cast<ARMeter *>    (o->isARMeter());
        ARNewSystem *sys   = static_cast<ARNewSystem *>(o->isARNewSystem());
		ARNewPage   *page  = static_cast<ARNewPage *>  (o->isARNewPage());

		if (!clef && !key && !meter && !sys && !page)
		{
            const ARMusicalTag *armt = NULL;
            if (o) armt = static_cast<const ARMusicalTag *>(o->isARMusicalTag());
			if (armt && armt->IsStateTag()) continue;
		}

		ARMusicalEvent * ev = ARMusicalEvent::cast(o);
		// then we have an event->that means the initial phase is over ...
		if (waitFirstEv)
		{
			if (ev)
			{
				evpos = lastpos;
				if (!meterpos)	meterpos = evpos;
				if (!keypos)	keypos = meterpos;

				// then we should introduce whatever is needed at savepos ...
				if (needsclef)
				{
					ARClef * tmp = newAutoClef (dynamic_cast<ARClef *>(vst.getCurStateTag(typeid(ARClef))), tp);
                    tmp->setIsInHeader(true);
					GuidoPos newpos = AddElementAt(keypos,tmp);
					vst.AddStateTag(tmp);
					if (keypos == firstpos)					// this is important for auto-ordering ...
						firstpos = newpos;
				}

				if (needskey)
				{
                    ARKey * tmp = newAutoKey (dynamic_cast<ARKey *>(vst.getCurStateTag(typeid(ARKey))), tp);
                    tmp->setIsInHeader(true);
					GuidoPos newpos = AddElementAt(meterpos,tmp);
					if (meterpos == firstpos)	firstpos = newpos;
					vst.AddStateTag(tmp);
				}

				// now we add a SecondGlue ....
				ARSecondGlue * arsglue = new ARSecondGlue();
				arsglue->setIsAuto(true);
				arsglue->setRelativeTimePosition(tp);
                arsglue->setIsInHeader(true);
				GuidoPos newpos = AddElementAt(evpos,arsglue);
				vst.AddStateTag(arsglue);
				if (evpos == firstpos)
					firstpos = newpos;

				int doclefkeymeterexchange = 1;

				ARMusicalTag * mytmptag = vst.getCurStateTag( typeid( ARAuto ));
				if (mytmptag)
				{
					ARAuto *autotag = dynamic_cast<ARAuto *>(mytmptag);
					if (autotag && autotag->getClefKeyMeterOrderState() == ARAuto::kOff)
						doclefkeymeterexchange = 0;
				}

				// now, we have to do an AUTO-Sort of clef, key, meter information ...
				if (doclefkeymeterexchange)
				{
					GuidoPos savefirstpos = firstpos;
					while (firstpos)
					{
						GuidoPos savepos = firstpos;
						ARMusicalObject *o1 = ObjectList::GetNext(firstpos);
						if (ARMusicalEvent::cast(o1))
							break;
						ARMusicalObject *o2 = GetAt(firstpos);
						if (ARMusicalEvent::cast(o2))
							break;
#if 1
						ARMusicalTag *t1 = dynamic_cast<ARMusicalTag*>(o1);
						ARMusicalTag *t2 = dynamic_cast<ARMusicalTag*>(o2);
						if (t1 && t2) {
							int pos1 = t1->getOrder(), pos2 = t2->getOrder();
							if ((pos1 < 0) || (pos2 < 0))		// don't move
								continue;
							else if (pos1 > pos2) {
								SetAt(savepos,o2);
								SetAt(firstpos,o1);
								firstpos = savefirstpos;
								continue;
							}
						}
#else
#if 0
						bool o2isMKC = o2->isARMeter() || o2->isARKey() || o2->isARClef();
						bool o1isMKC = o1->isARMeter() || o1->isARKey() || o1->isARClef();
						bool o1isOtherTag = o1->isARSecondGlue();
//						bool o1isOtherTag = !o1isMKC && dynamic_cast<ARMusicalTag*>(o1) ;
						if (   (o1isOtherTag && o2isMKC)
							|| (o1->isARMeter() && o2->isARClef())
							|| (o1->isARMeter() && o2->isARKey())
							|| (o1->isARKey()	&& o2->isARClef()))
#else
						if (   (static_cast<ARSecondGlue *>(o1->isARSecondGlue()) && static_cast<ARMeter *>(o2->isARMeter()))
                            || (static_cast<ARSecondGlue *>(o1->isARSecondGlue()) && static_cast<ARKey *>(o2->isARKey()))
                            || (static_cast<ARSecondGlue *>(o1->isARSecondGlue()) && static_cast<ARClef *>(o2->isARClef()))
							|| (static_cast<ARMeter *>(o1->isARMeter()) && static_cast<ARClef *>(o2->isARClef()))
							|| (static_cast<ARMeter *>(o1->isARMeter()) && static_cast<ARKey *>(o2->isARKey()))
							|| (static_cast<ARKey *>(o1->isARKey()) && static_cast<ARClef *>(o2->isARClef())))
#endif
						{
							// then we need to switch ...
							SetAt(savepos,o2);
							SetAt(firstpos,o1);
							firstpos = savefirstpos;
							continue;
						}
#endif
					}
				}
				waitFirstEv = false;
				needsclef = 1;
				needskey = 1;
			}
			else if (clef) { // then we don't need a clef ...
                clef->setIsInHeader(true);
                needsclef = 0;
            }
			else if (key) {
                key->setIsInHeader(true);
				needskey = 0;				// then we don't need a key
//				if (meterpos)
//                    GuidoWarn("Meter set before key !");
				keypos = lastpos;
			}
			else if (meter) {
                meter->setIsInHeader(true);
                meterpos = lastpos;
            }
			else {
				// this ensures, that clefs are added REALLY before any other tag.
				if (!meterpos)		meterpos = lastpos;
				if (!keypos)		keypos = meterpos;
			}
		}

		if (sys || page)
		{
			waitFirstEv = true;
			evpos = NULL;
			meterpos = NULL;
			keypos = NULL;
		}
	}
}


//____________________________________________________________________________________
/** \brief Splits the Event at the current position into two events;

	The first event gets a new duration as calculated with the tp-parameter.
	The Split Event is joint with a tie or a merge tag (according to last parameter).
	This routine works just like all the other GNF-Transformations ....
*/
void ARMusicalVoice::SplitEventAtPos( ARMusicalVoiceState & vst, const TYPE_TIMEPOSITION & tp, int /*tieormerge*/)
{
	ARMusicalEvent *ev = ARMusicalEvent::cast(GetAt(vst.vpos));
	assert(ev);

	// first, we calculate the tie-merge-count
	int tiemergecount = 0;
	if (vst.curpositiontags) {
		GuidoPos pos = vst.curpositiontags->GetHeadPosition();
		while (pos) {
			ARPositionTag * arpt = vst.curpositiontags->GetNext(pos);
			if (dynamic_cast<ARTie *>(arpt) || dynamic_cast<ARMerge *>(arpt))
				++ tiemergecount;
		}
	}

	// now, we need to split the event ... (just like during bar-creation)

	// this is the position within the positiontags where a tie/merge-tag can be inserted.
	// it lies just after the current event or is NULL (no mPosTagList, end of mPosTagList)

	// now calculate the durations ...
	const TYPE_DURATION olddur (ev->getDuration());
	const TYPE_DURATION newdur (tp - ev->getRelativeTimePosition());

	// copy this from other location ...
	if (vst.curchordtag) {
		// we are within a chord! then, we have to set the duration also of
		// the dispDur tag that must be present somewhere ....
		if (vst.fCurdispdur)
			vst.fCurdispdur->setDisplayDuration(newdur);

		// we have to set the timepositions of the other events and tags within the chord-range ....
		const TYPE_TIMEPOSITION mytp (ev->getRelativeTimePosition() + newdur);
		GuidoPos startpos = vst.curchordtag->getPosition();
		while (startpos) {
			GuidoPos tmppos = startpos;
			ARMusicalObject * obj = ObjectList::GetNext(startpos);
			if (obj && obj->getDuration() == DURATION_0)
				obj->setRelativeTimePosition(mytp);

			if (tmppos == vst.curchordtag->getEndPosition())
				break;
		}
	}
	// this sets the duration to the shorter length ...
	ev->setDuration(newdur);

	GuidoPos pos = vst.vpos;
	GuidoPos newpos = vst.vpos;
	if (vst.curchordtag) {
		// if we have a chord(tag), we need to copy the whole chord (including tags ....)
		// how do we do that?
		// copychord returns the position of the first event in the new (copied) chord.
		newpos = CopyChord(vst, tp, olddur - newdur);
		// in chordmode ... this should get to the first event after the chord ....
		GetNext(vst.vpos,vst);
		pos = vst.vpos;
	}
	else {
		// now we create a new event, that is a copy of ev and has a new duration ...
		ARMusicalEvent * ev2 = ARMusicalEvent::cast(ev->Copy());
		if (ev2) {
			ev2->setRelativeTimePosition( tp);
			ev2->setDuration( olddur - newdur);
			newpos = AddElementAfter(newpos,ev2);
		}
		else assert(false);

		// this position remembers the location of the MergeEnd or
		// tieEnd that could be introduced.
		// this is important, so that tie repositioning works ...
		GuidoPos newptagpos = NULL;

        ARNote * n2 = ev2->isARNote();
		// now look, if there are any ties and merge-tags ..
		if (tiemergecount==0 && n2 && !(n2->getName() == ARNoteName::empty) && !n2->isARTab())
		{
			if (mPosTagList == NULL) {
				mPosTagList = createPositionTagList();
				vst.ptagpos = mPosTagList->GetHeadPosition();
			}

			// so we add our tie ...
			ARTie *artie = new ARTie();
			artie->setID((int)gCurArMusic->mMaxTagId++);
			artie->setIsAuto(true);
			artie->setRelativeTimePosition( ev->getRelativeTimePosition());
			artie->setPosition(pos);
			ARDummyRangeEnd * arde = new ARDummyRangeEnd(TIEEND);
			arde->setID(artie->getID());
			arde->setPosition(newpos);
			artie->setCorrespondence(arde);
			arde->setCorrespondence(artie);

			if (vst.ptagpos != NULL) {
				mPosTagList->AddElementAt(vst.ptagpos,artie);
				// the position of this element must be saved!
				newptagpos = mPosTagList->AddElementAt(vst.ptagpos,arde);
			}
			else {
				mPosTagList->AddTail(artie);
				// remember this position!
				newptagpos = mPosTagList->AddTail(arde);
			}

			// add the tag to the currentposition-tags, but don't add it to the
			// addedtaglist ... this would otherwise confuse the beginning of this function.
			vst.AddPositionTag(artie,0);
		}

		// now we need to "repointer" the position tags that formerly were attached to ev -> they
		// now need to be attached to ev2 (which is at newpos)
		GuidoPos tmppos = vst.ptagpos;
		while (tmppos) {
			ARPositionTag * arpt = mPosTagList->GetNext(tmppos);
			ARTagEnd * arte = ARTagEnd::cast(arpt);
			if (arte) {
				if (arte->getPosition() == pos) {
					arte->setPosition(newpos);
					continue;
				}
			}
			break;
		}

		// the positiontagpointer now points on the FLA of ev_2 ...
		// (which is the added tie or merge)
		if (newptagpos != NULL)
			vst.ptagpos = newptagpos;

		// now, the added and removedposition
		// tags are removed ... cannot be of interest ....
		vst.DeleteAddedAndRemovedPTags();

		pos = newpos;
		vst.vpos = pos;
		vst.curtp = ev2->getRelativeTimePosition();
	}
}

// ============================================================================
//		ARTieStruct
// ============================================================================
struct ARTieStruct
{
// public:
	ARTie * tie;
	// origtie is the orignial tie (for autoties)
	// this is needed, so that if the orig tie
	// is closed, all autoties can be removed
	// as well ...
	ARTie * origtie;
	ARChordTag * curchordtag;
	ARNote * startnote;
	ARTieStruct()
	{
		tie = NULL;
		curchordtag = NULL;
		startnote = NULL;
		origtie = NULL;
	}
};

//____________________________________________________________________________________
/** \brief Traverses the voice and checks on the ties.

	All Ties are split, so that there are always TWO events within a tie-Range.

	This facilitates the handling of Ties in a System-Break-Setting
	This also complies with the notion, that all Graphical elements should be represented
	by a single ARObject ...
	The algorithm works as follows: Traverse the voice and keep track of tie-tags.
	whenever a tie-Tag is encountered, the tag (and its position in the mPosTagList
	is saved) and the tielookaheadstate is set. A new tagBegin is build, and added at the current ptagpos.
	then traversal continues.
	if the tielookahead is active, the current new tie-tag is closed (endposition is the current one).
	if the current position is not the former-tag endposition, a new Tie-Tag is build again.
*/
void ARMusicalVoice::doAutoTies()
{
	// we create IDs for the auto-ties ... starting with 100; needs to be elaborated later ...
	// int autotie = 43001;

	// the tielist contains the active ties. if a tie ends, then it is removed
	// from the tielist and also deleted from the voice completely
	KF_IPointerList<ARTieStruct> tiestructlist(1);

	// this list contains those ties that need to be removed after the whole thing has been handled ...
	KF_IPointerList<ARTie> deletelist(0);

	// the autotielist contains the active autoties these ties are created automatically
	// to be build precisely between two events (and not cover a whole range)
	KF_IPointerList<ARTieStruct> autotiestructlist(1);

	_readmode oldreadmode = readmode;

	// we read the voice in eventmode and not in chordmode.
	// the reason for this is: we want to tie chords correctly.
	readmode = EVENTMODE;

	ARMusicalVoiceState vst;
	GetHeadPosition(vst);

	ARChordTag *curchordtag  = vst.curchordtag;
	ARChordTag *prevchordtag = NULL;

	while (vst.vpos) {
		ARMusicalObject *o  = GetAt(vst.vpos);
		ARMusicalEvent  *ev = ARMusicalEvent::cast(o);

		// the ties that are added
		if (vst.addedpositiontags) {
			GuidoPos tmppos = vst.addedpositiontags->GetHeadPosition();

			while (tmppos) {
				ARTie *tie = dynamic_cast<ARTie *>(vst.addedpositiontags->GetNext(tmppos));

				if (tie) {
					ARTieStruct * tiestruct = new ARTieStruct();
					tiestruct->tie = tie;
					tiestruct->curchordtag = vst.curchordtag;
                    tiestruct->startnote = static_cast<ARNote *>(o->isARNote());
					tiestructlist.AddTail(tiestruct);
				}
			}
		}

		// then we check the tiestructlist and add autoties, if needed .
		if (ev) {
			GuidoPos postiestructlist = tiestructlist.GetHeadPosition();
			
            while (postiestructlist) {
				ARTieStruct *tiestruct = tiestructlist.GetNext(postiestructlist);

				// now check, whether this tie should be copied.
				// Questions to ask:
				// Did the original tie have its origin in a chord-event? If we are still in the
				// same chord and have already started an  autotie, then we do not need a new one.

				// test first, if the startnote of the tie is an empty event .... and if the tie began in a chord
				int ischordtie = 0;
				if (tiestruct->curchordtag) {
					ARNote *nt = tiestruct->startnote;
					if (nt && nt->getName() == ARNoteName::empty) // then, we have a tie that covers the whole chord.
						ischordtie = 1;
				}

				int mustcreate = 0;
				// only, if it is not a chordtie!, otherwise
				// I have to create a tie everywhereelse...
				if (!ischordtie && tiestruct->tie->getPosition() == vst.vpos) // this means, the tie originated at the current position.
					mustcreate = 1;                                           // we build a new one...
					                                                          // at least it starts here...
					                                                          // then we have to create a new one...
				else if (vst.curchordtag) {
					// we are inside a chord... then we have to check two things...
					// 1. we are not in the chord that has already been handled by the first case (vst.vpos is equal)
					// 2. the name and position match...
					if (ischordtie && tiestruct->tie->getPosition() != vst.vpos) {
                        ARNote *nt = o->isARNote();
						if (nt && !(nt->getName() == ARNoteName::empty))
							mustcreate = 1;
					}
					else if (tiestruct->curchordtag != vst.curchordtag) {
                        ARNote *nt = o->isARNote();
						if (nt && tiestruct->startnote && nt->CompareNameOctavePitch(*tiestruct->startnote))
							mustcreate = 1;
					}
				}
				else {
					// we are not in a chord...
                    ARNote *nt = o->isARNote();
					if (nt && tiestruct->startnote && nt->CompareNameOctavePitch(*tiestruct->startnote))
						mustcreate = 1;
				}

				if (mustcreate) {
					ARTie *mytie = new ARTie(tiestruct->tie->hideAccidentals());
					mytie->setID((int)gCurArMusic->mMaxTagId++);
					//the tie is set to auto only if it has been generated by do AutoDisplayCheck (note split)
					//we will need this information for the trill
					if (tiestruct->tie->getIsAuto())
						mytie->setIsAuto(true);
					mytie->copyParameters (tiestruct->tie->getTagParameters());
					mytie->setTagParameters(tiestruct->tie->getTagParameters());

					mytie->setStartPosition(vst.vpos);
					mPosTagList->AddElementAt(vst.ptagpos, mytie);

					ARTieStruct *atstruct = new ARTieStruct;
					atstruct->tie = mytie;
					// this is needed so that the tie can remove this autotie when it is closed...
					atstruct->origtie     = tiestruct->tie;
					atstruct->curchordtag = vst.curchordtag;
                    atstruct->startnote = static_cast<ARNote *>(o->isARNote());
					autotiestructlist.AddTail(atstruct);
				}
			// check the first element...
			}

			// here we deal with the events, that need to be closed...
			// then we have to end the autoties that need to be ended...
			GuidoPos mypos = autotiestructlist.GetHeadPosition();
			while (mypos) {
				ARTieStruct * tiestruct = autotiestructlist.GetAt(mypos);
				if (tiestruct->tie->getPosition() == vst.vpos) {
					// his means, we have just created this autotie at the current position...
					autotiestructlist.GetNext(mypos);
					continue;
				}

				int mustclose = 0;
                ARNote *nt = static_cast<ARNote *>(o->isARNote());
                if (vst.curchordtag) {
					if (tiestruct->curchordtag != vst.curchordtag) {
					    // then we are in another chord check, if the name matches...
						if (nt && tiestruct->startnote && nt->CompareNameOctavePitch(*tiestruct->startnote))
							mustclose = 1;
						else {
							// check, whether the autotie is in the correct domain...
							if (tiestruct->curchordtag != prevchordtag) {
								// then we delete it...
								// then, this is considered an error...
								// then we have to remove the autotie from the ptagspos...
								mPosTagList->RemoveElement(tiestruct->tie);
								autotiestructlist.RemoveElementAt(mypos);
								mypos = autotiestructlist.GetHeadPosition();
							}
						}
					}
				}
				else {
					// no chords currently...
					if (nt && tiestruct->startnote && nt->CompareNameOctavePitch(*tiestruct->startnote))
						mustclose = 1;
					else {
						// then, this is considered an error...
						mPosTagList->RemoveElement(tiestruct->tie);
						autotiestructlist.RemoveElementAt(mypos);
						mypos = autotiestructlist.GetHeadPosition();
					}
				}

				if (mustclose) {
					// then close the tie...
					// now we end the tie...
					ARDummyRangeEnd *arde = new ARDummyRangeEnd(TIEEND);
					arde->setID(tiestruct->tie->getID());
					arde->setPosition(vst.vpos);
					tiestruct->tie->setCorrespondence(arde);
					arde->setCorrespondence(tiestruct->tie);

					if (vst.ptagpos)
						mPosTagList->AddElementAt(vst.ptagpos,arde);
					else
						mPosTagList->AddTail(arde);

					// then, this autotie is finished...
					autotiestructlist.RemoveElementAt(mypos);
					mypos = autotiestructlist.GetHeadPosition();
				}
				else {
					// go to the next event...
					if (mypos)
						autotiestructlist.GetNext(mypos);
				}
			}
		}

		// go to the next event...
		GetNext(vst.vpos, vst);
		ARMusicalObject *nexto  = NULL;
		ARMusicalEvent  *nextev = NULL;
		if (vst.vpos) {
			nexto  = GetAt(vst.vpos);
			nextev = ARMusicalEvent::cast(nexto);
		}

		if (nextev && curchordtag != vst.curchordtag) {
			prevchordtag = curchordtag;
			curchordtag  = vst.curchordtag;
		}
		else if (nextev && vst.curchordtag == NULL)
			prevchordtag = curchordtag;

		// those that are removed
		if (vst.removedpositiontags) {
			GuidoPos tmppos = vst.removedpositiontags->GetHeadPosition();
			
            while (tmppos) {
				ARTie *tie = dynamic_cast<ARTie *>(vst.removedpositiontags->GetNext(tmppos));
				if (tie) {
					// remove the tie from the tielist...
					// find the element in the tiestructlist...
					GuidoPos postiestructlist = tiestructlist.GetHeadPosition();

					while (postiestructlist) {
						ARTieStruct *tiestruct = tiestructlist.GetAt(postiestructlist);
						if (tiestruct->tie == tie)
							break;
						tiestructlist.GetNext(postiestructlist);
					}

					if (postiestructlist) {
						// then we have to find the ones in the autostructlist and remove them...
						ARTieStruct *atstruct = NULL;
                        do {
							atstruct = NULL;
							GuidoPos mypos = autotiestructlist.GetHeadPosition();
							while (mypos) {
								atstruct = autotiestructlist.GetAt(mypos);
								if (atstruct->origtie == tie)
									break;
								autotiestructlist.GetNext(mypos);
							}

							if (mypos) {
								assert(atstruct);
								// remove this one (and the already started autotie...)
								mPosTagList->RemoveElement(atstruct->tie);
								autotiestructlist.RemoveElementAt(mypos);
							}
							else atstruct = NULL;
						}
						while (atstruct);
						tiestructlist.RemoveElementAt(postiestructlist);
					}
					deletelist.AddTail(tie);
				}
			}
		}
	} // while (vst.vpos) -> the whole voice.

	if (deletelist.GetCount() > 0) {
		GuidoPos pos = deletelist.GetHeadPosition();
		while (pos) {
			ARTie *tie = deletelist.GetNext(pos);
			// then, we delete this tie out of the positiontags!
			if (tie->getCorrespondence()) // the list owns this one...
				mPosTagList->RemoveElement(tie->getCorrespondence());
			// the mPosTagList owns the ties!
			mPosTagList->RemoveElement(tie);
		}
	}

	readmode = oldreadmode;
	// now we have the EndState ...
	delete endState;
	endState = new ARMusicalVoiceState(vst);
}

//------------------------------------------------------------------------------------
//doAutoGlissando...


struct ARGlissandoStruct
{
// public:
	ARGlissando * glissando;
	ARGlissando * originGlissando;
	ARChordTag * curchordtag;
	ARNote * startnote;
	ARGlissandoStruct()
	{
		glissando = NULL;
		curchordtag = NULL;
		startnote = NULL;
		originGlissando = NULL;
	}
};


void ARMusicalVoice::doAutoGlissando()
{
	KF_IPointerList<ARGlissandoStruct> glissStructlist(1);
	KF_IPointerList<ARGlissando> deletelist(0);
	KF_IPointerList<ARGlissandoStruct> autoglissStructlist(1);

	_readmode oldreadmode = readmode;
	readmode = EVENTMODE;

	ARMusicalVoiceState vst;
	GetHeadPosition(vst);

	ARChordTag * curchordtag = vst.curchordtag;
	ARChordTag * prevchordtag = NULL;

	//we're gonna look at the whole voice
	while (vst.vpos)
	{
		//for each position within the voice, we look for a glissando in the list addedpositiontags, and create a structure for each one
		ARMusicalObject * o = GetAt(vst.vpos);
        ARNote * note = static_cast<ARNote *>(o->isARNote());

		if (vst.addedpositiontags)
		{
			GuidoPos tmppos = vst.addedpositiontags->GetHeadPosition();
			while (tmppos)
			{
				ARGlissando * glissando = dynamic_cast<ARGlissando *>(vst.addedpositiontags->GetNext(tmppos));
				if (glissando)
				{
					ARGlissandoStruct * glissStruct = new ARGlissandoStruct();
					glissStruct->glissando = glissando;
					glissStruct->curchordtag = vst.curchordtag;
					glissStruct->startnote = note;
					glissStructlist.AddTail(glissStruct);
				}
			}
		}
		// for each position within the voice, we look for a note and check if it is affected by a glissando
		if (note)
		{
			GuidoPos posGlissStructlist = glissStructlist.GetHeadPosition();
			while (posGlissStructlist)
			{
				ARGlissandoStruct *glissStruct = glissStructlist.GetNext(posGlissStructlist);
				
				bool isTied = false;
				if(vst.curpositiontags)
				{
					GuidoPos tmppos = vst.curpositiontags->GetHeadPosition();
					while (tmppos)
					{
						ARTie * tie = dynamic_cast<ARTie *>(vst.curpositiontags->GetNext(tmppos));
						if (tie && tie->getStartPosition() == vst.vpos)
						{
							isTied = true;
						}
					}
				}
				// we create a glissando for each note (not empty, and not tied) within the origin glissando
				if (note->getName() != ARNoteName::empty && !isTied)
				{
					ARGlissando * myglissando = new ARGlissando(glissStruct->glissando);
					myglissando->setID((int)gCurArMusic->mMaxTagId++);
					myglissando->setIsAuto(true);
					myglissando->setStartPosition(vst.vpos);

					mPosTagList->AddElementAt(vst.ptagpos,myglissando);

					ARGlissandoStruct * agstruct = new ARGlissandoStruct;
					agstruct->glissando = myglissando;
					
					agstruct->originGlissando = glissStruct->glissando;
					agstruct->curchordtag = vst.curchordtag;
					agstruct->startnote = note;
					
					autoglissStructlist.AddTail(agstruct);
				}
			}

			// we know look at the list of glissando structures that we've created
			GuidoPos mypos = autoglissStructlist.GetHeadPosition();
			while (mypos)
			{
				ARGlissandoStruct * glissStruct = autoglissStructlist.GetAt(mypos);

				if (glissStruct->glissando->getPosition() == vst.vpos)
				{
					autoglissStructlist.GetNext(mypos);
					continue;
				}
				
				bool isTied = false;
				if(vst.curpositiontags) {
					GuidoPos tmppos = vst.curpositiontags->GetHeadPosition();
					while (tmppos) {
						ARTie * tie = dynamic_cast<ARTie *>(vst.curpositiontags->GetNext(tmppos));
						if (tie && tie->getEndPosition() == vst.vpos)
							isTied = true;
					}
				}

				if (glissStruct->curchordtag == prevchordtag && (glissStruct->startnote->getRelativeEndTimePosition() <= note->getRelativeTimePosition()))
				{
					if (!note->isEmptyNote() && (!note->CompareNameOctavePitch(*glissStruct->startnote) ||
						(note->CompareNameOctavePitch(*glissStruct->startnote) && note->getDetune() != glissStruct->startnote->getDetune()))
						&& !isTied)
					{
						ARDummyRangeEnd * arde = new ARDummyRangeEnd(GLISSANDOEND);
						arde->setID(glissStruct->glissando->getID());
						arde->setPosition(vst.vpos);
						glissStruct->glissando->setCorrespondence(arde);
						arde->setCorrespondence(glissStruct->glissando);

						if (vst.ptagpos)
							mPosTagList->AddElementAt(vst.ptagpos,arde);
						else
							mPosTagList->AddTail(arde);
						
						autoglissStructlist.RemoveElementAt(mypos);
						mypos = autoglissStructlist.GetHeadPosition();
						break;
					}
					else if (glissStruct->curchordtag != prevchordtag || isTied)
					{
						mPosTagList->RemoveElement(glissStruct->glissando);
						autoglissStructlist.RemoveElementAt(mypos);
						mypos = autoglissStructlist.GetHeadPosition();
					}
					else
					{
						// go to the next event...
						if (mypos)
							autoglissStructlist.GetNext(mypos);
					}
				}
				else
				{
					// go to the next event...
					if (mypos)
						autoglissStructlist.GetNext(mypos);
				}
			}
		}

		// go to the next event...
		GetNext(vst.vpos,vst);

		ARMusicalObject * nexto = NULL;
		ARNote * nextnote  = NULL;

		if (vst.vpos)
		{
			nexto = GetAt(vst.vpos);
            nextnote = static_cast<ARNote *>(nexto->isARNote());
		}

		if (nextnote && curchordtag != vst.curchordtag)
		{
			prevchordtag = curchordtag;
			curchordtag = vst.curchordtag;
		}
		else if (nextnote && vst.curchordtag == NULL)
		{
			prevchordtag = curchordtag;
		}
		
		if (vst.removedpositiontags)
		{
			GuidoPos tmppos = vst.removedpositiontags->GetHeadPosition();
			while (tmppos)
			{
				ARGlissando * glissando = dynamic_cast<ARGlissando *>
					(vst.removedpositiontags->GetNext(tmppos));
				if (glissando)
				{
					GuidoPos posGlissStructlist = glissStructlist.GetHeadPosition();
					while (posGlissStructlist)
					{
						ARGlissandoStruct *glissStruct = glissStructlist.GetAt(
							posGlissStructlist);
						if (glissStruct->glissando == glissando)
							break;
						glissStructlist.GetNext(posGlissStructlist);
					}

					if (posGlissStructlist)
					{
						ARGlissandoStruct *agstruct = NULL;
						do
						{
							agstruct = NULL;
							GuidoPos mypos = autoglissStructlist.GetHeadPosition();

							while (mypos)
							{
								agstruct = autoglissStructlist.GetAt(mypos);
								if (agstruct->originGlissando == glissando)
								{
									break;
								}
								autoglissStructlist.GetNext(mypos);
							}
							if (mypos)
							{
								assert(agstruct);
								mPosTagList->RemoveElement(agstruct->glissando);
								autoglissStructlist.RemoveElementAt(mypos);
							}
							else
								agstruct = NULL;
						}
						while (agstruct);

						glissStructlist.RemoveElementAt(posGlissStructlist);
					}
					deletelist.AddTail(glissando);

				}
			}
		}

	} // while (vst.vpos) -> the whole voice.

	if (deletelist.GetCount() > 0)
	{
		GuidoPos pos = deletelist.GetHeadPosition();
		while (pos)
		{
			ARGlissando * glissando = deletelist.GetNext(pos);

			if (glissando->getCorrespondence())
			{
				// the list owns this one...
				mPosTagList->RemoveElement(glissando->getCorrespondence());
			}

			// the mPosTagList owns the ties!
			mPosTagList->RemoveElement(glissando);
		}
	}

	readmode = oldreadmode;

	// now we have the EndState ...
	delete endState;
	endState = new ARMusicalVoiceState(vst);
}


//____________________________________________________________________________________
/** \brief (not implemented) Traverses the voice and breaks Fermata-Tags
	so that the range just covers a single note.

	In this way, fermatas no longer need
	to take care of dealing with more than
	one note...
*/
void ARMusicalVoice::doAutoFermatas()
{
}

//____________________________________________________________________________________
/** \brief Goes through the music and looks at key-changes.

	whenever a key-change (change!) occurs, the previous key needs to be naturalized
	which is done, by putting a /natkey infront of the key. This naturalizes
	any key that is present.
*/
void ARMusicalVoice::doAutoKeys()
{
	int currentKey = 0;
	GuidoPos pos = ObjectList::GetHeadPosition();
	while (pos) {
        ARMusicalObject *obj = GetAt(pos);

        if (obj) {
		   ARKey * key = static_cast<ARKey *>(obj->isARKey());
           if (key) {
				int keyn = key->getKeyNumber();
				bool showNaturals = currentKey;
				if (key->isHideAutoNaturalsSet()) showNaturals = !key->hideAutoNaturals();
				else {
					showNaturals = currentKey &&		// don't show naturals if there was none
								// don't show naturals when key increases or decreases in the same direction
								!(((currentKey > 0) && (keyn > 0)) || ((currentKey < 0) && (keyn < 0)));
								// don't show naturals when key increases in the same direction
//								!(((currentKey > 0) && (keyn > 0) && (currentKey <= keyn))
//								|| ((currentKey < 0) && (keyn < 0) && (currentKey >= keyn)));
				}
                if (showNaturals) {
                    // then we insert a key at this position
					ARNaturalKey * natkey = new ARNaturalKey();
					natkey->setIsAuto(true);
					natkey->setRelativeTimePosition(key->getRelativeTimePosition());
					AddElementAt(pos,natkey);
                }
                currentKey = key->getKeyNumber();
            }
        }
        ObjectList::GetNext(pos);
	}
}


//____________________________________________________________________________________
/** \brief Goes through the music and checks order of key changes and possible breaks
 */
void ARMusicalVoice::checkKeys()
{
    GuidoPos pos = ObjectList::GetHeadPosition();
    while (pos) {
        ARMusicalObject *obj = GetAt(pos);
        
        if (obj) {
            ARPossibleBreak *possibleBreak = static_cast<ARPossibleBreak *>(obj->isARPossibleBreak());
            
            if (possibleBreak)
            {
                mynode *breakPos = (mynode *) pos;
                // jfk, this is a bit hacky!
                // if ARPossibleBreak is followed by one or more ARKey, we change the order
                GuidoPos nextPos = breakPos->fNext;
                if( nextPos ){
                    
                    ARMusicalObject *key;
                    do{
                        ARMusicalObject *follower = GetAt(nextPos);
                        key = static_cast<ARKey *>(follower->isARKey());
                        if( !key ){
                            key = static_cast<ARNaturalKey *>(follower->isARNaturalKey());
                        }
                        if( key &&
                           possibleBreak->getRelativeTimePosition() == key->getRelativeTimePosition() ){
                            /** jfk
                             swap possibleBreak and key
                             **/
                            breakPos->fPrev->fNext = (mynode *) nextPos;
                            
                            ((mynode *)nextPos)->fPrev = breakPos->fPrev;
                            
                            breakPos->fPrev = (mynode *) nextPos;
                            breakPos->fNext =  ((mynode *)nextPos)->fNext;
                            
                            ((mynode *)nextPos)->fNext = breakPos;
                            nextPos = breakPos->fNext;

                        }
                        else{
                            // stop the loop
                            key = NULL;
                        }
                    }while( key );
                }
            }
        }
        ObjectList::GetNext(pos);
    }
}


//____________________________________________________________________________________
int ARMusicalVoice::removeTag(ARMusicalObject * obj)
{
	GuidoPos pos = GetTailPosition();

	bool found = false;
	while (pos)
	{
		ARMusicalObject * tmp = GetAt(pos);
		if (tmp == obj)
		{
			ownselements = 0;
			RemoveElementAt(pos);
			ownselements = 1;
			found = true;
			break;
		}
		GetPrev(pos);
	}

	ARPositionTag * ptag = dynamic_cast<ARPositionTag *>(obj);
	if (ptag)
		mCurVoiceState->RemovePositionTag(ptag);
	return found;
}

//____________________________________________________________________________________
void ARMusicalVoice::removeAutoTags()
{
	if (mPosTagList)
    {
		GuidoPos pos = mPosTagList->GetHeadPosition();
		while (pos)
		{
			GuidoPos tmppos = pos;
			ARMusicalTag * tg = dynamic_cast<ARMusicalTag *>(mPosTagList->GetNext(pos));
			if (tg && tg->getIsAuto())
			{
				mPosTagList->RemoveElementAt(tmppos);
			}
		}
    }

	GuidoPos pos = GetHeadPosition();

	while (pos) {
		GuidoPos tmppos = pos;
        ARMusicalObject *obj = GetNextObject(pos);

        if (obj) {
            const ARMusicalTag *tg = static_cast<const ARMusicalTag *>(obj->isARMusicalTag());

            if (tg && tg->getIsAuto())
                RemoveElementAt(tmppos);
        }
    }
}

//____________________________________________________________________________________
/** This routine must remember the state of the
	voice (including ptagpos...)
*/
// C.D. optimized 22/10/14
ARChordTag *ARMusicalVoice::BeginChord()
{
	// we create the empty event first with duration_0
	// we add the chord-tag...

	currentChord = new ARChordTag();
	currentChord->setIsAuto(true);
	AddPositionTag(currentChord);
	currentShareLocation = new ARShareLocation();
	currentShareLocation->setIsAuto(true);
	AddPositionTag(currentShareLocation);

	// this is the first event
	ARNote *tmp     = new ARNote("empty", 0, 0, 0, 1, 80);
	tmp->setAuto(true);
	posfirstinchord = AddTail(tmp);
	numchordvoice   = 0;

    isInChord = true;
    
    return currentChord;
}

//____________________________________________________________________________________
/** \brief this method is called when the chord is an ornament parameter (SB)
*/
// C.D. optimized 22/10/2014
vector<ARNote *> ARMusicalVoice::getCurrentChordNotes () const
{	
	vector<ARNote *> outNotes;
	GuidoPos pos = mCurVoiceState->vpos;
	for (int i = 0 ; i < 3; i++)
		ObjectList::GetNext(pos); // skip "empty, chordcomma, empty"

	while (pos) {
		ARMusicalObject *obj = ObjectList::GetNext(pos);
		ARNote * note = static_cast<ARNote *>(obj->isARNote());
		if (note && !note->isEmptyNote()) outNotes.push_back(note);
	}
	return outNotes;
}

//------------------------------------------------------------------------
// propagate trills inside a chord (lowest and highest notes only)
// only trills are concerned (do nothing for turn and mordent)
//------------------------------------------------------------------------
void ARMusicalVoice::finishTrilledChord ()
{	
	vector<ARNote *> notes = getCurrentChordNotes();
	auto sortbypitch = [] (const ARNote* n1, const ARNote* n2) -> bool { return n1->getMidiPitch() > n2->getMidiPitch(); };
	// sort notes by descending pitch
	std::sort (notes.begin(), notes.end(), sortbypitch);
	size_t n = notes.size();
	const ARTrill*  trill = (n > 1) ? notes[0]->getOrnament() : 0;
	for (size_t i=1; i < n; i++)	// preserve the first ornament (higher pitch) and remove the others
		notes[i]->setOrnament(0);
	if (trill && (trill->getOrnament() == ARTrill::TRILL)) {	// and for trill only
		ARTrill* copy = new ARTrill(-1, trill);
		copy->setTrillPos (ARTrill::BELOW);
		notes[n-1]->setOrnament (copy);				// set a trill to the lowest note
	}
}

//____________________________________________________________________________________
/** \brief this method is about clusters
*/
void ARMusicalVoice::setClusterChord(ARCluster *inCurrentCluster)
{
    if (!isInChord)
        return;

	GuidoPos posTmp = mCurVoiceState->vpos;

    for(int i = 0 ; i < 3 ; i++) {
        ObjectList::GetNext(posTmp); // skip "empty, chordcomma, empty"

        if (!posTmp)
            return;
    }

	int comptTemp = 0;
	ARMusicalObject * musicalObject = ObjectList::GetNext(posTmp);
    ARNote *firstNote = static_cast<ARNote *>(musicalObject->isARNote());

    ARCluster *currentCluster = firstNote->setCluster(inCurrentCluster, true, true);
    currentCluster->setARNote(firstNote);

    comptTemp++; // the first note is conserved

    bool isThereASecondNote = false;

    while (posTmp && comptTemp < 2) { // we only need to know the first 2 notes
        musicalObject = ObjectList::GetNext(posTmp);

        ARNote * noteTmp = static_cast<ARNote *>(musicalObject->isARNote());

        if (noteTmp && noteTmp->getPitch() != 0) {
            currentCluster->setARNote(noteTmp);
            noteTmp->setCluster(currentCluster);
            noteTmp->enableSubElements(false);

            isThereASecondNote = true;

            comptTemp++;
        }
    }

    //Other notes deletion
    while (posTmp) { // we only need to know the first 2 notes
        musicalObject = ObjectList::GetNext(posTmp);

        ARNote * noteTmp = static_cast<ARNote *>(musicalObject->isARNote());

        if (noteTmp && noteTmp->getPitch() != 0) {
            currentCluster->setARNote(noteTmp);
            noteTmp->setPitch(firstNote->getPitch()); // "hides" this note behind the first one
            noteTmp->setOctave(firstNote->getOctave());
            noteTmp->setCluster(currentCluster);
            noteTmp->setDrawGR(false);
        }
    }

    if (!isThereASecondNote) {
        currentCluster->setOnlyOneNoteInCluster();
        currentCluster->setARNote(firstNote);
        firstNote->setIsLonelyInCluster();
        firstNote->setCluster(currentCluster);
    }
}

//____________________________________________________________________________________
/** \brief this finished a chord after parsing it.
	it involves setting all the event-times and stuff !?
*/
// C.D. method optimized 22/10/14
void ARMusicalVoice::FinishChord(bool regular)
{
    if (!currentChord)
        return;

	TYPE_DURATION chorddur;

	_readmode oldreadmode = readmode;
	readmode = EVENTMODE;

	int onlyonegroup = 0;

	// now we have to go through the grouplist...
    if (chordgrouplist) {
        GuidoPos ptagposBackup = mCurVoiceState->ptagpos;

        /* Separated for performance improvement */
        if (chordgrouplist->GetCount() == 1)
            finishChordWithOneChordGroup(chorddur, regular);
        else {
            GuidoPos vposBackup = mCurVoiceState->vpos;
            finishChordWithSeveralChordGroups(chorddur, regular);
            mCurVoiceState->vpos = vposBackup;
        }
        mCurVoiceState->ptagpos = ptagposBackup;
    }

	// now we have to traverse the chord to see that we set the timepositions and also
	// remember the lastchordpos and ptagpos for adding the tags that end after the chord...

	// now I insert one more empty event...
	ARNote *tmpnote = new ARNote("empty", 0, 1, 0, 1, 80);
	tmpnote->setAuto(true);
	if (chordgrouplist) {
		ARChordGroup *tmp = chordgrouplist->GetTail();
		if (tmp) tmpnote->setDuration(tmp->dur);
	}

	AddTail(tmpnote);

	ARDummyRangeEnd * dummy = new ARDummyRangeEnd("\\shareLocationEnd");
	dummy->setIsAuto(true);
	dummy->setPosition(lastevposition);
	dummy->setCorrespondence(currentShareLocation);
	dummy->setAssociation(ARMusicalTag::LA);
	currentShareLocation->setCorrespondence(dummy);

	mPosTagList->AddTail(dummy);

	dummy = new ARDummyRangeEnd("\\autoChordEnd");
	dummy->setIsAuto(true);
	dummy->setPosition(lastevposition);
	dummy->setCorrespondence(currentChord);
	dummy->setAssociation(ARMusicalTag::LA);
	currentChord->setCorrespondence(dummy);
	dummy->setCorrespondence(currentChord);

	mPosTagList->AddTail(dummy);

	// now we have to traverse the voice once more to set the timepositions correctly
	TYPE_TIMEPOSITION starttp(mCurVoiceState->curtp);
    TYPE_TIMEPOSITION newtp(starttp + chorddur);
	int firstevent = 0;

	mPosTagList->GetNext(mCurVoiceState->ptagpos);

    if (onlyonegroup) {
        // we have to move the mPosTagList two more...
        // what about adding the correponding tags (at least to the curpositiontags...)?
        ARPositionTag *ptag = mPosTagList->GetNext(mCurVoiceState->ptagpos);
        mCurVoiceState->curpositiontags->AddTail(ptag);
        ptag = mPosTagList->GetNext(mCurVoiceState->ptagpos);
        mCurVoiceState->curpositiontags->AddTail(ptag);
    }

	while (mCurVoiceState->vpos && mCurVoiceState->curchordtag == currentChord) {
		ARMusicalObject *o  = GetNext(mCurVoiceState->vpos, *mCurVoiceState);
		ARMusicalEvent  *ev = ARMusicalEvent::cast(o);

		if (mCurVoiceState->addedpositiontags) {
			TYPE_TIMEPOSITION mytp;

			if (firstevent)
				mytp = newtp;
			else
				mytp = starttp;

			GuidoPos mypos = mCurVoiceState->addedpositiontags->GetHeadPosition();
			while (mypos) {
				ARPositionTag * ptag = mCurVoiceState->addedpositiontags->GetNext(mypos);
				if (ptag) {
					ARMusicalObject * po = dynamic_cast<ARMusicalObject *>(ptag);
					if (po) po->setRelativeTimePosition(mytp);
				}
			}
		}

		if (!firstevent && ev) {
			// we have our first event...
			firstevent = 1;
			ev->setDuration(chorddur);
		}
		else o->setRelativeTimePosition(starttp); 

		if (mCurVoiceState->removedpositiontags) {
			TYPE_TIMEPOSITION mytp;

			if (firstevent)
				mytp = newtp;
			else
				mytp = starttp;

			GuidoPos mypos = mCurVoiceState->removedpositiontags->GetHeadPosition();

			while (mypos) {
				ARPositionTag *ptag = mCurVoiceState->removedpositiontags->GetNext(mypos);

				if (ptag) {
					ARTagEnd *tgend = ARTagEnd::cast(ptag->getCorrespondence());
                    if (tgend) {
						ARMusicalObject *po = tgend;
						if (po) po->setRelativeTimePosition(mytp);
					}
				}
			}
		}
	}

	TYPE_DURATION d = getDuration();
	d += chorddur;
	setDuration( d );

	mCurVoiceState->RemovePositionTag(currentChord);
	mCurVoiceState->RemovePositionTag(currentShareLocation);
	currentChord         = NULL;
	currentShareLocation = NULL;

    isInChord = false;

	delete chordgrouplist;
	chordgrouplist = NULL;

	readmode        = oldreadmode;
	posfirstinchord = NULL;
	numchordvoice   = -1;
}

//____________________________________________________________________________________
/** \brief Called by FinishChord(), to finish chords which have only one chord group. This function
           has been implemented for optimization.
*/
void ARMusicalVoice::finishChordWithOneChordGroup(TYPE_DURATION &chorddur, bool regular)
{
    mPosTagList->GetNext(mCurVoiceState->ptagpos);
    ARChordGroup *group = chordgrouplist->GetAt(chordgrouplist->GetHeadPosition());

    if (group->dur > chorddur) chorddur = group->dur;

    // then we have to delete the one empty-event that is not needed any longer...
    RemoveElementAt(group->startpos);
    group->startpos = mCurVoiceState->vpos;

    // now we have to add the sharestem and dispdur-tags to the group...

    // we have to add the dispdur-tag only,
    // if there is not one already present because of grace-notes, example :
    // \grace( { e/8,g } ) c/4
    // how do we know ?
    ARDisplayDuration *dispdur = new ARDisplayDuration();
    ARDummyRangeEnd   *dispdum = new ARDummyRangeEnd(DISPDUREND);

    const PositionTagList *list = mCurVoiceState->getCurPositionTags();

    GuidoPos tagpos = list->GetHeadPosition();
    ARTremolo *trem = NULL;

    while (tagpos && !trem)
        trem = dynamic_cast<ARTremolo*>(list->GetNext(tagpos));

    if (mCurVoiceState->fCurdispdur && trem && trem->isPitched())
        dispdur->setDisplayDuration(mCurVoiceState->fCurdispdur->getDisplayDuration());
    else
        dispdur->setDisplayDuration(group->dur);

    dispdur->setIsAuto(true);
    dispdur->setAssociation(ARMusicalTag::RA);
    dispdur->setPosition(group->startpos);

    dispdum->setIsAuto(true);
    dispdum->setPosition(group->endpos);
    dispdum->setCorrespondence(dispdur);
    dispdum->setAssociation(ARMusicalTag::LA);
    dispdur->setCorrespondence(dispdum);

	if (dispdur) {
		if (mCurVoiceState->ptagpos)
			mPosTagList->AddElementAt(mCurVoiceState->ptagpos, dispdur);
		else
			mPosTagList->AddTail(dispdur);
	}
	ARShareStem * shrstem = regular ? createSharedStem(group) : nullptr;
	if (shrstem) {
		if (mCurVoiceState->ptagpos)
			mPosTagList->AddElementAt(mCurVoiceState->ptagpos, shrstem);
		else
			mPosTagList->AddTail(shrstem);
		mPosTagList->AddTail(shrstem->getCorrespondence());
	}
    if (dispdum) mPosTagList->AddTail(dispdum);
}

//____________________________________________________________________________________
ARShareStem * ARMusicalVoice::createSharedStem(ARChordGroup* group)
{
	ARShareStem     *shrstem = new ARShareStem();
	ARDummyRangeEnd *shrdum = new ARDummyRangeEnd(SHARESTEMEND);
	shrstem->setIsAuto(true);
	shrstem->setPosition(group->startpos);
	shrstem->setAssociation(ARMusicalTag::RA);
	shrdum->setIsAuto(true);
	shrdum->setPosition(group->endpos);
	shrdum->setCorrespondence(shrstem);
	shrdum->setAssociation(ARMusicalTag::LA);
	shrstem->setCorrespondence(shrdum);
	return shrstem;
}

//____________________________________________________________________________________
/** \brief Called by FinishChord(), to finish chords which have several chord groups. This function
           has been implemented for optimization.
*/
void ARMusicalVoice::finishChordWithSeveralChordGroups(TYPE_DURATION &chorddur, bool regular) {
    mPosTagList->GetNext(mCurVoiceState->ptagpos);

    GuidoPos pos = chordgrouplist->GetHeadPosition();

    while (pos) {
        ARChordGroup *group = chordgrouplist->GetNext(pos);

        if (group->dur > chorddur)
            chorddur = group->dur;

        // now we have to add the sharestem and dispdur-tags to the group...

        // we have to add the dispdur-tag only,
        // if there is not one already present because of grace-notes, example :
        // \grace( { e/8,g } ) c/4
        // how do we know ?
        ARDisplayDuration *dispdur = new ARDisplayDuration();
        ARDummyRangeEnd   *dispdum = new ARDummyRangeEnd(DISPDUREND);

        const PositionTagList* list = mCurVoiceState->getCurPositionTags();

        GuidoPos tagpos = list->GetHeadPosition();
        ARTremolo * trem = NULL;

        while(tagpos && !trem)
            trem = dynamic_cast<ARTremolo*>(list->GetNext(tagpos));

        if (mCurVoiceState->fCurdispdur && trem)
            dispdur->setDisplayDuration(mCurVoiceState->fCurdispdur->getDisplayDuration());
        else
            dispdur->setDisplayDuration(group->dur);

        dispdur->setIsAuto(true);
        dispdur->setAssociation(ARMusicalTag::RA);
        dispdur->setPosition(group->startpos);
        dispdum->setIsAuto(true);
        dispdum->setPosition(group->endpos);
        dispdum->setCorrespondence(dispdur);
        dispdum->setAssociation(ARMusicalTag::LA);
        dispdur->setCorrespondence(dispdum);

		ARShareStem * shrstem = regular ? createSharedStem(group) : nullptr;

        while (mCurVoiceState->vpos && mCurVoiceState->vpos != group->startpos)
            GetNext(mCurVoiceState->vpos, *mCurVoiceState);

        if (mCurVoiceState->ptagpos) {
            if (dispdur)
                mPosTagList->AddElementAt(mCurVoiceState->ptagpos, dispdur);
			if (shrstem) mPosTagList->AddElementAt(mCurVoiceState->ptagpos, shrstem);

            // this adds the tags to the curpositiontags
            // this is needed, because we traverse the voice later (GetNext) and then,
            // these tags must be in the curpositiontag list, so that the corresponding remove
            // tags can be removed correctly.
            if (dispdur)
                mCurVoiceState->AddPositionTag(dispdur, 0);

            if (shrstem) mCurVoiceState->AddPositionTag(shrstem, 0);
        }
        else {
            if (dispdur) mPosTagList->AddTail(dispdur);
            if (shrstem) mPosTagList->AddTail(shrstem);
        }

        if (pos != NULL) {
            while (mCurVoiceState->vpos && mCurVoiceState->vpos != group->endpos)
                GetNext(mCurVoiceState->vpos, *mCurVoiceState);
        }

        if (shrstem) mPosTagList->AddTail(shrstem->getCorrespondence());
        if (dispdum) mPosTagList->AddTail(dispdum);
    }
}

//____________________________________________________________________________________
/** \brief Called when a new chord note is started (that is
	 a commata is parsed within a chord.
*/
void ARMusicalVoice::initChordNote()
{
	// this call can be used to work on the inner chord-data structure...

	// AddTail is dependant on numchordvoice
	// (if numchordvoice == 1, then the tags are added BEFORE the first empty event of the chord.
	// therefore, I have to temporarilly set numchordvoice to zero, so that ARChordComma
	// is added at the correct position.
	int oldnumchordvoice = numchordvoice;

	numchordvoice = 0;
	ARChordComma * tmp = new ARChordComma();
	AddTail(tmp);
	numchordvoice = oldnumchordvoice;
}

//____________________________________________________________________________________
GuidoPos ARMusicalVoice::getPositionTagPos (const ARPositionTag *tag) const
{
	return mPosTagList->GetElementPos(tag);
}

//____________________________________________________________________________________
void ARMusicalVoice::MarkVoice(float from, float length, unsigned char red, unsigned char green, unsigned char blue)
{
	TYPE_TIMEPOSITION tpos(from);
	TYPE_DURATION duration(length);

	MarkVoice( tpos.getNumerator(), tpos.getDenominator(), duration.getNumerator(), duration.getDenominator(), red, green, blue);
}

//____________________________________________________________________________________
void ARMusicalVoice::MarkVoice( int fromnum, int fromdenom, int lengthnum, int lengthdenom, unsigned char red, unsigned char green, unsigned char blue)
{
	TYPE_TIMEPOSITION tpos(fromnum,fromdenom);
	TYPE_DURATION duration(lengthnum,lengthdenom);

	TYPE_TIMEPOSITION endtpos (tpos + duration);

	// now we traverse the voice and try to find the indicated timepositions
	ARMusicalVoiceState vst;

	GetHeadPosition(vst);

	GuidoPos startpos = NULL;
	GuidoPos endpos = NULL;

	while (vst.vpos)
	{
		GuidoPos prevpos = vst.vpos;
		// GuidoPos ptagpos = vst.ptagpos;
		const TYPE_TIMEPOSITION tp (vst.curtp);
		ARMusicalObject * o = GetNext(vst.vpos,vst);
		ARMusicalObject * oEv = ARMusicalEvent::cast(o);

		if (!startpos && oEv && tp == tpos)
		{
			// found the beginning...(?)
			startpos = prevpos;
		}

		if (startpos && oEv && vst.curtp == endtpos)
		{
			endpos = prevpos;
			break;
		}
	}
	// now we have the startpos and the endpos
	// to introduce a noteFormat-tag...
	if (startpos)
	{
		ARNoteFormat * ntformat = new ARNoteFormat;
		ntformat->setRelativeTimePosition(tpos);
		ntformat->setRGBColor(red, green, blue);
		AddElementAt(startpos,ntformat);

		if (endpos) {
			ntformat = new ARNoteFormat;
			ntformat->setRelativeTimePosition(endtpos);
			AddElementAfter(endpos,ntformat);
		}
	}
}

//____________________________________________________________________________________
void ARMusicalVoice::doAutoCluster()
{
	// We first look for each note and check if it has a cluster
	ARMusicalVoiceState armvs;
	GuidoPos posObj = GetHeadPosition(armvs);
    ARCluster *tmpCluster  = NULL;
    ARCluster *tmpCluster2 = NULL;

	while (posObj) {
        ARMusicalObject *obj  = GetNext(posObj, armvs);
		ARNote          *note = static_cast<ARNote *>(obj->isARNote());

		if (note) {
			ARCluster *cluster = note->getARCluster();

			if (cluster) {
				// if it has, we can check if the note is tied to another
				// if it is tied, we'll affect an ARCluster to the next note
				const PositionTagList *ptags = armvs.getCurPositionTags();

				if (ptags) {
					GuidoPos pos = ptags->GetHeadPosition();
					while (pos) {
						ARPositionTag *arpt = ptags->GetNext(pos);

						if (arpt) {
							ARTie * tie = dynamic_cast<ARTie *>(arpt);

                            if(tie) {
                                GuidoPos posNote = posObj;
                                ARNote * nextNote;

                                int clusterNoteNumber = cluster->getNoteCount();

                                tmpCluster = note->setCluster(cluster, true, true);

                                if (clusterNoteNumber > 1) {
                                    int currentNoteNumber = 2;

                                    do {
                                        ARMusicalObject *nextObject = GetNext(posNote, armvs);
                                        nextNote = static_cast<ARNote *>(nextObject->isARNote());
                                    }
                                    while(!nextNote || nextNote->getPitch() == 0);

                                    nextNote->setCluster(tmpCluster);

                                    do {
                                        ARMusicalObject *nextObject = GetNext(posNote, armvs);
                                        note = static_cast<ARNote *>(nextObject->isARNote());

                                        if (note && note->getPitch() != 0) {
                                            currentNoteNumber++;

                                            if (currentNoteNumber <= clusterNoteNumber)
                                                note->setCluster(tmpCluster);
                                        }
                                    }
                                    while(!note || note->getPitch() == 0 || currentNoteNumber <= clusterNoteNumber);

                                    do {
                                        ARMusicalObject *nextObject = GetNext(posNote, armvs);
                                        nextNote = static_cast<ARNote *>(nextObject->isARNote());
                                    }
                                    while(!nextNote || nextNote->getPitch() == 0);

                                    currentNoteNumber++;

                                    tmpCluster2 = note->setCluster(cluster, true, true);
                                    nextNote->setCluster(tmpCluster2);
                                    nextNote->enableSubElements(false);

                                    if (clusterNoteNumber > 2) {
                                        do
                                        {
                                            ARMusicalObject *nextObject = GetNext(posNote, armvs);
                                            note = static_cast<ARNote *>(nextObject->isARNote());

                                            if (note && note->getPitch() != 0) {
                                                currentNoteNumber++;

                                                if (currentNoteNumber <= 2 * clusterNoteNumber) {
                                                    note->setCluster(tmpCluster2);
                                                    note->enableSubElements(false);
                                                }
                                            }
                                        }
                                        while(!note || note->getPitch() == 0 || currentNoteNumber < 2 * clusterNoteNumber);
                                    }
                                }
                                else {
                                    note->setCluster(tmpCluster);

                                    do {
                                        ARMusicalObject *nextObject = GetNext(posNote, armvs);
                                        note = static_cast<ARNote *>(nextObject->isARNote());
                                    }
                                    while(!note || note->getPitch() == 0);

                                    tmpCluster = note->setCluster(cluster, true, true);
                                    note->setCluster(tmpCluster);
                                }

                                delete cluster;
                                posObj = posNote;
                            }
						}	
					}
				}
			}
		}
	}
}


//____________________________________________________________________________________
// set the begin- and end- durations for each feathered beam.
void ARMusicalVoice::doAutoFeatheredBeam()
{
	ARMusicalVoiceState armvs;
	GuidoPos pos = GetHeadPosition(armvs);

	while (pos) {
		GetNext(pos,armvs);
		const PositionTagList * ptags = armvs.getCurPositionTags();

		if (ptags) {
			GuidoPos posTag = ptags->GetHeadPosition();
			while(posTag)
			{
				ARPositionTag   *tag      = ptags->GetNext(posTag);
                ARFeatheredBeam *curfBeam = dynamic_cast<ARFeatheredBeam *>(tag);

				if (curfBeam) {
					GuidoPos posBegin = curfBeam->getPosition();
					ARMusicalObject *object = GetAt(posBegin);
                    ARNote          *note   = static_cast<ARNote *>(object->isARNote());

					if (note) {
						curfBeam->setBeginTimePosition(note->getRelativeTimePosition());
						curfBeam->setBeginDuration(note->getDuration());
					}

					GuidoPos posEnd = curfBeam->getEndPosition();
					object = GetAt(posEnd);
					note = static_cast<ARNote *>(object->isARNote());

					if (note) {
						curfBeam->setEndDuration(note->getDuration());
						curfBeam->setEndTimePosition(note->getRelativeEndTimePosition());
					}
					if (!curfBeam->isDurationsSet()) curfBeam->findDefaultPoints();
				}
			}
		}
	}
}
