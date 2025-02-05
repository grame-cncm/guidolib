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
#include "exceptions.h"
#include "guidoClosureValue.h"
#include "guidoScoreValue.h"
#include "visitor.h"

using namespace std;
using namespace guido;

namespace guidolang 
{

//______________________________________________________________________________
// guidoClosureValue
//______________________________________________________________________________
Sguidovalue	guidoClosureValue::create (Sguidoexpression& id, Sguidoexpression& body, SguidoEnv& env, 
					unsigned int length, const rational& dur, unsigned int voices)
	{ valuePrint("guidoClosureValue"); guidoClosureValue * o = new guidoClosureValue(id, body, env, length, dur, voices); assert(o!=0); return o; }

//______________________________________________________________________________
void guidoClosureValue::print(ostream& os) 
{
	os << "guidoClosureValue";
}

//______________________________________________________________________________
Sguidovalue	guidoClosureValue::apply (Sguidovalue& v)
{ 
	if (!fIdent)	throw(newException (kNullIdent)); 
	if (!fBody)		throw(newException (kNullBody));
	if (!fEnv)		throw(newException (kNullEnvironment));
	return fBody->eval(fEnv->bind (fIdent, v));
}

//______________________________________________________________________________
Sguidovalue	guidoClosureValue::head	(unsigned int length)
{ 
	return create(fIdent, fBody, fEnv, (length < fLength) ? length : fLength, fDuration, fVoices); 
}

Sguidovalue	guidoClosureValue::head	(const rational& dur)
{ 
	return create(fIdent, fBody, fEnv, fLength, (dur < fDuration) ? dur : fDuration, fVoices); 
}

Sguidovalue	guidoClosureValue::tail	(unsigned int length)
{ 
	if (length >= fLength) return guidoScoreValue::create();
	return create(fIdent, fBody, fEnv, fLength - length, fDuration, fVoices); 
}

Sguidovalue	guidoClosureValue::tail	(const rational& dur)
{ 
	if (dur >= fDuration) return guidoScoreValue::create();
	return create(fIdent, fBody, fEnv, fLength, fDuration - dur, fVoices); 
}

Sguidovalue	guidoClosureValue::top (unsigned int vnum)
{ 
	return create(fIdent, fBody, fEnv, fLength, fDuration, (vnum < fVoices) ? vnum : fVoices); 
}

Sguidovalue	guidoClosureValue::bottom (unsigned int vnum)
{ 
	if (vnum > fVoices) return guidoScoreValue::create();
	return create(fIdent, fBody, fEnv, fLength, fDuration, vnum); 
}


Sguidovalue	guidoClosureValue::transpose(int interval)
{

	return create(fIdent, fBody, fEnv, fLength, fDuration, fVoices);
}

Sguidovalue	guidoClosureValue::stretch (rational ratio)
{ 
	rational dur = ratio * fDuration;
	return create(fIdent, fBody, fEnv, fLength, dur.rationalise(), fVoices); 
}

Sguidovalue	guidoClosureValue::stretch (float ratio)
{ 
	rational dur = fDuration;
	if (ratio >= 0) dur.setNumerator(dur.getNumerator() * ratio);
	else dur.setDenominator(dur.getDenominator() * ratio);
	return create(fIdent, fBody, fEnv, fLength, dur.rationalise(), fVoices); 
}


//______________________________________________________________________________
unsigned int guidoClosureValue::length ()		{ return fLength; }
rational guidoClosureValue::duration()			{ return fDuration; }
unsigned int guidoClosureValue::voices ()		{ return fVoices; }

// todo: what's the meaning of pitch for a closure ?
unsigned int guidoClosureValue::pitch ()		{ 
	cerr << "warning: guidoClosureValue::pitch query !" << endl;
	return 60; 
}

//______________________________________________________________________________
void guidoClosureValue::acceptIn(basevisitor& v) {
	if (visitor<SguidoClosureValue>* p = dynamic_cast<visitor<SguidoClosureValue>*>(&v)) {
		SguidoClosureValue ge = this;
		p->visitStart (ge);
	}
}

//______________________________________________________________________________
void guidoClosureValue::acceptOut(basevisitor& v) {
	if (visitor<SguidoClosureValue>* p = dynamic_cast<visitor<SguidoClosureValue>*>(&v)) {
		SguidoClosureValue ge = this;
		p->visitEnd (ge);
	}
}

} // namespace
