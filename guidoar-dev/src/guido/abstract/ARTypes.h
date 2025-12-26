/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#ifndef __ARTypes__
#define __ARTypes__

#include "gar_smartpointer.h"
#include "guidotags.h"

namespace guido 
{

class guidoelement;
class guidoattribute;
class guidotag;

typedef SMARTP<guidoattribute> 	Sguidoattribute;
typedef SMARTP<guidoelement> 	Sguidoelement;
typedef SMARTP<guidotag>		Sguidotag;

class ARNote;
class ARMusic;
class ARChord;
class ARVoice;
template<int T> class ARTag;

typedef SMARTP<ARNote>		SARNote;
typedef SMARTP<ARMusic>		SARMusic;
typedef SMARTP<ARChord>		SARChord;
typedef SMARTP<ARVoice>		SARVoice;

typedef SMARTP<ARTag<kTAcc> > 		SARAcc;
typedef SMARTP<ARTag<kTAccel> > 	SARAccel;
typedef SMARTP<ARTag<kTAccelBegin> > SARAccelBegin;
typedef SMARTP<ARTag<kTAccelEnd> > 	SARAccelEnd;
typedef SMARTP<ARTag<kTAccent> > 	SARAccent;
typedef SMARTP<ARTag<kTAccol> > 	SARAccol;
typedef SMARTP<ARTag<kTAlter> > 	SARAlter;
typedef SMARTP<ARTag<kTAuto> > 		SARAuto;
typedef SMARTP<ARTag<kTBar> > 		SARBar;
typedef SMARTP<ARTag<kTBarFormat> > SARBarFormat;
typedef SMARTP<ARTag<kTBeam> > 		SARBeam;
typedef SMARTP<ARTag<kTBeamBegin> > SARBeamBegin;
typedef SMARTP<ARTag<kTBeamEnd> > 	SARBeamEnd;
typedef SMARTP<ARTag<kTBeamsAuto> > SARBeamsAuto;
typedef SMARTP<ARTag<kTBeamsOff> > 	SARBeamsOff;
typedef SMARTP<ARTag<kTBembel> > 	SARBembel;
typedef SMARTP<ARTag<kTBreathMark> > SARBreathMark;
typedef SMARTP<ARTag<kTClef> > 		SARClef;
typedef SMARTP<ARTag<kTCoda> > 		SARCoda;
typedef SMARTP<ARTag<kTColor> > 	SARColor;
typedef SMARTP<ARTag<kTComposer> > 	SARComposer;
typedef SMARTP<ARTag<kTCresc> > 	SARCresc;
typedef SMARTP<ARTag<kTCrescBegin> > SARCrescBegin;
typedef SMARTP<ARTag<kTCrescEnd> > 	SARCrescEnd;
typedef SMARTP<ARTag<kTCue> > 		SARCue;
typedef SMARTP<ARTag<kTDaCapo> > 	SARDaCapo;
typedef SMARTP<ARTag<kTDaCapoAlFine> > SARDaCapoAlFine;
typedef SMARTP<ARTag<kTDaCoda> > 	SARDaCoda;
typedef SMARTP<ARTag<kTDalSegno> > 	SARDalSegno;
typedef SMARTP<ARTag<kTDalSegnoAlFine> > SARDalSegnoAlFine;
typedef SMARTP<ARTag<kTDecresc> > 	SARDecresc;
typedef SMARTP<ARTag<kTDefineTag> > SARDefineTag;
typedef SMARTP<ARTag<kTDim> > 		SARDim;
typedef SMARTP<ARTag<kTDimBegin> > 	SARDimBegin;
typedef SMARTP<ARTag<kTDimEnd> > 	SARDimEnd;
typedef SMARTP<ARTag<kTDiminuendo> > SARDiminuendo;
typedef SMARTP<ARTag<kTDispDur> > 	SARDispDur;
typedef SMARTP<ARTag<kTDotFormat> > SARDotFormat;
typedef SMARTP<ARTag<kTDoubleBar> > SARDoubleBar;
typedef SMARTP<ARTag<kTEndBar> > 	SAREndBar;
typedef SMARTP<ARTag<kTFermata> > 	SARFermata;
typedef SMARTP<ARTag<kTFine> > 		SARFine;
typedef SMARTP<ARTag<kTFingering> > SARFingering;
typedef SMARTP<ARTag<kTGrace> > 	SARGrace;
typedef SMARTP<ARTag<kTHeadsCenter> > SARHeadsCenter;
typedef SMARTP<ARTag<kTHeadsLeft> > SARHeadsLeft;
typedef SMARTP<ARTag<kTHeadsNormal> > SARHeadsNormal;
typedef SMARTP<ARTag<kTHeadsReverse> > SARHeadsReverse;
typedef SMARTP<ARTag<kTHeadsRight> > SARHeadsRight;
typedef SMARTP<ARTag<kTInstr> > 	SARInstr;
typedef SMARTP<ARTag<kTIntens> > 	SARIntens;
typedef SMARTP<ARTag<kTKey> > 		SARKey;
typedef SMARTP<ARTag<kTLabel> > 	SARLabel;
typedef SMARTP<ARTag<kTLyrics> > 	SARLyrics;
typedef SMARTP<ARTag<kTMarcato> > 	SARMarcato;
typedef SMARTP<ARTag<kTMark> > 		SARMark;
typedef SMARTP<ARTag<kTMerge> > 	SARMerge;
typedef SMARTP<ARTag<kTMeter> > 	SARMeter;
typedef SMARTP<ARTag<kTMord> > 		SARMord;
typedef SMARTP<ARTag<kTNewLine> > 	SARNewLine;
typedef SMARTP<ARTag<kTNewPage> > 	SARNewPage;
typedef SMARTP<ARTag<kTNewSystem> > SARNewSystem;
typedef SMARTP<ARTag<kTNoteFormat> > SARNoteFormat;
typedef SMARTP<ARTag<kTOct> > 		SAROct;
typedef SMARTP<ARTag<kTPageFormat> > SARPageFormat;
typedef SMARTP<ARTag<kTPort> > 		SARPort;
typedef SMARTP<ARTag<kTRepeatBegin> > SARRepeatBegin;
typedef SMARTP<ARTag<kTRepeatEnd> > SARRepeatEnd;
typedef SMARTP<ARTag<kTRestFormat> > SARRestFormat;
typedef SMARTP<ARTag<kTRit> > 		SARRit;
typedef SMARTP<ARTag<kTRitBegin> > 	SARRitBegin;
typedef SMARTP<ARTag<kTRitEnd> > 	SARRitEnd;
typedef SMARTP<ARTag<kTSegno> > 	SARSegno;
typedef SMARTP<ARTag<kTShareLocation> > SARShareLocation;
typedef SMARTP<ARTag<kTSlur> > 		SARSlur;
typedef SMARTP<ARTag<kTSlurBegin> > SARSlurBegin;
typedef SMARTP<ARTag<kTSlurEnd> > 	SARSlurEnd;
typedef SMARTP<ARTag<kTSpace> > 	SARSpace;
typedef SMARTP<ARTag<kTSpecial> > 	SARSpecial;
typedef SMARTP<ARTag<kTSplitChord> > SARSplitChord;
typedef SMARTP<ARTag<kTStacc> > 	SARStacc;
typedef SMARTP<ARTag<kTStaccBegin> > SARStaccBegin;
typedef SMARTP<ARTag<kTStaccEnd> > 	SARStaccEnd;
typedef SMARTP<ARTag<kTStaff> > 	SARStaff;
typedef SMARTP<ARTag<kTStaffFormat> > SARStaffFormat;
typedef SMARTP<ARTag<kTStaffOff> > 	SARStaffOff;
typedef SMARTP<ARTag<kTStaffOn> > 	SARStaffOn;
typedef SMARTP<ARTag<kTStemsAuto> > SARStemsAuto;
typedef SMARTP<ARTag<kTStemsDown> > SARStemsDown;
typedef SMARTP<ARTag<kTStemsOff> > 	SARStemsOff;
typedef SMARTP<ARTag<kTStemsUp> > 	SARStemsUp;
typedef SMARTP<ARTag<kTSystemFormat> > 	SARSystemFormat;
typedef SMARTP<ARTag<kTTempo> > 	SARTempo;
typedef SMARTP<ARTag<kTTen> >		SARTen;
typedef SMARTP<ARTag<kTText> >		SARText;
typedef SMARTP<ARTag<kTTie> >		SARTie;
typedef SMARTP<ARTag<kTTieBegin> > 	SARTieBegin;
typedef SMARTP<ARTag<kTTieEnd> > 	SARTieEnd;
typedef SMARTP<ARTag<kTTitle> > 	SARTitle;
typedef SMARTP<ARTag<kTTrem> > 		SARTrem;
typedef SMARTP<ARTag<kTTrill> > 	SARTrill;
typedef SMARTP<ARTag<kTTuplet> > 	SARTuplet;
typedef SMARTP<ARTag<kTTurn> > 		SARTurn;
typedef SMARTP<ARTag<kTUnits> > 	SARUnits;
typedef SMARTP<ARTag<kTVolta> > 	SARVolta;
typedef SMARTP<ARTag<kTVoltaBegin> > SARVoltaBegin;
typedef SMARTP<ARTag<kTVoltaEnd> > 	SARVoltaEnd;
typedef SMARTP<ARTag<kTDrHoos> > 	SARDrHoos;
typedef SMARTP<ARTag<kTDrRenz> > 	SARDrRenz;
typedef SMARTP<ARTag<kTBackward> > 	SARBackward;

} // namespace

#endif
