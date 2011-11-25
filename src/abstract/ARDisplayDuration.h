#ifndef ARDisplayDuration_H
#define ARDisplayDuration_H

/*
	GUIDO Library
	Copyright (C) 2002  Holger Hoos, Juergen Kilian, Kai Renz

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

	virtual void print() const
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
