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

/** \brief class GRStaff is a graphical object that will be created and managed by 
	StaffManager derived from GRCompositeNotationElement it includes NotationElement
	(sorted by time position).
	GRStaffs will be painted and "owned" from GRSystem.
*/

#include <typeinfo>
#include <vector>
#include <cstdlib>
#include <iostream> // for debug only
#include <fstream>// for debug only
using namespace std;

// - Guido Misc
#include "secureio.h"
#include "guido.h"
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

// - Guido GR
#include "GRStaff.h"
#include "GRBar.h"
#include "GRBarFormat.h"
#include "GRClef.h"
#include "GRChord.h"
#include "GRCompositeNote.h"
#include "GRCrescendo.h"
#include "GRDoubleBar.h"
#include "GRDummy.h"
#include "GRFermata.h"
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

#include "GRStaffFormat.h"
#include "GRGlobalStem.h"
#include "GRStem.h"
#include "GRSText.h"
#include "GRSingleNote.h"
#include "GRStaffManager.h"
#include "GRSystem.h"
#include "GRSystemSlice.h"
#include "GRText.h"
#include "GRVoice.h"

#include "kf_ivect.h"


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
	mnum = 0;			// start at bar 0... (important for upbeat, maybe to be changed!)

	curmeter = NULL;
	keyset = false; 			// key signatur set?
	numkeys = 0;

	curbarfrmt = NULL;
	curstaffrmt = NULL;
	staffLSPACE = LSPACE;
	numlines = 5;				// Standard

	curkey = NULL;

	// clef-Parameter
	clefset = CLEFAUTO;			// CLEFINTERN, CLEFEXPLICIT, CLEFAUTO, [ CLEFOFF ]
	clefname = "g2";			// Standard
	basepit = NOTE_G;
	basepitoffs = 0;
	instrNumKeys = 0;
	baseoct = 1;
	octava = 0;
	baseline = 3;
	curclef = NULL;

	distanceset = false;
	distance = 0;
	for (int i = 0; i < NUMNOTES; ++i)
	{
		instrKeyArray[i] = 0; 
		KeyArray[i] = 0; 
	}
}

// ----------------------------------------------------------------------------
GRStaffState::~GRStaffState()
{
}

// ----------------------------------------------------------------------------
void GRStaffState::reset2key()
{
	for (int i = 0; i < NUMNOTES; ++i) {
		fMeasureAccidentals.setPitchClassAccidental (i, KeyArray[i] - instrKeyArray[i]);
	}
}

// ----------------------------------------------------------------------------
GRStaffState & GRStaffState::operator=(const GRStaffState & tmp)
{
	meterset = tmp.meterset; // is meter-signture set?. TRUE, false
	mnum = tmp.mnum;
	curmeter = tmp.curmeter;

	// Noteparameter
	keyset = tmp.keyset; // key-signature set?
	numkeys = tmp.numkeys;
	curkey = tmp.curkey;
	for (int i=0; i < NUMNOTES; ++i)
	{
		instrKeyArray[i] = tmp.instrKeyArray[i];
		KeyArray[i] = tmp.KeyArray[i];
//		MeasureAccidentals[i] = tmp.MeasureAccidentals[i];
	}
	
	fMeasureAccidentals = tmp.fMeasureAccidentals;

	// clef-Parameter
	clefset = tmp.clefset; // CLEFINTERN, CLEFEXPLICIT, CLEFAUTO, [ CLEFOFF ]
	clefname  = tmp.clefname;
	basepit = tmp.basepit;
	basepitoffs = tmp.basepitoffs;
	instrNumKeys = tmp.instrNumKeys;
	baseoct = tmp.baseoct;
	octava = tmp.octava;
	baseline = tmp.baseline;
	curclef = tmp.curclef;

	curstaffrmt = tmp.curstaffrmt;
	staffLSPACE = LSPACE;
	if (curstaffrmt && curstaffrmt->getSize() && curstaffrmt->getSize()->TagIsSet())
	{
		staffLSPACE = curstaffrmt->getSize()->getValue() * 2;
	}
	numlines = tmp.numlines; // Standard ...

	distanceset = tmp.distanceset;
	distance = tmp.distance;

	curbarfrmt = tmp.curbarfrmt;
	return (*this);
}

