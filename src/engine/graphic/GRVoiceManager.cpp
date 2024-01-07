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

#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <sstream>


#include "ARAccelerando.h"
#include "ARAccent.h"
#include "ARAccidental.h"
#include "ARAccolade.h"
#include "ARAlter.h"
#include "ARArpeggio.h"
#include "ARAuto.h"
#include "ARBarFormat.h"
#include "ARBase.h"
#include "ARBeam.h"
#include "ARBow.h"
#include "ARBreathMark.h"
#include "ARChordComma.h"
#include "ARChordTag.h"
#include "ARClef.h"
#include "ARCluster.h"
#include "ARCoda.h"
#include "ARColor.h"
#include "ARComposer.h"
#include "ARCrescendo.h"
#include "ARDaCapo.h"
#include "ARDaCoda.h"
#include "ARDalSegno.h"
#include "ARDiminuendo.h"
#include "ARDisplayDuration.h"
#include "ARDotFormat.h"
#include "ARDoubleBar.h"
#include "ARDrHoos.h"
#include "ARDrRenz.h"
#include "ARDummyRangeEnd.h"
#include "ARFeatheredBeam.h"
#include "ARFermata.h"
#include "ARFine.h"
#include "ARFingering.h"
#include "ARFinishBar.h"
#include "ARFooter.h"
#include "ARGlissando.h"
#include "ARGrace.h"
#include "ARHarmonic.h"
#include "ARInstrument.h"
#include "ARIntens.h"
#include "ARLabel.h"
#include "ARLyrics.h"
#include "ARMarcato.h"
#include "ARMark.h"
#include "ARMeter.h"
#include "ARMMRest.h"
#include "ARMusicalVoice.h"
#include "ARMusicalVoiceState.h"
#include "ARNaturalKey.h"
#include "ARNewPage.h"
#include "ARNewSystem.h"
#include "ARNotations.h"
#include "ARNote.h"
#include "ARNoteFormat.h"
#include "AROctava.h"
#include "ARPageFormat.h"
#include "ARPizzicato.h"
#include "ARPossibleBreak.h"
#include "ARRepeatBegin.h"
#include "ARRepeatEnd.h"
#include "ARRepeatEndRangeEnd.h"
#include "ARRest.h"
#include "ARRestFormat.h"
#include "ARRitardando.h"
#include "ARSecondGlue.h"
#include "ARSegno.h"
#include "ARShareLocation.h"
#include "ARShareStem.h"
#include "ARSpace.h"
#include "ARSpecial.h"
#include "ARStaccato.h"
#include "ARStaff.h"
#include "ARStaffFormat.h"
#include "ARStaffOff.h"
#include "ARStaffOn.h"
#include "ARSymbol.h"
#include "ARSystemFormat.h"
#include "ARTab.h"
#include "ARTempo.h"
#include "ARTenuto.h"
#include "ARHarmony.h"
#include "ARTHead.h"
#include "ARTie.h"
#include "ARTitle.h"
#include "ARTremolo.h"
#include "ARTrill.h"
#include "ARTStem.h"
#include "ARTuplet.h"
#include "ARUnits.h"
#include "ARUserChordTag.h"
#include "ARVolta.h"

#include "TagList.h"
#include "TagParameterFloat.h"

#include "GRArpeggio.h"
#include "GRArticulation.h"
#include "GRAutoBeam.h"
#include "GRBar.h"
#include "GRBeam.h"
#include "GRBreathMark.h"
#include "GRChordTag.h"
#include "GRClef.h"
#include "GRCluster.h"
#include "GRCoda.h"
#include "GRCrescendo.h"
#include "GRDiminuendo.h"
#include "GRDoubleBar.h"
#include "GRDrHoos.h"
#include "GRDrRenz.h"
#include "GRDummy.h"
#include "GREmpty.h"
#include "GRFingering.h"
#include "GRFinishBar.h"
#include "GRGlissando.h"
#include "GRGlobalLocation.h"
#include "GRGlobalStem.h"
#include "GRGlue.h"
#include "GRGrace.h"
#include "GRInstrument.h"
#include "GRIntens.h"
#include "GRJump.h"
#include "GRKey.h"
#include "GRMark.h"
#include "GRMeter.h"
#include "GRMusic.h"
#include "GRNotations.h"
#include "GROctava.h"
#include "GRPageText.h"
#include "GRRange.h"
#include "GRRepeatBegin.h"
#include "GRRepeatEnd.h"
#include "GRRest.h"
#include "GRSegno.h"
#include "GRSingleNote.h"
#include "GRSingleRest.h"
#include "GRSlur.h"
#include "GRSpace.h"
#include "GRSpecial.h"
#include "GRStaffManager.h"
#include "GRStdNoteHead.h"
#include "GRStem.h"
#include "GRSymbol.h"
#include "GRSystemSlice.h"
#include "GRSystemTag.h"
#include "GRTab.h"
#include "GRTag.h"
#include "GRTempo.h"
#include "GRTempoChange.h"
#include "GRText.h"
#include "GRHarmony.h"
#include "GRTie.h"
#include "GRTremolo.h"
#include "GRTrill.h"
#include "GRTuplet.h"
#include "GRVoiceManager.h"
#include "GRVolta.h"


using namespace std;


vector<GRSingleNote *> GRVoiceManager::fCurrentNotesTP;   // current notes at a given time position

GRVoiceManager::GRVoiceManager(GRMusic* music, GRStaffManager * p_staffmgr, const ARMusicalVoice * p_voice,  int p_voicenum)
{
	fMusic	= music;
	toadd = NULL;
	mCurGrace = NULL;
    mCurCluster = NULL;
	mCurGlissando = NULL;
	curglobalstem = NULL;
	curchordtag = NULL;
	curgloballocation = NULL;
	curstemstate = NULL;
	curheadstate = NULL;
	mStaffMgr = p_staffmgr;
	arVoice = p_voice;
	voicenum = p_voicenum;
	mCurGrStaff = NULL;
	grvoice = fMusic->getVoice(arVoice);

	fLastOctava = NULL;
	fGRTags = NULL;

	lastev = NULL;

	curnoteformat = NULL;
	curdotformat = NULL;
	currestformat = NULL;

	fLastbar = NULL;
	fLastnonzeroevent = NULL;

    mCurrentClusterNoteNumber = 0;
}

GRVoiceManager::~GRVoiceManager()
{
	delete toadd;	toadd = 0;
	delete fGRTags;	fGRTags = 0;
	delete fVoiceState;	fVoiceState = 0;
}

/** \brief check if next tag is a repeatBegin tag
*/
bool GRVoiceManager::checkRepeatBeginNext()
{
    ARMusicalVoiceState vst (*fVoiceState);
    GuidoPos pos = fVoiceState->vpos;
    ARMusicalObject *next;

    arVoice->GetNext(pos, vst);
    next = arVoice->GetNext(pos, vst);

    if (next) {
        if (static_cast<ARPossibleBreak *>(next->isARPossibleBreak()))
            next = arVoice->GetNext(pos, vst);
    }

    return (next && next->isARRepeatBegin()) ? true : false;
}

/** \brief Parses the state tags that are important for the voice-manager.

	it retuns 1, if the tag was handled.
	otherwise, 0 is returned.
*/
bool GRVoiceManager::parseStateTag(const ARMusicalTag * mtag)
{
	bool retval = true;

	const ARStaff       * mstaff;
	const ARTStem       * mstem;
	const ARTHead       * mhead;
	const ARColor       * theColor;
	const ARStaffFormat * staffrmt;
	const ARBarFormat   * barfrmt;
	const ARNoteFormat  * notefrmt;
	const ARDotFormat   * dotfrmt;
	const ARRestFormat  * restfrmt;
	const ARAuto        * arauto;

	mstaff = dynamic_cast<const ARStaff *>(mtag);

	if (mstaff) {
		// it is a staff-tag...

		// then, the staff will be changed...
		// the StaffManager (which is the owner
		// of this VoiceManager) is called; it
		// then prepares the staff...
		staffnum = mstaff->getStaffNumber();
		mStaffMgr->prepareStaff(staffnum);
		mCurGrStaff = mStaffMgr->getStaff(staffnum);
		if (mstaff->getDY() && mstaff->getDY()->TagIsSet()) {
			// what about staff-format and HS-values here ?
			mCurGrStaff->setDistance(mstaff->getDY()->getValue());
		}

		assert(mCurGrStaff);
	}
	else if ((staffrmt = dynamic_cast<const ARStaffFormat *>(mtag)) != 0)
		mCurGrStaff->setStaffFormat(staffrmt);
	else if ( (barfrmt = dynamic_cast<const ARBarFormat *>(mtag)) != 0)
		mStaffMgr->setBarFormat(barfrmt,mCurGrStaff);
	else if ( (mstem = dynamic_cast<const ARTStem *>(mtag)) != 0) {
		// it is a stem-tag (stemsUp, stemsDown, stemsAuto)
		// stemstate = mstem->getStemState();
		curstemstate = mstem;
	}
	else if ( (mhead = dynamic_cast<const ARTHead*>(mtag)) != 0) {
		// the headstate... 
		curheadstate = mhead;
	}
	else if ((theColor = dynamic_cast<const ARColor *>(mtag)) != 0) {
		// it is a color tag... (?)
	}
	else if (typeid(*mtag) == typeid(ARUnits)) {
		// just ignore units tag... (it is a state
		// tag after all...)
	}
	else if ((dotfrmt = dynamic_cast<const ARDotFormat *>(mtag)) != 0)
		curdotformat = dotfrmt;
	else if ((notefrmt = dynamic_cast<const ARNoteFormat *>(mtag)) != 0)
		curnoteformat = notefrmt;
	else if ((restfrmt = dynamic_cast<const ARRestFormat *>(mtag)) != 0)
		currestformat = restfrmt;
	else if ((arauto = dynamic_cast<const ARAuto *>(mtag)) != 0) {
			// we have an auto(set)-Tag set...
			mStaffMgr->setAutoTag(arauto);
			if (mCurGrStaff && arauto->hasMultiVoiceCollision()) {
				mCurGrStaff->getGRStaffState().setMultiVoiceCollisions (arauto->getAutoMultiVoiceCollisions());
			}
	}
	else
		retval = false;

	return retval;
}

