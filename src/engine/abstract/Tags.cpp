/*
  GUIDO Library
  Copyright (C) 2017 Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "Tags.h"

using namespace std;

string makeTag (const char* tag) {
	string backslah ("\\");
	return backslah + tag;
}

const char* kTagAcc			= "acc";
const char* kTagAccidental	= "accidental";
const char* kTagAccelBegin	= "accelBegin";
const char* kTagAccelEnd	= "accelEnd";
const char* kTagAccelerando	= "accelerando";
const char* kTagAccel		= "accel";
const char* kTagAccent		= "accent";
const char* kTagAccolade	= "accolade";
const char* kTagAccol		= "accol";
const char* kTagAlter		= "alter";
const char* kTagAuto		= "auto";
const char* kTagBar			= "bar";
const char* kTagBarFormat	= "barFormat";
const char* kTagBeam		= "beam";
const char* kTagBm			= "bm";
const char* kTagB			= "b";
const char* kTagBeamBegin	= "beamBegin";
const char* kTagBeamEnd		= "beamEnd";
const char* kTagBeamsAuto	= "beamsAuto";
const char* kTagBeamsFull	= "beamsFull";
const char* kTagBeamsOff	= "beamsOff";
const char* kTagBembel		= "bembel";
const char* kTagBreathMark	= "breathMark";
const char* kTagClef		= "clef";
const char* kTagCluster		= "cluster";
const char* kTagCoda		= "coda";
const char* kTagColor		= "color";
const char* kTagColour		= "colour";
const char* kTagComposer	= "composer";
const char* kTagCresc		= "cresc";
const char* kTagCrescendo	= "crescendo";
const char* kTagCrescBegin	= "crescBegin";
const char* kTagCrescEnd	= "crescEnd";
const char* kTagCue			= "cue";
const char* kTagDaCapo		= "daCapo";
const char* kTagDaCapoAlFine = "daCapoAlFine";
const char* kTagDaCoda		= "daCoda";
const char* kTagDalSegno	= "dalSegno";
const char* kTagDalSegnoAlFine = "dalSegnoAlFine";
const char* kTagDecresc		= "decresc";
const char* kTagDecrescendo	= "decrescendo";
const char* kTagDim			= "dim";
const char* kTagDiminuendo	= "diminuendo";
const char* kTagDecrescBegin= "decrescBegin";
const char* kTagDimBegin	= "dimBegin";
const char* kTagDiminuendoBegin	= "diminuendoBegin";
const char* kTagDecrescEnd		= "decrescEnd";
const char* kTagDimEnd			= "dimEnd";
const char* kTagDiminuendoEnd	= "diminuendoEnd";
const char* kTagDispDur			= "dispDur";
const char* kTagDisplayDuration	= "displayDuration";
const char* kTagDotFormat		= "dotFormat";
const char* kTagDoubleBar		= "doubleBar";
const char* kTagEndBar			= "endBar";
const char* kTagFBeam			= "fBeam";
const char* kTagFBeamBegin		= "fBeamBegin";
const char* kTagFBeamEnd		= "fBeamEnd";
const char* kTagFermata			= "fermata";
const char* kTagFine			= "fine";
const char* kTagFingering		= "fingering";
const char* kTagGlissando		= "glissando";
const char* kTagGlissandoBegin	= "glissandoBegin";
const char* kTagGlissandoEnd	= "glissandoEnd";
const char* kTagGrace		= "grace";
const char* kTagHarmonic	= "harmonic";
const char* kTagHarmony		= "harmony";
const char* kTagHeadsCenter	= "headsCenter";
const char* kTagHeadsLeft	= "headsLeft";
const char* kTagHeadsNormal	= "headsNormal";
const char* kTagHeadsReverse= "headsReverse";
const char* kTagHeadsRight	= "headsRight";
const char* kTagInstrument	= "instrument";
const char* kTagInstr		= "instr";
const char* kTagIntensity	= "intensity";
const char* kTagIntens		= "intens";
const char* kTagI			= "i";
const char* kTagKey			= "key";
const char* kTagLabel		= "label";
const char* kTagLyrics		= "lyrics";
const char* kTagMarcato		= "marcato";
const char* kTagMark		= "mark";
const char* kTagMerge		= "merge";
const char* kTagMeter		= "meter";
const char* kTagMordent		= "mordent";
const char* kTagMord		= "mord";
const char* kTagNewLine		= "newLine";
const char* kTagNewSystem	= "newSystem";
const char* kTagNewPage		= "newPage";
const char* kTagNoteFormat	= "noteFormat";
const char* kTagOct			= "oct";
const char* kTagOctava		= "octava";
const char* kTagPageFormat	= "pageFormat";
const char* kTagPizzicato	= "pizzicato";
const char* kTagPizz		= "pizz";
const char* kTagPort		= "port";
const char* kTagRepeatBegin	= "repeatBegin";
const char* kTagRepeatEnd	= "repeatEnd";
const char* kTagRestFormat	= "restFormat";
const char* kTagRitBegin	= "ritBegin";
const char* kTagRitEnd		= "ritEnd";
const char* kTagRitardando	= "ritardando";
const char* kTagRit			= "rit";
const char* kTagSegno		= "segno";
const char* kTagSet			= "set";
const char* kTagShareLocation	= "shareLocation";
const char* kTagShortFermata	= "shortFermata";
const char* kTagSlur		= "slur";
const char* kTagSl			= "sl";
const char* kTagSlurBegin	= "slurBegin";
const char* kTagSlurEnd		= "slurEnd";
const char* kTagSpace		= "space";
const char* kTagSpecial		= "special";
const char* kTagSplitChord	= "splitChord";
const char* kTagChord		= "chord";
const char* kTagStaccBegin	= "staccBegin";
const char* kTagStaccEnd	= "staccEnd";
const char* kTagStaccato	= "staccato";
const char* kTagStacc		= "stacc";
const char* kTagStaff		= "staff";
const char* kTagStaffFormat	= "staffFormat";
const char* kTagStaffOff	= "staffOff";
const char* kTagStaffOn		= "staffOn";
const char* kTagStemsAuto	= "stemsAuto";
const char* kTagStemsDown	= "stemsDown";
const char* kTagStemsOff	= "stemsOff";
const char* kTagStemsUp		= "stemsUp";
const char* kTagSymbol		= "symbol";
const char* kTagS			= "s";
const char* kTagSystemFormat= "systemFormat";
const char* kTagTempo		= "tempo";
const char* kTagTenuto		= "tenuto";
const char* kTagTen			= "ten";
const char* kTagText		= "text";
const char* kTagT			= "t";
const char* kTagTie			= "tie";
const char* kTagTieBegin	= "tieBegin";
const char* kTagTieEnd		= "tieEnd";
const char* kTagTitle		= "title";
const char* kTagTremolo		= "tremolo";
const char* kTagTrem		= "trem";
const char* kTagTremoloBegin= "tremoloBegin";
const char* kTagTremBegin	= "tremBegin";
const char* kTagTremoloEnd	= "tremoloEnd";
const char* kTagTremEnd		= "tremEnd";
const char* kTagTrill		= "trill";
const char* kTagTrillBegin	= "trillBegin";
const char* kTagTrillEnd	= "trillEnd";
const char* kTagTuplet		= "tuplet";
const char* kTagTurn		= "turn";
const char* kTagUnits		= "units";
const char* kTagVolta		= "volta";
const char* kTagVoltaBegin	= "voltaBegin";
const char* kTagVoltaEnd	= "voltaEnd";

