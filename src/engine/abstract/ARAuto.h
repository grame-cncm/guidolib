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

		state getStretchLastLineState() const		{ return fStretchLastLineState; }
		state getStretchFirstLineState() const		{ return fStretchFirstLineState; }
		state getClefKeyMeterOrderState() const		{ return fClefKeyMeterOrderState; }
		state getEndBarState() const				{ return fEndBarState; }
		state getSystemBreakState() const			{ return fSystemBreakState; }
		state getPageBreakState() const				{ return fPageBreakState; }
		state getAutoLyricsPos() const				{ return fLyricsAutoPos; }
		state getAutoInstrPos() const				{ return fInstrAutoPos; }
		position getFingeringPos() const			{ return fFingeringPos; }
		float    getFingeringSize() const			{ return fFingeringSize; }
		bool 	 hasFingeringPos() const			{ return fHasFingeringPos; }
		bool     hasFingeringSize() const			{ return fHasFingeringSize; }

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
		state fStretchLastLineState;
		state fStretchFirstLineState;
		state fLyricsAutoPos;
		state fInstrAutoPos;
	
		bool		fHasFingeringPos 	= false;
		bool		fHasFingeringSize	= false;
		position 	fFingeringPos;
		float		fFingeringSize;
};

#endif
