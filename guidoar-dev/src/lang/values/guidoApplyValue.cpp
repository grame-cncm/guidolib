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
#include "guidoApplyValue.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

#define propagate(f,val)  create(getArg1()->f(val), getArg2())

//______________________________________________________________________________
// guidoApplyValue
//______________________________________________________________________________
Sguidovalue	guidoApplyValue::create (Sguidovalue v1, Sguidovalue v2)
	{ valuePrint("guidoApplyValue"); guidoApplyValue * o = new guidoApplyValue(v1, v2); assert(o!=0); return o; }

//______________________________________________________________________________
void guidoApplyValue::print(ostream& os) 
{
	os << "guidoApplyValue: " << getArg1() << "@" << getArg2() ;
}

//______________________________________________________________________________
Sguidovalue	guidoApplyValue::apply	(Sguidovalue& arg)			{ return create(this, arg); }
Sguidovalue	guidoApplyValue::head	(unsigned int length)		{ return propagate(head, length); }
Sguidovalue	guidoApplyValue::head	(const rational& length)	{ return propagate(head, length); }
Sguidovalue	guidoApplyValue::tail	(unsigned int length)		{ return propagate(tail, length); }
Sguidovalue	guidoApplyValue::tail	(const rational& length)	{ return propagate(tail, length); }
Sguidovalue	guidoApplyValue::top (unsigned int vnum)			{ return propagate (top, vnum); }
Sguidovalue	guidoApplyValue::bottom (unsigned int vnum)			{ return propagate (bottom, vnum); }
Sguidovalue	guidoApplyValue::transpose(int interval)			{ return propagate (transpose, interval); }
Sguidovalue	guidoApplyValue::stretch (rational ratio)			{ return propagate (stretch, ratio); }
Sguidovalue	guidoApplyValue::stretch (float ratio)				{ return propagate (stretch, ratio); }

//______________________________________________________________________________
unsigned int guidoApplyValue::length ()			{ return getArg1()->length(); }
rational guidoApplyValue::duration()			{ return getArg1()->duration(); }
unsigned int guidoApplyValue::voices ()			{ return getArg1()->voices(); }
unsigned int guidoApplyValue::pitch ()			{ return getArg1()->pitch(); }

//______________________________________________________________________________
void guidoApplyValue::acceptIn(basevisitor& v) {
	if (visitor<SguidoApplyValue>* p = dynamic_cast<visitor<SguidoApplyValue>*>(&v)) {
		SguidoApplyValue ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoApplyValue::acceptOut(basevisitor& v) {
	if (visitor<SguidoApplyValue>* p = dynamic_cast<visitor<SguidoApplyValue>*>(&v)) {
		SguidoApplyValue ge = this;
		p->visitEnd (ge);
	}
}

} // namespace
