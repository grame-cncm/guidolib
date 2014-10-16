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
#include <cassert>

#include "ARNote.h"
#include "ARTrill.h"
#include "ARCluster.h"
#include "ARDefine.h"
#include "TimeUnwrap.h"
#include "nvstring.h"

int gd_noteName2pc(const char * name);
const char * gd_pc2noteName(int fPitch);


ARNote::ARNote(const TYPE_DURATION & durationOfNote)
	:	ARMusicalEvent(durationOfNote), fName("empty"), fPitch(UNKNOWN), fOctave(MIN_REGISTER),
    fAccidentals(0), fDetune(0), fIntensity(MIN_INTENSITY), fOrnament(NULL), fCluster(NULL), fIsLonelyInCluster(false),
    fClusterHaveToBeDrawn(false), fSubElementsHaveToBeDrawn(true), fTremolo(0), fStartPosition(-1,1)
{
}

ARNote::ARNote(const TYPE_TIMEPOSITION & relativeTimePositionOfNote, const TYPE_DURATION & durationOfNote)
	:	ARMusicalEvent( relativeTimePositionOfNote, durationOfNote), fName("noname"), fPitch(UNKNOWN),
		fOctave(MIN_REGISTER), fAccidentals(0), fDetune(0), fIntensity(MIN_INTENSITY), fOrnament(NULL), fCluster(NULL),
        fIsLonelyInCluster(false), fClusterHaveToBeDrawn(false),fTremolo(0), fSubElementsHaveToBeDrawn(true), fStartPosition(-1,1)
{
}

ARNote::ARNote( const std::string & inName, int theAccidentals, int theRegister, int theNumerator, 
				int theDenominator, int theIntensity )
	:	ARMusicalEvent(theNumerator, theDenominator), fName( inName ), fPitch ( UNKNOWN ),
		fOctave( theRegister ),	fAccidentals( theAccidentals ), fDetune(0), fIntensity( theIntensity ),
		fOrnament(NULL), fCluster(NULL), fIsLonelyInCluster(false), fClusterHaveToBeDrawn(false), fSubElementsHaveToBeDrawn(true), fTremolo(0), fStartPosition(-1,1)
{
	assert(fAccidentals>=MIN_ACCIDENTALS);
	assert(fAccidentals<=MAX_ACCIDENTALS);
	fName = NVstring::to_lower(fName.c_str());
	fPitch = gd_noteName2pc(fName.c_str());
}

ARNote::ARNote(const ARNote & arnote) 
	:	ARMusicalEvent( (const ARMusicalEvent &) arnote),
		fName(arnote.fName), fOrnament(NULL),  fCluster(NULL), fIsLonelyInCluster(false),
        fClusterHaveToBeDrawn(false), fSubElementsHaveToBeDrawn(true), fTremolo(0), fStartPosition(-1,1)
{
	fPitch = arnote.fPitch;
	fOctave = arnote.fOctave;
	fAccidentals = arnote.fAccidentals;
	fDetune = arnote.fDetune;
	fIntensity = arnote.fIntensity;
}

ARNote::~ARNote()
{
	delete fOrnament;		// makes the system crash - to be checked
}

ARMusicalObject * ARNote::Copy() const
{
	return new ARNote(*this);
}

static int round (float value) {
	int integer = int(value);
	float remain = value - integer;
	return (remain < 0.5) ? integer : integer + 1;
}

int	ARNote::detune2Quarters(float detune)
{
	return round(detune*2);	// detune in rounded quarter tones
}

void ARNote::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kNote);
}

int ARNote::midiPitch() const
{
	int oct = 12 * (fOctave+4);
	if (oct < 0) return 0;

	int pitch = -1;
	switch (fPitch) {
		case NOTE_C:
		case NOTE_D:
		case NOTE_E:	pitch = (fPitch - NOTE_C) * 2;
			break;
		case NOTE_F:
		case NOTE_G:
		case NOTE_A:
		case NOTE_H:	pitch = (fPitch - NOTE_C) * 2 - 1;
			break;
		
		case NOTE_CIS: 
		case NOTE_DIS:  pitch = (fPitch - NOTE_CIS) * 2 + 1;
			break;

		case NOTE_FIS:
		case NOTE_GIS:
		case NOTE_AIS:  pitch = (fPitch - NOTE_CIS) * 2 + 3;
			break;
		default:
			return pitch;
	}
	return oct + pitch + fAccidentals;
}

//## Other Operations (implementation)
void ARNote::print() const
{
	std::cout << "ARNote: name: '" << getName() << "' fPitch: " << getPitch()
		<< " oct: " << getOctave() << " accidental: " << getAccidentals() << " detune: " << getDetune()
        << " duration: " << getDuration().getNumerator() << "/" << getDuration().getDenominator() << '\n';
	ARMusicalEvent::print();
}

