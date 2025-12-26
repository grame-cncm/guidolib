/*

  MusicXML Library
  Copyright � Grame 2007

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

#ifndef __unrolled_guido_browser__
#define __unrolled_guido_browser__

#include <map>

#include "ARTypes.h"
#include "browser.h"
#include "arexport.h"
#include "guidorational.h"
#include "visitor.h"

namespace guido 
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief An guido AR tree browser that "unroll" the score.

  To "unroll" the score, structural information denoted by \b repeat, 
  \b ending, \b coda and \b segno signs is interpreted and the score
  is visited similarly to a musician that would play the score ie:
  for example a section repeated twice is visited twice.
  
  The unrolled_guido_browser makes use of iterators collected along the visit 
  and makes arbitrary jumps to these iterators to modify the tree visit. 
  
  When visiting measures, forwarding the visit to the embedded visitor depends on the current state. 
  In particular, it is verified that the current time a given measure is played corresponds to the expected 
  time (if any), and that we're not currently jumping to a coda, which location could be unknown at the
  time of the jump.
*/
class gar_export unrolled_guido_browser : public browser<Sguidoelement>,
	public visitor<SARRepeatBegin>,
	public visitor<SARRepeatEnd>,
	public visitor<SARDaCapo>,
	public visitor<SARDaCapoAlFine>,
	public visitor<SARDaCoda>,
	public visitor<SARDalSegno>,
	public visitor<SARDalSegnoAlFine>,
	public visitor<SARSegno>,
	public visitor<SARCoda>,
	public visitor<SARFine>,
	public visitor<SARVolta>,
	public visitor<SARVoltaBegin>,
	public visitor<SARVoltaEnd>,
	public visitor<SARVoice>,
	public visitor<SARNote>,
	public visitor<Sguidoelement>
{
	private:
		typedef struct { rational duration; int dots; int octave; } notestate;	// current implicit notes state

		std::map<Sguidoelement,int>	fRepeatMap;			// used to avoid loops in scores
		std::map<Sguidoelement,int>	fJumpsMap;			// used to avoid loops in scores
		bool	fWriteImplicit;		///< a boolean to control implicit notes values (for jumps and repeated sections)
		notestate	fCurrentNoteState;
		
		void reset();
		bool jump(ctree<guidoelement>::literator where, Sguidoelement elt);

	protected:
		basevisitor*	fVisitor;

		ctree<guidoelement>::literator	fFirstMeasure;		///< the first measure iterator
		ctree<guidoelement>::literator	fForwardRepeat;		///< the forward repeat location
		ctree<guidoelement>::literator	fEndIterator;		///< indicates the end of to visit
		ctree<guidoelement>::literator	fNextIterator;		///< indicates the next elt to visit
		ctree<guidoelement>::literator	fSegnoIterator;		///< the segno location
		ctree<guidoelement>::literator	fCodaIterator;		///< the segno location
		ctree<guidoelement>::literator	fFineIterator;		///< the fine location
		ctree<guidoelement>::literator*	fStoreIterator;		///< a pointer to store the current iterator

		virtual void visitStart( SARRepeatBegin& elt );
		virtual void visitStart( SARRepeatEnd& elt );
		virtual void visitStart( SARDaCapo& elt );
		virtual void visitStart( SARDaCapoAlFine& elt );
		virtual void visitStart( SARDaCoda& elt );
		virtual void visitStart( SARDalSegno& elt );
		virtual void visitStart( SARDalSegnoAlFine& elt);
		virtual void visitStart( SARSegno& elt );
		virtual void visitStart( SARCoda& elt );
		virtual void visitStart( SARFine& elt );
		virtual void visitStart( SARVolta& elt )		{}
		virtual void visitStart( SARVoltaBegin& elt )	{}
		virtual void visitStart( SARVoltaEnd& elt )		{}
		virtual void visitStart( SARNote& elt );
		virtual void visitStart( SARVoice& elt );
		virtual void visitStart( Sguidoelement& elt );


	public:
				 unrolled_guido_browser(basevisitor* v);
		virtual ~unrolled_guido_browser() {}

		virtual void browse (Sguidoelement& t);
		virtual void enter (Sguidoelement& t);
		virtual void leave (Sguidoelement& t);
};

/*! @} */

} // namespace MusicXML


#endif
