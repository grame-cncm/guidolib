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

#include <fstream>

#include "ARMusic.h"
#include "ARVoiceManager.h"
#include "ARAuto.h"
#include "ARMusicalVoice.h"
#include "AROctava.h"
#include "MeterVisitor.h"
#include "TempoVisitor.h"
#include "TimeMapper.h"
#include "GMNCodePrintVisitor.h"

#include "benchtools.h"

#include <iostream>
using namespace std;

int ARMusic::mRefCount = 0;

ARMusic * gCurArMusic = NULL;

ARMusic::ARMusic() : MusicalVoiceList(1) // owns Elements
{
	mRefCount++;
    mParseTime = -1;
}

ARMusic::~ARMusic()
{
	mRefCount--;
}

GuidoPos ARMusic::AddTail(ARMusicalVoice * newMusicalVoice)
{
	assert(newMusicalVoice);

	//eventSequence->insertCriticalVoiceElements(newMusicalVoice);
	return MusicalVoiceList::AddTail(newMusicalVoice);
}

/** \brief Gives voices an equal duration by adding rests at the end of voices
*/
void ARMusic::adjustDuration(TYPE_DURATION newDuration)
{
	TYPE_DURATION d = getDuration();
	assert(newDuration >= d); // keine Verk�rzung m�glich
	ARMusicalVoice * v=NULL;
	GuidoPos pos=GetHeadPosition();

	setDuration (newDuration);
	while(pos) // alle Stimmen des Segments auf die neue L�nge bringen
	{
		v=GetNext(pos);
		v->adjustDuration(newDuration);
	}
}

int ARMusic::countVoices () const
{
	int count = 0;
	GuidoPos pos = GetHeadPosition();
	while (pos) {
		count++;
		GetNext(pos);
	}
	return count;
}

bool ARMusic::getMeterAt (int voicenum, const GuidoDate &date, GuidoMeter& meter)
{
	if (voicenum <= 0) return false;

	GuidoPos pos = GetHeadPosition();
	ARMusicalVoice * voice = 0;
	while (pos && !voice) {
		if (!--voicenum) voice = GetNext(pos);
		else GetNext(pos);
	}
	if (!voice) return false;		// no such voice
	TYPE_TIMEPOSITION tp (date.num, date.denom);
	MeterVisitor mv (tp);
	voice->accept (mv);
	meter = mv.getMeter();
	return true;
}

bool ARMusic::getMetersAt (int voicenum, const GuidoDate &date, GuidoMeters& meters)
{
	if (voicenum <= 0) return false;

	GuidoPos pos = GetHeadPosition();
	ARMusicalVoice * voice = 0;
	while (pos && !voice) {
		ARMusicalVoice * tmp = GetNext(pos);
		if (tmp->getVoiceNum() == voicenum) voice = tmp;
	}
	if (!voice) return false;		// no such voice
	TYPE_TIMEPOSITION tp (date.num, date.denom);
	MetersVisitor mv (tp);
	voice->accept (mv);
	meters = mv.getMeters();
	return true;
}

size_t ARMusic::getTempoList (GuidoTempoList& tempi)
{
	TempoVisitor tv;
	int n=0;
	GuidoPos pos = GetHeadPosition();
	while (pos) {
		ARMusicalVoice * voice = GetNext(pos);
		tv.voice(n++);
		voice->accept (tv);
	}
	tempi = tv.getTempi();
	return tv.size();
}

void ARMusic::getTimeMap (TimeMapCollector& f) const
{
	GuidoPos pos = GetHeadPosition();
    if(pos)
    {
        ARMusicalVoice * voice = GetNext(pos);
        if (voice) {
            TimeMapper mapper (f, voice);
            voice->browse(mapper);	// actually browse the first voice only
        }
	}
}

void ARMusic::accept(ARVisitor& visitor)
{
	visitor.visitIn (this);
	GuidoPos pos = GetHeadPosition();
	while (pos) {
		ARMusicalVoice * e = GetNext(pos);
		e->accept(visitor);
	}
	visitor.visitOut (this);
}

//void ARMusic::resetGRRepresentation()
//{
//	ARMusicalEvent::resetGRRepresentation();
//	GuidoPos pos = GetHeadPosition();
//	ARMusicalVoice * e;
//	while(pos)
//	{
//		e=GetNext(pos);
//		e->resetGRRepresentation();
//	}
//}

