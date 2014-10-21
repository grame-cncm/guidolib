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

#include "TagList.h"
#include  "kf_ilist.h"		// for TagList

#include "ARMusicalVoiceState.h"
#include "ARMusicalTag.h"
#include "ARMeter.h"
#include "ARBeamState.h"
#include "ARAuto.h"
#include "ARDisplayDuration.h" 
#include "ARChordTag.h"
#include "ARGrace.h"
#include "ARTagEnd.h"

using namespace std;

ARMusicalVoiceState::ARMusicalVoiceState()
{
    curlastbarpos = NULL;
	curstatetags = NULL;
	curtags = NULL;
	curpositiontags = NULL;
	vpos = (GuidoPos) NULL;
	
	ptagpos = (GuidoPos) NULL;
	
	addedpositiontags = NULL;
	removedpositiontags = NULL;
	
	curmeter = NULL;
	curbeamstate = NULL;
	fCurdispdur = NULL;
	curchordtag = NULL;
	curgracetag = NULL;
//	curbreakstate = NULL;
	curautostate = NULL;

	chordState = NULL;
	prevchordState = NULL;
}

ARMusicalVoiceState::ARMusicalVoiceState(const ARMusicalVoiceState & vst)
{	
	curstatetags = NULL;
	curtags = NULL;
	curpositiontags = NULL;				
	vpos = (GuidoPos) NULL;
	
	ptagpos = (GuidoPos) NULL;
	
	addedpositiontags = NULL;			
	removedpositiontags = NULL;
				
	curmeter = NULL;
	curbeamstate = NULL;
	fCurdispdur = NULL;
	curchordtag = NULL;
	curgracetag = NULL;
//	curbreakstate = NULL;
	curautostate = NULL;

	chordState = NULL;
	prevchordState = NULL;
				
	*this = vst;
}

ARMusicalVoiceState::~ARMusicalVoiceState()
{
	DeleteAll();
}

ARMusicalVoiceState & ARMusicalVoiceState::operator = (const ARMusicalVoiceState & vst)
{
	DeleteAll();
	if (vst.curtags)
	{
		curtags = new TagList(*vst.curtags,0); // does not own!
	}
	else
		curtags = NULL;
	
	if (vst.curstatetags)
	{
		curstatetags = new TagList(*vst.curstatetags,0);
	}
	else
		curstatetags = NULL;
	
	if (vst.curpositiontags)
	{
		curpositiontags = new PositionTagList(*vst.curpositiontags,0);
	}
	else
		curpositiontags = NULL;
	
	if (vst.addedpositiontags)
	{
		// does not own
		addedpositiontags = new PositionTagList(*vst.addedpositiontags,0);
	}
	else
		addedpositiontags = NULL;
	
	if (vst.removedpositiontags)
	{
		// does not own
		removedpositiontags = new PositionTagList(*vst.removedpositiontags,0);
	}
	else
		removedpositiontags = NULL;
	
	vpos = vst.vpos;
	curtp = vst.curtp;
	
	ptagpos = vst.ptagpos;
	
	curmeter = vst.curmeter;
	curbeamstate = vst.curbeamstate;
	fCurdispdur = vst.fCurdispdur;
	curchordtag = vst.curchordtag;
	curgracetag = vst.curgracetag;
	
//	curbreakstate = vst.curbreakstate;
	curautostate = vst.curautostate;

	curlastbartp = vst.curlastbartp;
	curlastbarpos = vst.curlastbarpos;

	delete chordState;
	chordState = NULL;

	if (vst.chordState)
	{
		// make sure that the chordstate itself does not
		// have pointers to other chordStates or
		// prevChordState ...
		assert(vst.chordState->chordState == NULL);
		assert(vst.chordState->prevchordState == NULL);
		chordState = new ARMusicalVoiceState(*vst.chordState);
	}

	delete prevchordState;
	prevchordState = NULL;

	if (vst.prevchordState)
	{
		// this is important so that we only save the one
		// afterwards and nothing more ....
		/* ARMusicalVoiceState *tmpsave =
			vst.prevchordState->prevchordState;
		vst.prevchordState->prevchordState = NULL; */
		// make sure that the chordstate itself does not
		// have pointers to other chordStates or
		// prevChordState ...
		assert(vst.prevchordState->chordState == NULL);
		assert(vst.prevchordState->prevchordState == NULL);

		prevchordState = new ARMusicalVoiceState(*vst.prevchordState);

		// vst.prevchordState->prevchordState = tmpsave;
	}

	return *this;
}