/** \brief Called at the beginning of a management-Process. The VoiceManager initializes
	itself (time-Position, voice-state...) the staff-number is the default-staff-number.
*/
void GRVoiceManager::BeginManageVoice()
{
	// curtp = begintp;
	staffnum = arVoice->getVoiceNum();

	// this is called only once...!

	// this retrieves the current voice-state
	// (this is valid, if this is a "follow"-up after a newSystem, or newPage-Tag)
	
		fVoiceState = new ARMusicalVoiceState();
		// the very first position...
		arVoice->GetHeadPosition(*fVoiceState);
	

	/* actually, the NoteFactory is no longer needed;
	   all Elements in the voice have been "crushed", so that they can be displayed by a single graphical element.
	*/

    // Here, we define a Stack, that handles the TAGs. When a
    // Tag (e.g. the graphical representation of the Tag) is
	// on this tag, each Event (something that has a duration even
	// if it is 0) is associated with it. (addAssociation is called). This ensures,
	// that the graphical representation "knows", which tags belong to it.
	fGRTags = new GRTagPointerList();
	
	// the currently active tags can be accessed from the current voice-state
	
	// this is no longer needed, because there
	// is No Followup! -> This procedure is
	// called exactly one at the very beginning
	// At this point, there are NO StateTags yet!


	// we have to parse all the tags, that handle state-information (like pageformat,systemformat,staff etc...)
	ReadBeginTags(fVoiceState->curtp);

	// The StaffManager handles all staves in a list...
	mStaffMgr->prepareStaff(staffnum);
	
	// we have to re-initialize the map curStaffDraw because it is static
	GRVoiceManager::getCurStaffDraw(staffnum) = true;

	mCurGrStaff = mStaffMgr->getStaff(staffnum);
	assert(mCurGrStaff);

	// definitily not needed any longer 
	// (now that we use GSNF (Guido Semantic Normal Form)
	// Don't know, wether I need this - probably...
	// no longer needed 
	// grnotefactory->setGRStaff(mCurGrStaff);

	// mCurGrStaff->setTagList(fGRTags);

	// these are the positiontags that are open now 

	// Note: we can be sure, that in each case,
	// there is a TAG at the current position
	// (either we are at the beginning of the voice,
	// then there must be at least a clef introduced;
	// if we are in continuation, a clef has been
	// introduced as well in 
	// ARMusicalVoice::doAutoCheckStaffStateTags)

	// this means, that all curpositiontags are "open" ones, that is, they have not just been started.

	// just ignore this: this procedure is only
	// called once. There are NO OPEN TAGS!
}

/** \brief Actually does a newSystem or a newPage-break.

	if the tag at the current position is a 
	newSystem or a newPage-Tag, then the position is incremented.
*/
int GRVoiceManager::DoBreak(const TYPE_TIMEPOSITION & tp,
								int system_or_page)
{
	if (fVoiceState->curtp > tp)
	{
		// in the middle of an event!
		assert(false);
		// this can not be, because breaks are only done at positions, that are inbetween events.
		cerr << "GRVoiceManager::DoBreak Warning: curtp > tp (" << fVoiceState->curtp << " " << tp << ")" << endl;
	}
	else if (fVoiceState->curtp < tp)
	{
		assert(false);		// then we have a problem
	}

	ARMusicalObject *o = NULL;
	if (fVoiceState->vpos)
        o = arVoice->GetAt(fVoiceState->vpos);

    if (o && static_cast<ARPossibleBreak *>(o->isARPossibleBreak()))
        arVoice->GetNext(fVoiceState->vpos, *fVoiceState);
    else if ( o && static_cast<ARNewSystem *>(o->isARNewSystem())) {
//		debugstate ("before next:", fVoiceState->getCurStateTags());
		arVoice->GetNext(fVoiceState->vpos,*fVoiceState);
//		debugstate ("after next:", fVoiceState->getCurStateTags());
	}
    else if (o && static_cast<ARNewPage *>(o->isARNewPage()))
        arVoice->GetNext(fVoiceState->vpos,*fVoiceState);

	// newSystem or newPage we need to get a new mCurGrStaff... This automatically adds the startglue 
	if (system_or_page == 1 || system_or_page == 2)
	{
		// now, we need to handle the break within the voice, so that rod-creation works...
		grvoice->lastrod = NULL;
		// looking for newlines...
		grvoice->mIsNewLine = 1;
		grvoice->firstPositionInLine = NULL;
		grvoice->lastsprpos = NULL;

		// now we have to read the begintags...
		// this ensures, that the staff-Tag is read right after a newSystem-tag
		// and also the \pageFormat-Tag after a \newPage tag. 
		// Then, the necessary steps can be taken by the StaffManager/VoiceManager etc...
		ReadBeginTags(fVoiceState->curtp);

		// in order to pass the on/off information of the staff from a system to another...
		bool isOn = true;
		if(mCurGrStaff)
			isOn = mCurGrStaff->isStaffEndOn();
		mCurGrStaff->setNextOnOff(isOn);

		mStaffMgr->prepareStaff(staffnum);
		mCurGrStaff = mStaffMgr->getStaff(staffnum);

		mCurGrStaff->setOnOff(isOn);

		// this adds the two start glues...
		mCurGrStaff->BeginStaff(mStaffMgr);
		assert(mCurGrStaff);

		curtp = fVoiceState->curtp;
		beginOpenTags();				// then we can handle the openTags...
		fLastbar = NULL;					// there is no barline !
		fLastnonzeroevent = NULL;		// and no nonzeroevent...
	}
	else if (system_or_page == 3)
	{	
		// in order to pass the on/off information of the staff from a system to another...
		bool isOn = true;
		if(mCurGrStaff)
			isOn = mCurGrStaff->isStaffEndOn();

		mCurGrStaff = mStaffMgr->getStaff(staffnum);	// we are now working with slices...
		
 		mCurGrStaff->setOnOff(isOn);
	}

	return 1;
}


void GRVoiceManager::AddRegularEvent (GREvent * ev)
{
	if (ev && (ev->getNeedsSpring() == -1))
	{
		// then I need the springID of curglobalstem or curgloballocation
		GRNotationElement *firstEl = NULL;
		if (curgloballocation || curglobalstem)
		{
			if (curgloballocation)	firstEl = curgloballocation->getFirstEl();
			else					firstEl = curglobalstem->getFirstEl();
		}
		mStaffMgr->AddGRSyncElement(ev, mCurGrStaff, firstEl->getSpringID(), grvoice, firstEl);
	}
	else mStaffMgr->AddGRSyncElement(ev, mCurGrStaff, voicenum, grvoice);
}


/** \brief Iterates through the voice.

	dependant on the timeposition and filltagmode different behaviour occurs:
	if curtp>timepos then timepos is set to curtp and  CURTPBIGGER_ZEROFOLLOWS or CURTPBIGGER_EVFOLLOWS is returned.
	if curtp<timepos assert(false) is done.

	if we are at the end of the voice, ENDOFVOICE is returned
	dependant on filltagmode:

	filltagmode == 1 -> 
	if curtag==newSystem/newPage, NEWSYSTEM or NEWPAGE is returned

	if there are tags or events with dur==0 at the current VoicePosition, 
	than the respective NotationElements are created (tags that do not have a graphical representation are also 
	parsed one after another anyway -> to optimize, one could introduce an internal loop here...) 
	and DONE_ZEROFOLLOWS, DONE_EVFOLLOWS or DONE is returned
	(when the next element is a tag DONE_ZEROFOLLOWS is returned. 
	 If the next element is an event then DONE_EVFOLLOWS is returned. 
	 If there is no next element DONE is returned).

	if there is an event with dur>0 than MODEERROR is returned.

	filltagmode == 0:
	if there is an event at curtp then the GRNotationElement is created,the curtp is incremented; 
	DONE_ZEROFOLLOWS DONE_EVFOLLOWS or DONE is returned (dependant on next element).
	if there is no event here, then assert(false) is done (because this is not allowed to happen;
	otherwise, other voices could have progressed past the currentTP without handling Tags in a proper way).
*/
static const char* toText(int code) {
	switch (code) {
		case GRVoiceManager::CURTPBIGGER_ZEROFOLLOWS: 	return "CURTPBIGGER_ZEROFOLLOWS";
		case GRVoiceManager::CURTPBIGGER_EVFOLLOWS:		return "CURTPBIGGER_EVFOLLOWS";
		case GRVoiceManager::NEWSYSTEM:			return "NEWSYSTEM";
		case GRVoiceManager::NEWPAGE:			return "NEWPAGE";
		case GRVoiceManager::PBREAK:			return "PBREAK";
		case GRVoiceManager::MODEERROR:			return "MODEERROR";
		case GRVoiceManager::ENDOFVOICE:		return "ENDOFVOICE";
		case GRVoiceManager::DONE:				return "DONE";
		case GRVoiceManager::DONE_ZEROFOLLOWS:	return "DONE_ZEROFOLLOWS";
		case GRVoiceManager::DONE_EVFOLLOWS:	return "DONE_EVFOLLOWS";
	}
	return "UNKNOWN";
}

int GRVoiceManager::Iterate(TYPE_TIMEPOSITION &timepos, int filltagmode)
{
	int retCode = MODEERROR;

//	int vnum = getARVoice()->getVoiceNum();
//cerr << vnum << " GRVoiceManager::Iterate at pos " << timepos << " fCurrentTrill " << (void*)fCurrentTrill << endl;

	if (fVoiceState->vpos == NULL)
        return ENDOFVOICE;
	
	ARMusicalObject * obj = arVoice->GetAt(fVoiceState->vpos);

	if (fVoiceState->curtp > timepos) {
		timepos = fVoiceState->curtp;
		if (obj->getDuration() == DURATION_0) {
//cerr << vnum << " GRVoiceManager::Iterate return CURTPBIGGER_ZEROFOLLOWS " << obj << endl;
			return CURTPBIGGER_ZEROFOLLOWS;
		}
//cerr << vnum << " GRVoiceManager::Iterate return CURTPBIGGER_EVFOLLOWS " << obj << endl;
		return CURTPBIGGER_EVFOLLOWS;
	}

	if (filltagmode) {
        ARNewSystem *tmp = static_cast<ARNewSystem *>(obj->isARNewSystem());
		if (tmp) {
			if (tmp->getDY() && tmp->getDY()->TagIsSet()) // then we have a distance to the next system...
				mStaffMgr->setSystemDistance(tmp->getDY()->getValue(mCurGrStaff->getStaffLSPACE()), *this);
			return NEWSYSTEM;
		}
        else if (static_cast<ARNewPage *>(obj->isARNewPage()))
            return NEWPAGE;
		else if (static_cast<ARPossibleBreak *>(obj->isARPossibleBreak())) {
			pbreakval = static_cast<ARPossibleBreak *>(obj)->getValue();
//cerr << vnum << " GRVoiceManager::Iterate 	return PBREAK " << endl;
			return PBREAK;
		} 

		if (obj->getDuration() == DURATION_0) {
			// now we have a tag (no position tag!) or an event with duration 0, handle it...
			if (ARMusicalEvent::cast(obj)) {
				retCode = IterateNoDurEvent (obj, timepos);
//cerr << vnum << " GRVoiceManager::IterateNoDurEvent " << obj << " \t" << toText(retCode) << endl;
			}
			else {
				retCode = IterateTag(obj);
//cerr << vnum << " GRVoiceManager::IterateTag 		" << obj << " " << obj->getRelativeTimePosition() << " \t" << toText(retCode) << endl;
			}
		}
	}
	else {			// filltagmode == 0
		// We give to the object the information about the state on-off of the staff
		obj->setDrawGR(GRVoiceManager::getCurStaffDraw(staffnum) && obj->getDrawGR());

		if (obj->getDuration() == DURATION_0) {
			assert(false);
			// This MUST not happen, because then, other voices can already have progressed...
			// return MODEERROR;
		}
		else {	// handle the event...
			retCode = IterateEvent (ARMusicalEvent::cast(obj), timepos);
//cerr << vnum << " GRVoiceManager::IterateEvent 		" << obj << " \t" << toText(retCode) << endl;
		}
	}
	return retCode;
}

