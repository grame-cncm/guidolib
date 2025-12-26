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

#ifndef __countEventsVisitor__
#define __countEventsVisitor__

#include <vector>

#include "arexport.h"
#include "guidoelement.h"
#include "ARTypes.h"
#include "tree_browser.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

//_______________________________________________________________________________
/*!
\brief  a visitor to count the number of notes and chords.
		A chord counts for 1 and notes inside a chord are ignored.
*/
class gar_export counteventsvisitor :
	public visitor<SARChord>,
	public visitor<SARNote>
{
    public: 
				 counteventsvisitor() { fBrowser.set(this); }
		virtual ~counteventsvisitor() {}

		int count (const Sguidoelement&);
		int currentCount () const		{ return fCount; }

		virtual void reset()	{ fCount=0; fInChord=false; }

		virtual void visitStart( SARNote& elt );
		virtual void visitStart( SARChord& elt );
		virtual void visitEnd  ( SARChord& elt );

	protected:
		tree_browser<guidoelement> fBrowser;
		bool	fInChord;
		int		fCount;
};

//_______________________________________________________________________________
/*!
\brief  a visitor to count the number of notes and chords per voice.
*/
class gar_export countvoiceseventsvisitor :
	public counteventsvisitor,
	public visitor<SARVoice>
{
    public: 
				 countvoiceseventsvisitor() {}
		virtual ~countvoiceseventsvisitor() {}

		int count (const Sguidoelement&, unsigned int voice);

		virtual void reset()	{ fCurrentVoice = fTargetVoice = 0; fVoiceCount=-1; counteventsvisitor::reset(); }

	protected:
		virtual void visitStart ( SARVoice& elt );
		virtual void visitEnd	( SARVoice& elt );

		unsigned int	fTargetVoice;
		unsigned int	fCurrentVoice;
		int				fVoiceCount;
};

/*! @} */

} // namespace

#endif