void ARMusicalVoiceState::RemoveTag(ARMusicalTag * ntag)
{
	assert(false);
	if (!curtags)
	{
		// Should not happen!
		assert(false);
	}
	curtags->RemoveElement(ntag);
}

void ARMusicalVoiceState::AddTag(ARMusicalTag  * ntag)
{
	assert(false);
	if (!curtags)
		curtags = new TagList(0); // does not own
	
	curtags->AddTail(ntag);
}

void ARMusicalVoiceState::AddStateTag(ARMusicalTag * ntag)
{
	if (!curstatetags)
		curstatetags = new TagList(0); // does not own
	
	GuidoPos pos = curstatetags->GetHeadPosition();
	int found = 0;
	while(pos)
	{
		ARMusicalTag * e = curstatetags->GetAt(pos);
		if (typeid(*e) == typeid(*ntag))
		{
			// replace Tag!
			curstatetags->SetAt(pos,ntag);
			found = 1;
			break;
			
		}
		curstatetags->GetNext(pos);
	}
	if (!found)
	{
		// this must be an AddTail, otherwise
		// the corret order cannot be maintained
		// when processing the StateInformation
		// (staff-Tag needs to be before the
		// other Tags ...)
		curstatetags->AddTail(ntag);
	}
	
	ARMeter * armt;
	ARBeamState * arbms;
//	ARABreak *arabreak;
	ARAuto * arauto;
	if ( (armt = dynamic_cast<ARMeter *>(ntag)) != NULL)
	{
		curmeter = armt;
	}
	else if ( (arbms = dynamic_cast<ARBeamState *>(ntag)) != NULL)
	{
		curbeamstate = arbms;
	}
//	else if ( (arabreak = dynamic cast<ARABreak *>(ntag))
//		!= NULL)
//	{
//		curbreakstate = arabreak;
//	}
	else if ( (arauto = dynamic_cast<ARAuto *>(ntag)) != NULL)
	{
		curautostate = arauto;
	}
}

void ARMusicalVoiceState::AddPositionTag(ARPositionTag *ntag, int addtoaddedlist)
{
	if (!curpositiontags)
		curpositiontags = new PositionTagList(0);

	curpositiontags->AddTail(ntag);
	
	if (addtoaddedlist) {
		if (!addedpositiontags)
			addedpositiontags = new PositionTagList(0);

		addedpositiontags->AddTail(ntag);
	}
	
	ARDisplayDuration *arddur;
	if ((arddur = dynamic_cast<ARDisplayDuration *>(ntag)) != NULL)
		fCurdispdur = arddur;

	ARChordTag *chordtag;
	if ((chordtag = dynamic_cast<ARChordTag *>(ntag)) != NULL) {
		if (curchordtag != NULL) {
			GuidoTrace("nested chordtags are not allowed!");
			assert(false);
		}
		curchordtag = chordtag;
	}

	ARGrace *gracetag;
	if ((gracetag = dynamic_cast<ARGrace *>(ntag)) != NULL)
	{
		if (curgracetag != NULL) {
			GuidoTrace("nested gracetags are notallowed!");
			assert(false);
		}

		curgracetag = gracetag;
	}
}