/** \brief Introduces potential Breakpoints
	in the first voice of a piece of music.

	These potential Breakpoints are later used to actually do newSystem or newPage, if they
	have not been introduced already.
	The potential Breaks are given evaluation-values,that is, their break-potential is evaluated.
	This is done with respect to barline-positions. That is, a place, where all voices have barlines is
	a better break-place than one, that has no or only some barlines.
	The algorithm works as follows:
	The voices are traversed in parallel. newSystem or newPage-Tags are recognized;
	if there is no newSystem or newPage-Tag at a Tag-Position and if ALL voices have no current
	event, a potential break is introduced.
*/
void ARMusic::doAutoBreaks()
{
	// first, we create VoiceManagers. (in a list of voice-managers ...)
	KF_IPointerList<ARVoiceManager> mgrlst(1);

	GuidoPos pos = GetHeadPosition();
	ARMusicalVoice * e;
	int cnt = 0;
	while(pos)
	{
		e = GetNext(pos);
		++ cnt;
		mgrlst.AddTail( new ARVoiceManager(e) );
	}

	// this remembers, if all voices are at the end.
	bool ender;

	TYPE_TIMEPOSITION tp ( DURATION_0 );
	TYPE_TIMEPOSITION minswitchtp;
	TYPE_TIMEPOSITION mintp;

	bool filltagmode = true;
	bool conttagmode = false;
	int newline;
	bool modeerror = true;
	bool abreak = true; // autobreaks are on ...

	// cnt is the number of VoiceManagers ...
	do
	{
		// initialise ender so that it ends ...
		ender = true;
		conttagmode = false;
		modeerror = true;

		mintp = MAX_DURATION;
		minswitchtp = MAX_DURATION;

		newline = 0;
		abreak = true;

		TYPE_TIMEPOSITION tmptp;

		GuidoPos pos = mgrlst.GetHeadPosition();
		while (pos)
		{
			ARVoiceManager * vcemgr = mgrlst.GetNext(pos);

			tmptp = tp;
			int ret = vcemgr->Iterate(tmptp,filltagmode);
			if (vcemgr->mCurrVoiceState.curautostate &&
				vcemgr->mCurrVoiceState.curautostate->getSystemBreakState() == ARAuto::kOff)
				abreak = false;

			if (ret != ARVoiceManager::MODEERROR)
				modeerror = false;

			if (ret != ARVoiceManager::ENDOFVOICE) {
				ender = false;

				if (ret == ARVoiceManager::CURTPBIGGER_ZEROFOLLOWS || ret == ARVoiceManager::DONE_ZEROFOLLOWS) {
					if (tmptp < minswitchtp)
						minswitchtp = tmptp;
				}

				if (filltagmode) {
					if (ret == ARVoiceManager::NEWSYSTEM) {
						if (!newline)
							newline = 1;
					}
					else if (ret == ARVoiceManager::NEWPAGE) {
						if (!newline || newline == 1)
							newline = 2;
					}
					else if (ret == ARVoiceManager::DONE_ZEROFOLLOWS)
						conttagmode = true;
				}
				else { // filltagmode == 0;
					if (ret == ARVoiceManager::DONE ||
						ret == ARVoiceManager::DONE_EVFOLLOWS ||
						ret == ARVoiceManager::DONE_ZEROFOLLOWS ||
						ret == ARVoiceManager::CURTPBIGGER_EVFOLLOWS ||
						ret == ARVoiceManager::CURTPBIGGER_ZEROFOLLOWS )
					{
						if (tmptp < mintp)
							mintp = tmptp;
					}
				}
			}
		} // while (pos) mgrlst,

		// now we have worked on a complete slice ...
		if (filltagmode) {
			if (modeerror) {
				if (conttagmode == false) filltagmode = false;
			}
			else {
				// it is important, to do this only, after the TAG-Slice is complete
				// (or there are all explicit newlines ...)
				if (conttagmode == false) {
					if (newline) {
						// we have an explicit newsystem or an explicit newline ...
						// now, we need to tell ALL VoiceManagers to insert the equivalent break ...
						GuidoPos pos = mgrlst.GetHeadPosition();
						while ( pos) {
							ARVoiceManager * vm = mgrlst.GetNext(pos);
							vm->InsertBreak(tp,newline);
						}
						conttagmode = true;
					}
				}

				if (conttagmode == false)
				{
					// also, test the newline-position ... (feasable breaks?)
					float value = 0.0f;
					float count =  0.0f;
					GuidoPos pos = mgrlst.GetHeadPosition();
					while (pos) {
						value += mgrlst.GetNext(pos)->CheckBreakPosition(tp);
						++ count;
					}

					assert( count > 0 );
					value /= count;

					if (value >= 0.0f && abreak) {
						// then we are feasable
						GuidoPos pos = mgrlst.GetHeadPosition();
						while (pos) {
							// meaning, possible NewLine  (that is posibleBreak)
							mgrlst.GetNext(pos)->InsertBreak(tp,3,value);
						}
					}

					// then we need to switch ....
					filltagmode = false;
				} // if conttagmode==0
			} // else modeerror
		}
		else
		{
			// filltagmode == false
			if (mintp != MAX_DURATION)	tp = mintp;
			else if (!ender)			assert(false);

			// then we need to switch to tag-mode
			if (minswitchtp == tp)
				filltagmode = true;
			else {
				// OK, we continue in no filltagmode so NOW is the time to check for
				// feasable breaks.
				// also, test the newline-position ... (feasable breaks?)
				float value = 0.0f;
				float count = 0.0f;
				GuidoPos pos = mgrlst.GetHeadPosition();
				while (pos) {
					value += mgrlst.GetNext(pos)->CheckBreakPosition(tp);
					++ count;
				}

				assert( count > 0 );
				value /= count;
				if (value >= 0.0f && abreak) {
					// then we are feasable
					GuidoPos pos = mgrlst.GetHeadPosition();
					while (pos) {
						// meaning, possible NewLine (that is posibleBreak)
						mgrlst.GetNext(pos)->InsertBreak(tp,3,value);
					}
				}
			}
		} // else (filltagmode == 1)
	}
	while (!ender);
}

