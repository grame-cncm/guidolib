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

	enum state { ON,OFF };

			 ARAuto();
	virtual ~ARAuto();

	state getStretchLastLineState() const		{ return stretchLastLineState; }
	state getStretchFirstLineState() const		{ return stretchFirstLineState; }
	state getClefKeyMeterOrderState() const		{ return clefKeyMeterOrderState; }
	state getEndBarState() const				{ return endBarState; }
	state getSystemBreakState() const			{ return systemBreakState; }
	state getPageBreakState() const				{ return pageBreakState; } 

	virtual void setTagParameterList(TagParameterList & tpl);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual bool IsStateTag() const				{ return true; }

protected:
	state endBarState;
	state pageBreakState;
	state systemBreakState;
	state clefKeyMeterOrderState;
	state stretchLastLineState;
	state stretchFirstLineState;

	int numparset;
	static ListOfTPLs ltpls;
};

#endif
