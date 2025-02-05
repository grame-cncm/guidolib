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

#ifndef __durationVisitor__
#define __durationVisitor__

#include <ostream>

#include "arexport.h"
#include "guidoelement.h"
#include "ARTypes.h"
#include "guidorational.h"
#include "tree_browser.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief	A visitor to print the gmn description
*/
class gar_export durationvisitor :
	public visitor<SARVoice>,
	public visitor<SARChord>,
	public visitor<SARNote>
{
    public:
				 durationvisitor() { fBrowser.set(this); }
       	virtual ~durationvisitor() {}
              
		/*!
			\brief computes the duration of a score
			\param score an input score
			\return the total duration of the input score expressed as a rational (where 1 is a whole note)
		*/
		virtual rational duration(const Sguidoelement& score);
 
		virtual void reset();
		bool  inChord() const	{ return fInChord; }

		virtual void visitStart( SARVoice& elt );
		virtual void visitStart( SARChord& elt );
		virtual void visitStart( SARNote& elt );

		virtual void visitEnd  ( SARVoice& elt );
		virtual void visitEnd  ( SARChord& elt );

		virtual rational  currentVoiceDate() const		{ return fCurrentVoiceDuration; }
		virtual rational  currentNoteDuration() const	{ return fCurrentNoteDuration; }
		virtual int		  currentDots() const			{ return fCurrentDots; }

	protected:		
		virtual void stop (bool state=true)	{ fBrowser.stop (state); }

		rational	fCurrentVoiceDuration;
		rational	fCurrentChordDuration;
		rational	fCurrentNoteDuration;
		int			fCurrentDots;
		rational	fDuration;
		bool		fInChord;

		tree_browser<guidoelement> fBrowser;
};

/*! @} */

} // namespace

#endif
