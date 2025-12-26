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

#ifndef __firstpitchvisitor__
#define __firstpitchvisitor__

#include "arexport.h"
#include "ARTypes.h"
#include "visitor.h"
#include "tree_browser.h"


namespace guido 
{

/*!
\addtogroup visitors
@{
*/


//______________________________________________________________________________
/*!
\brief	A visitor to retrieve the pitch of the first note (first voice, lowest note) 
*/
class gar_export firstpitchvisitor :
	public tree_browser<guidoelement>,
	public visitor<SARChord>,
	public visitor<SARNote>
{
    public:
				 firstpitchvisitor() { set(this); }
       	virtual ~firstpitchvisitor() {}

		int		firstPitch (const Sguidoelement& score);
		
	protected:              
		bool 	done () const	{ return fDone; }

		virtual void visitStart( SARChord& elt );
		virtual void visitStart( SARNote& elt );
		virtual void visitEnd  ( SARChord& elt );
		int		fPitch, fCurrentOctave;
		bool	fInChord, fDone;
};


/*! @} */

} // namespace MusicXML


#endif