//-----------------------------------------------------------------------------------------
int GRVoiceManager::IterateNoDurEvent(ARMusicalObject * obj, const TYPE_TIMEPOSITION& timepos)
{
	// Then we create an EMPTY-Event handling all the startPTags and endPTags...
	GRTrill* savedCurrentTrill = fCurrentTrill;		// disable trills handling with chords
	fCurrentTrill = 0;
	checkStartPTags(fVoiceState->vpos);
	fCurrentTrill = savedCurrentTrill;				// retore trills handling
	
	GREvent *ev = NULL;
	if (mCurGrace) {
		// then we have to create a GRACE-Note (which is a real note, no duration but somewhat
		// drawn as well... the associations are set regardless...)
		// this must be the parameter from ARGrace...
		// check whether this is an empty-event anyhow...
		TYPE_DURATION dur(obj->getDuration());

		if (fVoiceState->fCurdispdur)
			dur = fVoiceState->fCurdispdur->getDisplayDuration();

		ev = CreateGraceNote(timepos,obj,dur);
		// this adds the Grace-Note as a regular  event...
		AddRegularEvent(ev);
	}
	else {
		// careful, what happens to dispDur !!!!
		if (fVoiceState->fCurdispdur != NULL && fVoiceState->fCurdispdur->getDisplayDuration() > DURATION_0) {
			if (obj->isARTab())
				ev = CreateTab(timepos,obj);
			else if (obj->isARNote())
				ev = CreateNote(timepos, obj);
			else if (obj->isARRest())
				ev = CreateRest(timepos, obj);
		}
		// changed on Apr 19 2011 DF
		// the test has been moved out of CreateEmpty
		else if (obj->getDuration() <= DURATION_0)
			ev = CreateEmpty (timepos, obj);
		else
			ev = 0;

		if (ev) {
			AddRegularEvent (ev);
			checkCluster(ev);
		}
	}
	return endIteration();
}

//-----------------------------------------------------------------------------------------
int GRVoiceManager::IterateTag	(ARMusicalObject * obj)
{
	GRNotationElement *grne = parseTag(obj);
	if (grne) {
		// tag was handled... here, we distinguish the different graphical TAG-Types
		GRTag *tag = dynamic_cast<GRTag *>(grne);
		
		if (tag && (tag->getTagType() == GRTag::SYSTEMTAG))
			mStaffMgr->AddSystemTag(grne,mCurGrStaff,voicenum);
		else if (tag && (tag->getTagType() == GRTag::PAGETAG))
			mStaffMgr->AddPageTag(grne,mCurGrStaff,voicenum);
		else if (grne->getNeedsSpring()) {
			if (curglobalstem || curgloballocation) {
				GuidoTrace("Tag with spring in a globalstem or global location!");
				// The tag is no longer added but gets associated with
				// the curglobalthing that is active at that point...
				GRNotationElement *firstEl = NULL;
				if (curgloballocation)
					firstEl = curgloballocation->getFirstEl();
				else if (curglobalstem)
					firstEl = curglobalstem->getFirstEl();

				grne->setNeedsSpring(-1);
				mStaffMgr->AddGRSyncElement(grne, mCurGrStaff, firstEl->getSpringID(), grvoice, firstEl);
			}
			else
				mStaffMgr->AddGRSyncElement(grne, mCurGrStaff,voicenum,grvoice);
		}
	}
	else {
		const ARMusicalTag *armt = static_cast<const ARMusicalTag *>(obj->isARMusicalTag());
		if (!armt || !armt->IsStateTag())
			cerr << "Warning: " << armt->getGMNName() << " not handled" << endl;
	}
	return endIteration();
}

//-----------------------------------------------------------------------------------------
// intended to multi voices staves to catch notes that cover each other
// e.g. the same note appears as a quarter and as a half note
// takes a list of notes as parameter, all the notes are supposed to be at the same time position
void GRVoiceManager::checkHiddenNotes(const std::vector<GRSingleNote *>& notes)
{
	if (notes.size() < 2) return;

	for (int i=0; i < notes.size()-1; i++) {
		GRSingleNote * n = notes[i];
		const ARNote* arn = n->getARNote();
		TYPE_DURATION nd = arn->getDuration();
		for(int j=i+1; j < notes.size(); j++){
			GRSingleNote * m = notes[j];
			const ARNote* arm = m->getARNote();
			if ((arn->getMidiPitch() == arm->getMidiPitch()) && (n->getGRStaff() == m->getGRStaff()) && (n->getOffset().x == m->getOffset().x)) {
				GRSingleNote* tohide = nullptr;
				TYPE_DURATION md = arm->getDuration();
				if (nd > md) {
					if (nd > DURATION_4) tohide = m;
				}
				else if (nd < md) {
					if (md > DURATION_4) tohide = n;
				}
				if (tohide && tohide->getStyle().empty())
					tohide->hideHead();
			}
		}
	}
}

//-----------------------------------------------------------------------------------------
int GRVoiceManager::IterateEvent(ARMusicalEvent * arev, TYPE_TIMEPOSITION &timepos)
{
	static TYPE_TIMEPOSITION currentTime;
	
	// This creates the graphical representation for position-tags, that start at the current position...
	checkStartPTags(fVoiceState->vpos);
	GREvent * grev = NULL;

	if (timepos != currentTime) {
		if (timepos > currentTime) checkHiddenNotes (fCurrentNotesTP);
		currentTime = timepos;
		fCurrentNotesTP.clear();
	}
	if (arev->isARTab())
		grev = CreateTab(timepos,arev);
    else if (arev->isARNote()) {
		grev = CreateNote(timepos, arev);
		GRSingleNote * note = grev->isSingleNote();
		if (note && note->getARNote()) {
			fCurrentNotesTP.push_back (note);
		}
	}
	else if (arev->isARRest())
		grev = CreateRest(timepos, arev);
	
	assert(grev);
	if (grev->getDuration() > DURATION_0)
		fLastnonzeroevent = grev;

	if (toadd && toadd->empty() == false )
	{
		GuidoPos mypos = toadd->GetHeadPosition();
		while (mypos)
		{
			GRNotationElement * el = dynamic_cast<GRNotationElement *>(toadd->GetNext(mypos));
			el->addAssociation(grev);
			grev->addAssociation(el);
		}
		toadd->RemoveAll();
	}
	AddRegularEvent(grev);
	int retCode = endIteration();
	if (fVoiceState->vpos && dynamic_cast<ARChordComma*>(arVoice->GetAt(fVoiceState->vpos)) && (getARVoice()->getVoiceNum() > getStaffNum() )) {
/*
	2023-06-24 DF. introduced to fix issue #169
	On multi voices staff, notes of chords may be created after a bar on the preceding voice (which resets the key)
	A normal solution would be to reorder the events creation so that chords are entirely parsed at a given time position.
	This is what the inactive IterateChord method do, but it infers very serioulsy with the space since it change
	also the order of the spring and rods creation.
 */
		getCurStaff()->inhibitNextReset2Key();
	}

	timepos = arev->getRelativeEndTimePosition();
	return retCode;
}

//-----------------------------------------------------------------------------------------
int GRVoiceManager::IterateChord (const TYPE_TIMEPOSITION& timepos)
{
	int retCode = MODEERROR;
	ARMusicalObject *obj = arVoice->GetAt(fVoiceState->vpos);
//cerr << " => GRVoiceManager::IterateChord " << obj << endl;
	while (obj && (obj->getRelativeTimePosition() == timepos) && fVoiceState->vpos) {
		if (obj->getDuration() == DURATION_0) {
			// now we have a tag (no position tag!) or an event with duration 0, handle it...
			if (ARMusicalEvent::cast(obj)) {
				if (obj->isEmptyNote()) return DONE_EVFOLLOWS;
				retCode = IterateNoDurEvent (obj, timepos);
//cerr << " ==> GRVoiceManager::IterateChord -> IterateNoDurEvent " << obj << " \t\t" << toText(retCode) << endl;
			}
			else {
				retCode = IterateTag(obj);
//cerr << " ==> GRVoiceManager::IterateChord -> IterateTag 		" << obj << " " << obj->getRelativeTimePosition() << " \t" << toText(retCode) << endl;
			}
		}
		else {
			break;
		}
		obj = arVoice->GetAt(fVoiceState->vpos);
	}
	return retCode;
}

//-----------------------------------------------------------------------------------------
void GRVoiceManager::checkCluster(GREvent *ev)
{
	GRNote *grnote = dynamic_cast<GRNote *>(ev);
	if (grnote) {
		if (!mCurCluster) {
			if (grnote->getClusterNoteBoolean()) {
				mCurCluster = grnote->createCluster(curnoteformat);
				mCurrentClusterNoteNumber++;
			}
		}
		else {
			if (grnote->getClusterNoteBoolean() && mCurrentClusterNoteNumber <= mCurCluster->getNoteCount()) {
				if (mCurrentClusterNoteNumber < 2)
					grnote->setGRCluster(mCurCluster);
				else
					grnote->setGRCluster(mCurCluster, false);
				mCurrentClusterNoteNumber++;
			}
		}
		if (mCurCluster && mCurrentClusterNoteNumber == mCurCluster->getNoteCount()) {
			mCurCluster = NULL; 
			mCurrentClusterNoteNumber = 0;
		}
	}
}

//-----------------------------------------------------------------------------------------
int GRVoiceManager::endIteration ()
{
		GuidoPos prevpos = fVoiceState->vpos;
		// increment the curvoice... increment the position...
		arVoice->GetNext(fVoiceState->vpos, *fVoiceState);
		
		// Check Ending Tags...
		if (fVoiceState->removedpositiontags)
			checkEndPTags(prevpos);
		if (fVoiceState->vpos) {
			// check, what the next element in the voice is (tag, zero-event or event)
			ARMusicalObject *o = arVoice->GetAt(fVoiceState->vpos);
			
			//we give to the object the information about the state on-off of the staff
			o->setDrawGR (GRVoiceManager::getCurStaffDraw(staffnum) && o->getDrawGR());

			if ((o->getDuration() == DURATION_0))
				return DONE_ZEROFOLLOWS;
			assert(ARMusicalEvent::cast(o));
			return DONE_EVFOLLOWS;
		}
		return DONE;
}

