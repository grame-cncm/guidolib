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
#include "guidoSeqValue.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

#define propagate(f,val)  create(getArg1()->f(val), getArg2()->f(val))

//______________________________________________________________________________
// guidoSeqValue
//______________________________________________________________________________
Sguidovalue	guidoSeqValue::create (Sguidovalue v1, Sguidovalue v2)
	{ valuePrint("guidoSeqValue"); guidoSeqValue * o = new guidoSeqValue(v1, v2); assert(o!=0); return o; }

//______________________________________________________________________________
void guidoSeqValue::print(ostream& os) 
{
	os << "guidoSeqValue";
}

//______________________________________________________________________________
Sguidovalue	guidoSeqValue::apply (Sguidovalue& arg)
{
	rational dur = getArg1()->duration();
	Sguidovalue head = arg->head(dur);
	Sguidovalue tail = arg->tail(dur);
	return create(getArg1()->apply(head), getArg2()->apply(tail));
}

//______________________________________________________________________________
Sguidovalue	guidoSeqValue::head	(unsigned int length)
{
	unsigned int l1 = getArg1()->length();
	return (length > l1) ? create(getArg1(), getArg2()->head(length - l1)) : getArg1()->head(length);
}

Sguidovalue	guidoSeqValue::head	(const rational& length)
{
	rational l1 = getArg1()->length();
	return (length > l1) ? create(getArg1(), getArg2()->head(l1)) : getArg1()->head(length);
}

//______________________________________________________________________________
Sguidovalue	guidoSeqValue::tail	(unsigned int length)
{
	unsigned int l1 = getArg1()->length();
	return (length <= l1) ? create(getArg1()->tail(length), getArg2()) : getArg2()->tail(length-l1);
}

Sguidovalue	guidoSeqValue::tail	(const rational& length)
{
	rational l1 = getArg1()->length();
	return (length > l1) ? getArg2()->tail(length - l1) : create(getArg1()->tail(length), getArg2());
}

//______________________________________________________________________________
Sguidovalue	guidoSeqValue::top (unsigned int vnum)		{ return propagate (top, vnum); }
Sguidovalue	guidoSeqValue::bottom (unsigned int vnum)	{ return propagate (bottom, vnum); }
Sguidovalue	guidoSeqValue::transpose(int interval)		{ return propagate (transpose, interval); }
Sguidovalue	guidoSeqValue::stretch (rational ratio)		{ return propagate (stretch, ratio); }
Sguidovalue	guidoSeqValue::stretch (float ratio)		{ return propagate (stretch, ratio); }

//______________________________________________________________________________
unsigned int guidoSeqValue::length ()
{
	unsigned int l1 = getArg1()->length();	if (l1 == kInfinite) return kInfinite;
	unsigned int l2 = getArg2()->length();	if (l2 == kInfinite) return kInfinite;
	return l1 + l2;
}

rational guidoSeqValue::duration()
{
	rational d1 = getArg1()->duration();	if (infinite(d1)) return d1;
	rational d2 = getArg2()->duration();	if (infinite(d2)) return d2;
	return (d1 + d2).rationalise();
}

unsigned int guidoSeqValue::voices ()
{
	unsigned int v1 = getArg1()->voices();	if (v1 == kInfinite) return kInfinite;
	unsigned int v2 = getArg2()->voices();	if (v2 == kInfinite) return kInfinite;
	return (v1 > v2) ? v1 : v2;
}

unsigned int guidoSeqValue::pitch ()
{
	return getArg1()->pitch();
}

//______________________________________________________________________________
void guidoSeqValue::acceptIn(basevisitor& v) {
	if (visitor<SguidoSeqValue>* p = dynamic_cast<visitor<SguidoSeqValue>*>(&v)) {
		SguidoSeqValue ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoSeqValue::acceptOut(basevisitor& v) {
	if (visitor<SguidoSeqValue>* p = dynamic_cast<visitor<SguidoSeqValue>*>(&v)) {
		SguidoSeqValue ge = this;
		p->visitEnd (ge);
	}
}

} // namespace
