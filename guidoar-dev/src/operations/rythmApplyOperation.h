/*
  Copyright © Grame 2008

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

#ifndef __rythmApplyOperation__
#define __rythmApplyOperation__

#include <vector>
#include <iostream>

#include "ARNote.h"
#include "AROthers.h"
#include "arexport.h"
#include "clonevisitor.h"
#include "operation.h"
#include "guidorational.h"
#include "rythmvisitor.h"
#include "tree_browser.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

/*!
\brief the base visitor for rythm application

	Intended to minimize code size.
	Can only be instantiated by derived class.
*/
class gar_export rythmApplyBaseOperation :
	public operation,
	public clonevisitor
{		
    protected:
 				 rythmApplyBaseOperation();
		virtual ~rythmApplyBaseOperation()	{}

		bool fInChord;
		tree_browser<guidoelement>	fBrowser;
		rational	fCurrentDuration;		// used to code implicit duration
		int			fCurrentDots;			// used to code implicit duration
		rational	fLastDuration;			// used to restore correct duration at iterator end pos
		
		Sguidoelement browse ( const Sguidoelement& score);
		virtual void visitStart( SARVoice& elt );
		virtual void visitStart( SARChord& elt );
		virtual void visitEnd  ( SARChord& elt );
};

/*!
\brief A visitor that applies a rythm to a score.
*/
template <typename T> class gar_export rythmApplyOperation : public rythmApplyBaseOperation
{
    public:
		typedef typename T::iterator durIterator;
		
 				 rythmApplyOperation() {}
		virtual ~rythmApplyOperation()	{}

		/*! applies a list of durations to a score
			\param score the taret score
			\param start an iterator on rythmic values
			\param end an iterator on the end of rythmic values
			\return a new score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, durIterator start, durIterator end ) {
			fRPos = start;
			fEndPos = end;
			return browse(score);
		}

		/*! applies a list of durations to a score
			\param score1 the taret score
			\param score2 a score which rythm is applied to the target score 
			\return a new score
			\note rythm is extracted from the first voice only.
		*/
		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 )
		{
			rythmvisitor rv;
			T list;
			rv.rythm(score2, 0, &list);
			Sguidoelement elt = (*this)(score1, list.begin(), list.end());
			return dynamic_cast<ARMusic*>((guidoelement*)elt);
		}

    protected:
		durIterator fRPos, fEndPos;
		
		virtual void visitStart( SARNote& elt ) {
			SARNote note = copy(elt);
			if (note) {
				if (!note->implicitDuration()) fLastDuration = note->duration();
				if (fRPos != fEndPos) {
					rational d  = *fRPos;
					if (d == fCurrentDuration) note->setImplicitDuration();
					else *note = fCurrentDuration = d;
					if (!fInChord) fRPos++;
				}
				else if (fLastDuration.getNumerator()) {
					*note = fLastDuration;
					fLastDuration.setNumerator(0);
				}
				push( note );
			}
		}

		virtual void visitEnd  ( SARChord& elt ) { 
			rythmApplyBaseOperation::visitEnd(elt);
			if (fRPos != fEndPos) fRPos++;
		}
};

/*! @} */

} // namespace MusicXML


#endif
