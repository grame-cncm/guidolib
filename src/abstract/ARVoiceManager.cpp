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


#include <iostream>

#include "ARVoiceManager.h"
#include "ARNewSystem.h"
#include "ARNewPage.h"
#include "ARPossibleBreak.h"
#include "ARMeter.h"
#include "ARNote.h"
#include "ARMusicalVoice.h"
#include "ARRepeatEnd.h"
#include "ARRepeatBegin.h"

ARVoiceManager::ARVoiceManager(ARMusicalVoice * arVoice)
{
	mVoice = arVoice;
	// this initiales the cuurent VoiceState.
	mVoice->GetHeadPosition(mCurrVoiceState);
	mLastBreakTimePos = DURATION_0;
}

ARVoiceManager::~ARVoiceManager()
{
}

/** \brief Iterates through the voice and returns the current time position (tp) and also
	the status of the current voice.

	Possible Outcome:

		- CURTPBIGGER_ZEROFOLLOWS, if we are alreadyfurther (the last event was longer). The
			next event is a tag or zero-event.
		- CURTPBIGGER_EVFOLLOWS, same as zerofollows, but the next event is an event (dur>0)
 		- DONE_EVFOLLOWS: the current entry has been recognized, next is an event.
		- DONE_ZEROFOLLOWS: the current entry has been recognized, next is a tag or zero event.
		- DONE: current entry has been recognized, next is not defined.
 		- ENDOFVOICE: the voice is finished.
		- NEWSYSTEM: the current entry is a newSystem-tag
		- NEWPAGE:   the current entry is a newPage-Tag
		- MODEERROR: we are in filltagmode, but the	current entry is not a tag ...
*/
int ARVoiceManager::Iterate( TYPE_TIMEPOSITION & tp, int filltagmode )
{
	if (mCurrVoiceState.vpos == NULL)
		return ENDOFVOICE;

	if (mCurrVoiceState.curtp > tp) {
		tp = mCurrVoiceState.curtp;
		ARMusicalObject * o = mVoice->GetAt(mCurrVoiceState.vpos);
		assert(o);
		if (o->getDuration() == DURATION_0)
			return CURTPBIGGER_ZEROFOLLOWS;
		return CURTPBIGGER_EVFOLLOWS;
	}

	assert( mCurrVoiceState.curtp == tp );
	ARMusicalObject *o = mVoice->GetAt(mCurrVoiceState.vpos);
	if (filltagmode)
	{
		if ((double)o->getDuration() > 0)
			return MODEERROR;

		if (dynamic_cast<ARNewSystem *>(o))
			return NEWSYSTEM;

		if (dynamic_cast<ARNewPage *>(o))
			return NEWPAGE;

		// here we have a regular tag.
		// this is just incremented ...
		mVoice->GetNext(mCurrVoiceState.vpos,mCurrVoiceState);

		if (mCurrVoiceState.vpos)
		{
			ARMusicalObject * o = mVoice->GetAt(mCurrVoiceState.vpos);
			if (o->getDuration() > DURATION_0)
				return DONE_EVFOLLOWS;
			return DONE_ZEROFOLLOWS;
		}
	}
	else
	{
		assert ((double)o->getDuration() != 0);
		mVoice->GetNext(mCurrVoiceState.vpos, mCurrVoiceState);
		tp = mCurrVoiceState.curtp;
		if (mCurrVoiceState.vpos)
		{
			ARMusicalObject *o = mVoice->GetAt(mCurrVoiceState.vpos);
			assert(o);
			if (o->getDuration() > DURATION_0)
				return DONE_EVFOLLOWS;
			return DONE_ZEROFOLLOWS;
		}
	}
	return DONE;
}