void ARMusicalVoiceState::RemovePositionTag(ARPositionTag * ntag, int addtoremovedlist)
{
	if (!curpositiontags)
	{
		assert(false);
		return; 
	}
		
	// this either deletes the matching begin-Tag or 
	// just the position-tag that was given!
	ARTagEnd * artgend = ARTagEnd::cast(ntag);
	
	ARPositionTag * start;
	if (artgend)
	{
		start = artgend->getCorrespondence();
		if (start == NULL)
		{ // no correspondence
			
			// not sure, whether this should happen at all.
			assert(false);
			
			start = /*dynamic cast<ARPositionTag *>*/(artgend);
		}
	}
	else 
		start = ntag;
	
	int checkforcurtags = 0;
	ARDisplayDuration * arddur;
	if ( (arddur = dynamic_cast<ARDisplayDuration *>(ntag)) != NULL)
	{
		fCurdispdur = NULL;
		checkforcurtags = 1;
	}

	ARChordTag * chordtag;
	if ( (chordtag = dynamic_cast<ARChordTag *>(ntag)) != NULL )
	{
		curchordtag = NULL;
	}

	ARGrace * gracetag;
	if ( (gracetag = dynamic_cast<ARGrace *>(ntag)) != NULL )
	{
		curgracetag = NULL;
	}

	GuidoPos pos = curpositiontags->GetHeadPosition();
	bool found = false;
	while (pos)
	{
		if (curpositiontags->GetAt(pos) == start)
		{
			curpositiontags->RemoveElementAt(pos);
			found = true;
			break;
		}
		curpositiontags->GetNext(pos);
	}
	if (found == false)
	{
		// Attention; think about what happens
		// here; we have a closing tag without
		// a matching beginning tag.
		// this should not happen!
		// assert(false);
		return;
	}

	if (addtoremovedlist)
	{
		if (!removedpositiontags)
			removedpositiontags = new PositionTagList(0);
		
		removedpositiontags->AddTail(ntag);
	}
	
	// it may be, that a tag that is removed has been
	// added during the same call!
	// (this may happen during GetPrevEvent)
	// this may also happen in Chord-Overread-mode
	// tags within chords are not interessting!
	if (addedpositiontags)
	{
		addedpositiontags->RemoveElement(ntag);
	}
	
	if (artgend)
	{
		if (artgend->getCorrespondence() == fCurdispdur)
		{
			fCurdispdur = NULL;
		}
		if (artgend->getCorrespondence() == curchordtag)
		{
			curchordtag = NULL;
		}
		if (artgend->getCorrespondence() == curgracetag)
		{
			curgracetag = NULL;
		}

	}

	// here, we have to check wether there are
	// any tags in the curpositiontags ...
	if (checkforcurtags)
	{
		GuidoPos pos = curpositiontags->GetTailPosition();
		while (pos)
		{
			ARDisplayDuration * tag = dynamic_cast<ARDisplayDuration *>(curpositiontags->GetPrev(pos));
			if (tag)
			{
				fCurdispdur = tag;
				break;
			}
		}		
	}	
}

void ARMusicalVoiceState::DeleteAll()
{
	delete curtags;				curtags = 0;
	delete curstatetags;		curstatetags = 0;
	delete curpositiontags;		curpositiontags = 0;
	delete addedpositiontags;	addedpositiontags = 0;
	delete removedpositiontags;	removedpositiontags = 0;
	delete chordState;			chordState = 0;
	delete prevchordState;		prevchordState = 0;
}

void ARMusicalVoiceState::DeleteAddedAndRemovedPTags()
{	
	delete addedpositiontags;	addedpositiontags = 0;
	delete removedpositiontags;	removedpositiontags = 0;
}

/** \brief Removes a state tag from the curstate-tag list.
	the list does not own the elements.
*/
ARMusicalTag * ARMusicalVoiceState::RemoveCurStateTag(const std::type_info &ti)
{
	if (!curstatetags)
		return NULL;
	
	GuidoPos pos = curstatetags->GetHeadPosition();
	while (pos)
	{
		GuidoPos prevpos = pos;
		ARMusicalTag * tmp = curstatetags->GetNext(pos);
		
		if ( typeid(*tmp) == ti )
		{
			curstatetags->RemoveElementAt(prevpos);
			return tmp;
		}
	}
	
	return 0;
}

/** \brief Returns a state-tag with the given type.
*/
ARMusicalTag * ARMusicalVoiceState::getCurStateTag(const std::type_info & ti)
{
	if (curstatetags == 0)
		return 0;
	
	GuidoPos pos = curstatetags->GetHeadPosition();
	while (pos)
	{
		ARMusicalTag * tmp = curstatetags->GetNext(pos);
		
		if ( typeid(*tmp) == ti )
		{
			return tmp;
		}
	}
	
	return 0;
}


