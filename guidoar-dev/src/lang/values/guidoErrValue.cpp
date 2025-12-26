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
#include "guidoErrValue.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

#define propagate(f,val)  create(getArg1()->f(val), getArg2()->f(val))

//______________________________________________________________________________
// guidoErrValue
//______________________________________________________________________________
Sguidovalue	guidoErrValue::create ()
	{ valuePrint("guidoErrValue"); guidoErrValue * o = new guidoErrValue(); assert(o!=0); return o; }

//______________________________________________________________________________
void guidoErrValue::print(ostream& os) 
{
	os << "guidoErrValue";
}

//______________________________________________________________________________
Sguidovalue	guidoErrValue::apply (Sguidovalue& arg)			{ return this; }
Sguidovalue	guidoErrValue::head	(unsigned int length)		{ return this; }
Sguidovalue	guidoErrValue::head	(const rational& length)	{ return this; }
Sguidovalue	guidoErrValue::tail	(unsigned int length)		{ return this; }
Sguidovalue	guidoErrValue::tail	(const rational& length)	{ return this; }

//______________________________________________________________________________
Sguidovalue	guidoErrValue::top (unsigned int vnum)		{ return this; }
Sguidovalue	guidoErrValue::bottom (unsigned int vnum)	{ return this; }
Sguidovalue	guidoErrValue::transpose(int interval)		{ return this; }
Sguidovalue	guidoErrValue::stretch (rational ratio)		{ return this; }
Sguidovalue	guidoErrValue::stretch (float ratio)		{ return this; }

//______________________________________________________________________________
unsigned int guidoErrValue::length ()	{ return 0; }
rational guidoErrValue::duration()		{ return rational(0,1); }
unsigned int guidoErrValue::voices ()	{ return 0; }
unsigned int guidoErrValue::pitch ()	{ return 60; }

//______________________________________________________________________________
void guidoErrValue::acceptIn(basevisitor& v) {
	if (visitor<SguidoErrValue>* p = dynamic_cast<visitor<SguidoErrValue>*>(&v)) {
		SguidoErrValue ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoErrValue::acceptOut(basevisitor& v) {
	if (visitor<SguidoErrValue>* p = dynamic_cast<visitor<SguidoErrValue>*>(&v)) {
		SguidoErrValue ge = this;
		p->visitEnd (ge);
	}
}

} // namespace
