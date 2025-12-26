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

#ifndef __topOperation__
#define __topOperation__

#include "arexport.h"
#include "ARTypes.h"
#include "clonevisitor.h"
#include "operation.h"
#include "tree_browser.h"

namespace guido 
{

/*!
\addtogroup operations
@{
*/

/*!
\brief A visitor that cuts the tail of a score voices.
*/
class gar_export topOperation :
	public operation,
	public clonevisitor
{		
    public:
 				 topOperation();
		virtual ~topOperation()	{}

		/*! cuts the tail of the score voices after a given voice
			\param score the score to be cut
			\param voicenum the score voices to preserve
			\return a new score
		*/
		Sguidoelement operator() ( const Sguidoelement& score, int voicenum );
 
		/*! cuts the tail of the score voices after a given voice
			\param score1 the score to be cut
			\param score2 a score which voice number is used as voice index
			\return a new score
		*/
		SARMusic operator() ( const SARMusic& score1, const SARMusic& score2 );

 		bool 	done () const	{ return !copy(); }

    protected:
		int			fVoiceNum, fCurrentVoice;
		tree_browser<guidoelement> fBrowser;

		virtual bool copy  () const;

		virtual void visitStart( SARVoice& elt );
		virtual void visitEnd  ( SARVoice& elt );
};

/*! @} */

} // namespace MusicXML


#endif