// ===========================================================================
//		GRStaff
// ===========================================================================

GRStaff::GRStaff( GRSystemSlice * systemslice ) 
						: mGrSystem(NULL), mGrSystemSlice( systemslice ), fLastSystemBarChecked(-1,1)
{
	mLength = 0;
	mRelativeTimePositionOfGR = systemslice->getRelativeTimePosition();
	
//	mDurationOfGR = DURATION_0; // (JB) TEST: removed !
	setClefParameters(0);
	setMeterParameters(0);
	setKeyParameters(0);

	startglue = 0;
	endglue = 0;
	secglue = 0;

	lastrod = 0;
	firstrod = 0;
}

// ----------------------------------------------------------------------------
GRStaff::~GRStaff()
{
	// this routine has been done in this fashion, 
	// because that way, the associations are done correctly ....
	// but who deletes the Events? and the Glue ?
	GuidoPos pos = mCompElements.GetHeadPosition();
	while (pos)
	{
		GuidoPos curpos = pos;
		GRNotationElement * el = mCompElements.GetNext(pos);

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
	if(( dynamic_cast<GRPositionTag *>(notationElement)) == 0 )
	{		
		if (mCompElements.GetTail())
		{ 
			const TYPE_TIMEPOSITION tp1 (notationElement->getRelativeTimePosition());

			// no notation element may be added to the staff, that
			// is EARLIER than the staff itself.
#ifdef DEBUG
			assert(tp1>=getRelativeTimePosition());
#endif
			const TYPE_TIMEPOSITION tp2 (mCompElements.GetTail()->getRelativeTimePosition());
			if( tp1 < tp2 )
			{
				// DF - 24/08/2009 - assert commented to avoid spurious exit
				// due to dynamic score coding or multiple fermata in a chord with variable length notes
				// assert(false);
				mCompElements.AddAtCorrectTimePosition(notationElement);
			}
			else
				mCompElements.AddTail(notationElement);
		}
		else
			mCompElements.AddTail(notationElement);
	}
	else
		mCompElements.AddTail(notationElement);

	// this should sort the elements ... it is no longer needed; sorting is done
	// on the abstract representation, and allelements are added/sorted there.
	//if (sortElements)
	//	mCompElements.sort( & GRNotationElement::comp);

	// This is old, horizontal synchronisation is done by StaffManager and not longer
	// by the System:
	
	// This is done, so that all Elements that are added are subject to vertical-slice-spacing 
	// right after the current time slice has been worked upon.
	// OLD: mGrSystem->AddToSpace(notationElement);

	// count the notes, evaluates the accidentals.
	GRNote * mynote = dynamic_cast<GRNote *>(notationElement);
	setNoteParameters(mynote);
}

// ----------------------------------------------------------------------------
GRClef * GRStaff::AddClef(ARClef * arclef)
{
	// To do: look, whether the clef really is a clef-
	// change. If not, do nothing!!!!
	GRClef * grclef = new GRClef(arclef, this);
	TYPE_TIMEPOSITION tmp (mRelativeTimePositionOfGR);
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
		ARClef * aclef = /*dynamic*/static_cast<ARClef *>(grclef->getAbstractRepresentation());
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
		mStaffState.curkey = /*dynamic*/static_cast<ARKey *>( inKey->getAbstractRepresentation());
		mStaffState.numkeys = inKey->getKeyArray(mStaffState.KeyArray);
	 }
	 mStaffState.reset2key();
}

// ----------------------------------------------------------------------------
void GRStaff::setNoteParameters(GRNote * inNote)
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
	ARNote * arnote = inNote->getARNote();
	const int tmppitch = arnote->getPitch() - NOTE_C;
	const int acc = arnote->getAccidentals() - mStaffState.instrKeyArray[tmppitch];
