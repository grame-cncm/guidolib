#ifndef ARBeam_H
#define ARBeam_H

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
#include "ARPositionTag.h"
// #include "ARMusicalEvent.h"

class GRBeam;

/** \brief Abstract representation of a beam.
*/
class ARBeam :  public ARMTParameter, public ARPositionTag
{
    friend class GRBeam;

public:
             ARBeam();
	virtual ~ARBeam();

	bool isGuidoSpecBeam() const;
	bool isFeatheredBeam() const {return feathered;}

	virtual bool MatchEndTag(const char * s);

	virtual void printName(std::ostream& os) const;
	virtual void printGMNName(std::ostream& os) const;
	virtual void printParameters(std::ostream& os) const;

	virtual void setTagParameterList(TagParameterList & tpl);
		
protected:
    static ListOfTPLs ltpls;

    TagParameterFloat * dx1;
    TagParameterFloat * dy1;
    TagParameterFloat * dx2;
    TagParameterFloat * dy2;
    TagParameterFloat * dx3;
    TagParameterFloat * dy3;
    TagParameterFloat * dx4;
    TagParameterFloat * dy4;

    bool feathered;
};

#endif

