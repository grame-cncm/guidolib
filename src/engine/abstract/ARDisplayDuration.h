#ifndef ARDisplayDuration_H
#define ARDisplayDuration_H

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

class TagParameterInt;

/** \brief not yet documented
*/
class ARDisplayDuration : 
	// public ARMusicalObject,
	public ARMTParameter,
	public ARPositionTag
{
public:

	ARDisplayDuration(const ARDisplayDuration * dspdur);
	ARDisplayDuration() 
	{
		rangesetting = ONLY;
		dots = 0;
		num = denom = ndots = NULL;
	}
	virtual ~ARDisplayDuration();

	virtual bool MatchEndTag(const char * s);

	virtual ARMusicalObject * Copy() const;

	virtual void print(std::ostream& os) const
	{}

	/* virtual ostream & operator<<(ostream &os) const
	{
		os << "\\dispDurBegin";
		os << "<" << dur.getNumerator() <<
			"," << dur.getDenominator() <<
			"," << dots << ">";
		if (getRange())
			os << "(";
		return os << " ";
	} */

	virtual void setTagParameterList(TagParameterList & theTagParameterList);

	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;
	void setDisplayDuration(const TYPE_DURATION & tmp);

	void setDisplayDuration(const TYPE_DURATION & tmp, int numdots)
	{ 
		dur = tmp; 
		dots = numdots; 
	}

	TYPE_DURATION getDisplayDuration() const
	{ return dur; }

	int getDots() const
	{ return dots; }
    
    /**** Function to avoid dynamic_cast ****/
    ARMusicalObject *isARDisplayDuration() { return this; }
    /*****************************************/

protected:
	static ListOfTPLs ltpls;

	TagParameterInt * num;
	TagParameterInt * denom;
	TagParameterInt * ndots;

	// the parameters ...
	TYPE_DURATION dur; // the dur of this tuplet
	int dots;
};


#endif
