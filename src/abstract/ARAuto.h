#ifndef ARAuto_H
#define ARAuto_H

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
// #include "ARMusicalObject.h"

class ARMusicalVoiceState;

/** \brief Auto tag
*/
class ARAuto : public ARMTParameter
	// ,
   // public ARMusicalObject
{
public:
	friend class ARMusicalVoiceState;

	enum state { ON,OFF };
	ARAuto();
	virtual ~ARAuto();

	state getStretchLastLineState() const
	{ return stretchLastLineState; }
	state getStretchFirstLineState() const
	{ return stretchFirstLineState; }
	state getClefKeyMeterOrderState() const
	{ return clefKeyMeterOrderState; }
	state getEndBarState() const
	{ return endBarState; }
	state getSystemBreakState() const
	{ return systemBreakState; }
	state getPageBreakState() const
	{ return pageBreakState; } 
	virtual void setTagParameterList(TagParameterList & tpl);
	virtual void PrintName(std::ostream & os) const;
	virtual void PrintParameters(std::ostream & os) const;
	/* virtual ostream & operator<<(ostream &os) const
	{
		os << "\\autoBreak";
		if (state == ON)
			os << "<\"on\">";
		else
			os << "<\"off\">";
		return os << " ";
	} */

	virtual void print() const {}
	virtual bool IsStateTag() const { return true; }

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