std::ostream & ARNote::operator<<(std::ostream &os) const
{
	os << fName;
	if(fAccidentals>0) for(int i=0;i<fAccidentals;i++) os << "#";
	else if(fAccidentals<0) for(int i=0;i>fAccidentals;i--) os << "&";

	// here we have to "uncalculate" the dots ...
	// there is a distinction between notes that
	// were put in with dots and those that weren't.

	TYPE_DURATION tmpdur (duration);
	if (mPoints==1)
	{
		tmpdur.setNumerator(2);
		tmpdur.normalize();
	}
	else if (mPoints == 2)
	{
		tmpdur.setNumerator(4);
		tmpdur.normalize();
	}
	else if (mPoints == 3)
	{
		tmpdur.setNumerator(8);
		tmpdur.normalize();
	}
	else if (mPoints != 0)
	{
		// there can only be 0,1, or 2 dots
		assert(false);
	}

	os << fOctave << "*" << 
		tmpdur.getNumerator() << "/" <<
		tmpdur.getDenominator();
	for (int i=0;i< (int) mPoints;i++)
	{
		os << ".";
	}
	os << " ";
	return os;
}


void ARNote::addFlat()
{
	--fAccidentals;
	assert(fAccidentals>=MIN_ACCIDENTALS);
}

void ARNote::addSharp()
{
	++fAccidentals;
	assert(fAccidentals<=MAX_ACCIDENTALS);
}

void ARNote::setRegister( int newRegister)
{
//	assert(newRegister>=MIN_REGISTER);
//	assert(newRegister<=MAX_REGISTER);
	fOctave=newRegister;
}

const ARNoteName & ARNote::getName() const
{
	return fName;
}

TYPE_REGISTER ARNote::getOctave() const
{
	return fOctave;
}

TYPE_PITCH ARNote::getPitch() const
{
	return fPitch;
}

void ARNote::offsetpitch(int steps)
{
  // this just tries to offset the fPitch ...
	if (fPitch >= NOTE_C && fPitch <= NOTE_H)
	{
		int tmppitch = fPitch - NOTE_C;
		tmppitch += steps;
		int regchange = 0;
		while (tmppitch > 6)
		{
			tmppitch -= 7;
			++regchange;
		}
		while (tmppitch < 0)
		{
			tmppitch += 7;
			--regchange;
		}

		fPitch = tmppitch + NOTE_C;
		if (regchange)
			fOctave += regchange;
		fName = gd_pc2noteName(fPitch);
	}
	else
	{
	// we do not care right now ...
	}
}

void ARNote::setPitch(TYPE_PITCH newpitch)
{
	fPitch = newpitch;
	fName = gd_pc2noteName(fPitch);
}

void ARNote::setAccidentals(int theAccidentals)
{
	assert(fAccidentals>=MIN_ACCIDENTALS);
	assert(fAccidentals<=MAX_ACCIDENTALS);
	fAccidentals=theAccidentals;
}

void ARNote::setOrnament(ARTrill * newOrnament)
{
	delete fOrnament;
//	if (!fOrnament) 
	fOrnament = new ARTrill(-1, newOrnament);
}

ARCluster *ARNote::setCluster(ARCluster *inCluster,
                              bool inClusterHaveToBeDrawn,
                              bool inHaveToBeCreated)
{
    if (!fClusterHaveToBeDrawn && inClusterHaveToBeDrawn)
        fClusterHaveToBeDrawn = true;

    if (inHaveToBeCreated)
        fCluster = new ARCluster(inCluster);
    else
        fCluster = inCluster;

    return fCluster;
}

void ARNote::setClusterPitchAndOctave()
{
    fCluster->setNotePitchAndOctave(fPitch, fOctave);
}

bool ARNote::CanBeMerged(const ARMusicalEvent * ev2)
{
	if (ARMusicalEvent::CanBeMerged(ev2))
	{
		// type is OK (checked in ARMusicalEvent)
		const ARNote *arn = dynamic_cast<const ARNote *>(ev2);
		if (!arn) return false;
		
		if (arn->fPitch == this->fPitch 
			&& arn->fOctave == this->fOctave)
			return true;
	}
	return false;
}

void ARNote::setDuration(const TYPE_DURATION & newdur)
{
	ARMusicalEvent::setDuration(newdur);
	if (newdur == DURATION_0)
		mPoints = 0;
}

const TYPE_TIMEPOSITION& ARNote::getStartTimePosition() const 
{
	return (fStartPosition.getNumerator() >= 0) ? fStartPosition : getRelativeTimePosition();
}

// this compares the name, fPitch, fOctave and fAccidentals
// returns 1 if it matches ...
int ARNote::CompareNameOctavePitch(const ARNote & nt)
{
	if (fName == nt.fName
		&& fPitch == nt.fPitch
		&& fOctave == nt.fOctave 
		&& fAccidentals == nt.fAccidentals)
		return 1;

	return 0;
}
