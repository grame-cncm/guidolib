#ifndef ARAuto_H
#define ARAuto_H

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

#include "ARMTParameter.h"

class ARMusicalVoiceState;


/*@mkdoc

@group:Miscellaneous

@tagname:\auto
@tagalias:\set
@tagtype:P
@tagnotation:sets voice level global settings
@tagdesc
@tagend

@params:
@param:autoEndBar:string:automatic end bar:on:true
@param:endBar:string:variant for autoEndBar:on:true

@param:autoPageBreak:string:automatic page break:on:true
@param:pageBreak:string:variant for autoPageBreak:on:true

@param:autoSystemBreak:string:automatic system break:on:true
@param:systemBreak:string:variant for autoSystemBreak:on:true

@param:autoClefKeyMeterOrder:string:automatic reordering of clef, key and meter:on:true
@param:clefKeyMeterOrder:string:variant for autoClefKeyMeterOrder:on:true

@param:autoLyricsPos:string:automatic collision avoidance for \lyrics:off:true
@param:lyricsAutoPos:string:ariant for autoLyricsPos:off:true

@param:autoInstrPos:string:automatic positionning for \instr:off:true
@param:instrAutoPos:string:variant for autoInstrPos:off:true

@param:autoIntensPos:string:automatic collision avoidance for \intens:off:true
@param:intensAutoPos:string:variant for autoIntensPos:off:true

@param:harmonyPos:string:above or below (see [\harmony](../Text/#harmony)):*none*:true
@param:fingeringPos:string:above or below (see [\fingering](../Text/#fingering)):*none*:true
@param:fingeringSize:float:the fingering text size (see [\fingering](../Text/#fingering)):*none*:true

@paramdesc
Collision management strategies can produce unexpected results or even create new collisions.
In this case, you should switch to manual layout using the 'dx' and 'dy' [common parameters](../../tagsparams#common-parameters).
@paramend

*/


/** \brief Auto tag
*/
class ARAuto : public ARMTParameter
{
	public:
		friend class ARMusicalVoiceState;

		enum state    { kOff,kOn };
		enum position { kDefault, kAbove,kBelow };

				 ARAuto();
		virtual ~ARAuto() {}

//		state getStretchLastLineState() const		{ return fStretchLastLineState; }
//		state getStretchFirstLineState() const		{ return fStretchFirstLineState; }
		state getClefKeyMeterOrderState() const		{ return fClefKeyMeterOrderState; }
		state getEndBarState() const				{ return fEndBarState; }
		state getSystemBreakState() const			{ return fSystemBreakState; }
		state getPageBreakState() const				{ return fPageBreakState; }
		state getAutoLyricsPos() const				{ return fLyricsAutoPos; }
		state getAutoInstrPos() const				{ return fInstrAutoPos; }
		state getAutoIntensPos() const				{ return fIntensAutoPos; }
		position getHarmonyPos() const				{ return fHarmonyPos; }
		position getFingeringPos() const			{ return fFingeringPos; }
		float    getFingeringSize() const			{ return fFingeringSize; }
		bool 	 hasFingeringPos() const			{ return fHasFingeringPos; }
		bool     hasFingeringSize() const			{ return fHasFingeringSize; }
		bool     hasHarmonyPos() const				{ return fHasHarmonyPos; }

		virtual void setTagParameters (const TagParameterMap& params);

		virtual const char*	getParamsStr() const	{ return kARAutoParams; };
		virtual const char*	getTagName () const		{ return "ARAuto"; };
		virtual std::string getGMNName () const		{ return "\\auto"; };

		virtual bool IsStateTag() const				{ return true; }

	protected:
		state fEndBarState;
		state fPageBreakState;
		state fSystemBreakState;
		state fClefKeyMeterOrderState;
//		state fStretchLastLineState;
//		state fStretchFirstLineState;
		state fLyricsAutoPos;
		state fInstrAutoPos;
		state fIntensAutoPos;

		bool		fHasFingeringPos 	= false;
		bool		fHasHarmonyPos		= false;
		bool		fHasFingeringSize	= false;
		position 	fFingeringPos;
		position 	fHarmonyPos;
		float		fFingeringSize;
};

#endif
