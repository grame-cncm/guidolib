#ifndef ARIntens_H
#define ARIntens_H

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

/** \brief The intensity tag.

	p, pp, ppp, f, ff, ffff, mf, mp, sf
*/

class ARIntens :  public ARMTParameter 
{
	public:

			ARIntens(const char * txt, float val = -1.0f);

			ARIntens() : ARMTParameter(), value(-1.0f)
			 {
			 	relativeTimePosition.set( -1, 1 );
			 	duration = DURATION_0;
			 }
		virtual void setTagParameterList(TagParameterList & tpl);

		virtual const NVstring & getText() const { return mIntensText; }
		virtual float getValue() const { return value; }

		virtual void print() const;
		// virtual ostream & operator<<(ostream &os) const;
		virtual void PrintName(std::ostream &os) const;
		virtual void PrintParameters(std::ostream &os) const;

		virtual void	browse(TimeUnwrap& mapper) const;


	protected:
		NVstring mIntensText; // der entsprechende Text ...
		float value;

		static ListOfTPLs ltpls;
};

#endif