//	mStaffState.MeasureAccidentals[tmppitch] = acc + arnote->getDetune();
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
			while (bottombound > (mStaffState.numlines)*getStaffLSPACE() );
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
	oct -= mStaffState.octava;	//  depends on current clef.

	const float myHalfSpace = getStaffLSPACE() * 0.5f;
	float calc = 0;
	if (pit >= NOTE_C && pit <= NOTE_H)
	{
		calc = (float)((mStaffState.basepit - pit ) * myHalfSpace + mStaffState.baseline * getStaffLSPACE() -
			((int)oct - mStaffState.baseoct) * (7 * myHalfSpace));
	}
	else if (pit>= NOTE_CIS && pit <= NOTE_DIS)
	{
		calc = (float)((mStaffState.basepit - (pit - 7) )* myHalfSpace + mStaffState.baseline * getStaffLSPACE()-
			((int)oct - mStaffState.baseoct) * (7 * myHalfSpace));
	}
	else if (pit>= NOTE_FIS && pit <= NOTE_AIS)
	{
		calc = (float)((mStaffState.basepit - (pit - 6) )* myHalfSpace + mStaffState.baseline * getStaffLSPACE() -
			((int)oct - mStaffState.baseoct) * (7 * myHalfSpace));
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
		if (bar) {
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
	const int shiftparm = mStaffState.instrKeyArray[pitchclass];
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
void GRStaff::newMeasure(const TYPE_TIMEPOSITION & tp)
{
staff_debug("newMeasure");
	if (mStaffState.meterset)
	{
		//mStaffState.nextmeasuretime = tp + mStaffState.measurelength;
	}

	// reset of accidentals
	// quick-hack-implementation -> encode which accidentals are already set
	// and which must be deleted in the next measure etc.
	mStaffState.reset2key ();
	++ mStaffState.mnum;
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
/** \brief Adds a Meter at the current position.
*/
GRMeter * GRStaff::AddMeter(ARMeter * armeter)
{
	// First check, whether this really means
	// a Change in Meter? If not, do not display anything!
	GRMeter * nmeter = new GRMeter(armeter, this);
	TYPE_TIMEPOSITION tmp ( mRelativeTimePositionOfGR );
	GRNotationElement * el = mCompElements.GetTail();
	if (el) 
		tmp = el->getRelativeEndTimePosition();

	nmeter->setRelativeTimePosition(tmp);
	setMeterParameters(nmeter);
	addNotationElement(nmeter);
	return nmeter;
}

// ----------------------------------------------------------------------------
GRNotationElement * GRStaff::AddOctava(AROctava * aroct)
{
	const char * s = "";
	if (aroct->getOctava() == 1)		s = "8";
	else if (aroct->getOctava() == 0)	s = "";
	else if (aroct->getOctava() == -1)	s = "8ba";
	
	GRSText * grstxt = new GRSText(this, s, -1);
	grstxt->setRelativeTimePosition( aroct->getRelativeTimePosition());
	AddTag(grstxt);

	mStaffState.octava  = aroct->getOctava();	
	return grstxt;
}

// ----------------------------------------------------------------------------
void GRStaff::AddTag(GRNotationElement * grtag)
{
	addNotationElement(grtag);
}

// ----------------------------------------------------------------------------
/** \brief This creates a repeatBegin.
*/
GRRepeatBegin * GRStaff::AddRepeatBegin(ARRepeatBegin * arrb)
{
    assert (arrb);
//	GRRepeatBegin * tmp = new GRRepeatBegin(arrb, this, arrb->getRelativeTimePosition());
	GRRepeatBegin * tmp = new GRRepeatBegin(arrb);
	addNotationElement(tmp);
	tmp->setGRStaff(this);
	tmp->updateBoundingBox();
	return tmp;
}

// ----------------------------------------------------------------------------
/** \brief This creates a repeatEnd 
*/
GRRepeatEnd * GRStaff::AddRepeatEnd( ARRepeatEnd * arre )
{
	if (arre->getNumRepeat() == 0 || !arre->getRange())
	{
        assert (arre);
		GRRepeatEnd * tmp = new GRRepeatEnd(arre, this, arre->getRelativeTimePosition());
		addNotationElement(tmp);
		return tmp;
	}
	return NULL;
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
GRInstrument * GRStaff::AddInstrument(ARInstrument * arinstr)
{
	GRInstrument * tmp = new GRInstrument(arinstr);

	addNotationElement(tmp);
	bool downwards = true;

	// now we need to test, whether the instrument is transposed (e.g. "clarinet in A")
	const TagParameterString * ts = arinstr->getTransp();
	if ( ts && ts->TagIsSet())
	{
		// then we have a transposition ....
		// this is always relative to "C-Major"
		// See MGG, Volume 9, column 1651

		// we can interpret this either as a number or as a string
		// "A" means A-Major
		// "B&" means B-flat Major etc.

		// we have to calculate distance from C ..
		// so we just interpret this as a notename and change the case 
		const NVstring & mystr = ts->getValue();

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
GRBar * GRStaff::AddBar(ARBar * abar, const TYPE_TIMEPOSITION & von)
{
staff_debug("AddBar");
	newMeasure(von); // erhoeht u.a. mnum!
	if (abar->getBarNumber() != -1)
		mStaffState.mnum = abar->getBarNumber();

	GRBar * ntakt = new GRBar( abar, this, von);
	// depending on current bar Format, we have to tell the staffmanager (or the system) 
	if (mStaffState.curbarfrmt && mStaffState.curbarfrmt->getStyle()
		&& mStaffState.curbarfrmt->getStyle()->TagIsSet())
	{
		const NVstring & style = mStaffState.curbarfrmt->getStyle()->getValue();
		// NVstring str_system("system");
		if (style == "system" ) // ((const NVstring &) str_system))
		{
			mGrSystemSlice->addBar(ntakt, GRSystem::SYSTEM, this);
		}
	}

	// change of  Measuretime ...
	mStaffState.reset2key ();
	addNotationElement(ntakt); 
	return ntakt;
}

// ----------------------------------------------------------------------------
GRKey * GRStaff::AddKey(ARKey * arkey)
{
	ARNaturalKey * natkey = dynamic_cast<ARNaturalKey *>(arkey);

	// it may happen, that the natkey is not needed because a newsystem has occured!
	if (natkey && mStaffState.curkey)
	{
		// else we have to naturalize ...
		// First, we copy the current key
		ARKey * arnatkey = new ARKey(*mStaffState.curkey);
		GRKey * natkey = new GRKey(this, arnatkey, 1, 1);

		// the new natkey MUST get a Spring / and a Rod
		// otherwise, we have a Problem!
		TYPE_TIMEPOSITION tmp (mRelativeTimePositionOfGR);
		GRNotationElement * el = mCompElements.GetTail();
		if (el) 
			tmp = el->getRelativeEndTimePosition();
		natkey->setRelativeTimePosition(tmp);	
		
		addNotationElement(natkey);
		return natkey;
	}

	if (arkey->getIsAuto())
	{
		// then we can change the keynumber if we want to ....
		if (mStaffState.instrNumKeys != arkey->getKeyNumber() )
			arkey->setKeyNumber(mStaffState.instrNumKeys);
	}

	GRKey * key = new GRKey(this, arkey, 0, 0);
	TYPE_TIMEPOSITION tmp (mRelativeTimePositionOfGR);
	GRNotationElement * el = mCompElements.GetTail();
	if (el) 
		tmp = el->getRelativeEndTimePosition();

	key->setRelativeTimePosition(tmp);
	setKeyParameters(key);
	addNotationElement(key);
	return key;
}

// ----------------------------------------------------------------------------
GRText * GRStaff::AddText(ARText * atext)
{
	GRText * gtext = new GRText(this, atext);
	addNotationElement(gtext);

	return gtext;
}

// ----------------------------------------------------------------------------
GRIntens * GRStaff::AddIntens(ARIntens * aintens)
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
		startglue->setRelativeTimePosition(mRelativeTimePositionOfGR);
		// always at the beginning!
		mCompElements.AddHead(startglue);
	}

	if (secglue == 0)
	{
		secglue = new GRGlue(this, -1);
		secglue->setRelativeTimePosition( mRelativeTimePositionOfGR);
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
GuidoPos GRStaff::EndStaff(const TYPE_TIMEPOSITION & tp, 
					   GRStaffManager * staffmgr, 
					   GuidoPos endpos, 
					   int lastline)
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
GRDoubleBar * GRStaff::AddDoubleBar(ARDoubleBar * ardbar, const TYPE_TIMEPOSITION & von)
{
staff_debug("AddDoubleBar");
	newMeasure(von); // erhoeht u.a. mnum!
	if (ardbar->getBarNumber() != -1)
		mStaffState.mnum = ardbar->getBarNumber();

	GRDoubleBar * ntakt = new GRDoubleBar( ardbar, this, von);
	// depending on current bar Format, we have to tell the staffmanager (or the system) 
	if (mStaffState.curbarfrmt && mStaffState.curbarfrmt->getStyle() && mStaffState.curbarfrmt->getStyle()->TagIsSet())
	{
		const NVstring & style = mStaffState.curbarfrmt->getStyle()->getValue();
		if (style == "system" )
		{
			mGrSystemSlice->addDoubleBar(ntakt, GRSystem::SYSTEM, this);
		}
	}
	addNotationElement(ntakt); // change of Measuretime ...
	return ntakt;
}

// ----------------------------------------------------------------------------
GRFinishBar * GRStaff::AddFinishBar(ARFinishBar * arfbar, const TYPE_TIMEPOSITION & von)
{
staff_debug("AddFinishBar");
	newMeasure(von); // erhoeht u.a. mnum!
	if (arfbar->getBarNumber() != -1)
		mStaffState.mnum = arfbar->getBarNumber();

	GRFinishBar * ntakt = new GRFinishBar( arfbar, this, von);

	// depending on current bar Format, we have to tell the staffmanager (or the system) 
	if (mStaffState.curbarfrmt && mStaffState.curbarfrmt->getStyle() && mStaffState.curbarfrmt->getStyle()->TagIsSet())
	{
		const NVstring & style = mStaffState.curbarfrmt->getStyle()->getValue();
		if (style == "system" )
		{
			mGrSystemSlice->addFinishBar(ntakt, GRSystem::SYSTEM, this);
		}
	}
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
	
	// this is not possible, because staffmgr chokes on TWO Glues after another ...
	//startglue = new GRGlue(this, 1);
	//startglue->setRelativeTimePosition(relativeTimePositionOfGR);
	//addNotationElement(startglue);
	//staffmgr->AddGRSyncElement(startglue, this, staffnum);

	// we have to look, what kind of state-settings are set.
	if (state.curbarfrmt != NULL)
	{
		// why do we need a copy?
		setBarFormat(state.curbarfrmt);
	}

	if (state.curstaffrmt != NULL)
	{
		setStaffFormat(state.curstaffrmt);
		// we have a stafffrmt ...
	}

	if (state.curclef != NULL)
	{

		// now I have to deal with basepitoffs (for transposed instruments)

		ARClef * arclef = new ARClef(*state.curclef);
		arclef->setRelativeTimePosition( mRelativeTimePositionOfGR );
		// owns abstract ....!
		GRClef * grclef = new GRClef(arclef, this, 1);

		// grclef->setRelativeTimePosition(relativeTimePositionOfGR);
		assert(grclef->getRelativeTimePosition() == mRelativeTimePositionOfGR );

		setClefParameters(grclef, GRStaffState::CLEFEXPLICIT);
		addNotationElement(grclef);

		// this handles the spring-synchronization ...
		staffmgr->AddGRSyncElement(grclef, this, staffnum, NULL);	
	}
	if (state.curkey != NULL)
	{
		// I have to think about keynumber (transposed instruments)

		ARKey * arkey = new ARKey(*state.curkey);
		arkey->setRelativeTimePosition(mRelativeTimePositionOfGR);

		GRKey * grkey = new GRKey(this, arkey, 0, 1);

		// grkey->setRelativeTimePosition(relativeTimePositionOfGR);
		assert(grkey->getRelativeTimePosition() == mRelativeTimePositionOfGR );
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
	secglue->setRelativeTimePosition( mRelativeTimePositionOfGR );
	addNotationElement(secglue);
	staffmgr->AddGRSyncElement(secglue, this, staffnum, NULL);
}

// ----------------------------------------------------------------------------
/** \brief createNewRods is called to create the rods for a staff.
	The lastrod is saved internally, so that only rods are created for new neighbors.
*/
void GRStaff::createNewRods(GRStaffManager * staffmgr, int & startspr, int & endspr)
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
						leftspace, prevsprid, mylist.GetHead()->getSpringID());
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
			GRRod * rod = new GRRod(leftspace + rightspace, prevsprid, mysprid);
			
			// no space, add at head
			staffmgr->addRod(rod, 0, 1);
			lastrod = rod;
		}
		// this is a future rod ....
		if (newrightspace > 0)
		{
			GRRod * rod = new GRRod(newrightspace, mysprid, mysprid + 1);
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
void GRStaff::setStaffFormat(ARStaffFormat * staffrmt)
{
	mStaffState.curstaffrmt = staffrmt;
	if (mStaffState.curstaffrmt)
	{
		if (mStaffState.curstaffrmt->getSize() && mStaffState.curstaffrmt->getSize()->TagIsSet())
		{
			mStaffState.staffLSPACE = mStaffState.curstaffrmt->getSize()->getValue() * 2;
		}
		if (mStaffState.curstaffrmt->getStyle() &&
			mStaffState.curstaffrmt->getStyle()->TagIsSet())
		{
			// other then standard? -> rather n-line ....?
			const NVstring & mystr = mStaffState.curstaffrmt->getStyle()->getValue();
			if (mystr.substr(1, 5) == "-line")
			{
				const int tmp = atoi(mystr.substr(0, 1).c_str());
				if (tmp >= 0 && tmp <= 7)
					mStaffState.numlines = tmp;
			}
		}
	}

	// I have to deal with Size - parameter!
}

// ----------------------------------------------------------------------------
void GRStaff::setBarFormat(ARBarFormat * barfrmt)
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
const GRStaffState * GRStaff::getStaffState()	{ return &mStaffState; }

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
	updateBoundingBox();
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
			GRPositionTag * ptag = dynamic_cast<GRPositionTag *>(e);
           if (ptag) continue;

            tmp = e->getBoundingBox();
            p = getPosition();
            p.y = 0;
            tmp += e->getPosition() - p;
            r.Merge (tmp);
/*
	useless: stem bounding boxes are included in events bounding boxes - DF sept 18 2009
	in addition, stems bb may be incorrect
            
            GREvent * ev = dynamic_cast<GREvent *>(e);
            if (!ev) continue;

            GRGlobalStem * gstem = ev->getGlobalStem();
            if (!gstem) continue;
            GRStem * stem = gstem->getGRStem();
            if (stem) {
                tmp = stem->getBoundingBox();
				p = getPosition();
                p.y = 0;
                tmp += e->getPosition() - p;
                r.Merge (tmp);
           }
*/
        }
    }
	mBoundingBox.Merge (r);
	mMapping.top = mMapping.left = 0;
	mMapping.right = mLength;
	mMapping.bottom = getDredgeSize();
	mMapping += mPosition + getOffset();
}

// ----------------------------------------------------------------------------
/** \brief gives the previous staff
*/
GRStaff * GRStaff::getPreviousStaff() const
{
    GRSystem * system = getGRSystem();
    GRSystemSlice * curslice = getGRSystemSlice();
    if (!system || !curslice) return 0;

    SSliceList * sl = system->getSlices();          // get the list of system slices
    if (!sl) return 0;
    
    GuidoPos pos = sl->GetElementPos(curslice);                 // looks for the current slice
    GRSystemSlice * previousSlice = pos ? sl->GetPrev(pos) : 0;  // get the previous twice
    previousSlice = pos ? sl->GetPrev(pos) : 0;                  // this is to skip the current slice
    if (!previousSlice) return 0;                               // fails to find the previous

	int	num = curslice->getStaffNumber(this);

    StaffVector * sv = previousSlice->getStaves();  // get the staves list
    if (!sv) return 0;

    GRStaff * pstaff = sv->Get(num);                // get the staff carrying the same number
    return pstaff;
}

// ----------------------------------------------------------------------------
/** \brief Retrieves the mapping
*/
void GRStaff::GetMap( GuidoeElementSelector sel, MapCollector& f, MapInfos& infos ) const
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
	DrawStaffUsingLines( hdc );
#endif

	// - 
	DrawNotationElements( hdc );
	if (gBoundingBoxesMap & kStavesBB) {
		DrawBoundingBox( hdc, kStaffBBColor);
	}
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
		for( int i = 0; i < mStaffState.numlines; ++i )
		{
			yOffset = staffPos.y + i * kStaffLSPace;
			hdc.OffsetOrigin( 0, yOffset );
			hdc.DrawMusicSymbol( 0, 0, kStaffLineSymbol );
			hdc.OffsetOrigin( 0, -yOffset );
		}
	}
	
	// - Restore the orginal state of the matrix
	hdc.SetScale( prevXScale, prevYScale );
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
/** \brief Draws the staff lines with vector lines.
*/
void GRStaff::DrawStaffUsingLines( VGDevice & hdc ) const
{
	const float lspace = getStaffLSPACE(); // Space between two lines
	const NVPoint & staffPos = getPosition();
	
	const float xStart = staffPos.x;
	const float xEnd = xStart + mLength; //  cause gaps in stafflines: - (0.5f * kLineThick);
	float yPos = staffPos.y;
	
	/* - Debug ->
	hdc.PushPen( GColor( 255, 0, 0, 128 ), 10 );
	hdc.Line( xStart - 100, yPos, xStart + 100, yPos );
	hdc.Line( xStart, yPos - 100, xStart, yPos + 100 );
	hdc.PopPen();

	*/
//	hdc.PushPen( VGColor( 0, 0, 0 ), kLineThick );// TODO: use correct color
	hdc.PushPenWidth( kLineThick );

	for( int i = 0; i < mStaffState.numlines; ++i )
	{
		hdc.Line( xStart, yPos, xEnd, yPos );
		yPos += lspace;
	}

	hdc.PopPenWidth();
//	hdc.PopPen();
}

// ----------------------------------------------------------------------------
void GRStaff::DrawNotationElements( VGDevice & hdc ) const
{

	const float xOffset = 0; 					// mPosition.x	
	const float yOffset = mPosition.y;			// (JB) sign change

	hdc.OffsetOrigin( xOffset, yOffset );  		// Set the origin at beginning of staff
	GuidoPos pos = mCompElements.GetHeadPosition();

	while (pos)
	{
		GRNotationElement * e = mCompElements.GetNext(pos);
		e->OnDraw(hdc);
		
#ifdef _DEBUG
		//draw element's bounding box
		e->DrawBoundingBox( hdc, VGColor(0,0,200)); // debug
#endif
	}
	hdc.OffsetOrigin( -xOffset, -yOffset ); // restore origin
}

// ----------------------------------------------------------------------------
void GRStaff::print() const
{
	GRNotationElement * e;
	fprintf(stderr, "GRStaff::print(): %.2f-%.2f ", 
		(float) mRelativeTimePositionOfGR, (float) getRelativeEndTimePosition());
	GuidoPos pos = mCompElements.GetHeadPosition();
	while(pos)
	{
		e = mCompElements.GetNext(pos);
		e->print();
	}
	fprintf(stderr, "\n");
}

// ----------------------------------------------------------------------------
void GRStaff::GGSOutput() const
{
	char buffer[200];
	assert(endglue);
	snprintf(buffer, 200, "\\draw_staff<%ld,%d,%d,%d,%d>\n", 
		getID(), 
		mStaffState.numlines, 
		(int)(mPosition.x + ggsoffsetx), 
		(int)(mPosition.y + 4 * LSPACE + ggsoffsety),  
		(int)(endglue->getPosition().x ));
		// position.x + mBoundingBox.right);

	AddGGSOutput(buffer);

	ggsoffsetx += (long)mPosition.x;
	ggsoffsety += (long)mPosition.y;

	GRNotationElement * e;
	GuidoPos pos = mCompElements.GetHeadPosition();
	while (pos)
	{
		e = mCompElements.GetNext(pos);
		e->GGSOutput();
	}

	ggsoffsetx -= (long)mPosition.x;
	ggsoffsety -= (long)mPosition.y;
}

