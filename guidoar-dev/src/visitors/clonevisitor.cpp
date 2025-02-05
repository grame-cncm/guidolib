/*

  MusicXML Library
  Copyright (C) 2006,2007  Grame

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
#pragma warning (disable : 4786)
#endif

#include <iostream>

#include "ARFactory.h"
#include "ARChord.h"
#include "ARNote.h"
#include "AROthers.h"
#include "ARTag.h"
#include "clonevisitor.h"
#include "guidoelement.h"
#include "tree_browser.h"

using namespace std;

namespace guido 
{

//______________________________________________________________________________
Sguidoelement clonevisitor::clone(const Sguidoelement& elt)
{ 
	if (elt) {
		tree_browser<guidoelement> tb(this);
		tb.browse (*elt);
		Sguidoelement copy = fStack.top();
		fStack.pop();
		return copy;
	}
	return elt;
}

//______________________________________________________________________________
void clonevisitor::copyAttributes (const Sguidoelement& src, Sguidoelement& dst ) const
{
	Sguidoattributes attr = src->attributes();
	Sguidoattributes::const_iterator iter;
	for (iter=attr.begin(); iter != attr.end(); iter++) {
		Sguidoattribute ac = guidoattribute::create();
		ac->setName ( (*iter)->getName());
		ac->setValue( (*iter)->getValue(), (*iter)->quoteVal());
		ac->setUnit ( (*iter)->getUnit());
		dst->add( ac );
	}
}

//______________________________________________________________________________
Sguidoelement clonevisitor::copy (const Sguidoelement& src, Sguidoelement& dst) const
{
	if (dst) {
		dst->setName( src->getName());
		dst->setAuto( src->getAuto());
		copyAttributes (src, dst);
	}
	return dst;
}

//______________________________________________________________________________
SARNote clonevisitor::copy( const SARNote& elt ) const
{
	SARNote note = ARFactory::instance().createNote(elt->getName());
	if (note) {
		note->SetOctave (elt->GetOctave());
		note->SetAccidental (elt->GetAccidental());
		note->SetDots (elt->GetDots());
		*note = elt->duration();
		const Sguidoelement src = elt;
		Sguidoelement dst = note;
		copy (src, dst);
	}
	return note;
}

//______________________________________________________________________________
void clonevisitor::push( const SARNote& note, bool stack )
{
	Sguidoelement elt = note;
	push (elt, stack);
}

//______________________________________________________________________________
void clonevisitor::push( const Sguidoelement& elt, bool stack )
{
	if (fStack.empty())
		fStack.push(elt);
	else fStack.top()->push(elt);
	if (stack) fStack.push (elt);	
}

//______________________________________________________________________________
// the visit methods
//______________________________________________________________________________
void clonevisitor::visitStart( SARMusic& elt )
{
	fStack.push (ARFactory::instance().createMusic());
}

//______________________________________________________________________________
void clonevisitor::visitStart( SARVoice& elt )
{
	if (copy()) {
		Sguidoelement cc = ARFactory::instance().createVoice();
		push( copy (elt, cc) );
	}
}

//______________________________________________________________________________
void clonevisitor::visitStart( SARChord& elt )
{
	if (copy()) {
		Sguidoelement cc = ARFactory::instance().createChord();
		push( copy (elt, cc) );
	}
}

//______________________________________________________________________________
void clonevisitor::visitStart( SARNote& elt )
{
	if (copy()) {
		push( copy(elt), false );
	}
}

//______________________________________________________________________________
void clonevisitor::visitStart( Sguidotag& elt )
{
	if (copy()) {
		Sguidoelement cc = ARFactory::instance().createTag(elt->getName(), elt->getID() );
		push( copy (elt, cc), elt->size() ? true : false );
	}
}

//______________________________________________________________________________
void clonevisitor::visitEnd  ( SARVoice& elt )		{ if (copy()) fStack.pop(); }
void clonevisitor::visitEnd  ( SARChord& elt )		{ if (copy()) fStack.pop(); }
void clonevisitor::visitEnd  ( Sguidotag& elt )		{ if (copy() && elt->size()) fStack.pop(); }


}