void GRVoiceManager::checkFillBar (GRTagARNotationElement* bar)
{
	GREvent * lastEv = fLastnonzeroevent;
	if (lastEv) // && (lastEv->getAbstractRepresentation()->getDuration() > DURATION_4))
	{
		const ARMeter* meter = fVoiceState ? fVoiceState->curMeter() : 0;
		bool filled = meter ? meter->getMeterDuration() == lastEv->getDuration() : false;

		if (meter && (fLastbar || mCurGrStaff->secglue || lastEv->getRelativeTimePosition() == DURATION_0))
		{
			GRNotationElement * lastel = 0;
			if (fLastbar)
				lastel = fLastbar;
			else if (mCurGrStaff->secglue)
				lastel = mCurGrStaff->secglue;

			const TYPE_TIMEPOSITION tp1 ( lastEv->getRelativeTimePosition());
			if (tp1 == DURATION_0 || (lastel && tp1 == lastel->getRelativeTimePosition())) {
				if (lastEv->getRelativeEndTimePosition() == bar->getRelativeTimePosition()
					&& lastEv->getGRStaff() == bar->getGRStaff())
				{
					// then we have to make this event the ultimate barfiller !
					lastEv->setFillsBar(true, lastel, bar, filled);
					bar->addAssociation(lastEv);
				}
			}
		}
	}
	fLastnonzeroevent = NULL;
	fLastbar = bar;
}

/** \brief Creates the graphical Objects from the given Tag.

	Part of the AR to GR process.
*/
GRNotationElement * GRVoiceManager::parseTag(ARMusicalObject * arOfCompleteObject)
{
	const TYPE_TIMEPOSITION & von = arOfCompleteObject->getRelativeTimePosition();
	GRNotationElement * grne = 0;

	const std::type_info & tinf = typeid(*arOfCompleteObject);
	const ARMusicalTag * mytag = static_cast<const ARMusicalTag *>(arOfCompleteObject->isARMusicalTag());
	if (mytag)
	{
		if (mytag->getRange()){
			GuidoTrace("range without range tag");
		}
	}
    else if (static_cast<const ARDummyRangeEnd *>(arOfCompleteObject->isARDummyRangeEnd()))
	{
	}
	// DF - 24/08/2009 - assert commented to avoid spurious exit
	// due to dynamic score coding or multiple fermata in a chord with variable length notes
	// else assert(false); // don't know...
	
	// Here the most common tags must be at the top of the if / elseif tests, for better performances.
	// first, we look whether it is a state tag (that is one like \staff, \stemsUp, \colour... 
	if (mytag && mytag->IsStateTag() && parseStateTag(mytag))
	{
		// is handled...
	}
	else if (tinf == typeid(ARClef)) 
	{
		grne = mCurGrStaff->AddClef(static_cast<const ARClef *>(arOfCompleteObject));
		
		// here the baseline etc. will be changed
		if (grne) fMusic->addVoiceElement(arVoice,grne);
		
	}
	else if (tinf == typeid(ARMeter))
	{		
		grne = mCurGrStaff->AddMeter( static_cast<const ARMeter*>(arOfCompleteObject));
		fMusic->addVoiceElement(arVoice,grne);		
	}
	else if (tinf == typeid(ARNaturalKey))
	{
		// this is done to naturalize a key
		// before a new key is set...
		GRKey * grkey = mCurGrStaff->AddKey( static_cast<const ARKey *>( arOfCompleteObject));
		if (grkey)
		{
			fMusic->addVoiceElement( arVoice,grkey);
			grne = grkey;
		}
	}
	else if (tinf == typeid(ARKey))
	{
		// it may happen, that a key is not  created...
		GRKey * grkey = mCurGrStaff->AddKey( static_cast<const ARKey *>(arOfCompleteObject));
		if (grkey)
		{
			fMusic->addVoiceElement( arVoice,grkey );
			grne = grkey;
		}	
	}
	else if (tinf == typeid(ARSecondGlue))
	{
		// then, the STAFF gets a second glue...
		GRGlue * glue = new GRGlue(mCurGrStaff,-1);
		glue->setRelativeTimePosition(arOfCompleteObject->getRelativeTimePosition());
		mCurGrStaff->AddSecondGlue(glue);
		fMusic->addVoiceElement(arVoice,glue);
		grne = glue;
	}
	else if (tinf == typeid(ARBar))
	{
		GRBar * grbar = mCurGrStaff->AddBar( static_cast<ARBar*>(arOfCompleteObject), von );
		grne = grbar;
		fMusic->addVoiceElement(arVoice, grne);
		checkFillBar (grbar);
	}
	else if (tinf == typeid(ARDoubleBar))
	{
		GRBar * grbar = mCurGrStaff->AddDoubleBar(static_cast<ARDoubleBar *>( arOfCompleteObject),von);
		grne = grbar;
		fMusic->addVoiceElement(arVoice, grne);
		checkFillBar (grbar);
	}
	else if (tinf == typeid(ARFinishBar))
	{
		GRBar * grbar = mCurGrStaff->AddFinishBar(static_cast<ARFinishBar *>( arOfCompleteObject),von);
		grne = grbar;
		fMusic->addVoiceElement(arVoice, grne);
		checkFillBar (grbar);
	}
	else if (tinf == typeid(ARRepeatBegin))
	{
		GRTagARNotationElement * grbar = mCurGrStaff->AddRepeatBegin (static_cast<ARRepeatBegin *>(arOfCompleteObject));
		grne = grbar;
		fMusic->addVoiceElement(arVoice, grne);
		checkFillBar (grbar);
	}
    else if (tinf == typeid(ARRepeatEnd)) 
    {
		GRRepeatEnd * grbar = mCurGrStaff->AddRepeatEnd(static_cast<ARRepeatEnd *>( arOfCompleteObject));
		if (grbar) {
			if (checkRepeatBeginNext())
				grbar->setSConst (100.0f);
			grne = grbar;
			fMusic->addVoiceElement(arVoice, grne);
			checkFillBar (grbar);
			grbar->updateBoundingBox();
		}
	}
/* range not any more supported - DF sept 1 2004
	else if (tinf == typeid(ARRepeatEndRangeEnd))
	{	
		grne = (mCurGrStaff->AddRepeatEndRangeEnd(
			static_cast<ARRepeatEndRangeEnd *>( arOfCompleteObject)));
		fMusic->addVoiceElement(arVoice,	grne);
	}
*/
	else if (tinf == typeid(ARText))
	{
		// this is a No-Range Text-Tag...
		GRText * grtxt = new GRText(mCurGrStaff, static_cast<const ARText *>(arOfCompleteObject));
		grtxt->setNeedsSpring(1);	// needs a Spring
		mCurGrStaff->AddTag(grtxt);
		fMusic->addVoiceElement(arVoice,grtxt);
		grne = grtxt;
	}
    else if (tinf == typeid(ARHarmony))
    {
        // this is a No-Range Text-Tag...
        GRHarmony * grtxt = new GRHarmony(mCurGrStaff, static_cast<const ARHarmony *>(arOfCompleteObject));

        grtxt->setNeedsSpring(1);	// needs a Spring
        mCurGrStaff->AddTag(grtxt);
        fMusic->addVoiceElement(arVoice,grtxt);
        
        grne = grtxt;
        
    }
	else if (tinf == typeid(ARLabel))
	{
		// this is a No-Range Text-Tag...
		GRText * grtxt = new GRText(mCurGrStaff, static_cast<const ARLabel *>(arOfCompleteObject));
		grtxt->setNeedsSpring(1);
		mCurGrStaff->AddTag(grtxt);
		fMusic->addVoiceElement(arVoice,grtxt);
		grne = grtxt;
	}
	else if (tinf == typeid(ARTempo))
	{
		GRTempo * grtempo = new GRTempo(mCurGrStaff, static_cast<const ARTempo *>(arOfCompleteObject));
		// not even added to the music, because this is used for RODS. System-Tags do
		// not get rods (not really)
		// fMusic->addVoiceElement(arVoice,grtempo);
		grne = grtempo;
	}
	else if (tinf == typeid(ARIntens))
	{
		grne = /* dynamic cast<GRNotationElement *>*/(
		mCurGrStaff->AddIntens(static_cast<const ARIntens *>( arOfCompleteObject)));
		fMusic->addVoiceElement(arVoice,	grne);
		curIntens = dynamic_cast<GRIntens *>(grne);
	}
	else if (tinf == typeid(ARFermata))
	{
		// This is a singular Fermata...  it should appear inbetween the current events...
		// Create a fermata that is spaced correctly...
		GRArticulation * grarti = new GRArticulation(mytag, LSPACE);
		grarti->setNeedsSpring(1);
		mCurGrStaff->addNotationElement(grarti);
		fMusic->addVoiceElement(arVoice,grarti);
		grne = grarti;
	}
	// Space-Tag
	else if (tinf == typeid(ARSpace))
	{
		// no space follow at the end ? ->
		// what about newline?
		// if (atEnd) return retval;
		const ARSpace * myspc = static_cast<const ARSpace *>(arOfCompleteObject);
		GRSpace * tmp = new GRSpace(myspc,0);
		tmp->setNeedsSpring(0);	
		
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARBreathMark)) {
		GRBreathMark * tmp = new GRBreathMark(static_cast<ARBreathMark *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARCoda)) {
		GRCoda * tmp = new GRCoda(static_cast<ARCoda *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARDaCapo)) {
		GRJump * tmp = new GRJump(static_cast<ARDaCapo *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARDaCapoAlFine)) {
		GRJump * tmp = new GRJump(static_cast<ARDaCapoAlFine *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARDaCoda)) {
		GRJump * tmp = new GRJump(static_cast<ARDaCoda *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARDalSegno)) {
		GRJump * tmp = new GRJump(static_cast<ARDalSegno *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARDalSegnoAlFine)) {
		GRJump * tmp = new GRJump(static_cast<ARDalSegnoAlFine *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARFine)) {
		GRJump * tmp = new GRJump(static_cast<ARFine *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARSegno)) {
		GRSegno * tmp = new GRSegno(static_cast<ARSegno *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARInstrument))
	{		
		grne = mCurGrStaff->AddInstrument(static_cast<ARInstrument *>( arOfCompleteObject));
		fMusic->addVoiceElement(arVoice, grne);
	}
	else if (tinf == typeid(ARMark))
	{
		// then we introduce a text...
		GRMark * grmark = new GRMark(mCurGrStaff, static_cast<ARMark *>(arOfCompleteObject));
		mCurGrStaff->AddTag(grmark);
		fMusic->addVoiceElement(arVoice,grmark);

		grne = grmark;
	}
	else if (tinf == typeid(ARNotations))
	{
		GRNotations * notation = new GRNotations(mCurGrStaff, static_cast<ARNotations *>(arOfCompleteObject));
		notation->setNeedsSpring(1);
		mCurGrStaff->AddTag(notation);
		fMusic->addVoiceElement(arVoice,notation);
		grne = notation;
	}
	else if (tinf == typeid(ARSpecial))
	{
		GRSpecial * tmp = new GRSpecial (static_cast<ARSpecial *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARTitle))
	{
		ARTitle * artitle = static_cast<ARTitle *>(arOfCompleteObject);
		grne = new GRPageText (artitle, NULL, artitle->getName(), artitle->getPageFormat());
	}
	else if (tinf == typeid(ARComposer))
	{
		ARComposer * arcomp = static_cast<ARComposer *>(arOfCompleteObject);
		GRPageText * tmp = new GRPageText (arcomp, NULL, arcomp->getName(), arcomp->getPageFormat());
		grne = tmp;
	}
	else if (tinf == typeid(ARFooter))
	{
		ARFooter * footer = static_cast<ARFooter *>(arOfCompleteObject);
		GRPageText * tmp = new GRPageText (footer, NULL, footer->getName(), footer->getPageFormat());
		grne = tmp;
	}
	else if (tinf == typeid(AROctava))
	{
		AROctava * tmp = static_cast<AROctava *>(arOfCompleteObject);
		if (fLastOctava) fGRTags->RemoveElement(fLastOctava);
		fLastOctava = mCurGrStaff->AddOctava(tmp);
		addGRTag (fLastOctava);
		grne = fLastOctava;
		fMusic->addVoiceElement(arVoice,grne);
	}
	else if (tinf == typeid(ARChordComma))
	{
		ARChordComma * tmp = static_cast<ARChordComma *>(arOfCompleteObject);
		grne = new GRTagARNotationElement(tmp, LSPACE );
		mCurGrStaff->AddTag(grne);
		fMusic->addVoiceElement(arVoice,grne);

	}
