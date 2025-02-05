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

#include "counteventsvisitor.h"
#include "durationvisitor.h"
#include "firstpitchvisitor.h"
#include "guidoApplyValue.h"
#include "guidoScoreValue.h"
#include "visitor.h"

#include "headOperation.h"
#include "eheadOperation.h"
#include "tailOperation.h"
#include "etailOperation.h"
#include "topOperation.h"
#include "bottomOperation.h"
#include "transposeOperation.h"
#include "durationOperation.h"

#include "ARFactory.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

#define compose(operation,val)  operation op; return create(op(fScore,val))

//______________________________________________________________________________
// guidoScoreValue
//______________________________________________________________________________
Sguidovalue	guidoScoreValue::create ()
{ 
	valuePrint("guidoScoreValue");
	guidoScoreValue * o = new guidoScoreValue(); assert(o!=0);
	o->fScore = ARFactory::instance().createMusic();
	return o; 
}

//______________________________________________________________________________
void guidoScoreValue::print(ostream& os) 
{
	os << "guidoScoreValue";
}

//______________________________________________________________________________
Sguidovalue	guidoScoreValue::create (Sguidoelement score)
	{ guidoScoreValue * o = new guidoScoreValue(score); assert(o!=0); return o; }

//______________________________________________________________________________
Sguidovalue	guidoScoreValue::apply (Sguidovalue& arg)
{
	return guidoApplyValue::create(this, arg);
}

//______________________________________________________________________________
Sguidovalue	guidoScoreValue::head	(unsigned int length)		{ compose (eheadOperation, length); }
Sguidovalue	guidoScoreValue::head	(const rational& length)	{ compose (headOperation, length); }
Sguidovalue	guidoScoreValue::tail	(unsigned int length)		{ compose (etailOperation, length); }
Sguidovalue	guidoScoreValue::tail	(const rational& length)	{ compose (tailOperation, length); }

//______________________________________________________________________________
Sguidovalue	guidoScoreValue::top (unsigned int vnum)		{ compose (topOperation, vnum); }
Sguidovalue	guidoScoreValue::bottom (unsigned int vnum)		{ compose (bottomOperation, vnum); }
Sguidovalue	guidoScoreValue::transpose(int interval)		{ compose (transposeOperation, interval); }
Sguidovalue	guidoScoreValue::stretch (rational ratio)		{ compose (durationOperation, ratio); }
Sguidovalue	guidoScoreValue::stretch (float ratio)			{ compose (durationOperation, ratio); }

//______________________________________________________________________________
unsigned int guidoScoreValue::length ()
{
	if (!fScore) return 0;

	unsigned int max=0;
	for (unsigned int i=0; i < fScore->size(); i++) {
		countvoiceseventsvisitor cvev;
		unsigned int count = cvev.count (fScore, i);
		if (count > max) max = count;
	}
	return max;
}

rational guidoScoreValue::duration()
{
	durationvisitor dv;
	return dv.duration (fScore);
}

unsigned int guidoScoreValue::voices ()
{
	return fScore ? fScore->size() : 0;
}

unsigned int guidoScoreValue::pitch ()
{
	firstpitchvisitor fpv;
	int pitch = fpv.firstPitch(fScore);
	return (pitch > 0) ? pitch : 60;
}

//______________________________________________________________________________
void guidoScoreValue::acceptIn(basevisitor& v) {
	if (visitor<SguidoScoreValue>* p = dynamic_cast<visitor<SguidoScoreValue>*>(&v)) {
		SguidoScoreValue ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoScoreValue::acceptOut(basevisitor& v) {
	if (visitor<SguidoScoreValue>* p = dynamic_cast<visitor<SguidoScoreValue>*>(&v)) {
		SguidoScoreValue ge = this;
		p->visitEnd (ge);
	}
}

} // namespace