// this actually inserts a break in the voice.
// newline	= 1 : newSystem 
//			= 2 : newPage
//			= 3 : potential Break
int ARVoiceManager::InsertBreak( const TYPE_TIMEPOSITION & tp, int breaktype, float value)
{

//	if (mCurrVoiceState.vpos == NULL)
//		return -1;


	if (mCurrVoiceState.curtp > tp)
	{
		// then we have to decrement
		// and adjust the event 
		// accordingly.
		// Also, a Tie-Tag
		// has to be introduced.

		mVoice->GetPrevEvent(mCurrVoiceState.vpos,mCurrVoiceState);

		// now we point at the last event before the break ....
		// the 1 means: use a TIE-Tag (2 would be a Merge-Tag)
		mVoice->SplitEventAtPos(mCurrVoiceState,tp,1);

		ARMusicalTag * mytag;
		// now we add the NewSystem or NewPAge-Tag
		if (breaktype == 1)
			mytag = new ARNewSystem();
		else if (breaktype == 2)
			mytag = new ARNewPage();
		else
		{
			ARPossibleBreak * arpb = new ARPossibleBreak();
			arpb->value = value;
			mytag = arpb;
		}

		// these tags area auto-tags!
		mytag->setIsAuto(1);
		mytag->setRelativeTimePosition(mCurrVoiceState.curtp);
		if (mCurrVoiceState.curtp == mCurrVoiceState.curlastbartp)
			mVoice->AddElementAfter( mCurrVoiceState.curlastbarpos, mytag );
		else
			mVoice->AddElementAt( mCurrVoiceState.vpos, mytag );

		// so that we are at the next event ...
		// voice->GetNext(mCurrVoiceState.vpos,mCurrVoiceState);
	}
	else 
	{
		// mCurrVoiceState.curtp == tp
		// then we check, whether we have a  break at the current position ...
		if (!mCurrVoiceState.vpos) 
		{ // then we are already at the End of the voice. We just add the break at the very end ...
			ARMusicalTag * mytag;
			// now we add the NewSystem or NewPAge-Tag
			if (breaktype == 1)
				mytag = new ARNewSystem();
			else if (breaktype == 2)
				mytag = new ARNewPage();
			else
			{
				ARPossibleBreak * arpb = new ARPossibleBreak();
				arpb->value = value;
				mytag = arpb;
			}

			// if the timeposition of the /newSystem|Page|PBreak is greater than mCurrVoiceState.curtp, 
			// we need to add an empty- event to cover the distance. Otherwise it will not match
			// in GRVoiceManager!
			TYPE_DURATION dur = tp - mCurrVoiceState.curtp;
			if (dur > DURATION_0)
			{
				ARNote * note = new ARNote(mCurrVoiceState.curtp, dur);
				note->setPitch( EMPTY );
				mVoice->AddTail(note);
				mCurrVoiceState.curtp += dur;
			}

			// these tags area auto-tags!
			mytag->setIsAuto( true );
			mytag->setRelativeTimePosition(mCurrVoiceState.curtp);
			if (mCurrVoiceState.curtp == mCurrVoiceState.curlastbartp)
				mVoice->AddElementAfter( mCurrVoiceState.curlastbarpos, mytag );
			else
				mVoice->AddTail( mytag );
			return 0;
		}

		ARMusicalObject * o = mVoice->GetAt( mCurrVoiceState.vpos );
		if (dynamic_cast<ARNewPage *>(o) || dynamic_cast<ARNewSystem *>(o))
		{
			// then we just need an increment ...
			mVoice->GetNext(mCurrVoiceState.vpos,mCurrVoiceState);
		}
		else
		{
			// then we really need to insert something at the current position ...			
			ARMusicalTag * mytag;
			
			// now we add the NewSystem or NewPAge-Tag
			if (breaktype == 1)
				mytag = new ARNewSystem();
			else if (breaktype == 2)
				mytag = new ARNewPage();
			else
			{
				ARPossibleBreak *arpb = new ARPossibleBreak();
				arpb->value = value;
				mytag = arpb;
			}
			mytag->setIsAuto(true);
			mytag->setRelativeTimePosition(mCurrVoiceState.curtp);
			if (mCurrVoiceState.curtp == mCurrVoiceState.curlastbartp)
				mVoice->AddElementAfter(mCurrVoiceState.curlastbarpos,mytag);
			else
				mVoice->AddElementAt(mCurrVoiceState.vpos,mytag);
		}
	}
	mLastBreakTimePos = tp;
	return 0;
}

/** \brief Checks wether a break at tp would be feasable. 

	If there is an event occuring right now, then -1 is returned (meaning NOT
	feasable). If there is no Barline here (that is lastbartp != tp), then 0 is returned.
	If a BarLine is here, 1 is returned	meaning: feasable.
	If we are in the middle of a bar, then 0.25	is returned.
*/
float ARVoiceManager::CheckBreakPosition(const TYPE_TIMEPOSITION &tp) const
{
	if (tp == DURATION_0)
        return -1.0f;
	if (tp == mVoice->getDuration())
        return -1.0f;
	
	// then, there will be no break here!
	if (tp == mLastBreakTimePos)
		return -10000.0f;
    
    // D.F. generates a break for range repeat begin
    GuidoPos vpos = mVoice->GetHeadPosition();

	while (vpos) {
		ARMusicalObject *o  = mVoice->GetNextObject(vpos);

        if (tp == o->getRepeatBeginRelativeTimePosition())
            return 1.5f;
	}

	// this only happens, if there is no meter
	// in the current voice ....
	if (mCurrVoiceState.curtp > tp) {
		Fraction tmp(2, 1);

		if (tp - mLastBreakTimePos < tmp)
			return -2.0f;

		if (!mCurrVoiceState.curmeter || mCurrVoiceState.curmeter->getNumerator() == 0) {
			ARMusicalObject *obj;

			if (mCurrVoiceState.vpos == NULL)
				obj = mVoice->GetTail(); // then we are at the end
			else {
				GuidoPos tmp = mCurrVoiceState.vpos;				
				obj = mVoice->GetPrev(tmp);

				if (tmp)
					obj = mVoice->GetAt(tmp);
			}

			if (obj)
				return 0;
		}
		else {
			// we have a meter
			if (tp - mLastBreakTimePos > tmp)
				return 2.0f;
		}

		return -2.0f;
	}
	
	// if the voice is at the end already ...
	// assert commented to avoid spurious exit (due to dynamic score coding)
	// assert (mCurrVoiceState.vpos != NULL);
	if (mCurrVoiceState.vpos == NULL)
		return -1.0f;

	assert(mCurrVoiceState.curtp == tp);

	if (mCurrVoiceState.curlastbartp == tp)	// it is a barline-position
		return 1.5f;
    else if (mCurrVoiceState.curmeter) {
        if (mCurrVoiceState.curtp - (mCurrVoiceState.curmeter->getMeterTime() * DURATION_2) == mCurrVoiceState.curlastbartp)
            return -1.0f;
        else {
            TYPE_TIMEPOSITION tmptp(tp - mCurrVoiceState.curlastbartp);

            if (tmptp.getDenominator() == mCurrVoiceState.curmeter->getDenominator())
                return -1.0f;
        }
    }

 	return -1.0f;
}