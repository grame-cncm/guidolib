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

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <iostream>

#include "clonevisitor.h"
#include "durationvisitor.h"
#include "interleaveOperation.h"
#include "counteventsvisitor.h"
#include "ARFactory.h"
#include "ARNote.h"
#include "AROthers.h"
#include "tree_browser.h"


using namespace std;

namespace guido 
{

//_______________________________________________________________________________
SARVoice interleaveOperation::interleave ( const Sguidoelement& voice1, const Sguidoelement& voice2, int offset )
{
	SARVoice elt = ARFactory::instance().createVoice();
	if (elt) {
	}
	return elt;
}

//_______________________________________________________________________________
SARMusic interleaveOperation::operator() ( const SARMusic& score1, const SARMusic& score2 )
{
	SARMusic elt = ARFactory::instance().createMusic();
	if (elt) {
		clonevisitor cv;
		SARMusic cs1 = dynamic_cast<ARMusic*>((guidoelement*)cv.clone(score1));
		SARMusic cs2 = dynamic_cast<ARMusic*>((guidoelement*)cv.clone(score2));
		
		ctree<guidoelement>::const_literator i1 = cs1->lbegin();
		ctree<guidoelement>::const_literator i2 = cs2->lbegin();
		while ((i1 != cs1->lend()) && (i2 != cs2->lend())) {
			// interleaves voice by voice with a start offset depending on the requested alignment
			int offset = 0;
			if (fMode == kRight) {
				counteventsvisitor evc;
				offset = evc.count (*i1) - evc.count (*i2);
			} 
			cout << "offset is " << offset << endl;		
			elt->push (interleave (*i1, *i2, offset));
			i1++; i2++;
		}
		while (i1 != cs1->lend())
			elt->push(*i1++);
		while (i2 != cs2->lend())
			elt->push (*i2++);
	}
	return elt;
}


} // namespace
