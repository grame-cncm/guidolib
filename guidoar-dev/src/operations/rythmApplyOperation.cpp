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

#include "rythmApplyOperation.h"

using namespace std;

namespace guido 
{

//_______________________________________________________________________________
 rythmApplyBaseOperation::rythmApplyBaseOperation() 
 { 
	fBrowser.set(this); 
	fLastDuration = fCurrentDuration = ARNote::getDefaultDuration(); 
	fCurrentDots = 0;
 }

//_______________________________________________________________________________
Sguidoelement rythmApplyBaseOperation::browse ( const Sguidoelement& score) {
	fInChord = false;
	Sguidoelement outscore;
	if (score) {
		fBrowser.browse (*score);
		outscore = fStack.top();
		fStack.pop();
	}
	return outscore;
}

//_______________________________________________________________________________
void rythmApplyBaseOperation::visitStart( SARVoice& elt ) 
{
	fInChord = false;
	fLastDuration = fCurrentDuration = ARNote::getDefaultDuration();
	fCurrentDots = 0;
	clonevisitor::visitStart (elt);
}

//_______________________________________________________________________________
void rythmApplyBaseOperation::visitStart( SARChord& elt ) 
{ 
	fInChord = true; 
	clonevisitor::visitStart (elt); 
}

//_______________________________________________________________________________
void rythmApplyBaseOperation::visitEnd  ( SARChord& elt ) 
{ 
	fInChord = false; 
	clonevisitor::visitEnd (elt); 
}

}
