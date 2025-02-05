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

#include "AROthers.h"
#include "visitor.h"

using namespace std;

namespace guido 
{

//______________________________________________________________________________
//
//   ARMusic
//______________________________________________________________________________
void ARMusic::acceptIn(basevisitor& v) {
	if (visitor<SMARTP<ARMusic> >* p = dynamic_cast<visitor<SMARTP<ARMusic> >*>(&v)) {
		SMARTP<ARMusic> sptr = this;
		p->visitStart(sptr);
	}
	else guidoelement::acceptIn(v);
}

//______________________________________________________________________________
void ARMusic::acceptOut(basevisitor& v) {
	if (visitor<SMARTP<ARMusic> >* p = dynamic_cast<visitor<SMARTP<ARMusic> >*>(&v)) {
		SMARTP<ARMusic> sptr = this;
		p->visitEnd(sptr);
	}
	else guidoelement::acceptOut(v);
}

//______________________________________________________________________________
SMARTP<ARMusic> ARMusic::create()
    { ARMusic* o = new ARMusic; assert(o!=0); return o; }


//______________________________________________________________________________
//
//   ARVoice
//______________________________________________________________________________
void ARVoice::acceptIn(basevisitor& v) {
	if (visitor<SMARTP<ARVoice> >* p = dynamic_cast<visitor<SMARTP<ARVoice> >*>(&v)) {
		SMARTP<ARVoice> sptr = this;
		p->visitStart(sptr);
	}
	else guidoelement::acceptIn(v);
}

//______________________________________________________________________________
void ARVoice::acceptOut(basevisitor& v) {
	if (visitor<SMARTP<ARVoice> >* p = dynamic_cast<visitor<SMARTP<ARVoice> >*>(&v)) {
		SMARTP<ARVoice> sptr = this;
		p->visitEnd(sptr);
	}
	else guidoelement::acceptOut(v);
}

//______________________________________________________________________________
SMARTP<ARVoice> ARVoice::create()
    { ARVoice* o = new ARVoice; assert(o!=0); return o; }

} // namespace
