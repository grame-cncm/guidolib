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
