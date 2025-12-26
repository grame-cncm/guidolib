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

#ifndef __countVisitor__
#define __countVisitor__

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
template <typename C> class gar_export countvisitor : public visitor<C>
{
    public: 
				 countvisitor() { fBrowser.set(this); }
		virtual ~countvisitor() {}
    
    int count (const Sguidoelement& elt) {
        fCount = 0;
        fCurrentVoice = fTargetVoice = 0;
        if (elt) fBrowser.browse (*elt);
        return fCount;
    }
    
    int count (const Sguidoelement& elt, unsigned int voice) {
        fCount = 0;
        fCurrentVoice = 0;
        fTargetVoice = voice;
        if (elt) fBrowser.browse (*elt);
        return fCount;
    }
    
    virtual void visitStart( C& elt )   { if (fCurrentVoice == fTargetVoice) fCount++; }
    virtual void visitStart ( SARVoice& elt )   { fCurrentVoice++; }

	protected:
		tree_browser<guidoelement> fBrowser;
        unsigned int	fTargetVoice;
        unsigned int	fCurrentVoice;
		int             fCount;
};

/*! @} */

} // namespace

#endif