//---------------------------------------------------
// ARAlter implementation finalized as a position tag
// DF Apr. 20 2011
//
//	else if (tinf == typeid(ARAlter))
//	{
//		// This should not happen, at least not right now!
////		GuidoWarn("need to work on this, add ARAlter");
////		grne = NULL;	
//
//	}
	else if (tinf == typeid(ARDrHoos))
	{
		GRDrHoos * tmp = new GRDrHoos(static_cast<ARDrHoos *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
	else if (tinf == typeid(ARDrRenz))
	{
		GRDrRenz * tmp = new GRDrRenz(static_cast<ARDrRenz *>(arOfCompleteObject));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
		grne = tmp;
	}
    else if (tinf == typeid(ARSymbol))
	{
		// this is a No-Range Symbol-Tag...
		GRSymbol * grsymb = new GRSymbol(mCurGrStaff, static_cast<ARSymbol *>(arOfCompleteObject));		

		grsymb->setNeedsSpring(1);	// needs a Spring
		mCurGrStaff->AddTag(grsymb);
		fMusic->addVoiceElement(arVoice,grsymb);

		grne = grsymb;
	}
	else if (tinf == typeid(ARStaffOff))
	{
		// we set the current staff as off (first setting means that it should be given to all following staves)
		mCurGrStaff->setOnOff(false, von);
		// we remember the current state of the current staff, associated with its staffnum
		GRVoiceManager::getCurStaffDraw(staffnum) = false;
	}
	else if (tinf == typeid(ARStaffOn))
	{
		// we set the current staff as on (first setting means that it should be given to all following staves)
		mCurGrStaff->setOnOff(true, von);
		// we remember the current state of the current staff, associated with its staffnum
		GRVoiceManager::getCurStaffDraw(staffnum) = true;
	}
	else if (tinf == typeid(ARMusicalTag))
	{
		// Here, the not yet implemented tags are saved on the
		// Range-Stack, so that it doesn't get confused by
		// ARRangeEnd-Messages...
		ARMusicalTag * mt = static_cast<ARMusicalTag *>(arOfCompleteObject);
		if (mt->getRange())
		{
			GRTag * grt = new GRTag();
			addGRTag(grt);
		}
	}
	else
		grne = NULL;

	if(grne)
		grne->setDrawOnOff(GRVoiceManager::getCurStaffDraw(staffnum));

	// This sets the new state in the staff changes meter,clef and key-settings
	mStaffMgr->setStaffStateTag(mytag,staffnum);
	return grne;
}


/** \brief Sets all the Position-Tags in the current voice-state
*/
//void GRVoiceManager::addStartPTags()			<<<<=== unused
//{
//	PositionTagList * ptaglst = fVoiceState->curpositiontags;
//	if (ptaglst == 0) return;	// nothing to do ?
//
//	GuidoPos pos = ptaglst->GetHeadPosition();
//	while (pos)
//	{
//		ARPositionTag *  apt = ptaglst->GetNext(pos);
//		parsePositionTag(apt);
//	}
//
//}

/** \brief Checks whether there are any position-Tags that need to be added to
	the current-Tag-Lists, due to Position.
*/
void GRVoiceManager::checkStartPTags(GuidoPos tstpos)
{
//	ARPositionTag * apt;

    // look if there are Tags with matching start-positions...
	// new version: look only in newly added positiontags!
	PositionTagList * ptaglst = fVoiceState->addedpositiontags;
	if (!ptaglst) return;									// nothing to do ?

	GuidoPos pos = ptaglst->GetHeadPosition();
	while (pos)
	{
		ARPositionTag * apt = ptaglst->GetNext(pos);
		if (apt->getPosition() == tstpos)
		{ // found a match!
			parsePositionTag(apt);
		}
		// the following two lines only work when using the addedpositiontagslist!
		else assert(false);
	}
}

void GRVoiceManager::parsePositionTag (ARPositionTag *apt)
{
	ARMusicalTag * mtag = dynamic_cast<ARMusicalTag *>(apt);
	// added on sept. 2008 : to catch the bug with \tagBegin \tagEnd form 
	if (mtag && mtag->getError()) return;	// do nothing in case of error -- added

	//we give to the tag the information about the state on-off of the staff
	mtag->setDrawGR(GRVoiceManager::getCurStaffDraw(staffnum) && mtag->getDrawGR());


	const std::type_info & tinf = typeid(*apt);

// trills are created by the note, which breaks the standard mechanisms
// that should be change
	if (tinf == typeid(ARTrill))
	{
		// this position tag is somewhat different, because it changes the appearance of chords...
		// only the first voice of  a chord is actually drawn, the rest is just ignored...
		// because it is only needed for playback...
		GRTrill * grtrill = new GRTrill(mCurGrStaff, static_cast<const ARTrill *>(apt));
		addGRTag(grtrill);
		mCurGrStaff->AddTag(grtrill);
		fMusic->addVoiceElement(arVoice,grtrill);
		if (fCurrentTrill)	setTrillNext(grtrill);
		fCurrentTrill = grtrill;
	}
	else if (tinf == typeid(ARVolta))
	{
		GRVolta * tmp = new GRVolta(mCurGrStaff, static_cast<const ARVolta *>(apt));
		addGRTag(tmp,0);
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
//		grne = tmp;
	}
	else if (tinf == typeid(ARGrace))
	{
		// This position tag is somewhat different from the other ones...
		GRGrace * grgrace = new GRGrace(mCurGrStaff, static_cast<const ARGrace *>(apt));
        mCurGrace = grgrace;
		addGRTag(grgrace);
		mCurGrStaff->AddTag(grgrace);
		fMusic->addVoiceElement(arVoice,grgrace);
	}
	else if (tinf == typeid(ARAutoBeam))
	{
		// Beams that start at the end of the line are parsed for the next line...
		// ignored for now, maybe do something later?
		// if (atEnd) return retval;

		// d.h. es ist ein AutoBeam-Tag angekommen!
		GRAutoBeam * grbeam = new GRAutoBeam(
			mCurGrStaff, static_cast<const ARAutoBeam *>(apt));

		// this only needs to be added, if the Beam is really drawn...
		// otherwise it is not interesting...
		if (grbeam->getError() == 0)
		{
			// this tag will be handled differently to the standard RangeTags
			// ATTENTION... AddTail!!!!
			addGRTag(grbeam,0);
			mCurGrStaff->AddTag(grbeam);
			fMusic->addVoiceElement(arVoice,grbeam);
		}
		else delete grbeam;
	}
	else if (tinf == typeid(ARCrescendo))
	{
		GRCrescendo * grcresc = new GRCrescendo(mCurGrStaff, static_cast<const ARCrescendo *>(apt));
		addGRTag(grcresc,0);
		mCurGrStaff->AddTag(grcresc);
		fMusic->addVoiceElement(arVoice,grcresc);
	}
	else if (tinf == typeid(ARDiminuendo))
	{
		GRDiminuendo * grdim = new GRDiminuendo(mCurGrStaff, static_cast<const ARDiminuendo *>(apt));
		addGRTag(grdim,0);
		mCurGrStaff->AddTag(grdim);
		fMusic->addVoiceElement(arVoice,grdim);
	}
	else if (tinf == typeid(ARSlur))
	{
		// No slur starts at the end... if (atEnd) return retval;
		// the graphical slur...
		GRSlur * grslur = new GRSlur(mCurGrStaff, static_cast<const ARSlur *>(apt));
		
		// the slur is added to the Tag-Stack...
		addGRTag(grslur,0);
		mCurGrStaff->AddTag(grslur);
		fMusic->addVoiceElement(arVoice,grslur);
	}
	else if (tinf == typeid(ARTie))
	{
		// No tie starts at the end. if (atEnd) return retval;
		const ARTie * tie = static_cast<const ARTie *>(apt);
		GRTie * grtie = new GRTie(mCurGrStaff, tie, tie->hideAccidentals());
		addGRTag(grtie,0);
		mCurGrStaff->AddTag(grtie);
		fMusic->addVoiceElement(arVoice,grtie);

	} 
	else if (tinf == typeid(ARBeam))
	{
		// Beams that start at the end of the line are parsed for the next line...
		// if (atEnd) return retval;
		GRBeam * grbeam = new GRBeam(mCurGrStaff, static_cast<const ARBeam *>(apt));
		addGRTag(grbeam,0);
		mCurGrStaff->AddTag(grbeam);
		fMusic->addVoiceElement(arVoice,grbeam);
	}
	else if (tinf == typeid(ARArpeggio))
	{
		// Beams that start at the end of the line are parsed for the next line...
		// if (atEnd) return retval;
		GRArpeggio * grarp = new GRArpeggio(mCurGrStaff, static_cast<const ARArpeggio *>(apt));
		addGRTag(grarp,0);
		mCurGrStaff->AddTag(grarp);
		fMusic->addVoiceElement(arVoice,grarp);
	}
	else if (tinf == typeid(ARFeatheredBeam))
	{
		GRBeam * grbeam = new GRBeam(mCurGrStaff, static_cast<const ARFeatheredBeam *>(apt));
		addGRTag(grbeam,0);
		mCurGrStaff->AddTag(grbeam);
		fMusic->addVoiceElement(arVoice,grbeam);
	}
	else if (tinf == typeid(ARText))
	{
		const ARText * artxt = static_cast<const ARText *>(apt);
		GRText * gtext = new GRText(mCurGrStaff, artxt);
		// this is a range-tag... therefore no spring is needed... it is handled
		// by the event at this timeposition
		// adds the tag at the end...
		addGRTag(gtext,0);
		mCurGrStaff->AddTag(gtext);
		fMusic->addVoiceElement(arVoice,gtext);
	}
	else if (tinf == typeid(ARLabel))
	{
		const ARLabel * artxt = static_cast<const ARLabel *>(apt);
		GRText * gtext = new GRText(mCurGrStaff,artxt);
		// adds the tag at the end...
		addGRTag( gtext, 0 );
		mCurGrStaff->AddTag(gtext);
		fMusic->addVoiceElement(arVoice,gtext);
	}
/* ARRepeatEnd is not any more a range tag. DF August 30 2004
	else if (tinf == typeid(ARRepeatEnd))
	{
		ARRepeatEnd * arRepeat = static_cast<ARRepeatEnd *>(apt);
        if (!arRepeat->getRange()) {
            GRRepeatEnd * grRepeat = mCurGrStaff->AddRepeatEnd( arRepeat );
            
            // addGRTag( grRepeat, 0 ); // (JB) Added.
            // mCurGrStaff->AddTag( grRepeat );

            // if ARRepeatEnd is a position-Tag then it has a range...
            // We have a problem here!...
            // ATTENTION, this is wrong!
            
            fMusic->addVoiceElement( arVoice, grRepeat );
        }
	}
*/
	else if (tinf == typeid(ARFingering))
	{
		const ARFingering * artxt = static_cast<const ARFingering *>(apt);
//		GRText * gtext = new GRText(mCurGrStaff,artxt);
//		gtext->mustFollowPitch( true );
		GRText * gtext = new GRFingering(mCurGrStaff,artxt);
		// adds the tag at the end...
		addGRTag(gtext,0);

		mCurGrStaff->AddTag(gtext);
		fMusic->addVoiceElement(arVoice,gtext);
	}
	else if (tinf == typeid(ARAccidental))
	{
		const ARAccidental * aracc = static_cast<const ARAccidental *>(apt);
		GRRange * range = new GRRange(mCurGrStaff, aracc);
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);		
	}
	else if (tinf == typeid(ARFermata))
	{
		// this is a fermata-position-tag when it is active, every event gets a fermata-tag.
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARFermata *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}
	else if (tinf == typeid(ARTremolo))
	{
		GRTremolo * tmp = new GRTremolo(mCurGrStaff, static_cast<const ARTremolo *>(apt));
		addGRTag(tmp);
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
	}
	else if (tinf == typeid(ARRitardando))
	{
		GRTempoChange * tmp = new GRTempoChange(mCurGrStaff, static_cast<const ARAccelerando *>(apt), "rit.");
		addGRTag(tmp);
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
	}
	else if (tinf == typeid(ARAccelerando))
	{
		GRTempoChange * tmp = new GRTempoChange(mCurGrStaff, static_cast<const ARAccelerando *>(apt), "accel.");
		addGRTag(tmp);
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
	}
	else if (tinf == typeid(ARStaccato))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARStaccato *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}
	else if (tinf == typeid(ARPizzicato))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARPizzicato *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice, range);
	}
	else if (tinf == typeid(ARAccent))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARAccent *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}
	// ARAlter is implementated as a position tag
	// DF Apr. 20 2011
	else if (tinf == typeid(ARAlter))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARAlter *>(apt));

		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}
	else if (tinf == typeid(ARMarcato))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARMarcato *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}
	else if (tinf == typeid(ARBow))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARBow *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}
	else if (tinf == typeid(ARTenuto))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARTenuto *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}
	else if (tinf == typeid(ARHarmonic))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARHarmonic *>(apt));
		
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice, range);
	}
	else if (tinf == typeid(ARBase))
	{
		// then, we have a base-tag (this is just like a tuplet-tag)
		// we have to draw a tuplet-range...
	}
	else if (tinf == typeid(ARDisplayDuration))
	{
		// this is handled with the current Voice state (that is cuvst.dispdur)
		// no action is required here...
	}
	else if (tinf == typeid(ARTuplet))
	{
		GRTuplet * grtupl = new GRTuplet(mCurGrStaff, static_cast<const ARTuplet *>(apt));

		addGRTag(grtupl, 0);
		mCurGrStaff->AddTag(grtupl);
		fMusic->addVoiceElement(arVoice,grtupl);
	}
	else if (tinf == typeid(ARShareStem))
	{
		GRGlobalStem * othergstem = 0;

		// check whether there is a chord-tag with the same label active...
		if (curchordtag)
		{
			const NVstring & label = curchordtag->getLabel();
			// check whether there is another systemtag with this label, that is different from
			// the curchordtag...
			GRSystemSlice * grsystemslice = mCurGrStaff->getGRSystemSlice();

			othergstem = mStaffMgr->getOtherGlobalStem(grsystemslice,this, label);		
		}

		if (othergstem)
		{
			// we have another globalstem that has the same CHORD-LABEL!
			addGRTag(othergstem);
			curglobalstem = othergstem;

			// we do not add this to the staff or the voice...
			// but the associations are set as if the stem was in the current voice.

			// we need a dummy globalstem that is inactive...
			GRGlobalStem * grgstem = new GRGlobalStem (mCurGrStaff,static_cast<const ARShareStem *>(apt), curstemstate,
				fVoiceState->fCurdispdur, curnoteformat);

			grgstem->setError(1);
			addGRTag(grgstem);
			
			mCurGrStaff->AddTag(grgstem);
			fMusic->addVoiceElement(arVoice,grgstem);
		}
		else
		{
			GRGlobalStem * grgstem = new GRGlobalStem(mCurGrStaff,
							static_cast<const ARShareStem *>(apt), curstemstate, fVoiceState->fCurdispdur, curnoteformat);
				addGRTag(grgstem);

			if (mCurGrace) {
				const float mysize = mCurGrStaff->getSizeRatio() * 0.75f;
				grgstem->setSize(mysize);
			}
			curglobalstem = grgstem;
			mCurGrStaff->AddTag(grgstem);
			fMusic->addVoiceElement(arVoice,grgstem);
		}
	}
	else if (tinf == typeid(ARShareLocation))
	{
		GRGlobalLocation * grgloc =	new GRGlobalLocation(mCurGrStaff, static_cast<const ARShareLocation *>(apt));
		addGRTag(grgloc);
		curgloballocation = grgloc;
		mCurGrStaff->AddTag(grgloc);
		fMusic->addVoiceElement(arVoice,grgloc);

	}
	else if (tinf == typeid(ARChordTag))
	{
	}
	else if (tinf == typeid(ARUserChordTag)) // "\chord"
	{
		GRChordTag * tmp = new GRChordTag(mCurGrStaff, static_cast<const ARUserChordTag *>(apt) );
		addGRTag(tmp);
		curchordtag = tmp;
		GRSystemSlice * grsystemslice = mCurGrStaff->getGRSystemSlice();
		grsystemslice->addSystemTag(new GRSystemTag(tmp));
		mCurGrStaff->AddTag(tmp);
		fMusic->addVoiceElement(arVoice,tmp);
	}
	else if (tinf == typeid(ARCluster))
	{
        // Not needed because cluster is like an ornament
	}
	else if (tinf == typeid(ARGlissando)) 
	{
		GRGlissando * grglissando = new GRGlissando(mCurGrStaff, static_cast<const ARGlissando *>(apt));
		addGRTag(grglissando,0);
		mCurGrStaff->AddTag(grglissando);
		fMusic->addVoiceElement(arVoice,grglissando);
	}
    else if (tinf == typeid(ARSymbol))
	{
		const ARSymbol * arSymb = static_cast<const ARSymbol *>(apt);
		GRSymbol * grSymb = new GRSymbol(mCurGrStaff, arSymb);
		// this is a range-tag... therefore no spring is needed... it is handled
		// by the event at this timeposition
		// adds the tag at the end...
        addGRTag(grSymb,0);
        mCurGrStaff->AddTag(grSymb);
        fMusic->addVoiceElement(arVoice,grSymb);
	}
    else if (tinf == typeid(ARLyrics)) {}		// do nothing
	else if (tinf == typeid(ARMMRest))
	{
		GRRange * range = new GRRange(mCurGrStaff, static_cast<const ARMMRest *>(apt));
		addGRTag(range);
		mCurGrStaff->AddTag(range);
		fMusic->addVoiceElement(arVoice,range);
	}

	else cerr << "Warning: " << apt->getGMNName() << " not handled" << endl;
}

