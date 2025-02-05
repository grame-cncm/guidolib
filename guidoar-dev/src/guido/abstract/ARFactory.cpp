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

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <iostream>

#include "guidotags.h"
#include "ARFactory.h"
#include "ARChord.h"
#include "ARNote.h"
#include "AROthers.h"
#include "ARTag.h"

using namespace std; 

namespace guido 
{

//______________________________________________________________________________
template<int elt>
class newTagFunctor : public functor<Sguidotag,long> {
	public:
		Sguidotag operator ()(long id) { return ARTag<elt>::create(id); }
};

//______________________________________________________________________________
SARMusic ARFactory::createMusic() const
{
	SARMusic elt = ARMusic::create();
	if (elt) elt->setName("music");
	return elt;
}

//______________________________________________________________________________
SARVoice ARFactory::createVoice() const
{
	SARVoice elt = ARVoice::create();
	if (elt) elt->setName("voice");
	return elt;
}

//______________________________________________________________________________
SARChord ARFactory::createChord() const
{
	SARChord elt = ARChord::create();
	if (elt) elt->setName("chord");
	return elt;
}

//______________________________________________________________________________
SARNote ARFactory::createNote(const std::string& name) const
{
	SARNote elt = ARNote::create();
	if (elt) elt->setName(name);
	return elt;
}

//______________________________________________________________________________
Sguidotag ARFactory::createTag(const string& eltname, long id) const
{ 
	map<std::string, NewTagFunctor*>::const_iterator i = fMap.find( eltname );
	if (i != fMap.end()) {
		NewTagFunctor* f= i->second;
		if (f) {
			Sguidotag elt = (*f)(id);
			elt->setName(eltname);
			return elt;
		}
	}
	cerr << "Sguidoelement factory::create called with unknown element \"" << eltname << "\"" << endl;
	return 0;
}

//______________________________________________________________________________
ARFactory::ARFactory() 
{
	fMap["acc"]			= new newTagFunctor<kTAcc>;
	fMap["accel"]		= new newTagFunctor<kTAccel>;
	fMap["accelBegin"]	= new newTagFunctor<kTAccelBegin>;
	fMap["accelEnd"]	= new newTagFunctor<kTAccelEnd>;
	fMap["accent"]		= new newTagFunctor<kTAccent>;
	fMap["accol"]		= new newTagFunctor<kTAccol>;
	fMap["alter"]		= new newTagFunctor<kTAlter>;
	fMap["auto"]		= new newTagFunctor<kTAuto>;
	fMap["autoBreak"]	= new newTagFunctor<kTAutoBreak>;
	fMap["bar"]			= new newTagFunctor<kTBar>;
	fMap["barFormat"]	= new newTagFunctor<kTBarFormat>;
	fMap["beam"]		= new newTagFunctor<kTBeam>;
	fMap["bm"]			= fMap["beam"];
	fMap["b"]			= fMap["beam"];
	fMap["beamBegin"]	= new newTagFunctor<kTBeamBegin>;
	fMap["beamEnd"]		= new newTagFunctor<kTBeamEnd>;
	fMap["beamsAuto"]	= new newTagFunctor<kTBeamsAuto>;
	fMap["beamsOff"]	= new newTagFunctor<kTBeamsOff>;
	fMap["bembel"]		= new newTagFunctor<kTBembel>;
	fMap["breathMark"]	= new newTagFunctor<kTBreathMark>;
	fMap["clef"]		= new newTagFunctor<kTClef>;
	fMap["coda"]		= new newTagFunctor<kTCoda>;
	fMap["color"]		= new newTagFunctor<kTColor>;
	fMap["colour"]		= fMap["color"];
	fMap["composer"]	= new newTagFunctor<kTComposer>;
	fMap["cresc"]		= new newTagFunctor<kTCresc>;
	fMap["crescBegin"]	= new newTagFunctor<kTCrescBegin>;
	fMap["crescEnd"]	= new newTagFunctor<kTCrescEnd>;
	fMap["cue"]			= new newTagFunctor<kTCue>;
	fMap["daCapo"]		= new newTagFunctor<kTDaCapo>;
	fMap["daCapoAlFine"]	= new newTagFunctor<kTDaCapoAlFine>;
	fMap["daCoda"]			= new newTagFunctor<kTDaCoda>;
	fMap["dalSegno"]		= new newTagFunctor<kTDalSegno>;
	fMap["dalSegnoAlFine"]	= new newTagFunctor<kTDalSegnoAlFine>;
	fMap["decresc"]		= new newTagFunctor<kTDecresc>;
	fMap["defineTag"]	= new newTagFunctor<kTDefineTag>;
	fMap["dim"]			= new newTagFunctor<kTDim>;
	fMap["dimBegin"]	= new newTagFunctor<kTDimBegin>;
	fMap["dimEnd"]		= new newTagFunctor<kTDimEnd>;
	fMap["diminuendo"]	= new newTagFunctor<kTDiminuendo>;
	fMap["dispDur"]		= new newTagFunctor<kTDispDur>;
	fMap["dotFormat"]	= new newTagFunctor<kTDotFormat>;
	fMap["doubleBar"]	= new newTagFunctor<kTDoubleBar>;
	fMap["endBar"]		= new newTagFunctor<kTEndBar>;
	fMap["fermata"]		= new newTagFunctor<kTFermata>;
	fMap["fine"]		= new newTagFunctor<kTFine>;
	fMap["fingering"]	= new newTagFunctor<kTFingering>;
	fMap["grace"]		= new newTagFunctor<kTGrace>;
	fMap["headsCenter"]	= new newTagFunctor<kTHeadsCenter>;
	fMap["headsLeft"]	= new newTagFunctor<kTHeadsLeft>;
	fMap["headsNormal"]	= new newTagFunctor<kTHeadsNormal>;
	fMap["headsReverse"]= new newTagFunctor<kTHeadsReverse>;
	fMap["headsRight"]	= new newTagFunctor<kTHeadsRight>;
	fMap["instr"]		= new newTagFunctor<kTInstr>;
	fMap["intens"]		= new newTagFunctor<kTIntens>;
	fMap["i"]			= fMap["intens"];
	fMap["key"]			= new newTagFunctor<kTKey>;
	fMap["label"]		= new newTagFunctor<kTLabel>;
	fMap["lyrics"]		= new newTagFunctor<kTLyrics>;
	fMap["marcato"]		= new newTagFunctor<kTMarcato>;
	fMap["mark"]		= new newTagFunctor<kTMark>;
	fMap["merge"]		= new newTagFunctor<kTMerge>;
	fMap["meter"]		= new newTagFunctor<kTMeter>;
	fMap["mord"]		= new newTagFunctor<kTMord>;
	fMap["newLine"]		= new newTagFunctor<kTNewLine>;
	fMap["newPage"]		= new newTagFunctor<kTNewPage>;
	fMap["newSystem"]	= new newTagFunctor<kTNewSystem>;
	fMap["noteFormat"]	= new newTagFunctor<kTNoteFormat>;
	fMap["oct"]			= new newTagFunctor<kTOct>;
	fMap["pageFormat"]	= new newTagFunctor<kTPageFormat>;
	fMap["port"]		= new newTagFunctor<kTPort>;
	fMap["repeatBegin"]	= new newTagFunctor<kTRepeatBegin>;
	fMap["repeatEnd"]	= new newTagFunctor<kTRepeatEnd>;
	fMap["restFormat"]	= new newTagFunctor<kTRestFormat>;
	fMap["rit"]			= new newTagFunctor<kTRit>;
	fMap["ritBegin"]	= new newTagFunctor<kTRitBegin>;
	fMap["ritEnd"]		= new newTagFunctor<kTRitEnd>;
	fMap["segno"]		= new newTagFunctor<kTSegno>;
	fMap["set"]			= fMap["auto"];
	fMap["shareLocation"]	= new newTagFunctor<kTShareLocation>;
	fMap["slur"]		= new newTagFunctor<kTSlur>;
	fMap["sl"]			= fMap["slur"];
	fMap["slurBegin"]	= new newTagFunctor<kTSlurBegin>;
	fMap["slurEnd"]		= new newTagFunctor<kTSlurEnd>;
	fMap["space"]		= new newTagFunctor<kTSpace>;
	fMap["special"]		= new newTagFunctor<kTSpecial>;
	fMap["splitChord"]	= new newTagFunctor<kTSplitChord>;
	fMap["chord"]		= fMap["splitChord"];
	fMap["stacc"]		= new newTagFunctor<kTStacc>;
	fMap["staccBegin"]	= new newTagFunctor<kTStaccBegin>;
	fMap["staccEnd"]	= new newTagFunctor<kTStaccEnd>;
	fMap["staff"]		= new newTagFunctor<kTStaff>;
	fMap["staffFormat"]	= new newTagFunctor<kTStaffFormat>;
	fMap["staffOff"]	= new newTagFunctor<kTStaffOff>;
	fMap["staffOn"]		= new newTagFunctor<kTStaffOn>;
	fMap["stemsAuto"]	= new newTagFunctor<kTStemsAuto>;
	fMap["stemsDown"]	= new newTagFunctor<kTStemsDown>;
	fMap["stemsOff"]	= new newTagFunctor<kTStemsOff>;
	fMap["stemsUp"]		= new newTagFunctor<kTStemsUp>;
	fMap["systemFormat"]= new newTagFunctor<kTSystemFormat>;
	fMap["tempo"]		= new newTagFunctor<kTTempo>;
	fMap["ten"]			= new newTagFunctor<kTTen>;
	fMap["text"]		= new newTagFunctor<kTText>;
	fMap["t"]			= fMap["text"];
	fMap["tie"]			= new newTagFunctor<kTTie>;
	fMap["tieBegin"]	= new newTagFunctor<kTTieBegin>;
	fMap["tieEnd"]		= new newTagFunctor<kTTieEnd>;
	fMap["title"]		= new newTagFunctor<kTTitle>;
	fMap["trem"]		= new newTagFunctor<kTTrem>;
	fMap["trill"]		= new newTagFunctor<kTTrill>;
	fMap["tuplet"]		= new newTagFunctor<kTTuplet>;
	fMap["turn"]		= new newTagFunctor<kTTurn>;
	fMap["units"]		= new newTagFunctor<kTUnits>;
	fMap["volta"]		= new newTagFunctor<kTVolta>;
	fMap["voltaBegin"]	= new newTagFunctor<kTVoltaBegin>;
	fMap["voltaEnd"]	= new newTagFunctor<kTVoltaEnd>;
	fMap["DrHoos"]		= new newTagFunctor<kTDrHoos>;
	fMap["DrRenz"]		= new newTagFunctor<kTDrRenz>;
	fMap["backward"]	= new newTagFunctor<kTBackward>;
	// new tags Guido v.1.38
	fMap["harmonic"]	= new newTagFunctor<kTHarmonic>;
	fMap["pizz"]		= new newTagFunctor<kTPizz>;
}

//______________________________________________________________________________
// WARNING !
// the functors map contains aliases that must be set to 0 before deallocation
//
ARFactory::~ARFactory()
{
	fMap["bm"]			= 0;
	fMap["b"]			= 0;
	fMap["colour"]		= 0;
	fMap["i"]			= 0;
	fMap["set"]			= 0;
	fMap["sl"]			= 0;
	fMap["chord"]		= 0;
	fMap["t"]			= 0;
	TFunctors::iterator i = fMap.begin();
	while (i != fMap.end()) {
		delete i->second;
		i++;
	}
}


} // namespace
