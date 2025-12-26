/*
  GUIDO Library
  Copyright (C) 2006  Grame

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

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
  research@grame.fr

*/

#include <iostream>
#include "guidoMixValue.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

#define propagate(f,val)  create(getArg1()->f(val), getArg2()->f(val))

//______________________________________________________________________________
// guidoMixValue
//______________________________________________________________________________
Sguidovalue	guidoMixValue::create (Sguidovalue v1, Sguidovalue v2)
	{ valuePrint("guidoMixValue"); guidoMixValue * o = new guidoMixValue(v1, v2); assert(o!=0); return o; }

//______________________________________________________________________________
void guidoMixValue::print(ostream& os) 
{
	os << "guidoMixValue";
}

//______________________________________________________________________________
Sguidovalue	guidoMixValue::apply (Sguidovalue& arg)	
{ 
	int voices = getArg1()->voices();
	Sguidovalue top = arg->top(voices);
	Sguidovalue bottom = arg->bottom(voices);
	return create(getArg1()->apply(top), getArg2()->apply(bottom));
}

//______________________________________________________________________________
Sguidovalue	guidoMixValue::head	(unsigned int length)		{ return propagate (head, length); }
Sguidovalue	guidoMixValue::head	(const rational& length)	{ return propagate (head, length); }
Sguidovalue	guidoMixValue::tail	(unsigned int length)		{ return propagate (tail, length); }
Sguidovalue	guidoMixValue::tail	(const rational& length)	{ return propagate (tail, length); }
Sguidovalue	guidoMixValue::transpose(int interval)			{ return propagate (transpose, interval); }
Sguidovalue	guidoMixValue::stretch (rational ratio)			{ return propagate (stretch, ratio); }
Sguidovalue	guidoMixValue::stretch (float ratio)			{ return propagate (stretch, ratio); }

//______________________________________________________________________________
Sguidovalue	guidoMixValue::top (unsigned int vnum)
{
	unsigned int vcount = getArg1()->voices();
	return (vnum > vcount) ? create(getArg1(), getArg2()->top(vnum - vcount)) : getArg1()->top(vnum);
}

Sguidovalue	guidoMixValue::bottom (unsigned int vnum)
{
	unsigned int vcount = getArg1()->voices();
	return (vnum < vcount) ? create(getArg1()->bottom(vnum), getArg2()) : getArg2()->bottom(vnum - vcount);
}

//______________________________________________________________________________
unsigned int guidoMixValue::length ()
{
	unsigned int l1 = getArg1()->length();	if (l1 == kInfinite) return kInfinite;
	unsigned int l2 = getArg2()->length();	if (l2 == kInfinite) return kInfinite;
	return (l1 > l2) ? l1 : l2;
}

rational guidoMixValue::duration()
{
	rational d1 = getArg1()->duration();	if (infinite(d1)) return d1;
	rational d2 = getArg2()->duration();	if (infinite(d2)) return d2;
	return (d1 > d2) ? d1 : d2;
}

unsigned int guidoMixValue::voices ()
{
	unsigned int v1 = getArg1()->voices();	if (v1 == kInfinite) return kInfinite;
	unsigned int v2 = getArg2()->voices();	if (v2 == kInfinite) return kInfinite;
	return v1 + v2;
}

unsigned int guidoMixValue::pitch ()
{
	return getArg1()->pitch();
}

//______________________________________________________________________________
void guidoMixValue::acceptIn(basevisitor& v) {
	if (visitor<SguidoMixValue>* p = dynamic_cast<visitor<SguidoMixValue>*>(&v)) {
		SguidoMixValue ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoMixValue::acceptOut(basevisitor& v) {
	if (visitor<SguidoMixValue>* p = dynamic_cast<visitor<SguidoMixValue>*>(&v)) {
		SguidoMixValue ge = this;
		p->visitEnd (ge);
	}
}

} // namespace