/** \brief Checks whether Tags can be removed due to matching end-Positions
*/
void GRVoiceManager::checkEndPTags(GuidoPos tstpos)
{
	// the following deletes the Tags which have matching end-Positions
	GuidoPos mpos = fGRTags->GetHeadPosition();
	while (mpos) {
		GuidoPos curpos = mpos;
		GRTag * g = fGRTags->GetNext(mpos);
		GRPositionTag * gpt = dynamic_cast<GRPositionTag *>(g);
		if( gpt ) {
			if (gpt->getEndPos() == tstpos) {
				if (dynamic_cast<GRGrace *>(g)) {
					// does not own the tags...
					if (toadd == 0)
						toadd = new GRTagPointerList(0);
					
					toadd->AddTail(g);
					mCurGrace = NULL;
                }
				else if (dynamic_cast<GRGlobalStem *>(g)) {
					// the global-Stems ends...
					if (g != curglobalstem && curglobalstem)
					{
						// then we have another stem...
						curglobalstem->RangeEnd(mCurGrStaff);
						fGRTags->RemoveElement(curglobalstem);
					}
					curglobalstem = NULL;
				}
				else if (dynamic_cast<GRGlobalLocation *>(g)) {
					// the global-Stems ends...
					curgloballocation = NULL;
					handleSharedArticulations (fSharedArticulations);
					fSharedArticulations.clear();
				}
                else if (dynamic_cast<GRChordTag *>(g))
                    curchordtag = NULL;

				else if(dynamic_cast<GRGlissando *>(g))
					organizeGlissando(g);

				else if(dynamic_cast<GRBeam *>(g))
					organizeBeaming(static_cast<GRBeam*>(g));

				g->RangeEnd(mCurGrStaff);
				fGRTags->RemoveElementAt(curpos);
				mpos = fGRTags->GetHeadPosition();
			}
		}
	}
}

/** \brief Gets called so that possible NewLine-Positions can be retrieved easily.
*/
void GRVoiceManager::setPossibleNLinePosition( const TYPE_TIMEPOSITION &)
{
	assert(false);
}

void GRVoiceManager::rememberLastNLinePosition( const TYPE_TIMEPOSITION & tp)
{
	if (fMusic)
	{
		fMusic->rememberVoiceNLinePosition(arVoice,tp);
	}
}

/** \brief Adds a tag to the GRTags-list. Second parameter
	specifies, if the tags are added at the head of the list.
*/
void GRVoiceManager::addGRTag(GRTag * grtag,int head)
{
	if (fGRTags)
	{
		if (head)
			fGRTags->AddHead(grtag);
		else
			fGRTags->AddTail(grtag);
	}
}

GRStaff * GRVoiceManager::getCurStaff() const
{
	return mCurGrStaff;
}

/** \brief This routine removes the associations from the
	fGRTags that have been added.
*/
void GRVoiceManager::removeAssociations(const NEPointerList & nl)
{
	// Associate the note with the current tags...
	GuidoPos pos = fGRTags->GetHeadPosition();
	while (pos)
	{
		GRNotationElement * el =  dynamic_cast<GRNotationElement *>(fGRTags->GetNext(pos));
		if (el)
			el->removeAssociation(nl);
	}
}

