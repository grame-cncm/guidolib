/*
  GUIDO Library
  Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz
  Copyright (C) 2002 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

/** \brief class GRStaff is a graphical object that will be created and managed by 
	StaffManager derived from GRCompositeNotationElement it includes NotationElement
	(sorted by time position).
	GRStaffs will be painted and "owned" from GRSystem.
*/

#include <typeinfo>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <iostream> // for debug only
#ifdef android
#include <ctype.h>
#endif

using namespace std;

// - Guido Misc
#include "secureio.h"
#include "gddefs.h"
#include "GuidoDefs.h" 		// for LSPACE
#include "GUIDOInternal.h"

#include "VGDevice.h"
#include "VGFont.h"
#include "TagParameterString.h"
#include "TagParameterFloat.h"

// - Guido AR
#include "ARBarFormat.h"
#include "ARClef.h"
#include "ARDoubleBar.h"
#include "ARKey.h"
#include "AROctava.h"
#include "ARNaturalKey.h"
#include "ARFinishBar.h"
#include "ARInstrument.h"
#include "ARMeter.h"
#include "ARRepeatBegin.h"
#include "ARRepeatEnd.h"
#include "ARRepeatEndRangeEnd.h"
#include "ARStaffFormat.h"
#include "ARNote.h"
#include "ARSpace.h"

// - Guido GR
#include "GRStaff.h"
#include "GRBar.h"
#include "GRBarFormat.h"
#include "GRBeam.h"
#include "GRClef.h"
#include "GRDoubleBar.h"
#include "GRDummy.h"
#include "GRFinishBar.h"
#include "GRGlue.h"
#include "GRInstrument.h"
#include "GRIntens.h"
#include "GRKey.h"
#include "GRMeter.h"
#include "GRMusic.h"
#include "GRNote.h"
#include "GRRest.h"
#include "GRRepeatBegin.h"
#include "GRRepeatEnd.h"
#include "GRRod.h"
#include "GRSpace.h"
#include "GRSpecial.h"

#include "GRGlobalStem.h"
#include "GRStem.h"
#include "GROctava.h"
#include "GRSingleNote.h"
#include "GRStaffManager.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRText.h"
#include "GRVoice.h"

#include "kf_ivect.h"
#include "TCollisions.h"

int gd_noteName2pc(const char *name);

#if 0
#define staff_debug(m)  cout << this << " GRStaff::" << m << endl
#else
#define staff_debug(m)
#endif

//#define TRACE
#ifdef TRACE
#define traceMethod(method)		cout << (void*)this << " GRStaff::" << method << endl
#else
#define traceMethod(method)	
#endif
#define trace1Method(method)		cout << (void*)this << " GRStaff::" << method << endl

//#define EXTENDEDBB

// ===========================================================================
//		MeasureAccidentals
// ===========================================================================
void MeasureAccidentals::reset() 
{
	for (int pitch=0; pitch < kPitchClasses; pitch++) {
		fPClass[pitch] = 0;
		for (int oct=0; oct < kOctRange; oct++)
			fAccidentals[pitch][oct] = 0;
	}
}

void MeasureAccidentals::setAccidental(int pitch, int oct, float accidental) 
{
	oct += 4;		// guido octave numbering offset
	if ( (pitch >= kPitchClasses) || (oct >= kOctRange) || (oct < 0)) return;
	fAccidentals[pitch][oct] = accidental;
	fPClass[pitch] = accidental;
}

void MeasureAccidentals::setPitchClassAccidental(int pitch, float accidental) 
{
	if (pitch > kPitchClasses) return;
	fPClass[pitch] = accidental;
	for (int oct = 0; oct < kOctRange; oct++)
		fAccidentals[pitch][oct] = accidental;
}

float MeasureAccidentals::accidental (int pitch, int oct) const
{ 
	// guido octave numbering offset
	return fAccidentals[pitch][oct+4]; 
}

float MeasureAccidentals::accidental (int pitch) const
{ 
	return fPClass[pitch]; 
}

MeasureAccidentals & MeasureAccidentals::operator=(const MeasureAccidentals &ma)
{
	for (int pitch=0; pitch < kPitchClasses; pitch++) {
		fPClass[pitch] = ma.accidental(pitch);
		for (int oct=0; oct < kOctRange; oct++) {
			fAccidentals[pitch][oct] = ma.accidental(pitch, oct-4);
		}
	}
	return *this;
}

// ===========================================================================
//		GRStaffState
// ===========================================================================

GRStaffState::GRStaffState()
{
	meterset = false;	// meter information set?. TRUE, false

	curmeter = NULL;
	keyset   = false; 			// key signatur set?
	numkeys  = 0;

	curbarfrmt    = NULL;
	curstaffrmt   = NULL;
    staffDistance = NULL;
	staffLSPACE   = LSPACE;
	numlines      = 5;              // Standard
    lineThickness = LSPACE * 0.08f;
    fYOffset       = 0;
    colRef        = 0;

	curkey = NULL;

	// clef-Parameter
	clefset      = CLEFAUTO;   // CLEFINTERN, CLEFEXPLICIT, CLEFAUTO, [ CLEFOFF ]
	clefname     = "g2";       // Standard
	basepit      = NOTE_G;
	basepitoffs  = 0;
	instrNumKeys = 0;
	baseoct      = 1;
	octava       = 0;
	baseline     = 3;
	curclef      = NULL;

	distanceset = false;
	distance    = 0;

	for (int i = 0; i < NUMNOTES; ++i) {
		instrKeyArray[i] = 0; 
		KeyArray[i] = 0; 
	}
}

// ----------------------------------------------------------------------------
GRStaffState::~GRStaffState()
{
	delete [] colRef;
}

// ----------------------------------------------------------------------------
void GRStaffState::reset2key()
{
	for (int i = 0; i < NUMNOTES; ++i) {
		fMeasureAccidentals.setPitchClassAccidental (i, KeyArray[i] - instrKeyArray[i]);
	}
}

// ----------------------------------------------------------------------------
GRStaffState & GRStaffState::operator=(const GRStaffState & state)
{
	meterset = state.meterset; // is meter-signture set?. TRUE, false
	curmeter = state.curmeter;

	// Noteparameter
	keyset	= state.keyset; // key-signature set?
	numkeys = state.numkeys;
	curkey	= state.curkey;
	for (int i=0; i < NUMNOTES; ++i)
	{
		instrKeyArray[i] = state.instrKeyArray[i];
		KeyArray[i] = state.KeyArray[i];
//		MeasureAccidentals[i] = state.MeasureAccidentals[i];
	}
	
	fMeasureAccidentals = state.fMeasureAccidentals;
	fInstrument = state.fInstrument;

	// clef-Parameter
	clefset		= state.clefset; // CLEFINTERN, CLEFEXPLICIT, CLEFAUTO, [ CLEFOFF ]
	clefname	= state.clefname;
	basepit		= state.basepit;
	basepitoffs = state.basepitoffs;
	instrNumKeys = state.instrNumKeys;
	baseoct		= state.baseoct;
	octava		= state.octava;
	baseline	= state.baseline;
	curclef		= state.curclef;

	curstaffrmt = state.curstaffrmt;
	staffLSPACE = LSPACE;
	if (curstaffrmt && curstaffrmt->getSize() && curstaffrmt->getSize()->TagIsSet())
	{
		staffLSPACE = curstaffrmt->getSize()->getValue() * 2;
	}
	numlines		= state.numlines; // Standard ...
    lineThickness	= state.lineThickness;

	distanceset = state.distanceset;
	distance	= state.distance;

	curbarfrmt	= state.curbarfrmt;
	return (*this);
}

// ===========================================================================
//		GRStaff
// ===========================================================================

GRStaff::GRStaff( GRSystemSlice * systemslice, float propRender )
						: mGrSystem(NULL), mGrSystemSlice( systemslice ), fLastSystemBarChecked(-1,1), fProportionnalRendering(propRender)
{
	mLength = 0;
	setRelativeTimePosition(systemslice->getRelativeTimePosition());
	
//	mDurationOfGR = DURATION_0; // (JB) TEST: removed !
	setClefParameters(0);
	setMeterParameters(0);
	setKeyParameters(0);

	startglue = 0;
	endglue = 0;
	secglue = 0;

	lastrod = 0;
	firstrod = 0;

	setOnOff(true);
	isNextOn = true;
	firstOnOffSetting = false;
}

// ----------------------------------------------------------------------------
GRStaff::~GRStaff()
{
	// this routine has been done in this fashion, 
	// because that way, the associations are done correctly ....
	// but who deletes the Events? and the Glue ?
	GuidoPos pos = getElements()->GetHeadPosition();
	while (pos)
	{
		GuidoPos curpos = pos;
		GRNotationElement * el = getElements()->GetNext(pos);	
		if (!GREvent::cast(el) && !dynamic_cast<GRGlue *>(el))
		{
			GRDummy * grdum = dynamic_cast<GRDummy *>(el);
			GRPositionTag * ptag = dynamic_cast<GRPositionTag *>(el);
			if (ptag)
			{
				if (!ptag->DeleteStaff(this))
				{
					mCompElements.setOwnership(false);
					mCompElements.RemoveElementAt(curpos);
					mCompElements.setOwnership(true);
				}
				else
					mCompElements.RemoveElementAt(curpos);
			}
			else if (grdum)
			{
				// it is a dummy (stands for a position tag ...)
				ptag = grdum->getPositionTag();
				if (ptag)
				{
					// if ptag->DeleteStaff returns 1, then
					// the sse-list inside the position tag 
					// is empty, then the whole tag MUST be deleted.
					// The reason, why this needs to be done here
					// is that systems are deleted from the beginning
					// towards the end: the original positionTag in
					// the previous system has had DeleteStaff called
					// for the previous staff. 
					if (ptag->DeleteStaff(this))
					{
						delete ptag;
						ptag = 0;
					}
				}
				mCompElements.RemoveElementAt(curpos);
			}
			else
				mCompElements.RemoveElementAt(curpos);
		}
	}
}


// ----------------------------------------------------------------------------
/** \brief Inserts the NotationElement into the staff.
	 The correct position in the timeframe is checked
*/
// ATTENTION: the runtime could be improved here a lot
// Question do we need a timeframe checking?
// actual the grafical layer is "timeless" if 
// looking only for grafical elements