void ARMusic::doAutoStuff()
{
	// this is important so that the voices now about the maximum tag-id.
	gCurArMusic = this;

	// First, we adjust all durations ...
	TYPE_DURATION maxdur = DURATION_0;
	TYPE_DURATION tmp;
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		tmp = GetNext(pos)->getDuration();
		if (tmp>maxdur)
			maxdur = tmp;
	}

	adjustDuration(maxdur);

	int counter = 0;
	pos = GetHeadPosition();

	while (pos) {
		++counter;
		ARMusicalVoice * arvc = GetNext(pos);
		timebench("doAutoStuff1", arvc->doAutoStuff1());
	}

//cerr << "======================== after auto stuff 1" << endl;
//	GMNCodePrintVisitor va(cerr);
//	this->accept (va);


	// now, we do the stuff that needs to be done by ALL voices ...
	// AutoBreaks inserts possible breaks at the positions, keeping track of explicit newlines ....
	// the breaks are put in at bar-lines-positions or at positions, where all voices end.
    timebench("doAutoBreaks", doAutoBreaks());

	counter = 0;
	pos = GetHeadPosition();

	while (pos) {
		counter++;
		// now we can check, wether newSystem and newPage are followed by correct clef/key information ...
		// also do AutoDisplayCheck and autoBeaming.
		ARMusicalVoice * arvc = GetNext(pos);
		timebench("doAutoStuff2", arvc->doAutoStuff2());
	}

//	GMNCodePrintVisitor v(cerr);
//	this->accept (v);
}

//____________________________________________________________________________________
/** \brief check multiple octava on multiple voice, same staff
*/
void ARMusic::doOctavaCheck()
{
	int num = 1;
	map<int, vector<AROctava*> > map;
	GuidoPos pos = GetHeadPosition();
	while (pos) {
		ARMusicalVoice * arvc = GetNext(pos);
		arvc->getOctava(num++, map);
	}
	
	for (auto elt: map) {
		if (elt.second.size() > 1) {
cerr << "ARMusic::doOctavaCheck check dup on staff " << elt.first << endl;
			for (AROctava* o: elt.second) {
cerr << "	- date: " << o->getRelativeTimePosition() << " end: " << o->getEnd() << endl;
				
			}
		}
		
	}
}

/** \brief Removes tags that were added automatically
*/
void ARMusic::removeAutoTags()
{
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		ARMusicalVoice * arvc = GetNext(pos);
		arvc->removeAutoTags();
    }
}

/* \brief Must be called after the ARMusic has been created (ie: by GuidoParse)

	It finds the voice and introduces a noteFormat-tag at the
	indicated position.
*/
void ARMusic::MarkVoice(int voicenum,float from,float length, unsigned char red, unsigned char green, unsigned char blue)
{
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		ARMusicalVoice * arvc = GetNext(pos);
		if (arvc->getVoiceNum() == voicenum)
		{
			arvc->MarkVoice(from, length, red, green, blue);
			break;
		}
	}
}

void ARMusic::MarkVoice(int voicenum,
						int fromnum,int fromdenom,
						int lengthnum,int lengthdenom,
                        unsigned char red, unsigned char green, unsigned char blue)
{
	GuidoPos pos = GetHeadPosition();
	while (pos)
	{
		ARMusicalVoice * arvc = GetNext(pos);
		if (arvc->getVoiceNum() == voicenum)
		{
			arvc->MarkVoice(fromnum,fromdenom,
				lengthnum,lengthdenom, red, green, blue);
			break;
		}
	}

}