/** \brief Called to check whether there is a Rest that needs to be centered
	in the last Measure.

	! unused !
*/
void GRVoiceManager::checkCenterRest(GRStaff * grstaff, float lastpos, float newpos)
{
	if (grstaff != mCurGrStaff) return;
	
	if (lastev) 
	{
		GRRest * grrest = dynamic_cast<GRRest *>(lastev);
		if (grrest && grrest->getWholeMeasure())
		{
			// is a whole-Measure-Note
			// now we have to center it in the measure...
			NVPoint mypos = grrest->getPosition();

			mypos.x = (GCoord)((newpos - lastpos) * 0.5f + lastpos);
			grrest->setPosition(mypos);
		}
	}
}

//-------------------------------------------------------------------------------------------------
/** \brief Creates a GRNote from a ARNote
*/
GREvent * GRVoiceManager::CreateNote( const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject)
{
    ARNote * arnote = arObject->isARNote();
	if ((arObject->getDuration() <= DURATION_0) && (fVoiceState->fCurdispdur == NULL))
		return NULL;		// this should not happen...

	else if (arnote->getPitch() == EMPTY)
		return CreateEmpty(tp, arObject);

	return CreateSingleNote (tp, arObject);
}

//-------------------------------------------------------------------------------------------------
/** \brief Creates a GRNote from a ARNote
*/
GREvent * GRVoiceManager::CreateTab( const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject)
{
    ARTab * artab = arObject->isARTab();
	if ((artab->getDuration() <= DURATION_0) && (fVoiceState->fCurdispdur == NULL))
		return NULL;		// this should not happen...

	curev = ARMusicalEvent::cast(arObject);

	TYPE_DURATION dur = findDuration (fVoiceState, curev);
	dur.normalize();
	GRTab * grtab = new GRTab(mCurGrStaff, curev->isARTab(), tp, arObject->getDuration());
	grtab->setDuration(dur);

	if (curnoteformat != NULL)		grtab->setNoteFormat(curnoteformat);

	// Associate the note with the current tags...
	GuidoPos pos = fGRTags->GetHeadPosition();
	bool addedToTrill = false;
	while (pos)
	{
		GRNotationElement * el = dynamic_cast<GRNotationElement *>(fGRTags->GetNext(pos));
		if (el)	{
			el->addAssociation(grtab);
			if (fCurrentTrill && (fCurrentTrill == el)) addedToTrill = true;
		}
	}
	if (fCurrentTrill && !addedToTrill)
		setTrillNext (grtab);
	mCurGrStaff->addNotationElement(grtab);
	fMusic->addVoiceElement(arVoice,grtab);
	lastev = grtab;

	return grtab;

}

//-------------------------------------------------------------------------------------------------
/** \brief scan for associated tags
*/
void GRVoiceManager::doAssociate(GRSingleNote * grnote)
{
	// Associate the note with the current tags...
	GuidoPos pos = fGRTags->GetHeadPosition();
	bool addedToTrill = false;
	while (pos)
	{
		GRNotationElement * el = dynamic_cast<GRNotationElement *>(fGRTags->GetNext(pos));
		if (el)	{
            GRRange * r = dynamic_cast<GRRange *>(el);
			const ARAccidental* acc = r ? dynamic_cast<const ARAccidental*>(r->getAbstractRepresentation()) : 0;
			if (r && !acc && curgloballocation) {
				fSharedArticulations.push_back(make_pair(r, grnote));
			}
			else
				el->addAssociation(grnote);
			if (fCurrentTrill && (fCurrentTrill == el)) addedToTrill = true;
		}
	}
	if (fCurrentTrill && !addedToTrill)
		setTrillNext (grnote);
}

//-------------------------------------------------------------------------------------------------
/** \brief scan for the event duration
*/
TYPE_DURATION GRVoiceManager::findDuration(const ARMusicalVoiceState * state, const ARMusicalEvent* ev ) const
{
	TYPE_DURATION duration;
	if (state->fCurdispdur != NULL) {
		// make sure to recognize the displayduration-tag...
		duration = state->fCurdispdur->getDisplayDuration();
		int i = state->fCurdispdur->getDots();
		TYPE_DURATION tmpdur (duration);
		while (i>0) {
			// this takes care of dots maybe this should be a parameter for GRSingleNote later...
			tmpdur = tmpdur * DURATION_2;
			duration = duration + tmpdur;
			-- i;
		}
	}
	else duration = ev->getDuration();
	return duration;
}

//-------------------------------------------------------------------------------------------------
/** \brief Creates a GRNote from a ARNote
*/
GRSingleNote * GRVoiceManager::CreateSingleNote( const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject, float size, bool isGrace)
{
	curev = ARMusicalEvent::cast(arObject);

	TYPE_DURATION dtempl = findDuration (fVoiceState, curev);

	// we need to take care of dots !
    const ARNote * tmpNote = static_cast<const ARNote *>(curev->isARNote());
	dtempl.normalize();

	GRSingleNote * grnote = new GRSingleNote(mCurGrStaff, tmpNote, tp, arObject->getDuration());
    grnote->setGraceNote(isGrace);
	if (size)						grnote->setSize(size);
	if (curglobalstem)				grnote->setGlobalStem(curglobalstem);
	else if (curstemstate)
	{
		// stemdirection... (direction type should be the same for AR and GR...)
		switch (curstemstate->getStemState()) {
			case ARTStem::AUTO:		grnote->setStemDirection(dirAUTO); break;
			case ARTStem::UP:		grnote->setStemDirection(dirUP); break;
			case ARTStem::DOWN:		grnote->setStemDirection(dirDOWN); break;
			case ARTStem::OFF:		grnote->setStemDirection(dirOFF); break;
		}
		const TagParameterFloat * tpf = curstemstate->getLength();
		if (tpf && tpf->TagIsSet())
			grnote->setStemLength(tpf->getValue(mCurGrStaff->getStaffLSPACE()), true);
	}

	if (curheadstate)				grnote->setHeadState(curheadstate);
	if (curnoteformat != NULL)		grnote->setNoteFormat(curnoteformat);
	grnote->doCreateNote(dtempl);

	if (curdotformat != NULL)		grnote->setDotFormat(curdotformat);
	else {
		ARDotFormat defaultARDotFormat (0,0);
		grnote->setDotFormat(&defaultARDotFormat);
	}


	// Associate the note with the current tags...
	doAssociate (grnote);
	mCurGrStaff->addNotationElement(grnote);
	fMusic->addVoiceElement(arVoice,grnote);
	lastev = grnote;

	// the section below associate the current intens with the corresponding note
	if (curIntens) {
		const GRSingleNote* note = curIntens->getNote();
		bool inchord = !grnote->getDuration();
		if (note) {
			if (inchord) {		// in case of chord, scan for the lowest note
				int cp, co, ca;
				note->getPitchAndOctave( &cp, &co, &ca );
				int p, o, a;
				grnote->getPitchAndOctave( &p, &o, &a );
				if ((o < co) || ((o==co) && (p < cp)) )
					curIntens->setNote (grnote);
			}
		}
		else {
			curIntens->setNote (grnote);
		}
		if (!inchord) curIntens = 0;	// when done, clear the current intens
	}
	return grnote;
}


//-------------------------------------------------------------------------------------------------
/** \brief Creates a Grace-Note which is just a regular note; it
	is not added to the staff or the voice because it belongs to GRGrace.

	Independant of this, the GraceNote gets all the associations that a "real" event would get.
	All State-Information is used as well.
*/
GREvent * GRVoiceManager::CreateGraceNote( const TYPE_TIMEPOSITION & tp, ARMusicalObject *arObject, const TYPE_DURATION & dur)
{
    ARNote * arnote = static_cast<ARNote *>(arObject->isARNote());
	if (!arnote) return NULL;

	float size = 0.75f;
	if (mCurGrStaff) size *= mCurGrStaff->getSizeRatio();

	// what about chords in grace-notes ?		
	if (arnote->getPitch() == EMPTY)
	{
		// now we add a GREmpty-Event...
		GREvent* empty = CreateEmpty (tp, arObject);
		empty->setSize(size);
		return empty;
	} 
	GRSingleNote * grnote = CreateSingleNote (tp, arObject, size, true);
	const TagParameterFloat * tpf = curstemstate ? curstemstate->getLength() : 0;
	if (tpf && tpf->TagIsSet())
		grnote->setStemLength(tpf->getValue(), true);
	return grnote;
}

/** Creates a graphical rest from an abstract rest.
*/
GREvent * GRVoiceManager::CreateRest( const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject)
{
	const TYPE_TIMEPOSITION von (tp);
	const TYPE_DURATION bis (arObject->getDuration());
	
	// Rests can have 0 Durations! but they are not handled correctly within tuplets... 
	// just ignore them for now	
	if (bis <= DURATION_0)
	{
	}
	else
	{
		curev = ARMusicalEvent::cast(arObject);
		// this is new:
		TYPE_DURATION dtempl;
		if (fVoiceState->fCurdispdur != NULL)
		{
			dtempl = fVoiceState->fCurdispdur->getDisplayDuration();
			int i = fVoiceState->fCurdispdur->getDots();
			TYPE_DURATION tmpdur (dtempl);
			while (i>0)
			{
				// this takes care of dots maybe this should be
				// a parameter for GRSingleNote later...
				tmpdur = tmpdur * DURATION_2;
				dtempl = dtempl + tmpdur;
				-- i;
			}
		}
		else dtempl = curev->getDuration();
		
		dtempl.normalize();
		GRRest * grrest = new GRSingleRest(mCurGrStaff, dynamic_cast<const ARRest *>(curev), von,bis,dtempl);
		grrest->setRestFormat(currestformat);
	
		// - We need to take care of dots !
		if (curdotformat != 0)	grrest->setDotFormat(curdotformat);
		else {
			ARDotFormat defaultARDotFormat (0,0);
			grrest->setDotFormat(&defaultARDotFormat);
		}
		
		// Associate the rest with the current tags...
		addAssociations(grrest);
		mCurGrStaff->addNotationElement(grrest);
		fMusic->addVoiceElement(arVoice,grrest);
		lastev = grrest;
		return grrest;
	}
	return NULL;
}

//------------------------------------------------------------------------------------------------
void GRVoiceManager::setTrillNext(GRNotationElement* ev)
{
	TYPE_TIMEPOSITION evdate = ev->getAbstractRepresentation()->getRelativeTimePosition();
	const GRTrill* trill = ev->isGRTrill();
	if (trill && !trill->getBegin())
		fCurrentTrill->setNextEvent (ev);
	else if (fLastbar && (fLastbar->getAbstractRepresentation()->getRelativeTimePosition() == evdate)) {
		fCurrentTrill->setNextEvent (fLastbar);
	}
	else {
		fCurrentTrill->setNextEvent (ev);
	}
	fCurrentTrill = 0;
}

//------------------------------------------------------------------------------------------------
void GRVoiceManager::addAssociations(GREvent* ev, bool setnext)
{
	bool addedToTrill = false;
	GuidoPos pos = fGRTags->GetHeadPosition();
	while (pos)
	{
		GRNotationElement * el = dynamic_cast<GRNotationElement *>(fGRTags->GetNext(pos));
		if (el)		el->addAssociation(ev);
		if (fCurrentTrill && (el == fCurrentTrill)) addedToTrill = true;
	}
	if (setnext && fCurrentTrill && !addedToTrill && !ev->isEmpty())
		setTrillNext (ev);
}

