/*

  Copyright (C) 2007  Grame


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

    Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
    research@grame.fr

*/

#ifdef WIN32
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <string>

#include "ARNote.h"
#include "ARFactory.h"
#include "AROthers.h"
#include "ARTag.h"
#include "clonevisitor.h"
#include "counteventsvisitor.h"
#include "eheadOperation.h"
#include "event2timevisitor.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//_______________________________________________________________________________
SARMusic eheadOperation::operator() ( const SARMusic& score1, const SARMusic& score2 )
{
	if (!score2) return 0;
	counteventsvisitor cev;
	Sguidoelement elt = (*this)(score1, cev.count(score2));
	return dynamic_cast<ARMusic*>((guidoelement*)elt);
}

//_______________________________________________________________________________
Sguidoelement eheadOperation::operator() ( const Sguidoelement& score, unsigned int evIndex, unsigned int voiceIndex )
{
	Sguidoelement outscore;
	if (score) {
		event2timevisitor convert;
		rational time = convert.event2time(score, evIndex, voiceIndex);
		if (time < rational(0,1))				// event index too large
			time = fDuration.duration(score);	// use the whole score duration
		outscore = headOperation::operator()(score, time);
	}
	return outscore;
}

}
