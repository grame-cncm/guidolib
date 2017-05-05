#ifndef ARAuto_H
#define ARAuto_H

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

#include "ARMTParameter.h"

class ARMusicalVoiceState;

/** \brief Auto tag
*/
class ARAuto : public ARMTParameter
{
public:
	friend class ARMusicalVoiceState;

	enum state { kOff,kOn };

			 ARAuto();
	virtual ~ARAuto();

	state getStretchLastLineState() const		{ return fStretchLastLineState; }
	state getStretchFirstLineState() const		{ return fStretchFirstLineState; }
	state getClefKeyMeterOrderState() const		{ return fClefKeyMeterOrderState; }
	state getEndBarState() const				{ return fEndBarState; }
	state getSystemBreakState() const			{ return fSystemBreakState; }
	state getPageBreakState() const				{ return fPageBreakState; }
	state getAutoLyricsPos() const				{ return fLyricsAutoPos; }
	state getAutoInstrPos() const				{ return fInstrAutoPos; }

	virtual void setTagParameterList(TagParameterList & tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

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

	static ListOfTPLs ltpls;
};

#endif
