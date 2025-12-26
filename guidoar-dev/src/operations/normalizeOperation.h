/*
  Copyright � Grame 2008

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

#ifndef __normalizeOperation__
#define __normalizeOperation__

#include <map>
#include <string>

#include "arexport.h"
#include "guidoelement.h"
#include "clonevisitor.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

/*!
\brief A visitor that produces a normalized version of a score.

	A score normal form is intended to compare scores. The normal form is based
	on pitch and duration only: it contains only voices, notes and chords, 
	where octave and duration are made explicit for every note.
	Thus, all the other tags are omitted (i.e. clef, meter slurs etc...). 
	However, we need to introduce a new specific \backward<dur> tag to catch
	the case where a single note is tied to a chord.
*/
class gar_export normalizeOperation :
	public clonevisitor
{		
    public:
 				 normalizeOperation()	{}
		virtual ~normalizeOperation()	{}

		/*! creates a normal form of a score
			\param score the score to be normalized
			\return a new score in normal form
		*/
		Sguidoelement operator() ( const Sguidoelement& score );
 
     protected:
		rational fCurrentDuration;
		int		 fCurrentOctave;
		
		virtual void visitStart( SARVoice& elt );
		virtual void visitStart( SARChord& elt );
		virtual void visitStart( SARNote& elt );

		virtual void visitEnd  ( SARVoice& elt );
		virtual void visitEnd  ( SARChord& elt );

		virtual void visitStart( Sguidotag& elt )	{}		// skip tags
		virtual void visitEnd  ( Sguidotag& elt )	{} 		// skip tags

     private:
};

/*! @} */

} // namespace MusicXML


#endif
