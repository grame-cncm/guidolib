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
// GuidoCreator.h: declaration/interface of class CGuidoCreator.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUIDOCREATOR_H__E7BE0C60_9611_11D5_9AE7_00600857CEE6__INCLUDED_)
#define AFX_GUIDOCREATOR_H__E7BE0C60_9611_11D5_9AE7_00600857CEE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>

class CGuidoCreator  
{
public:
	void keyS();
	void clefS();
	void meterS();
	void measureLine();
	void segmentEnd();
	void segmentBegin();
	void sequenceBegin(int track, int subtrack);
	void sequenceEnd();
	void beamEnd(int b);
	void beamBegin(int i);
	void graceEnd();
	void graceBegin();
	void cueEnd();
	void cueBegin();
	void chordEnd(int label);
	void chordBegin(int label);
	void slurEnd(int id);
	void slurBegin(int id);
	void tieEnd(int id);
	void tieBegin(int id);
	void key(int i);
	void meter(int num,int den);
	void clef(int clefCode);
	void note(char pitch, int chromatic, int octave, int durNumerator, int durDenominator, int dispNumerator, int dispDenominator, int dispDots);
	void empty(int numerator, int denominator, int dots);
	void note(char pitch,int chromatic, int octave, int numerator, int denominator, int dots);
	void setFile(FILE*f);
	void stemsOff();
	void stemsDown();
	void stemsUp();
	void init();
	void staff(int i);
	CGuidoCreator();
	virtual ~CGuidoCreator();

protected:
	FILE* file;
	//Segment-information
	int sequenceCount;

	//Sequence-information
	int beamsOpen;
	int lastStaff;
	int lastStem;
	int lastNoteOctave;
	int lastNoteNumerator;
	int lastNoteDenominator;
	int lastNoteDots;
	int inChord;

	//chord-information
	int notesInChord;

	//information for delayed printing of clefs, meters and keys		int cleff1;
	int clef1;
	int clef2;
	int meterDen;
	int meterNum;
	int key_;

};

extern CGuidoCreator guidoCreator;

#endif // !defined(AFX_GUIDOCREATOR_H__E7BE0C60_9611_11D5_9AE7_00600857CEE6__INCLUDED_)