void GRStaff::addNotationElement(GRNotationElement * notationElement)
{
staff_debug("addNotationElement");
	assert(notationElement);

	// this time, the time-check is disabled, if
	// we have a positiontag (because those are
	// added some more times than just once!)
	bool added = false;
	if ((dynamic_cast<GRPositionTag *>(notationElement)) == NULL) {
		if (getElements()->GetTail()) {
			const TYPE_TIMEPOSITION tp1 (notationElement->getRelativeTimePosition());

			// no notation element may be added to the staff, that
			// is EARLIER than the staff itself.
#ifdef DEBUG
			assert(tp1>=getRelativeTimePosition());
#endif
			const TYPE_TIMEPOSITION tp2 (getElements()->GetTail()->getRelativeTimePosition());
			if( tp1 < tp2 )
			{
				// DF - 24/08/2009 - assert commented to avoid spurious exit
				// due to dynamic score coding or multiple fermata in a chord with variable length notes
				// assert(false);
				mCompElements.AddAtCorrectTimePosition(notationElement);
				added = true;
			}
		}
	}
	if (!added) {
#if 1
	// this part fix a bug with incorrect placement of position tags in multi voices scores using \staff tags
	// the last mCompElements note duration could be greater than previous one
	// the result was an incorrect date of the position tag in between
	// the section below makes sure that the tail of mCompElements is always the smallest note
	// tags with no duration are ignored
		GuidoPos pos = mCompElements.GetTailPosition();
		if (!pos) mCompElements.AddTail(notationElement);
		else {
			GRNotationElement * el = mCompElements.GetAt(pos);
			TYPE_DURATION d1 = notationElement->getDuration();
			TYPE_DURATION d2 = el->getDuration();
			if (d1 && d2 && (d2 < d1)) {
				if (mCompElements.GetPrev(pos))
					mCompElements.AddElementAfter(pos, notationElement);
				else mCompElements.AddTail(notationElement);
			}
			else mCompElements.AddTail(notationElement);
		}
#else
		mCompElements.AddTail(notationElement);
#endif
	}

	// this should sort the elements ... it is no longer needed; sorting is done
	// on the abstract representation, and all elements are added/sorted there.
	//if (sortElements)
	//	mCompElements.sort( & GRNotationElement::comp);

	// This is old, horizontal synchronisation is done by StaffManager and not longer
	// by the System:
	
	// This is done, so that all Elements that are added are subject to vertical-slice-spacing 
	// right after the current time slice has been worked upon.
	// OLD: mGrSystem->AddToSpace(notationElement);

	// count the notes, evaluates the accidentals.
    const GRNote * mynote = notationElement->isGRNote();
	setNoteParameters(mynote);
}


// ----------------------------------------------------------------------------
void GRStaff::print(std::ostream& os) const
{
	const NEPointerList& elts = getElements();
	GuidoPos pos = elts.GetHeadPosition();
	while (pos) {
		os << elts.GetNext(pos) << endl;
	}
}
std::ostream& operator<< (std::ostream& os, const GRStaff& staff)			{ staff.print(os); return os; }
std::ostream& operator<< (std::ostream& os, const GRStaff* staff)			{ staff->print(os); return os; }

class TXInterval : public std::pair<float, float>
{
	public:
		TXInterval () : std::pair<float, float>(0.f,0.f) {}
		TXInterval (float a, float b) : std::pair<float, float>(a, b) {}
		void	clear ()					{ second = first = 0; }
		bool	empty () const				{ return (second - first) <= 0; }
		float	width () const				{ return second - first; }
		bool	collides (const TXInterval& i) const	{
			return ((first >= i.first) && ((first < i.second))) ||
					((second <= i.second) && ((second > i.first)));
		}
};

//-------------------------------------------------------------------------------
static bool sortbypos (const TXInterval& i, const TXInterval& j) {
	return (i.first < j.first);
}

//-------------------------------------------------------------------------------
static vector<TXInterval> stripintervals (const vector<TXInterval>& list) {
	vector<TXInterval> outlist;
	size_t n = list.size();
	TXInterval lastcollide;
	for (size_t i=1; i<n ; i++) {
		if (list[i-1].empty()) continue;		// empty ignore empty intervals

		if (lastcollide.width()) {				// check if the is already a pending extension
			if (list[i].collides(lastcollide))
				lastcollide.second = list[i].second;	// extend the pending interval
			else {
				outlist.push_back(lastcollide);			// otherwise store
				lastcollide.clear();					// and clear the pending interval
			}
		}
		else if (list[i].collides(list[i-1])) {
			float first		= min(list[i].first, list[i-1].first);
			float second	= max(list[i].second, list[i-1].second);
			lastcollide = TXInterval(first, second);
		}
		else outlist.push_back(list[i-1]);
	}
	if (lastcollide.width()) outlist.push_back(lastcollide);
	else if (n) outlist.push_back(list[n-1]);
	return outlist;
}

// ----------------------------------------------------------------------------
float GRStaff::getNotesDensity () const
{
	vector<TXInterval> xintervals;
	const NEPointerList& elts = getElements();
	GuidoPos pos = elts.GetHeadPosition();
	while (pos) {
		const GRNotationElement * e = elts.GetNext(pos);
		if (e  && (e->isSingleNote() || e->isRest()) && !e->isEmpty()) {
			NVRect bb = e->getBoundingBox();
			bb += e->getPosition();
			xintervals.push_back (TXInterval (bb.left, bb.right));
		}
	}
	sort (xintervals.begin(), xintervals.end(), sortbypos);
	xintervals = stripintervals (xintervals);
	size_t n = xintervals.size();
	float occupied = 0;
	for (size_t i=0; i<n ; i++)
		occupied += xintervals[i].width();
	return occupied / getBoundingBox().Width();
}