//------------------------------------------------------------------------------------------------
GREvent * GRVoiceManager::CreateEmpty( const TYPE_TIMEPOSITION & tp, ARMusicalObject * arObject)
{	
	ARMusicalEvent * ev = ARMusicalEvent::cast(arObject);
	assert(ev);

	GREmpty * grempty = new GREmpty(mCurGrStaff, ev, tp, arObject->getDuration());
	const ARNote * note = arObject->isARNote();
	if (note) grempty->setAuto (note->isAuto()); // propagate auto status, used to denote chords
	// the associations have to be handled just the same...
	addAssociations (grempty, false);
	mCurGrStaff->addNotationElement(grempty);
	fMusic->addVoiceElement(arVoice,grempty);
	if (fCurrentTrill && !grempty->isAuto())
		setTrillNext (grempty);
	lastev = grempty;
	return grempty;
}

GRVoice * GRVoiceManager::getGRVoice()
{
	return grvoice;
}

/** \brief Returns a score for breaking at the current position; the TIME_POSITION
	maybe off this position, which makes it not very feasable to break here...
	Also taken into consideration is bar-line-positions (that is meter...)
*/
float GRVoiceManager::GetBreakScore(const TYPE_TIMEPOSITION & tp)
{
	float penalty = 0;

	if (fVoiceState->curtp > tp)
		penalty += 20000; // BREAKDURINGEVENTPENALTY
	else if (fVoiceState->curtp < tp)
		assert(false);

	if (fVoiceState->curlastbartp == tp)
		penalty -= 2000; // BREAKATBARPRAISE
	else
		penalty += 1000; // BREAKATNOBARPENALTY
	return penalty;
}


void GRVoiceManager::closeOpenTags()
{
  GuidoPos pos = fGRTags->GetHeadPosition();
  while (pos)
  {
	GRTag * el = fGRTags->GetNext(pos);
	el->StaffFinished(mCurGrStaff);
  }

  // we don't need to remove all the tags, because
  // this would mean, that we would need to REPARSE
  // them again...

  // this removes all the tags...
  // fGRTags->RemoveAll();

}

/** \brief Called, when a new staff has begun and we
	have to handle all the open tags at that point.
	(open left range and all that)
*/
void GRVoiceManager::beginOpenTags()
{
	// just look at the fGRTags...
	if (fGRTags == 0) return;

	GuidoPos pos = fGRTags->GetHeadPosition();
	while (pos)
	{
		GRTag * tag = fGRTags->GetNext(pos);
		if (tag)
		{
			tag->StaffBegin(mCurGrStaff);
			
			// new: create a dummy-element that just wraps all calls...
			GRPositionTag * ptag = dynamic_cast<GRPositionTag *>(tag);
			GRDummy * grdum = new GRDummy(ptag);
			grdum->setRelativeTimePosition(curtp);
			mCurGrStaff->addNotationElement(grdum);
		}
	}
}

void GRVoiceManager::setGRStaff(GRStaff * newstaff)
{
	mCurGrStaff = newstaff;
	// this must be reflected in the OPEN Tags as-well
	// otherwise the SSE-Lookup will not work !
	assert(false);
	GuidoPos mypos = fGRTags->GetHeadPosition();
	while (mypos)
	{
		GRTag * tag = fGRTags->GetNext(mypos);
		GRPositionTag * grpt = dynamic_cast<GRPositionTag *>(tag);
		if (grpt)
			grpt->changeCurrentSystem(newstaff->getGRSystem());
	}

	// this is important for centering rests between barlines. 
	// this is called, when a new staff-line starts (a new system)
	fLastbar = NULL;
	fLastnonzeroevent = NULL;
}


/** \brief This routine reads all Tags that have to
	be read at the beginning (that is pageformat, systemformat, staff...)
*/
void GRVoiceManager::ReadBeginTags(const TYPE_TIMEPOSITION & tp)
{
	bool ende = false;

	ARMusicalVoiceState * mystate = new ARMusicalVoiceState(*fVoiceState);

	// the idea is as follows: 
	// when reading begin tags it is important to read only the first occurence of a tag.
	// regarding the \staff-Tag this is a little bit more complicated, because ANY other tag that creates a
	// symbol (or manipulates something) implies that it is set on the current staff (although we do not know 
	// this really for example consider \title, \pageNumber or whatever tag...?
	// as a rule we can state: the very first tag after a \newSystem or \newPage tag MUST be a \staff-Tag, 
	// iff the staff in this voice is changing then !
	bool staffread           = false;
	const ARStaff        * mstaff  = NULL;
	const ARStaffFormat  * stffrm  = NULL;
	const ARPageFormat   * pform   = NULL;
	const ARSystemFormat * sysform = NULL;
	const ARUnits        * units   = NULL;
	const ARAccolade     * accol   = NULL;
	const ARAuto         * arauto  = NULL;

	while (mystate->vpos && !ende)
	{
		ende = false;
		ARMusicalObject * o = arVoice->GetAt(mystate->vpos);

		if (!o)
		{   
			delete mystate;
		    return; 
		}

		const ARMusicalTag * mtag = static_cast<const ARMusicalTag *>(o->isARMusicalTag());
		if (mtag == 0)
		{ 
			delete mystate;
			return;
		}

		if ( !pform && (pform = dynamic_cast<const ARPageFormat *>(mtag)) != 0)
			mStaffMgr->setPageFormat(pform);		// we have a page-Format-tag
		else if ( !sysform && (sysform = dynamic_cast<const ARSystemFormat *>(mtag)) != 0)
			mStaffMgr->setSystemFormat(sysform);	// we have a systemFormat-tag
		else if (!arauto && (arauto = dynamic_cast<const ARAuto *>(mtag)) != 0)
			mStaffMgr->setAutoTag(arauto);
		else if (!stffrm && (stffrm = dynamic_cast<const ARStaffFormat *>(mtag)) != NULL) {
			// we have a staffFormat-tag if we do not have a mCurGrStaff yet ?
			mCurGrStaff = mStaffMgr->getStaff(staffnum);
			mCurGrStaff->setStaffFormat(stffrm);
		}
		else if (!staffread && (mstaff = dynamic_cast<const ARStaff *>(mtag)) != 0) {
			// it is a staff-tag...
			// then, the staff will be changed... the StaffManager (which is the owner
			// of this VoiceManager) is called; it then prepares the staff...
			staffnum = mstaff->getStaffNumber();
			mStaffMgr->prepareStaff(staffnum);
			mCurGrStaff = mStaffMgr->getStaff(staffnum);
			assert(mCurGrStaff);
		}
		else if ( !units && (units = dynamic_cast<const ARUnits *>(mtag)) != 0) {
			// just ignore the units tag...
		}
		else if ( !accol && (accol = dynamic_cast<const ARAccolade *>(mtag)) != 0) {
			// (JB) Each system should have a list of accolade tags.
			mStaffMgr->notifyAccoladeTag( accol );
		}
		else if ( o->getRelativeTimePosition() > tp)
			ende = true;		

		if (!ende)
            arVoice->GetNext(mystate->vpos,*mystate);
		// this means: only the VERY FIRST TAG can be a \staff-Tag everything else is discarded...

		staffread = true;
	}

	delete mystate;	
}

void GRVoiceManager::organizeGlissando(GRTag * g)
{
	GRGlissando * gliss = dynamic_cast<GRGlissando *>(g);
	if( gliss )
	{
		GRGlissando * prevGliss = mCurGlissando;
		if(prevGliss != NULL )
			gliss->setPrevGlissando(prevGliss);
		mCurGlissando = gliss;
	}
	
}

bool & GRVoiceManager::getCurStaffDraw(int index)
{
	static std::map<int, bool> mCurStaffDraw;
	
	if(mCurStaffDraw.count(index)==0)
	{
		mCurStaffDraw.insert(std::pair<int, bool>(index, true));
	}
	return mCurStaffDraw[index];
}	

/*
	check nested beams :
	when a beam is included in a parent beam sets the beam parent
	this avoids to re-compute the main beam
	nested beams are not supported for grace notes
 */
void GRVoiceManager::organizeBeaming(GRBeam * beam)
{
	if (beam->isGraceBeaming()) return;

	NEPointerList* assoc = beam->getAssociations();
	GRNotationElement* lastEvt = assoc ? assoc->GetAt(assoc->GetTailPosition()) : nullptr;
	if (!lastEvt) return;

	for (GRBeam* b: fBeams) {
		NEPointerList* assoc = b->getAssociations();
		GRNotationElement* bLast = assoc ? assoc->GetAt(assoc->GetTailPosition()) : nullptr;
		if (! bLast) continue;
		if (beam->isGraceBeaming() != b->isGraceBeaming()) continue;
		if ((b->getRelativeTimePosition() <= beam->getRelativeTimePosition()) && (bLast->getRelativeTimePosition() >= lastEvt->getRelativeTimePosition())) {
			b->addSmallerBeam(beam);
			beam->setParent(b);
		}
		else if ((b->getRelativeTimePosition() >= beam->getRelativeTimePosition()) && (bLast->getRelativeTimePosition() <= lastEvt->getRelativeTimePosition())) {
			beam->addSmallerBeam(b);
			b->setParent(beam);
		}
	}
	fBeams.push_back(beam);
}

//----------------------------------------------------------------------------------
// shared articulations are articulations that are placed on shored locations like chord.
// to avoid duplicate articulations, they are handled at the end of the shared location
//----------------------------------------------------------------------------------
void GRVoiceManager::handleSharedArticulations(const TSharedArticulationsList& list)
{
	size_t n = list.size();
	if (!n) return;				// list is empty

	GRSingleNote* high = list[0].second;		// start to find the highest and lowest notes
	GRSingleNote* low = high;
	for ( size_t i = 1; i <n; i++) {
		int pitch = list[i].second->getARNote()->getMidiPitch();
		if (pitch > high->getARNote()->getMidiPitch()) {
			high = list[i].second;
		}
		else if (pitch < low->getARNote()->getMidiPitch()) {
			low = list[i].second;
		}
	}

	int placement = ARArticulation::kDefaultPosition;

	GDirection stemdir = high->getStemDirection();
	low->setStemDirection(stemdir);			// required for a correct placement of the articulation
	high->setStemDirection(stemdir);			// required for a correct placement of the articulation

	std::map<GRRange*, bool> previous;
	for (auto elt: list) {
		GRRange* r = elt.first;
		if (previous[r]) continue;
		previous[r] = true;

		const ARArticulation* art = dynamic_cast<const ARArticulation*>(r->getAbstractRepresentation());
		if (art)  placement = art->getArticulationPosition();

		if (placement == ARArticulation::kAbove)
			r->addAssociation (high);
		else if (placement == ARArticulation::kBelow)
			r->addAssociation (low);
		else {
			switch (stemdir) {
			case dirOFF:
				r->addAssociation (high);
				break;
			case dirUP:
				r->addAssociation (low);
				break;
			case dirDOWN:
				r->addAssociation (high);
				break;
			case dirAUTO:
				r->addAssociation (high);
				break;
			}
		}
	}
}

