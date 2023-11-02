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

#include <iostream>
#include <sstream>
#include <cassert>

#include "ARNote.h"
#include "ARTrill.h"
#include "ARCluster.h"
#include "ARDefine.h"
#include "TimeUnwrap.h"
#include "nvstring.h"

using namespace std;

int gd_noteName2pc(const char * name);
const char * gd_pc2noteName(int fPitch);


ARNote::ARNote(const TYPE_DURATION & durationOfNote)
	:	ARMusicalEvent(durationOfNote), fName("empty"), fPitch(UNKNOWN), fOctave(MIN_REGISTER),
    fAccidentals(0), fIntensity(MIN_INTENSITY), fOrnament(NULL), fCluster(NULL), fOwnCluster(false), fIsLonelyInCluster(false),
    fClusterHaveToBeDrawn(false), fSubElementsHaveToBeDrawn(true), fAuto(false), fTremolo(0), fNoteAppearance(""), fOctava(0)
{
}

ARNote::ARNote(const TYPE_TIMEPOSITION & relativeTimePositionOfNote, const TYPE_DURATION & durationOfNote)
	:	ARMusicalEvent( relativeTimePositionOfNote, durationOfNote), fName("noname"), fPitch(UNKNOWN),
		fOctave(MIN_REGISTER), fAccidentals(0), fIntensity(MIN_INTENSITY), fOrnament(NULL), fCluster(NULL),
        fOwnCluster(false), fIsLonelyInCluster(false), fClusterHaveToBeDrawn(false), fSubElementsHaveToBeDrawn(true), fAuto(false), fTremolo(0),
        fNoteAppearance(""), fOctava(0)
{
}

ARNote::ARNote( const std::string & name, int accidentals, int octave, int numerator, int denominator, int intensity )
	:	ARMusicalEvent(numerator, denominator), fName( name ), fPitch ( UNKNOWN ),
		fOctave( octave ),	fAccidentals( accidentals ), fIntensity( intensity ),
		fOrnament(NULL), fCluster(NULL), fOwnCluster(false), fIsLonelyInCluster(false), fClusterHaveToBeDrawn(false), 
		fSubElementsHaveToBeDrawn(true), fAuto(false), fTremolo(0), fNoteAppearance(""), fOctava(0)
{
	assert(fAccidentals>=MIN_ACCIDENTALS);
	assert(fAccidentals<=MAX_ACCIDENTALS);
	fName = NVstring::to_lower(fName.c_str());
	fPitch = gd_noteName2pc(fName.c_str());
}

ARNote::ARNote(const ARNote & arnote, bool istied)
	:	ARMusicalEvent( (const ARMusicalEvent &) arnote),
		fName(arnote.fName), fOrnament(NULL),  fCluster(NULL), fOwnCluster(false), fIsLonelyInCluster(false),
        fClusterHaveToBeDrawn(false), fSubElementsHaveToBeDrawn(true), fAuto(true), fTremolo(0), fOctava(0)
{
	(*this) = &arnote;
}

ARNote * ARNote::Clone(bool istied) const	{ return new ARNote (*this, istied); }

ARNote::~ARNote()
{
	if (fTrillOwner)	delete fOrnament;
	if (fOwnCluster)	delete fCluster;
}

void ARNote::operator= (const ARNote* note)
{
	fPitch = note->fPitch;
	fOctave = note->fOctave;
	fAccidentals = note->fAccidentals;
	fAlter = note->getAlter();
	fIntensity = note->fIntensity;
    fVoiceNum = note->getVoiceNum(); // Added to fix a bug during chord copy (in doAutoBarlines)
	fOctava = note->getOctava();
	const ARTrill* trill = note->getOrnament();
	if (trill) {
		ARTrill* copy = new ARTrill(-1, trill);
		copy->setContinue();
		if (isAuto()) copy->setIsAuto(true);
		setOrnament(copy);
	}
}

ARMusicalObject * ARNote::Copy() const
{
	return new ARNote(*this);
}

static int iround (float value) {
	int integer = int(value);
	float remain = value - integer;
	return (remain < 0.5) ? integer : integer + 1;
}

int	ARNote::detune2Quarters(float detune)
{
	return iround(detune*2);	// detune in rounded quarter tones
}

void ARNote::browse(TimeUnwrap& mapper) const
{
	mapper.AtPos (this, TimeUnwrap::kNote);
}

int ARNote::getMidiPitch() const
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

void ARNote::setOrnament(const ARTrill * newOrnament, bool trillOwner)
{
	if (fTrillOwner)
		delete fOrnament;
//	fOrnament = newOrnament ? new ARTrill(-1, newOrnament) : 0;
	fOrnament = newOrnament;
	fTrillOwner = trillOwner;
}

ARCluster *ARNote::setCluster(ARCluster *inCluster,
                              bool inClusterHaveToBeDrawn,
                              bool inHaveToBeCreated)
{
    if (!fClusterHaveToBeDrawn && inClusterHaveToBeDrawn)
        fClusterHaveToBeDrawn = true;

    inCluster->setVoiceNum(getVoiceNum());

    if (inHaveToBeCreated) {
        fCluster = new ARCluster(inCluster);
		fOwnCluster = true; 
	}    
	else
        fCluster = inCluster;

    return fCluster;
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

void ARNote::forceNoteAppearance(NVstring noteAppearance) {
    fNoteAppearance = noteAppearance;
}

void ARNote::print(std::ostream& os) const
{
	os << getGMNName();
}

string ARNote::getGMNName () const
{
    const char* accidental = "";
    switch (getAccidentals()) {
		case -2:
			accidental = "&&";
			break;
		case -1:
			accidental = "&";
			break;
		case 1:
			accidental = "#";
			break;
		case 2:
			accidental = "##";
			break;
		default:
			break;
    }
	stringstream s;
	if (!isEmptyNote())
		s << getName() << accidental << getOctave() << "*" << getDuration();
    else
        s << getName() << "*" << getDuration();
	return s.str();
}