// ----------------------------------------------------------------------------
// detect and fix multi-voices note collisions e.g. { [ \stemsUp g ], [\staff<1>\stemsDown f] }
void GRStaff::checkMultiVoiceNotesCollision ()
{
	bool inChord = false, pendingChord = false;		// used for chords detection
	NEPointerList* elts = getElements();
	map<TYPE_TIMEPOSITION, map<GRSingleNote*, float> > vmap;	// map of notes at a given date
																// notes are associated to their shift value (which is currently no used)
	map<GRSingleNote*, GRBeam*> beamed;							// map of notes that are beamed

	GuidoPos pos = elts->GetHeadPosition();
	while (pos) {
		GRNotationElement * e = elts->GetNext(pos);
		GRBeam* beam = dynamic_cast<GRBeam*>(e);
		if (e->isEmpty()) {
			if (inChord)							// at this point this is a chord end
				inChord = pendingChord = false;		// set the chord flags off
			else pendingChord = true;				// at this point, we possibly enter a chord
		}
		else if (e->isChordComma() && pendingChord)
			inChord = true;							// at this point we're entering a chord
		else if (!inChord) {						// here we deal with notes outside chords
			GRSingleNote* note = e->isSingleNote();
			if (note && !note->isGraceNote() && !note->isEmpty()) { // ignore empty and grace notes
				TYPE_TIMEPOSITION date = e->getRelativeTimePosition();
				map<GRSingleNote*, float> list = vmap[date];
				float shift = 0;
				for (auto elt: list) {								// check each note at a given date
					const ARNote* first = elt.first->getARNote();
					const ARNote* second = note->getARNote();
					if ((first->getOctave() == second->getOctave()) &&  (std::abs(first->getPitch() - second->getPitch()) == 1)) {
						GRSingleNote* target = note;
						NVPoint npos = target->getPosition();
						shift = getStaffLSPACE() * ((target->getStemDirection() == dirUP) ? -0.9f : 0.83f);
						npos.x += shift;
						target->setPosition(npos);
						GRBeam* beam = beamed[target];
						if (beam) beam->refreshPosition();			// beams of shifted note needs to ne refreshed
						break;
					}
				}
				vmap[date][note] = shift;							// store note in the map with it's shift value
			}
			else if (beam) {										// collect beams and associated notes
				NEPointerList * assoc = beam->getAssociations();
				GuidoPos bpos = assoc ? assoc->GetHeadPosition() : 0;
				while (bpos) {
					GRNotationElement * e = assoc->GetNext(bpos);
					beamed[e->isSingleNote()] = beam;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
// detect and fix multi-voices note collisions e.g. { [ \stemsUp g ], [\staff<1>\stemsDown f] }
// works only
//void GRStaff::checkMultiVoiceNotesCollision ()
//{
//	bool inChord = false, pendingChord = false;		// used for chords detection
//	NEPointerList* elts = getElements();
//
//	map<TYPE_TIMEPOSITION, GRSingleNote*> vmap;
//	map<GRSingleNote*, bool> shifted;
//	map<GRSingleNote*, GRBeam*> beamed;
//	GuidoPos pos = elts->GetHeadPosition();
//	while (pos) {
//		GRNotationElement * e = elts->GetNext(pos);
//		GRBeam* beam = dynamic_cast<GRBeam*>(e);
//		if (e->isEmpty()) {
//			if (inChord)							// at this point this is a chord end
//				inChord = pendingChord = false;		// set the chord flags off
//			else pendingChord = true;				// at this point, we possibly enter a chord
//		}
//		else if (e->isChordComma() && pendingChord)
//			inChord = true;							// at this point we're entering a chord
//		else if (!inChord) {						// here we deal notes outside chords
//			GRSingleNote* note = e->isSingleNote();
//			if (note && !note->isGraceNote() && !note->isEmpty()) { // ignore empty and grace notes
//				TYPE_TIMEPOSITION date = e->getRelativeTimePosition();
//				GRSingleNote* prev = vmap[date];
//				if (prev) {
//					const ARNote* first = prev->getARNote();
//					const ARNote* second = note->getARNote();
//					if ((first->getOctave() == second->getOctave()) &&  (std::abs(first->getPitch() - second->getPitch()) == 1)) {
//						NVPoint npos = note->getPosition();
//						npos.x += shifted[prev] ? getStaffLSPACE()*-0.30f : getStaffLSPACE()*0.83f;
//						note->setPosition(npos);
//						GRBeam* beam = beamed[note];
//						if (beam) beam->refreshPosition();
//
//						shifted[note] = true;
//					}
//				}
//				vmap[date] = note;
//			}
//			else if (beam) {
//				NEPointerList * assoc = beam->getAssociations();
//				GuidoPos bpos = assoc ? assoc->GetHeadPosition() : 0;
//				while (bpos) {
//					GRNotationElement * e = assoc->GetNext(bpos);
//					beamed[e->isSingleNote()] = beam;
//				}
//			}
//		}
//	}
//}

// ----------------------------------------------------------------------------
void GRStaff::checkCollisions (TCollisions& state) const
{
if (state.lastElement())
cerr << "GRStaff::checkCollisions " << state.getSystem() << "/" << state.getStaff() << " last: " << state.lastElement() << endl;
else
cerr << "GRStaff::checkCollisions " << state.getSystem() << "/" << state.getStaff() << endl;
	
	NVRect chordbb;									// the last chord bounding box
	bool inChord = false, pendingChord = false;		// used for chords detection
	const NEPointerList& elts = getElements();

	GuidoPos pos = elts.GetHeadPosition();
	while (pos) {
		const GRNotationElement * e = elts.GetNext(pos);
cerr << "GRStaff::checkCollisions " << e << endl;
		if (e->isEmpty()) {
			if (inChord) {							// at this point this is a chord end
				inChord = pendingChord = false;		// set the chord flags off
				state.check(chordbb);				// and check for collision
				state.update (e, chordbb);			// update the collision tracking state
			}
			else pendingChord = true;				// at this point, we possibly enter a chord
		}
		else if (e->isChordComma() && pendingChord) {
			inChord = true;							// at this point we're entering a chord
			chordbb.Set(0,0,0,0);
		}
		else {										// here we deal with non empty events
			pendingChord = false;					// we can safely assume that there is no pending chord (but can be inside a chord)
			NVRect r = e->getBoundingBox();			// collect the event bounding box
			r += e->getPosition();					// adjust the position
		
			const GRSingleNote* note = e->isSingleNote();
			if (note) {
				if (inChord) chordbb.Merge (r);		// inside a chord, we accumulate the notes bounding boxes
				else {								// outside the chord:
					state.check(r);					// check for collision
					state.update (e, r);			// update the collision tracking state
				}
			}
			else if (e->checkCollisionWith() && r.Width()) {	// this is not a note and if collision checking is required
				state.check(r);						// check for collision
				state.update (e, r);				// update the collision tracking state
			}
		}
	}
}

// --------------------------------------------------------------------------
void GRStaff::accept (GRVisitor& visitor)
{
	visitor.visitStart (this);
	NEPointerList* elts = getElements();
	GuidoPos pos = elts->GetHeadPosition();
	while (pos) {
		GRNotationElement * e = elts->GetNext(pos);
		e->accept (visitor);
	}
	visitor.visitEnd (this);
}

// ----------------------------------------------------------------------------
size_t GRStaff::getLyrics (vector<const GRNotationElement*>& list) const
{
	const NEPointerList& elts = getElements();
	GuidoPos pos = elts.GetHeadPosition();
	while (pos) {
		const GRNotationElement * e = elts.GetNext(pos);
		const GRText* text = e->isText();
		if (text && text->isLyrics()) {
			list.push_back (e);
		}
	}
	return list.size();
}

// ----------------------------------------------------------------------------
GRClef * GRStaff::AddClef(const ARClef * arclef)
{
	// To do: look, whether the clef really is a clef-
	// change. If not, do nothing!!!!
	GRClef * grclef = new GRClef(arclef, this);
	TYPE_TIMEPOSITION tmp = getRelativeTimePosition();
	GRNotationElement * el = mCompElements.GetTail();
	if (el)
		tmp = el->getRelativeEndTimePosition();
	grclef->setRelativeTimePosition(tmp);

	setClefParameters(grclef, GRStaffState::CLEFEXPLICIT);
	addNotationElement(grclef);
	mStaffState.reset2key();
	return grclef;
}

// ----------------------------------------------------------------------------
void GRStaff::setMeterParameters(GRMeter * grmeter)
{
	mStaffState.meterset = false;
	if ( (grmeter != 0) && 
		(grmeter->getARMeter() != 0) &&
		(grmeter->getARMeter()->getMeterType() != ARMeter::NONE ))
	{
		// length of measure
		mStaffState.curmeter = grmeter->getARMeter();
		mStaffState.meterset = true;
//		ARMeter::metertype mtype = mStaffState.curmeter->getMeterType();

	} 		
}

// ----------------------------------------------------------------------------
void GRStaff::setClefParameters(GRClef * grclef, GRStaffState::clefstate cstate)
{
	mStaffState.clefset = cstate;
	if (grclef == NULL)
	{ // Standard ...
		mStaffState.curclef = NULL;
		mStaffState.clefname = "";
		mStaffState.basepit = NOTE_G + mStaffState.basepitoffs;
		mStaffState.baseoct = 1;
		mStaffState.octava = 0;
		mStaffState.baseline = 3;
		avg_position = 0;
		mNoteCount = 0;
	}
	else
	{ // Standard ...
		const ARClef * aclef = /*dynamic*/static_cast<const ARClef *>(grclef->getAbstractRepresentation());
		mStaffState.curclef = aclef;
		mStaffState.clefname = aclef->getName();
		mStaffState.basepit = grclef->getBasePitch() + mStaffState.basepitoffs;
		mStaffState.baseoct = grclef->getBaseOct();
		mStaffState.octava = 0;
		mStaffState.baseline = grclef->getBaseLine();
	}

	// now we have to take into consideration the 
	// instrument-offset (e.g. "clarinet in A")
	// which changes the basepitch ....
	// (relative to the "normal" C-Major oriented scale)
}

// ----------------------------------------------------------------------------
void GRStaff::setKeyParameters(GRKey * inKey)
{
	// set current accidental 
	if (inKey == NULL)
	{
		mStaffState.curkey = NULL;
		mStaffState.keyset = false;
		mStaffState.numkeys = 0;
		for (int i = 0; i < NUMNOTES; ++i)
			mStaffState.KeyArray[i] = 0;
	 }
	 else
	 {
		mStaffState.keyset = true;
		mStaffState.curkey = /*dynamic*/static_cast<const ARKey *>( inKey->getAbstractRepresentation());
		mStaffState.numkeys = inKey->getKeyArray(mStaffState.KeyArray);
	 }
	 mStaffState.reset2key();
}

// ----------------------------------------------------------------------------
void GRStaff::setNoteParameters(const GRNote * inNote)
{
	if (inNote == 0)	return;

	// Test the current key!
	// Analyse the key 
	if (mStaffState.clefset == GRStaffState::CLEFAUTO)
	{
	  avg_position += inNote->getPosition().y;
	  ++mNoteCount;
	}

	// Reset of accidentals
	const ARNote * arnote = inNote->getARNote();
	const int tmppitch = arnote->getPitch() - NOTE_C;
	const int acc = arnote->getAccidentals() - (int)mStaffState.instrKeyArray[tmppitch];
	mStaffState.fMeasureAccidentals.setAccidental(tmppitch, arnote->getOctave(), acc + arnote->getDetune());
}

// ----------------------------------------------------------------------------
VGColor GRStaff::getNoteColor(TYPE_PITCH pit)  const
{
	if (pit >= NOTE_C && pit <= NOTE_H)		return (VGColor(0, 0, 0)); // black
	if (pit >= NOTE_CIS && pit <= NOTE_AIS)	return (VGColor(0, 0, 255)); // blue
	return (VGColor(255, 0, 0)); // red
}

// ----------------------------------------------------------------------------
/** \brief Returns the graphical y-position of a key.

	This algorithm works as follows:
	1. we check, whether we have a treble or a bass-clef 
	if yes, the upperbound and lowerbound is determined (g# for sharps and f& for flats)
	if not, we determine the upperbound for the topmost not within the system for sharps or
	the bottombound for the lowest note for flats.
*/
float GRStaff::getKeyPosition(TYPE_PITCH pit, int numkeys) const
{
	float topbound = 0;
	float bottombound = 0;
	if (	(mStaffState.basepit - mStaffState.basepitoffs == NOTE_G && mStaffState.baseline == 3 )
		||	(mStaffState.basepit - mStaffState.basepitoffs == NOTE_F && mStaffState.baseline == 1 ) )
	{   // treble- or bass-clef
		// flats ?
		if (numkeys < 0)
		{
			// determine lowerbound
			int baseoct = mStaffState.baseoct - 1;
			do {
				bottombound = getNotePosition( NOTE_F, baseoct );
				++ baseoct;
			}
			while (bottombound > (mStaffState.numlines) * getStaffLSPACE() );
			// Watch it here: the f-flat can be just outside (below lowest line)
		}
		else		// sharps
		{			
			int baseoct = mStaffState.baseoct + 1;
			do {
				topbound = getNotePosition( NOTE_G, baseoct );
				-- baseoct;
			}
			while (topbound < -getStaffLSPACE());
			// The g-sharp can be just above the top-line
		} 
	}
	else 
	{
		// we have another clef ...
		// find out the topmost and the bottommost note
		int topnote		= ((mStaffState.basepit - NOTE_C +  ( mStaffState.baseline * 2 )) % 7 ) + NOTE_C;
		int bottomnote	= ((mStaffState.basepit - NOTE_C ) - (int)(mStaffState.numlines - 1 - mStaffState.baseline) * 2 % 7);
		if (bottomnote < 0) bottomnote += 7;
		bottomnote += NOTE_C;

		// now find the positions for the notes.
		int baseoct = mStaffState.baseoct+1;
		do {
			topbound = getNotePosition(topnote, baseoct);
			-- baseoct;
		}
		while (topbound< 0);
		// The note must lie completly within the system.

		baseoct = mStaffState.baseoct - 1;
		do {
			bottombound = getNotePosition(bottomnote, baseoct);
			++ baseoct;
		}
		while (bottombound > (mStaffState.numlines - 1) * getStaffLSPACE());
		// the note must lie within the system!
	}
	
	// now find the real position ...
	if (numkeys >= 0)	// sharps
	{
		// find the TOPMOST position for pit, that lies below the topbound.
		int baseoct = mStaffState.baseoct+1;
		float tmp;
		while (( tmp = getNotePosition(pit, baseoct) ) < topbound)
			-- baseoct;
		return tmp;
	}

	// now for flats.
	// find the BOTTOMMOST position for pit that is above the bottombound
	int baseoct = mStaffState.baseoct - 1;
	float tmp;
	while ( (tmp = getNotePosition(pit, baseoct) ) > bottombound)
		++ baseoct;
	return tmp;
}

// ----------------------------------------------------------------------------
/** \brief Returns the graphical y-position of a note.

	Idea: Each clef has a root-note  (violin-
	clef has G, base-clef has F) and a
	corresponding staff line
*/
float GRStaff::getNotePosition(TYPE_PITCH pit, TYPE_REGISTER oct) const
{
//cerr << "GRStaff::getNotePosition " << pit << " " << oct << " state: " << mStaffState.basepit << " " << mStaffState.baseline << " " <<  mStaffState.baseoct << endl;
	return getNotePosition(pit, oct, mStaffState.basepit, mStaffState.baseline, mStaffState.baseoct);
}

// ----------------------------------------------------------------------------
/** \brief Returns the graphical y-position of a note.
*/
float GRStaff::getNotePosition(TYPE_PITCH pit, TYPE_REGISTER oct, int basePitch, int baseLine, int baseOct) const
{
// redundant correction of octave: already shifted when the GRNote is created
//	oct -= mStaffState.octava;	//  depends on current clef.

	const float myHalfSpace = getStaffLSPACE() * 0.5f;
	float calc = 0;
	if (pit >= NOTE_C && pit <= NOTE_H)
	{
		calc = (float)((basePitch - pit ) * myHalfSpace + baseLine * getStaffLSPACE() -
			((int)oct - baseOct) * (7 * myHalfSpace));
	}
	else if (pit>= NOTE_CIS && pit <= NOTE_DIS)
	{
		calc = (float)((basePitch - (pit - 7) )* myHalfSpace + baseLine * getStaffLSPACE()-
			((int)oct - baseOct) * (7 * myHalfSpace));
	}
	else if (pit>= NOTE_FIS && pit <= NOTE_AIS)
	{
		calc = (float)((basePitch - (pit - 6) )* myHalfSpace + baseLine * getStaffLSPACE() -
			((int)oct - baseOct) * (7 * myHalfSpace));
	}
	return calc;
}

// ----------------------------------------------------------------------------
/** \brief Returns the stem-direction.

	-1 : down
	1  : up
*/
GDirection GRStaff::getDefaultThroatDirection( TYPE_PITCH pit, TYPE_REGISTER oct) const
{
	const float tmp = getNotePosition(pit, oct);
	if (tmp <= mStaffState.numlines / 2 * getStaffLSPACE())
		return dirDOWN;

	return dirUP;
}

// ----------------------------------------------------------------------------
/** \brief Function for number of ledger lines.

	0: no ledger lines
	> 0 number of ledgerlines above staff
	< 0 number of ledgerlines below staff
*/
int GRStaff::getNumHelplines(TYPE_PITCH pit, TYPE_REGISTER oct) const
{
	// we get a "rounding-problem" for non-standard sizes ...
	// therfore we do it differently ...

	// we calculate the difference
	if (getStaffLSPACE() < kMinNoteSize) return 0;

	float calc = getNotePosition(pit, oct) / getStaffLSPACE();
	if (calc<0)		calc -= 0.25f;
	else			calc += 0.25f;

	int icalc = (int)(calc);
	if (icalc < 0)
		return ( -icalc);

	if (icalc>= mStaffState.numlines)
	  	return -(icalc-(mStaffState.numlines-1));

	return 0;
}

// ----------------------------------------------------------------------------
void GRStaff::checkSystemBar(const TYPE_TIMEPOSITION & pos)
{
	// whe the position has already been checked, we're in a multivoice staff: don't reset the key again 
	if (fLastSystemBarChecked == pos)	return;
	fLastSystemBarChecked = pos;
	GRSystemSlice * systemslice = getGRSystemSlice();
	if (systemslice && systemslice->hasSystemBars()) {
		GRBar* bar = systemslice->getBarAt(pos);
		if (bar && !bar->isFinishBar()) {
			mStaffState.reset2key ();
		}
	}
}

// ----------------------------------------------------------------------------
/** \brief Called by GRSingleNote::createNote, when it wants to build its
		accidental list.

	\bug This function migh be concerned with the "accidentals bug after key changes".
*/
AccList * GRStaff::askAccidentals( int p_pit, int p_oct, int p_acc, float detune)
{
	 DebugPrintState( "askAccidentals" );

	// Retrieve the staff-data 
	AccList * mylist = new AccList();
	if (p_pit >= NOTE_CIS && p_pit <= NOTE_AIS)
		mylist->AddHead(1);

	int pitchclass = p_pit - NOTE_C;
	const float classAccidental = mStaffState.fMeasureAccidentals.accidental(pitchclass);
	const float noteAccidental = mStaffState.fMeasureAccidentals.accidental(pitchclass, p_oct);
	const int shiftparm = (int)mStaffState.instrKeyArray[pitchclass];
	const float sounds = shiftparm + noteAccidental;
	if ((sounds == p_acc + detune)	&& (noteAccidental == classAccidental))
		return mylist;

	const float myacc = p_acc + detune - shiftparm;

	if (myacc == 0)
	{
		mylist->AddTail(-10); // ?
		return mylist;
	}
	mylist->AddTail(myacc);
	return mylist;
}

// ----------------------------------------------------------------------------
/* for debug purpose
*/
void GRStaff::DebugPrintState(const char * info) const
{
/*	static ofstream myof ( "c:\\GRStaff.txt" );


	int i;
	myof << "--------------------------------------------" << endl;
	myof << info << endl;
	   
	// - KeyArray
	myof << "KeyArray: ";
	for ( i = 0; i < NUMNOTES; ++i)
	   myof << mStaffState.KeyArray[i] << " ";

	myof << endl;

	// - instrKeyArray
	myof << "instrKeyArray: ";
	for ( i = 0; i < NUMNOTES; ++i)
	   myof << mStaffState.instrKeyArray[i] << " ";
  
	myof << endl;

	// - Misc

	myof << "keyset= " << mStaffState.keyset;
	myof << ", numkeys= " << mStaffState.numkeys;
	myof << ", clefset= " << mStaffState.clefset;
	myof << ", basepit= " << mStaffState.basepit;
	myof << ", basepitoffs= " << mStaffState.basepitoffs;
	myof << ", instrNumKeys= " << mStaffState.instrNumKeys;

	
	myof << endl;

//	myof.close();
*/

}

// ----------------------------------------------------------------------------
/** \brief A new measure.
*/
void GRStaff::newMeasure(const TYPE_TIMEPOSITION & tp, bool reset)
{
	// reset of accidentals
	// quick-hack-implementation -> encode which accidentals are already set
	// and which must be deleted in the next measure etc.
	if (reset) mStaffState.reset2key ();
}

// ----------------------------------------------------------------------------
const GRNote * GRStaff::getFirstNote() const
{
	const NEPointerList& elts = getElements();
	GuidoPos pos = elts.GetHeadPosition();
	while (pos) {
		const GRNotationElement* e = elts.GetNext(pos);
		const GRNote * grNote = e->isGRNote();
		if( grNote )
			return grNote;
	}
	return 0;
}

// ----------------------------------------------------------------------------
GRNote * GRStaff::getLastNote() const
{
	GuidoPos pos = mCompElements.GetTailPosition();
	GRNotationElement * e = 0;
	while (pos)
	{
		e = mCompElements.GetPrev(pos);
		GRNote * grNote = dynamic_cast<GRNote *>(e);
		if( grNote )
			return grNote;
	}
	return 0;
}

// ----------------------------------------------------------------------------
GRBar * GRStaff::getLastBar() const
{
	GuidoPos pos = mCompElements.GetTailPosition();
	GRNotationElement * e = 0;
	while (pos)
	{
		e = mCompElements.GetPrev(pos);
		GRBar * bar = dynamic_cast<GRBar *>(e);
		if( bar ) return bar;
	}
	return 0;
}

// ----------------------------------------------------------------------------
/** \brief Adds a Meter at the current position.
*/
GRMeter * GRStaff::AddMeter(const ARMeter * armeter)
{
	// First check, whether this really means
	// a Change in Meter? If not, do not display anything!
	GRMeter * nmeter = new GRMeter(armeter, this);
	TYPE_TIMEPOSITION tmp ( getRelativeTimePosition() );
	GRNotationElement * el = mCompElements.GetTail();
	if (el) 
		tmp = el->getRelativeEndTimePosition();

	nmeter->setRelativeTimePosition(tmp);
	setMeterParameters(nmeter);
	addNotationElement(nmeter);
	return nmeter;
}

// ----------------------------------------------------------------------------
GROctava * GRStaff::AddOctava(const AROctava * aroct)
{
	const char * s = "";
	switch (aroct->getOctava()) {
		case 3 : s = "22"; break;
		case 2 : s = "15"; break;
		case 1 : s = "8"; break;
		case 0 : s = ""; break;
		case -1 : s = "8"; break;
		case -2 : s = "15"; break;
		case -3 : s = "22"; break;
	}
//	if (aroct->getOctava() == 1)		s = "8";
//	else if (aroct->getOctava() == 0)	s = "";
//	else if (aroct->getOctava() == -1)	s = "8ba";
	
	GROctava * groctava = new GROctava(this, s, aroct, (aroct->getOctava() < 0));
	groctava->setRelativeTimePosition( aroct->getRelativeTimePosition());
    if (aroct->getColor() && aroct->getColor()->getValue())
        groctava->setColRef(aroct->getColor());
	AddTag(groctava);

	mStaffState.octava  = aroct->getOctava();	
	return groctava;
}

// ----------------------------------------------------------------------------
void GRStaff::AddTag(GRNotationElement * grtag)
{
	addNotationElement(grtag);
}

// ----------------------------------------------------------------------------
/** \brief This creates a repeatBegin.
*/
GRRepeatBegin * GRStaff::AddRepeatBegin(ARRepeatBegin *arrb)
{
    assert(arrb);
	if (mStaffState.curbarfrmt) arrb->setRanges (mStaffState.curbarfrmt->getRanges());
	GRRepeatBegin * tmp = new GRRepeatBegin(arrb, this);
	if (mStaffState.curbarfrmt && (mStaffState.curbarfrmt->getStyle() == ARBarFormat::kStyleSystem))
		mGrSystemSlice->addRepeatBegin(tmp, mStaffState.curbarfrmt->getRanges(), this);

	addNotationElement(tmp);
    // repeatBegin must reset the keyState, jfk, 9/2015
    mStaffState.reset2key ();
	return tmp;
}

// ----------------------------------------------------------------------------
/** \brief This creates a repeatEnd 
*/
GRRepeatEnd * GRStaff::AddRepeatEnd( ARRepeatEnd * arre )
{
	assert (arre);
	if (mStaffState.curbarfrmt) arre->setRanges(mStaffState.curbarfrmt->getRanges());
	GRRepeatEnd * tmp = new GRRepeatEnd(arre, this, arre->getRelativeTimePosition(), fProportionnalRendering);
	if (mStaffState.curbarfrmt && (mStaffState.curbarfrmt->getStyle() == ARBarFormat::kStyleSystem))
		mGrSystemSlice->addRepeatEnd(tmp, mStaffState.curbarfrmt->getRanges(), this);
	addNotationElement(tmp);
	tmp->updateBoundingBox();
	 // repeatBegin must reset the keyState, jfk, 9/2015
	mStaffState.reset2key ();
	return tmp;
}

// ----------------------------------------------------------------------------
/* range not any more supported - DF sept 1 2004
GRRepeatEnd * 
GRStaff::AddRepeatEndRangeEnd( ARRepeatEndRangeEnd * arre )
{
	// (JB) test for measure repeat, always create:
	
	// this creates a repeatEnd ...
//	GRRepeatEnd * tmp = new GRRepeatEnd(arre);
//	addNotationElement(tmp);
//	return tmp;

//  was:
	
	// this creates a repeatEnd ...
//	const int numRepeat = arre->getNumRepeat();
//	if( numRepeat == 1 )
//	{
//		GRRepeatEnd * tmp = new GRRepeatEnd(arre);
//		addNotationElement(tmp);
//		return tmp;
//	}

	return 0;
}
*/

// ----------------------------------------------------------------------------
/** \brief Returns -1 if staff is not in the system.
*/
int 
GRStaff::getStaffNumber() const
{
	GRSystem * system = getGRSystem();
	if( system )
		return system->getStaffNumber( this );
	
	return -1;
}

// ----------------------------------------------------------------------------
/** \brief this routine exists so that tranposing instruments
	can be handled.

	It checks the tranps-parameter of the instrument
	tag and sets a pitch-offset accordingly. 
*/
GRInstrument * GRStaff::AddInstrument(const ARInstrument * arinstr)
{
	GRInstrument * tmp = new GRInstrument(arinstr, this);
	addNotationElement(tmp);
	if (arinstr->repeat())
		mStaffState.fInstrument = tmp;
	bool downwards = true;

	// now we need to test, whether the instrument is transposed (e.g. "clarinet in A")
	const string& ts = arinstr->getTransp();
	if ( ts.size() )
	{
		// then we have a transposition ....
		// this is always relative to "C-Major"
		// See MGG, Volume 9, column 1651

		// we can interpret this either as a number or as a string
		// "A" means A-Major
		// "B&" means B-flat Major etc.

		// we have to calculate distance from C ..
		// so we just interpret this as a notename and change the case 
		const NVstring & mystr = ts; //ts->getValue();

		if (mystr.length() >= 1)
		{
		  NVstring notename ( mystr.substr(0, 1));

		  notename.to_lower();
		  int pitch = gd_noteName2pc(notename.c_str());

		  int keynumber = 0; // C-Major is standard ...

		  // now for later key-stuff ....
				int t = (int)mystr[0];
				
				int major = 0;
				if (t == toupper(t) ) // uppercase letter
					major = 1; // major key
				t = toupper(t);
				
				switch (t) {
					case 'F': 	keynumber = -1;	break;
					case 'C': 	keynumber = 0;	break;
					case 'G': 	keynumber = 1;	break;
					case 'D': 	keynumber = 2;	break;
					case 'A': 	keynumber = 3;	break;
					case 'E': 	keynumber = 4;	break;
					case 'H':
					case 'B':	keynumber = 5;	break;
					default:	 keynumber = 0;
				}
				
				if (!major)
					keynumber -= 3; // minus 3 accidentals (A-Major becomes a-minor ...)
				
				if (mystr.length() > 1)
				{
					t = mystr[1];
					
					if (t == '#')	keynumber += 7;
					if (t == '&')	keynumber -= 7;
					if (t == '+')	downwards = false;
				}
				if (mystr.length() > 2)
				{
					t = mystr[2];
					if (t == '+')
						downwards = false;
				}

				// we can now work with an implicit key ....
				mStaffState.instrNumKeys = GRKey::getNonFreeKeyArray (keynumber, mStaffState.instrKeyArray);
				mStaffState.reset2key();

		  // then we have a pitch ...
		  int value = pitch - NOTE_C;
		  if (downwards)
		  {
			  // we just do the downward stuff ...
			  value -= 7;
		  }

		  // the value is the pitch-shift ...
		  mStaffState.basepitoffs = value;
		  mStaffState.instrNumKeys = keynumber;
		}
	}
	else mStaffState.basepitoffs = 0;
	return tmp;
}

// ----------------------------------------------------------------------------
GRBar * GRStaff::AddBar(ARBar * abar, const TYPE_TIMEPOSITION & date)
{
staff_debug("AddBar");
	newMeasure(date, !fInhibitNextReset2key); // erhoeht u.a. mnum!
	fInhibitNextReset2key = false;

	if (mStaffState.curbarfrmt) abar->setRanges(mStaffState.curbarfrmt->getRanges());
	GRBar * bar = new GRBar( abar, this, date, fProportionnalRendering);
	// depending on current bar Format, we have to tell the staffmanager (or the system) 
	if (mStaffState.curbarfrmt && (mStaffState.curbarfrmt->getStyle() == ARBarFormat::kStyleSystem))
		mGrSystemSlice->addBar(bar, mStaffState.curbarfrmt->getRanges(), this);
	addNotationElement(bar);
	return bar;
}

// ----------------------------------------------------------------------------
GRKey * GRStaff::AddKey (const ARKey * arkey)
{
	const ARNaturalKey * natkey = dynamic_cast<const ARNaturalKey *>(arkey);
	// it may happen, that the natkey is not needed because a newsystem has occured!
	if (natkey && mStaffState.curkey)
	{
		// else we have to naturalize ...
		// First, we copy the current key
		ARKey * arnatkey = new ARKey(*mStaffState.curkey);
		GRKey * grkey = new GRKey(this, arnatkey, 1, 1);

		// the new grkey MUST get a Spring / and a Rod otherwise, we have a Problem!
		TYPE_TIMEPOSITION tmp = getRelativeTimePosition();
		GRNotationElement * el = mCompElements.GetTail();
		if (el) 
			tmp = el->getRelativeEndTimePosition();
		grkey->setRelativeTimePosition(tmp);
		addNotationElement(grkey);
		return grkey;
	}

	if (arkey->getIsAuto())
	{
		// then we can change the keynumber if we want to ....
		if (mStaffState.instrNumKeys != arkey->getKeyNumber() )
cerr << "==============================================>  GRStaff::AddKey auto setKN " << mStaffState.instrNumKeys << endl;
//			arkey->setKeyNumber(mStaffState.instrNumKeys);
	}

	GRKey * key = new GRKey(this, arkey, 0, 0);
	TYPE_TIMEPOSITION tmp = getRelativeTimePosition();
	GRNotationElement * el = mCompElements.GetTail();
	if (el) 
		tmp = el->getRelativeEndTimePosition();

	key->setRelativeTimePosition(tmp);
	setKeyParameters(key);
	addNotationElement(key);
	return key;
}

// ----------------------------------------------------------------------------
GRText * GRStaff::AddText(const ARText * atext)
{
	GRText * gtext = new GRText(this, atext);
	addNotationElement(gtext);

	return gtext;
}

// ----------------------------------------------------------------------------
GRIntens * GRStaff::AddIntens(const ARIntens * aintens)
{
	GRIntens * gintens = new GRIntens(this, aintens);
	addNotationElement(gintens);
	return gintens;
}

// ----------------------------------------------------------------------------
void GRStaff::BeginStaff(GRStaffManager * staffmgr)
{
staff_debug("BeginStaff");
	if (startglue == 0)
	{
		startglue = new GRGlue(this);
		startglue->setRelativeTimePosition(getRelativeTimePosition());
		// always at the beginning!
		mCompElements.AddHead(startglue);
	}

	if (secglue == 0)
	{
		secglue = new GRGlue(this, -1);
		secglue->setRelativeTimePosition( getRelativeTimePosition());
		addNotationElement(secglue);
	}

	// adds the glue to the first spring of the current staffmanager.
	staffmgr->addElementToSpring(startglue, 0);
}

// ----------------------------------------------------------------------------
void GRStaff::EndStaff(const TYPE_TIMEPOSITION & tp, GRSpring * spr)
{
staff_debug("EndStaff 1");
	assert(!endglue);

	setRelativeEndTimePosition(tp);

	// now we add a endbar ...
	endglue = new GRGlue(this, 0);
	endglue->setRelativeTimePosition(tp);
	addNotationElement(endglue);

	// adds the End-Glue to the last spring ....
	spr->addElement(endglue, NULL);
	endglue->tellSpringID(spr->getID());
}

// ----------------------------------------------------------------------------
/** \brief Ends the staff. Returns the position of the element after endpos
	(only if endpos != NULL) else returns NULL.
*/
GuidoPos GRStaff::EndStaff(const TYPE_TIMEPOSITION & tp, GRStaffManager * staffmgr, GuidoPos endpos, int lastline)
{
staff_debug("EndStaff 2");
	assert(!endglue);

	GuidoPos returnpos = endpos;
	if (returnpos)
		mCompElements.GetNext(returnpos);

	if (endpos)
	{
		// now, we set the last element to endpos ...
		mCompElements.SetTailPosition(endpos);
	}

	setRelativeEndTimePosition(tp);

	// now we add a endbar ...

	endglue = new GRGlue(this, 0);
	endglue->setRelativeTimePosition(tp);
	addNotationElement(endglue);

	// adds the End-Glue to the last spring ....
	staffmgr->addElementToSpring(endglue, -1);

	return returnpos;
}


// ----------------------------------------------------------------------------
GRDoubleBar * GRStaff::AddDoubleBar(ARDoubleBar * ardbar, const TYPE_TIMEPOSITION & date)
{
staff_debug("AddDoubleBar");
	newMeasure(date, !fInhibitNextReset2key); // erhoeht u.a. mnum!
	fInhibitNextReset2key = false;

	if (mStaffState.curbarfrmt) ardbar->setRanges(mStaffState.curbarfrmt->getRanges());
	GRDoubleBar * ntakt = new GRDoubleBar( ardbar, this, date, fProportionnalRendering);
	// depending on current bar Format, we have to tell the staffmanager (or the system) 
	if (mStaffState.curbarfrmt) {
		if (mStaffState.curbarfrmt->getStyle() == ARBarFormat::kStyleSystem)
			mGrSystemSlice->addDoubleBar(ntakt, mStaffState.curbarfrmt->getRanges(), this);
	}
	addNotationElement(ntakt); // change of Measuretime ...
	return ntakt;
}

// ----------------------------------------------------------------------------
GRFinishBar * GRStaff::AddFinishBar(ARFinishBar * arfbar, const TYPE_TIMEPOSITION & date)
{
staff_debug("AddFinishBar");
	newMeasure(date, false); // erhoeht u.a. mnum!
	if (mStaffState.curbarfrmt) arfbar->setRanges(mStaffState.curbarfrmt->getRanges());
	GRFinishBar * ntakt = new GRFinishBar( arfbar, this, date, fProportionnalRendering);

	// depending on current bar Format, we have to tell the staffmanager (or the system) 
	if (mStaffState.curbarfrmt && (mStaffState.curbarfrmt->getStyle() == ARBarFormat::kStyleSystem))
		mGrSystemSlice->addFinishBar(ntakt, mStaffState.curbarfrmt->getRanges(), this);
	addNotationElement(ntakt); // change of Measuretime ...
	return ntakt;
}

// ----------------------------------------------------------------------------
/** \brief CreateBeginElements() routine is called when a new staff has been
	constructed by a conversion of a potential break into a real break.
*/
void GRStaff::CreateBeginElements( GRStaffManager * staffmgr, GRStaffState & state, int staffnum )
{
staff_debug("CreateBeginElements");
	mStaffState.basepitoffs = state.basepitoffs;
	mStaffState.instrNumKeys = state.instrNumKeys;
	mStaffState.fInstrument = state.fInstrument;
	
	// we have to look, what kind of state-settings are set.
	if (state.curbarfrmt != NULL)
		setBarFormat(state.curbarfrmt);

	if (state.curstaffrmt != NULL)
		setStaffFormat(state.curstaffrmt);

	if (state.curclef != NULL) {
		// now I have to deal with basepitoffs (for transposed instruments)

		ARClef * arclef = new ARClef(*state.curclef);
		arclef->setRelativeTimePosition( getRelativeTimePosition() );
        arclef->setIsInHeader(true);
		// owns abstract ....!
		GRClef * grclef = new GRClef(arclef, this, 1);

		// grclef->setRelativeTimePosition(relativeTimePositionOfGR);
		assert(grclef->getRelativeTimePosition() == getRelativeTimePosition());

		setClefParameters(grclef, GRStaffState::CLEFEXPLICIT);
		addNotationElement(grclef);

		// this handles the spring-synchronization ...
		staffmgr->AddGRSyncElement(grclef, this, staffnum, NULL);	
	}
	if (state.curkey != NULL)
	{
		// I have to think about keynumber (transposed instruments)

		ARKey * arkey = new ARKey(*state.curkey);
		arkey->setRelativeTimePosition(getRelativeTimePosition());
        arkey->setIsInHeader(true);

		GRKey * grkey = new GRKey(this, arkey, 0, 1);

		// grkey->setRelativeTimePosition(relativeTimePositionOfGR);
		assert(grkey->getRelativeTimePosition() == getRelativeTimePosition() );
		setKeyParameters(grkey);		
		addNotationElement(grkey);

		// this handles the spring-synchronization ...
		staffmgr->AddGRSyncElement(grkey, this, staffnum, NULL);	
	}
	else if (state.instrNumKeys != 0)
	{
		// then I have to set the state information anyways ....
		mStaffState.instrNumKeys = GRKey::getNonFreeKeyArray(state.instrNumKeys, mStaffState.instrKeyArray);
		mStaffState.reset2key ();
	}

	// now we add the secondglue
	assert(!secglue);
	secglue = new GRGlue(this, -1);
	secglue->setRelativeTimePosition( getRelativeTimePosition() );
	addNotationElement(secglue);
	staffmgr->AddGRSyncElement(secglue, this, staffnum, NULL);
}

// ----------------------------------------------------------------------------
/** \brief createNewRods is called to create the rods for a staff.
	The lastrod is saved internally, so that only rods are created for new neighbors.
*/
void GRStaff::createNewRods(GRStaffManager * staffmgr, int & startspr, int & endspr, float optForce)
{
staff_debug("createNewRods");
	// ATTENTION, needs to be realized.
	// including firstrod/lastrod etc...

	GuidoPos pos = mCompElements.GetHeadPosition();

	NEPointerList mylist(0);

	GRNotationElement * el = mCompElements.GetAt(pos);
	int sprid = -10000;

	GCoord rightspace = 0;
	GCoord leftspace = 0;
	GCoord newrightspace = 0;
	int prevsprid = startspr;
	while (pos)		
	{
		// now, we gather all the elements with the same springID together. 
		// Then we do a collision-detecion and then give out a rod for this area.
		el = mCompElements.GetAt(pos);

		if (!el->getNeedsSpring())
		{
			mCompElements.GetNext(pos);
			continue;
		}

		if (el->getSpringID()>=endspr)
			break;

		if (sprid == -10000)
		{
			sprid = el->getSpringID();
		}

		if (el->getSpringID() == sprid)
		{
			mylist.AddTail(el);
			if (el->getLeftSpace() > leftspace)
				leftspace = el->getLeftSpace();
			if (el->getRightSpace() > newrightspace)
				newrightspace = el->getRightSpace();
		}
		else
		{
			// now deal with the elements in mylist.

			// do a collision check .. (later)

			GuidoPos mypos = mylist.GetHeadPosition();
			while (mypos)
			{
				/*GRNotationElement *el =*/ mylist.GetNext(mypos); // commented to supress warning
				// check for colloisions ...
			}

			if (prevsprid != sprid)
			{
				if (rightspace+leftspace>0)
				{
					GRRod * rod = new GRRod(rightspace +
						leftspace, prevsprid, mylist.GetHead()->getSpringID(), optForce);
					// no space, add at head
					staffmgr->addRod(rod, 0, 1);
					lastrod = rod;
				}
			}

			prevsprid = mylist.GetHead()->getSpringID();
			sprid = el->getSpringID();

			mylist.RemoveAll();

			rightspace = newrightspace;
			newrightspace = 0;
			leftspace = 0;

			continue;
		}

		sprid = el->getSpringID();
		mCompElements.GetNext(pos);
	}

	// now, we deal with the rest ...
	if (mylist.size() > 0)
	{
		// do a collision check (later)

		int mysprid = mylist.GetHead()->getSpringID();
		// this is the rod between the previous and the current spring
		if (leftspace + rightspace > 0)
		{
			GRRod * rod = new GRRod(leftspace + rightspace, prevsprid, mysprid, optForce);
			
			// no space, add at head
			staffmgr->addRod(rod, 0, 1);
			lastrod = rod;
		}
		// this is a future rod ....
		if (newrightspace > 0)
		{
			GRRod * rod = new GRRod(newrightspace, mysprid, mysprid + 1, optForce);
			staffmgr->addRod(rod, 0, 1);
			lastrod = rod;
		}
	}
}

// ----------------------------------------------------------------------------
/** \brief Called by GRPossibleBreakState::GRVoiceTagsAndStaff::BeginAfterBreak
	to ensure that the open tags are inserted at the right position.
*/
void GRStaff::AddElementAt(GuidoPos pos, GRNotationElement * el)
{
	// no time-check whatsoever is performed.
	mCompElements.AddElementAt(pos, el);
}

// ----------------------------------------------------------------------------
/** \brief Tells all elements from position pos 
	that they now belong to a new staff.
*/
void GRStaff::TellNewStaff(GuidoPos pos)
{
staff_debug("TellNewStaff");
	while (pos)
	{
		GRNotationElement * el = mCompElements.GetNext(pos);
		
		GRPositionTag * pt = dynamic_cast<GRPositionTag *>(el);
		if (pt)
			pt->changeCurrentSystem(this->getGRSystem());
		else
			el->setGRStaff(this);
	}
}

// ----------------------------------------------------------------------------
/** \brief Called by the staff manager to update the staff-state
	after a finishSyncSlice has produced a conflict.

	When this function is called, there will be just one active entry per
	springID in the staff elements.
	This routine goes from the position following pos to the tail of the element-
	list and sets the staffstate of the active staff-tags.
*/
void GRStaff::UpdateStaffState(GuidoPos pos)
{
staff_debug("UpdateStaffState");
	if (pos == 0)
		pos = mCompElements.GetHeadPosition();
	
	while (pos)
	{
		GRNotationElement * el = mCompElements.GetNext(pos);
		GRTag * tag = dynamic_cast<GRTag *>(el);
		if (tag)
		{
			if (!tag->getError() && tag->IsStateTag())
			{
				GRClef * clef = dynamic_cast<GRClef *>(tag);
				GRKey * key;
				if (clef)
				{
					setClefParameters(clef);
				}
				else if ((key = dynamic_cast<GRKey *>(tag)) != 0)
				{
					setKeyParameters(key);
					key->recalcVerticalPosition();
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
/** \brief Called if there is a forced second glue in the voice; 
		
		this happens always, if a newSystem-Tag has been encountered before.
*/
void GRStaff::AddSecondGlue(GRGlue * myglue)
{
staff_debug("AddSecondGlue");
	if (secglue)
	{
		if (!secglue->mPartner)
		{
			myglue->addAssociation(secglue);
			secglue->mPartner = myglue;
		}
	}
	else
		secglue = myglue;
	// now we have to make sure, that myglue and secglue are tied to the same springID!
	addNotationElement(myglue);
}

// ----------------------------------------------------------------------------
void GRStaff::setStaffFormat( const ARStaffFormat * staffrmt)
{
	mStaffState.curstaffrmt = staffrmt;
    
	if (staffrmt) {
		const TagParameterFloat* size = staffrmt->getSize();
		if (size && size->TagIsSet())
			mStaffState.staffLSPACE = size->getValue() * 2;
		else if (staffrmt->isTAB())
			mStaffState.staffLSPACE = 70.f;
		
		mStaffState.numlines = staffrmt->getLinesCount();		
        mStaffState.lineThickness = staffrmt->getLineThickness();
        mStaffState.staffDistance = staffrmt->getStaffDistance();
        if (staffrmt->getDY())
            mStaffState.fYOffset = - (staffrmt->getDY()->getValue());
        if (staffrmt->getColor()) {
            if (!mStaffState.colRef)
                mStaffState.colRef = new unsigned char[4];
            staffrmt->getColor()->getRGB(mStaffState.colRef);
        }
	}
}

// ----------------------------------------------------------------------------
void GRStaff::setBarFormat(const ARBarFormat * barfrmt)
{
	mStaffState.curbarfrmt = barfrmt;
}

// ----------------------------------------------------------------------------
/** \brief Copies the relevant instrument information
	for a "new" staff. Namely the transposition parameters
*/
void GRStaff::setInstrumentFormat(const GRStaffState & state)
{
	mStaffState.basepitoffs = state.basepitoffs;
	mStaffState.instrNumKeys = state.instrNumKeys;
	// what happens with key? Wait and see.
	int i;
	for (i=0; i < NUMNOTES; ++i)
	{
		mStaffState.instrKeyArray[i] = state.instrKeyArray[i];
	}
	mStaffState.fInstrument = state.getRepeatInstrument();
}

// ----------------------------------------------------------------------------
/** \brief Called by prepare staff from GRStaffManager
	this is done everytime a "new" staff is created
	(which is mostly part of a systemslice)
*/
void GRStaff::setStaffState(GRStaffState * state)
{
staff_debug("setStaffState");
	// \bug it looses key infos ?
	// we have to deal with key and clef stuff ....
	mStaffState.curclef = state->curclef;
	mStaffState.clefname = state->clefname; 
	mStaffState.basepit = state->basepit;
	mStaffState.baseoct = state->baseoct;
	mStaffState.octava = state->octava;
	mStaffState.baseline = state->baseline;

	mStaffState.keyset = state->keyset;
	mStaffState.curkey = state->curkey;
	mStaffState.numkeys = state->numkeys;
	mStaffState.fInstrument = state->fInstrument;
	mStaffState.fMultiVoiceCollisions = state->fMultiVoiceCollisions;
	for ( int i = 0; i < NUMNOTES; ++i )
	{
//		mStaffState.MeasureAccidentals[i]	= state->MeasureAccidentals[i];
//		mStaffState.MeasureDetune[i]		= state->MeasureDetune[i];	
		mStaffState.KeyArray[i]	= state->KeyArray[i];
	}

	mStaffState.fMeasureAccidentals	= state->fMeasureAccidentals;

	setInstrumentFormat(*state);
	setStaffFormat(state->curstaffrmt);
	setBarFormat(state->curbarfrmt);

	mStaffState.distanceset = state->distanceset;
	mStaffState.distance = state->distance;
// DebugPrintState( "setStaffState apres" );

}

// ----------------------------------------------------------------------------
GRGlue * GRStaff::getStartGlue() const		{ return startglue; }
GRGlue * GRStaff::getEndGlue() const		{ return endglue; }
GRGlue * GRStaff::getSecondGlue() const		{ return secglue; }

// ----------------------------------------------------------------------------
GRSystem * GRStaff::getGRSystem() const
{
	if (!mGrSystem && mGrSystemSlice)
		return mGrSystemSlice->getGRSystem();

	return mGrSystem;
}

// ----------------------------------------------------------------------------
GRSystemSlice * GRStaff::getGRSystemSlice() const
{
	return mGrSystemSlice;
}

// ----------------------------------------------------------------------------
float GRStaff::getDistance() const
{
	if (mStaffState.distanceset)
		return mStaffState.distance;
	return (float)(-1.0);
}

// ----------------------------------------------------------------------------
/** \brief Returns the height covered by the staff lines.
	
	It is almost always different than the height of bounding box.
	It depends only on the number of the staff lines and the space between two lines.

	Currently, it does not take in account the thickness of lines.
*/
float GRStaff::getDredgeSize() const
{
	const int lineCount = getNumlines();
	const float lineSpace = getStaffLSPACE();

	return ((lineCount - 1) *  lineSpace); // TODO: add kLineThick ?
}

// ----------------------------------------------------------------------------
/** \brief Returns the height covered by the staff lines, only for staff mapping !

    It takes line thickness in account.
*/
float GRStaff::getMappingDredgeSize() const
{
    const int   lineCount = (getNumlines() > 0 ? getNumlines() : 1);
	const float lineSpace = getStaffLSPACE();

    float result = (lineCount - 1) * lineSpace + getLineThickness() * 0.5f + 1; // 1 in order to have a non-null height

	return result;
}

// ----------------------------------------------------------------------------
int GRStaff::getFontSize() const
{
	return (int)(getStaffLSPACE() * 4);
}

// ----------------------------------------------------------------------------
float GRStaff::FirstNoteORRestXPos() const
{
	GuidoPos pos = mCompElements.GetHeadPosition();
    while (pos) {
        GRNotationElement * e = mCompElements.GetNext(pos);
        GREvent * ev = dynamic_cast<GREvent *>(e);
        if (ev) {
			NVPoint p = ev->getPosition() + ev->getOffset();
			const NVPoint & refpos = ev->getReferencePosition();
			return p.x + refpos.x * ev->getSize() + ev->getBoundingBox().left;
		}
    }
	return getPosition().x;
}

// ----------------------------------------------------------------------------
void GRStaff::FinishStaff() 
{
    traceMethod("FinishStaff");
	NVRect r; NVRect tmp; NVPoint p;
    vector<GRPositionTag *> ptags;
	TYPE_DURATION duration;
	TYPE_TIMEPOSITION date;

	GuidoPos pos = mCompElements.GetHeadPosition();
    while (pos) {
        GRNotationElement * e = mCompElements.GetNext(pos);
        if (e) {
            GRPositionTag * ptag = dynamic_cast<GRPositionTag *>(e);
            if (ptag)
                ptags.push_back(ptag);
			if (e->getRelativeTimePosition() > date) {
				date = e->getRelativeTimePosition();
				duration = e->getDuration();
			}
      }
    }
	setDuration (date - getRelativeTimePosition() + duration);
    vector<GRPositionTag *>::iterator i;
    for (i=ptags.begin(); i!=ptags.end(); i++) {
       (*i)->FinishPTag (this);
    }
    if (mStaffState.fMultiVoiceCollisions) checkMultiVoiceNotesCollision();
	updateBoundingBox();
//	GRStaffOnOffVisitor v;
//	accept (v);
}

// ----------------------------------------------------------------------------
/* 
	the method is actually called by GRSystemSlice::Finish()
	it computes a 'preview' of the staff bounding box, in particular
	events positions seems not to be fully known at the time of the call.
	Don't know how it work but it does !! DF - sept 14 2009
*/
void GRStaff::boundingBoxPreview()
{
    traceMethod("boundingBoxPreview");
#ifdef EXTENDEDBB
	updateBoundingBox();
	return;
#endif
    mBoundingBox.Set (0,0,0,0);
	GuidoPos pos = mCompElements.GetHeadPosition();
	while (pos)
	{
		GRNotationElement * e = mCompElements.GetNext(pos);
		NVRect eltBox (e->getBoundingBox());
		eltBox += e->getPosition();
		mBoundingBox.Merge( eltBox );
	 }
}

// ----------------------------------------------------------------------------
/* 
	the method was called by GRSystemSlice::Finish()
	it has been renamed boundingBoxPreview() (see above)
	and the final bounding box is computed by GRStaff::FinishStaff()
*/
void GRStaff::updateBoundingBox()
{
    traceMethod("updateBoundingBox");
	NVRect r; NVRect tmp; NVPoint p;
    mBoundingBox.Set (0,0,0,0);
	
	GuidoPos pos = mCompElements.GetHeadPosition();
    while (pos) {
        GRNotationElement * e = mCompElements.GetNext(pos);
        if (e) {
#ifdef EXTENDEDBB
			if (e->isGRSlur()) {
				tmp = e->getBoundingBox() + e->getPosition();
				if (tmp.Height() < 300) {
					if (r.top > tmp.top) 		r.top = tmp.top;
					if (r.bottom < tmp.bottom)	r.bottom = tmp.bottom;
				}
			}

			GRPositionTag * ptag = dynamic_cast<GRPositionTag *>(e);
			if (ptag) {
				if (e->isText()) { // || e->isGRHarmony()) {
					tmp = e->getBoundingBox() + e->getPosition();
					if (r.top > tmp.top) 		r.top = tmp.top;
					if (r.bottom < tmp.bottom)	r.bottom = tmp.bottom;
				}
				continue;
			}

			const GRSingleNote * note = e->isSingleNote();
			if (note) {
				NVRect b = note->getEnclosingBox();
				if (b.Height() < 500) {
					if (r.top > b.top) 			r.top = b.top;
					if (r.bottom < b.bottom)	r.bottom = b.bottom;
				}
			}
#else
			GRPositionTag * ptag = dynamic_cast<GRPositionTag *>(e);
			if (ptag) {
				continue;
			}
#endif
            tmp = e->getBoundingBox();
            p = getPosition();
            p.y = 0;
            tmp += e->getPosition() - p;
            r.Merge (tmp);
        }
    }
	mBoundingBox.Merge (r);
	mMapping.top    = mMapping.left = - getLineThickness() / 2;
	mMapping.right  = mLength + getLineThickness() / 2;;
	mMapping.bottom = getMappingDredgeSize();
	mMapping       += mPosition + getOffset();
}

// ----------------------------------------------------------------------------
/** \brief gives the previous staff
*/
GRStaff * GRStaff::getPreviousStaff() const
{
    GRSystem * system = getGRSystem();
    GRSystemSlice * curslice = getGRSystemSlice();
    if (!system || !curslice) return 0;

    const SSliceList * sl = system->getSlices();          // get the list of system slices
    if (!sl) return 0;
    
    GuidoPos pos = sl->GetElementPos(curslice);                 // looks for the current slice
    GRSystemSlice * previousSlice = pos ? sl->GetPrev(pos) : 0;  // get the previous twice
    previousSlice = pos ? sl->GetPrev(pos) : 0;                  // this is to skip the current slice
    if (!previousSlice) return 0;                               // fails to find the previous

	int	num = curslice->getStaffNumber(this);

    const StaffVector * sv = previousSlice->getStaves();  // get the staves list
    if (!sv) return 0;

    GRStaff * pstaff = sv->Get(num);                // get the staff carrying the same number
    return pstaff;
}

GRStaff * GRStaff::getNextStaff() const
{
    GRSystem * system = getGRSystem();
    GRSystemSlice * curslice = getGRSystemSlice();
    if (!system || !curslice) return 0;

    const SSliceList * sl = system->getSlices();          // get the list of system slices
    if (!sl) return 0;
    
    GuidoPos pos = sl->GetElementPos(curslice);                 // looks for the current slice
    GRSystemSlice * nextSlice = pos ? sl->GetNext(pos) : 0;  // get the previous twice
    nextSlice = pos ? sl->GetNext(pos) : 0;                  // this is to skip the current slice
    if (!nextSlice) return 0;                               // fails to find the previous

	int	num = curslice->getStaffNumber(this);

    const StaffVector * sv = nextSlice->getStaves();  // get the staves list
    if (!sv) return 0;

    GRStaff * pstaff = sv->Get(num);                // get the staff carrying the same number
    return pstaff;
}


// ----------------------------------------------------------------------------
/** \brief Gives the bottom of a staff

	Browse the previous and next staves and returns the max of the bounding boxes bottom
*/
float GRStaff::getStaffBottom() const
{
	float bottom = mBoundingBox.bottom;

	const GRStaff* prev = getPreviousStaff();
	while (prev) {
		if (prev->getBoundingBox().bottom > bottom)
			bottom = prev->getBoundingBox().bottom;
		prev = prev->getPreviousStaff();
	}

	const GRStaff* next = getNextStaff();
	while (next) {
		if (next->getBoundingBox().bottom > bottom)
			bottom = next->getBoundingBox().bottom;
		next = next->getNextStaff();
	}
	return bottom;
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRStaff::GetMap( GuidoElementSelector sel, MapCollector& f, MapInfos& infos ) const
{
	if (sel == kGuidoStaff) {
		SendMap (f, getRelativeTimePosition(), getDuration(), kStaff, infos);
	}
	else {
		//	DrawNotationElements
		infos.fPos.y += mPosition.y;
		GuidoPos pos = mCompElements.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * e = mCompElements.GetNext(pos);
			if (e) e->GetMap (sel, f, infos);
		}
		infos.fPos.y -= mPosition.y;
	}
}

// ----------------------------------------------------------------------------
/** \brief Draws the staff lines and notations elements.
*/
void GRStaff::OnDraw( VGDevice & hdc ) const
{
    traceMethod("OnDraw");
#if 0
	// - Change font settings
	const int fontsize = getFontSize();
	
	const VGFont* myfont = FontManager::FindOrCreateFont( fontsize );
    const VGFont* hfontold = hdc.GetMusicFont();
    hdc.SetMusicFont( myfont );
	hdc.SetFontAlign( VGDevice::kAlignLeft | VGDevice::kAlignBase );

	// - Draw the staff
	// DrawStaffUsingSymbolScale( hdc );
	DrawStaffUsingSymbolRepeat( hdc );

	// - Restore font settings
//	hdc.SetTextAlign( ta );
// 	hdc.SelectFont( hfontold );// JB test for optimisation: do not restore font context.

#else
	DrawStaffUsingLines(hdc);	
#endif
	
	// - 
	DrawNotationElements(hdc);

	
	if ((mPosition.x == 0)  && getStaffState()->getRepeatInstrument()) {
		const GRInstrument* instr = getStaffState()->getRepeatInstrument();
		if (instr) instr->OnDraw (hdc, mPosition.y);
	}

	if (gBoundingBoxesMap & kStavesBB)
		DrawBoundingBox(hdc, kStaffBBColor);
//cerr << "GRStaff::OnDraw bb: " << mBoundingBox << endl;
}

// ----------------------------------------------------------------------------
/** \brief Draws the staff lines by scaling the staff glyph of the musical font.
*/
void GRStaff::DrawStaffUsingSymbolScale( VGDevice & hdc ) const
{
	// - Get the dims of of the staff symbol
	float staffCharWidth;
	float staffCharHeight;
	hdc.GetMusicFont()->GetExtent( kStaffSymbol, &staffCharWidth, &staffCharHeight, &hdc );

	const float kStaffLSPace = getStaffLSPACE(); // Space between two lines

	const NVPoint & staffPos = getPosition();
	float yOffset = 0;
	
	const float prevXScale = hdc.GetXScale();
	const float prevYScale = hdc.GetYScale();
	
	hdc.OffsetOrigin( staffPos.x, 0 );
	hdc.SetScale( prevXScale * mLength / staffCharWidth, prevYScale );
	
	if( mStaffState.numlines == 5 )
	{	
		yOffset = staffPos.y + 4 * kStaffLSPace;
		hdc.OffsetOrigin( 0, yOffset );
		hdc.DrawMusicSymbol( 0, 0, kStaffSymbol );
		hdc.OffsetOrigin( 0, -yOffset );
	}
	else
	{
		for (int i = 0; i < mStaffState.numlines; ++i)
		{
			yOffset = staffPos.y + i * kStaffLSPace;
			hdc.OffsetOrigin(0, yOffset);
			hdc.DrawMusicSymbol( 0, 0, kStaffLineSymbol);
			hdc.OffsetOrigin(0, - yOffset);
		}
	}
	
	// - Restore the orginal state of the matrix
	hdc.SetScale(1 / (prevXScale * mLength / staffCharWidth), 1 / prevYScale);
	hdc.OffsetOrigin( - staffPos.x, 0 );
}

// ----------------------------------------------------------------------------
/** \brief Draws the staff lines by repeating the staff glyph of the musical font.
*/
void GRStaff::DrawStaffUsingSymbolRepeat( VGDevice & hdc ) const
{
	// - Get the dims of of the staff symbol
	float staffCharWidth;
	float staffCharHeight;
	hdc.GetMusicFont()->GetExtent( kStaffSymbol, &staffCharWidth, &staffCharHeight, &hdc );

	const NVPoint & staffPos = getPosition();
	const float lineToX = (float)( staffPos.x + mLength ); // have to work on that !

	const float kStaffLSPace = getStaffLSPACE();

	for( float posx = staffPos.x; posx < lineToX; posx += staffCharWidth ) 
	{
		bool ende = 0;
		if( posx + staffCharWidth > lineToX ) {
			if (lineToX >= (staffCharWidth + 2))
				posx = lineToX - staffCharWidth - 2;
			else 
				posx = staffPos.x;
			ende = true;
		}

		float diffx = -1;
		if (posx == staffPos.x)
			diffx = 0;

		if (mStaffState.numlines == 5) {
			hdc.DrawMusicSymbol( posx + diffx, (staffPos.y + 4 * kStaffLSPace), kStaffSymbol );
		}
		else		// we have to draw the staff with a different sign ...
		{
			for ( int i=0; i < mStaffState.numlines; i++ ) {
				hdc.DrawMusicSymbol( posx, (staffPos.y + i * kStaffLSPace), kStaffLineSymbol );	// + diffx is not required ?
			}
		}
		if (ende)
			break;
	}
}

// ----------------------------------------------------------------------------
float GRStaff::currentLineThikness() const
{
    return mStaffState.curstaffrmt ? mStaffState.curstaffrmt->getLineThickness() : kLineThick;
}

// ----------------------------------------------------------------------------
/** \brief Draws the staff lines with vector lines.
*/
void GRStaff::DrawStaffUsingLines( VGDevice & hdc ) const
{
    float sizeRatio = getSizeRatio();
    if (sizeRatio < kMinNoteSize) // Too small, don't draw
        return;
    if (currentLineThikness() < 0.5)
        return;
    
    if (mStaffState.colRef)
        hdc.PushPenColor(VGColor(mStaffState.colRef));
    hdc.PushPenWidth(currentLineThikness() * getSizeRatio());
	
	const float lspace = getStaffLSPACE(); // Space between two lines
    std::map<float,float>::const_iterator it = fPositions.begin();
    while (it != fPositions.end()) {
        float x1 = it->first;
        float x2 = it->second;
		float yPos = getPosition().y;
        for (int i = 0; i < mStaffState.numlines; i++) {
            hdc.Line(x1, yPos, x2, yPos);
            yPos += lspace;
        }
        it++;
    }

    hdc.PopPenWidth();
    if (mStaffState.colRef)
        hdc.PopPenColor();
}

// ----------------------------------------------------------------------------
void GRStaff::DrawNotationElements( VGDevice & hdc ) const
{
	const float xOffset = 0; 					// mPosition.x	
	const float yOffset = mPosition.y;			// (JB) sign change

	hdc.OffsetOrigin( xOffset, yOffset );  		// Set the origin at beginning of staff
	GuidoPos pos = mCompElements.GetHeadPosition();
	while (pos) {
		GRNotationElement * e = mCompElements.GetNext(pos);
		e->OnDraw(hdc);
		
#ifdef _DEBUG
		//draw element's bounding box
		//e->DrawBoundingBox( hdc, VGColor(0,0,200)); // debug
#endif
	}
	hdc.OffsetOrigin( -xOffset, -yOffset ); // restore origin
}

//-------------------------------------------------
/** \brief Find the spatial x end position from the time position and the duration
*/
float GRStaff::getXEndPosition(TYPE_TIMEPOSITION pos, TYPE_DURATION dur) const
{
	float x = 0;
	TYPE_TIMEPOSITION end = pos + dur;
	const NEPointerList& elmts = getElements();
//cerr << "=> GRStaff::getXEndPosition " << end << " " << elmts.size() << endl;
	if (elmts.size())
    {
		NEPointerList * elmtsAtEndOfDuration = elmts.getElementsWithTimePosition(end);
		if(elmtsAtEndOfDuration)
        {
//cerr << "GRStaff::getXEndPosition " << elmtsAtEndOfDuration->size() << endl;
//GuidoPos pos = elmtsAtEndOfDuration->GetHeadPosition();
//while (pos) {
//	const GRNotationElement * elt = elmtsAtEndOfDuration->GetNext(pos);
//cerr << "GRStaff::getXEndPosition " << elt << endl;
//}
			const GRNotationElement * elmt = elmtsAtEndOfDuration->GetHead();
			if(elmt) {
				NVPoint position = elmt->getPosition();
				x = position.x;
				const GREvent * gevent = dynamic_cast<const GREvent *>(elmt);
                if (gevent)
					x -= LSPACE;
//                delete elmtsAtEndOfDuration;
//				return X;
			}
			delete elmtsAtEndOfDuration;
		}
	}
    return x;
}

void GRStaff::setOnOff(bool on, TYPE_TIMEPOSITION tp)
{
//	fOnOffList.push_back (make_pair(tp,on));
	fOnOffList[tp] = on;

//	if(fOnOffList.count(tp) > 0)
//	{
//		std::map<TYPE_TIMEPOSITION, bool>::iterator it;
//		it = fOnOffList.find(tp);
//		it->second = on;
//	}
//	else
//		fOnOffList.insert(std::pair<TYPE_TIMEPOSITION, bool>(tp, on));
}

void GRStaff::setOnOff(bool on)
{
	setOnOff(on, getRelativeTimePosition());
}

bool GRStaff::isStaffEndOn()
{
//  std::map<TYPE_TIMEPOSITION, bool>::reverse_iterator rit;
  auto rit = fOnOffList.rbegin();
  return (rit == fOnOffList.rend() ? false : rit->second);
}

bool GRStaff::isStaffBeginOn()
{
//  std::map<TYPE_TIMEPOSITION, bool>::iterator it;
  auto it = fOnOffList.begin();
  return (it == fOnOffList.end() ? false : it->second);
}

//--------------------------------------------------------------------
void GRStaff::generatePositions()
{
	//const float lspace = getStaffLSPACE(); // Space between two lines
	const NVPoint & staffPos = getPosition();
	
	const float xStart = staffPos.x;
	const float xEnd = xStart + mLength;

	// The staves at the begining of a new a system are somehow specials, we have to tell them explicitely 
	// not to draw if the next one is invisible.
	if(mStaffState.clefset && !mStaffState.meterset && !isNextOn)
	{
		setOnOff(false);
		GuidoPos pos = mCompElements.GetHeadPosition();
		while (pos)
		{
			GRNotationElement * e = mCompElements.GetNext(pos);
			e->setDrawOnOff(false);
		}
	
	}

	// this will be the end of the measure
	NVRect r = getBoundingBox();
	r += getPosition();
	float xEnd2 = r.right;

	// Now we have to see if there is one or more \staffOff- \staffOn- tag(s)
	auto it = fOnOffList.begin();
	TYPE_TIMEPOSITION t;
	TYPE_TIMEPOSITION t2 = it->first;
	bool draw = it->second;
	float x;
	float next = xStart;

	it++;

	while (it != fOnOffList.end()) {
		t = t2;
		t2 = it->first;
		x = next;
		TYPE_DURATION dur = t2 - t;
		next = getXEndPosition(t, dur);

		if (draw)
			fPositions.insert(std::pair<float,float>(x, next));

		draw = it->second;

		if (next == 0)
			draw = false;

		it++;
	}

	// when we arrive to the last element (possibly the same as the first one) we draw
	// from the last x posititon to the end of the measure
	if (it == fOnOffList.end() && draw)
		fPositions.insert(std::pair<float,float>(next, xEnd2));
		
	// the begining of the next measure has to be drawn by this staff...
	if (isNextOn && xEnd != xEnd2) {
		if (fPositions.count(xEnd2) > 0)
			fPositions.erase(xEnd2);

		fPositions.insert(std::pair<float,float>(xEnd2, xEnd));
	}
}
