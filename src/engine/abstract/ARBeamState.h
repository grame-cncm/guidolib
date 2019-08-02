#ifndef ARBeamState_H
#define ARBeamState_H

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

#include "ARMusicalTag.h"

/*@mkdoc
@group:Beaming

@tagname:\beamsAuto
@tagalias:
@tagtype:P
@tagnotation:set automatic beaming on
@tagdesc
@tagend

@tagname:\beamsOff
@tagalias:
@tagtype:P
@tagnotation:set automatic beaming off
@tagdesc
@tagend

@tagname:\beamsFull
@tagalias:
@tagtype:P
@tagnotation:set full automatic beaming on
@tagdesc
A rest inside a group of notes prevents automatic beaming unless full beaming is on.

@tagend

*/

/** \brief not yet documented
*/
class ARBeamState : public ARMusicalTag  
{
public: 
	enum beamstate { AUTO, OFF, FULL };

				 ARBeamState(beamstate p_state) { state = p_state; }
	virtual		~ARBeamState() {}
		
	virtual bool IsStateTag() const		{ return true; }

	virtual const char*	getParamsStr() const	{ return ""; };
	virtual const char*	getTagName() const		{ return "ARBeamState"; };
	
	beamstate getBeamState() const		{ return state; }
    ARMusicalObject *isARBeamState()	{ return this; }

protected:
	beamstate state;
};

#endif
